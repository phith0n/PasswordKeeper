#pragma once

#include "afxwin.h"

#include "sqlite/dosql.h"

//用户输入的信息
class InputInfo{
public:
	CString strPass;
	CString strTag;
	CString strUrl;
	CString strRemark;

	InputInfo(){

	}

	InputInfo(CString strPass, CString strTag, CString strUrl, CString strRemark){
		this->strPass = strPass;
		this->strTag = strTag;
		this->strUrl = strUrl;
		this->strRemark = strRemark;
	}
};

// CAddPassDlg 对话框

class CAddPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPassDlg)

public:
	CAddPassDlg(doSql * sql, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddPassDlg();

// 对话框数据
	enum { IDD = IDD_DLGADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdddlgCancel();
	virtual BOOL OnInitDialog();

private:
	CRect m_bmpRect;
	CWnd * m_pSeed;
	int m_seed;
	doSql * m_sql;
	CWnd * m_Parent;
	CString m_strStatus;

	void CAddPassDlg::SelectBox(int name, BOOL select = TRUE);
	CString CAddPassDlg::DelRepet(TCHAR * szStr, int len);
	void CAddPassDlg::setRandSeed();

public:
	afx_msg void OnBnClickedAdddlgChkengsmall();
	afx_msg void OnBnClickedAdddlgChkenglarge();
	afx_msg void OnBnClickedAdddlgChknum();
	afx_msg void OnBnClickedAdddlgSig();
	afx_msg void OnStnDblclickBmppass();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	InputInfo CAddPassDlg::getInput();
	bool m_bEdit;
	int EditPid;
	
private:
	CString m_strPass;
	CString m_strTag;
	CString m_strUrl;
	CString m_strRemark;
	CString m_strUser;

public:
	afx_msg void OnBnClickedAdddlgOk();
	afx_msg void OnClose();
	afx_msg void OnPaint();
};