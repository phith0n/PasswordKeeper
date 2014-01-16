// ListBoxEx.cpp : 实现文件
//

#include "stdafx.h"
#include "PassManager.h"
#include "ListBoxEx.h"
#include "SearchDlg.h"

// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

CListBoxEx::CListBoxEx()
{
	m_PopMenu.LoadMenu(IDR_MENU_SEARCH);
}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SEARCH_COPY, &CListBoxEx::OnSearchCopy)
END_MESSAGE_MAP()



// CListBoxEx 消息处理程序




void CListBoxEx::OnRButtonUp(UINT nFlags, CPoint point)
{
	int iSelect = this->GetCurSel();
	CMenu * pMenu = NULL;
	CPoint mousept;
	GetCursorPos(&mousept);
	pMenu = m_PopMenu.GetSubMenu(0);
	if (LB_ERR == iSelect)
	{
	pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
	}
	else
	{
	pMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	}
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, mousept.x, mousept.y, this);
	CListBox::OnRButtonUp(nFlags, point);
}


void CListBoxEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	CListBox::OnRButtonDown(nFlags, point);
}


void CListBoxEx::OnSearchCopy()
{
	// TODO: 在此添加命令处理程序代码
	CString strInfo;
	int iSelect = this->GetCurSel();
	if (iSelect == LB_ERR)
	{
		return ;
	}
	CSearchDlg * p = (CSearchDlg *)GetParent();
	strInfo = p->m_passArr.GetAt(iSelect);
	if(!OpenClipboard())
	{
		return ;
	}
	EmptyClipboard();
	HANDLE hClip;
	hClip=GlobalAlloc(GMEM_MOVEABLE,strInfo.GetLength()+1);
	char *pBuf;
	pBuf=(char *)GlobalLock(hClip);
	strcpy(pBuf,strInfo.GetBuffer());
	GlobalUnlock(hClip);
	SetClipboardData(CF_TEXT,hClip);
	CloseClipboard();
}
