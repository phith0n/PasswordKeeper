// RepassDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "PassManagerDlg.h"
#include "RepassDlg.h"
#include "afxdialogex.h"

// CRepassDlg 对话框

IMPLEMENT_DYNAMIC(CRepassDlg, CDialogEx)

CRepassDlg::CRepassDlg(doSql * sql, CWnd* pParent /*=NULL*/)
	: CDialogEx(CRepassDlg::IDD, pParent)
	, m_strPass(_T(""))
{
	m_sql = sql;
}

CRepassDlg::~CRepassDlg()
{
}

void CRepassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REPDLG_INPUT, m_strPass);
}


BEGIN_MESSAGE_MAP(CRepassDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REPDLG_OK, &CRepassDlg::OnBnClickedRepdlgOk)
END_MESSAGE_MAP()


// CRepassDlg 消息处理程序


void CRepassDlg::OnBnClickedRepdlgOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CPassManagerDlg * p = (CPassManagerDlg *)GetParent();
	int rc = m_sql->reKey(m_strPass.GetBuffer(), m_strPass.GetLength());
	m_strPass.ReleaseBuffer();
	if (rc)
	{
		p->OnSonDlgClose("修改密码成功，请牢记密码，忘记密码将无法找回！");
	}else
	{
		p->OnSonDlgClose("修改密码失败");
	}
	SendMessage(WM_CLOSE);
}
