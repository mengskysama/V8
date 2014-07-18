#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/types.h>
#include "Types.h"

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#else
//#include <windows.h>
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <Winsock2.h>
#endif


#ifdef WIN32
#define strcasecmp(x, y) _stricmp(x, y)
#define strncasecmp(x, y, z) _strnicmp(x, y, z)
#define snprintf		 _snprintf
//#define open(x,y,z)			 _open(x,y,z)
#define strdup			 _strdup
//typedef int				ssize_t;
#endif

#define MAX_MSG_SIZE 64000
#define MAX_BUFFER_LEN 64000
#define LINE_BUFFER 	1024
//#define HTTP_BUF_SIZE 	4096
#define HTTP_BUF_SIZE 	64000

extern void delay_ms(int ms);
extern int DiffTime(struct timeval *result, struct timeval *x, struct timeval *y);
extern void ShowMsg(const char * args, ...);
extern int Panic(const char * args, ...);
extern int safe_open(const char *pathname,  int flags);
//extern ssize_t safe_write(int fd,  const void *buf,  size_t count);
//extern ssize_t safe_read(int fd,  void *buf,  size_t count);
//extern ssize_t full_write(int fd, const void *buf, size_t len);
extern int  CreateOutFile(const char * PathName);
extern bool IsTargetFileExist(const char * FileName, ULONG64& filesize);
extern void base64_encode(const char *s, char *store, int length);
extern char * StrDup(const char * str);
//extern int checkBufLen(char* buf, ULONG64 needSize, ULONG64& currentSize);
#ifdef WIN32
extern void thread_lock_init(CRITICAL_SECTION* m_thread_mutex);
extern void thread_lock_uninit(CRITICAL_SECTION* m_thread_mutex);
extern int thread_lock(CRITICAL_SECTION* m_thread_mutex);
extern int thread_unlock(CRITICAL_SECTION* m_thread_mutex);
#else
extern void thread_lock_init(pthread_mutex_t* m_thread_mutex);
extern void thread_lock_uninit(pthread_mutex_t* m_thread_mutex);
extern int thread_lock(pthread_mutex_t* m_thread_mutex);
extern int thread_unlock(pthread_mutex_t* m_thread_mutex);
#endif


#define ISDIGIT(x) ((x) >= '0' && (x) <= '9')

#define ISBLANK(x) ((x) == ' ' || (x) == '\t')

#define RETURN_IF_FAILED( test ) \
	{ int ret; if((ret=(test)) != SUCC) return ret; }
#define EXIT_IF_FAILED( test ) \
	{ int ret; if((ret=(test)) != SUCC) return ret; }

enum {
	BASE64_FLAG_NONE = 0,
	BASE64_FLAG_NOPAD = 1,
	BASE64_FLAG_NOCRLF = 2
};

extern int Base64Encode(const unsigned char *pbSrcData, int nSrcLen, char* szDest, int *pnDestLen, unsigned int dwFlags);
extern int DecodeBase64Char(unsigned int ch);
extern bool Base64Decode(const char* szSrc, int nSrcLen, unsigned char *pbDest, int *pnDestLen);


#endif
