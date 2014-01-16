#pragma once

typedef struct sqlite3 sqlite3;

class InfoRow{
public:
	int pid;
	char szUser[32];
	char szPass[128];
	char szUrl[256];
	char szRemark[256];
};

class doSql
{
public:
	doSql();
	~doSql(void);
	int getErrNo();
	bool doSql::addPass(char * pass, char * user, char * url, char * tags[], int num, char * remark);
	bool createPassBook();
	bool InitCreate(const char * pName, const char * key, int len);
	bool InitOpen(const char * pName, const char * key, int len);
	int doSql::SearchPass(char * szKeyWord, InfoRow ** info);
	bool doSql::reKey(char szKey[], int len);
	bool doSql::bakDatabase(char szPath[], char szPass[], int OutType, bool bPass);
	int doSql::getInfoById(int pid, InfoRow * info);
	int doSql::getTagByPid(int pid, char szTag[]);
	bool doSql::editPass(int pid, char * pass, char * user, char * url, char * tags[], int num, char * remark);
	bool doSql::delPass(int pid);
	int doSql::delMoreTag();

private:
	int getMaxId(char * tbname);
	bool doSql::bakToPas(char szPath[], bool bPass, char *szPass = NULL);
	bool doSql::bakToSql(char szPath[], char *szPass);
	bool doSql::bakToCsv(char szPath[], char * szPass);
	int doSql::MbcsToUtf8(const char *file, char * outfile);

	sqlite3 * m_db;
	int m_errNum;
	char * m_errMsg;
	int m_passInsertId;
	int m_tagInsertId;
};

//回调函数
int cb_GetMaxId(void * para,int nCount,char** pValue,char** pName);
int cb_getTid(void * para,int nCount,char** pValue,char** pName);
int cb_getPass(void * para,int nCount,char** pValue,char** pName);
int cb_getPid(void * para,int nCount,char** pValue,char** pName);
int cb_Exists(void * para,int nCount,char** pValue,char** pName);
int cb_getTagName(void * para,int nCount,char** pValue,char** pName);
int cb_getAllTid(void * para,int nCount,char** pValue,char** pName);
int cb_TidFew(void * para,int nCount,char** pValue,char** pName);
void CopyFileThrd( void * para);
void BakNoPassBook(void * para);
void cb_DumpSql(void * para);
void cb_DumpCsv(void * para);

//101				打开数据库文件错误
//102				创建密码本错误
//103				插入密码错误
//104				插入标签错误
//105				插入关系出错
//106				数据库文件不存在
//107				数据库密码错误
//108				查询错误
//109				修改错误
//110				删除密码错误