#include "stdafx.h"
#include "RequestManager.h"
#include "process.h"
#include "utility/stringhelper.h"
#include "../../include/log.h"
#include "HttpClient.h"
#include "WebModule.h"

#define MAX_RETRY_TIMES	3//最大重试次数

using namespace webinterface;
using namespace common::utility;

CRequestManager::CRequestManager()
{
	m_pHttpClient = NULL;
	m_ResponseParser.SetReuqestManager(this);
	m_nReqSeq = 100;			//	序号从100开始
}

CRequestManager::~CRequestManager()
{
	if(m_pHttpClient)
	{
		delete m_pHttpClient;
		m_pHttpClient = NULL;
	}
}

void 
CRequestManager::Init()
{
	m_pHttpClient->SetNoProxy();
}

void 
CRequestManager::Clear()
{
	if( m_pHttpClient)
		m_pHttpClient->CleanAll();
}

int 
CRequestManager::GetRetCodeByStatus( int nStatusCode)
{
	switch( nStatusCode)
	{
		case 200: 	return WEB_RET_SUCCESS;
		case 401:	return WEB_RET_COMMON_NO_AUTH;
		case 403:	return WEB_RET_COMMON_NO_PERMIT;
		case 404:	return WEB_RET_COMMON_NO_URL;
		case 405:	return WEB_RET_COMMON_METHOD_NOT_SUPPORT;
		case 500: 	return WEB_RET_COMMON_SERVER_INTERNAL_ERROR;
		case 407:	return WEB_RET_COMMON_PROXY_NEED_AUTH;
		case 501:	return WEB_RET_COMMON_SERVER_NOT_IMPL;
		case 503:	return WEB_RET_COMMON_SERVER_NOT_AVAIL;
		default:	return WEB_RET_COMMON_ERROR_INTERNAL;
	}
}

uint32 
CRequestManager::PushUrl(EventValue nEventValue, int64 nParam0, wstring strUrl, POST_PARAMS* pArgument, 
						 RequestType eReqType, wstring wstrWritedFileName,  UINT nTimeout,BOOL bAllowRepeated,BOOL bResumable)
{
	POST_PARAMS* pPostArgument= NULL;
	if( pArgument != NULL)
	{
		pPostArgument = new POST_PARAMS(*pArgument);
	}

	HTTPCONTEXT* pCxt = new HTTPCONTEXT( stringhelper::UnicodeToAscii(strUrl));
	pCxt->nEventValue = nEventValue;
	pCxt->uParam0 = nParam0;
	pCxt->strURL = stringhelper::UnicodeToAscii( strUrl);
	pCxt->eReqType = eReqType;
	pCxt->wstrWritedFileName = wstrWritedFileName;
	pCxt->pArgument = pPostArgument;
	pCxt->nTimeout	=	 nTimeout/MAX_RETRY_TIMES;
	pCxt->bAllowRepeated = bAllowRepeated;
	pCxt->bResumable = bResumable;
	pCxt->nReqSeq = ++m_nReqSeq;

	m_vRequestQueue.push_back( pCxt);

	return pCxt->nReqSeq;
}

uint32		
CRequestManager::PushUrl(  HTTPCONTEXT* pContext)
{
	HTTPCONTEXT* pCxt = new HTTPCONTEXT(*pContext);
	pCxt->nReqSeq = ++m_nReqSeq;
	m_vRequestQueue.push_back( pCxt);

	return pCxt->nReqSeq;
}

uint8 
CRequestManager::PopUrl( HTTPCONTEXT*& pContext )
{
	std::vector<HTTPCONTEXT*>::iterator itr = m_vRequestQueue.begin();
	if( itr == m_vRequestQueue.end())
		return 0;

	pContext = *itr;
	m_vRequestQueue.erase( itr);
	return 1;
}

/**
* Function		检查给定的URL是否已经在请求队列中
*/	
BOOL		
CRequestManager::IsExistUrl( string strUrl)
{
	for( size_t i = 0; i<m_vRequestQueue.size(); i++)
	{
		HTTPCONTEXT* pCxt = m_vRequestQueue[i];
		if( pCxt)
		{
			if( pCxt->strURL ==  strUrl)
				return TRUE;
		}
	}

	return FALSE;
}

void 
CRequestManager::ReqUrl( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
	{
		assert(0);
		return;
	}

	if( pContext->strURL != "")
	{
		// 不允许重复的URL
		if( pContext->bAllowRepeated == FALSE)
		{
			if( IsExistUrl( pContext->strURL) == TRUE)
			{
				LOG_ERROR(L"%s 已经存在，并且不允许重复，不再请求该URL", stringhelper::AsciiToUnicode(pContext->strURL.c_str()));
				return;
			}
		}

		// 未离线的情况下才进行请求
		if( pContext->pArgument == NULL)
			m_pHttpClient->ReqURL( pContext);
		else
			m_pHttpClient->ReqPostUrl( pContext);
	}
}

void	
CRequestManager::ReReqUrl( string strUrl, HTTPCONTEXT* pContext, POST_PARAMS* pArgument)
{
	if( strUrl == "" || pContext == NULL)
		return;

	m_pHttpClient->ClearSeqIdMap( pContext->nReqSeq);

	if(pArgument == NULL)
		m_pHttpClient->ReReqURL( pContext);
	else
		m_pHttpClient->ReReqPostUrl( pContext);
}

void		
CRequestManager::CancelTask( uint32 nSeqNo)
{
	if( m_pHttpClient)
		m_pHttpClient->CancelTask( nSeqNo);
}


// 该函数的主要作用就是从HTTPClient的返回队列中不断的取出已经处理过的
// 结果，或者是返回的报文数据，或者是操作失败的结果
// 函数的返回值的意义如下：
//	-1		没有取到有效的响应
//	0		表示正在进行重新请求，因此返回的HTTPCONTEXT不会被删除
//	WEB_RET_SUCCESS	正常处理完毕
//	其余>0		发生了其余的错误，

HTTPCONTEXT
CRequestManager::ProcessResp()
{
	HTTPCONTEXT httpContext = m_pHttpClient->RetriveResp();
	
	if( httpContext.uOpPolicy == INVALID_RESP)
		return httpContext;

	string strUrl = httpContext.strURL;
	if( strUrl == "")	
	{
		httpContext.uOpPolicy = INVALID_RESP;
		return httpContext;
	}

	/** 当前请求处理中发生了失败，没有接收到服务器的响应*/
	/** 磁盘写入错误单独处理*/
	if( httpContext.uResult == HTTP_FAILED )
	{
		if( httpContext.nErrorCode != ERR_TASK_WRITE_FILE_ERROR)
		{
			httpContext.nFailedTime++;

			// 如果总的尝试次数小于三次，此时则再次发送该请求
			if( httpContext.nFailedTime < MAX_RETRY_TIMES)
			{
				ReReqUrl( strUrl, &httpContext, httpContext.pArgument);
				httpContext.uOpPolicy = REPEATED_REQ;
				return httpContext;
			}
			
			httpContext.uOpPolicy =  WEB_RET_NET_ERROR_TIMEOUT;
			return httpContext;
		}

		LOG_ERROR(L"\n\n");
		LOG_ERROR(L"=================== %s FATAL ERROR BEGIN==================", __FUNCTIONW__);
		LOG_ERROR(L"磁盘写入错误");
		LOG_ERROR(L"==========================FATAL ERROR END  ====================================\n\n");
		
		// 写磁盘错误不需要重试，直接返回
		httpContext.uOpPolicy =  WEB_RET_COMMON_WRITE_FILE_ERROR;
		return httpContext;
	}

	/** 
	 * 接收到服务器返回的报文，此时检测返回的报文的StatusCode，
	 * 只有200的状态码才是正确的
	 */
	if( httpContext.nHttpCode != 200 && httpContext.nErrorCode != 0)
	{
		LOG_ERROR(L"服务器返回的状态码错误 %d", httpContext.nHttpCode);
		httpContext.uOpPolicy =  GetRetCodeByStatus( httpContext.nHttpCode);
		return httpContext;
	}

	/** 
	 * 成功接收到服务器的响应报文，对该报文进行解析，检查是否正确，并根据解析结果
	 * 进行返回
	 */
	int nEltsNum = 0;
	m_ResponseParser.SetUin( httpContext.uParam0);
	m_ResponseParser.ParseResponse( &httpContext);

	httpContext.uOpPolicy =  SUCCESS_RESP;
	return httpContext;
}

bool CRequestManager::QueryDownloadProcess( uint32 nSeqNo, uint32* uPercent, 
											  uint32* uTotalLength, uint32* uFinishedLength, uint32* uSpeed, int32* uRemainedTime)
{
	HTTPCONTEXT* pHttpContext = m_pHttpClient->GetContext( nSeqNo);
	if( pHttpContext == NULL)
		return false;

	if( uTotalLength)
		*uTotalLength = pHttpContext->nContentLength;

	if( uFinishedLength)
		*uFinishedLength = pHttpContext->nFinishedLength;

	if( uPercent)
	{
		if(0 == pHttpContext->nContentLength)
		{
			*uPercent = 0;
		}
		else
		{
			//*uPercent = (int)(pHttpContext->nFinishedLength*100/pHttpContext->nContentLength);   此方法计算进度时， 数值太大会出错
			*uPercent = (int)(pHttpContext->nFinishedLength*1.0/pHttpContext->nContentLength*100);
		}
	}

	if( uSpeed)
		*uSpeed = pHttpContext->nSpeed;
	else
		pHttpContext->nSpeed = 0;

	if( uRemainedTime)
		*uRemainedTime = pHttpContext->nRemainedTime;
	else
		*uRemainedTime = -1;


	return true;
}