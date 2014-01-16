
// PassManagerDlg.h : 头文件
//

#pragma once

#include "RepassDlg.h"
#include "SearchDlg.h"
#include "FastKeyDlg.h"
#include "AddPassDlg.h"
#include "sqlite/dosql.h"
#include "afxcmn.h"
#include "afxwin.h"

// CPassManagerDlg 对话框
class CPassManagerDlg : public CDialogEx
{
// 构造
public:
	CPassManagerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPassManagerDlg(){
		if (m_sql) delete m_sql;
		if (m_addPassDlg && m_addPassDlg->m_hWnd) delete m_addPassDlg;
		if (m_fastDlg && m_fastDlg->m_hWnd) delete m_addPassDlg;
		if (m_SearchDlg && m_SearchDlg->m_hWnd) delete m_SearchDlg;
	}

// 对话框数据
	enum { IDD = IDD_PASSMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	HMENU mainMenu;
	CString m_dbPath;
	doSql * m_sql;
	CListCtrl m_listShow;
	CStatusBar m_bar;
	CAddPassDlg * m_addPassDlg;
	CMenu m_PopMenu;
	bool m_bShow;
	CFastKeyDlg * m_fastDlg;
	CString m_iniFilePath;
	CSearchDlg * m_SearchDlg;
	bool m_bEdit;
	CArray<int, int> m_IdArr;

	void CPassManagerDlg::InitList();
	void CPassManagerDlg::setStatusBar(CString str);
	void CPassManagerDlg::EnableMenus();
	void CPassManagerDlg::EnableDlg(int name, BOOL bEnable = TRUE);
	void CPassManagerDlg::EnableMenu(CMenu * pMenu, int refer, UINT name, BOOL bEnable = TRUE);
	void CPassManagerDlg::DisableMenus();
	BOOL CPassManagerDlg::TrayMyIcon(bool bShow);
	LRESULT CPassManagerDlg::OnTrayCallBackMsg(WPARAM wparam, LPARAM lparam);
	VOID CPassManagerDlg::PopNotifyMenu();
	void CPassManagerDlg::DoShowWindow(bool bShow);
	bool CPassManagerDlg::CreateDefaultIniFile();
	void CPassManagerDlg::ShowSearchDlg(bool bShow);
	void CPassManagerDlg::clearListEdit();
	void CPassManagerDlg::OpenPassBook(CString strFile, CString strPass);


public:
	bool m_bSearchShow;
	afx_msg void OnNewbook();
	afx_msg void OnOpenbook();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnQuit();
	afx_msg void OnNMRClickRelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAdd();
	void CPassManagerDlg::OnSonDlgClose(CString strStatus);
	afx_msg void OnBnClickedBtnsearch();
private:
	CString m_strSearch;
public:
	afx_msg void OnPopcreate();
	afx_msg void OnPopopen();
	afx_msg void OnMpopadd();
	afx_msg void OnMpopcopy();
	afx_msg void OnClosebook();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnFast();
	afx_msg void OnTrayQuit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnQuery();
	afx_msg void OnTraySearch();
	afx_msg void OnTrayOpen();
	afx_msg void OnRepass();
	afx_msg void OnBak();
	afx_msg void OnMpopedit();
	afx_msg void OnMpopdel();
private:
	CEdit m_EditSearch;
public:
	afx_msg void OnAbout();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnOption();
	afx_msg void OnNMDblclkRelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHelp();
	afx_msg void OnLeavesongs();
};
