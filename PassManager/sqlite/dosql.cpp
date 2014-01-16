#ifndef SQLITE_HAS_CODEC

#define SQLITE_HAS_CODEC

#endif

#include "stdafx.h"
#include "sqlite3.h"
#include "dosql.h"
#include "../resource.h"
#include <shlwapi.h>
#include <string.h>
#include <process.h>
#include <stdio.h>

#pragma comment(lib, "sqlite3")
#pragma comment(lib, "shlwapi.lib")

int countRow;
char szSrcPath[MAX_PATH] = {0};
char szDstPath[MAX_PATH] = {0};

doSql::doSql()
{
	m_errNum = 0;
	m_db = NULL;
	m_errMsg = NULL;
}

bool doSql::InitOpen(const char * pName, const char * key, int len)
{
	BOOL fileexists = PathFileExists(pName);
	if (!fileexists)
	{
		m_errNum = 106;
		return false;
	}
	char szPath[MAX_PATH];
	MbcsToUtf8(pName, szPath);
	int rc = sqlite3_open(szPath, &m_db);
	sqlite3_key(m_db, key, len);
	if (SQLITE_OK != rc)
	{
		m_errNum = 101;
		return false;
	}
	m_passInsertId = getMaxId("pass");
	if (m_passInsertId == -1)
	{
		return false;
	}
	m_passInsertId += 1;
	m_tagInsertId = getMaxId("tag") + 1;
	return true;
}

bool doSql::InitCreate(const char * pName, const char * key, int len)
{
	BOOL fileexists = PathFileExists(pName);
	if (fileexists)
	{
		DeleteFile(pName);
	}
	char szPath[MAX_PATH];
	MbcsToUtf8(pName, szPath);
	int rc = sqlite3_open(szPath, &m_db);
	sqlite3_key(m_db, key, len);
	if (SQLITE_OK != rc)
	{
		m_errNum = 101;
		return false;
	}
	if(!createPassBook()){
		m_errNum = 102;
		return false;
	}
	m_passInsertId = 1;
	m_tagInsertId = 1;
	return true;
}

int doSql::MbcsToUtf8(const char *file, char * outfile)
{
	WCHAR   *pwchar=0;
	CHAR    *pchar=0;	
	int len=0;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	len = MultiByteToWideChar(codepage, 0, file, -1, NULL,0);
	pwchar=new WCHAR[len];
	if(pwchar!=0)
	{
		len = MultiByteToWideChar(codepage, 0, file, -1, pwchar, len);
		if( len!=0 ) 
		{
			len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, 0, 0, 0, 0);
			pchar=new CHAR[len];
			if(pchar!=0)
			{
				len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, pchar, len,0, 0);
				if(len!=0)                 
				{
					strcpy_s(outfile, len, pchar);
				}
				delete pchar;
			}
			delete pwchar; 
		}
	}
	return len;
}

int doSql::getMaxId(char * tbname)
{
	sqlite3_stmt * stmt;
	char idname[16] = {'\0'};
	idname[0] = tbname[0];
	strcat(idname, "id");
	int max;
	char sql[256];
	char * err;
	sprintf(sql, "SELECT MAX(%s) FROM '%s';", idname, tbname);
	int rc = sqlite3_exec(m_db, sql, cb_GetMaxId, &max,&err);
	if (rc != SQLITE_OK)
	{
		m_errNum = 107;
		m_errMsg = err;
		return -1;
	}else{
		return max;
	}
}

int cb_GetMaxId(void * para,int nCount,char** pValue,char** pName)
{
	int *max = (int *)para;
	if (pValue[0] == NULL)
	{
		*max = 1;
	}else{
		*max = atoi(pValue[0]);
	}	
	return 0;
}

//创建一个新密码本
bool doSql::createPassBook()
{
	int rc;
	rc = sqlite3_exec(m_db, "PRAGMA foreign_keys = ON;", 0, 0, &m_errMsg); //开启外键支持
	if (SQLITE_OK != rc)
	{
		return false;
	}
	char sql[512] = "CREATE TABLE IF NOT EXISTS pass( \
				pid INTEGER UNSIGNED NOT NULL, \
				user VARCHAR(32) NOT NULL, \
				password VARCHAR(128) NOT NULL, \
				url VARCHAR(256), \
				remarks VARCHAR(256), \
				PRIMARY KEY(pid) \
		);";
	rc = sqlite3_exec(m_db,sql,NULL,NULL,&m_errMsg);
	if (SQLITE_OK != rc)
	{
		return false;
	}
	strcpy(sql, "CREATE TABLE IF NOT EXISTS tag(  \
		tid INTEGER UNSIGNED NOT NULL, \
		name VARCHAR(32) NOT NULL UNIQUE, \
		PRIMARY KEY(tid));");
	rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
	if (SQLITE_OK != rc)
	{
		return false;
	}
	strcpy(sql, "CREATE TABLE IF NOT EXISTS pt(  \
				pid INTEGER UNSIGNED NOT NULL, \
				tid INTEGER UNSIGNED NOT NULL, \
				FOREIGN KEY(pid) REFERENCES pass(pid), \
				FOREIGN KEY(tid) REFERENCES tag(tid), \
				PRIMARY KEY(pid, tid)); ");
	rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
	if (SQLITE_OK != rc)
	{
		return false;
	}
	return true;
}

//增加一个密码
bool doSql::addPass(char * pass, char * user, char * url, char * tags[], int num, char * remark)
{
	char sql[1024];
	int rc;
	sqlite3_exec(m_db, "begin;", NULL, NULL, NULL);

	sprintf_s(sql, 1024, "INSERT INTO pass(pid, user, password, url, remarks) VALUES(%d, '%s', '%s', '%s', '%s');", 
		m_passInsertId, user, pass, url, remark);
	rc = sqlite3_exec(m_db, sql, NULL ,NULL, &m_errMsg);
	if (rc != SQLITE_OK)
	{
		sqlite3_exec(m_db, "rollback;", NULL, NULL, NULL);
		m_errNum = 103;
		return false;
	}

	for (int i = 0 ; i < num ; i++)
	{
		int exitid = 0;
		sprintf_s(sql, 1024, "SELECT tid FROM tag WHERE name='%s';", tags[i]);
		rc = sqlite3_exec(m_db, sql, cb_Exists, &exitid, &m_errMsg);
		if (exitid)
		{
			sprintf(sql, "INSERT INTO pt(pid, tid) VALUES(%d, %d);", m_passInsertId, exitid);
			rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
			if (rc != SQLITE_OK)
			{
				sqlite3_exec(m_db, "rollback;", NULL, NULL, NULL);
				m_errNum = 105;
				return false;
			}
		} 
		else
		{
			sprintf_s(sql, 1024, "INSERT INTO tag(tid, name) VALUES(%d, '%s');", m_tagInsertId, tags[i]);
			rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
			if (rc != SQLITE_OK)
			{
				sqlite3_exec(m_db, "rollback;", NULL, NULL, NULL);
				m_errNum = 104;
				return false;
			}
			sprintf_s(sql, 1024, "INSERT INTO pt(pid, tid) VALUES(%d, %d);", m_passInsertId, m_tagInsertId++);
			rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
			if (rc != SQLITE_OK)
			{
				sqlite3_exec(m_db, "rollback;", NULL, NULL, NULL);
				m_errNum = 105;
				return false;
			}
		}		
	}
	m_passInsertId++;
	rc = sqlite3_exec(m_db, "commit;", NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		return false;
	}
	
	return true;
}

//修改一个密码
bool doSql::editPass(int pid, char * pass, char * user, char * url, char * tags[], int num, char * remark)
{
	char sql[1024];
	sprintf_s(sql, 1024, "UPDATE pass SET user = '%s', password = '%s', url = '%s', remarks = '%s' WHERE pid = %d;",
		user, pass, url, remark, pid);
	int rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
	if (SQLITE_OK != rc)
	{
		m_errNum = 109;
		return false;
	}
	for (int i = 0 ; i < num ; i++)
	{
		sprintf_s(sql, 1024, "INSERT INTO tag(tid, name) VALUES(%d, '%s')", m_tagInsertId, tags[i]);
		rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
		int tid;
		if (rc == SQLITE_OK)
		{
			tid = m_tagInsertId;
		}
		else
		{
			sprintf_s(sql, 1024, "SELECT tid FROM tag WHERE name='%s';", tags[i]);
			int rc = sqlite3_exec(m_db, sql, cb_getTid, &tid, &m_errMsg);
		}
		sprintf_s(sql, 1024, "INSERT INTO pt(pid, tid) VALUES(%d, %d);", pid, tid);
		int re = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
		if (re == SQLITE_OK)
		{
			m_tagInsertId++;
		}
	}
	return true;
}

//删除一个密码
bool doSql::delPass(int pid)
{
	char sql[64];
	sprintf_s(sql, 64, "DELETE FROM pt WHERE pid = %d;", pid);
	int rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
	if (SQLITE_OK != rc)
	{
		m_errNum = 110;
		return false;
	}
	sprintf_s(sql, 64, "DELETE FROM pass WHERE pid = %d;", pid);
	rc = sqlite3_exec(m_db, sql, NULL, NULL, &m_errMsg);
	if (SQLITE_OK != rc)
	{
		m_errNum = 110;
		return false;
	}
	return true;
}

//删除多余的标签
int doSql::delMoreTag()
{
	char sql[256] = "SELECT tid FROM tag;";
	int rc = sqlite3_exec(m_db, sql, cb_getAllTid, m_db, &m_errMsg);
	return !rc;
}

int cb_getAllTid(void * para,int nCount,char** pValue,char** pName)
{
	sqlite3 * db = (sqlite3 *)para;
	char sql[64];
	sprintf_s(sql, 64, "SELECT * FROM pt WHERE tid = %d;", pValue[0]);
	bool isExists = false;
	int rc = sqlite3_exec(db, sql, cb_TidFew, &isExists, NULL);
	if (rc == SQLITE_OK && !isExists)
	{
		sprintf_s(sql, 64, "DELETE FROM tag WHERE tid = %d;", pValue[0]);
		sqlite3_exec(db, sql, NULL, NULL, NULL);
	}
	return 0;
}

int cb_TidFew(void * para,int nCount,char** pValue,char** pName)
{
	bool * isExists = (bool *)para;
	*isExists = true;
	return 0;
}

//搜索数据的主要方法，第一个参数是搜索关键字，第二个参数是返回项目。返回值是返回项目的个数。
int doSql::SearchPass(char * szKeyWord, InfoRow ** info)
{
	char sql[256];
	int tid = 0;
	int pids[50] = {0};
	sprintf_s(sql, 256, "SELECT tid FROM tag WHERE name='%s';", szKeyWord);
	int rc = sqlite3_exec(m_db, sql, cb_getTid, &tid, &m_errMsg);
	if (rc != SQLITE_OK)
	{
		m_errNum = 108;
		return -1;
	}
	if (!tid)
	{
		return 0;
	}
	countRow = 1;
	sprintf_s(sql, 256, "SELECT pid FROM pt WHERE tid=%d", tid);
	rc = sqlite3_exec(m_db, sql, cb_getPid, pids, &m_errMsg);
	if (rc != SQLITE_OK || !pids[0])
	{
		m_errNum = 108;
		return -1;
	}
	*info = new InfoRow[pids[0]];
	for (int i = 1 ; i <= pids[0] ; i++)
	{
		sprintf_s(sql, 256, "SELECT user, password, url, remarks,pid FROM pass WHERE pid=%d;", pids[i], szKeyWord);
		rc = sqlite3_exec(m_db, sql, cb_getPass, (*info) + (i - 1), &m_errMsg);
	}
	return pids[0];
}

int cb_getPid(void * para,int nCount,char** pValue,char** pName)
{
	if (countRow >= 50)
	{
		return 0;
	}
	int * pArr = (int *)para;
	pArr[countRow] = atoi(pValue[0]);
	pArr[0] = countRow++;
	return 0;
}

int doSql::getInfoById(int pid, InfoRow * info)
{
	char sql[128];
	sprintf_s(sql, 128, "SELECT user, password, url, remarks, pid FROM pass WHERE pid=%d;", pid);
	int rc = sqlite3_exec(m_db, sql, cb_getPass, info, &m_errMsg);
	if (rc != SQLITE_OK)
	{
		return 0;
	}else
	{
		return 1;
	}
}

int doSql::getTagByPid(int pid, char szTag[])
{
	char sql[256];
	int Tids[50];
	countRow = 1;
	sprintf_s(sql, 256, "SELECT tid FROM pt WHERE pid = %d;", pid);
	sqlite3_exec(m_db, sql, cb_getPid, Tids, &m_errMsg);
	for (int i = 1 ; i <= Tids[0] ; i++)
	{
		sprintf_s(sql, 256, "SELECT name FROM tag WHERE tid=%d;", Tids[i]);
		sqlite3_exec(m_db, sql, cb_getTagName, szTag, &m_errMsg);
	}
	return 0;
}

int cb_getTagName(void * para,int nCount,char** pValue,char** pName)
{
	char * szTag = (char *)para;
	strcat(szTag, "|");
	strcat(szTag, pValue[0]);
	return 0;
}

//备份数据库
//注意参数szPass请申请动态空间
bool doSql::bakDatabase(char szPath[], char szPass[], int OutType, bool bPass)
{
	switch (OutType)
	{
	case TYPE_PAS:
		return bakToPas(szPath, bPass, szPass);
	case TYPE_SQL:
		return bakToSql(szPath, szPass);
		break;
	case TYPE_CSV:
		return bakToCsv(szPath, szPass);
		break;
	default:

		break;
	}
	return true;
}

//bPass == true 带密码备份
//bPass == false 不带密码备份
bool doSql::bakToPas(char szPath[], bool bPass, char *szPass)
{
	if (!strcmp("", szPath))
	{
		return false;
	}
	const char * szDbFile = sqlite3_db_filename(m_db, "main");
	strcpy_s(szSrcPath, MAX_PATH, szDbFile);
	strcpy_s(szDstPath, MAX_PATH, szPath);
	if (bPass)
	{
		_beginthread(CopyFileThrd, 0, NULL);
	} 
	else
	{
		_beginthread(BakNoPassBook, 0, szPass);
	}
	return true;
}

//修改密码
bool doSql::reKey(char szKey[], int len)
{
	int rc = sqlite3_rekey(m_db, szKey, len);
	return !rc;
}

//获得错误代码
int doSql::getErrNo()
{
	return this->m_errNum;
}


doSql::~doSql(void)
{
	sqlite3_close(m_db);
}

//回调 - 获得tid
int cb_getTid(void * para,int nCount,char** pValue,char** pName)
{
	int * tid = (int *)para;
	*tid = atoi(pValue[0]);
	return 0;
}

int cb_getPass(void * para,int nCount,char** pValue,char** pName)
{
	InfoRow * info = (InfoRow *)para;
	info->pid = atoi(pValue[4]);
	strcpy_s(info->szUser, _countof(info->szUser), pValue[0]);
	strcpy_s(info->szPass, _countof(info->szPass), pValue[1]);
	strcpy_s(info->szUrl, _countof(info->szUrl), pValue[2]);
	strcpy_s(info->szRemark, _countof(info->szRemark), pValue[3]);
	return 0;
}

int cb_Exists(void * para,int nCount,char** pValue,char** pName)
{
	int * b = (int *)para;
	*b = atoi(pValue[0]);
	return 0;
}

//复制文件的线程函数
void CopyFileThrd( void * para)
{
	BOOL rc = CopyFile(szSrcPath, szDstPath, TRUE);
	if (rc)
	{
		MessageBox(NULL, "备份完成", "提示", MB_OK);
	}else{
		MessageBox(NULL, "备份文件出错", "提示", MB_OK);
	}
}

void BakNoPassBook(void * para)
{
	char * szPass = (char *)para;
	int len = 0;
	len = strlen(szPass);
	BOOL rc = CopyFile(szSrcPath, szDstPath, TRUE);
	if (rc)
	{
		sqlite3 * bak_db;
		int re = sqlite3_open(szDstPath, &bak_db);
		if (re != SQLITE_OK)
		{
			MessageBox(NULL, "备份失败请重试！", "提示", MB_OK);
			delete szPass;
			return ;
		}
		re = sqlite3_key(bak_db, szPass, len);
		re = sqlite3_rekey(bak_db, NULL, 0);
		sqlite3_close(bak_db);
		if (re == SQLITE_OK)
		{
			char szTmp[MAX_PATH + 16];
			sprintf_s(szTmp, MAX_PATH + 16, "/select,%s", szDstPath);
			MessageBox(NULL, "备份文件成功，请保管好未加密的密码本！", "提示", MB_OK);
			ShellExecute(NULL, "open", "Explorer.exe", szTmp, NULL, SW_SHOWNORMAL);
		}else
		{
			MessageBox(NULL, "密码错误！", "提示", MB_OK);
		}		
	}else{
		MessageBox(NULL, "备份文件出错！", "提示", MB_OK);
	}
	delete szPass;
}

bool doSql::bakToSql(char szPath[], char *szPass)
{
	if (!strcmp("", szPath))
	{
		return false;
	}
	const char * szDbFile = sqlite3_db_filename(m_db, "main");
	strcpy_s(szSrcPath, MAX_PATH, szDbFile);
	strcpy_s(szDstPath, MAX_PATH, szPath);
	_beginthread(cb_DumpSql, 0, szPass);
	return true;
}


bool doSql::bakToCsv(char szPath[], char * szPass)
{
	if (!strcmp("", szPath))
	{
		return false;
	}
	const char * szDbFile = sqlite3_db_filename(m_db, "main");
	strcpy_s(szSrcPath, MAX_PATH, szDbFile);
	strcpy_s(szDstPath, MAX_PATH, szPath);
	_beginthread(cb_DumpCsv, 0, szPass);
	return true;
}

void cb_DumpCsv(void * para)
{
	TCHAR szTmpPath[MAX_PATH], szTmpDbFile[MAX_PATH];
	GetTempPath(MAX_PATH, szTmpPath);
	GetTempFileName(szTmpPath, "idb", 0, szTmpDbFile);
	char * szPass = (char *)para;
	int len = strlen(szPass);
	BOOL rc = CopyFile(szSrcPath, szTmpDbFile, FALSE);
	if (rc)
	{
		FILE * fp = fopen(szDstPath, "w");
		sqlite3 * bak_db;
		int re = sqlite3_open(szTmpDbFile, &bak_db);
		re = sqlite3_key(bak_db, szPass, len);
		re = sqlite3_rekey(bak_db, NULL, 0);
		if (SQLITE_OK != re)
		{
			MessageBox(NULL, "密码错误！", "提示", MB_OK);
			goto EndPos;
		}
		sqlite3_stmt * stmt;
		sqlite3_prepare(bak_db, "SELECT pid,user,password,url,remarks FROM pass;", -1, &stmt, NULL);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			fprintf_s(fp, "\"%i\",\"%s\",\"%s\",\"%s\",\"%s\"\n", sqlite3_column_int(stmt, 0), 
				sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3),
				sqlite3_column_text(stmt, 4));
		}
		sqlite3_finalize(stmt);
		sqlite3_close(bak_db);
		fclose(fp);
		char szCmd[MAX_PATH + 16];
		sprintf_s(szCmd, MAX_PATH + 16 , "/select,%s", szDstPath);
		ShellExecute(NULL, "open", "Explorer.exe", szCmd, NULL, SW_SHOWNORMAL);
	} 
	else
	{
		MessageBox(NULL, "备份文件失败！", "提示", MB_OK);
	}

EndPos:
	DeleteFile(szTmpDbFile);
	delete szPass;
}


//导出sql文件的线程函数
void cb_DumpSql(void * para)
{
	TCHAR szTmpPath[MAX_PATH], szTmpExeFile[MAX_PATH], szTmpDbFile[MAX_PATH];
	GetTempPath(MAX_PATH, szTmpPath);
	GetTempFileName(szTmpPath, "idb", 0, szTmpDbFile);
	srand(time(0));
	sprintf_s(szTmpExeFile, MAX_PATH, "%sdbpas_%d.exe", szTmpPath, rand());
	char * szPass = (char *)para;
	int len = strlen(szPass);
	BOOL rc = CopyFile(szSrcPath, szTmpDbFile, FALSE);
	if (rc)
	{
		//定位我们的自定义资源，这里因为我们是从本模块定位资源，所以将句柄简单地置为NULL即可
		HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCEA(IDR_RCEXE), TEXT("RCEXE"));
		if (NULL == hRsrc)
			goto ErrPos;
		//获取资源的大小
		DWORD dwSize = SizeofResource(NULL, hRsrc); 
		if (0 == dwSize)
			goto ErrPos;
		//加载资源
		HGLOBAL hGlobal = LoadResource(NULL, hRsrc); 
		if (NULL == hGlobal)
			goto ErrPos;
		//锁定资源
		LPVOID pBuffer = LockResource(hGlobal); 
		if (NULL == pBuffer)
			goto ErrPos;

		BOOL bRt = FALSE;
		FILE* fp = fopen(szTmpExeFile, "wb");
		if (fp != NULL)
		{
			if (dwSize == fwrite(pBuffer, sizeof(char), dwSize, fp))
				bRt = TRUE;
			fclose(fp);
		}
		if (!bRt)
		{
			MessageBox(NULL, "释放资源失败！", "提示", MB_OK);
			goto ErrPos;
		}
		sqlite3 * bak_db;
		int re = sqlite3_open(szTmpDbFile, &bak_db);
		re = sqlite3_key(bak_db, szPass, len);
		re = sqlite3_rekey(bak_db, NULL, 0);
		sqlite3_close(bak_db);
		if (re != SQLITE_OK)
		{
			MessageBox(NULL, "密码错误！", "提示", MB_OK);
			goto ErrPos;
		}
		char szCmd[MAX_PATH * 2 + 20];
		sprintf_s(szCmd, MAX_PATH * 2 + 20, "\"%s\" %s .dump>%s", szTmpExeFile, szTmpDbFile, szDstPath);
		system(szCmd);
		sprintf_s(szCmd, MAX_PATH * 2 + 20 , "/select,%s", szDstPath);
		ShellExecute(NULL, "open", "Explorer.exe", szCmd, NULL, SW_SHOWNORMAL);
	}
	else
	{
		MessageBox(NULL, "备份文件失败！", "提示", MB_OK);
	}
ErrPos:
	DeleteFile(szTmpDbFile);
	DeleteFile(szTmpExeFile);
	delete szPass;
}