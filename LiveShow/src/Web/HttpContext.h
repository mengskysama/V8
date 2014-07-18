// 请求的类型
#ifndef _HTTPCONTEXT_H_
#define _HTTPCONTEXT_H_

#include <string>
#include "webdefine.h"
#include "time.h"
#include "../CommonLibrary/src/HttpClient/include/httpmanager.h"

#define HTTP_SUCCESS	1
#define HTTP_FAILED	0
#define HTTP_EXISTS 2

enum RequestType
{
	/**
	*	接收到的数据完全是缓冲区，这些数据将一次性提交给Parser解析
	*/
	ONCE_BUFFER = 0,

	/**
	*	接受到的数据完全是缓冲区，这些数据不是一次性提交给parser，而是分好多次提交
	*/
	FRAG_BUFFER,

	/**
	*	接收到的数据是文件数据，这些数据在读取到的时候就会写入文件中
	*/
	ONCE_FILE,
};

#define		INVALID_RESP	0			//	无效的响应
#define		REPEATED_REQ	1			//	该请求处理失败，将重复进行处理
#define		SUCCESS_RESP	2			//	响应成功


// 每一次请求的上下文
typedef struct HTTPCONTEXT
{
	HTTPCONTEXT(string& url)
	{
		uOpPolicy			=	SUCCESS_RESP;			//	对该上下文处理的策略

		nEventValue			=	0;						//	IN 当前对应的事件值
		strURL				=	url;					//	IN 当前请求的URL
		eReqType			=	ONCE_BUFFER;			//	IN 请求的类型
		pArgument			=	NULL;					//	IN 请求所需要的Post参数
		nTimeout			=	0;						//	IN 超时
		wstrWritedFileName	=	L"";					//	IN 写入的文件的名称
		bResumable          = false;					//  IN 是否断点续传
		nResumableHead      =   TRUE;
		nResumableStartSize =   0;
		uParam0				=	0;						//	额外的参数
		uParam1				=	0;						//	额外的参数
		uParam2				=	0;						//	额外的参数

		/**
		*	返回内容
		*/
		nReqSeq				=	0;						//	OUT 当前请求的序列号
		nTaskId				=	0;						//	OUT 当前序列号对应的内部ID
		uAddTime			=	time(NULL);				//	OUT 加入队列的时间
		uEndTime			=	time(NULL);				//	OUT	处理结束时间
		nFailedTime			=	0;						//	OUT 请求失败的次数
		nSpeed				=	0;						//	OUT	传输速率，xxK/秒
		nRemainedTime		=	0;						//	OUT	估计的剩余时间

		uResult				=	HTTP_SUCCESS;				//	OUT 本次操作的结结果，HTTP_SUCCESS OR HTTP_FAILED	
		nErrorCode			=	0;						//	OUT 具体的错误码
		nHttpCode			=	200;					//	OUT HTTP响应码

		hFileHandle			=	NULL;					//	OUT 文件的句柄
		nContentLength		=	0;						//	OUT 整个报文的大小
		nFinishedLength		=	0;						//	OUT 已经读取的报文的长度
		strContentData		=	"";						//	OUT 本次读取的数据，对于FRAG_BUFFER而言，则是一次读取的数据，不是全部
		bAllowRepeated		=	TRUE;
		uLastModefied       =   0;
		
	}

	HTTPCONTEXT()
	{
		uOpPolicy			=	SUCCESS_RESP;			//	对该上下文处理的策略

		nEventValue			=	0;						//	IN 当前对应的事件值
		strURL				=	"";						//	IN 当前请求的URL
		eReqType			=	ONCE_BUFFER;			//	IN 请求的类型
		pArgument			=	NULL;					//	IN 请求所需要的Post参数
		nTimeout			=	30;						//	IN 超时
		wstrWritedFileName	=	L"";					//	IN 写入的文件的名称
		bResumable          = false;					//  IN 是否断点续传
		nResumableHead      =   TRUE;
		nResumableStartSize =   0;
		uParam0				=	0;						//	额外的参数
		uParam1				=	0;						//	额外的参数
		uParam2				=	0;						//	额外的参数

		/**
		*	返回内容
		*/
		nReqSeq				=	0;						//	OUT 当前请求的序列号
		nTaskId				=	0;						//	OUT 当前序列号对应的内部ID
		uAddTime			=	time(NULL);				//	OUT 加入队列的时间
		uEndTime			=	time(NULL);				//	OUT	处理结束时间
		nFailedTime			=	0;						//	OUT 请求失败的次数
		nSpeed				=	0;						//	OUT	传输速率，xxK/秒
		nRemainedTime		=	0;						//	OUT	估计的剩余时间

		uResult				=	HTTP_SUCCESS;				//	OUT 本次操作的结结果，HTTP_SUCCESS OR HTTP_FAILED	
		nErrorCode			=	0;						//	OUT 具体的错误码
		nHttpCode			=	200;					//	OUT HTTP响应码

		hFileHandle			=	NULL;					//	OUT 文件的句柄
		nContentLength		=	0;						//	OUT 整个报文的大小
		nFinishedLength		=	0;						//	OUT 已经读取的报文的长度
		strContentData		=	"";						//	OUT 本次读取的数据，对于FRAG_BUFFER而言，则是一次读取的数据，不是全部
		bAllowRepeated		=	TRUE;
		uLastModefied       =   0;
	}

	HTTPCONTEXT( HTTPCONTEXT* pContext)
	{
		uOpPolicy			=	pContext->uOpPolicy;			//	对该上下文处理的策略
		nEventValue			=	pContext->nEventValue;			//	IN 当前对应的事件值
		strURL				=	pContext->strURL;				//	IN 当前请求的URL
		eReqType			=	pContext->eReqType;				//	IN 请求的类型
		nTimeout			=	pContext->nTimeout;				//	IN 超时
		wstrWritedFileName	=	pContext->wstrWritedFileName;	//	IN 写入的文件的名称
		bResumable          =   pContext->bResumable;			//  IN 是否断点续传
		nResumableHead      =   TRUE;
		nResumableStartSize =   0;
		uParam0				=	pContext->uParam0;				//	额外的参数
		uParam1				=	pContext->uParam1;				//	额外的参数
		uParam2				=	pContext->uParam2;				//	额外的参数
		
		/**
		*	返回内容
		*/
		nReqSeq				=	pContext->nReqSeq;				//	OUT 当前请求的序列号
		nTaskId				=	pContext->nTaskId;				//	OUT 当前序列号对应的内部ID
		uAddTime			=	pContext->uAddTime;				//	OUT 加入队列的时间
		uEndTime			=	pContext->uEndTime;				//	OUT	处理结束时间
		nFailedTime			=	pContext->nFailedTime;			//	OUT 请求失败的次数
		nSpeed				=	pContext->nSpeed;				//	OUT	传输速率，xxK/秒
		nRemainedTime		=	pContext->nRemainedTime;		//	OUT	估计的剩余时间

		uResult				=	pContext->uResult;				//	OUT 本次操作的结结果，HTTP_SUCCESS OR HTTP_FAILED	
		nErrorCode			=	pContext->nErrorCode;			//	OUT 具体的错误码
		nHttpCode			=	pContext->nHttpCode;			//	OUT HTTP响应码

		hFileHandle			=	pContext->hFileHandle;			//	OUT 文件的句柄
		nContentLength		=	pContext->nContentLength;		//	OUT 整个报文的大小
		nFinishedLength		=	pContext->nFinishedLength;		//	OUT 已经读取的报文的长度
		strContentData		=	pContext->strContentData;		//	OUT 本次读取的数据，对于FRAG_BUFFER而言，则是一次读取的数据，不是全部
		if( pContext->pArgument == NULL)
			pArgument = NULL;
		else
			pArgument = new POST_PARAMS( *pContext->pArgument);

		bAllowRepeated		=	pContext->bAllowRepeated;
		uLastModefied       =   0;
		

	}

	~HTTPCONTEXT()
	{
	}

	/**
	*	请求数据内容
	*/
	// 本次请求操作的结果
	uint32			uOpPolicy;			//	对该上下文处理的策略

	EventValue		nEventValue;		//	IN 当前对应的事件值
	string			strURL;				//	IN 当前请求的URL
	RequestType		eReqType;			//	IN 请求的类型
	POST_PARAMS*	pArgument;			//	IN 请求所需要的Post参数
	UINT			nTimeout;			//	IN 超时
	wstring			wstrWritedFileName;	//	IN 写入的文件的名称
	BOOL			bAllowRepeated;		//	IN 是否允许请求相同的URL
	BOOL			bResumable ;		//  IN 是否断点续传
	BOOL            nResumableHead;     //  IN 是否为断点续传时http头信息  bResumable为true时有用
	off_t           nResumableStartSize;//  IN 断点续传时的开始下载位置  bResumable为true时有用
	int64			uParam0;			//	额外的参数
	int64			uParam1;			//	额外的参数
	int64			uParam2;			//	额外的参数
	

	/**
	*	返回内容
	*/
	uint32			nReqSeq;			//	OUT 当前请求的序列号
	uint32			nTaskId;			//	OUT 当前序列号对应的内部ID
	time_t			uAddTime;			//	OUT 加入队列的时间
	time_t			uEndTime;			//	OUT	处理结束时间
	uint8			nFailedTime;		//	OUT 请求失败的次数
	uint32			nSpeed;				//	OUT	传输速率，xxK/秒
	uint32			nRemainedTime;		//	OUT	估计的剩余时间

	uint8			uResult;			//	OUT 本次操作的结结果，HTTP_SUCCESS OR HTTP_FAILED	
	uint32			nErrorCode;			//	OUT 具体的错误码
	uint32			nHttpCode;			//	OUT HTTP响应码

	HANDLE			hFileHandle;		//	OUT 文件的句柄
	int32			nContentLength;		//	OUT 整个报文的大小, (注：断点续传时为整个服务器文件的大小)
	int32			nFinishedLength;	//	OUT 已经读取的报文的长度  (注：断点续传时为本地已下载文件的大小)
	string			strContentData;		//	OUT 本次读取的数据，对于FRAG_BUFFER而言，则是一次读取的数据，不是全部
	time_t			uLastModefied;      //  OUT 最后修改时间
	

} HTTPCONTEXT;

#endif