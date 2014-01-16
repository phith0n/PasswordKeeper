#pragma once


// CInputPassDlg 对话框

class CInputPassDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputPassDlg)

public:
	CInputPassDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputPassDlg();

// 对话框数据
	enum { IDD = IDD_INPUTPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPasscancel();
	CString CInputPassDlg::getPassword();

private:
	CString m_strPass;
public:
	afx_msg void OnBnClickedPassok();
};
