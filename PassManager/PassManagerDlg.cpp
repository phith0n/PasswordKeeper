
// PassManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "PassManagerDlg.h"
#include "afxdialogex.h"
#include "InputPassDlg.h"
#include "BakDlg.h"
#include "OptionDlg.h"
#include "DetailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT indicators[] = {
	IDS_STRBAR
};

const char * Author = "http://www.leavesongs.com";
const char * HelpSite = "http://pk.leavesongs.com";

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPassManagerDlg 对话框

CPassManagerDlg::CPassManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassManagerDlg::IDD, pParent)
	, m_strSearch(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINICON);
	m_sql = NULL;
	m_addPassDlg = NULL;
	m_fastDlg = NULL;
	m_SearchDlg = NULL;
	m_bEdit = false;
}

void CPassManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RELIST, m_listShow);
	DDX_Text(pDX, IDC_EDITWORD, m_strSearch);
	DDX_Control(pDX, IDC_EDITWORD, m_EditSearch);
}

BEGIN_MESSAGE_MAP(CPassManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(MENU_NEWBOOK, &CPassManagerDlg::OnNewbook)
	ON_COMMAND(MENU_OPENBOOK, &CPassManagerDlg::OnOpenbook)
//	ON_WM_SIZE()
	ON_COMMAND(MENU_QUIT, &CPassManagerDlg::OnQuit)
	ON_NOTIFY(NM_RCLICK, IDC_RELIST, &CPassManagerDlg::OnNMRClickRelist)
	ON_COMMAND(MENU_ADD, &CPassManagerDlg::OnAdd)
	ON_BN_CLICKED(IDC_BTNSEARCH, &CPassManagerDlg::OnBnClickedBtnsearch)
	ON_COMMAND(ID_POPCREATE, &CPassManagerDlg::OnPopcreate)
	ON_COMMAND(ID_POPOPEN, &CPassManagerDlg::OnPopopen)
	ON_COMMAND(ID_MPOPADD, &CPassManagerDlg::OnMpopadd)
	ON_COMMAND(ID_MPOPCOPY, &CPassManagerDlg::OnMpopcopy)
	ON_COMMAND(ID_CLOSEBOOK, &CPassManagerDlg::OnClosebook)
	ON_MESSAGE(WM_TRAYICON_MOUSEOVER, OnTrayCallBackMsg)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(MENU_FAST, &CPassManagerDlg::OnFast)
	ON_COMMAND(ID_TRAY_QUIT, &CPassManagerDlg::OnTrayQuit)
	ON_COMMAND(MENU_QUERY, &CPassManagerDlg::OnQuery)
	ON_COMMAND(ID_TRAY_SEARCH, &CPassManagerDlg::OnTraySearch)
	ON_COMMAND(ID_TRAY_OPEN, &CPassManagerDlg::OnTrayOpen)
	ON_COMMAND(MENU_REPASS, &CPassManagerDlg::OnRepass)
	ON_COMMAND(MENU_BAK, &CPassManagerDlg::OnBak)
	ON_COMMAND(ID_MPOPEDIT, &CPassManagerDlg::OnMpopedit)
	ON_COMMAND(ID_MPOPDEL, &CPassManagerDlg::OnMpopdel)
	ON_COMMAND(MENU_ABOUT, &CPassManagerDlg::OnAbout)
	ON_WM_DROPFILES()
	ON_COMMAND(MENU_OPTION, &CPassManagerDlg::OnOption)
	ON_NOTIFY(NM_DBLCLK, IDC_RELIST, &CPassManagerDlg::OnNMDblclkRelist)
	ON_COMMAND(MENU_HELP, &CPassManagerDlg::OnHelp)
	ON_COMMAND(MENU_LEAVESONGS, &CPassManagerDlg::OnLeavesongs)
END_MESSAGE_MAP()


// CPassManagerDlg 消息处理程序

BOOL CPassManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//SetWindowSkin( this->m_hWnd, "Dialog" ); 
	//SetDialogSkin( "Dialog" );
	mainMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENUMAIN));
	::SetMenu(this->m_hWnd, mainMenu);
	::DrawMenuBar(this->m_hWnd);

	CRect rect;
	m_bar.Create(this); //We create the status bar  
	m_bar.SetIndicators(indicators,1);
	GetClientRect(&rect); 
	m_bar.SetPaneInfo(0,IDS_STRBAR,SBPS_NORMAL,rect.Width());
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,IDS_STRBAR);

	//初始化列表
	InitList();

	//创建托盘图标
	TrayMyIcon(true);
	m_bShow = true;

	//初始化配置文件并注册热键
	CreateDefaultIniFile();

	//是否能显示快速搜索对话框
	m_bSearchShow = false;

	//获得命令行参数，如果有参数的话，则该参数就是需要打开的密码本文件
	CString strCmd = GetCommandLine();
	//MessageBox(strCmd);
	BOOL isExists = AfxExtractSubString(m_dbPath, strCmd, 1, ' ');
	m_dbPath.Remove('"');
	if (isExists && !m_dbPath.IsEmpty() && PathFileExists(m_dbPath))
	{
		CInputPassDlg passDlg(this);
		INT_PTR rt = passDlg.DoModal();
		if (!rt)
		{
			return TRUE;
		}
		CString strPass = passDlg.getPassword();
		OpenPassBook(m_dbPath, strPass);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPassManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPassManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPassManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//点击“新建密码本”后触发
void CPassManagerDlg::OnNewbook()
{
	if (m_sql)
	{
		if (MessageBox("关闭当前密码本，并新建一个新的密码本？", "提醒", MB_OKCANCEL) == IDCANCEL)
		{
			return ;
		}else{
			OnClosebook();
		}
	}
	TCHAR szNowDir[MAX_PATH + 10];
	GetCurrentDirectory(MAX_PATH + 10, szNowDir);
	_tcscat_s(szNowDir, MAX_PATH + 9, "\\me.pas");
	CString strEditName, strPass;
	CFileDialog dlg(FALSE, "pas", szNowDir, OFN_EXPLORER | OFN_OVERWRITEPROMPT, "Password Book (*.pas)|*.pas||", this, 0, FALSE);
	CInputPassDlg passDlg(this);
	INT_PTR rt = dlg.DoModal();
	if (IDOK != rt)
	{
		return ;
	}
	rt = passDlg.DoModal();
	if (!rt)
	{
		return ;
	}
	m_dbPath = dlg.GetPathName();
	strPass = passDlg.getPassword();
	m_sql = new doSql();
	if(! m_sql->InitCreate(m_dbPath.GetBuffer(), strPass.GetBuffer(), strPass.GetLength())){
		int errNo;
		CString str;
		errNo = m_sql->getErrNo();
		str.Format("出错，错误代码：%d", errNo);
		MessageBox(str);
	}else{
		setStatusBar("新建密码本成功");
		EnableMenus();
		clearListEdit();
	}
	m_dbPath.ReleaseBuffer();
	strPass.ReleaseBuffer();
}

//点击“打开密码本”时触发
void CPassManagerDlg::OnOpenbook()
{
	if (m_sql)
	{
		if (MessageBox("关闭当前密码本，并打开另一个一个密码本？", "提醒", MB_OKCANCEL) == IDCANCEL)
		{
			return ;
		}else{
			OnClosebook();
		}
	}
	// TODO: 在此添加命令处理程序代码
	TCHAR szNowDir[MAX_PATH + 10];
	CString strEditName, strPass, strNowDir;
	GetModuleFileName(NULL, szNowDir, MAX_PATH + 10);
	strNowDir = szNowDir;
	strNowDir = strNowDir.Left(strNowDir.ReverseFind('\\'));
	strNowDir += "\\me.pas";
	CFileDialog dlg(TRUE, "pas", strNowDir, OFN_EXPLORER | OFN_OVERWRITEPROMPT, "Password Book (*.pas)|*.pas||", this, 0, FALSE);
	CInputPassDlg passDlg(this);
	INT_PTR rt = dlg.DoModal();
	if (IDOK != rt)
	{
		return ;
	}
	rt = passDlg.DoModal();
	if (!rt)
	{
		return ;
	}
	m_dbPath = dlg.GetPathName();
	strPass = passDlg.getPassword();
	OpenPassBook(m_dbPath, strPass);
}

void CPassManagerDlg::InitList()
{
	CRect rect;
	GetWindowRect(&rect);
	int Width = rect.Width() - 2;
	int Height = rect.Height();
	m_listShow.InsertColumn(0, "username", LVCFMT_LEFT, Width / 6);
	m_listShow.InsertColumn(1, "password", LVCFMT_LEFT, Width / 6);
	m_listShow.InsertColumn(2, "remarks", LVCFMT_LEFT, int(Width / 2.5));
	m_listShow.InsertColumn(3, "url", LVCFMT_LEFT, Width - int(Width/3) - int(Width / 2.5) - 10);
	m_listShow.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//初始化右键菜单
	m_PopMenu.LoadMenu(IDR_MENUPASSPOP);
}

void CPassManagerDlg::setStatusBar(CString str)
{
	m_bar.SetPaneText(0, str);

}

//点击退出时触发
void CPassManagerDlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	SendMessage(WM_CLOSE);
}

//用户在列表中右键
void CPassManagerDlg::OnNMRClickRelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CPoint mousept;
	GetCursorPos(&mousept);
	CMenu * pMenu = NULL;
	pMenu = m_PopMenu.GetSubMenu(0);
	if (m_listShow.GetSelectedCount() > 0)
	{
		EnableMenu(&m_PopMenu, 0, ID_MPOPCOPY);
		EnableMenu(&m_PopMenu, 0, ID_MPOPDEL);
		EnableMenu(&m_PopMenu, 0, ID_MPOPEDIT);
	}
	else
	{
		EnableMenu(&m_PopMenu, 0, ID_MPOPCOPY, FALSE);
		EnableMenu(&m_PopMenu, 0, ID_MPOPDEL, FALSE);
		EnableMenu(&m_PopMenu, 0, ID_MPOPEDIT, FALSE);
	}
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, mousept.x, mousept.y, this);
	*pResult = 0;
}

void CPassManagerDlg::EnableMenus()
{
	CMenu * pMenu = GetMenu();
	EnableMenu(pMenu, 0, ID_CLOSEBOOK);
	EnableMenu(pMenu, 1, MENU_ADD);
	EnableMenu(pMenu, 1, MENU_QUERY);
	EnableDlg(IDC_BTNSEARCH);
	EnableMenu(&m_PopMenu, 0, ID_MPOPADD);
	EnableMenu(&m_PopMenu, 0, MENU_REPASS);
	EnableMenu(pMenu, 2, MENU_REPASS);
	EnableMenu(pMenu, 1, MENU_BAK);
}

void CPassManagerDlg::DisableMenus()
{
	CMenu * pMenu = GetMenu();
	EnableMenu(pMenu, 0, ID_CLOSEBOOK, FALSE);
	EnableMenu(pMenu, 1, MENU_ADD, FALSE);
	EnableMenu(pMenu, 1, MENU_QUERY, FALSE);
	EnableDlg(IDC_BTNSEARCH, FALSE);
	EnableMenu(&m_PopMenu, 0, ID_MPOPADD, FALSE);
	EnableMenu(&m_PopMenu, 0, MENU_REPASS, FALSE);
	EnableMenu(pMenu, 2, MENU_REPASS, FALSE);
	EnableMenu(pMenu, 1, MENU_BAK, FALSE);
}

void CPassManagerDlg::EnableDlg(int name, BOOL bEnable)
{
	CWnd * pWin = GetDlgItem(name);
	pWin->EnableWindow(bEnable);
}

void CPassManagerDlg::EnableMenu(CMenu * pMenu, int refer, UINT name, BOOL bEnable)
{
	if (bEnable)
	{
		pMenu->GetSubMenu(refer)->EnableMenuItem(name, MF_BYCOMMAND | MF_ENABLED);
	}else{
		pMenu->GetSubMenu(refer)->EnableMenuItem(name, MF_BYCOMMAND | MF_DISABLED);
	}
}

//增加或修改一个密码时调用
//调用之前设置
void CPassManagerDlg::OnAdd()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_addPassDlg || !m_addPassDlg->m_hWnd)
	{
		m_addPassDlg = new CAddPassDlg(m_sql, this);
		m_addPassDlg->Create(IDD_DLGADD);
	}
	m_addPassDlg->m_bEdit = m_bEdit;
	if (m_bEdit) m_addPassDlg->EditPid = m_IdArr.GetAt(m_listShow.GetSelectionMark());
	m_addPassDlg->ShowWindow(SW_SHOWNORMAL);
}

void CPassManagerDlg::OnSonDlgClose(CString strStatus)
{
	setStatusBar(strStatus);

}

//点击搜索按钮
void CPassManagerDlg::OnBnClickedBtnsearch()
{
	m_listShow.DeleteAllItems();
	m_IdArr.RemoveAll();
	InfoRow * info = NULL;
	UpdateData(TRUE);
	int count = m_sql->SearchPass(m_strSearch.GetBuffer(), &info);
	for (int i = 0 ; i < count ; i++)
	{
		m_listShow.InsertItem(i,info[i].szUser);
		m_listShow.SetItemText(i, 1, info[i].szPass);
		m_listShow.SetItemText(i, 2, info[i].szRemark);
		m_listShow.SetItemText(i, 3, info[i].szUrl);
		m_IdArr.Add(info[i].pid);
	}
	delete [] info;
	m_EditSearch.SetSel(0, -1);
}

//点击右键弹出菜单的创建按钮
void CPassManagerDlg::OnPopcreate()
{
	// TODO: 在此添加命令处理程序代码
	OnNewbook();
}

//点击右键弹出菜单的打开按钮
void CPassManagerDlg::OnPopopen()
{
	// TODO: 在此添加命令处理程序代码
	OnOpenbook();
}

//点击右键弹出菜单的增加密码按钮
void CPassManagerDlg::OnMpopadd()
{
	// TODO: 在此添加命令处理程序代码
	OnAdd();
}

//点击右键弹出菜单的复制密码按钮
void CPassManagerDlg::OnMpopcopy()
{
	// TODO: 在此添加命令处理程序代码
	CString strCopy;
	if(!OpenClipboard())
	{
		setStatusBar("复制出错");
		return ;
	}
	EmptyClipboard();
	int nowPos = m_listShow.GetSelectionMark();
	strCopy = m_listShow.GetItemText(nowPos, 1);
	HANDLE hClip;
	hClip=GlobalAlloc(GMEM_MOVEABLE,strCopy.GetLength()+1);
	char *pBuf;
	pBuf=(char *)GlobalLock(hClip);
	strcpy(pBuf,strCopy.GetBuffer());
	GlobalUnlock(hClip);
	SetClipboardData(CF_TEXT,hClip);
	CloseClipboard();
	setStatusBar("选择的密码已复制");
}

//关闭密码本
void CPassManagerDlg::OnClosebook()
{
	// TODO: 在此添加命令处理程序代码
	delete m_sql;
	m_sql = NULL;
	DisableMenus();
	clearListEdit();
	setStatusBar("密码本已关闭，请重新创建或打开密码本");
}

//清除编辑框和列表栏
void CPassManagerDlg::clearListEdit()
{
	m_EditSearch.SetWindowText("");
	m_listShow.DeleteAllItems();
}

//显示或销毁托盘图标，在程序启动、关闭时分别调用一次
BOOL CPassManagerDlg::TrayMyIcon(bool bShow)
{
	BOOL bRet = FALSE;
	NOTIFYICONDATA tnd;
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = this->m_hWnd;
	tnd.uID = IDR_MENU_NOTIFY;
	if (bShow)
	{
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = WM_TRAYICON_MOUSEOVER;
		tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAINICON));
		_tcscpy_s(tnd.szTip, sizeof(tnd.szTip), _T("密码圣经 V1.0"));
		bRet = Shell_NotifyIcon(NIM_ADD, &tnd);
	} 
	else
	{
		SetForegroundWindow();
		bRet = Shell_NotifyIcon(NIM_DELETE, &tnd);
	}
	return bRet;
}

//相应鼠标移动到托盘上的消息
LRESULT CPassManagerDlg::OnTrayCallBackMsg(WPARAM wparam, LPARAM lparam)
{
	switch (lparam)
	{
	case WM_RBUTTONUP:
		{
			PopNotifyMenu();
			SetForegroundWindow();
			break;
		}
	case WM_LBUTTONDBLCLK:
		{
			DoShowWindow(!m_bShow);
			break;
		}
	default:
		{
			break;
		}
	}
	return NULL;
}

//弹出托盘菜单
VOID CPassManagerDlg::PopNotifyMenu()
{
	CPoint pt;
	GetCursorPos(&pt);
	CMenu mMenu, * pMenu = NULL;
	mMenu.LoadMenu(IDR_MENU_NOTIFY);
	pMenu = mMenu.GetSubMenu(0);
	SetForegroundWindow();
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}

//当窗口大小该表时调用
void CPassManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// 当消息是最小化程序时
	if (SIZE_MINIMIZED == nType)
	{
		DoShowWindow(false);
		return ;
	}
}

void CPassManagerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TrayMyIcon(false);
	CDialogEx::OnClose();
}

//显示或隐藏窗口
void CPassManagerDlg::DoShowWindow(bool bShow)
{
	if (bShow)
	{
		ShowWindow(SW_RESTORE);
		ShowWindow(SW_SHOWNA);
		SetForegroundWindow();
	}else
	{
		ShowWindow(SW_HIDE);
	}
	this->m_bShow = bShow;
	return ;
}

void CPassManagerDlg::OnFast()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_fastDlg || !m_fastDlg->m_hWnd)
	{
		m_fastDlg = new CFastKeyDlg(m_iniFilePath, this);
		m_fastDlg->Create(IDD_DLGFAST);
	}
	m_fastDlg->ShowWindow(SW_SHOWNORMAL);
}

//程序打开时创建一个默认的INI文件
bool CPassManagerDlg::CreateDefaultIniFile()
{
	TCHAR szFilePathName[MAX_PATH];
	GetModuleFileName(NULL, szFilePathName, MAX_PATH);
	m_iniFilePath = szFilePathName;
	m_iniFilePath = m_iniFilePath.Left(m_iniFilePath.ReverseFind('\\'));
	m_iniFilePath += "\\Setting.ini";
	CFileFind finder;
	BOOL ifFind = finder.FindFile(m_iniFilePath);
	if( !ifFind )
	{
		CString strHotKey;
		DWORD hotKey;
		hotKey = (MOD_ALT << 16) | ('Q');
		strHotKey.Format("%ld", hotKey);
		WritePrivateProfileString(_T("HOTKEY"),_T("SHOW"),
			strHotKey, m_iniFilePath);
		hotKey = (MOD_ALT << 16) | ('E');
		strHotKey.Format("%ld", hotKey);
		WritePrivateProfileString(_T("HOTKEY"),_T("SEARCH"),
			strHotKey, m_iniFilePath);
		BOOL rc = RegisterHotKey(this->GetSafeHwnd(), WM_HOTKEY_SHOW, MOD_ALT, 'Q');
		rc = RegisterHotKey(this->GetSafeHwnd(), WM_HOTKEY_SEARCH, MOD_ALT, 'E');
		if (!rc)
		{
			setStatusBar("热键注册失败");
		}
	}else
	{
		UINT showHotKey = GetPrivateProfileInt("HOTKEY", "SHOW", 0, m_iniFilePath);
		UINT searchHotKey = GetPrivateProfileInt("HOTKEY", "SEARCH", 0, m_iniFilePath);
		if (showHotKey && searchHotKey)
		{
			BOOL rc = RegisterHotKey(this->GetSafeHwnd(), WM_HOTKEY_SHOW, showHotKey >> 16, showHotKey & 0x0000FFFF);
			rc = RegisterHotKey(this->GetSafeHwnd(), WM_HOTKEY_SEARCH, searchHotKey >> 16, searchHotKey & 0x0000FFFF);
			if (!rc)
			{
				setStatusBar("热键注册失败");
			}
		}	
	}
	
	return true;
}

//点击托盘菜单的退出按钮
void CPassManagerDlg::OnTrayQuit()
{
	// TODO: 在此添加命令处理程序代码
	OnQuit();
}

BOOL CPassManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_HOTKEY)
	{
		switch (pMsg->wParam)
		{
		case WM_HOTKEY_SHOW:
			DoShowWindow(!m_bShow);
			break;
		case WM_HOTKEY_SEARCH:
			if (m_sql) ShowSearchDlg(!m_bSearchShow);
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//显示快速搜索对话框
void CPassManagerDlg::ShowSearchDlg(bool bShow)
{
	if (!m_SearchDlg || !m_SearchDlg->m_hWnd)
	{
		m_SearchDlg = new CSearchDlg(m_sql, this);
		m_SearchDlg->Create(IDD_SEARCHDLG);
	}
	if (bShow)
	{
		m_SearchDlg->ShowWindow(SW_SHOWNORMAL);
	} 
	else
	{
		m_SearchDlg->ShowWindow(SW_HIDE);
	}
	m_bSearchShow = bShow;
	CRect rect;
	m_SearchDlg->GetWindowRect(&rect);
	m_SearchDlg->MoveWindow(rect.left, rect.top, 150, 50);
	m_SearchDlg->SetFocus();
}

void CPassManagerDlg::OnQuery()
{
	// TODO: 在此添加命令处理程序代码
	if (m_sql) ShowSearchDlg(true);
}


void CPassManagerDlg::OnTraySearch()
{
	OnQuery();
	// TODO: 在此添加命令处理程序代码
}


void CPassManagerDlg::OnTrayOpen()
{
	DoShowWindow(TRUE);
	// TODO: 在此添加命令处理程序代码
}


void CPassManagerDlg::OnRepass()
{
	// TODO: 在此添加命令处理程序代码
	CRepassDlg repDlg(m_sql, this);
	repDlg.DoModal();
}


void CPassManagerDlg::OnBak()
{
	// TODO: 在此添加命令处理程序代码
	CBakDlg bakDlg(m_sql, this);
	bakDlg.DoModal();
}

//点击修改密码按钮
void CPassManagerDlg::OnMpopedit()
{
	// TODO: 在此添加命令处理程序代码
	m_bEdit = true;
	OnAdd();
	m_bEdit = false;
}

//点击删除密码按钮
void CPassManagerDlg::OnMpopdel()
{
	if (IDCANCEL == MessageBox("是否要删除此条密码", "提示", MB_OKCANCEL))
	{
		return ;
	}
	int at = m_IdArr.GetAt(m_listShow.GetSelectionMark());
	bool bOk = m_sql->delPass(at);
	if (!bOk)
	{
		setStatusBar("删除密码失败");
	}else{
		setStatusBar("删除密码成功");
		OnBnClickedBtnsearch();
	}
}


void CPassManagerDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	static CAboutDlg m_abt;
	if (!m_abt.m_hWnd)
	{
		m_abt.Create(IDD_ABOUTBOX);
	}
	m_abt.ShowWindow(SW_SHOWNORMAL);
}


void CPassManagerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_sql)
	{
		if (MessageBox("关闭当前密码本，并打开另一个一个密码本？", "提醒", MB_OKCANCEL) == IDCANCEL)
		{
			return ;
		}else{
			OnClosebook();
		}
	}
	char pFilePathName[MAX_PATH];
	::DragQueryFile(hDropInfo, 0, pFilePathName, MAX_PATH);  // 获取拖放文件的完整文件名
	m_dbPath = pFilePathName;
	::DragFinish(hDropInfo);   // 注意这个不能少，它用于释放Windows 为处理文件拖放而分配的内存

	CInputPassDlg passDlg(this);
	INT_PTR rt = passDlg.DoModal();
	if (!rt)
	{
		return ;
	}
	CString strPass = passDlg.getPassword();
	OpenPassBook(m_dbPath, strPass);

	CDialogEx::OnDropFiles(hDropInfo);
}

void CPassManagerDlg::OpenPassBook(CString strFile, CString strPass)
{
	m_sql = new doSql();
	if( ! m_sql->InitOpen((LPCTSTR)strFile, (LPCTSTR)strPass, strPass.GetLength())){
		int errNo;
		CString str;
		errNo = m_sql->getErrNo();
		str.Format("出错，错误代码：%d", errNo);
		MessageBox(str);
		delete m_sql;
		m_sql = NULL;
		return ;
	}else{
		setStatusBar("打开密码本成功");
		EnableMenus();
		clearListEdit();
		m_sql->delMoreTag();
	}
}

void CPassManagerDlg::OnOption()
{
	// TODO: 在此添加命令处理程序代码
	COptionDlg dlg(m_iniFilePath, this);
	dlg.DoModal();
}

//双击某一项，打开密码详情窗口
void CPassManagerDlg::OnNMDblclkRelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_listShow.GetSelectedCount() > 0)
	{
		int nSelect = m_listShow.GetSelectionMark();
		CString strPass, strUser, strUrl, strMak;
		strUser = m_listShow.GetItemText(nSelect, 0);
		strPass = m_listShow.GetItemText(nSelect, 1);
		strUrl = m_listShow.GetItemText(nSelect, 3);
		strMak = m_listShow.GetItemText(nSelect, 2);
		CDetailDlg dlg(strUser, strPass, strUrl, strMak, this);
		dlg.DoModal();
	}
	*pResult = 0;
}

//点击菜单上的帮助按钮
void CPassManagerDlg::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(0, _T("open"), HelpSite, NULL, NULL, SW_SHOWMAXIMIZED);
}

//点击菜单上的离别歌按钮，进入我的主页
void CPassManagerDlg::OnLeavesongs()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(0, _T("open"), Author, NULL, NULL, SW_SHOWMAXIMIZED);
}
