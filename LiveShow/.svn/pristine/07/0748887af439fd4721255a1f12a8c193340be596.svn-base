#include "HttpClient.h"
#include "shlwapi.h"
#include "../../../include/Log.h"
#include "utility/stringhelper.h"
#include "utility/FileHelper.h"
#include "WebModule.h"
#include "assert.h"
#include "../CommonLibrary/src/HttpClient/include/httpmanager.h"

#pragma comment(lib,"shlwapi.lib")

#define DOWNLOAD_COMPARE_ERROR -1
#define DOWNLOAD_COMPARE_EXISTS  -2
using namespace common::utility;


void SetFileModifyTime(HANDLE hFile,time_t modifyTime)
{
	FILETIME lastModifyTime;
	FILETIME CreationTime, LastAccessTime, LastWriteTime;	
	::GetFileTime(hFile,&CreationTime,&LastAccessTime,&LastWriteTime);
	common::utility::filehelper::UinxTimeToFileTime(modifyTime,&lastModifyTime);	
	::SetFileTime(hFile,&CreationTime,&LastAccessTime,&lastModifyTime);
}

//服务器文件与本地文件的比较,返回需要下载的位置
off_t FileCompare(HTTPCONTEXT* pContext)
{
	if(!pContext)
	{
		return DOWNLOAD_COMPARE_ERROR;
	}


	BOOL isLocalExist = common::utility::filehelper::IsFileExist(pContext->wstrWritedFileName.c_str());
	if(!isLocalExist)  //本地文件不存在
	{
		return 0; //从头开始下载
	}


	FILETIME CreationTime, LastAccessTime, lastModifyTime;
	time_t uLocalLastModifyTime = 0;
	common::utility::filehelper::GetFileTime(pContext->wstrWritedFileName,&CreationTime,&LastAccessTime,&lastModifyTime);
	common::utility::filehelper::FileTimeToUinxTime(&lastModifyTime,&uLocalLastModifyTime);
	if(pContext->uLastModefied != uLocalLastModifyTime) //服务器文件与本地文件的修改时间不一至
	{
		DeleteFile(pContext->wstrWritedFileName.c_str());
		return 0; //从头开始下载
	}

	DWORD localSize = 0;	
	localSize = common::utility::filehelper::GetFileSize(pContext->wstrWritedFileName.c_str());		

	//服务器文件与本地文件的修改时间一至
	if(localSize == pContext->nContentLength)  //本地文件大小与服务器文件大小一至，不需要下载
	{
		return DOWNLOAD_COMPARE_EXISTS;  //不需要下载
	}
	if(localSize < pContext->nContentLength)   //本地文件比服务器文件小，需从本地文件的最后继续下载
	{
		return localSize;
	}
	if(localSize > pContext->nContentLength)   //本地文件比服务器文件大，可能上次下载出错了，需从头下载
	{
		DeleteFile(pContext->wstrWritedFileName.c_str());
		return 0;  //从头开始下载
	}
}

HANDLE get_file_handle( HTTPCONTEXT* pContext)
{
	// 创建文件，并写入数据
	if(!pContext)
	{
		return NULL;
	}
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if(!pContext->bResumable)  //无断点续传
	{
		return ::CreateFile(pContext->wstrWritedFileName.c_str(), GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else //有断点续传
	{
		BOOL fileExist = common::utility::filehelper::IsFileExist(pContext->wstrWritedFileName.c_str());
		if(!fileExist) //未找到wstrWritedFileName文件
		{		
			HANDLE hfile = ::CreateFile(pContext->wstrWritedFileName.c_str(), GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);			
			SetFileModifyTime(hfile,pContext->uLastModefied);
			return hfile;

		}
		else //有找到wstrWritedFileName文件
		{
			HANDLE hfile = ::CreateFile(pContext->wstrWritedFileName.c_str(), GENERIC_WRITE, 0, &sa, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD dwPtr = SetFilePointer(hfile, 0, NULL, FILE_END);  //移动文件指针至文件尾
			return  hfile;
		}
	}	
}

INT32 handle_init(INT32 taskid, const TaskInfo* taskinfo)
{
	return 0;
}

INT32 handle_headcomplete(INT32 taskid, const TaskInfo* taskinfo)
{
	HttpClient* pHttpClient = HttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);

	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);

		// 不是边读边写类型
		if( !pContext || (pContext && pContext->eReqType == ONCE_BUFFER ))
			return -1;

		if(!pContext->bResumable)
		{
			pContext->nContentLength = (int32)taskinfo->contentLength;
		}
		
		return 0;
	}

	return -1;
}

INT32 handle_process(INT32 taskid, const TaskInfo* taskinfo)
{
	static int threadid = GetCurrentThreadId();
	if(threadid != GetCurrentThreadId())
	{
		assert(L"老兄，回调真的不是在同一个线程进行的哦!!!");
		LOG_ERROR(L"老兄，回调真的不是在同一个线程进行的哦!!!");
	}

	if( taskinfo == NULL )
	{
		LOG_ERROR(L"\n\n");
		LOG_ERROR(L"==========================FATAL ERROR BEGIN====================================");
		LOG_ERROR(L"In handle_finish, TaskInfo 或者 TaskInfo->respData为无效指针");
		LOG_ERROR(L"==========================FATAL ERROR END  ====================================\n\n");

		return -1;
	}

	HttpClient* pHttpClient = HttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);

	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);
		if( !pContext )
			return -1;

		pContext->uResult = HTTP_SUCCESS;
		pContext->nHttpCode = taskinfo->httpCode;
		pContext->nErrorCode = taskinfo->errcode;

		//  get方法的时候会自动把头部剔除 
		if( taskinfo->taskType == TASK_DOWNLOAD)
		{
			if(pContext->bResumable)  //断点续传时
			{
				pContext->nFinishedLength = pContext->nResumableStartSize + (int32)taskinfo->doneSize;
			}
			else
			{
				pContext->nFinishedLength = (int32)taskinfo->doneSize;
				pContext->nContentLength = (int32)taskinfo->contentLength;
				pContext->uLastModefied = taskinfo->lastModified;
			}
			
		}
		// 上载的时候需要剔除上载的头部
		else if( taskinfo->taskType == TASK_UPLOAD || taskinfo->taskType == TASK_BINARY)
		{
			pContext->nFinishedLength = (int32)(taskinfo->doneSize- taskinfo->htrequest->GetAllHdrLen());
			pContext->nContentLength = (int32)(taskinfo->contentLength - taskinfo->htrequest->GetAllHdrLen());
		}
		// 对于Query方法，它的DoneSize也是剔除了头之后的长度，其余的为下载，剔除下载头部
		else
		{
			pContext->nFinishedLength = (int32)taskinfo->doneSize;
			pContext->nContentLength = (int32)taskinfo->contentLength;
			pContext->uLastModefied = taskinfo->lastModified;
		}

		pContext->nSpeed	= (int32)taskinfo->transferSpeed;
		pContext->nRemainedTime	= (int32)taskinfo->remainTime;

		if( taskinfo->taskType == TASK_UPLOAD )
		{
			LOG_TRACE(L"\n\n");
			LOG_TRACE(L"===========UPLOAD PROCESSS RECEIVE DATA  %d ====================================", taskid);
			LOG_TRACE(L"SeqNo %d", nSeqNo);
			LOG_TRACE(L"DoneSize %d", (*taskinfo->respData).size());
			LOG_TRACE(L"FinishedLength %d", pContext->nFinishedLength);
			LOG_TRACE(L"==========================FATAL ERROR END  ====================================\n\n");
		}

		 // 将接收到的数据写入文件
		if( pContext->eReqType == ONCE_FILE)
		{
			
			if((pContext->bResumable && !pContext->nResumableHead) || !pContext->bResumable)
			{
				if( pContext->hFileHandle == 0)			
					pContext->hFileHandle = get_file_handle( pContext);

				string strCurrentBuffer;
				strCurrentBuffer.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());

				DWORD dwWrite = 0;				
				BOOL bRet = ::WriteFile( pContext->hFileHandle, strCurrentBuffer.c_str(), (DWORD)strCurrentBuffer.size(), &dwWrite, NULL);   //写入
				SetFileModifyTime(pContext->hFileHandle,pContext->uLastModefied);
				// 写入磁盘失败
				if( bRet == FALSE)
				{
					LOG_ERROR(L"\n\n");
					LOG_ERROR(L"==========================FATAL ERROR BEGIN====================================");
					LOG_ERROR(L"EEROR:	将文件写入磁盘失败，可能磁盘已满");
					LOG_ERROR(L"==========================FATAL ERROR END  ====================================\n\n");

					pContext->uResult = HTTP_FAILED;
					pContext->nErrorCode = ERR_TASK_WRITE_FILE_ERROR;

					pHttpClient->Cleanup( pContext);
				}
				
			}
			if(pContext->bResumable && pContext->nResumableHead)
			{
				off_t startPos = FileCompare(pContext); //服务器文件与本地文件的比较,返回需要下载的位置
				if(startPos >= 0) //需要下载
				{
					pContext->nResumableStartSize = startPos;  //断点续传时的开始下载位置
					pContext->nFinishedLength = startPos + (int32)taskinfo->doneSize;
					pContext->nResumableHead = FALSE;  //断点续传时获取数据
					pContext->uResult = HTTP_FAILED;
					pHttpClient->ReReqURL( pContext);  //重新发送以获取数据
					return 0;
				}	
				else if(startPos == DOWNLOAD_COMPARE_EXISTS)  //要下载的文件已经在存且与远程文件一至，不用下载
				{
					pContext->nFinishedLength = pContext->nContentLength;
					pContext->uResult = HTTP_EXISTS; //要下载的文件已经在存且与远程文件一至，不用下载
				}
			
			}

			

		}

		// 对于读取到的数据立即返回给上层应用
		else if( pContext->eReqType == FRAG_BUFFER)
		{
			pContext->strContentData.clear();
			pContext->strContentData.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());
            
            // param0用以表示是否是中间状态回复，而不是最后的回复
            pContext->uParam0 = 0;

 			pHttpClient->Cleanup( pContext);
		}                        

		return 0;
	}

	return -1;
}

INT32 handle_finish(INT32 taskid, const TaskInfo* taskinfo)
{
	static int threadid = GetCurrentThreadId();
	if(threadid != GetCurrentThreadId())
	{
		assert(L"老兄，回调真的不是在同一个线程进行的哦!!!");
		LOG_ERROR(L"老兄，回调真的不是在同一个线程进行的哦!!!");
	}

	if( taskinfo == NULL || taskinfo->respData == NULL)
	{
		LOG_ERROR(L"\n\n");
		LOG_ERROR(L"==========================FATAL ERROR BEGIN====================================");
		LOG_ERROR(L"In handle_finish, TaskInfo 或者 TaskInfo->respData为无效指针");
		LOG_ERROR(L"==========================FATAL ERROR END  ====================================\n\n");

		return -1;
	}

	HttpClient* pHttpClient = HttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);

	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);
		if( pContext)
		{
			pContext->uResult = HTTP_SUCCESS;
			pContext->nHttpCode = taskinfo->httpCode;
			pContext->nErrorCode = taskinfo->errcode;

			//  get方法的时候会自动把头部剔除 
			if( taskinfo->taskType == TASK_DOWNLOAD)
			{
				if(pContext->bResumable)  //断点续传时
				{
					pContext->nFinishedLength = pContext->nResumableStartSize + (int32)taskinfo->doneSize;
				}
				else
				{
					pContext->nFinishedLength = (int32)taskinfo->doneSize;
					pContext->nContentLength = (int32)taskinfo->contentLength;
					pContext->uLastModefied = taskinfo->lastModified;
				}
				

			}
			// 上载的时候需要剔除上载的头部
			else if( taskinfo->taskType == TASK_UPLOAD || taskinfo->taskType == TASK_BINARY)
			{
				pContext->nFinishedLength = (int32)(taskinfo->doneSize- taskinfo->htrequest->GetAllHdrLen());
				pContext->nContentLength = (int32)(taskinfo->contentLength - taskinfo->htrequest->GetAllHdrLen());
			}
			// 对于Query方法，它的DoneSize也是剔除了头之后的长度，其余的为下载，剔除下载头部
			else
			{
				pContext->nFinishedLength = (int32)taskinfo->doneSize;
				if( pContext->nFinishedLength < 0)
					pContext->nFinishedLength = 0;

				pContext->nContentLength = (int32)taskinfo->contentLength;
				if( pContext->nContentLength < 0)
					pContext->nContentLength = 0;

				pContext->uLastModefied = taskinfo->lastModified;
			}

			pContext->nSpeed	= (int32)taskinfo->transferSpeed;
			pContext->nRemainedTime	= (int32)taskinfo->remainTime;

			if( taskinfo->taskType == TASK_UPLOAD && taskinfo->respData->size())
			{
				LOG_TRACE(L"\n\n");
				LOG_TRACE(L"===========UPLOAD FINISH RECEIVE NULL DATA  %d ====================================", taskid);
				LOG_TRACE(L"SeqNo %d", nSeqNo);
				LOG_TRACE(L"DoneSize %d", (*taskinfo->respData).size());
				LOG_TRACE(L"FinishedLength %d", pContext->nFinishedLength);
				LOG_TRACE(L"==========================FATAL ERROR END  ====================================\n\n");
			}

			// 需要立即写入到文件中
			if( pContext->eReqType == ONCE_FILE)
			{		

				if((pContext->bResumable && !pContext->nResumableHead) || !pContext->bResumable)
				{
					if( pContext->hFileHandle == 0)
						pContext->hFileHandle = get_file_handle(pContext);

					string strCurrentBuffer;
					strCurrentBuffer.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());

					DWORD dwWrite = 0 ;
					BOOL bRet = ::WriteFile( pContext->hFileHandle, strCurrentBuffer.c_str(), (DWORD)strCurrentBuffer.size(), &dwWrite, NULL);   //写入
					SetFileModifyTime(pContext->hFileHandle,pContext->uLastModefied);
					CloseHandle( pContext->hFileHandle);
					pContext->hFileHandle = 0;

					// 写入磁盘失败
					if( bRet == FALSE)
					{
						LOG_ERROR(L"\n\n");
						LOG_ERROR(L"==========================FATAL ERROR BEGIN====================================");
						LOG_ERROR(L"EEROR:	将文件写入磁盘失败，可能磁盘已满");
						LOG_ERROR(L"==========================FATAL ERROR END  ====================================\n\n");

						pContext->uResult = HTTP_FAILED;
						pContext->nErrorCode = ERR_TASK_WRITE_FILE_ERROR;

						pHttpClient->Cleanup( pContext);
					}
					
				}
				if(pContext->bResumable && pContext->nResumableHead)
				{
					off_t startPos = FileCompare(pContext); //服务器文件与本地文件的比较,返回需要下载的位置
					if(startPos >= 0) //需要下载
					{
						pContext->nResumableStartSize = startPos;  //断点续传时的开始下载位置
						pContext->nFinishedLength = startPos + (int32)taskinfo->doneSize;
						pContext->nResumableHead = FALSE;  //断点续传时获取数据
						pContext->uResult = HTTP_FAILED;
						pHttpClient->ReReqURL( pContext);  //重新发送以获取数据
						return 0;
					}
					else if(startPos == DOWNLOAD_COMPARE_EXISTS)  //要下载的文件已经在存且与远程文件一至，不用下载
					{						
						pContext->nFinishedLength = pContext->nContentLength;						
						pContext->uResult = HTTP_EXISTS; //要下载的文件已经在存且与远程文件一至，不用下载
					}

				}

				
               
			}

			// 全部读取，最后一次性处理
			else
			{
                pContext->strContentData.clear();
				pContext->strContentData.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());
                pContext->uParam2 = 1;
			}
			
			pHttpClient->Cleanup( pContext);
		}

		return 0;
	}

	return -1;
}

INT32 handle_error(INT32 taskid, const TaskInfo* taskinfo)
{
	static int threadid = GetCurrentThreadId();
	if(threadid != GetCurrentThreadId())
	{
		assert(L"老兄，回调真的不是在同一个线程进行的哦!!!");
		LOG_ERROR(L"老兄，回调真的不是在同一个线程进行的哦!!!");
	}

	HttpClient* pHttpClient = HttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);
	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);
		if( pContext)
		{
			LOG_ERROR(L"\n\n");
			LOG_ERROR(L"=====================%s HTTP处理失败====================================", __FUNCTIONW__);
			LOG_ERROR(L"URL：%s", stringhelper::AsciiToUnicode(pContext->strURL).c_str());
			LOG_ERROR(L"序列号为：%d", nSeqNo);
			LOG_ERROR(L"内部错误码为：%d", taskinfo->errcode);
			LOG_ERROR(L"HTTP错误码为：%d", taskinfo->httpCode);
			LOG_ERROR(L"===========================================================================\n\n");

			pContext->uResult = HTTP_FAILED;
			pContext->nHttpCode = (int32)taskinfo->httpCode;
			pContext->nErrorCode = (int32)taskinfo->errcode;
			pContext->nContentLength = (int32)taskinfo->contentLength;
			if( taskinfo->respData != NULL && (*(taskinfo->respData)).size() > 0)
			{
				pContext->strContentData.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());
			}
			else
			{
				LOG_ERROR(L"\n\n");
				LOG_ERROR(L"===================%s FATAL ERROR BEGIN==================", __FUNCTIONW__ );
				LOG_ERROR(L"TaskInfo->respData为无效指针");
				LOG_ERROR(L"==========================FATAL ERROR END  ====================================\n\n");
			}

			pHttpClient->Cleanup( pContext);
		}

		return 0;
	}

	return -1;
}
