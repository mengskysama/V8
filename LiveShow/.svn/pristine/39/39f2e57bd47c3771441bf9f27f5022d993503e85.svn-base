/******************************************************************************
 * httptransfer.cpp - TranferMgr类的实现
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     n/a
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
#ifdef WIN32
#include <process.h>
#include <direct.h>

#endif
#include "..\include\httptransfer.h"
#include "..\include\httpmanager.h"
#include "..\include\transferworker.h"
#include "..\include\Utils.h"
#include "..\include\task.h"
#include "..\include\HttpCli.h"

#ifndef	 HTTPTRANSFER_RELEASE
#include "..\include\log.h"
#else
#include "..\include\log_stun.h"
#endif

#include <errno.h>

//extern int errno;

char log_path[1024] = {0};
#define MAX_RUN_TASK 32

// CALL_BACK_S init_callbacks = {NULL, NULL, NULL, NULL, NULL};
// OPTIONS init_options = {0, 0, 0, 0};
// int itest = 1;

#ifdef WIN32
	HANDLE	m_hThread;
#else
	pthread_t m_hThread;
#endif

#ifdef WIN32
unsigned __stdcall run(void* param)
#else
void* run(void* param)
#endif
{
	int ret = 0;
	
	TTransferWorker* pWorker = (TTransferWorker*)param;
	if( pWorker)
	{
		ret = pWorker->run();

		LOG_DEBUG("[TranferManager]pWorker run ret=%d\n", ret);
		delete	pWorker;
		pWorker = NULL;
	}


	//工作线程即将退出(出错，或人工终止)	
//线程退出
	LOG_DEBUG("call _endthread to end thread.\n");
#ifdef WIN32
	WSACleanup();
	_endthread();
#else
	pthread_exit(NULL);
#endif
	return 0;
}

int ret_to_err(int retcode)
{
	switch(retcode)
	{	
		
	case RET_NO_FILE:
		return ERR_TASK_NO_FILE;
		
	case RET_NO_MEMORY:
		return ERR_TASK_NO_MEMORY;
		
	case RET_FILE_READ:
		return ERR_TASK_FILE_READ;
		
	case RET_FILE_CANNOT_OPEN:
		return ERR_TASK_FILE_CANNOT_OPEN;
		
	case RET_FILE_CHANGED:
		return ERR_TASK_FILE_CHANGED;
		
	case RET_POSTPARAM_TYPE_INVALID:
		return ERR_TASK_POSTPARAM_TYPE_INVALID;
		
	case RET_OK:
	case RET_PARSE_RESQ_FAIL:
	case RET_READ_RESQ_FAIL:
	case RET_NEWDIRECTION:
	case RET_CLOSE:
	case RET_SOCKET_CLOSE:
	case RET_SOCKET_ERROR:
	case RET_SOCKET_FINISH:
	case RET_SOCKET_CONTINUE:		
	case RET_HTTP_ERR:		
	case RET_FAIL:
	case RET_RETRY:
	default:
		//对于没有设置对应错误码的，则原值返回
		return retcode;
	}
}


INT POST_PARAMS::add_param(const char* paramName,
					const char* buffer,	
					UINT32 bufferLen,
					PARAM_TYPE paramType)
{
	POST_PARAM param;


	memset(&param, 0x0, sizeof(POST_PARAM));
	strncpy(param.paramName, paramName, sizeof(param.paramName));
	if(paramType == PARAM_FILENAME)
	{
		int file_path_len = (int)strlen(buffer);
		param.paramBuffer = new char[file_path_len+1];
		param.paramBufferLen = bufferLen;
		
		memset(param.paramBuffer, 0x0, file_path_len+1);
		memcpy(param.paramBuffer, buffer, file_path_len+1);
		LOG_DEBUG("[add_param]PARAM_FILENAME: paramname=%s filename=%s bufferLen=%d",
						paramName, buffer, bufferLen);
	}
	else if(paramType == PARAM_TEXT)
	{
		param.paramBufferLen = (int)strlen(buffer)+1;
		param.paramBuffer = new char[param.paramBufferLen];		
		memset(param.paramBuffer, 0x0, param.paramBufferLen);
		memcpy(param.paramBuffer, buffer, param.paramBufferLen);
		LOG_DEBUG("[add_param]PARAM_FILENAME: paramname=%s paramvalue=%s bufferLen=%d",
						paramName, buffer, bufferLen);
	}
	else
	{
		param.paramBuffer = new char[bufferLen];
		param.paramBufferLen = bufferLen;
		memset(param.paramBuffer, 0x0, bufferLen);
		memcpy(param.paramBuffer, buffer, bufferLen);
		LOG_DEBUG("[add_param]PARAM_TYP=%d: paramname=%s bufferLen=%d",
						paramType, paramName, bufferLen);
	}
	param.paramType = paramType;

	postParams.push_back(param);
	return RET_OK;
}

POST_PARAMS::~POST_PARAMS()
{
	for(UINT i = 0; i < postParams.size(); i++)
	{
		if(postParams[i].paramBuffer != NULL)
		{
			delete[] postParams[i].paramBuffer;
			postParams[i].paramBuffer = NULL;
		}
	}		
	return;
}

POST_PARAMS::POST_PARAMS(POST_PARAMS &in_post_param)
{
	PARAMS_V* pParams = in_post_param.get_params();
	for(UINT i = 0; i < pParams->size(); i++)
	{
		char* paramName = (*pParams)[i].paramName;
		char* paramBuffer = (*pParams)[i].paramBuffer;
		UINT32 paramBufferLen = (*pParams)[i].paramBufferLen;
		PARAM_TYPE paramType = (*pParams)[i].paramType;
		
		add_param(paramName, paramBuffer, 
				paramBufferLen, paramType);
	}	
}

bool POST_PARAMS::is_uploadtype()
{
	for( unsigned int i = 0; i< postParams.size(); i++)
	{
		POST_PARAM* pparam = &postParams[i];
		if( pparam->paramType == PARAM_FILENAME || pparam->paramType == PARAM_BINARY)
			return true;
	}

	return false;
}

bool POST_PARAMS::delete_param( const string strParamName)
{
	for( unsigned int i = 0; i< postParams.size(); i++)
	{
		POST_PARAM* pparam = &postParams[i];
		if( strParamName == pparam->paramName )
		{
			if(pparam->paramBuffer != NULL)
			{
				delete[] pparam->paramBuffer;
				postParams[i].paramBuffer = NULL;
			}
			postParams.erase( postParams.begin() + i);
			return true;
		}
	}

	return false;
}

namespace httpservice
{

TranferManager* TranferManager::instance = NULL;
TranferManager* TranferManager::getInstance() 
{
	if(NULL == instance)
	{
		instance = new TranferManager();
	}
	return instance;
}

void TranferManager::delInstance() 
{
	if(NULL != instance)
	{
		delete instance;
		instance = NULL;
	}
}

UINT32 TranferManager::setMaxRunningTaskNum(UINT32 maxRunningTaskNum)
{
	UINT32 tmpNum = maxRunTaskNum;
	maxRunTaskNum = maxRunningTaskNum;
	return tmpNum;
}

INT32 TranferManager::setProxy(Proxy_t proxy)
{
	INT32 ret = useProxy;
	
	if(proxy.type >= HTTPDOWN_PROXY_HTTP && proxy.type < HTTPDOWN_PROXY_BUTT)
	{
		proxyInfo.type = proxy.type;
		proxyInfo.host = proxy.host;
		if(proxy.user.length() == 0)
		{
			proxyInfo.user = "";
		}
		else
		{
			proxyInfo.user = proxy.user;
		}
		
		proxyInfo.pwd  = proxy.pwd;
		proxyInfo.domain = proxy.domain;
		proxyInfo.port = proxy.port;
		useProxy = true;
		LOG_DEBUG("[setProxy]proxy add success. proxyInfo.type =%d proxyInfo.host=%s proxyInfo.user=%s proxyInfo.pwd=%s proxyInfo.domain=%s proxyInfo.port=%d\n",
					proxyInfo.type, proxyInfo.host.c_str(), proxyInfo.user.c_str(),
					proxyInfo.pwd.c_str(), proxyInfo.domain.c_str(), proxyInfo.port);
	}
	else
	{
		if(proxy.type == HTTPDOWN_PROXY_BUTT)
		{
			useProxy = false;
		}
		else
		{
			LOG_DEBUG("[setProxy]proxy add fail.\n");
			ret = -1;
		}
	}
	return ret;
}

INT32 TranferManager::taskParamCheck(string url, 
					TASK_TYPE	  task_type,
					POST_PARAMS*  postParams,
					CALL_BACK_S	  callbacks,
					OPTIONS		  options,
					HttpHdrs * httpHdrs,
					ULONG64& postDataSize)
{
	//首先检查URL
	if(url.length() <= 0)
	{
		LOG_ERROR("[taskParamCheck]Fail, url Too Short.\n");
		return ERR_TASK_PARAM_URL;
	}


	//检查成功，失败回调函数
	if(NULL == callbacks.handle_finish || NULL == callbacks.handle_error)
	{
		LOG_ERROR("[taskParamCheck]Fail, handle_finish OR handle_error is NULL.\n");
		return ERR_TASK_PARAM_CALLBACK;
	}

	//针对post的检查
	if(TASK_UPLOAD == task_type || TASK_QUERY == task_type)
	{
		//检查post参数
		if(NULL == postParams)
		{
			return ERR_TASK_PARAM_POSTPARAM_NONE;	//POST参数为空
		}
		
		PARAMS_V* pParams = postParams->get_params();
		for(UINT i = 0; i < pParams->size(); i++)
		{
			char* paramName = (*pParams)[i].paramName;
			char* paramBuffer = (*pParams)[i].paramBuffer;
			UINT32 paramBufferLen = (*pParams)[i].paramBufferLen;
			PARAM_TYPE paramType = (*pParams)[i].paramType;
			bool fileExist = true;

			if(NULL == paramName || 0 == strlen(paramName))
			{
				return ERR_TASK_PARAM_POSTPARAM_NAME;	//Post 参数的参数名不合法
			}
			
			if(0 == paramBufferLen && paramType == PARAM_BINARY)
			{
				return ERR_TASK_PARAM_POSTPARAM_LEN;	//Post 参数的参数值长度不合法
			}

			if(NULL == paramBuffer)
			{
				return ERR_TASK_PARAM_POSTPARAM_VALUE;
			}
			
			
			switch(paramType)
			{
				case PARAM_TEXT:
				{
//						if(0 == strlen(paramBuffer))
//						{
//							return ERR_TASK_PARAM_POSTPARAM_VALUE;	//Post 参数的参数值不合法	
//						}
					postDataSize += HTTP_POSTPARAM_BROUNDARY_FIXLEN + strlen("\r\n")
									+ POSTPARAM_TEMPLATE_TEXT_LEN
									+ strlen(paramName) 
									+ strlen(paramBuffer) 
									+ strlen("\r\n");
					break;
				}
				case PARAM_FILENAME:
				{
					if(0 == strlen(paramBuffer))
					{
						return ERR_TASK_PARAM_POSTPARAM_VALUE;	//Post 参数的参数值不合法	
					}

					//检查文件是否存在
					ULONG64 filesize = 0;
					LONG64 realSize = 0;
					fileExist = IsTargetFileExist(paramBuffer, filesize);
					if(!fileExist)
					{
						return ERR_TASK_PARAM_POSTPARAM_FILENOTEXIST;	//Post 参数中指定的文件不存在							
					}

					if(paramBufferLen > 0 && filesize <= paramBufferLen)
					{
						return ERR_TASK_PARAM_POSTPARAM_VALUE;
					}

					realSize = (paramBufferLen > 0) ? (filesize - paramBufferLen):filesize;

					postDataSize += HTTP_POSTPARAM_BROUNDARY_FIXLEN + strlen("\r\n")
									+ POSTPARAM_TEMPLATE_FILE_LEN
									+ strlen(paramName) 
									+ strlen(paramBuffer)
									+ strlen(getMimeType(paramBuffer))									
									+ realSize
									+ strlen("\r\n");	//文件数据结尾处需要追加"\r\n"
								
					break;
				}
				case PARAM_BINARY:
				{
					postDataSize += HTTP_POSTPARAM_BROUNDARY_FIXLEN + strlen("\r\n")
									+ POSTPARAM_TEMPLATE_BINARY_LEN
									+ strlen(paramName) 
									+ paramBufferLen 
									+ strlen("\r\n");
					break;
				}
				default:
					return ERR_TASK_POSTPARAM_TYPE_INVALID;	//Post 参数的参数类型不合法
			}
		}	
		//Post 数据必须以HTTP_POSTPARAM_BROUNDARY+"--\r\n"结尾
		postDataSize += HTTP_POSTPARAM_BROUNDARY_FIXLEN+strlen("--\r\n");
	}
	else if(TASK_BINARY == task_type)
	{		
		//参数值存储在POST_PARAMS中，只能有一个参数，
		//且参数类型必须为 PARAM_BINARY, 参数名为anonymous

		//检查post参数
		if(NULL == postParams)
		{
			return ERR_TASK_PARAM_POSTPARAM_NONE;	//POST参数为空
		}
		PARAMS_V* pParams = postParams->get_params();
		UINT i = (UINT)pParams->size(); 
		if(i != 1)
		{
			return ERR_TASK_PARAM_POSTPARAM_EXCEED;	//POST参数超出限制
		}
		else
		{
			char* paramName = (*pParams)[i].paramName;
			char* paramBuffer = (*pParams)[i].paramBuffer;
			UINT32 paramBufferLen = (*pParams)[i].paramBufferLen;
			PARAM_TYPE paramType = (*pParams)[i].paramType;
			//bool fileExist = true;

			if(paramType != PARAM_BINARY)
			{
				return ERR_TASK_POSTPARAM_TYPE_INVALID; //Post 参数的参数类型不合法
			}
			if(NULL == paramName || 0 == strlen(paramName) || NULL == paramBuffer)
			{
				return ERR_TASK_PARAM_POSTPARAM_NAME;	//Post 参数的参数名不合法
			}			
			if(0 == paramBufferLen)
			{
				return ERR_TASK_PARAM_POSTPARAM_LEN;	//Post 参数的参数值长度不合法
			}
			
			postDataSize = paramBufferLen;
		}
	}
	
	if(options.startSize != 0 || options.endSize != 0)
	{
		if(options.startSize < 0 || options.endSize < 0
			|| (options.startSize > options.endSize && options.endSize != 0))
		{
			return ERR_TASK_OPTION_SIZE_INVALID;
		}		
	}	
	
	return RET_OK;
}

INT32 TranferManager::addTask(string url, 
					TASK_TYPE	  task_type,
					POST_PARAMS* 	  postParams,
					CALL_BACK_S	  callbacks,
					OPTIONS		  options,
					HttpHdrs * httpHdrs)

{
	INT32 ret = 0;	
	int taskPos = 0;
	TTask* pTask = NULL;
	UINT32 runningTaskCount = 0;
	ULONG64 postDataSize = 0;
	
	LOG_INFO("[addTask]Begin add task url=%s.\n", url.c_str());

	ret = taskParamCheck(url, 
						task_type, 
						postParams,
						callbacks,
						options,
						httpHdrs,
					  	postDataSize);

	if(RET_OK != ret)
	{
		LOG_ERROR("[addTask]taskParamCheck fail ret=%d.\n", ret);
		return -ret;
	}

	if(!mgrStarted)
	{		
		LOG_ERROR("[addTask]Fail, Mgr not started.\n");
		return -ERR_TASK_MGR_NOT_START;
	}
	LOG_DEBUG("[addTask]Mgr already started.\n");
	
	if(!useProxy)
	{	
		LOG_DEBUG("[addTask]without proxy.\n");
		pTask = new TTask(url, task_type);
	}
	else
	{
		LOG_DEBUG("[addTask]with proxy.\n");
		pTask = new TTask(url, task_type, proxyInfo);
	}
	LOG_DEBUG("[addTask]New task finished %p.\n", pTask);

	//pTask->cookies			= cookies;
	//pTask->postParams 		= postParams;
	pTask->postParams		= NULL;
	if(NULL != postParams)
	{
		pTask->postParams		= new POST_PARAMS(*postParams);
	} 	
	pTask->httpHdrs			= httpHdrs;
	//pTask->fileNames		= fileNames;
	pTask->handle_init 		= callbacks.handle_init;
	pTask->handle_process 	= callbacks.handle_process;
	pTask->handle_headcomplete = callbacks.handle_headcomplete;
	pTask->handle_finish 	= callbacks.handle_finish;
	pTask->handle_error 	= callbacks.handle_error;
	//pTask->respData			= respData;
	pTask->postDataSize		= postDataSize;
	pTask->timeoutlimit		= options.tasktimeout;	
	pTask->startSize = options.startSize;			
	pTask->endSize = options.endSize;		
	pTask->dataClean = options.dataClean;		
	
	runningTaskCount = pWorker->getRunningTaskCount();

	LOG_DEBUG("[addTask]runningTaskCount=%d, maxRunTaskNum=%d.\n",
			runningTaskCount, maxRunTaskNum);
	
	pTask->taskState = TSK_IDLE;
	
	pTask->last_active_time = time(NULL);

	//由于同一时间只会存在一个transferMgr执行AddTask任务，
	//所以不需要对任务队列加锁
	taskPos = pWorker->findTaskPos();
	LOG_INFO("[addTask]findTaskPos ret=%d.\n", taskPos);

	if(taskPos < 0)
	{
		delete pTask;
		ret = ERR_TASK_FULL;
	}
	else
	{
		taskQue[taskPos] = pTask;
		return taskPos;
	}
	
	LOG_WARN("[addTask]ret=%d.\n", -ret);

	return -ret;
}



INT32 TranferManager::cancelTask(UINT32 taskID, TaskInfo& taskinfo)
{
	INT32 ret = 0;
	
	if(!mgrStarted)
	{		
		LOG_DEBUG("[addTask]Fail, Mgr not started.\n");
		return ERR_TASK_MGR_NOT_START;
	}	

	thread_lock(&pWorker->task_que_lock); 
	if(NULL != taskQue[taskID])
	{
//		taskQue[taskID]->updateTaskInfo();
//		memcpy(&taskinfo, &(taskQue[taskID]->taskInfo), sizeof(TaskInfo));	//将指定task的详细信息记录下来
		taskQue[taskID]->del = true;
	}
	else
	{
		ret = ERR_TASK_NOT_EXIST;
	}
	thread_unlock(&pWorker->task_que_lock); 
	return ret;
}

INT32 TranferManager::clear()
{
	int i = 0;

	if( !mgrStarted)
	{		
		LOG_DEBUG("[clear]Fail, Mgr not started.\n");
		return ERR_TASK_MGR_NOT_START;
	}	

	
	for(i = 0; i < MAX_TASKQUE_SIZE; i++)
	{
		thread_lock(&pWorker->task_que_lock); 
		if(NULL != taskQue[i])
		{
			taskQue[i]->del = true;
		}
		thread_unlock(&pWorker->task_que_lock); 
	}
	return 0;
}

INT32 TranferManager::stop()
{
	int ret = 0;
	if(!mgrStarted)
	{		
		LOG_DEBUG("[stop]Fail, Mgr not started.\n");
		ret = ERR_TASK_MGR_NOT_START;
	}	

	if(NULL != pWorker)
	{		
		clear();
		pWorker->stop = true;
		delay_ms(1000);
		pWorker = NULL;	//由线程启动函数自行删除pWorker
		
		mgrStarted = false;
	}

	LOG_DEBUG("[stop]ret=%d.\n", ret);

	
	return ret;
}

void TranferManager::setLogPath( char* local_log_path)
{

#ifndef	 HTTPTRANSFER_RELEASE
	LOG_CLOSE();
	strncpy( log_path, local_log_path, 1024);
	//log_path = local_log_path;	
	
	if(log_path != NULL)
	{
#ifdef WIN32
		LOG_INIT(log_path,
			10000000,
			HTTPTRANSFER_LOG_LEVEL);

#else
		LOG_INIT(log_path,
			10000000,
			HTTPTRANSFER_LOG_LEVEL);
#endif
	}
	else
	{
#ifdef WIN32
		//创建日志目录
		_mkdir("Log");
		LOG_INIT("log\\httptransfer",
			10000000,
			HTTPTRANSFER_LOG_LEVEL);

#else
		mkdir("../Log");
		LOG_INIT("../Log/httptransfer",
			10000000,
			HTTPTRANSFER_LOG_LEVEL);
#endif
	}
	/*int ret = */LOG_OPEN();
#endif

}


TranferManager::TranferManager()
{
	lastTaskQuePos = 0;
	waitingTaskCount = 0;
	runningTaskCount = 0;
	finshedTaskCount = 0;
	errorTaskCount	 = 0;
	maxRunTaskNum = MAX_RUN_TASK;
	useProxy = false;
	mgrStarted = false;
	
	pWorker = NULL;
	taskQue = NULL;	
	
	LOG_INFO("TranferManager GENERATE...");
}

TranferManager::~TranferManager()
{
	stop();
#ifndef	 HTTPTRANSFER_RELEASE	
	LOG_CLOSE();
#endif
}

INT32 TranferManager::start()
{
#ifndef	 HTTPTRANSFER_RELEASE
	if(log_path != NULL)
	{
#ifdef WIN32
		LOG_INIT(log_path,
			10000000,
			HTTPTRANSFER_LOG_LEVEL);

#else
		LOG_INIT(log_path,
			10000000,
			HTTPTRANSFER_LOG_LEVEL);
#endif
	}
	else
	{
#ifdef WIN32
		//创建日志目录
		_mkdir("Log");
		LOG_INIT("log\\httptransfer",
			10000000,
			HTTPTRANSFER_LOG_LEVEL);

#else
		mkdir("../Log");
		LOG_INIT("../Log/httptransfer",
			10000000,
			HTTPTRANSFER_LOG_LEVEL);
#endif
	}
	/*int ret = */LOG_OPEN();
#endif

	LOG_INFO("[start]begin.\n");
	
	if(mgrStarted)
	{		
		LOG_ERROR("[start]Fail, Already started.\n");
		return ERR_TASK_ALREADY_STARTED;
	}	

#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if( err != 0 ) 
	{
		LOG_ERROR("[start]Fail, Init socket fail.\n");
		return ERR_TASK_SOCKET_INIT_FAIL;
	}
	if( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		LOG_ERROR("[start]Fail, socket Version not correct.\n");
		WSACleanup();
	  	return ERR_TASK_SOCKET_WRONG;
	}
#endif

	if(NULL != pWorker)
	{
		LOG_WARN("[TranferManager]Start Old worker exist, delete it.\n");
		delete pWorker;
	}
	
	pWorker = new TTransferWorker(maxRunTaskNum);
	taskQue = pWorker->taskQue;

	//Todo:   startThread   pthread_create(run, pWorker);
	#ifdef WIN32
	m_hThread = (HANDLE)_beginthreadex(NULL,0,run,(void*)pWorker,0,NULL);
	//m_hThread = (HANDLE)_beginthreadex(run, 0, (void*)pWorker);
	#else
	pthread_create(&m_hThread, NULL, run, (void*)pWorker);
	#endif
	if(m_hThread <= 0)
	{
		LOG_ERROR("[TranferManager]Start thread fail tid=%d err=%d errinfo=%s.\n",
					m_hThread, errno, strerror(errno));
		return ERR_TASK_THREAD_START_FAIL;
	}
	delay_ms(1000);
	mgrStarted = true;
	
	LOG_INFO("[TranferManager]Start thread success tid=%d.\n",
				m_hThread);
	return 0;
}

}


