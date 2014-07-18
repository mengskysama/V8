#include "stdafx.h"
#include "HttpClient.h"
#include "stdio.h"
#include "utility/stringhelper.h"
#include "utility/FileHelper.h"
#include "assert.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sstream>

#include "HttpHandler.h"
#include "HttpContext.h"
#include "WebModule.h"

#include "../../../include/Log.h"
#include "../CommonLibrary/src/HttpClient/include/httpmanager.h"


using namespace webinterface;
using namespace common::utility;

#define	END_FLAG_LENGTH		256


//==========================================================================================================//
//																											//
//                          HttpClient的实现代码															//
//																											//
//==========================================================================================================//

HttpClient* HttpClient::Singleton = 0;


BOOL HttpClient::DetectIEProxySettings(WINHTTP_PROXY_INFO* pProxyInfo)
{
	BOOL bResult = FALSE;

	WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieProxyConfig;
	::ZeroMemory(&ieProxyConfig, sizeof(ieProxyConfig));

	if (::WinHttpGetIEProxyConfigForCurrentUser(&ieProxyConfig))
	{
		if (ieProxyConfig.lpszProxy != NULL)
		{
			pProxyInfo->dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
			pProxyInfo->lpszProxy = ieProxyConfig.lpszProxy;
			pProxyInfo->lpszProxyBypass = ieProxyConfig.lpszProxyBypass;
			bResult = TRUE;
		}
	}

	return bResult;
}

HttpClient::HttpClient( )
{
	m_pTransferManager = NULL;
	m_pTransferManager = TranferManager::getInstance();

	if( m_pTransferManager)
	{
		m_pTransferManager->setLogPath( const_cast<char*>(stringhelper::UnicodeToAscii(g_WebModule->GetLogPath(GetCurInfo().unUIN)).c_str()));

		INT32 nRet = m_pTransferManager->start();
		if( nRet != 0 )
		{
			m_pTransferManager = NULL;
		}
		else
		{
			m_Callback.handle_init = handle_init;
			m_Callback.handle_headcomplete = handle_headcomplete;
			m_Callback.handle_process = handle_process;
			m_Callback.handle_finish = handle_finish;
			m_Callback.handle_error = handle_error;
		}
	}

	Singleton = this;
}

void	
HttpClient::SetNoProxy()
{
	Proxy_t proxyInfo;

	m_pTransferManager = TranferManager::getInstance();
	proxyInfo.type = HTTPDOWN_PROXY_BUTT;
	m_pTransferManager->setProxy(proxyInfo);
}

HttpClient::~HttpClient()
{
	if( m_pTransferManager)
	{
		m_pTransferManager->stop();
	}

	map<unsigned int, HTTPCONTEXT*>::iterator iterMap;
	for(iterMap = m_mapRequest.begin(); iterMap != m_mapRequest.end(); ++iterMap)
	{
		if( iterMap->second)
		{
			delete iterMap->second;
			iterMap->second = NULL;
		}
	}
	m_mapRequest.clear();

	m_mapTaskid2Seqno.clear();
	m_mapSeqNo2TaskId.clear();
	m_lstResult.clear();

	TranferManager::delInstance();
	Singleton = NULL;
}

HttpClient*	HttpClient::Instance()
{
	if( Singleton == NULL)
		Singleton = new HttpClient();

	assert( Singleton != 0);

	return Singleton;
}

void	
HttpClient::ClearSeqIdMap( uint32 nSeqNo)
{
	std::map< uint32, int32>::iterator itr = m_mapSeqNo2TaskId.find( nSeqNo);
	if( itr != m_mapSeqNo2TaskId.end())
	{
		m_mapTaskid2Seqno.erase(itr->second);

		TaskInfo taskInfo;
		m_pTransferManager->cancelTask( itr->second, taskInfo);
	}
	m_mapSeqNo2TaskId.erase( nSeqNo);
}

void    
HttpClient::DeleteSeqNo( uint32 nSeqNo)
{
	ClearSeqIdMap( nSeqNo);

	map<uint32, HTTPCONTEXT*>::iterator itr = m_mapRequest.find( nSeqNo);
	if( itr != m_mapRequest.end() && itr->second)
	{
		if( itr->second->pArgument)
		{
			delete itr->second->pArgument;
			itr->second->pArgument = NULL;
		}

		delete itr->second;
		itr->second = NULL;
	}

    m_mapRequest.erase( nSeqNo);
}

void	
HttpClient::CleanAll()
{
	map<uint32, HTTPCONTEXT*>::iterator itr = m_mapRequest.begin();
	for( ;itr != m_mapRequest.end(); itr++)
	{
		if( itr->second)
		{
			if( itr->second->pArgument)
			{
				delete itr->second->pArgument;
				itr->second->pArgument = NULL;
			}

			delete itr->second;
			itr->second = NULL;
		}
	}

	m_mapRequest.clear();

	m_mapTaskid2Seqno.clear();
	m_mapSeqNo2TaskId.clear();
	m_lstResult.clear();
}


HTTPCONTEXT*	
HttpClient::GetContext( unsigned int nSeqNo)
{
	std::map< unsigned int, HTTPCONTEXT*>::iterator itr = m_mapRequest.find( nSeqNo);
	if( itr == m_mapRequest.end())
		return NULL;

	return itr->second;
}

/**
* Function 请求处理过程中失败的时候进行的清理工作
* @return 
**/
void 
HttpClient::Cleanup( HTTPCONTEXT* pContext)
{
	m_Lock.Enter();
    m_lstResult.push_back( *pContext);
	m_Lock.Leave();
}

int32	
HttpClient::ReqURL( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
	{
		assert(0);
		return -1;
	}

	if( m_pTransferManager == NULL)
	{
		LOG_ERROR(L"ReqURL_m_pTransferManager为空");
		return -1;
	}

	m_Lock.Enter();
	m_mapRequest[pContext->nReqSeq] = pContext;
	m_Lock.Leave();

	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;
	if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
		stOpts.dataClean = true;

	TASK_TYPE taskType = TASK_DOWNLOAD;

	if(pContext->bResumable) //断点续传
	{
		if(pContext->nResumableHead)  //断点续传时获取头信息
		{
			taskType = TASK_HEAD;   //使用HEAD向服务器获取http响应头
		}
		else  //断点续传时获取数据
		{
			DWORD localSize = 0;
			if(pContext->hFileHandle == NULL)
			{
				localSize = common::utility::filehelper::GetFileSize(pContext->wstrWritedFileName.c_str());
			}
			else
			{
				localSize = ::GetFileSize(pContext->hFileHandle, 0);
			}
			 
			if(localSize < 0 )
			{
				localSize = 0;
			}		
			stOpts.startSize = localSize;

			taskType = TASK_DOWNLOAD; //使用Get从服务器下载文件
		}
		
	}	

	int32 ret = m_pTransferManager->addTask( 
		pContext->strURL, 
		taskType, 
		NULL, 
		m_Callback, 
		stOpts
		);

	LOG_TRACE(L"\n\n\n");
	LOG_TRACE(L"=========================================================");
	LOG_TRACE(L"HttpClient AddTask");
	LOG_TRACE(L"URL : %s", stringhelper::AsciiToUnicode(pContext->strURL).c_str());
	LOG_TRACE(L"SeqNo : %d", pContext->nReqSeq);
	LOG_TRACE(L"ret: %d", ret);
	LOG_TRACE(L"尝试次数: %d", pContext->nFailedTime);
	LOG_TRACE(L"=========================================================\n\n");

	if( ret < 0)
		return ret;

	pContext->nTaskId = ret;
	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;

	return 0;
}

int32 
HttpClient::ReReqURL( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
		return -1;

	if( m_pTransferManager == NULL)
	{
		LOG_ERROR(L"ReqURL_m_pTransferManager为空");
		return -1;
	}

	HTTPCONTEXT* pCxt = GetContext( pContext->nReqSeq);
	if( pCxt == NULL)
		return -1;

	m_Lock.Enter();
	pCxt->nFailedTime = pContext->nFailedTime;
	m_Lock.Leave();

	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;	
	if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
		stOpts.dataClean = true;

	TASK_TYPE taskType = TASK_DOWNLOAD;
	if(pContext->bResumable) //断点续传
	{
		if(pContext->nResumableHead)  //断点续传时获取头信息
		{
			taskType = TASK_HEAD;   //使用HEAD向服务器获取http响应头
		}
		else  //断点续传时获取数据
		{
			DWORD localSize = 0;
			if(pContext->hFileHandle == NULL)
			{
				localSize = common::utility::filehelper::GetFileSize(pContext->wstrWritedFileName.c_str());
			}
			else
			{
				localSize = ::GetFileSize(pContext->hFileHandle, 0);
			}

			if(localSize < 0 )
			{
				localSize = 0;
			}			
			stOpts.startSize = localSize;

			taskType = TASK_DOWNLOAD; //使用Get从服务器下载文件
		}

	}

	int32 ret = m_pTransferManager->addTask(
		pContext->strURL, 
		taskType, 
		NULL, 
		m_Callback, 
		stOpts
		);

	LOG_WARN(L"\n\n\n=========================================================");
	LOG_WARN(L"HttpClient AddTask");
	LOG_WARN(L"URL : %s", stringhelper::AsciiToUnicode(pContext->strURL).c_str());
	LOG_WARN(L"SeqNo : %d", pContext->nReqSeq);
	LOG_WARN(L"ret: %d", ret);
	LOG_WARN(L"尝试次数: %d", pContext->nFailedTime);
	LOG_WARN(L"=========================================================\n\n");

	if( ret < 0)
		return ret;

	pContext->nTaskId = ret;
	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;

	return 0;
}

int32	
HttpClient::ReqPostUrl( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
		return -1;

	if( m_pTransferManager == NULL)
	{
		LOG_ERROR(L"ReqURL_m_pTransferManager为空");
		return -1;
	}

	/** 对Map进行操作的时候需要加锁*/
	m_Lock.Enter();
	m_mapRequest[pContext->nReqSeq] = pContext;
	m_Lock.Leave();

	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;
    if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
        stOpts.dataClean = true;

	int ret = 0;
	if( pContext->pArgument->is_uploadtype())
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_UPLOAD, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}
	else
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_QUERY, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}

	LOG_WARN(L"\n\n\n=========================================================");
	LOG_WARN(L"HttpClient AddTask");
	LOG_WARN(L"URL : %s" , stringhelper::AsciiToUnicode(pContext->strURL).c_str());
	LOG_WARN(L"SeqNo : %d", pContext->nReqSeq);
	LOG_WARN(L"ret : %d", ret);
	LOG_WARN(L"尝试次数 : %d", pContext->nFailedTime);
	LOG_WARN(L"=========================================================\n\n");

	if( ret < 0 )
	{
		assert(0);
		return ret;
	}

	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;
	pContext->nTaskId = ret;

	return 0;
}

int32 
HttpClient::ReReqPostUrl( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
		return -1;
	
	if( m_pTransferManager == NULL)
	{
		LOG_ERROR(L"ReqURL_m_pTransferManager为空");
		return -1;
	}

	HTTPCONTEXT* pCxt = GetContext( pContext->nReqSeq);
	if( pCxt == NULL)
		return -1;

	m_Lock.Enter();
	pCxt->nFailedTime = pContext->nFailedTime;
	m_Lock.Leave();

	/** 对Map进行操作的时候需要加锁*/
	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;
    if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
        stOpts.dataClean = true;

	int ret = 0;
	if( pContext->pArgument && pContext->pArgument->is_uploadtype())
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_UPLOAD, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}
	else
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_QUERY, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}

	LOG_TRACE(L"\n\n\n=========================================================");
	LOG_TRACE(L"HttpClient AddTask");
	LOG_TRACE(L"URL : %s", stringhelper::AsciiToUnicode(pContext->strURL).c_str());
	LOG_TRACE(L"SeqNo : %d", pContext->nReqSeq);
	LOG_TRACE(L"ret : %d", ret);
	LOG_TRACE(L"尝试次数 : %d", pContext->nFailedTime);
	LOG_TRACE(L"=========================================================\n\n");

	if( ret < 0 )
		return ret;

	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;
	pContext->nTaskId = ret;

	return 0;
}

void	
HttpClient::CancelTask( uint32 nSeqNo)
{
	HTTPCONTEXT* pContext = GetContext(nSeqNo);
	if( pContext && pContext->hFileHandle)
	{
		::CloseHandle( pContext->hFileHandle);
	}

	TaskInfo taskInfo;

	int taskId = 0;
	std::map<uint32, int32>::iterator itr = m_mapSeqNo2TaskId.find( nSeqNo);
	if( itr != m_mapSeqNo2TaskId.end())
	{
		taskId = itr->second;
		
		if( m_pTransferManager)
			m_pTransferManager->cancelTask( taskId, taskInfo);

		m_mapSeqNo2TaskId.erase( nSeqNo);
		m_mapTaskid2Seqno.erase( taskId);
		m_mapRequest.erase( nSeqNo);
	}
}

/**
* Function 从响应集中获取一个响应
* @param stContext 返回的响应
* @return 返回true 表示返回的是一个有效的响应，false则表示是一个无效的响应
* stContext中的数据是无效数据
**/
HTTPCONTEXT 
HttpClient::RetriveResp( )
{
	HTTPCONTEXT stContext;
	stContext.uOpPolicy = INVALID_RESP;

	m_Lock.Enter();
	if(m_lstResult.size())
	{
		stContext = m_lstResult.front();
		m_lstResult.pop_front();
	}
	m_Lock.Leave();

	return stContext;
}

int32	
HttpClient::GetSeqNo( int32	nTaskId)
{
	std::map< int32, uint32 >::iterator itr = m_mapTaskid2Seqno.find( nTaskId);
	if( itr == m_mapTaskid2Seqno.end())
		return -1;

	return itr->second;
}
//#endif