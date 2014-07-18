#ifndef __COMMON_H__
#define __COMMON_H__

#define SAFE_DELETE(ptr)  if ((ptr)) { delete   (ptr); (ptr) = NULL; }
#define SAFE_DELETEA(ptr) if ((ptr)) { delete[] (ptr); (ptr) = NULL; }
#define SAFE_DELETE_DIALOG(ptr) if ((ptr)) { ptr->DestroyWindow(); delete (ptr); (ptr) = NULL; }

#ifdef COMMON_DLL
#define COMMON_OUT_CLASS  _declspec(dllexport)
#else
#define COMMON_OUT_CLASS  _declspec(dllimport)
#endif

		#define _64BITARG_ "i64"
		/* Includes */
		#include <windows.h>
		#include <winsock2.h>
		#include <mswsock.h>
		#include <process.h>
		#include <ws2tcpip.h>
		#include <fcntl.h>
		#include <io.h>
		#include <direct.h>
		#include <errno.h>
		#include <Mmsystem.h>
        #include <time.h>


/* Typedefs */
		typedef unsigned int		PointerSizedInt;
		typedef unsigned char		UInt8;
		typedef signed char			SInt8;
		typedef unsigned short		UInt16;
		typedef signed short		SInt16;
		typedef unsigned int		UInt32;
		typedef signed int			SInt32;
		typedef LONGLONG			SInt64;
		typedef ULONGLONG			UInt64;
		typedef float				Float32;
		typedef double				Float64;
		typedef UInt16				Bool;

		typedef unsigned long		FourCharCode;

		#ifdef	FOUR_CHARS_TO_INT
		#error  Conflicting Macro "FOUR_CHARS_TO_INT"
		#endif

		#define	FOUR_CHARS_TO_INT( c1, c2, c3, c4 )  ( c1 << 24 | c2 << 16 | c3 << 8 | c4 )
		#define TOUPPER(c) ((((c) >= 'a') && ((c) <= 'z')) ? (c)+'A'-'a' : (c))

		#ifdef	TW0_CHARS_TO_INT
		#error Conflicting Macro "TW0_CHARS_TO_INT"
		#endif

		#define	TW0_CHARS_TO_INT( c1, c2 )  ( c1 << 8 | c2 )

		 // POSIX errorcodes
		#define		ENOTCONN 				1002
		#define		EADDRINUSE 				1004
		#define		EINPROGRESS 			1007
		#define		ENOBUFS 				1008
		#define		EADDRNOTAVAIL 			1009


		#define		ENOERR					0
		#define		EPARAMATERS				-1

		//////////////////////////////////////////////////////////////////////////
		//Buffer 错误
		#define     ECIRBUFFERPOS			-11
		#define     ECIRBUFFEROVERFLOW		-12
		#define		ECIRBUFFERNOTREADY		-13

		//////////////////////////////////////////////////////////////////////////
		//媒体文件和Socket操作错误代码
		#define     EOPENFAILED             -21			//打开媒体文件出错;
		#define     EBINDFAILED             -22			//绑定Socket出错;
		#define     EINVALIDFILEFMT			-23			//媒体错误;
		#define		EFILEDATA				-24			//媒体数据出错;
		#define		EEOF					-25			//文件结束;
		#define		EIOFAILED				-26			//IO错误;

#endif //__COMMON_H__
