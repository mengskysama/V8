

#ifndef	 HTTPTRANSFER_RELEASE
#include "..\include\log.h"
#else
#include "..\include\log_stun.h"
#endif
#include "..\include\transferworker.h"
#include "..\include\HttpCli.h"



//#include "task.h"

#define WORKER_SLEEP_TIME 100				//Unit -- ms


INT32 TTransferWorker::findTaskPos()
{
	int i = curPos;	
	do
	{
		if(NULL == taskQue[i])
		{	//找到空闲位置, 则将curPos更新为当前位置的下一个
			curPos = (i+1)%MAX_TASKQUE_SIZE;

			if(i > maxPos)
			{
				maxPos = i;
			}

			return i;
		}
		
		i++;
		if(i == MAX_TASKQUE_SIZE)
		{	//到达数组末端，从头开始寻找
			i = 0;
		}
	}while(i != curPos);	//如i == curPos, 则表明全部队列查找完毕，均未找到空闲位置


	return -1;	//查找失败，无可用位置
}

void TTransferWorker::setMaxRunTaskNum(UINT32 in_maxRunTaskNum)
{
	maxRunTaskNum = in_maxRunTaskNum;
}


UINT32 TTransferWorker::getRunningTaskCount()
{
	return runningTaskCount;
}


/******************************************************************************
 * TTransferWorker.scanTaskQue - 扫描任务数组
 * DESCRIPTION: - 
 *    根据任务当前状态执行响应动作，进行状态变迁 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 18nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
void TTransferWorker::scanTaskQue()
{
	int i = 0;
	int ret = 0;
	TTask* pTask = NULL;

	int runningCount = 0;
	int startingCount = 0;
	int waittingCount = 0;
	int connectingCount = 0;
	int finishCount = 0;
	int errorCount = 0;
	int unkownCount = 0;
	
	LOG_DEBUG("Scan taskQue begin, tasksize=%d runningCount=%d startingCount=%d waittingCount=%d finishCount=%d errorCount=%d unkownCount=%d\n",
		MAX_TASKQUE_SIZE, runningTaskCount, startingTaskCount, waittingTaskCount, finishTaskCount, errorTaskCount, unkownTaskCount);
	
	int scheduledcount = runningTaskCount
							+startingTaskCount
							+finishTaskCount
							+errorTaskCount
							+unkownTaskCount
							+connectingTaskCount;

	int	remainposition = 0;
	if((int)maxRunTaskNum > scheduledcount)
	{
		remainposition = maxRunTaskNum - scheduledcount;
	}


	//扫描任务数组
	for(i = 0; i <= maxPos; i++)
	{
		if(stop)
		{
			LOG_INFO("[scanTaskQue]Found worker stop set, break task quene process\n");		
			return;
		}


		if(NULL == taskQue[i])
		{					
			continue;	//该位置没有任务
		}

		pTask = taskQue[i];
		if(pTask->taskState != TSK_IDLE)
		{
			pTask->checkTimeOut();
		}		

		//此处加锁，用于和httpmanager的canceltask, clear同步，从而保证不会
		//出现canceltask, clear返回后所操作任务的回调函数又被执行的情况。
		//如果不加锁，则可能出现pTask->del检查通过，之后canceltask被调用，且
		//很快返回，然后本函数在task状态处理中又调用task的回调函数的情况
//			thread_lock(&task_que_lock);	
		if(pTask->del)
		{
			clearTaskInfo(i);			
			LOG_INFO("Task=%d was cancelled.\n", i);
//				thread_unlock(&task_que_lock);
			continue;
		}

		if(pTask->respData == NULL)
		{
			//出现严重内部错误，任务失败---原因未知，此处进行保护
			pTask->taskState = TSK_ERROR;
			pTask->errcode = ERR_TASK_PARAM_RESPDATA;
			LOG_WARN("[scanTaskQue]respData is NULL. old status=%d, set task to TSK_ERROR\n", 
						ret, pTask->taskState);	
		}		
		
		switch(pTask->taskState)
		{
			case 	TSK_IDLE:
			{
				if(remainposition > 0
					&& s_t_map.size()<64)
				{			
					//当前运行任务数未达到限制
					pTask->taskState = TSK_CONNECT;
					remainposition --;
				}
				else
				{				
					LOG_DEBUG("MxRunTaskNum=%d currentRun=%d, mapsize=%d, limit reached.\n", 
						maxRunTaskNum, getRunningTaskCount(), s_t_map.size());
				}
				waittingCount++;
				break;					
			}

			case	TSK_CONNECT:
			{				
				//当前运行任务数未达到限制值，尝试激活
				ret = pTask->tryConnect();
				LOG_INFO("tryActiveTask ret=%d for task=%d\n\n", ret, i);

				if(RET_SOCKET_CONTINUE == ret)
				{
					//异步connect,等待socket 可用,记录socket和task的对应关系
					s_t_map[pTask->hcp->getSocket()->get_socketfd()] = i;			
					FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
									&writefds);
					setMaxfd(pTask->hcp->getSocket()->get_socketfd());					
				}

				if(RET_OK == ret)	//连接直接成功，记录socket和task的对应关系
				{
					s_t_map[pTask->hcp->getSocket()->get_socketfd()] = i;
				}
				
				break;
			}

			case   TSK_CONNECTWAIT:
			{	
				//等待连接成功中，需要检查是否超时
				int sockfd = pTask->hcp->getSocket()->get_socketfd();
				ret = pTask->checkConnectTimeout();
				LOG_DEBUG("checkConnectTimeout ret=%d for task=%d\n\n", ret, i);
				if(RET_CONNECT_TIMEOUT == ret)
				{					
					FD_CLR(sockfd, &writefds);
					s_t_map.erase(sockfd);
				}
				
				connectingCount++;
				break;
			}

			case TSK_AUTH:
			{
				ret = pTask->tryAuth();
				if(ret == RET_PROXY_BEGIN_AUTH)
				{
					//开始进行socks认证，需要读取认证响应
					LOG_INFO("Socks authing for task=%d, set fd=%d, wait resq.\n", 
									i, pTask->hcp->getSocket()->get_socketfd());
					FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
							&readfds);
					setMaxfd(pTask->hcp->getSocket()->get_socketfd());						
				}
				break;
			}
			
			case 	TSK_START:				
			{	//检查是否获取到真实地址，如果成功则发送请求，开始下载，状态迁移至running
				if(MAX_REDIRECTIONS <= pTask->redirectionTimes)
				{	//重定向次数超出限制
					LOG_WARN("MAX_REDIRECTIONS=%d redirectionTimes=%d, limit reached.\n", 
								MAX_REDIRECTIONS, pTask->redirectionTimes);
					pTask->taskState = TSK_ERROR;
					pTask->errcode = ERR_TASK_REDIRECTION_EXCEED;
				}
				else
				{	
					//判断是否要重新发送请求，此标志第一次发送请求是为true
					//发送请求后置为false，收到重定向请求后会重新置为true
					if(pTask->shouldGetFileInfo)
					{
						LOG_INFO("Send Req for task=%d.\n", i);
						ret = pTask->sendHttpReq();
								
						if(RET_SOCKET_FINISH == ret)
						{	
							//发送http请求成功，设置读取标记，等待响应到达
							LOG_INFO("Send Req success for task=%d, set fd=%d, wait resq.\n", 
									i, pTask->hcp->getSocket()->get_socketfd());
							FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
									&readfds);
							setMaxfd(pTask->hcp->getSocket()->get_socketfd());						
						}
						else if(RET_SOCKET_CONTINUE == ret)
						{	
							//请求的数据量较大，设置写入标记，等待socket可写后继续写入
							LOG_DEBUG("Send Req block for task=%d, set fd=%d, wait resq.\n", 
											i, pTask->hcp->getSocket()->get_socketfd());
							FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
									&writefds);
							setMaxfd(pTask->hcp->getSocket()->get_socketfd());							
						}
						else						
						{
							//发送HTTP请求失败，任务失败
							LOG_WARN("Send Req fail ret=%d for task=%d.\n", ret, i);
						}
					}
					else
					{
						pTask->updateTaskInfo();
						
						if(NULL != pTask->handle_init)
						{
							thread_lock(&task_que_lock);	
							if(!pTask->del)
							{
								pTask->handle_init(i, &pTask->taskInfo);
							}
							thread_unlock(&task_que_lock);	
						}
						LOG_INFO("Req already sent for task=%d, waiting resq.\n", i);
					}
					
				}
				startingCount++;
				break;
			}
			
			case 	TSK_RUNNING:
			{				
				LOG_DEBUG("task=%d running, try call callback.\n", i);
				//此处调用任务回调函数汇报进度信息
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_process
						&& pTask->dataChanged)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_process(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
					if(pTask->dataClean)
					{
						LOG_DEBUG("task=%d data changed and dataClean set, Clean current data --len=%d.\n", 
									i, pTask->respData->size());
						pTask->respData->clear();
					}
				}
				else
				{
					LOG_DEBUG("task=%d data not change, Don't call callback.\n", i);
				}
				runningCount++;
				break;
			}
				
			case 	TSK_FINISHED:
			{
				LOG_INFO("task=%d finished, try call callback, and delete task info.\n", i);
				//此处调用任务回调函数汇报进度信息
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_finish)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_finish(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
				}	
				

				//TODO:此处应清除任务相关信息
				clearTaskInfo(i);
				
				finishCount++;
				break;
			}
			
			case 	TSK_ERROR:
			{
				LOG_WARN("task=%d Error, try call callback, and delete task info.\n", i);
				//此处调用任务回调函数上报错误
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_error)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_error(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
				}	

				//TODO:此处应清除任务相关信息
				clearTaskInfo(i);

				errorCount++;
				break;
			}
			
			default:
				LOG_WARN("task=%d Status Unkonw, delete task info.\n", i);
				//TODO:此处应清除任务相关信息
				pTask->errcode = ERR_TASK_UNKNOWN_STATUS;	
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_error)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_error(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
				}
				clearTaskInfo(i);
				unkownCount++;
				break;			
		}		
//			thread_unlock(&task_que_lock);
	}


	runningTaskCount =	 runningCount;	
	startingTaskCount =  startingCount; 
	waittingTaskCount =  waittingCount; 
	finishTaskCount =	 finishCount;	
	errorTaskCount =	 errorCount;	
	unkownTaskCount =	 unkownCount;	
	connectingTaskCount = connectingCount;
	
	LOG_DEBUG("Scan taskQue finished, tasksize=%d runningCount=%d startingCount=%d waittingCount=%d finishCount=%d errorCount=%d unkownCount=%d\n",
		MAX_TASKQUE_SIZE, runningTaskCount, startingTaskCount, waittingTaskCount, finishTaskCount, errorTaskCount, unkownTaskCount);

}

int TTransferWorker::scanIO()
{
	//开始select IO 队列
	
	int nreadys = 0;
	int readed = 0;
	int wrote = 0;
	timeval tm;	
	fd_set tmpreadfds;
	fd_set tmpwritefds;
	int ret = 0;
	TTask* pTask = NULL;

	Socket_task_map	delete_map;
	Socket_task_map	insert_map;
	
	//这里为了加快循环，采用了500us的延时
	tm.tv_sec = 0;
	tm.tv_usec = 500;
	
	LOG_DEBUG("[scanIO]Set %d read sockets %d write sockets, mapsize %d, maxfd %d\n", 
						readfds.fd_count, writefds.fd_count, s_t_map.size(), maxfd);
	
	if(0 == readfds.fd_count && 0 == writefds.fd_count)
	{
		if(s_t_map.size() != 0)
		{
			LOG_WARN("[scanIO]Set %d read sockets %d write sockets, mapsize %d\n", 
				readfds.fd_count, writefds.fd_count, s_t_map.size());
		}
		else
		{
			//LOG_DEBUG("[scanIO]No read or write sockets select.\n");
			return RET_NO_SOCKET_READY;
		}
	}
	
	tmpreadfds = readfds;
	tmpwritefds = writefds;
	nreadys = select(maxfd+1, &tmpreadfds, &tmpwritefds, NULL, &tm);
	if (nreadys > 0)
	{
		Socket_task_map::iterator it = s_t_map.begin();
		int sockfd = 0;
		int taskid = 0;
		delete_map.clear();
		insert_map.clear();
		for(; it != s_t_map.end(); it++)
		{	//Find the matched socket fd.
			if(stop)
			{
				LOG_INFO("[scanIO]Found worker stop set, break s_t_map IO process\n");
				
				return RET_WORKER_STOPED;
			}

			sockfd = it->first;
			taskid = it->second;
			pTask =  taskQue[taskid];

			if(TSK_CONNECTWAIT > pTask->taskState 
				|| TSK_RUNNING < pTask->taskState) 
			{
				//非中间状态收到数据, 则不做处理
				continue;
			}

			if(pTask->respData == NULL)
			{
				//出现严重内部错误，任务失败---原因未知，此处进行保护
				pTask->taskState = TSK_ERROR;
				pTask->errcode = ERR_TASK_PARAM_RESPDATA;
				LOG_WARN("[scanIO]respData is NULL. old status=%d, set task to TSK_ERROR\n", 
							ret, pTask->taskState);	
			}	
			
			/******************************************************/
			//发送数据，续发http请求，一般发生在用Post上传文件等请求数据量大时//
			/******************************************************/
			if(FD_ISSET(sockfd, &tmpwritefds))
			{
				LOG_DEBUG("Socket=%d of task=%d state=%d ready for write.\n", 
					sockfd, it->second, pTask->taskState);

				//处于TSK_CONNECTWAIT任务可读，则代表连接成功 ---此处特殊处理
				if(TSK_CONNECTWAIT == pTask->taskState)
				{
					pTask->onConnected();
					FD_CLR(sockfd, &writefds);		//清除标记					
					continue;
				}

				ret = pTask->sendHttpReqContinue();
				if(RET_SOCKET_FINISH == ret)
				{	
					//发送http请求成功，删除写标记，设置读取标记，等待响应到达
					LOG_INFO("Send Req success for task=%d, set fd=%d, wait resq.\n", 
							taskid, pTask->hcp->getSocket()->get_socketfd());
					FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
							&readfds);
					FD_CLR(sockfd, &writefds);											
				}
				else if(RET_SOCKET_CONTINUE == ret)
				{	
					//请求的数据量较大，等待socket的下一次可写后继续写入
					LOG_DEBUG("Send Req block for task=%d, set fd=%d, wait resq.\n", 
							taskid, pTask->hcp->getSocket()->get_socketfd());							
				}
				else						
				{
					//发送HTTP请求失败，任务失败  --- 此处应清楚写标记
					FD_CLR(sockfd, &writefds);	
					//需要删除老的socket和task的对应关系
					delete_map[sockfd] = taskid;
					LOG_WARN("Send Req fail ret=%d for task=%d.\n", ret, taskid);
				}	
				LOG_DEBUG("write data to socket=%d of task=%d success\n", sockfd, taskid);
				wrote++;
			}


			/******************************************************/
			//读入数据_包括响应头和响应体//
			/******************************************************/
			if(FD_ISSET(sockfd, &tmpreadfds))
			{	//Found target fd.			
				LOG_DEBUG("Socket=%d of task=%d state=%d ready for read.\n", 
					sockfd, it->second, pTask->taskState);

				//处于TSK_AUTH任务可读，则代表处于socks认证中 ---此处特殊处理
				if(TSK_AUTH == pTask->taskState)
				{
					ret = pTask->onAuthResp();
					
					//正在代理建立中，需等待下一次数据到达，其它情况，则清楚标记
					if(ret != RET_PROXY_DOING_AUTH)
					{
						FD_CLR(sockfd, &readfds);		//清除标记	
					}
					continue;
				}
	
				ret = pTask->processHttpResp();
				if(ret != RET_OK && ret != RET_SOCKET_CONTINUE)
				{	//该连接已不需要继续监听, 则解除监听，解除映射
					if(ret == RET_PROXY_DOING_AUTH)
					{
						LOG_INFO("processHttpResp ret=%d, socket=%d of task=%d\n", 
									ret, sockfd, taskid);
					}
					else
					{
						LOG_INFO("processHttpResp success ret=%d, Release socket=%d of task=%d\n", 
									ret, sockfd, taskid);
						//需要删除老的socket和task的对应关系
						delete_map[sockfd] = taskid;	
					}
					FD_CLR(sockfd, &readfds);	//不再从这里读取数据
					
				}
				else
				{
					//由于需要和canceltask,clear做同步，故所有pTask->del检查的地方都要加锁		
					thread_lock(&task_que_lock);	
					if(	!pTask->del					
						&& pTask->respHeadCompleted 
						&& pTask->handle_headcomplete)
					{
						//响应头已经得到，调用回调函数，报告已经收到响应头					
						pTask->updateTaskInfo();
						LOG_INFO("CALL handle_headcomplete FOR task=%d\n", taskid);
						pTask->handle_headcomplete(taskid, &pTask->taskInfo);
						//该回调函数只调用一次
						pTask->handle_headcomplete = NULL;
					}
					thread_unlock(&task_que_lock);
					LOG_DEBUG("read data from socket=%d of task=%d success\n", sockfd, taskid);
				}
				
				readed++;
			}
			else	//该socket没有数据到达
			{
				LOG_DEBUG("NO data from socket=%d of task=%d\n", sockfd, taskid);
			}
		}
		LOG_DEBUG("All s_t_map scaned, processed readed=%d wrote=%d of nreadys=%d\n", readed, wrote, nreadys);

		for(it = delete_map.begin(); it != delete_map.end(); it++)
		{
			LOG_DEBUG("Delete sock=%d of task=%d\n", 
						it->first, it->second);
			s_t_map.erase(it->first);
		}

		for(it = insert_map.begin(); it != insert_map.end(); it++)
		{
			LOG_DEBUG("Insert sock=%d of task=%d\n", 
						it->first, it->second);
			s_t_map[it->first] = it->second; 
		}
		
	}
	else if(0 == nreadys)
	{
		LOG_DEBUG("No socket ready\n");
		return RET_NO_SOCKET_READY;
	}
	else
	{
		LOG_DEBUG("Select failed ready = %d errno=%d errinfo=%s\n", 
				nreadys, socket_errno(), socket_errinfo());
		return RET_SOCKET_SELECT_FAIL;
	}	
	return RET_OK;
}


void TTransferWorker::setMaxfd(int newfd)
{
	maxfd = maxfd > newfd? maxfd:newfd;
}


INT32 TTransferWorker::run()
{
	int i = 0;
	int ret = 0;
	
	while(!stop)
	{
		LOG_DEBUG("Begin worker intevel %d\n", i);
		//扫描任务队列，完成状态处理，迁移等工作
		scanTaskQue();

		//扫描监听的socket，收取响应数据，并执行相应动作
		ret = scanIO();
		LOG_DEBUG("End worker intevel %d\n", i);
		i++;
		if(RET_NO_SOCKET_READY == ret && !stop)
		{
			delay_ms(WORKER_SLEEP_TIME);
		}
	}

	/**** worker被终止，此处应进行清理工作 ****/
	LOG_DEBUG("Worker was stoped, begin cleaning job\n");
	//首先清理任务对列
	for(i = 0; i < MAX_TASKQUE_SIZE; i++)
	{
		if(NULL != taskQue[i])
		{	
			clearTaskInfo(i);
		}
	}
	LOG_DEBUG("taskQue cleaned\n");
	//清理map表
	s_t_map.clear();
	//TODO:其它清理工作
	LOG_DEBUG("s_t_map cleaned\n");
	
	return 0;
}


/******************************************************************************
 * TTransferWorker.clearTaskInfo - 清理任务的所有信息，并释放任务所占用任务队列空间 
 * ---- 注，调用此函数请需确保获取了 队列锁task_que_lock
 * DESCRIPTION: - 
 *    n/a 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 01dec2008, Davidfan written
 * --------------------
 ******************************************************************************/
void TTransferWorker::clearTaskInfo(int taskid)
{
	TTask* pTask = taskQue[taskid];
	if(NULL != pTask)
	{
		LOG_DEBUG("Clearing task=%d\n", taskid);
//			if(pTask->taskState > TSK_START)
//			{
//	#ifndef HTTPTRANSFER_RELEASE
//				pTask->dumpDataToFile();
//	#endif
//			}
		
		int sockfd = pTask->hcp->getSocket()->get_socketfd();
		
		//将socket从监听队列中清楚
		FD_CLR(sockfd, &readfds);
		FD_CLR(sockfd, &writefds);
		//将socket从映射表中删除
		s_t_map.erase(sockfd);

		//将socket从监听队列中清楚

		//和外部线程获取pTask指针进行同步本需要加锁，
		//但由于调用该函数的scanTaskQue已经加锁，故此处不需加锁---
		thread_lock(&task_que_lock);	
		delete pTask;		//调用TASK的析构函数完成内部清理工作
		taskQue[taskid] = NULL;	
		thread_unlock(&task_que_lock);
		
	}
	else
	{
		LOG_DEBUG("Task=%d not exist when clearing\n", taskid);		
	}
}

TTransferWorker::TTransferWorker(UINT32 in_maxRunTaskNum)
{
	maxPos = 0;
	curPos = 0;
	maxRunTaskNum = in_maxRunTaskNum;
	stop = false;
	maxfd = 0;
	FD_ZERO(&readfds);	
	FD_ZERO(&writefds);
	
	for(int i = 0; i< MAX_TASKQUE_SIZE; i++)
	{
		taskQue[i] = NULL;
	}
	//memset(taskQue, 0x0, sizeof(taskQue)*sizeof(TTask*));
	runningTaskCount = 0;
	startingTaskCount = 0;
	waittingTaskCount = 0;
	finishTaskCount = 0;
	errorTaskCount = 0;
	unkownTaskCount = 0;
	connectingTaskCount = 0;

	thread_lock_init(&task_que_lock);
}

TTransferWorker::~TTransferWorker()
{
	thread_lock_uninit(&task_que_lock);
}



