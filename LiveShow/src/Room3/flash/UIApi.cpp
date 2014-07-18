/********************************************************************
	created:	2006/04/12
	created:	12:4:2006   16:41
	file base:	UIString
	file ext:	cpp
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "UIApi.h"

//////////////////////////////////////////////////////////////////////////
namespace DirectUI
{
	void UIGetPlainText(LPTSTR pstrText, char chStart /* = '<' */, char chEnd /* = '>' */)
	{	
		if(pstrText == NULL)
			return;
		
		LPTSTR psSch = (LPTSTR)pstrText;
		LPTSTR psRes = (LPTSTR)pstrText;
		
		while( *psSch != '\0') 
		{
			if(*psSch != chStart)
			{
				*psRes++ = *psSch++;
			}
			else
			{
				while( *psSch != '\0' && *psSch != chEnd)
				{
					psSch++;
				}
				
				psSch++;
			}
		}
		
		*psRes = '\0';
	}
	
	BOOL UIFlashWindowEx(HWND hWnd, DWORD dwTimeout /* = 0 */, UINT uCount /* = -1 */, DWORD dwFlags /* = FLASHW_TIMERNOFG | FLASHW_TRAY | FLASHW_CAPTION */)
	{
		FLASHWINFO fl = {0};
		
		fl.cbSize = sizeof(FLASHWINFO);
		fl.hwnd = hWnd;
		fl.dwTimeout = dwTimeout;
		fl.uCount = uCount;
		fl.dwFlags = dwFlags;				
		
		return ::FlashWindowEx(&fl);
	}
	
#ifndef UNICODE
	//////////////////////////////////////////////////////////////////////////
	void BIG52GBK(char *szBuf)
	{
		if(!strcmp(szBuf, ""))
			return;
		
		int n_tcslen = _tcslen(szBuf);
		wchar_t *pws = new wchar_t[n_tcslen + 1];
		
		int nReturn = MultiByteToWideChar(950, 0, szBuf, n_tcslen, pws, n_tcslen + 1);
		BOOL bValue = false;
		nReturn = WideCharToMultiByte(936, 0, pws, nReturn, szBuf, n_tcslen + 1, "?", &bValue);
		szBuf[nReturn] = 0;
	}
	//---------------------------------------------------------------------------
	// GBK转大五码
	// 中華人民共和國 --> い地チ㎝瓣
	void GBK2BIG5(char *szBuf)
	{
		if(!strcmp(szBuf, ""))
			return ;
		
		int n_tcslen = _tcslen(szBuf);
		wchar_t *pws = new wchar_t[n_tcslen + 1];	
		
		MultiByteToWideChar(936, 0, szBuf, n_tcslen, pws, n_tcslen + 1);
		BOOL bValue = false;
		WideCharToMultiByte(950, 0, pws, n_tcslen, szBuf, n_tcslen + 1, "?", &bValue);
		szBuf[n_tcslen] = 0;
	}
	
	//----------------------------------------------------------------------------
	// GB2312码转GBK码
	// 中华人民共和国 --> 中華人民共和國
	void GB2GBK(char *szBuf)
	{
		if(!strcmp(szBuf, ""))
			return;
		
		int n_tcslen = _tcslen(szBuf);
		LCID wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
		int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, n_tcslen, NULL, 0);
		
		if(!nReturn)
			return;
		
		char *pcBuf = new char[nReturn + 1];
		
		wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
		LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
		
		strncpy(szBuf, pcBuf, nReturn);
	}
	
	//---------------------------------------------------------------------------
	// GBK码转GB2312码
	// 中華人民共和國 --> 中华人民共和国
	void GBK2GB(char *szBuf)
	{
		if(!strcmp(szBuf, ""))
			return;
		
		int n_tcslen = _tcslen(szBuf);
		
		LCID wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
		int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, n_tcslen, NULL, 0);
		
		if(!nReturn)
			return;
		
		char *pcBuf = new char[nReturn + 1];
		
		wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
		LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
		strncpy(szBuf, pcBuf, nReturn);
	}
	
#endif //UINCODE	
		
#define LY_PER_INCH   1440

	void CharFormatFromLogFont( CHARFORMATW &cf, const LOGFONT &lf )
	{
		// Set CHARFORMAT structure
		cf.cbSize = sizeof(CHARFORMAT2);

		HDC hdc = ::GetDC(HWND_DESKTOP);
		LONG yPixPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
		cf.yHeight = lf.lfHeight * LY_PER_INCH / yPixPerInch ;
		::ReleaseDC(NULL, hdc);

		cf.yOffset = 0;
		cf.crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);

		cf.dwEffects = CFM_EFFECTS | ~CFE_AUTOBACKCOLOR;
		cf.dwEffects &= ~(CFE_PROTECTED | CFE_LINK);

		if(lf.lfWeight < FW_BOLD)
			cf.dwEffects &= ~CFE_BOLD;
		if(!lf.lfItalic)
			cf.dwEffects &= ~CFE_ITALIC;
		if(!lf.lfUnderline)
			cf.dwEffects |= CFE_UNDERLINE;
		if(!lf.lfStrikeOut)
			cf.dwEffects &= ~CFE_STRIKEOUT;

		cf.dwMask = CFM_ALL | CFM_BACKCOLOR;
		cf.bCharSet = lf.lfCharSet;
		cf.bPitchAndFamily = lf.lfPitchAndFamily;

#ifdef UNICODE
		_tcscpy(cf.szFaceName, lf.lfFaceName);
#else
		//need to thunk cf.szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
		MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, cf.szFaceName, LF_FACESIZE) ;
#endif
	}

	LRESULT MapHResultToLResult(HRESULT hr, UINT msg)
	{
		LRESULT lres = hr;

		switch(msg)
		{
		case EM_GETMODIFY:
			lres = (hr == S_OK) ? -1 : 0;

			break;

			// These messages must return TRUE/FALSE rather than an hresult.
		case EM_UNDO:
		case WM_UNDO:
		case EM_CANUNDO:
		case EM_CANPASTE:
		case EM_LINESCROLL:

			// Hresults are backwards from TRUE and FALSE so we need
			// to do that remapping here as well.

			lres = (hr == S_OK) ? TRUE : FALSE;

			break;

		case EM_EXLINEFROMCHAR:
		case EM_LINEFROMCHAR:

			// If success, then hr a number. If error, it s/b 0.
			lres = SUCCEEDED(hr) ? (LRESULT) hr : 0;
			break;

		case EM_LINEINDEX:

			// If success, then hr a number. If error, it s/b -1.
			lres = SUCCEEDED(hr) ? (LRESULT) hr : -1;
			break;	

		default:
			lres = (LRESULT) hr;		
		}

		return lres;
	}

	LPCTSTR GetIIDString(REFIID iid)
	{
		//USES_CONVERSION;
		static TCHAR szIID[100];
		szIID[0]			= 0;
		DWORD dwSize		= sizeof(szIID);
		HKEY hKey			= NULL;
		LPOLESTR pszGUID	= NULL;

		do 
		{
			if(!SUCCEEDED(StringFromCLSID(iid, &pszGUID)))
				break;
			if(!pszGUID)
				break;
			CString strGUID(pszGUID);
			// Attempt to find it in the Interfaces section
			if ((-1 != _stprintf_s(szIID,_countof(szIID), _T("Interface\\%s"), (LPCTSTR)strGUID))
				&& (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, szIID, 0, KEY_READ, &hKey)))
			{
				RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)szIID, &dwSize);
				RegCloseKey(hKey);
			}
			// Attempt to find it in the CLSID section
			else
			{
				if ((-1 != _stprintf_s(szIID,_countof(szIID), _T("CLSID\\%s"), (LPCTSTR)strGUID))
					&& (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, szIID, 0, KEY_READ, &hKey)))
				{
					RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)szIID, &dwSize);
					RegCloseKey(hKey);
				}
				else
				{
					_tcscpy_s(szIID, _countof(szIID), (LPCTSTR)strGUID);
				}
			}
			CoTaskMemFree(pszGUID);		
		} while (0);

		return szIID;
	}
}