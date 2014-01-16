// DetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "DetailDlg.h"
#include "afxdialogex.h"
#include "PassManagerDlg.h"


// CDetailDlg 对话框

IMPLEMENT_DYNAMIC(CDetailDlg, CDialogEx)

CDetailDlg::CDetailDlg(CString strUser, CString strPass, CString strUrl, CString strMak, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetailDlg::IDD, pParent)
{
	m_strUser = strUser;
	m_strPass = strPass;
	m_strUrl = strUrl;
	m_strMak = strMak;
	pPar = pParent;
}

CDetailDlg::~CDetailDlg()
{
}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DETAIL_RETURN, &CDetailDlg::OnBnClickedDetailReturn)
	ON_BN_CLICKED(IDC_BTNDETAIL_USER, &CDetailDlg::OnBnClickedBtndetailUser)
	ON_BN_CLICKED(IDC_BTNDETAIL_PASS, &CDetailDlg::OnBnClickedBtndetailPass)
	ON_BN_CLICKED(IDC_BTNDETAIL_URL, &CDetailDlg::OnBnClickedBtndetailUrl)
	ON_BN_CLICKED(IDC_BTNDETAIL_MAK, &CDetailDlg::OnBnClickedBtndetailMak)
END_MESSAGE_MAP()


// CDetailDlg 消息处理程序


void CDetailDlg::OnBnClickedDetailReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}


BOOL CDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_DETAILEDIT_USER, (LPCTSTR)m_strUser);
	SetDlgItemText(IDC_DETAILEDIT_PASS, (LPCTSTR)m_strPass);
	SetDlgItemText(IDC_DETAILEDIT_URL, (LPCTSTR)m_strUrl);
	SetDlgItemText(IDC_DETAILEDIT_MAK, (LPCTSTR)m_strMak);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//复制函数
void CDetailDlg::CopyStr(CString strCopy)
{
	CPassManagerDlg * p = (CPassManagerDlg *)pPar;
	if(!OpenClipboard())
	{
		p->OnSonDlgClose("复制出错");
		return ;
	}
	EmptyClipboard();
	HANDLE hClip;
	hClip=GlobalAlloc(GMEM_MOVEABLE,strCopy.GetLength()+1);
	char *pBuf;
	pBuf=(char *)GlobalLock(hClip);
	strcpy(pBuf,strCopy.GetBuffer());
	GlobalUnlock(hClip);
	SetClipboardData(CF_TEXT,hClip);
	CloseClipboard();
	p->OnSonDlgClose("选择的内容已复制");
	OnBnClickedDetailReturn();
}

//复制用户名
void CDetailDlg::OnBnClickedBtndetailUser()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_DETAILEDIT_USER, str);
	CopyStr(str);
}

//复制密码
void CDetailDlg::OnBnClickedBtndetailPass()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_DETAILEDIT_PASS, str);
	CopyStr(str);
}

//复制url
void CDetailDlg::OnBnClickedBtndetailUrl()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_DETAILEDIT_URL, str);
	CopyStr(str);
}

//复制备注
void CDetailDlg::OnBnClickedBtndetailMak()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_DETAILEDIT_MAK, str);
	CopyStr(str);
}
