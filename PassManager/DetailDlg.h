#pragma once


// CDetailDlg 对话框

class CDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailDlg)

public:
	CDetailDlg(CString strUser, CString strPass, CString strUrl, CString strMak, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetailDlg();

// 对话框数据
	enum { IDD = IDD_DETAILDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_strUser;
	CString m_strPass;
	CString m_strUrl;
	CString m_strMak;
	CWnd * pPar;

	void CDetailDlg::CopyStr(CString str);

public:
	afx_msg void OnBnClickedDetailReturn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtndetailUser();
	afx_msg void OnBnClickedBtndetailPass();
	afx_msg void OnBnClickedBtndetailUrl();
	afx_msg void OnBnClickedBtndetailMak();
};
