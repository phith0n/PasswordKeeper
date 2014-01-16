// AddPassDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "PassManagerDlg.h"
#include "AddPassDlg.h"
#include "afxdialogex.h"

#include <ctime>

void QuickSort(char * arr,int low,int high);
int FindPos(char * arr,int low,int high);

// CAddPassDlg 对话框

IMPLEMENT_DYNAMIC(CAddPassDlg, CDialogEx)

CAddPassDlg::CAddPassDlg(doSql * sql, CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddPassDlg::IDD, pParent)
	, m_strPass(_T(""))
	, m_strTag(_T(""))
	, m_strUrl(_T(""))
	, m_strRemark(_T(""))
	, m_strUser(_T(""))
{
	m_sql = sql;
	m_Parent = pParent;
	EditPid = -1;
}

CAddPassDlg::~CAddPassDlg()
{
}

void CAddPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDDLG_PASS, m_strPass);
	DDX_Text(pDX, IDC_ADDDLG_TAG, m_strTag);
	DDX_Text(pDX, IDC_ADDDLG_URL, m_strUrl);
	DDX_Text(pDX, IDC_ADDDLG_REMARK, m_strRemark);
	DDX_Text(pDX, IDC_ADDDLG_USER, m_strUser);
}


BEGIN_MESSAGE_MAP(CAddPassDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADDDLG_CANCEL, &CAddPassDlg::OnBnClickedAdddlgCancel)
	ON_BN_CLICKED(IDC_ADDDLG_CHKENGSMALL, &CAddPassDlg::OnBnClickedAdddlgChkengsmall)
	ON_BN_CLICKED(IDC_ADDDLG_CHKENGLARGE, &CAddPassDlg::OnBnClickedAdddlgChkenglarge)
	ON_BN_CLICKED(IDC_ADDDLG_CHKNUM, &CAddPassDlg::OnBnClickedAdddlgChknum)
	ON_BN_CLICKED(IDC_ADDDLG_SIG, &CAddPassDlg::OnBnClickedAdddlgSig)
	ON_WM_MOUSEMOVE()
	ON_STN_DBLCLK(IDC_BMPPASS, &CAddPassDlg::OnStnDblclickBmppass)
	ON_BN_CLICKED(IDC_ADDDLG_OK, &CAddPassDlg::OnBnClickedAdddlgOk)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAddPassDlg 消息处理程序


void CAddPassDlg::OnBnClickedAdddlgCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strStatus = "你可以使用你新建密码时填写的“标签”来查找密码";
	SendMessage(WM_CLOSE);
}


BOOL CAddPassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SelectBox(IDC_ADDDLG_CHKENGSMALL);
	SelectBox(IDC_ADDDLG_CHKNUM);
	SelectBox(IDC_ADDDLG_CHKENGLARGE);
	SetDlgItemText(IDC_ADDDLG_ALLWORD, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	SetDlgItemInt(IDC_ADDDLG_PASSLEN, 12);
	m_pSeed = GetDlgItem(IDC_ADDDLG_SEED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddPassDlg::SelectBox(int name, BOOL select)
{
	CButton * pWin = (CButton *)GetDlgItem(name);
	if (select)
	{
		pWin->SetCheck(BST_CHECKED);
	}else{
		pWin->SetCheck(BST_UNCHECKED);
	}
}

//去除字符串中的重复字符
CString CAddPassDlg::DelRepet(TCHAR * szStr, int len)
{
	CString strRe("");
	QuickSort(szStr, 0, len);
	for (int i = 0 ; i <= len ; i++)
	{
		if (i > 0 && szStr[i] == szStr[i - 1])
		{
			continue;
		}
		strRe += szStr[i];
	}
	return strRe;
}

//快速排序。
//high为字符串长度 - 1
void QuickSort(TCHAR * arr,int low,int high){
	int pos;
	if(low < high){
		pos = FindPos(arr,low,high);
		QuickSort(arr,low,pos-1);
		QuickSort(arr,pos+1,high);
	}
	return ;
}

int FindPos(TCHAR * arr,int low,int high)
{
	int val = arr[low];
	while(low < high){
		while(arr[high] >= val && low < high)
			high--;
		arr[low] = arr[high];
		while(arr[low] <= val && low < high)
			low++;
		arr[high] = arr[low];
	} //循环完成后high == low
	arr[low] = val;
	return(low);
}

//点击小写复选框
void CAddPassDlg::OnBnClickedAdddlgChkengsmall()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton * pWin = (CButton *)GetDlgItem(IDC_ADDDLG_CHKENGSMALL);
	int bCheck = pWin->GetCheck();
	TCHAR szChar[192];
	if (bCheck == BST_CHECKED)
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars += "abcdefghijklmnopqrstuvwxyz";
		strChars = DelRepet(strChars.GetBuffer(), strChars.GetLength() - 1);
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
	else
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars.Replace("abcdefghijklmnopqrstuvwxyz", "");
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
}

void CAddPassDlg::OnBnClickedAdddlgChkenglarge()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton * pWin = (CButton *)GetDlgItem(IDC_ADDDLG_CHKENGLARGE);
	int bCheck = pWin->GetCheck();
	TCHAR szChar[192];
	if (bCheck == BST_CHECKED)
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		strChars = DelRepet(strChars.GetBuffer(), strChars.GetLength() - 1);
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
	else
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars.Replace("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "");
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
}


void CAddPassDlg::OnBnClickedAdddlgChknum()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton * pWin = (CButton *)GetDlgItem(IDC_ADDDLG_CHKNUM);
	int bCheck = pWin->GetCheck();
	TCHAR szChar[192];
	if (bCheck == BST_CHECKED)
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars += "0123456789";
		strChars = DelRepet(strChars.GetBuffer(), strChars.GetLength() - 1);
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
	else
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars.Replace("0123456789", "");
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
}


void CAddPassDlg::OnBnClickedAdddlgSig()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton * pWin = (CButton *)GetDlgItem(IDC_ADDDLG_SIG);
	int bCheck = pWin->GetCheck();
	TCHAR szChar[192];
	if (bCheck == BST_CHECKED)
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars += "!#$%&()*+,-.:;=?@";
		strChars = DelRepet(strChars.GetBuffer(), strChars.GetLength() - 1);
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
	else
	{
		GetDlgItemText(IDC_ADDDLG_ALLWORD, szChar, 96);
		CString strChars(szChar);
		strChars.Replace("!#$%&()*+,-.", "");
		strChars.Replace(":;=?@", "");
		SetDlgItemText(IDC_ADDDLG_ALLWORD, strChars.GetBuffer());
	}
}

//双击图片
void CAddPassDlg::OnStnDblclickBmppass()
{
	// TODO: 在此添加控件通知处理程序代码
	int len = GetDlgItemInt(IDC_ADDDLG_PASSLEN);
	len = len > 16 ? 16 : len;
	CString strChars, strPass("");
	GetDlgItem(IDC_ADDDLG_ALLWORD)->GetWindowText(strChars);
	if (strChars.IsEmpty())
	{
		MessageBox("预选密码框不能为空");
		return ;
	}
	HINSTANCE hCryptDll = LoadLibrary(TEXT("Cryptdll.dll"));
	if (INVALID_HANDLE_VALUE==hCryptDll||NULL==hCryptDll)
	{
		MessageBox("你的计算机不支持生成完美的随机数，请手动输入密码!", "出错");
		return ;
	}
	HCRYPTPROV crypt_prov_;
	if( !CryptAcquireContext( &crypt_prov_, NULL, NULL, PROV_RSA_FULL, 0) ) 
	{
		if( !CryptAcquireContext( &crypt_prov_, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET) ) 
			MessageBox("你的计算机不支持生成完美的随机数，请手动输入密码!", "出错");
	}
	BYTE * data = new BYTE[len];
	unsigned long r = 0;
	if( crypt_prov_ ){
		if( CryptGenRandom( crypt_prov_, sizeof(data), data) ) 
		{
			memcpy( &r, data, sizeof(data) );
		}
	}
	CryptReleaseContext(crypt_prov_, 0);
	for (int i = 0 ; i < len ; i++)
	{
		int count = strChars.GetLength();
		strPass += strChars[abs((data[i] ^ (m_seed >> i)) % count)];
	}
	SetDlgItemText(IDC_ADDDLG_PASS, strPass);
	setRandSeed();
}


BOOL CAddPassDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{
		setRandSeed();
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CAddPassDlg::setRandSeed()
{
	CPoint point;
	GetCursorPos(&point);
	GetDlgItem(IDC_BMPPASS)->GetWindowRect(&m_bmpRect);
	//ScreenToClient(m_bmpRect);
	srand(unsigned(time(0) + GetTickCount()));
	if (m_bmpRect.PtInRect(point))
	{
		int x = point.x - m_bmpRect.left,
			y = point.y - m_bmpRect.top;
		m_seed = (x ^ rand()) * y - (y ^ rand()) * x;
		CString strSeed;
		strSeed.Format("%d", m_seed);
		SetDlgItemText(IDC_ADDDLG_SEED, strSeed);
	}
}

InputInfo CAddPassDlg::getInput()
{
	UpdateData(TRUE);
	InputInfo info(m_strPass, m_strTag, m_strUrl, m_strRemark);
	return info;
}

//点击添加按钮
void CAddPassDlg::OnBnClickedAdddlgOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strPass.GetLength() > 128 || m_strUser.GetLength() > 32 || m_strUrl.GetLength() > 256 ||
		m_strTag.GetLength() > 256)
	{
		MessageBox("填写的信息过长");
		return ;
	}
	if (m_strUser.IsEmpty() || m_strPass.IsEmpty() || m_strTag.IsEmpty())
	{
		MessageBox("账号、密码或标签不能为空");
		return ;
	}
	int i = 0;
	TCHAR * token;
	char * strTags[5];
	token = _tcstok(m_strTag.GetBuffer(), "|");
	while (token && i < 5)
	{
		strTags[i++] = token;
		if (strlen(strTags[i - 1]) > 32)
		{
			MessageBox("填写的信息过长");
			return ;
		}
		token = _tcstok(NULL, "|");
	}
	bool rc;
	if(m_bEdit){
		rc = m_sql->editPass(EditPid, m_strPass.GetBuffer(), m_strUser.GetBuffer(), m_strUrl.GetBuffer(),
			strTags, i, m_strRemark.GetBuffer());
	}
	else{
		rc = m_sql->addPass(m_strPass.GetBuffer(), m_strUser.GetBuffer(), m_strUrl.GetBuffer(),
		strTags, i, m_strRemark.GetBuffer());
	}
	if (!rc)
	{
		CString str;
		str.Format("操作数据库失败！错误代码：%d", m_sql->getErrNo());
		MessageBox(str);
		return ;
	}
	m_strStatus.Format("操作成功", m_strUrl);
	SendMessage(WM_CLOSE);
}

void CAddPassDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPassManagerDlg * p = (CPassManagerDlg *)m_Parent;
	p->OnSonDlgClose(m_strStatus);
	p->OnBnClickedBtnsearch();
	CDialogEx::OnClose();
}

void CAddPassDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	if (m_bEdit && EditPid > 0)
	{
		InfoRow info;
		SetWindowText("修改密码");
		m_sql->getInfoById(EditPid, &info);
		m_strPass = info.szPass;
		m_strUser = info.szUser;
		m_strUrl = info.szUrl;
		m_strRemark = info.szRemark;
		char szTag[256] = {0};
		m_sql->getTagByPid(EditPid, szTag);
		m_strTag = szTag;
		if (m_strTag[0] == '|')
		{
			m_strTag.Delete(0, 1);
		}
		UpdateData(FALSE);
	}
	else
	{
		SetWindowText("增加密码");
		m_strPass = m_strUrl = m_strTag = m_strUser = m_strRemark = "";
		UpdateData(FALSE);
		CEdit * p = (CEdit *)GetDlgItem(IDC_ADDDLG_USER);
		p->SetSel(0, -1);
	}
}
