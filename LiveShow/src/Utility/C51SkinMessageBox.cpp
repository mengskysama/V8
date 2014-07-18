#include "StdAfx.h"
#include "Resource.h"
#include "C51SkinMessageBox.h"

int C51SkinMessageBox::Show(LPCTSTR pzPrompt)
{
    return ::MessageBox(NULL, pzPrompt, _T("提示"), MB_OK);
	//CSkinMessageBox box(pzPrompt, _T("提示"), MB_OK);
	//return box.Show();
}

int C51SkinMessageBox::Show(CWnd* pParent, LPCTSTR pzPrompt)
{
    HWND hParent = pParent ? pParent->GetSafeHwnd() : NULL;
    return ::MessageBox(hParent, pzPrompt, _T("提示"), MB_OK);
	//CSkinMessageBox box(pParent, pzPrompt, _T("提示"), MB_OK);
	//return box.Show();
}

int C51SkinMessageBox::Show(HWND hParent, LPCTSTR pzPrompt)
{
    return ::MessageBox(hParent, pzPrompt, _T("提示"), MB_OK);
	//if (! ::IsWindow(hParent))
	//	return IDCLOSE;
	//return Show(CWnd::FromHandle(hParent), pzPrompt);
}

int C51SkinMessageBox::Show(LPCTSTR pzPrompt, LPCTSTR pzTitle, UINT uType)
{
    if (pzTitle == NULL)
        pzTitle = _T("提示");
    return ::MessageBox(NULL, pzPrompt, pzTitle, uType);
	//CSkinMessageBox box(pzPrompt, pzTitle, uType);
	//return box.Show();
}

int C51SkinMessageBox::Show(CWnd* pParent, LPCTSTR pzPrompt, LPCTSTR pzTitle, UINT uType)
{
    if (pzTitle == NULL)
        pzTitle = _T("提示");
    HWND hParent = pParent ? pParent->GetSafeHwnd() : NULL;
    return ::MessageBox(hParent, pzPrompt, pzTitle, uType);
	//CSkinMessageBox box(pParent, pzPrompt, pzTitle, uType);
	//return box.Show();
}

int C51SkinMessageBox::Show(HWND hParent, LPCTSTR pzPrompt, LPCTSTR pzTitle, UINT uType)
{
    if (pzTitle == NULL)
        pzTitle = _T("提示");
    return ::MessageBox(hParent, pzPrompt, pzTitle, uType);
	//if (! ::IsWindow(hParent))
	//	return IDCLOSE;
	//return Show(CWnd::FromHandle(hParent), pzPrompt, pzTitle, uType);
}
