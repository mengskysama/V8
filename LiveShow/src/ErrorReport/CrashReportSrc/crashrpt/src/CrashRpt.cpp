///////////////////////////////////////////////////////////////////////////////
//
//  Module: CrashRpt.cpp
//
//    Desc: See CrashRpt.h
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#include "../../../stdafx.h"
#include "../../../include/crashrpt.h"
#include "CrashHandler.h"

REPORTOPTION g_ReportOption;

#ifdef _DEBUG
#define CRASH_ASSERT(pObj)          \
   if (!pObj || sizeof(*pObj) != sizeof(CCrashHandler))  \
      DebugBreak()                                       
#else
#define CRASH_ASSERT(pObj)
#endif // _DEBUG
CCrashHandler * g_pImpl=NULL;
CRASHRPTAPI LPVOID InstallCrashRpt(LPCTSTR lpcszTo, 
						   LPCTSTR lpcszSubject, 
						   LPCTSTR lpAppName,				
						   LPCTSTR lpVersion )
{
	if(g_pImpl) delete g_pImpl;
		g_pImpl=NULL;

	CCrashHandler *pImpl = new CCrashHandler(lpcszTo, lpcszSubject, lpAppName, lpVersion);
	CRASH_ASSERT(pImpl);
	g_pImpl=pImpl;

	OutputDebugString("Install\n");

   return pImpl;
}

VOID ReInstall(/*LPGETLOGFILE pfn, LPCTSTR lpcszTo, LPCTSTR lpcszSubject*/)
{
	if(g_pImpl)
	{
		LPGETLOGFILE pfn=g_pImpl->m_lpfnCallback;
		CString sTo=g_pImpl->m_sTo;
		CString sSubject=g_pImpl->m_sSubject;
		delete g_pImpl;
		CCrashHandler *pImpl = new CCrashHandler(sTo, sSubject);
		CRASH_ASSERT(pImpl);
		g_pImpl=pImpl;
	}
}

CRASHRPTAPI void UninstallCrashRpt(LPVOID lpState)
{
   CCrashHandler *pImpl = (CCrashHandler*)lpState;
   CRASH_ASSERT(pImpl);

   delete pImpl;
}

CRASHRPTAPI void AddFile(LPVOID lpState, LPCTSTR lpFile, LPCTSTR lpDesc)
{
   CCrashHandler *pImpl = (CCrashHandler*)lpState;
   CRASH_ASSERT(pImpl);

   pImpl->AddFile(lpFile, lpDesc);
}

CRASHRPTAPI void GenerateErrorReport(LPVOID lpState, PEXCEPTION_POINTERS pExInfo)
{
   CCrashHandler *pImpl = (CCrashHandler*)lpState;
   CRASH_ASSERT(pImpl);

   pImpl->GenerateErrorReport(pExInfo);
}

CString XORCode(const CString & str)
{
	int len=str.GetLength();
	CString strRet;
	LPTSTR szBuf=strRet.GetBuffer(len+2);
	LPCTSTR szSrc=str;

	int i;
	for(i=0;i<len;i++)
	{
		TCHAR c=szSrc[i]^len;
		szBuf[i]=c?c:szSrc[i];
	}
	szBuf[i]=0;
	strRet.ReleaseBuffer();

	return strRet;
}

CRASHRPTAPI bool SetReportOptions(int nType,LPVOID lpData)
{
	switch(nType)
	{
	case SRO_SMTP_SVR:
		g_ReportOption.strSmtp=(LPCTSTR)lpData;
		break;
	case SRO_DLG_TITLE:
		g_ReportOption.strDlgTitle=(LPCTSTR)lpData;
		break;
	case SRO_USERNAME:
		g_ReportOption.strUsername=(LPCTSTR)lpData;//XORCode((LPCTSTR)lpData);
		break;
	case SRO_PASSWORD:
		g_ReportOption.strPassword=(LPCTSTR)lpData;//XORCode((LPCTSTR)lpData);
		break;
	case SRO_BODY:
		g_ReportOption.strBody=(LPCTSTR)lpData;
	case SRO_ALL_SEND:
		g_ReportOption.bSendAll=(bool)lpData;
		break;
	case SRO_SILENT:
		g_ReportOption.bSilent=(bool)lpData;
		break;
	case SRO_EXTERNAL_RPT:
		g_ReportOption.strExternalRpt=(LPCTSTR)lpData;
		break;
	case SRO_ALIAS:
		g_ReportOption.strAlias=(LPCTSTR)lpData;
		break;
	case SRO_CALLBACK:
		g_ReportOption.func=(ErrorReportCallback)lpData;
		break;
	default:
		return false;
	}
	return true;
}