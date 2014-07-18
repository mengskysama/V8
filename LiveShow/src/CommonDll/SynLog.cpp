#include "stdafx.h"
#include "SynLog.h"
#include <Shlwapi.h>
#include <time.h>
#include <share.h>

#define BUFFER_QUEUE_MAX_SIZE 500  //queue中最多日志缓冲条数 防止溢出
#define BUFFER_LOGFILE_SIZE_LIMIT  1024*1024*1  //日志文件大小限制  1M

CSynLogManager g_synlogmanager;

LogMutex::LogMutex()
{
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
}

LogMutex::~LogMutex()
{
	::CloseHandle(m_mutex);
}


bool LogMutex::Lock() const
{
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	return d!=WAIT_OBJECT_0 ? FALSE:TRUE;
}

bool LogMutex::Unlock() const
{
	return ::ReleaseMutex(m_mutex);
}

bool LogMutex::TryLock() const
{
	DWORD d = WaitForSingleObject(m_mutex, 0);
	return d!=WAIT_OBJECT_0 ? FALSE:TRUE;
}

//--------------------------------------------------------------------------------------

CSynLog::CSynLog()
{
	pf = NULL;
	m_bIsDirExists = true;
}

CSynLog::~CSynLog()
{
	SendStopRequest();
	//SetRunningState(false);
	StopAndWaitForThread();

	if (pf)
	{
		fclose(pf);
	}
}

bool CSynLog::addlog(string str)
{
	int i = 0;
	while (m_bIsDirExists && i++<3)
	{
		if (m_logmutex1.TryLock())
		{
			if (queue_buffer1.size() < BUFFER_QUEUE_MAX_SIZE)
			{
				queue_buffer1.push("[buffer1]: "+str);
			}
			m_logmutex1.Unlock();
			return true;
		}
		
		if (m_logmutex2.TryLock())
		{
			if (queue_buffer2.size() < BUFFER_QUEUE_MAX_SIZE)
			{
				queue_buffer2.push("[buffer2]: "+str);
			}
			m_logmutex2.Unlock();
			return true;
		}

		SwitchToThread();
	}
	
	return false;
}

bool CSynLog::createlogfile()
{
	if (pf)
	{
		try
		{
			fflush(pf);
			fclose(pf);
			pf = NULL;
		}
		catch (...)
		{
			clearerr(pf);
			pf = NULL;
		}
	}

	char buf[MAX_PATH];
	char* temp = getlogname(buf,MAX_PATH);
	if (!temp)
	{
		pf = NULL;
		return false;
	}
	pf = _fsopen(buf,"a+",_SH_DENYNO);
	return (pf!=NULL);
}

void CSynLog::Entry()
{
	static unsigned int s_icount = 0;
	static unsigned int s_iwriteline = 0;
	
	SetRunningState(TRUE);

	while (IsRunningState() && !IsStopRequested())
	{
		DWORD dwStart = GetTickCount();
		while (!queue_buffer1.empty() || !queue_buffer2.empty())
		{
			if (!pf)
			{
				createlogfile();
				if (pf && ferror(pf)) clearerr(pf);
			}

			if (s_icount++ % 20 ==0 && pf && ftell(pf) > BUFFER_LOGFILE_SIZE_LIMIT)
			{
				createlogfile();
				if (pf && ferror(pf)) clearerr(pf);
			}

			if (!m_bIsDirExists)
			{
				SetRunningState(false);
				break;
			}

			if (!pf)
			{
				break;
			}
			
			if (queue_buffer1.size()>0 && m_logmutex1.TryLock())
			{
				::vfprintf(pf,queue_buffer1.front().c_str(),NULL);
				queue_buffer1.pop();

				//if (++s_iwriteline % 20 == 0)
				{
					fflush(pf);
				}
				m_logmutex1.Unlock();
			}

			if (queue_buffer2.size()>0 && m_logmutex2.TryLock())
			{
				::vfprintf(pf,queue_buffer2.front().c_str(),NULL);
				queue_buffer2.pop();

				//if (++s_iwriteline % 20 == 0)
				{
					fflush(pf);
				}

				m_logmutex2.Unlock();
			}
		}
		DWORD dwEnd = GetTickCount();
		if (dwEnd - dwStart < 150)
		{
			Sleep(150 - dwEnd + dwStart);
		}
	}

	if (pf)
	{
		try
		{
			fflush(pf);
			fclose(pf);
			pf = NULL;
		}
		catch (...)
		{
			clearerr(pf);
			pf = NULL;
		}
	}
}

char* CSynLog::getcurrpath(char* buf,int len)
{
	int nlen = ::GetModuleFileNameA(NULL,buf,len);
	while (1)
	{
		if (buf[nlen--]=='\\')
			break;
	}
	nlen++;
	buf[++nlen] = '\0';
	return &buf[nlen];
}

char* CSynLog::getlogname(char* buf,int len)
{
	//判断路径是否存在 生成路径
	char* pname = getcurrpath(buf,len);
	sprintf(pname,"%s\\","log");
	if (!::PathIsDirectoryA(buf))
	{
		//::CreateDirectoryA(buf,NULL);
		m_bIsDirExists = false;
		return NULL;
	}

	//生成日志文件名称
	time_t tt;
	time(&tt);
	struct tm* t = localtime(&tt);
	sprintf(pname,"log\\log%4d%02d%02d_%02d.%02d.%02d.log",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
	*(pname+30) = '\0';
	return buf;
}