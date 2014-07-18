#include "stdafx.h"
#include "HeadDownload.h"
#include "webdefine.h"
#include "../hall.h"
#include "../CommonLibrary/include/utility/FileHelper.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include <algorithm>

CHeadDownload::CHeadDownload()
{
	strHeadImageDir.Format(_T("%sresource\\head\\"),common::utility::systemhelper::Get179AppPath().c_str());
}

CHeadDownload::~CHeadDownload()
{

}

 //发送最新头像下载请求
void CHeadDownload::EventSendNewHeadDown(uint32 unUIN)
{
	CString formUrl,headImageDown,unUinImage;		
	unUinImage.Format(_T("%s%d.jpg"),strHeadImageDir,unUIN);//用户头像路径

	WIN32_FIND_DATA filedata;   //以下代码为获取image文件最后修改时间并转为time_t 
	time_t writeTime = 0;
	HANDLE hFile2  = FindFirstFile(unUinImage,&filedata);

	if(hFile2 != INVALID_HANDLE_VALUE)
	{
		common::utility::filehelper::FileTimeToUinxTime(&filedata.ftLastWriteTime,&writeTime); //FILETIME 转 time_t
	}
	
	FindClose(hFile2);         //以下代码为获取image文件最后修改时间并转为time_t 

	formUrl.Format(_T("%s?uin=%d&modifytime=%ld"),_T("http://interface.179.com/user/userDown"),unUIN,writeTime);  //头像下载地址	
	headImageDown.Format(_T("%s%d.jpg"),strHeadImageDir,unUIN);  //头像下载至本地地址

	webinterface::download_head_image_event_req getDownloadEventReq;
	getDownloadEventReq.srcMId	=	MODULE_ID_HALL;
	getDownloadEventReq.desMId	=	MODULE_ID_WEBSERVICE;
	getDownloadEventReq.unUIN = unUIN;
	getDownloadEventReq.wstrUrl = formUrl;
	getDownloadEventReq.wstrImagePath = headImageDown;

	IModuleManager* pModuleManager = g_HallModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		uint32 nHeadSeqNo = pModuleManager->CallService(getDownloadEventReq.eventValue,(param)&getDownloadEventReq);
		m_VecHeadSeqNO.push_back(nHeadSeqNo);
	}
}

//最新头像下载请求返回响应函数，若最新头像下载成功则自动发送消息 MESSAGE_VALUE_HALL_NEW_HEAD_IMAGE
void CHeadDownload::OnEventRespNewHeadDown(Event const& e)
{	
	uint32 nSeqNo = e.param2;

	if(!FindSeqNoInMapHeadImage(nSeqNo)) //查询nSeqNo序列号是否在头像下载序列号表中
	{
		return ;
	}	

	if( e.param0 == webinterface::WEB_RET_SUCCESS)  //已下载最新头像
	{		
		ASSERT(e.m_pstExtraInfo != NULL);
		webinterface::download_head_image_event_resp* pDownEvent =(webinterface::download_head_image_event_resp* ) (e.m_pstExtraInfo);
		MessageHeadDownResp(pDownEvent->unUIN); //发送最新头像下载完毕消息
	}

	//m_VecHeadSeqNO.erase(nSeqNo);


}

//获取本地头像图片路径，若未找到指定unUIN的对应头像图片则使用默认男女头像图片
CString CHeadDownload::GetLocalHeadImage(uint32 unUIN,bool bMan)
{	
	CString unUinImage;
	unUinImage.Format(_T("%s%d.jpg"),strHeadImageDir,unUIN);//用户头像路径

	WIN32_FIND_DATA filedata;  
	HANDLE hFile2  = FindFirstFile(unUinImage,&filedata);
	if(hFile2 == INVALID_HANDLE_VALUE)  //未找到用户unUIN所对应的头像图片  即需要使用默认男女头像图片
	{
		if(bMan)
		{
			unUinImage.Format(_T("%smen.jpg"),strHeadImageDir);
		}
		else
		{
			unUinImage.Format(_T("%swomen.jpg"),strHeadImageDir);
		}

	}	
	FindClose(hFile2); 
	return unUinImage;

}

//获取本地默认游客图片
CString CHeadDownload::GetGuestHeadImage()
{
	CString unUinImage;		
	unUinImage.Format(_T("%sdefault.png"),strHeadImageDir);	
	return unUinImage;
}

void CHeadDownload::MessageHeadDownResp(uint32 unUIN)
{
	IModuleManager * pModuleManager = g_HallModule->m_pModuleManager;	
	if (pModuleManager != NULL)
	{
		pModuleManager->PushMessage(MakeMessage<MODULE_ID_HALL>()(Hall::MESSAGE_VALUE_HALL_HEAD_IMAGE_UPDATE,(param)unUIN , 0, 0, 0));
	}
}

//查询nSeqNo序列号是否在头像下载序列号表中
bool CHeadDownload::FindSeqNoInMapHeadImage(uint32 nSeqNo)
{
	bool seqNoOK = false;	
	vector<uint32>::iterator result = ::find(m_VecHeadSeqNO.begin(),m_VecHeadSeqNO.end(),nSeqNo); //在头像下载序列号表中查找nSeqNo
	if(result != m_VecHeadSeqNO.end()) //找到
	{
		seqNoOK = true;
	}

	return seqNoOK;

}
