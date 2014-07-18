#include "stdafx.h"
#include "GGLog.h"
#include "GGHelper.h"
#include "string_helper.h"
#include "assert.h"

CLog* CLog::Singleton = 0;

CLog::CLog()
{
	::InitializeCriticalSection(&m_hMutex);
	m_strLogPath = string_helper::from(::GetModulePath());
	m_strLogPath += "log\\";

	Singleton = this;
}


CLog::~CLog()
{
	::DeleteCriticalSection(&m_hMutex);
}

CLog*	CLog::Instance()
{
	if( Singleton == 0)
		Singleton = new CLog();

	assert( Singleton != 0);

	return Singleton;
}


 
/*================================================================ 
* 函数名：    InitLog
* 参数：      LPCTST lpszLogPath
* 功能描述:   初始化日志(设置日志文件的路径)
* 返回值：    void
* 作 者：     tingya 2005年01月06日 
================================================================*/ 
void CLog::InitLog(LPCSTR lpszLogPath)   
{	
	m_strLogPath = lpszLogPath;
}

void CLog::Add(const char* filename, const char* fmt, ...)
{
	if( m_strLogPath.empty() || filename == NULL)
		return;

	string strFileName = m_strLogPath + filename;
 
	::EnterCriticalSection(&m_hMutex);  
	
	try      
	{
		va_list argptr;
		va_start(argptr, fmt);
		_vsnprintf_s(m_tBuf, BUFSIZE, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}

	m_pFile = NULL;
	fopen_s( &m_pFile,strFileName.c_str(), "a"); //以添加的方式输出到文件

	if (m_pFile == NULL )
	{
		::LeaveCriticalSection(&m_hMutex); 
		return;
	}

	m_tBuf[BUFSIZE-1] = '\0';
	fprintf(m_pFile, "%s\n", m_tBuf); 
	fclose(m_pFile);

	::LeaveCriticalSection(&m_hMutex);  
}

void CLog::AddText( const char* pText)
{
	if( pText == NULL)
		return;

	::EnterCriticalSection(&m_hMutex);  

	if( m_pFile)
	{
		fprintf(m_pFile, "%s\n", pText);  
	}

	::LeaveCriticalSection(&m_hMutex);  

}

