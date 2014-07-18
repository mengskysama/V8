// Skin.h : main header file for the Skin DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#ifdef _USRDLL

// CSkinApp
// See Skin.cpp for the implementation of this class
//

class CSkinApp : public CWinApp
{
public:
	CSkinApp();
	~CSkinApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

#endif //_USRDLL
