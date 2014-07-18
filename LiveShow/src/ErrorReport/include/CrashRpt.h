///////////////////////////////////////////////////////////////////////////////
//
//  Module: CrashRpt.h
//
//    Desc: Defines the interface for the CrashRpt.DLL.
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _CRASHRPT_H_
#define _CRASHRPT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>

// #ifndef _UNICODE
// #error "hello "
// #endif

// CrashRpt.h
#ifdef CRASHRPTAPI

// CRASHRPTAPI should be defined in all of the DLL's source files as
// #define CRASHRPTAPI extern "C" __declspec(dllexport)

#else

// This header file is included by an EXE - export
#define CRASHRPTAPI extern "C" __declspec(dllimport)

#endif



// Client crash callback
typedef BOOL (CALLBACK *LPGETLOGFILE) (LPVOID lpvState);

//-----------------------------------------------------------------------------
// Install
//    Initializes the library and optionally set the client crash callback and
//    sets up the email details.
//
// Parameters
//    pfn         Client crash callback
//    lpTo        Email address to send crash report
//    lpSubject   Subject line to be used with email
//
// Return Values
//    If the function succeeds, the return value is a pointer to the underlying
//    crash object created.  This state information is required as the first
//    parameter to all other crash report functions.
//
// Remarks
//    Passing NULL for lpTo will disable the email feature and cause the crash 
//    report to be saved to disk.
//
CRASHRPTAPI 
LPVOID 
InstallCrashRpt(
   IN LPCTSTR lpTo OPTIONAL = NULL,                    // Email:to
   IN LPCTSTR lpSubject OPTIONAL = NULL,               // Email:subject
   IN LPCTSTR lpAppName OPTIONAL = NULL,				// 应用程序的名称
   IN LPCTSTR lpVersion	OPTIONAL = NULL				// 版本号
   );

//-----------------------------------------------------------------------------
// Uninstall
//    Uninstalls the unhandled exception filter set up in Install().
//
// Parameters
//    lpState     State information returned from Install()
//
// Return Values
//    void
//
// Remarks
//    This call is optional.  The crash report library will automatically 
//    deinitialize when the library is unloaded.  Call this function to
//    unhook the exception filter manually.
//
CRASHRPTAPI 
void 
UninstallCrashRpt(
   IN LPVOID lpState                            // State from Install()
   );

//-----------------------------------------------------------------------------
// AddFile
//    Adds a file to the crash report.
//
// Parameters
//    lpState     State information returned from Install()
//    lpFile      Fully qualified file name
//    lpDesc      Description of file, used by details dialog
//
// Return Values
//    void
//
// Remarks
//    This function can be called anytime after Install() to add one or more
//    files to the generated crash report.
//
CRASHRPTAPI 
void 
AddFile(
   IN LPVOID lpState,                           // State from Install()
   IN LPCTSTR lpFile,                           // File name
   IN LPCTSTR lpDesc                            // File desc
   );

//-----------------------------------------------------------------------------
// GenerateErrorReport
//    Generates the crash report.
//
// Parameters
//    lpState     State information returned from Install()
//    pExInfo     Pointer to an EXCEPTION_POINTERS structure
//
// Return Values
//    void
//
// Remarks
//    Call this function to manually generate a crash report.
//
CRASHRPTAPI 
void 
GenerateErrorReport(
   IN LPVOID lpState,
   IN PEXCEPTION_POINTERS pExInfo OPTIONAL
   );

#define SRO_SMTP_SVR		1
#define SRO_DLG_TITLE		2
#define SRO_USERNAME		3
#define SRO_PASSWORD		4
#define SRO_BODY			5
#define SRO_ALL_SEND		6
#define SRO_SILENT			7
#define SRO_EXTERNAL_RPT	8
#define SRO_ALIAS			9
#define SRO_CALLBACK		10

typedef int (WINAPI *ErrorReportCallback)(int status);

CRASHRPTAPI 
bool 
SetReportOptions(
	IN int nType,
	IN LPVOID lpData
	);

LPVOID ReInstall(LPGETLOGFILE pfn, LPCTSTR lpcszTo, LPCTSTR lpcszSubject);

void  __declspec( dllexport ) SetLoginAccountInfo(unsigned int _179id);
void  __declspec( dllexport ) SetDumpVecAddr(void *p, void * p2);
void  __declspec( dllexport ) SetFlashVersion(unsigned int uin);

#endif


