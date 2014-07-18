///////////////////////////////////////////////////////////////////////////////
//
//  Module: Utility.cpp
//
//    Desc: See Utility.h
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////
#include "../../../stdafx.h"
#include "Utility.h"
#include "..\..\..\resource.h"

FILETIME CUtility::getLastWriteFileTime(const CString &sFile)
{
   FILETIME          ftLocal = {0};
   HANDLE            hFind;
   WIN32_FIND_DATA   ff32;
   hFind = FindFirstFile(sFile, &ff32);
   if (INVALID_HANDLE_VALUE != hFind)
   {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftLocal);
      FindClose(hFind);        
   }
   return ftLocal;
}


CString CUtility::getAppName()
{
   TCHAR szFileName[_MAX_PATH];
   GetModuleFileName(NULL, szFileName, _MAX_FNAME);

   CString sAppName; // Extract from last '\' to '.'
   sAppName = szFileName;
   sAppName = sAppName.Mid(sAppName.ReverseFind(_T('\\')) + 1)
                      .SpanExcluding(_T("."));

   return sAppName;
}


CString CUtility::getTempFileName()
{
   TCHAR szTempDir[MAX_PATH - 14]   = _T("");
   TCHAR szTempFile[MAX_PATH]       = _T("");

   if (GetTempPath(MAX_PATH - 14, szTempDir))
      GetTempFileName(szTempDir, getAppName(), 0, szTempFile);

   return szTempFile;
}


CString CUtility::getSaveFileName()
{
   CString sFilter((LPCTSTR)IDS_ZIP_FILTER);

   CString temp=getAppName();
   temp.Append(".rar");

   CFileDialog fd(
      FALSE, 
      _T("zip"), 
     temp, 
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
      sFilter);

	if (IDOK == fd.DoModal())
   {
      DeleteFile(fd.GetPathName());  // Just in-case it already exist
      return fd.GetPathName();
   }

   return _T("");
}

CString CUtility::GetAppPath()
{
	// Get the path.
	CString strAppPath;
	TCHAR szPath[_MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	DWORD dwLen = GetLongPathName(szPath, NULL, 0);
	GetLongPathName(szPath, strAppPath.GetBuffer(dwLen + 1), dwLen + 1);	
	strAppPath.ReleaseBuffer();

	int iPos = strAppPath.ReverseFind(_T('\\'));
	if (-1 != iPos)
	{
		strAppPath = strAppPath.Left(iPos + 1);
	}

	SetCurrentDirectory(strAppPath);

	return strAppPath;
}
