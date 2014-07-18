#pragma once

//cygnus 2012-5-25  v 1.0
#include <winsock2.h>
#include "Thread.h"
#include <queue>
#include <stdio.h>
#include <string.h>
#include "TimeUtils.h"

using namespace std;

class LogMutex
{
public:
	LogMutex();
	~LogMutex();

	virtual bool Lock() const;
	virtual bool Unlock() const;
	virtual bool TryLock() const;
	
protected:
	HANDLE m_mutex;
};

class CSynLog : public BaseThread
{
public:
	CSynLog();
	~CSynLog();
	virtual bool addlog(string str);
	virtual void Entry();
protected:
	//获取当前可执行程序路径
	char* getcurrpath(char* buf,int len);
	char* getlogname(char* buf,int len);
	bool createlogfile();
protected:
	bool m_bIsDirExists;
	FILE * pf;
	LogMutex m_logmutex1;
	LogMutex m_logmutex2;
	queue<string> queue_buffer1;
	queue<string> queue_buffer2;
};

class CSynLogManager
{
public:
	CSynLogManager()
	{
#if __LOG__
		m_plog = new CSynLog();
		m_plog->Start();
#endif
	}

	void addLog(string str)
	{
#if __LOG__
		if (m_plog)
		{
			m_plog->addlog(str);
		}
#endif
	}

	void LogAssert(const char* format, ... )
	{
#if __LOG__
		char cBuf[1024];
		char myBuf[2048] = {0};
		::_snprintf_s(cBuf, sizeof(cBuf), "[%s] %s", TimeUtils::GetCurTime().c_str(), format);
		va_list   args;
		va_start(args, format);
		_vsnprintf(myBuf,2048,format,args);

		va_end (args);
		TRACE("%s",myBuf);
		addLog(myBuf);
#endif
	}

	~CSynLogManager()
	{
#if __LOG__
		m_plog->StopAndWaitForThread();
		SAFE_DELETE(m_plog)
#endif
	}

private:
	CSynLog * m_plog;
};

extern CSynLogManager g_synlogmanager;