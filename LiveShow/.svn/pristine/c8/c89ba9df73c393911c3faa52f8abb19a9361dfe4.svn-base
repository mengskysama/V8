#include "stdafx.h"
#include "MyAssert.h"
#include "TimeUtils.h"
#include <assert.h>
#include <string.h>
#include <Shlwapi.h>
#include <time.h>
#include "Mutex.h"
#include "SynLog.h"

#if __LOG__

#ifdef WIN32
#define snprintf	_snprintf
#endif

#endif

AssertLogger            g_Logger;

using namespace std;

#if __LOG__
char* AssertLogger::GetCurrPath(char* buf,int len)
{
	int nlen =  ::GetModuleFileName(NULL,buf,len); 
	while(TRUE) 
	{ 
		if   (buf[nlen--]== '\\') 
			break; 
	} 
	++nlen;
	buf[++nlen] = '\0';
	return &buf[nlen]; 
}
#endif


AssertLogger::AssertLogger()
{
// #if __LOG__
// 	char buf[MAX_PATH]={0};
// 	memset(buf,0,MAX_PATH);
// 	char* pEnd = GetCurrPath(buf,MAX_PATH);
// 	sprintf(pEnd,"log");
// 	*(pEnd+3) = '\0';
// 	if (!::PathIsDirectoryA(buf))
// 	{
// 		CreateDirectoryA(buf,NULL);
// 	}
// 
// 	struct tm* t;
// 	time_t tt;
// 	time(&tt);
// 	t = localtime(&tt);
// 	sprintf(pEnd,"log\\av%4d%02d%02d%02d%02d%02d.log",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
// 	*(pEnd+24) = '\0';
// 	OutputDebugStringA(buf);
// 	m_pFd = ::_fsopen(buf, "w", _SH_DENYNO);
// 	if(m_pFd!=NULL)
// 	{
// 		::fclose(m_pFd);
// 	}
// 	m_pFd = ::_fsopen(buf, "a", _SH_DENYNO);
// 	//m_pFd = ::_fsopen("c:\\Relay.log", "a", _SH_DENYNO);
// #endif
}

AssertLogger:: ~AssertLogger()
{
// #if __LOG__
// 
// 	if(m_pFd!=NULL)
// 	{
// 		::fclose(m_pFd);
// 	}
// 
// #endif
}

void AssertLogger::LogAssert(const char* format, ... )
{
 //#if _DEBUG
// 	
// 	static Mutex s_mutex;
// 	MutexLocker locker(&s_mutex);

	char cBuf[256];
	char myBuf[512] = {0};
	::_snprintf_s(cBuf, sizeof(cBuf), "[%s] %s", TimeUtils::GetCurTime().c_str(), format);
	va_list   args;
	va_start(args, format);
// 	if (m_pFd && ftell(m_pFd) > 1024*1024*70) //70M
// 	{
// 		if(m_pFd!=NULL)
// 		{
// 			try
// 			{
// 				::fflush(m_pFd);
// 				::fclose(m_pFd);
// 			}
// 			catch (...)
// 			{
// 				clearerr(m_pFd);
// 				m_pFd = NULL;
// 			}
// 		}
// 		char buf[MAX_PATH]={0};
// 		memset(buf,0,MAX_PATH);
// 		char* pEnd = GetCurrPath(buf,MAX_PATH);
// 		sprintf(pEnd,"log");
// 		*(pEnd+3) = '\0';
// 		if (!::PathIsDirectoryA(buf))
// 		{
// 			CreateDirectoryA(buf,NULL);
// 		}
// 
// 		struct tm* t;
// 		time_t tt;
// 		time(&tt);
// 		t = localtime(&tt);
// 		sprintf(pEnd,"log\\av%4d%02d%02d%02d%02d%02d.log",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
// 		*(pEnd+24) = '\0';
// 		OutputDebugStringA(buf);
// // 		m_pFd = ::_fsopen(buf, "w", _SH_DENYNO);
// // 		if(m_pFd!=NULL)
// // 		{
// // 			::fclose(m_pFd);
// // 		}
// 		m_pFd = ::_fsopen(buf, "a", _SH_DENYNO);
// 	}
// 	if (m_pFd && ferror(m_pFd)) clearerr(m_pFd);
// 	if (m_pFd) ::vfprintf(m_pFd, cBuf, args);
// #ifdef _DEBUG
// 	::vprintf(cBuf, args);
// #endif
 	_vsnprintf(myBuf,512,cBuf,args);
 
 	va_end (args);
 	TRACE("%s",myBuf);
	g_synlogmanager.addLog(myBuf);
// 	if (m_pFd) ::fflush(m_pFd);
//#endif
}