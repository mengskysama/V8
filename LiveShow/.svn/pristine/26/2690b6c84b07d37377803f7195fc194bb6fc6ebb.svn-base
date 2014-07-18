/******************************************************************************
 * transferworker.h - HttpTranfer的实际工作线程，负责任务调度，数据收发，上报状态
 
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
#ifndef _TRANSFERWORKER_H
#define _TRANSFERWORKER_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <winsock2.h>
#endif


#include <map>

#include "httptransfer.h"
#include "task.h"

#define MAX_TASKQUE_SIZE 1024

class TTransferWorker
{
	typedef map<int, int> Socket_task_map;	

public:
	TTransferWorker(UINT32 in_maxRunTaskNum);
	~TTransferWorker();
	
	TTask* taskQue[MAX_TASKQUE_SIZE];
	
#ifdef WIN32
	CRITICAL_SECTION task_que_lock;
#else
	pthread_mutex_t task_que_lock;
#endif

	INT32 findTaskPos();
	UINT32 getRunningTaskCount();
	void setMaxRunTaskNum(UINT32);
	INT32 run();
	bool stop;
	
private:
	void scanTaskQue();
	void setMaxfd(int newfd);
	void clearTaskInfo(int taskid);
	int scanIO();
	INT32 curPos;
	INT32 maxPos;//当前被占用的最大的pos

	UINT32 runningTaskCount;
	UINT32 startingTaskCount;
	UINT32 waittingTaskCount;
	UINT32 finishTaskCount;
	UINT32 errorTaskCount;
	UINT32 unkownTaskCount;
	UINT32 connectingTaskCount;

	
	UINT32 maxRunTaskNum;
	Socket_task_map s_t_map;

	int maxfd;
	fd_set readfds;
	fd_set writefds;
	
};

#endif  /* _TRANSFERWORKER_H */

