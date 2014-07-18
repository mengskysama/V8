#include "stdafx.h"
#include "StatReport.h"
#include "Avdefine.h"
#include "WinSock2.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "coredefine.h"
#include "..\room.h"

#define ROOM_STATREPORT_BUFFER_SIZE   10240
#define ROOM_NETSTATE_MIN_TOTALPACKET  10000  //音视频发包收包最底总包个数

#define WM_ROOM_STATREPORT_STEP1  1   //获取本机IP地址申请 第一步 打开页面http://www.ip138.com
#define WM_ROOM_STATREPORT_STEP2  2    //获取本机IP地址申请 第二步 打开上步页面中分析得到的获取IP的url

CString CStatReport::m_ipCity = _T("");

CStatReport::CStatReport()
{
	m_AsyOpenURL = NULL;
	m_hRcvWnd = NULL;
	m_Buffer = NULL;
}

CStatReport::~CStatReport()
{
	ClearAsyOpenURL();
	ClearBuffer();
}

void CStatReport::IPStatReportRequest(HWND hRcvWnd,UINT umMessage)
{
	m_hRcvWnd = hRcvWnd;
	m_umMessage = umMessage;

	CString IpUrl = _T("http://www.ip138.com");
	OpenIPUrl(m_hRcvWnd,m_umMessage,WM_ROOM_STATREPORT_STEP1,IpUrl);


}

//获取本机IP地址申请的返回函数
void CStatReport::IPStatReportResp(UINT unParam)
{
	if(m_Buffer== NULL)
	{
		return ;
	}


	if(unParam == WM_ROOM_STATREPORT_STEP1) 
	{
		IPStatReportRespStep1();
	}
	else //WM_ROOM_STATREPORT_STEP2
	{
		IPStatReportRespStep2();
	}
		
}

void CStatReport::IPStatReportRespStep1()
{
	CString ipUrl = AnalysisIPUrl();
	if(!ipUrl.IsEmpty())
	{
		OpenIPUrl(m_hRcvWnd,m_umMessage,WM_ROOM_STATREPORT_STEP2,ipUrl);
		return ;
	}
}

void CStatReport::IPStatReportRespStep2()
{
	CString ipCity;
	CStringA ipBuffer;
	ipBuffer.Format("%s",m_Buffer);

	CString cIpBuffer(ipBuffer);
	cIpBuffer.MakeLower(); //转小写

	//分析获取IP地址
	CString sfind = _T("<center>");
	int ibegin = cIpBuffer.Find(sfind);
	if(ibegin != -1)
	{
		ibegin = ibegin+sfind.GetLength();
		int iend = cIpBuffer.Find(_T("</center>"),ibegin);
		if(iend != -1)
		{
			CString sbody = cIpBuffer.Mid(ibegin,iend-ibegin);
			int iCity = sbody.Find(_T("来自："));
			if(iCity != -1)
			{
				ipCity = sbody.Mid(iCity+3);
			}

		}
	}

	ClearAsyOpenURL();
	ClearBuffer();

	m_ipCity = ipCity;		
}


//分析获取IP的url
CString CStatReport::AnalysisIPUrl()
{
	if(m_Buffer== NULL)
	{
		return _T("");
	}

	CString ipUrl;
	CStringA ipBuffer;
	ipBuffer.Format("%s",m_Buffer);

	CString cIpBuffer(ipBuffer);
	cIpBuffer.MakeLower(); //转小写

	//分析获取IP地址
	int ifind = cIpBuffer.Find(_T("<iframe"));
	if(ifind != -1)
	{
		int ibegin = cIpBuffer.Find(_T("\""),ifind);
		if(ibegin != -1)
		{
			int iend = cIpBuffer.Find(_T("\""),ibegin+1);
			if(iend != -1)
			{					
				ipUrl = cIpBuffer.Mid(ibegin+1,iend-ibegin-1);				

			}
		}			
	}

	return ipUrl;
}

void CStatReport::OnGetNetState(uint32&iRelayIP, struct Service_Av_Net_Status_All& sansa)
{
	if(iRelayIP == 0 || GetIpCity().IsEmpty())
	{
		return;
	}

// 	if(sansa.iVideoOutTotalPacket < ROOM_NETSTATE_MIN_TOTALPACKET && sansa.iAudioOutTotalPacket < ROOM_NETSTATE_MIN_TOTALPACKET
// 		&& sansa.iVideoInTotalPacket < ROOM_NETSTATE_MIN_TOTALPACKET && sansa.iAudioInTotalPacket < ROOM_NETSTATE_MIN_TOTALPACKET)
// 	{
// 		return;
// 	}

	std::string strRelayIp = "";
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = iRelayIP;
	strRelayIp = inet_ntoa(addr.sin_addr);

	std::wstring wRelayIp = common::utility::stringhelper::AsciiToUnicode(strRelayIp);

	core::ReportCollectInfoEvent *collectInfoEvent = new core::ReportCollectInfoEvent();	
	core::ReportCollectInfoEvent::Key2Value upDown;

	CString upPackVideo,upPackAudio,downPcakVideo,downPcakAudio;
	int irate =0;

	// 格式：   广东电信:Video:60.130.43.1:950
	if(sansa.iVideoOutTotalPacket >= ROOM_NETSTATE_MIN_TOTALPACKET) //下载视频数据包总个数
	{
		irate = sansa.fVideoOutLostRate*100*100;
		upPackVideo.Format(_T("%s:Video:%s:%d"),m_ipCity,wRelayIp.c_str(),irate);  //下载视频数据

		upDown.key = COLLECT_INFO_DOWN;
		upDown.value = upPackVideo;
		collectInfoEvent->vecCollecInfo.push_back(upDown);
	}
	if(sansa.iAudioOutTotalPacket >= ROOM_NETSTATE_MIN_TOTALPACKET) //下载音频数据包总个数
	{
		irate = sansa.fAudioOutLostRate*100*100;
		upPackAudio.Format(_T("%s:Audio:%s:%d"),m_ipCity,wRelayIp.c_str(),irate);  //下载音频数据
		upDown.key = COLLECT_INFO_DOWN;
		upDown.value = upPackAudio;
		collectInfoEvent->vecCollecInfo.push_back(upDown);
	}
	if(sansa.iVideoInTotalPacket >= ROOM_NETSTATE_MIN_TOTALPACKET) //上传视频数据包总个数
	{
		irate = sansa.fVideoInLostRate*100*100;
		downPcakVideo.Format(_T("%s:Video:%s:%d"),m_ipCity,wRelayIp.c_str(),irate);  //上传视频数据
		upDown.key = COLLECT_INFO_UPLOST; 
		upDown.value = downPcakVideo;
		collectInfoEvent->vecCollecInfo.push_back(upDown);
	}
	if(sansa.iAudioInTotalPacket >= ROOM_NETSTATE_MIN_TOTALPACKET) //上传的音频数据包总个数
	{
		irate = sansa.fAudioInLostRate*100*100;
		downPcakAudio.Format(_T("%s:Audio:%s:%d"),m_ipCity,wRelayIp.c_str(),irate);  //上传音频数据
		upDown.key = COLLECT_INFO_UPLOST;
		upDown.value = downPcakAudio;
		collectInfoEvent->vecCollecInfo.push_back(upDown);
	}

	if(collectInfoEvent->vecCollecInfo.size() > 0)
	{
		Event _event = MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_REPORT_COLLECT_INFO, 
			MODULE_ID_CORE,0,0,0,collectInfoEvent);
		g_RoomModule->m_pModuleManager->PushEvent(_event);
	}
	else
	{
		delete collectInfoEvent;
	}
}

void CStatReport::OpenIPUrl(HWND hRcvWnd,UINT umMessage,UINT unParam,CString strUrl)
{
	ClearAsyOpenURL();

	if(m_Buffer == NULL)
	{
		m_Buffer = new BYTE[ROOM_STATREPORT_BUFFER_SIZE];
	}

	if(m_Buffer != NULL)
	{
		memset(m_Buffer,0,ROOM_STATREPORT_BUFFER_SIZE);
		//使用sizeof(m_Buffer)-1 是因为如果m_Buffer大小不够时，最后一位可存放'\0',否则(即若使用sizeof(m_Buffer))，最后存放'\0'时将会下标超出
		m_AsyOpenURL = new common::utility::CAsynOpenURL(hRcvWnd,umMessage,unParam,m_Buffer,ROOM_STATREPORT_BUFFER_SIZE-1);

		m_AsyOpenURL->OpenURL(strUrl);
		
	}
}

CString CStatReport::GetIpCity()
{
	return m_ipCity;
}



void CStatReport::ClearAsyOpenURL()
{
	if(m_AsyOpenURL != NULL)
	{
		delete m_AsyOpenURL;
		m_AsyOpenURL = NULL;
	}
}

void CStatReport::ClearBuffer()
{
	if(m_Buffer != NULL)
	{
		delete m_Buffer;
		m_Buffer = NULL;
	}
}