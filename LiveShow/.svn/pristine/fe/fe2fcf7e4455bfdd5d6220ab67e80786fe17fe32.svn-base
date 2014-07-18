// WebService.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "WebModule.h"
#include "process.h"
#include "51ktv.h"
#include "shlwapi.h"
#include "PostEncode.h"
#include "log.h"
#include "ximage.h"
#include "ResponseParser.h"
#include "webdefine.h"
#include "../../CommonLibrary/include/utility/Md5.h"
#include "../../CommonLibrary/include/utility/Base64Coding.h"
#include "../../CommonLibrary/include/utility/URLEncode.h"
#include "../../CommonLibrary/include/utility/StringHelper.h"
#include "../../CommonLibrary/include/utility/SystemHelper.h"
#include "../../CommonLibrary/include/utility/FileHelper.h"
#include "../../CommonLibrary/include/utility/URLEncode.h"

#pragma comment(lib,"shlwapi.lib")
#pragma warning (disable:4996)

using namespace webinterface;
using namespace common::utility;

#define CHECK_EVENT( pEvent, event_value) \
	if( pEvent == NULL || pEvent->eventValue != event_value) \
		return INVALID_SEQNO;	\
	if( pEvent->srcMId == 0)   \
	{		\
		assert(0);		\
		return INVALID_SEQNO;	\
	}

namespace webinterface
{
	WebModule*	g_WebModule = NULL;
	INSTANCE_LOG(L"WebModule");
}

uint32 CALLSERVICE(ModuleId moduleId, param wparam, param lparam)
{
	IModuleManager* pModuleManager = webinterface::g_WebModule->m_pModuleManager;
	if (pModuleManager) 
	{ 
		return pModuleManager->CallService(wparam, lparam); 
	} 
	else 
	{ 
		//ASSERT(0); 
	}

	return -1;
}

IModule*  GetModule()
{
	if(g_WebModule == NULL)
	{
		g_WebModule = new WebModule();
	}

	return g_WebModule;
}

void ReleaseModule()
{
	delete g_WebModule ;
	g_WebModule = NULL;
}

void PUSH_MESSAGE( EventValue event_value, param param0, param param1,param param2)
{
	Message msg;	
	msg.messageValue = event_value;	
	msg.param0 = param0;	
	msg.param1 = param1;	
	msg.param2 = param2;	
	webinterface::g_WebModule->m_pModuleManager->PushMessage(msg);
}

void PUSH_EVENT( Event& event_)
{
	LOG_TRACE(L"PUSH_EVENT, 指针为 ：%d",  &event_);
	event_.srcMId = MODULE_ID_WEBSERVICE;
	webinterface::g_WebModule->PushEvent_( &event_);
}

core::CurInfo GetCurInfo()
{
	core::CurInfo user_info;
	CALLSERVICE(MODULE_ID_CORE,  core::SERVICE_VALUE_CORE_GET_CUR_INFO, (param)&user_info);

	return user_info;
}

static string GetTokenStr()
{
	return stringhelper::UnicodeToAscii(GetCurInfo().strToken);
}

static string GetUinStr()
{
	return stringhelper::IntToStringA( GetCurInfo().unUIN);
}

WebModule::HandlerTableEntry WebModule::m_tableDriven[] =
{
	{ event_value_web_get_update_config_xml_req,				WebModule::ProcessReqGetUpdateConfigXml },
	{ event_value_web_get_filter_words_req,						WebModule::ProcessGetFilterWords},
	{ service_value_web_upload_head_image_req,				WebModule::ProcessReqUploadHeadImage },
	{ service_value_web_download_head_image_req,				WebModule::ProcessReqDownloadHeadImage },		
	{ service_value_web_download_file_req,						WebModule::ProcessReqGetFileReq },
	{ service_value_web_query_download_process_req,				WebModule::ProcessReqQueryDownloadProcess },
	{ service_value_web_get_http_req,							WebModule::ProcessReqGetHttp },

	{ service_value_web_get_login_identify_code_req,			WebModule::ProcessGetLoginIdentifyCode },
	{ 0, NULL}
};

WebModule::WebModule(void) : m_pModuleManager( NULL )
{
	m_pRequestManager	=  NULL;	
	m_bLogout = true;
}

WebModule::~WebModule(void)
{
}

void 
WebModule::Load( IModuleManager * pModuleManager)
{
	m_pModuleManager = pModuleManager;
	m_pRequestManager	= new	CRequestManager();
	m_pRequestManager->m_pHttpClient = HttpClient::Instance();
}

void 
WebModule::Unload()
{
	// 此处需要修改
	if( m_pRequestManager )
	{
		delete m_pRequestManager;
		m_pRequestManager = NULL;
	}
}

wstring	
WebModule::GetLogPath( uin nUin)
{
#ifdef _MODULE_LOG_
	wstring	strLogPath;

	strLogPath += systemhelper::Get179AppPath();
	strLogPath += L"log/web/";
	filehelper::CreateAllDirectory( strLogPath);

	strLogPath += L"web_";
	strLogPath += stringhelper::IntToString(nUin);
	strLogPath += wstring(L"_") + stringhelper::IntToString((long)time(0));
	strLogPath += wstring(L".log");

	return strLogPath;
#else
	return L"";
#endif
}

void 
WebModule::ProcessEvent( Event const & event_)
{
	EventValue event_value = event_.eventValue;
	switch( event_value)
	{
		/*case core::EVENT_VALUE_CORE_BEGIN_DOWNLOAD_FILTERWORDS:
			{
				get_filter_words_req e;
				e.srcMId = MODULE_ID_CORE;
				e.desMId = MODULE_ID_WEBSERVICE;
				CALLSERVICE( MODULE_ID_WEBSERVICE, e.eventValue, (param)&e);

				break;
			}*/

		default:
			{
				break;
			}
	}


	Web_Req_Event* pEvent = (Web_Req_Event*)event_.m_pstExtraInfo;
	if( pEvent == NULL )
		return ;

	if( pEvent->bAllowOffline == false && !GetCurInfo().bLogined)
	{
		LOG_TRACE(L"Not Allow Offline, Event Value  %d", pEvent->eventValue);
		return ;
	}

	HandlerTableEntry* pEntry = &WebModule::m_tableDriven[0];
	while( pEntry)
	{
		if( pEntry->m_nEventValue == 0)
			break;

		if( pEntry->m_nEventValue == event_value
			&& pEntry->m_hHandler != NULL)
		{
			(*pEntry->m_hHandler)(this, pEvent);
			break;
		}

		++pEntry;
	}

}

void 
WebModule::ProcessMessage(Message const & _message)
{
	MessageValue msg_value = _message.messageValue;
	switch( msg_value)
	{
		case MESSAGE_VALUE_SYS_CYCLE_TRIGGER:
			{
				ProcessGetResponse();
				ProcessSendUrl();
				break;
			}

		case core::MESSAGE_VALUE_CORE_LOGIN_COMPLETE:
			{
				OnMessagePreLoginResp( _message);
				break;
			}

		case MESSAGE_VALUE_SYS_PRELOGOUT:
		case MESSAGE_VALUE_SYS_CHANGE_USER:
			{
				OnMessageOffline();
				break;
			}

		default:
			break;
	}
}

void 
WebModule::PushEvent_( Event* event_)
{
	m_pModuleManager->PushEvent(*event_);
}

bool 
WebModule::ProcessImage( wstring inImagePath, wstring& outImagePath)
{
	// 检查文件是否存在
	if( PathFileExistsW( inImagePath.c_str()) != TRUE)
	{
		assert(0);
		return false;
	}

	outImagePath = inImagePath;
	return true;
}


uint32 
WebModule::ProcessReqGetUpdateConfigXml( WebModule* pModule, Event* pEvent)
{
	CHECK_EVENT( pEvent, event_value_web_get_update_config_xml_req);

	wstring wstrUrl = L"http://imupdate.caihong.com/caihong_update.xml";
	uint32 nSeqNo = pModule->m_pRequestManager->PushUrl(	pEvent->eventValue,GetCurInfo().unUIN,wstrUrl,NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	return INVALID_SEQNO;
}

uint32 
WebModule::ProcessReqUploadHeadImage(WebModule* pModule,  Event* pEvent)
{
	CHECK_EVENT( pEvent, service_value_web_upload_head_image_req);

	upload_head_image_event_req*	pE = (upload_head_image_event_req*)pEvent;

	POST_PARAMS httpArgument;;
	httpArgument.add_param("uin",GetUinStr().c_str(), (UINT32)GetUinStr().size() + 1, PARAM_TEXT);	

	wstring wstrOutputImagePath;
	bool bRet = pModule->ProcessHeadImage( pE->wstrImagePath, wstrOutputImagePath);
	if( bRet == false)
	{
		assert(L"上传的用户头像图片有误，无法上传");
		return INVALID_SEQNO;
	}
	
	string strImgePath = stringhelper::UnicodeToAscii(wstrOutputImagePath);
	httpArgument.add_param("filename", strImgePath.c_str(), 0, PARAM_FILENAME);

	uint32 nSeqNo =  pModule->m_pRequestManager->PushUrl( pEvent->eventValue, 0,pE->wstrUrl,&httpArgument);

	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	upload_head_image_event_req* pReq = new upload_head_image_event_req;
	pReq->wstrImagePath	=	pE->wstrImagePath;
	pModule->m_mapRequestEvent[nSeqNo] = pReq;

	return nSeqNo;
}

uint32 
WebModule::ProcessReqDownloadHeadImage(WebModule* pModule,  Event*	pEvent)
{
	CHECK_EVENT( pEvent, service_value_web_download_head_image_req);

	download_head_image_event_req*	pE = (download_head_image_event_req*)pEvent;

	wstring wstrPath = pE->wstrImagePath;
	stringhelper::Replace( wstrPath, L"/", L"\\");

	size_t nPos = wstrPath.find_last_of('\\');
	wstrPath = wstrPath.substr(0, nPos);

	if( !PathFileExistsW( wstrPath.c_str()))
	{
		filehelper::CreateAllDirectory(wstrPath.c_str());
	}

	uint32 nSeqNo =  pModule->m_pRequestManager->PushUrl( pE->eventValue, pE->unUIN, 
		pE->wstrUrl, NULL, ONCE_BUFFER, pE->wstrImagePath);

	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	LOG_TRACE(L"Push Url -- nSeqNo ：%d",  nSeqNo);

	return nSeqNo;
}

uint32 
WebModule::ProcessReqGetFileReq( WebModule* pModule, Event*	pEvent)
{
	CHECK_EVENT( pEvent, service_value_web_download_file_req);

	get_download_file_event_req* pE = (get_download_file_event_req*)pEvent;
	
	wstring wstrPath = pE->wstrFileName;
	stringhelper::Replace( wstrPath, L"/", L"\\");

	size_t nPos = wstrPath.find_last_of('\\');
	wstrPath = wstrPath.substr(0, nPos);

	if( !PathFileExistsW( wstrPath.c_str()))
	{
		filehelper::CreateAllDirectory(wstrPath.c_str());
	}

	uint32 nSeqNo =  pModule->m_pRequestManager->PushUrl( pE->eventValue, GetCurInfo().unUIN, 
		pE->wstrUrl, NULL, ONCE_FILE, pE->wstrFileName,pE->nTimeout,pE->bAllowRepeated,pE->bResumable);

	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	LOG_TRACE(L"Push Url -- nSeqNo ：%d",  nSeqNo);

	return nSeqNo;

}

uint32 
WebModule::ProcessReqQueryDownloadProcess( WebModule* pModule, Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != service_value_web_query_download_process_req)
		return INVALID_SEQNO;

	query_download_file_process_req* pE = (query_download_file_process_req*)pEvent;

	uint32 ret = pModule->m_pRequestManager->QueryDownloadProcess( pE->nSeqNo, &pE->uPercent, 
		&pE->uTotalLength, &pE->uFinishedLength, &pE->uSpeed, &pE->uRemainedTime);

	if( ret == false)
	{
		LOG_ERROR(L"ProcessReqQueryDownloadProcess: Failed");
		return INVALID_SEQNO;
	}
	else
	{
		LOG_TRACE(L"ProcessReqQueryDownloadProcess: uTotalLength: %d FinishedLength: %d",
			pE->uTotalLength, pE->uFinishedLength);

		return READY_SEQNO;
	}
}

uint32 
WebModule::ProcessReqGetHttp( WebModule* pModule, Event* pEvent)
{
	CHECK_EVENT( pEvent, service_value_web_get_http_req);

	get_http_url_event_req* pE = ( get_http_url_event_req*)pEvent;

	uint32 nSeqNo = pModule->m_pRequestManager->PushUrl( pEvent->eventValue,  GetCurInfo().unUIN, pE->wstrUrl, NULL,
		ONCE_BUFFER, L"", pE->nTimeout, 1);

	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	return nSeqNo;
}

uint32 WebModule::ProcessGetFilterWords( WebModule* pModule, Event* pEvent)
{
	CHECK_EVENT( pEvent, event_value_web_get_filter_words_req);

	POST_PARAMS httpArgument;
	httpArgument.add_param("uin",GetUinStr().c_str(), (UINT32)GetUinStr().size() + 1, PARAM_TEXT);
	httpArgument.add_param("token", GetTokenStr().c_str(), (UINT32)GetTokenStr().size() + 1, PARAM_TEXT);

	wstring wstrUrl = L"http://pic.51.com/ck/im/check_keyword.xml";
	uint32 nSeqNo =  pModule->m_pRequestManager->PushUrl( pEvent->eventValue,GetCurInfo().unUIN,wstrUrl,NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	return nSeqNo;
}

uint32 WebModule::ProcessCancelHttpTask( WebModule* pModule, Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != event_value_web_cancel_http_task)
		return INVALID_SEQNO;

	cancel_http_task_req* pE = (cancel_http_task_req*)pEvent;
	uint32 nSeqNo = pE->nSeqNo;

	pModule->m_pRequestManager->CancelTask( nSeqNo);

	return READY_SEQNO;
}

bool 
WebModule::ProcessHeadImage( wstring wstrImagePath, wstring& wstrOutputPath)
{
	wstring wstrOutImagePath = systemhelper::Get179AppPath();
	if( !PathFileExistsW(wstrOutImagePath.c_str()))
		filehelper::CreateAllDirectory( wstrOutImagePath.c_str());

	wstrOutImagePath += wstring(L"head\\");

	bool bRet = ProcessImage( wstrImagePath, wstrOutImagePath);

	wstrOutputPath = wstrOutImagePath;

	return bRet;
}
void WebModule::ProcessGetResponse()
{
	// 处理WinHTTP响应报文
	uint8 uNum = 0;
	while( 1)
	{
		HTTPCONTEXT httpContext  = m_pRequestManager->ProcessResp();
		if( uNum++ > 10)
			break;

		// 没有获取到有效的响应数据
		if( httpContext.uOpPolicy == INVALID_SEQNO  )
		{
			break;
		}

		// 重新请求
		if( httpContext.uOpPolicy == REPEATED_REQ)
		{
			LOG_WARN(L"\n\n\n");
			LOG_WARN(L"==================== 发现因失败进行重新请求 ==================");
			LOG_WARN(L"FailTime: %d", httpContext.nFailedTime);
			LOG_WARN(L"SeqNo: %d", httpContext.nReqSeq);
			LOG_WARN(L"URL: %s", stringhelper::AsciiToUnicode(httpContext.strURL).c_str());
			LOG_WARN(L"=========================================================\n\n");

			httpContext.strContentData.clear();

			continue;
		}

		// 读取数据成功
		if( httpContext.uOpPolicy == SUCCESS_RESP)
		{
			// 如果是收到的buffer立即交给上层，则上下文不删除，继续利用
			if( httpContext.eReqType == FRAG_BUFFER )
			{
				LOG_TRACE(L"\n\n\n");
				LOG_TRACE(L"==================== FRAG_BUFFER  响应处理成功，响应立即提交上层处理，请求上下文重复使用 ==================");
				LOG_TRACE(L" SeqNo: %d", httpContext.nReqSeq);
				LOG_TRACE(L" URL: %s", stringhelper::AsciiToUnicode(httpContext.strURL).c_str());
				LOG_TRACE(L"=========================================================\n\n");

				assert( m_pRequestManager && m_pRequestManager->m_pHttpClient );

                // 如果是最后一次
                if( httpContext.uParam2 == 1)
                {
                    m_pRequestManager->m_pHttpClient->DeleteSeqNo(httpContext.nReqSeq);
                }
			}
			else
			{
				LOG_TRACE(L"\n\n\n");
				LOG_TRACE(L"=================== 响应处理成功，请求上下文将被删除 ==================");
				LOG_TRACE(L"SeqNo: %d", httpContext.nReqSeq);
				LOG_TRACE(L"URL: %s", stringhelper::AsciiToUnicode(httpContext.strURL).c_str());

                m_pRequestManager->m_pHttpClient->DeleteSeqNo( httpContext.nReqSeq);
			}
		}
		else
		{
			LOG_ERROR(L"\n\n\n");
			LOG_ERROR(L"====================  响应失败  ==================");
			LOG_ERROR(L"SeqNo: %d", httpContext.nReqSeq);
			LOG_ERROR(L"URL: %s", stringhelper::AsciiToUnicode(httpContext.strURL).c_str());

			if( httpContext.nErrorCode == ERR_TASK_WRITE_FILE_ERROR)
			{
				LOG_ERROR(L"磁盘写入错误");
			}
			else
			{
				LOG_ERROR(L"其余超时错误");
			}
			LOG_ERROR(L"=========================================================\n\n");

			// 发生了超时等错误或者写入磁盘错误
			Event e;
			e.param0 = httpContext.uOpPolicy;
			e.param2 = httpContext.nReqSeq;
		//	e.eventValue = httpContext.nEventValue + 1;    //注：因发送web任务时是有些是使用CallService中的webinterface_service_value事件，
			                                               //    而接收下载结束返回时是使用PushEvent事件中的webinterface_event_value事件，
			                                               //    若在原web事件中直接加1，可能会使返回事件值错误，以至程序蹦掉
			if(IsServiceValue(httpContext.nEventValue))
			{
				serviceValueResp ServerEvent;
				e.eventValue = ServerEvent.GetServiceValueResp(httpContext.nEventValue);//获取某service_value事件值的对应返回event_value事件值
			}
			else
			{
				e.eventValue = httpContext.nEventValue + 1; 
			}		
			
			e.srcMId = MODULE_ID_WEBSERVICE;			
 
			std::map<uint32, uint32>::iterator itr = g_WebModule->m_mapSeqNo2ModuleId.find( httpContext.nReqSeq);
			if( itr != g_WebModule->m_mapSeqNo2ModuleId.end())
			{
				e.desMId = itr->second;
			}

			g_WebModule->m_pModuleManager->PushEvent(e);

            m_pRequestManager->m_pHttpClient->DeleteSeqNo( httpContext.nReqSeq);
		}
	}
}

void WebModule::ProcessSendUrl( )
{
	UINT nNum = 0;
	while( nNum < 3)
	{
		HTTPCONTEXT* pContext = NULL ;
		if( m_pRequestManager->PopUrl( pContext) )
		{
			LOG_TRACE(L"POP URL   :%s", stringhelper::AsciiToUnicode(pContext->strURL).c_str());
			m_pRequestManager->ReqUrl( pContext);
		}

		nNum++;
	}
}

void	
WebModule::OnMessagePreLoginResp( Message const& _message)
{
	if( _message.param0 == core::LOGIN_ERROR_SUCCESS)
	{
		LOG_TRACE(L"内部Uin :%d", GetCurInfo().unUIN);
		LOG_TRACE(L"外部Uin :%d", GetCurInfo().un179id);

		m_bLogout = false;
	}
}

void	
WebModule::OnMessageOffline( )
{
	if( m_pRequestManager)
	{
		m_pRequestManager->Clear();
	}
}

//外部模块调用此方法，获取一个消息对应的具体内容
//virtual void FetchMessage(Message *const )=0;

//CallService替代FetchMessage，不仅用于取数据，同时可用于执行操作。比如本地存储。
//wparam对应一个EventValue或者MessageValue，lapram对应具体数据结构。
//宏"IsEventValue"和"IsMessageValue"用于判断是一个EventValue还是一个MessageValue

int32
WebModule::Invoke(param const wparam,param lparam)
{
	ServiceValue event_value = (ServiceValue)wparam;
	Web_Req_Event* pEvent = (Web_Req_Event*)lparam;
	if( pEvent == NULL )
		return INVALID_SEQNO;

	if( pEvent->bAllowOffline == false && !GetCurInfo().bLogined)
	{
		LOG_TRACE(L"Not Allow Offline, Event Value  %d", pEvent->eventValue);
		return INVALID_SEQNO;
	}

	HandlerTableEntry* pEntry = &WebModule::m_tableDriven[0];
	while( pEntry)
	{
		if( pEntry->m_nEventValue == 0)
			break;

		if( pEntry->m_nEventValue == event_value
			&& pEntry->m_hHandler != NULL)
		{
			return (*pEntry->m_hHandler)(this, pEvent);
		}

		++pEntry;
	}

	return INVALID_SEQNO;
}

void	
WebModule::PaybackExtraInfo(uint32  eventValue ,void *  p)
{
	if(IsEventValue(eventValue))
	{
		Event * pstEvent=static_cast<Event*>(p);
		if(pstEvent->m_pstExtraInfo)
		{
			delete pstEvent->m_pstExtraInfo;
			pstEvent->m_pstExtraInfo=NULL;
		}
	}
	else if(IsMessageValue(eventValue))
	{
		Message * pstMessage=static_cast<Message*>(p);
		if(pstMessage->m_pstExtraInfo)
		{
			delete pstMessage->m_pstExtraInfo;
			pstMessage->m_pstExtraInfo=NULL;
		}
	}
}

uint32 
WebModule::ProcessGetLoginIdentifyCode( WebModule* pModule, Event* pEvent)
{
	CHECK_EVENT( pEvent, service_value_web_get_login_identify_code_req);

	get_login_identify_code_service_req* pE = (get_login_identify_code_service_req*)pEvent;

	POST_PARAMS httpArgument;

	httpArgument.add_param("f", string("1").c_str(), (UINT32)string("1").size() + 1, PARAM_TEXT);

	httpArgument.add_param("width",stringhelper::IntToStringA(pE->nWidth).c_str(), 
		(UINT32)stringhelper::IntToStringA(pE->nWidth).size() + 1, PARAM_TEXT);
	httpArgument.add_param("height",stringhelper::IntToStringA(pE->nHeight).c_str(), 
		(UINT32)stringhelper::IntToStringA(pE->nHeight).size() + 1, PARAM_TEXT);
	httpArgument.add_param("imgtype", string("jpeg").c_str(), (UINT32)string("jpeg").size() + 1, PARAM_TEXT);
	httpArgument.add_param("length",stringhelper::IntToStringA(pE->nCharNum).c_str(), 
		(UINT32)stringhelper::IntToStringA(pE->nCharNum).size() + 1, PARAM_TEXT);

	wstring wstrUrl = L"http://passport.51.com/securecode/securecode.php";
	uint32 nSeqNo =  pModule->m_pRequestManager->PushUrl( pEvent->eventValue,GetCurInfo().unUIN,wstrUrl,&httpArgument);

	if( nSeqNo != INVALID_SEQNO)
	{
		pModule->m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}

	return nSeqNo;
}
