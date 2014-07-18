/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLEntityResolver.cpp
 *	
 *	Written By Gurmeet S. Kochar <gomzygotit@hotmail.com>
 *	Copyright (c) 2004. All rights reserved.
 *
 *	This code may be used in compiled form in any way you desire 
 *	(including commercial use). The code may be redistributed 
 *	unmodified by any means PROVIDING it is not sold for profit 
 *	without the authors written consent, and providing that this 
 *	notice and the authors name and all copyright notices remains 
 *	intact. However, this file and the accompanying source code may 
 *	not be hosted on a website or bulletin board without the authors 
 *	written permission.
 *	
 *	This file is provided "AS IS" with no expressed or implied warranty.
 *	The author accepts no liability for any damage/loss of business that
 *	this product may cause.
 *
 *	Although it is not necessary, but if you use this code in any of 
 *	your application (commercial or non-commercial), please INFORM me 
 *	so that I may know how useful this library is. This will encourage 
 *	me to keep updating it.
 */
#include "stdafx.h"
#include "LiteHTMLEntityResolver.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG


	UINT CLiteHTMLEntityResolver::resolveEntity(LPCTSTR lpszEntity, TCHAR &chSubst)
	{
		ASSERT(m_CharEntityRefs.GetCount());
		ASSERT(AfxIsValidString(lpszEntity));

		LPCTSTR	lpszBegin = lpszEntity, 
			lpszEnd = ::_tcschr(lpszEntity, _T(';'));
		TCHAR	chTemp = 0;

		// entity references always end with a semi-colon ';'
		if (lpszEnd == NULL)
			return (0);

		// skip leading white-space characters
		while (::_istspace(*lpszBegin))
			lpszBegin = ::_tcsinc(lpszBegin);

		// remaining string (including semi-colon) 
		// must be at least 4 characters in length
		if (lpszEnd - lpszBegin < 3)
			return (0U);

		// entity references always begin with an ampersand '&' symbol
		if (*lpszBegin != _T('&'))
			return (0U);
		lpszBegin = ::_tcsinc(lpszBegin);

		// numeric (decimal or hexadecimal) entity reference?
		if (*lpszBegin == _T('#'))
		{
			lpszBegin = ::_tcsinc(lpszBegin);
			chTemp = *lpszBegin;
			int	radix = (::_istdigit(chTemp) ? 10 : 
			(chTemp == _T('x') || 
				chTemp == _T('X') ? 16 : 0));
			if (radix)
			{
				if (radix == 16)
					lpszBegin = ::_tcsinc(lpszBegin);

				unsigned long	ulNum = ::_tcstoul(lpszBegin, NULL, radix);
				chSubst = (TCHAR)ulNum;
				lpszEnd = ::_tcsinc(lpszEnd);
				return (lpszEnd - lpszEntity);
			}
		}

		// character entity reference?
		else
		{
			CString	strKey(lpszBegin, lpszEnd - lpszBegin);

			// because some character entity references are 
			// case-sensitive, we must fix them manually
			if (!strKey.CompareNoCase(_T("eth")) || 
				!strKey.CompareNoCase(_T("thorn")))
			{
				if (::_istupper(strKey[0]))
					strKey.MakeUpper();
				else
					strKey.MakeLower();
			}
			else if (!strKey.CompareNoCase(_T("Oslash")))
			{
				strKey.MakeLower();
				strKey.SetAt(0, _T('O'));
			}
			else if (!strKey.CompareNoCase(_T("AElig")))
			{
				strKey.MakeLower();
				strKey.SetAt(0, _T('A'));
				strKey.SetAt(1, _T('E'));
			}
			else
			{
				CString	strT = strKey.Mid(1);
				strKey.MakeLower();
				if (strT.CompareNoCase(_T("grave")) == 0 || 
					strT.CompareNoCase(_T("acute")) == 0 || 
					strT.CompareNoCase(_T("circ"))  == 0 || 
					strT.CompareNoCase(_T("uml"))   == 0 || 
					strT.CompareNoCase(_T("tilde")) == 0 || 
					strT.CompareNoCase(_T("cedil")) == 0 || 
					strT.CompareNoCase(_T("ring"))  == 0)
				{
					strKey.SetAt(0, strT[0]);
				}
			}

			// is this a known entity reference?
			if (m_CharEntityRefs.Lookup(strKey, chTemp))
			{
				chSubst = chTemp;
				lpszEnd = ::_tcsinc(lpszEnd);
				return (lpszEnd - lpszEntity);
			}
		}

		return (0U);
	}


#pragma warning(push, 4)
CLiteHTMLEntityResolver::CCharEntityRefs  CLiteHTMLEntityResolver::m_CharEntityRefs;
#pragma warning(pop)
