// SearchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include "PassManagerDlg.h"


// CSearchDlg 对话框

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(doSql * sql, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchDlg::IDD, pParent)
	, m_strSearch(_T(""))
{
	m_sql = sql;
}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEARCHDLG_INPUT, m_strSearch);
	DDX_Control(pDX, IDC_SEARCHDLG_LIST, m_SearchList);
	DDX_Control(pDX, IDC_SEARCHDLG_INPUT, m_EditInput);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSearchDlg 消息处理程序

BOOL CSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN:
				SearchPass();
				m_EditInput.SetSel(0, -1);
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//搜索密码
void CSearchDlg::SearchPass()
{
	m_passArr.RemoveAll();
	m_SearchList.ResetContent();
	InfoRow * info = NULL;
	UpdateData(TRUE);
	if (m_strSearch.IsEmpty()) return ;
	int count = m_sql->SearchPass(m_strSearch.GetBuffer(), &info);
	for (int i = 0 ; i < count ; i++)
	{
		CString str;
		str.Format("%s|%s", info[i].szUser, info[i].szPass);
		m_SearchList.InsertString(i, str.GetBuffer());
		str = info[i].szPass;
		m_passArr.Add(str);
	}
	CRect rect;
	int Height = count > 10 ? 200 : 50 + count * 15;
	this->GetWindowRect(&rect);
	this->MoveWindow(rect.left, rect.top, 220, Height);
	delete [] info;
}

void CSearchDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CEdit * p = (CEdit *)GetDlgItem(IDC_SEARCHDLG_INPUT);
	p->SetSel(0, -1);
	SetForegroundWindow();
}


void CSearchDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CDialogEx::OnClose();
	CPassManagerDlg * p = (CPassManagerDlg *)GetParent();
	p->m_bSearchShow = false;
	this->ShowWindow(SW_HIDE);
}


BOOL CSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect;
	this->GetWindowRect(&rect);
	this->MoveWindow(rect.left, rect.top, 220, rect.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
