#pragma once
#include "afxwin.h"


// COptionDlg 对话框

class COptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CString strIniName, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COptionDlg();

// 对话框数据
	enum { IDD = IDD_OPTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	int COptionDlg::SetAutoRun(int bAutoRun);
	int COptionDlg::SetFileRele(int bRele);
	CString m_IniFileName;
	CString m_errStr;
	bool bAutoChng;
	bool bReleChng;
	bool bInitAuto;
	bool bInitRele;
	bool COptionDlg::setFileAssociation(CString ext,  //后缀“.pas”
		CString applicationName,  //程序地址
		CString progId, // 一个名字"PK_File"
		CString defaultIcon,  //默认图标
		CString description); //描述
	bool COptionDlg::isAutoRun();
	bool COptionDlg::isReleFile();
	bool COptionDlg::delFileAssociation();

public:
	afx_msg void OnBnClickedOptiondlgCancel();
	afx_msg void OnBnClickedOptiondlgOk();
private:
	CButton m_AutoRun;
	//CButton m_HidePass;
	CButton m_RelePas;
//	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOptiondlgConn();
	afx_msg void OnBnClickedOptiondlgAutorun();
};
