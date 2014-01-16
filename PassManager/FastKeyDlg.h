#pragma once
#include "afxcmn.h"


// CFastKeyDlg 对话框

class CFastKeyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFastKeyDlg)

public:
	CFastKeyDlg(CString strIniName, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFastKeyDlg();

// 对话框数据
	enum { IDD = IDD_DLGFAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CWnd * m_Parent;
	CString m_strIniPath;

public:
	CHotKeyCtrl m_ShowKey;
	afx_msg void OnBnClickedFastdlgOk();
	afx_msg void OnBnClickedFastdlgCancel();
	CHotKeyCtrl m_SearchKey;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFastdlgClear();
};
