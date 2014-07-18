#include "../StdAfx.h"
#include "../../include/ui/C179MsgBox.h"
#include "../../include/ui/MsgBox.h"
#include "../../include/utility/SystemHelper.h"

#define  MSGBOX_STYLE INT

BOOL C179MsgBox::s_bChecked = TRUE;

BOOL C179MsgBox::IsMsgBoxVisible()
{
	return CMsgBox::GetMsgBoxVisible();
}

BOOL C179MsgBox::IsChecked()
{
	return s_bChecked;
}


////////////////////////////  新提示框接口  //////////////////////////////////
//无图标类
int C179MsgBox::Show( CWnd* pParent, MSGBOX_STYLE nStyle, LPCTSTR pzPrompt1, LPCTSTR pzPrompt2, LPCTSTR pzPrompt3, LPCTSTR linkLabel, LPCTSTR linkAddress, LPCTSTR pzTitle, LPCTSTR pzCheckBox)
{
	SwitchResourceToModule(_T("Hall"));
	if(!(pParent && pParent->GetSafeHwnd()) )
	{
		pParent = NULL;
	}
	CMsgBox mb(pParent);
	int nRet = mb.Show(nStyle, pzPrompt1, pzPrompt2, pzPrompt3, linkLabel, linkAddress, pzTitle, pzCheckBox);
	s_bChecked = mb.GetIsChecked();
	return nRet;
}

//有图标类
int C179MsgBox::Show(CWnd* pParent, MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1, LPCTSTR pzPrompt2, LPCTSTR pzTitle)
{	
	SwitchResourceToModule(_T("Hall"));
	if(!(pParent && pParent->GetSafeHwnd()) )
	{
		pParent = NULL;
	}
	CMsgBox mb(pParent);
	return mb.Show(nStyle, nIconType,  pzPrompt1, pzPrompt2, pzTitle);
}

int C179MsgBox::ShowForOrderSong( CWnd* pParent, MSGBOX_STYLE nStyle, LPCTSTR pzString1/*=_T("")*/, LPCTSTR pzString2/*=_T("")*/, LPCTSTR colorString1/*=_T("")*/, LPCTSTR colorString2/*=_T("")*/, LPCTSTR btnOKLabel/*=_T("?定")*/, LPCTSTR btnCancelLabel/*=_T("取消")*/, LPCTSTR pzTitle/*=_T("点歌结果")*/ )
{
	SwitchResourceToModule(_T("Hall"));
	if(!(pParent && pParent->GetSafeHwnd()) )
	{
		pParent = NULL;
	}
	CMsgBox mb(pParent);
	return mb.ShowForOrderSong(nStyle, pzString1, pzString2, colorString1, colorString2, btnOKLabel, btnCancelLabel, pzTitle);
}
//非模态类
CMsgBox* C179MsgBox::ShowNonDomodal(CWnd* pParent, MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1/* =_T */, LPCTSTR pzTitle)
{
	SwitchResourceToModule(_T("Hall"));
	if(!(pParent && pParent->GetSafeHwnd()) )
	{
		pParent = NULL;
	}

	CMsgBox* pDlg = new CMsgBox(pParent);
	pDlg->Create(CMsgBox::IDD, pParent);
	pDlg->ShowNonDomodal(nStyle, nIconType, pzPrompt1, pzTitle);
	return pDlg;
}