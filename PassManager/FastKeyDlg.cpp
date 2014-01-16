// FastKeyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "FastKeyDlg.h"
#include "afxdialogex.h"


// CFastKeyDlg 对话框

IMPLEMENT_DYNAMIC(CFastKeyDlg, CDialogEx)

CFastKeyDlg::CFastKeyDlg(CString strIniName, CWnd* pParent /*=NULL*/)
	: CDialogEx(CFastKeyDlg::IDD, pParent)
{
	m_strIniPath = strIniName;
	m_Parent = pParent;
}

CFastKeyDlg::~CFastKeyDlg()
{
	
}

void CFastKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FASTDLG_SHOW, m_ShowKey);
	DDX_Control(pDX, IDC_FASTDLG_SEARCH, m_SearchKey);
}


BEGIN_MESSAGE_MAP(CFastKeyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FASTDLG_OK, &CFastKeyDlg::OnBnClickedFastdlgOk)
	ON_BN_CLICKED(IDC_FASTDLG_CANCEL, &CFastKeyDlg::OnBnClickedFastdlgCancel)
	
	ON_BN_CLICKED(IDC_FASTDLG_CLEAR, &CFastKeyDlg::OnBnClickedFastdlgClear)
END_MESSAGE_MAP()


// CFastKeyDlg 消息处理程序


void CFastKeyDlg::OnBnClickedFastdlgOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UnregisterHotKey(m_Parent->GetSafeHwnd(), WM_HOTKEY_SHOW);
	UnregisterHotKey(m_Parent->GetSafeHwnd(), WM_HOTKEY_SEARCH);
	DWORD dwHotKey;
	WORD dHigh, dLow; //dHigh是ALT、CTRL等键的代码，dLow是键盘上某个键
	CString strKey;
	m_ShowKey.GetHotKey(dLow, dHigh);
	if (dHigh == HOTKEYF_ALT)
	{
		dHigh = MOD_ALT;
	}else
	if (dHigh == HOTKEYF_SHIFT)
	{
		dHigh = MOD_SHIFT; 
	}
	//注册热键
	RegisterHotKey(m_Parent->GetSafeHwnd(), WM_HOTKEY_SHOW, dHigh, dLow);
	dwHotKey = (dHigh << 16) | dLow;
	strKey.Format("%ld", dwHotKey);
	WritePrivateProfileString("HOTKEY", "SHOW", strKey, m_strIniPath);
	m_SearchKey.GetHotKey(dLow, dHigh);
	if (dHigh == HOTKEYF_ALT)
	{
		dHigh = MOD_ALT;
	}else
	if (dHigh == HOTKEYF_SHIFT)
	{
		dHigh = MOD_SHIFT; 
	}
	//注册热键
	RegisterHotKey(m_Parent->GetSafeHwnd(), WM_HOTKEY_SEARCH, dHigh, dLow);
	dwHotKey = (dHigh << 16) | dLow;
	strKey.Format("%ld", dwHotKey);
	WritePrivateProfileString("HOTKEY", "SEARCH", strKey, m_strIniPath);
	SendMessage(WM_CLOSE);
	return ;
}


void CFastKeyDlg::OnBnClickedFastdlgCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


BOOL CFastKeyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UINT showHotKey = GetPrivateProfileInt("HOTKEY", "SHOW", 0, m_strIniPath);
	UINT searchHotKey = GetPrivateProfileInt("HOTKEY", "SEARCH", 0, m_strIniPath);

	if (showHotKey && searchHotKey)
	{
		if (showHotKey >> 16 == HOTKEYF_ALT)
		{
			showHotKey = (HOTKEYF_SHIFT << 16) + (showHotKey & 0x0000FFFF);
		}else
		if (showHotKey >> 16 == HOTKEYF_SHIFT)
		{
			showHotKey = (HOTKEYF_ALT << 16) + (showHotKey & 0x0000FFFF);
		}
		if (searchHotKey >> 16 == HOTKEYF_ALT)
		{
			searchHotKey = (HOTKEYF_SHIFT << 16) + (searchHotKey & 0x0000FFFF);
		}else
		if (searchHotKey >> 16 == HOTKEYF_SHIFT)
		{
			searchHotKey = (HOTKEYF_ALT << 16) + (searchHotKey & 0x0000FFFF);
		}
		m_ShowKey.SetHotKey(showHotKey & 0x0000FFFF, showHotKey >> 16);
		m_SearchKey.SetHotKey(searchHotKey & 0x0000FFFF, searchHotKey >> 16);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFastKeyDlg::OnBnClickedFastdlgClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ShowKey.SetHotKey(0, 0);
	m_SearchKey.SetHotKey(0, 0);
}
