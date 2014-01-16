#pragma once

#include "sqlite/dosql.h"

// CRepassDlg 对话框

class CRepassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRepassDlg)

public:
	CRepassDlg(doSql * sql, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRepassDlg();

// 对话框数据
	enum { IDD = IDD_REPDLG };
	
private:
	doSql * m_sql;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strPass;
public:
	afx_msg void OnBnClickedRepdlgOk();
};
