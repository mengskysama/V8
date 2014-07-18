#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#include "IOThread.h"
#include "WorkThread.h"

#define  DEFAULT_SOCKETNUM_PER_THREAD                       1024

class COMMON_OUT_CLASS ThreadsManager
{
public:
	ThreadsManager(){}

	~ThreadsManager(){}

	//////////////////////////////////////////////////////////////////////////
	//初始化所有工作线程；缺省状况下，
	//系统中有多少个CPU就初始化多少个工作线程;
	static void				startThreads(UInt32 iWorkThreadsNum=1, UInt32 iSocketNumPerThread=DEFAULT_SOCKETNUM_PER_THREAD);
	
	//停止所有工作线程；
	static void				stopThreads();

	static WorkThread**		s_pWorkThreads;				//工作线程指针列表;
	static UInt32			s_iWorkThreadsNum;			//总共的线程数;
	static UInt32           s_iSocketNumPerThread;      //每个工作线程的Socket数目;

	static IOThread*		s_pIOThread;
};

#endif //__THREADMANAGER_H__