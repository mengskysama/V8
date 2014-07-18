#ifndef _STANDARD_H_
#define _STANDARD_H_

// Import ADO stuff?
#ifdef _IMPORT_ADO_
#import "C:\Program Files\Common Files\system\ado\msado15.dll" rename("EOF", "EndOfFile")
#import "C:\Program Files\Common Files\system\ado\msadox.dll"  
#import "C:\Program Files\Common Files\System\ado\MSJRO.DLL" no_namespace
#endif

// We need this.
#define OEMRESOURCE
#undef new

// Include CRT headers.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// Include windows headers.
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <crtdbg.h>
#include <new.h>
#include <tchar.h>
#include <shlwapi.h>

// Don't care if the compiler won't inline
// functions. Also don't care about unused
// parameters;
#pragma warning( disable: 4710 4100 )

#ifdef _DEBUG
	// Replacements for the new and delete operators. These will report the
	// file name and line number where blocks of unfreed memory where
	// allocated.
	extern void * operator new( size_t nSize, const char *lpszFileName, int nLine );
	extern void operator delete( void *pData, const char *lpszFileName, int nLine );

	// Some defines used in the code on debug builds.
	#define DEBUG_NEW new(THIS_FILE,__LINE__)
	#define malloc(x) _malloc_dbg( x, _CLIENT_BLOCK, THIS_FILE, __LINE__ )
	#define free(x) _free_dbg( x, _CLIENT_BLOCK )

	// The _ASSERT macro from the "crtdbg.h" file
	// generates a warning 4127 when compiling at
	// warning level 4.
	#pragma warning( disable: 4127 )

	// Assert when the argument is zero.
	#define _ASSERT_VALID( x ) _ASSERT( x != NULL )

	// Assert when the expression return 0;
	#define _VERIFY( x ) _ASSERT( x )

	// Assert with descriptive error.
	#define _ASSERT_TEXT( x, t )\
		do { if ( ! (x ) &&\
			(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, t, 0 )))\
			_CrtDbgBreak(); } while ( 0 )

	// This will prevent the compiler from generating a default
	// copy constructor and = operator overload.
	#define _NO_COPY(c)		private: c(const c& src) {;} operator=(const c& src ) {;}
	#define _NO_COPY_PARAM(c,p)	private: c(const c& src) : p(0) {;} operator=(const c& src ) {;}
	
	// Calls the _CrtDumpMemoryLeaks() routine
	// asserts when a leak is detected.
	#define _TEST_HEAP\
	if ( _CrtDumpMemoryLeaks())\
		if (( 1 == _CrtDbgReport( _CRT_ERROR, __FILE__, __LINE__, NULL, "Memory leak detected!", 0 )))\
			_CrtDbgBreak()

	// Output debug data. Note this uses the OutputDebugString() API which
	// will only be visible when a running under debugger control. Output
	// will show in the "Debug" tab of Visual C++ 6.0
	extern void ClsTrace( LPCTSTR pszFormatString, ... );
	#define TRACE(s)		::ClsTrace( _T( "%s(%i) : " ) s, __FILE__, __LINE__ )
	#define TRACE1(s,a1)		::ClsTrace( _T( "%s(%i) : " ) s, __FILE__, __LINE__, a1 )
	#define TRACE2(s,a1,a2)		::ClsTrace( _T( "%s(%i) : " ) s, __FILE__, __LINE__, a1, a2 )
	#define TRACE3(s,a1,a2,a3)	::ClsTrace( _T( "%s(%i) : " ) s, __FILE__, __LINE__, a1, a2, a3 )
	#define TRACE4(s,a1,a2,a3,a4)	::ClsTrace( _T( "%s(%i) : " ) s, __FILE__, __LINE__, a1, a2, a3, a4 )
	#define XTRACE(s)		::ClsTrace( _T( "%s(%i) : " ) + s, __FILE__, __LINE__ )
	#define XTRACE1(s,a1)		::ClsTrace( _T( "%s(%i) : " ) + s, __FILE__, __LINE__, a1 )
	#define XTRACE2(s,a1,a2)	::ClsTrace( _T( "%s(%i) : " ) + s, __FILE__, __LINE__, a1, a2 )
	#define XTRACE3(s,a1,a2,a3)	::ClsTrace( _T( "%s(%i) : " ) + s, __FILE__, __LINE__, a1, a2, a3 )
	#define XTRACE4(s,a1,a2,a3,a4)	::ClsTrace( _T( "%s(%i) : " ) + s, __FILE__, __LINE__, a1, a2, a3, a4 )
#else
	// Make sure the expression is compiled.
	#define _VERIFY( x )		x

	// Compiles to nothing in release builds.
	#define _NO_COPY(c)
	#define _NO_COPY_PARAM(c,p)
	#define _ASSERT_VALID(x)	((void)0)
	#define _ASSERT_TEXT(x,t)	((void)0)
	#define _TEST_HEAP		((void)0)
	#define TRACE(s)		((void)0)
	#define TRACE1(s,a1)		((void)0)
	#define TRACE2(s,a1,a2)		((void)0)
	#define TRACE3(s,a1,a2,a3)	((void)0)
	#define TRACE4(s,a1,a2,a3,a4)	((void)0)
	#define XTRACE(s)		((void)0)
	#define XTRACE1(s,a1)		((void)0)
	#define XTRACE2(s,a1,a2)	((void)0)
	#define XTRACE3(s,a1,a2,a3)	((void)0)
	#define XTRACE4(s,a1,a2,a3,a4)	((void)0)
#endif

#endif // _STANDARD_H_