#pragma once

#include <string>
#include "webdefine.h"
#include "ResponseParser.h"
#include "HttpContext.h"
#include "../CommonLibrary/src/HttpClient/include/httpmanager.h"

using namespace std;

class HttpClient;
/**************************************************************
* ClassName:   CRequestManager
* Author:	   Zhang Z.Q
* Using:	   该类负责处理HTTP请求，包括下面的功能
*				1) 接口上层传递的URL，然后将其发送给Web服务器
*				2) 接受Web服务器的响应，并将其响应内容保存到指定的响应队列中
* Verion:
*		Created			2007,12,12
**************************************************************/

class CWebData;

class CRequestManager
{
public:

	CRequestManager();
	~CRequestManager();

	void		Init();
	void		Clear();
	static int	GetRetCodeByStatus( int nStatusCode);

	/**
	 * 将给定的URL放入到延迟发送请求队列中
	 * @param		nEventValue 该请求对应的EventValue
	 * @param		nUin		请求所对应的nUin
	 * @param		strUrl		请求对应的URL
	 * @param		pArgument	如果请求是post请求，则为post请求参数
	 *							对于get请求而言,该参数必须被设置为NULL
	 * @param		eReqType	请求类型
	 * @param		bAllowRepeated	该请求的URL是否允许重复
	 * @param		bResumable   是否需要断点续传， 下载文件时使用

	 * @return		返回当前的任务序列号
	 **/
	uint32		PushUrl(EventValue nEventValue, int64 nParam0, wstring strUrl, 
						POST_PARAMS* pArgument, RequestType eReqType = ONCE_BUFFER, 
						wstring wstrWritedFileName = L"", UINT nTimeout = 30, BOOL bAllowRepeated= TRUE,BOOL bResumable = FALSE);  
	uint32		PushUrl( HTTPCONTEXT* pContext);

	/**
	* 从队列中获取一个新的请求
	* @param		nEventValue 该请求对应的EventValue
	* @return		返回当前的任务序列号
	**/
	uint8		PopUrl( HTTPCONTEXT*& pContext);

	/**
	 * Function		请求给定的URL
	 * @param		pContext	请求上下文
	 * @param		nTimeout	超时时间，默认是10
	 * @return		
	 **/
	void		ReqUrl( HTTPCONTEXT* pContext);
	void		ReReqUrl( string strUrl,HTTPCONTEXT* pContext, POST_PARAMS* pArgument);

	/**
	 * Function		取消给定的Task
	 * @param		nSeqNo		需要取消的序列号
	 */
	void		CancelTask( uint32 nSeqNo);

	HTTPCONTEXT		ProcessResp( );

	/**
	 * Function		查询给定下载任务的状态
	 * @param		nSeqNo	任务的序列号，通过CallService返回
	 * @param		uPercent		下载的任务比例
	 * @param		uTotalLength	需要下载的总长度
	 * @param		uFinishedLength	已经完成的长度
	 * @param		如果查找到该seqno，返回true，否则返回false
					uPercent、uTotalLength以及uFinishedLength只有返回true的时候才有效
	 */
	bool		QueryDownloadProcess( uint32 nSeqNo, uint32* uPercent, 
		uint32* uTotalLength, uint32* uFinishedLength,uint32* uSpeed, int32* uRemainedTime);


	/**
	* Function		检查给定的URL是否已经在请求队列中
	*/	
	BOOL		IsExistUrl( string strUrl);

public:

	HttpClient*		m_pHttpClient;
	CResponseParser	m_ResponseParser;				// 对响应报文进行解析
	unsigned int	m_nReqSeq;
	std::vector< HTTPCONTEXT*>	m_vRequestQueue;	//	保存所有的请求
	CWebData*		m_pWebData;
};
