#pragma once


// CFileDialogEx

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(BOOL bOpenFileDialog, // 对于 FileOpen 为 TRUE，对于 FileSaveAs 为 FALSE
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CFileDialogEx();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnFileNameOK();
};


