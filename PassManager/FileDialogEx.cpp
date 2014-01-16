// FileDialogEx.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "FileDialogEx.h"


// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

CFileDialogEx::~CFileDialogEx()
{
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
END_MESSAGE_MAP()



// CFileDialogEx 消息处理程序




BOOL CFileDialogEx::OnFileNameOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFileDialog::OnFileNameOK();
}
