#ifndef _TYPES_H
#define _TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef NULL
#define NULL 0
#endif
#ifndef min
#define min(a,b) ( ((a)<(b)) ? (a) : (b) )
#endif
#ifndef max
#define max(a,b) ( ((a)>(b)) ? (a) : (b) )
#endif
#define _FILE_OFFSET_BITS 64
typedef unsigned int UINT;

typedef void* PVOID;
typedef const void* CPVOID;

typedef char* PCHAR;
typedef const char* CPCHAR;

typedef unsigned char BYTE;
typedef unsigned char* PBYTE;
typedef const unsigned char* CPBYTE;

typedef signed   char INT8;
typedef unsigned char UINT8;

typedef signed   short INT16;
typedef unsigned short WORD;
typedef unsigned short UINT16;

typedef signed   int INT32;
typedef unsigned int UINT32;

#ifndef WIN32
typedef signed long 	LONG32;
typedef unsigned long 	ULONG32;
#else
typedef INT8 int8_t;	
typedef INT16 int16_t;	
typedef INT32 int32_t;	

typedef UINT8 uint8_t;	
typedef UINT16 uint16_t;	
typedef UINT32 uint32_t;	

#endif

typedef signed __int64 	LONG64;
typedef unsigned __int64  ULONG64;

typedef enum 
{
	/*Connect establishment related values */
	NOCONERROR, 			//0
	HOSTERR,
	SSLCONNERR, //for SSL Connection
	
	CONSOCKERR, 			//3//
	CONERROR, 
	CONREFUSED,
	CONPORTERR, 
	CONCLOSED, 
	SERVERCLOSECONERR,		//8

	BINDERR,				//9 
	BINDOK,
	
	LISTENERR, 
	LISTENOK, 

	ACCEPTERR, 
	ACCEPTOK,				//14

    /* URL handling related value */	//15
	URLOK, 
	URLHTTP, 
	URLFTP, 
	URLFILE, 
	URLUNKNOWN, 
	URLBADPORT,
	URLBADHOST, 
	URLBADPATTERN,				//22

	/* HTTP related values */
	NEWLOCATION, 				//23
	HOK,    					//24
	HEOF,   
	HERR,   
	HAUTHREQ, 
	HAUTHFAIL,
	HTTPNSFOD,


	/*Error values that can happen due to failed proxie request */
	GATEWAYTIMEOUT, 
	SERVICEUNAVAIL, 
	BADGATEWAY, 
	INTERNALSERVERR,
	UNKNOWNREQ,
	
	/*File handling related return values. */
	FOPENERR, 
	FWRITEERR,
	RETROK,

	/*Download related retrn values. */
	DLINPROGRESS, 
	DLABORTED, 
	DLDONE, 
	CANTRESUME, 
	DLLOCALFATAL,
	DLREMOTEFATAL,
	
	SOCKSERR,
	SOCKSOK,

	RIPERR,
	UNKNOWNERROR
} uerr_t;

typedef enum
{
	RET_OK = 0,
	RET_PARSE_RESQ_FAIL,
	RET_READ_RESQ_FAIL,
	RET_NEWDIRECTION,
	RET_CLOSE,
	RET_SOCKET_CREATE_FAIL,

	//6
	RET_CONNECT_TIMEOUT,
	RET_SOCKET_CLOSE,
	RET_SOCKET_ERROR,
	RET_SOCKET_FINISH,
	RET_SOCKET_CONTINUE,

	//11
	RET_PROXY_BEGIN_AUTH,
	RET_PROXY_DOING_AUTH,
	RET_PROXY_AUTH_UNSUPPORTED,
	RET_SOCKET_CONNECT_FAIL,
	RET_SOCKET_SELECT_FAIL,

	//16
	RET_NO_SOCKET_READY,
	RET_SOCKET_HOST_ERROR,
	RET_PROXY_RESQ_CHECK_FAIL,
	RET_PROXYTYPE_INVALID,
	RET_HTTP_ERR,

	//21
	RET_NO_FILE,
	RET_NO_MEMORY,
	RET_FILE_READ,
	RET_FILE_CANNOT_OPEN,
	RET_FILE_CHANGED,

	//26
	RET_POSTPARAM_TYPE_INVALID,
	RET_FAIL,
	RET_RETRY,
	RET_WORKER_STOPED,
	
}RET_CODE;


// time_t is signed
#define MAX_TIME_T 0x7FFFFFFF

#ifdef WIN32
#define off_t _off_t
#define ssize_t SSIZE_T
//#define size_t SIZE_T
#else
#ifndef HIWORD
#define HIWORD(dw) ((dw)>>16)
#endif
#ifndef LOWORD
#define LOWORD(dw) ((dw)&0xffff)
#endif
#ifndef INADDR_NONE
#define INADDR_NONE ((UINT32)0xFFFFFFFF)
#endif
#ifndef INADDR_ANY
#define INADDR_ANY  ((UINT32)0x00000000)
#endif
#endif

#ifndef MAKEDWORD
#define MAKEDWORD(w1,w2) (((w1)<<16)|(w2))
#endif
#ifndef LINE_BUFFER
#define LINE_BUFFER 	1024
#endif

// Routines to copy and byteswap blocks of memory
// Note: cnt is always the ordinal number of values, NOT bytes
void htons_buf( UINT16* pnbuf, const void* phbuf, UINT cnt );
void ntohs_buf( UINT16* phbuf, const void* pnbuf, UINT cnt );
void htonl_buf( UINT32* pnbuf, const void* phbuf, UINT cnt );
void ntohl_buf( UINT32* phbuf, const void* pnbuf, UINT cnt );

#endif 
