/********************************************************************
	created:	2006/04/12
	created:	12:4:2006   16:41
	file base:	UIString
	file ext:	cpp
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#ifndef __DIRECTUI_API_H__
#define __DIRECTUI_API_H__

namespace DirectUI
{
	void UIGetPlainText(LPTSTR pstrText, char chStart = '<', char chEnd = '>');
	
#ifndef FLASHW_STOP
#define FLASHW_STOP         0
#define FLASHW_CAPTION      0x00000001
#define FLASHW_TRAY         0x00000002
#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
#define FLASHW_TIMER        0x00000004
#define FLASHW_TIMERNOFG    0x0000000C
#endif
	
	
	BOOL UIFlashWindowEx(HWND hWnd, DWORD dwTimeout = 0, UINT uCount = -1, DWORD dwFlags = FLASHW_TIMER | FLASHW_TRAY | FLASHW_CAPTION);
	
	
#define STARTFLASHWINDOW(hwnd)				DirectUI::UIFlashWindowEx(hwnd)
#define STOPFLASHWINDOW(hwnd)				DirectUI::UIFlashWindowEx(hwnd, 0, 0, FLASHW_STOP)
	
	
	//////////////////////////////////////////////////////////////////////////
#ifndef UNICODE
	void IG52GBK(char *szBuf);
	void GBK2BIG5(char *szBuf);
	void GB2GBK(char *szBuf);
	void GBK2GB(char *szBuf);
#endif
		
	//Char Format
	void CharFormatFromLogFont( CHARFORMATW &cf, const LOGFONT &lf );
	
	//
	LRESULT MapHResultToLResult(HRESULT hr, UINT msg);

	LPCTSTR GetIIDString(REFIID iid);

}

#endif //__DIRECTUI_API_H__
