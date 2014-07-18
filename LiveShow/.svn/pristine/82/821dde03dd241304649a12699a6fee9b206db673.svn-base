// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxole.h>         // MFC OLE classes
//#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//////////////////////////////////////////////////////////////////////////
//COM支持
//////////////////////////////////////////////////////////////////////////
//Dual Interface
#include "mfcdual.h"

//COM Definition
#include <comdef.h>

//////////////////////////////////////////////////////////////////////////
//全局钩子
//////////////////////////////////////////////////////////////////////////
//#define USE_GLOBAL_MOUSE_HOOKER
//#define USE_GLOBAL_KEYBOARD_HOOKER

//////////////////////////////////////////////////////////////////////////
//XML解析器
//////////////////////////////////////////////////////////////////////////
//MSXML XML解析器支持
#define MSXML_SUPPORT
#ifdef MSXML_SUPPORT
#include <comdefsp.h>
#endif //MS_XML_SUPPORT

//Expat XML解析器支持
//#define EXPAT_SUPPORT
#ifdef EXPAT_SUPPORT
#endif //EXPAT_SUPPORT

//选择XML解析器
#define USE_MSXML
//#define USE_EXPAT
#if defined(USE_MSXML) && defined(USE_EXPAT)
#error both USE_MSXML and USE_EXPAT are set
#endif

//////////////////////////////////////////////////////////////////////////
//JavaScript引擎
//////////////////////////////////////////////////////////////////////////
//MS JS 引擎支持
//#define MS_JS_ENGINE_SUPPORT
#ifdef MS_JS_ENGINE_SUPPORT
#include <ACTIVSCP.H>
#ifdef _DEBUG 
#pragma comment (lib, "comsuppwd.lib") 
#else 
#pragma comment (lib, "comsuppw.lib") 
#endif //_DEBUG
static const GUID CLSID_JAVAScript = 
{ 0xf414c260, 0x6ac0, 0x11cf, { 0xb6, 0xd1, 0x00, 0xaa, 0x00, 0xbb, 0xbb, 0x58} };
#endif //MS_JS_ENGINE_SUPPORT

//V8 JS 引擎支持
#define V8_JS_ENGINE_SUPPORT
#ifdef V8_JS_ENGINE_SUPPORT

//是否使用新的v8
#define V8_NEW

#ifndef V8_NEW

#include "Include/v8.h"

//是否静态链接
//#define V8_LINK_STATIC

#ifdef _DEBUG
#pragma comment(lib, "Lib/v8d.lib")
#ifdef V8_LINK_STATIC
#pragma comment(lib, "Lib/v8_based.lib")
#pragma comment(lib, "Winmm.lib")
#endif //V8_LINK_STATIC
#else //_DEBUG
#pragma comment(lib, "Lib/v8.lib")
#ifdef V8_LINK_STATIC
#pragma comment(lib, "Lib/v8_base.lib")
#pragma comment(lib, "Winmm.lib")
#endif //V8_LINK_STATIC
#endif //_DEBUG

#else //V8_NEW

#include "Include/v8New.h"

#ifdef _DEBUG
#pragma comment(lib, "Lib/JsEngineD.lib")
#else
#pragma comment(lib, "Lib/JsEngine.lib")
#endif

#endif //V8_NEW

#endif //V8_JS_ENGINE_SUPPORT

//选择JS引擎
#define USE_V8_JS_ENGINE
//#define USE_MS_JS_ENGINE
#if defined(USE_V8_JS_ENGINE) && defined(USE_MS_JS_ENGINE)
#error both USE_V8_JS_ENGINE and USE_MS_JS_ENGINE are set
#endif
#if defined(USE_V8_JS_ENGINE) && !defined(V8_JS_ENGINE_SUPPORT)
#error can't USE V8_JS_ENGINE, V8_JS_ENGINE NOT SUPPORTED
#endif
#if defined(USE_MS_JS_ENGINE) && !defined(MS_JS_ENGINE_SUPPORT)
#error can't USE MS_JS_ENGINE, MS_JS_ENGINE NOT SUPPORTED
#endif

//////////////////////////////////////////////////////////////////////////
//自绘滚动条支持
//////////////////////////////////////////////////////////////////////////
#define CUSTOM_SCROLLBAR_SUPPORT
#ifdef CUSTOM_SCROLLBAR_SUPPORT
#include "coolscroll.h"
#include "coolsb_detours.h"
#ifdef _DEBUG
#pragma comment( lib, "NewScrollBarD.lib")
#else 
#pragma comment( lib, "NewScrollBar.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////
//多线程支持
//////////////////////////////////////////////////////////////////////////
//#define MULTI_THREAD_SUPPORT
#ifdef MULTI_THREAD_SUPPORT
#include "LockMgr.h"
#endif 

//////////////////////////////////////////////////////////////////////////
//图像显示库支持
//////////////////////////////////////////////////////////////////////////
//使用CDib
//#define USE_CDIB
#ifdef USE_CDIB
#include "./dib.h"
#endif

//使用Gdiplus库
#define USE_GDIPLUS
#ifdef USE_GDIPLUS
#define GDIPVER 0x0100
#include "Gdiplus.h"
#pragma comment( lib, "gdiplus.lib")
#endif //USE_GDIPLUS

//使用CxImage库
#define USE_CXIMAGE
#ifdef USE_CXIMAGE
#pragma warning (push)
#pragma warning (disable:4996)
#include "ximage.h"
#pragma warning (pop)
#ifdef _DEBUG
#pragma comment( lib, "jpegd.lib")
#pragma comment( lib, "pngd.lib")
#pragma comment( lib, "zlibd.lib")
#pragma comment( lib, "ximaged.lib")
#else 
#pragma comment( lib, "jpeg.lib")
#pragma comment( lib, "png.lib")
#pragma comment( lib, "zlib.lib")
#pragma comment( lib, "ximage.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////
//图像实现支持
//////////////////////////////////////////////////////////////////////////
//CDib支持
//#define CDIB_SUPPORT
#ifdef CDIB_SUPPORT
#ifndef USE_CDIB
#error CDib must be used.
#endif
#endif

//Gdiplus::Bitmap支持
//#define GDIPLUS_BITMAP_SUPPORT
#ifdef GDIPLUS_BITMAP_SUPPORT
#ifndef USE_GDIPLUS
#error Gdiplus must be used.
#endif
#endif

//CxImage支持
#define CXIMAGE_SUPPORT
#ifdef CXIMAGE_SUPPORT
#ifndef USE_CXIMAGE
#error CxImage must be used.
#endif
#endif

//#define USE_IMAGE_CDIB
//#define USE_IMAGE_GDIPLUS_BITMAP
#define USE_IMAGE_CXIMAGE
#if defined(USE_IMAGE_CDIB) && !defined(CDIB_SUPPORT)
#error can't use Image CDib, CDib not supported.
#endif
#if defined(USE_IMAGE_GDIPLUS_BITMAP) && !defined(GDIPLUS_BITMAP_SUPPORT)
#error can't use Image Bitmap, Gdiplus not supported.
#endif
#if defined(USE_IMAGE_CXIMAGE) && !defined(CXIMAGE_SUPPORT)
#error can't use Image CxImage, CxImage not supported.
#endif

//////////////////////////////////////////////////////////////////////////
//皮肤消息
//////////////////////////////////////////////////////////////////////////
/*
@param wParam notify code
@param lParam 
*/
#define WM_IMSKIN_NOTIFY (WM_USER + 2001)
/*
@param wParam notify code
@param lParam 
*/
#define WM_IM_NOTIFY (WM_USER + 2002 )
/*
@param wParam
@param lParam
*/
#define WM_CTRL_CLICK_3 (WM_USER + 2003)
/*
@param wParam HDC DC
@param lParam HRGN Clip Region
*/
#define WM_SKIN_PAINT (WM_USER + 0x9876)
/*
@param wParam HDC DC
*/
#define WM_SKIN_ERASEBKGND (WM_USER + 0x9877)

#define WM_MOUSEENTER (WM_MOUSELEAVE + 1)

#define WM_SKIN_MOUSEENTER	(WM_USER + 0x9874)
#define WM_SKIN_MOUSELEAVE	(WM_USER + 0x9875)

//////////////////////////////////////////////////////////////////////////
//帮助宏
//////////////////////////////////////////////////////////////////////////
#define GETXPARAM(x) ((SHORT)LOWORD(x))
#define GETYPARAM(x) ((SHORT)HIWORD(x))

//////////////////////////////////////////////////////////////////////////
//兼容不安全的CRT
//////////////////////////////////////////////////////////////////////////
#if (_MSC_VER >= 1400)
#define _macro_tcscpy(dst, dstLen, src) _tcscpy_s((dst), (dstLen), (src))
#define _macro_tcsncpy(dst, dstLen, src, srcLen) _tcsncpy_s((dst), (dstLen), (src), (srcLen))
#define _macro_memcpy(dst, dstLen, src, srcLen) memcpy_s((dst), (dstLen), (src), (srcLen))
#else 
#define _macro_tcscpy(dst, dstLen, src) _tcscpy((dst), (src))
#define _macro_tcsncpy(dst, dstLen, src, srcLen) _tcsncpy((dst), (dstLen), (src))
#define _macro_memcpy(dst, dstLen, src, srcLen) memcpy((dst), (src), (srcLen))
#endif //_MSC_VER < 1400

//////////////////////////////////////////////////////////////////////////
//帮助函数
//////////////////////////////////////////////////////////////////////////
HRESULT CreateStreamOnMemory(IN void* pSrc, IN INT iSize, OUT LPSTREAM FAR* ppstm);

//////////////////////////////////////////////////////////////////////////
//MFC正规DLL 与 MFC扩展DLL 设置
//////////////////////////////////////////////////////////////////////////
#if defined(_USRDLL) && defined(_AFXEXT)
#error both _USRDLL and _AFXEXT are defined!
#endif

#ifdef _USRDLL 
#endif //_USRDLL

#ifdef _AFXEXT
#undef AFX_MANAGE_STATE
#define AFX_MANAGE_STATE(p) 
#endif //_AFXEXT

#ifdef _DEBUG
//#include "vld.h"
#endif