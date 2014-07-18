#include "StdAfx.h"
#include "RoomAVLogic.h"
#include <assert.h>
#include "ximage.h"
#include "utility/StringHelper.h"

using namespace common::utility;

void RoomAVLogic::ConnectRelayServer(uint32 iRelayIP,uint32 iRelayPort)
{
	MutexLocker locker(&m_mutex);//房间线程 
	if (m_bIsStart) return;

	this->m_iRelayIP = iRelayIP;
	this->m_sRelayPort = iRelayPort;

	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{  //获取中转库运行开始时间
		Service_AV_Req_Time srt;
		memset(&srt,0,sizeof(Service_AV_Req_Time));
		pModuleManager->CallService(SERVICE_AV_RELAY_REQ,(param)&srt);
		this->m_nRelayStartTime = srt.lRelaySymbolTime;
	}
	
	{   //设置中转服务器IP Port
		Service_Relay_Config src;
		memset(&src,0,sizeof(Service_Relay_Config));
		src.iMyUin = this->m_iSelfUin;
		src.unP2PTimeout = MAX_CONNECTING_TIME;
		src.iRelayIP = this->m_iRelayIP;
		src.sRelayPort = this->m_sRelayPort;
		pModuleManager->CallService(SERVICE_AV_RELAY_CFG,(param)&src);
	}
	
	{	//配置视频采集大小等参数
		Service_AV_Video_Cfg svc;
		memset(&svc,0,sizeof(Service_AV_Video_Cfg));
		svc.iDeviceID = this->m_iDevice;
		svc.iHeight = this->m_iHeight;
		svc.iWidth = this->m_iWidth;
		svc.iQuality = this->m_iQuality;
		svc.iVideoMode = (uin)this->m_eVideoMode;
		pModuleManager->CallService(SERVICE_AV_VIDEO_CFG,(param)&svc);
	}

	{ //设置音频采集设备
		Service_AV_Audio_Cfg svac;
		memset(&svac,0,sizeof(Service_AV_Audio_Cfg));
		svac.iInputDev = this->m_iInputDev;
		svac.iOutputDev = this->m_iOutputDev;
		pModuleManager->CallService(SERVICE_AV_AUDIO_CFG,(param)&svac);
	}
	
	{//设置音视频Key
		Service_Av_Key_Set_Cfg saksc;
		memset(&saksc,0,sizeof(Service_Av_Key_Set_Cfg));
		saksc.iKey = this->m_iKey;
		saksc.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_KEY_SET_CFG,(param)&saksc);
	}

	{  //创建UDP连接，连接中转服务器
		Service_AV_Relay_Conn src;
		memset(&src,0,sizeof(Service_AV_Relay_Conn));
		src.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_RELAY_CONN,(param)&src);
	}
	m_bIsStart = true; //已经连接过服务器
}

void RoomAVLogic::SetVideoCfg(int iDevice,int iHeight,int iWidth,int iQuality,enuROOM_AV_VIDEO_MODE eMode/* = ROOM_VIDEO_CRF_MODE*/)
{
	this->m_iDevice = iDevice;
	this->m_iHeight = iHeight;
	this->m_iWidth = iWidth;
	this->m_iQuality = iQuality;
	this->m_eVideoMode = eMode;
}

void RoomAVLogic::SetAudioCfg(int iInputDev,int iOutputDev)
{
	this->m_iInputDev = iInputDev;
	this->m_iOutputDev = iOutputDev;
}

void RoomAVLogic::SetKeyCfg(uint32 iKey)
{
	this->m_iKey = iKey;
}

bool RoomAVLogic::FlushVideoDev(int iDev)//刷新视频设备 用于打开虚拟摄像头程序
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_Av_Flush_Video_Dev safvd;
		safvd.iDev = iDev;
		pModuleManager->CallService(SERVICE_AV_FLUSH_VIDEO_DEV,(param)&safvd);
		return safvd.bRet;
	}
}

bool RoomAVLogic::ChangeRelayServer(uint32 iRelayIp,uint16 sRelayPort) //变更中转服务器地址
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_Av_Change_Relay_Svr safvd;
		safvd.iRoomId = this->m_iRoomId;
		safvd.ip = iRelayIp;
		safvd.sport = sRelayPort;
		pModuleManager->CallService(SERVICE_AV_CHANGE_RELAY_SVR,(param)&safvd);
		return safvd.bret;
	}
}

void RoomAVLogic::UpSelf(HWND hWndLocal,enuVIDEO_TYPE enVideoType)
{
	Trace("RoomAVLogic::UpSelf begin\n");
	MutexLocker locker(&m_mutex);//房间线程 
	if (ROOM_AV_AV == m_rState || ROOM_AV_AUDIO == m_rState) return;

	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	if (!IsVideoDeviceInit(this->m_iDevice))
	{	//配置视频采集大小等参数
		Service_AV_Video_Cfg svc;
		memset(&svc,0,sizeof(Service_AV_Video_Cfg));
		svc.iDeviceID = this->m_iDevice;
		svc.iHeight = this->m_iHeight;
		svc.iWidth = this->m_iWidth;
		svc.iQuality = this->m_iQuality;
		pModuleManager->CallService(SERVICE_AV_VIDEO_CFG,(param)&svc);
	}

	Trace("RoomAVLogic::UpSelf SERVICE_AV_VIDEO_CFG\n");
	m_rState = ROOM_AV_AV;
	this->m_hWndLocal = hWndLocal;
	{//开启本地语音
		Trace("RoomAVLogic::UpSelf开始开启本地语音..... \n");
		Service_AV_Audio_Start sas;
		memset(&sas,0,sizeof(Service_AV_Audio_Start));
		sas.cLogicID = m_cLogicId;
		sas.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_START,(param)&sas);
	}
	
	Trace("RoomAVLogic::UpSelf SERVICE_AV_AUDIO_START\n");
	{//开启本地视频
		Trace("RoomAVLogic::UpSelf开始开启本地视频..... \n");
		Service_AV_Video_Start svs;	
		memset(&svs,0,sizeof(Service_AV_Video_Start));
		svs.hWndLocal = hWndLocal;
		this->m_hWndLocal = hWndLocal; 
		svs.iDeviceId = this->m_iDevice;
		svs.iRoomId = this->m_iRoomId;
		svs.enType = enVideoType;
		pModuleManager->CallService(SERVICE_AV_VIDEO_START,(param)&svs);
	}
	
	Trace("RoomAVLogic::UpSelf end\n");
}

void RoomAVLogic::DownSelf()
{
	Trace("RoomAVLogic::DownSelf begin\n");
	MutexLocker locker(&m_mutex);//房间线程 
	if (ROOM_AV_AV != m_rState) return;
	m_rState = ROOM_AV_NONE;
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//用户下公麦、私麦、私聊等关闭语音
		Service_AV_Audio_Stop sas;
		memset(&sas,0,sizeof(Service_AV_Audio_Stop));
		sas.cLogicID = this->m_cLogicId;
		sas.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_STOP,(param)&sas);
	}

	{//用户下公麦、私麦、私聊等关闭视频
		Service_AV_Video_Stop svs;
		memset(&svs,0,sizeof(Service_AV_Video_Stop));
		svs.hWndLocal = this->m_hWndLocal;
		svs.iDeviceID = this->m_iDevice;
		svs.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_VIDEO_STOP,(param)&svs);
	}
	Trace("RoomAVLogic::DownSelf end\n");
}

void RoomAVLogic::UpSelfVoice()//开启语音 仅开启语音，不开启视频
{
	MutexLocker locker(&m_mutex);//房间线程 
	if (ROOM_AV_AV == m_rState || ROOM_AV_AUDIO == m_rState) return;
	m_rState = ROOM_AV_AUDIO;
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//开启本地语音
		Service_AV_Audio_Start sas;
		memset(&sas,0,sizeof(Service_AV_Audio_Start));
		sas.cLogicID = m_cLogicId;
		sas.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_START,(param)&sas);
	}
}

void RoomAVLogic::DownSelfVoice()//关闭语音
{
	MutexLocker locker(&m_mutex);//房间线程 
	if (ROOM_AV_AUDIO != m_rState) return;
	m_rState = ROOM_AV_NONE;
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//关闭语音
		Service_AV_Audio_Stop sas;
		memset(&sas,0,sizeof(Service_AV_Audio_Stop));
		sas.cLogicID = this->m_cLogicId;
		sas.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_STOP,(param)&sas);
	}
}

void RoomAVLogic::RecvAV(uint32 iFromUin,HWND hWndPartner,UInt32 iWidth, UInt32 iHeight,enuVIDEO_TYPE enVideoType /*= VIDEO_PRIV_MIC*/,enuROOM_AV_VIDEO_MODE eMode /*= ROOM_VIDEO_CRF_MODE*/)//接收公麦、私麦、私聊视频+语音
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//添加公麦等用户 语音
		Service_Av_Audio_Opp_Start saos;
		memset(&saos,0,sizeof(Service_Av_Audio_Opp_Start));
		saos.lRelaySymbolTime = this->m_nRelayStartTime;
		saos.iFromUin = iFromUin;
		saos.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_OPP_START,(param)&saos);

		AddUserMap(iFromUin,saos.cLogicID,ROOM_AV_OPP_AUDIO);
	}

	{//添加公麦等用户 视频
		Service_Av_Video_Opp_Start svos;
		memset(&svos,0,sizeof(Service_Av_Audio_Opp_Start));
		svos.lRelaySymbolTime = this->m_nRelayStartTime;
		svos.iFromUin = iFromUin;
		svos.iRoomId = this->m_iRoomId;
		svos.iHeight = iHeight;//this->m_iHeight;
		svos.iQuality = this->m_iQuality;
		svos.iWidth = iWidth;//this->m_iWidth;
		svos.iVideoMode = (enuROOM_AV_VIDEO_MODE)eMode;
		svos.hWndPartner=hWndPartner;
		svos.enType=enVideoType;
		pModuleManager->CallService(SERVICE_AV_VIDEO_OPP_START,(param)&svos);

		AddUserMap(iFromUin,svos.cLogicID,ROOM_AV_OPP_VIDEO,hWndPartner);
	}
}



void RoomAVLogic::RecvAVChg(uint32 iFromUin,HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_AV_Video_Change_Handle svch;
		svch.hOldWnd = hOldWnd;
		svch.hWndPartner = hPartnerWnd;
		svch.iHeight = iHeight;
		svch.iWidth = iWidth;
		svch.iRoomId = this->m_iRoomId;
		svch.iFromUin = iFromUin;
		pModuleManager->CallService(SERVICE_AV_VIDEO_CHG_HANDLE,(param)&svch);
	}
}

void RoomAVLogic::LocalAVChg(HWND hNewWnd,HWND hOldWnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		if (hOldWnd != this->m_hWndLocal){
			AssertVS(0,0,"老句柄不存在...");
		}
		this->m_hWndLocal = hNewWnd;
		Service_AV_Video_Change_LocalHDL savcl;
		savcl.iRoomId = this->m_iRoomId;
		savcl.hNewWnd = hNewWnd;
		savcl.hOldWnd = hOldWnd;
		pModuleManager->CallService(SERVICE_AV_VIDEO_CHG_LOCALHDL,(param)&savcl);
	}
}

void RoomAVLogic::LocalAVEffact(HWND hLocalWnd,bool bEnable)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_AV_Video_Chg_Effact savce;
		savce.iRoomId = this->m_iRoomId;
		savce.hWndLocal = hLocalWnd;
		savce.bEnable = bEnable;
		pModuleManager->CallService(SERVICE_AV_VIDEO_CHG_EFFACT,(param)&savce);
	}
}

bool RoomAVLogic::LocalPaintBk(HWND hLocalWnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_Av_Video_Paint_Local savpl;
		savpl.hWnd = hLocalWnd;
		savpl.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_VIDEO_PAINT_LOCAL,(param)&savpl);
		return savpl.bRet;
	}
	return false;
}

bool RoomAVLogic::RemotePaintBk(uint32 iFromUin,HWND hPartnerWnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_Av_Video_Paint_Remote savpr;
		savpr.hWnd = hPartnerWnd;
		savpr.iFromUin = iFromUin;
		savpr.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_VIDEO_PAINT_REMOTE,(param)&savpr);
		return savpr.bRet;
	}
	return false;
}

bool RoomAVLogic::IsLocalAVEffact( HWND localHwnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{
		Service_Is_Open_Av_Effact sioae;
		sioae.iRoomId = this->m_iRoomId;
		sioae.hLocalWnd = localHwnd;
		pModuleManager->CallService(SERVICE_IS_OPEN_AV_EFFACT,(param)&sioae);
		return sioae.bOpen;
	}
	return false;
}

void RoomAVLogic::RemoveAV(uint32 iFromUin)//移除公麦、私麦、私聊用户视频+语音
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//关闭公麦等语音
		RCPtr<strucROOMAVUSER> pUserRoom = FindUserMap(iFromUin,ROOM_AV_OPP_AUDIO);
		if (!pUserRoom) return;

		Service_Av_Audio_Opp_Stop saos;
		memset(&saos,0,sizeof(Service_Av_Audio_Opp_Stop));
		saos.cLogicID = pUserRoom->cLogicId;
		saos.iFromUin =  iFromUin;
		saos.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_OPP_STOP,(param)&saos);

		RemoveUserMap(pUserRoom->cLogicId);
	}

	{//关闭公麦等视频
		RCPtr<strucROOMAVUSER> pUserRoom = FindUserMap(iFromUin,ROOM_AV_OPP_VIDEO);
		if (!pUserRoom) return;

		Service_Av_Video_Opp_Stop svos;
		memset(&svos,0,sizeof(Service_Av_Audio_Opp_Stop));
		svos.cLogicID = pUserRoom->cLogicId;
		svos.iFromUin = iFromUin;
		svos.hWndPartner = pUserRoom->hWndPartner;
		svos.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_VIDEO_OPP_STOP,(param)&svos);

		RemoveUserMap(pUserRoom->cLogicId);
	}
}

void RoomAVLogic::RecvAudio(uint32 iFromUin)//接收私聊语音
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//添加私麦等用户 语音
		Service_Av_Audio_Opp_Start saos;
		memset(&saos,0,sizeof(Service_Av_Audio_Opp_Start));
		saos.lRelaySymbolTime = this->m_nRelayStartTime;
		saos.iFromUin = iFromUin;
		saos.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_OPP_START,(param)&saos);
		
		AddUserMap(iFromUin,saos.cLogicID,ROOM_AV_OPP_AUDIO);
	}
}

void RoomAVLogic::RemoveAudio(uint32 iFromUin)//移除私聊语音
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//关闭私麦等语音
		RCPtr<strucROOMAVUSER> pUserRoom = FindUserMap(iFromUin,ROOM_AV_OPP_AUDIO);
		if (!pUserRoom) return;

		Service_Av_Audio_Opp_Stop saos;
		memset(&saos,0,sizeof(Service_Av_Audio_Opp_Stop));
		saos.cLogicID = pUserRoom->cLogicId;
		saos.iFromUin =  iFromUin;
		saos.iRoomId = this->m_iRoomId;
		pModuleManager->CallService(SERVICE_AV_AUDIO_OPP_STOP,(param)&saos);

		RemoveUserMap(pUserRoom->cLogicId);
	}
}

bool RoomAVLogic::IsVideoDeviceInit(int iDevice)//视频设备是否初始化
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Is_Video_Dev_Init sivdi;
	sivdi.iDev = iDevice;
	sivdi.bInit = false;
	pModuleManager->CallService(SERVICE_IS_VIDEO_DEV_INIT,(param)&sivdi);
	return sivdi.bInit;
}

void RoomAVLogic::ClearRoomAV() //房间退出时，清理该房间分配的资源
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	{//房间退出时，清理房间分配的资源
		Service_Av_Clear_Exit sace;
		sace.iRoomId = this->m_iRoomId;
		sace.bRet = false;
		pModuleManager->CallService(SERVICE_AV_CLEAR_EXIT,(param)&sace);
	}
}

void RoomAVLogic::AddUserMap(uint32 iFromUin,uint8 cLogicId,enuROOM_AV_USER_STATE eState,HWND hWndPartner)
{
	MutexLocker locker(&m_mutex);//房间线程 
	RCPtr<strucROOMAVUSER> pRoomUser(new strucROOMAVUSER());
	pRoomUser->cLogicId = cLogicId;
	pRoomUser->eState = eState;
	pRoomUser->iFromUin = iFromUin;
	pRoomUser->hWndPartner = hWndPartner;
	m_userMap[pRoomUser->cLogicId] = pRoomUser;
}

RCPtr<strucROOMAVUSER> RoomAVLogic::FindUserMap(uint32 iFromUin,enuROOM_AV_USER_STATE eState)
{
	MutexLocker locker(&m_mutex);//房间线程 
	RCPtr<strucROOMAVUSER> p(NULL);
	std::map<uint8,RCPtr<strucROOMAVUSER>>::iterator iter;
	for(iter=m_userMap.begin();iter!=m_userMap.end();++iter)
	{
		if (eState == iter->second->eState && iFromUin == iter->second->iFromUin)
		{
			return iter->second;
		}
	}
	return p;
}

void RoomAVLogic::RemoveUserMap(uint8 cLogicId)
{
	MutexLocker locker(&m_mutex);//房间线程 
	m_userMap.erase(m_userMap.find(cLogicId));
}

void RoomAVLogic::PauseLocalAudio(bool bPause)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_AV_Audio_Local sval;
	memset(&sval,0,sizeof(Service_AV_Audio_Local));
	sval.bPause = bPause;
	sval.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_AV_AUDIO_LOCAL,(param)&sval);
}

void RoomAVLogic::PauseRemoteAudio(uint32 iFromUin,bool bPause)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_AV_Audio_Remote svar;
	memset(&svar,0,sizeof(Service_AV_Audio_Remote));
	svar.bPause = bPause;
	svar.iFromUin = iFromUin;
	svar.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_AV_AUDIO_REMOTE,(param)&svar);
}

void RoomAVLogic::PauseLocalVideo(bool bPause)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_AV_Video_Local svvl;
	memset(&svvl,0,sizeof(Service_AV_Video_Local));
	svvl.bPause = bPause;
	svvl.iRoomId = this->m_iRoomId;
	svvl.hWndLocal = this->m_hWndLocal;
	pModuleManager->CallService(SERVICE_AV_VIDEO_LOCAL,(param)&svvl);
}

void RoomAVLogic::PauseRemoteVideo(uint32 iFromUin,bool bPause)
{
	MutexLocker locker(&m_mutex);//房间线程 
	RCPtr<strucROOMAVUSER> pUserRoom = FindUserMap(iFromUin,ROOM_AV_OPP_VIDEO);
	if (!pUserRoom) return;

	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_AV_Video_Remote savr;
	memset(&savr,0,sizeof(Service_AV_Video_Remote));
	savr.bPause = bPause;
	savr.hWndPartner = pUserRoom->hWndPartner;
	savr.iFromUin = iFromUin;
	savr.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_AV_VIDEO_REMOTE,(param)&savr);
}

bool RoomAVLogic::IsPhotoableLocalHwnd(HWND localHwnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Is_Photo_Able_Local sipal;
	memset(&sipal,0,sizeof(Service_Is_Photo_Able_Local));
	sipal.localHwnd = localHwnd;
	sipal.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_IS_PHOTO_ABLE_LOCAL,(param)&sipal);
	return sipal.bAble;
}

bool RoomAVLogic::IsPhotoableRemoteHwnd(UInt32 iFromUin, HWND partnerHwnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Is_Photo_Able_Remote sipar;
	memset(&sipar,0,sizeof(Service_Is_Photo_Able_Remote));
	sipar.iFromUin = iFromUin;
	sipar.partnerHwnd = partnerHwnd;
	sipar.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_IS_PHOTO_ABLE_REMOTE,(param)&sipar);
	return sipar.bAble;
}

bool RoomAVLogic::IsVideoVirtualDevice(UInt32 iDevId)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	
	Service_Av_Video_Virtual savv;
	savv.iDeviceId = iDevId;
	pModuleManager->CallService(SERVICE_AV_VIDEO_VIRTUAL,(param)&savv);
	return savv.bReturn;
}

bool RoomAVLogic::IsStartAVEngine()
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Is_Start_Av_Engine sisae;
	sisae.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_IS_START_AV_ENGINE,(param)&sisae);
	return sisae.bAble;
}

bool RoomAVLogic::ShowVideoAjustDlg(HWND hParentWnd,uint32 iDeviceId)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Show_Video_Ajust ssva;
	ssva.hParentWnd = hParentWnd;
	ssva.iDeviceID = iDeviceId;
	pModuleManager->CallService(SERVICE_SHOW_VIDEO_AJUST,(param)&ssva);
	return true;
}

bool RoomAVLogic::TestCheckSoundDevice(bool bOpen)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Check_Sound_Device scsd;
	scsd.bOpen = bOpen;
	pModuleManager->CallService(SERVICE_CHK_SOUND_DEVICE,(param)&scsd);
	return true;
}

bool RoomAVLogic::TestSetAudioDevice(int iInputDevice, int iOutputDevice)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_AV_Audio_Cfg svac;
	svac.iInputDev = iInputDevice;
	svac.iOutputDev = iOutputDevice;
	pModuleManager->CallService(SERVICE_AV_AUDIO_CFG,(param)&svac);
	return true;
}

bool RoomAVLogic::TestSetDefaultAudioDevice()
{
	MutexLocker locker(&m_mutex);//房间线程 
	TestSetAudioDevice(this->m_iInputDev,this->m_iOutputDev);
	return true;
}

void   RoomAVLogic::SetSpeakerVolume(UInt32 iVolume)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Speaker_Volume sssv;
	sssv.iRoomId = this->m_iRoomId;
	sssv.iVolume = iVolume;
	pModuleManager->CallService(SERVICE_SET_SPEAKER_VOLUME,(param)&sssv);
}

UInt32 RoomAVLogic::GetSpeakerVolume()
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Speaker_Volume sgsv;
	sgsv.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_GET_SPEAKER_VOLUME,(param)&sgsv);
	return sgsv.iVolume;
}

void RoomAVLogic::SetSpeakerRoomVolume(UInt32 iVolume)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Speaker_Room_Volume sssv;
	sssv.iRoomId = this->m_iRoomId;
	sssv.iVolume = iVolume;
	pModuleManager->CallService(SERVICE_SET_SPEAKER_ROOM_VOLUME,(param)&sssv);
}

UInt32 RoomAVLogic::GetSpeakerRoomVolume()
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Speaker_Room_Volume sgsv;
	sgsv.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_GET_SPEAKER_ROOM_VOLUME,(param)&sgsv);
	return sgsv.iVolume;
}

void   RoomAVLogic::SetMicRoomAllVolume(UInt32 iVolume)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Room_All_Volume sssv;
	sssv.iVolume = iVolume;
	pModuleManager->CallService(SERVICE_SET_ROOM_ALL_VOLUME,(param)&sssv);
}

UInt32 RoomAVLogic::GetMicRoomAllVolume()
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Room_All_Volume sgsv;
	pModuleManager->CallService(SERVICE_GET_ROOM_ALL_VOLUME,(param)&sgsv);
	return sgsv.iVolume;
}

void RoomAVLogic::SetSysPlayerVolume(UInt32 iVolume)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_SysPlayer_Volume sssv;
	sssv.iRoomId = this->m_iRoomId;
	sssv.iVolume = iVolume;
	pModuleManager->CallService(SERVICE_SET_SYSPLAYER_VOLUME,(param)&sssv);
}

UInt32 RoomAVLogic::GetSysPlayerVolume()
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_SysPlayer_Volume sgsv;
	sgsv.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_GET_SYSPLAYER_VOLUME,(param)&sgsv);
	return sgsv.iVolume;
}

void   RoomAVLogic::SetMicVolume(UInt32 iVolume)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Mic_Volume ssmv;
	ssmv.iRoomId = this->m_iRoomId;
	ssmv.iVolume = iVolume;
	pModuleManager->CallService(SERVICE_SET_MIC_VOLUME,(param)&ssmv);
}

UInt32 RoomAVLogic::GetMicVolume()
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Mic_Volume sgmv;
	sgmv.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_GET_MIC_VOLUME,(param)&sgmv);
	return sgmv.iVolume;
}

void RoomAVLogic::GetSoundLevel(UInt32 &iOutput, UInt32 &iInput)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Sound_Level sgsl; 
	pModuleManager->CallService(SERVICE_GET_SOUND_LEVEL,(param)&sgsl);
	iOutput = sgsl.iOutput;
	iInput = sgsl.iInput;
}

void RoomAVLogic::GetRecordDeviceName(int iIndex, wchar_t *pDeviceName,int iNameLen)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Record_Device_Name sgrd;
	sgrd.iDeviceIndex = iIndex;
	pModuleManager->CallService(SERVICE_GET_MIC_VOLUME,(param)&sgrd);
	wcsncpy_s(pDeviceName,iNameLen,&sgrd.strDeviceName[0],DEVICE_NAME_LEN);
}

void RoomAVLogic::GetPlayoutDeviceName(int iIndex, wchar_t *pDeviceName, int iNameLen)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_PlayOut_Device_Name sgpd;
	sgpd.iDeviceIndex = iIndex;
	pModuleManager->CallService(SERVICE_GET_PLAYOUT_DEVICE_NAME,(param)&sgpd);
	wcsncpy_s(pDeviceName,iNameLen,&sgpd.strDeviceName[0],DEVICE_NAME_LEN);
}

void RoomAVLogic::GetAudioDeviceList(int &iDeviceNum,wchar_t wsDeviceName[][DEVICE_NAME_LEN], bool bInput)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Audio_Device_List sgad;
	sgad.bInput = bInput;
	pModuleManager->CallService(SERVICE_GET_AUDIO_DEVICE_LIST,(param)&sgad);
	for(int i=0;i<sgad.iDeviceNum;i++)
	{
		wcsncpy_s(&wsDeviceName[i][0],DEVICE_NAME_LEN,&sgad.wsDeviceName[i][0],DEVICE_NAME_LEN);
	}
	iDeviceNum = sgad.iDeviceNum;
}


void RoomAVLogic::GetVideoDeviceList(int &iDeviceNum,wchar_t wsDeviceName[][DEVICE_NAME_LEN])
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Video_Device_List sgvd;
	pModuleManager->CallService(SERVICE_GET_VIDEO_DEVICE_LIST,(param)&sgvd);
	for(int i=0;i<sgvd.iDeviceNum;i++)
	{
		wcsncpy_s(&wsDeviceName[i][0],DEVICE_NAME_LEN,&sgvd.wsDeviceName[i][0],DEVICE_NAME_LEN);
	}
	iDeviceNum = sgvd.iDeviceNum;
}

void RoomAVLogic::GetVideoDeviceInfoList(int&iDeviceNum,Service_Video_Info struDeviceInfo[])
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Video_DevInfo_List sgvdil;
	pModuleManager->CallService(SERVICE_GET_VIDEO_DEVINFO_LIST,(param)&sgvdil);
	for(int i=0;i<sgvdil.iDeviceNum;i++)
	{
		memcpy_s(&struDeviceInfo[i],sizeof(Service_Video_Info),&sgvdil.strucDeviceName[i],sizeof(Service_Video_Info));
	}
	iDeviceNum = sgvdil.iDeviceNum;
}

void RoomAVLogic::GetNumOfSoundDevices(int &iPlayNum, int &iRecordNum)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Sound_Device sgsd;
	pModuleManager->CallService(SERVICE_GET_SOUND_DEVICE_NUM,(param)&sgsd);
	iPlayNum = sgsd.iPlayout;
	iRecordNum = sgsd.iRecord;
}

void RoomAVLogic::TestSafeCloseInputDevice()
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	pModuleManager->CallService(SERVICE_AV_AUDIO_STOP_SAFE,NULL);
}

void RoomAVLogic::TestSetVideoDevice(int iDeviceId)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_AV_Video_Cfg svc;
	memset(&svc,0,sizeof(Service_AV_Video_Cfg));
	svc.iDeviceID = iDeviceId;
	svc.iHeight = this->m_iHeight;
	svc.iWidth = this->m_iWidth;
	svc.iQuality = this->m_iQuality;
	pModuleManager->CallService(SERVICE_AV_VIDEO_CFG,(param)&svc);
}

bool RoomAVLogic::TestStartVideoDevice(int iDeviceId,HWND hWnd,UInt32 &iResult)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Av_Video_Start_Test savst;
	memset(&savst,0,sizeof(Service_Av_Video_Start_Test));
	savst.iDeviceId = iDeviceId;
	savst.hWnd = hWnd;
	pModuleManager->CallService(SERVICE_AV_VIDEO_START_TEST,(param)&savst);
	iResult = savst.iResult;
	return savst.bRet;
}

bool RoomAVLogic::TestStopVideoDevice(int iDeviceId,HWND hWnd)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Av_Video_Stop_Safe savss;
	memset(&savss,0,sizeof(Service_Av_Video_Stop_Safe));
	savss.hWnd = hWnd;
	savss.iDeviceId = iDeviceId;
	pModuleManager->CallService(SERVICE_AV_VIDEO_STOP_SAFE,(param)&savss);
	return savss.bRet;
}

bool   RoomAVLogic::SetMicMute(bool bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Mic_Mute ssmm;
	ssmm.iRoomId  = this->m_iRoomId;
	ssmm.bMute = bMute;
	pModuleManager->CallService(SERVICE_SET_MIC_MUTE,(param)&ssmm);
	return ssmm.bResult;
}

bool   RoomAVLogic::SetPlayerMute(bool bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Player_Mute sspm;
	sspm.iRoomId = this->m_iRoomId;
	sspm.bMute = bMute;
	pModuleManager->CallService(SERVICE_SET_PLAYER_MUTE,(param)&sspm);
	return sspm.bResult;
}

bool   RoomAVLogic::SetSondMute(bool bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Sound_Mute sssm;
	sssm.iRoomId = this->m_iRoomId;
	sssm.bMute = bMute;
	pModuleManager->CallService(SERVICE_SET_SOUND_MUTE,(param)&sssm);
	return sssm.bResult;
}

bool   RoomAVLogic::GetMicMute(bool& bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Mic_Mute sgmm;
	sgmm.iRoomId = this->m_iRoomId;
	sgmm.bMute = false;
	pModuleManager->CallService(SERVICE_GET_MIC_MUTE,(param)&sgmm);
	bMute =  sgmm.bMute;
	return sgmm.bResult;
}

bool   RoomAVLogic::GetPlayerMute(bool& bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Player_Mute sgpm;
	sgpm.iRoomId = this->m_iRoomId;
	sgpm.bMute = false;
	pModuleManager->CallService(SERVICE_GET_PLAYER_MUTE,(param)&sgpm);
	bMute = sgpm.bMute;
	return sgpm.bResult;
}

bool   RoomAVLogic::GetSoundMute(bool& bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Sound_Mute sgsm;
	sgsm.iRoomId = this->m_iRoomId;
	sgsm.bMute = false;
	pModuleManager->CallService(SERVICE_GET_SOUND_MUTE,(param)&sgsm);
	bMute = sgsm.bMute;
	return sgsm.bResult;
}

void RoomAVLogic::SavePhotoToBmp(HWND hWnd, CString strPath)
{
	MutexLocker locker(&m_mutex);//房间线程 
	CWnd* pWnd = CWnd::FromHandle(hWnd);
	if (pWnd)
	{
		RECT rcWindow;
		pWnd->GetWindowRect(&rcWindow);
		int nWidth = rcWindow.right - rcWindow.left;
		int nHeight = rcWindow.bottom - rcWindow.top;

		CDC* pDC = CWnd::FromHandle(hWnd)->GetWindowDC();
		if (pDC)
		{
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, nWidth, nHeight);

			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap* pOldBmp = memDC.SelectObject(&bmp);
			memDC.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);

			CxImage xImagebmp(CXIMAGE_FORMAT_BMP);
			xImagebmp.CreateFromHBITMAP((HBITMAP)bmp);
			if(!xImagebmp.IsGrayScale()) xImagebmp.IncreaseBpp(24);
			xImagebmp.Save(stringhelper::UnicodeToAscii(strPath.GetBuffer()).c_str(), CXIMAGE_FORMAT_BMP);
			memDC.SelectObject(pOldBmp);
			bmp.DeleteObject();
			memDC.DeleteDC();
		}

		CWnd::FromHandle(hWnd)->ReleaseDC(pDC);
	}
}

bool   RoomAVLogic::GetAllSoundMute(bool& bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Room_All_Mute sgsm;
	pModuleManager->CallService(SERVICE_GET_ROOM_ALL_MUTE,(param)&sgsm);
	bMute = sgsm.bMute;
	return sgsm.bResult;
}

bool   RoomAVLogic::SetAllSoundMute(bool bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Room_All_Mute sgsm;
	sgsm.bMute = bMute;
	pModuleManager->CallService(SERVICE_SET_ROOM_ALL_MUTE,(param)&sgsm);
	return sgsm.bResult;
}

bool RoomAVLogic::GetAllMicMute(bool& bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Room_Mic_Mute ssrmm;
	pModuleManager->CallService(SERVICE_GET_ROOM_MIC_MUTE,(param)&ssrmm);
	 bMute = ssrmm.bMute;
	return ssrmm.bResult;	
}

bool RoomAVLogic::SetAllMicMute(bool bMute)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Room_Mic_Mute ssrmm;
	ssrmm.bMute = bMute;
	pModuleManager->CallService(SERVICE_SET_ROOM_MIC_MUTE,(param)&ssrmm);
	return ssrmm.bResult;
}

void RoomAVLogic::GetNetState(float& fRate)
{	
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Net_Start_Run_Rate snsrr;
	pModuleManager->CallService(SERVICE_NET_STATE_RUN_RATE,(param)&snsrr);
	fRate = snsrr.fRate;
}

bool RoomAVLogic::GetNetState(uint32&iRelayIP,struct Service_Av_Net_Status_All& sansa) //获取音视频发包收包丢包率等状态
{
	iRelayIP = this->m_iRelayIP;
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Av_Net_Status_All_Ex sav;
	sav.iRoomId = this->m_iRoomId;
	pModuleManager->CallService(SERVICE_AV_NET_STATUS_ALL,(param)&sav);
	memcpy(&sansa,&sav.sansa,sizeof(Service_Av_Net_Status_All));
	return true;
}

bool RoomAVLogic::SetMicGreater(bool bEnable)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_Mic_Greater ssmg;
	ssmg.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_SET_MIC_GREATER,(param)&ssmg);
	return ssmg.bReturn;
}

bool RoomAVLogic::GetMicGreater(bool& bEnable)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_Mic_Greater sgmg;
	pModuleManager->CallService(SERVICE_GET_MIC_GREATER,(param)&sgmg);
	bEnable = sgmg.bEnable;
	return sgmg.bReturn;
}

void RoomAVLogic::SetViewFFTWav(UInt32 iFromUin,bool bEnable)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_View_FFT_Wav savf;
	savf.iFromUin = iFromUin;
	savf.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_SET_VIEW_FFT_WAV,(param)&savf);
}

void RoomAVLogic::SetViewFFTALL(bool bEnable)//设置所有是否显示波形图
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_View_FFT_ALL  ssvfa;
	ssvfa.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_SET_VIEW_FFT_ALL,(param)&ssvfa);
}

void RoomAVLogic::SetViewFFTME(bool bEnable)//设置自己是否显示波形图
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Set_View_FFT_ME  ssvfa;
	ssvfa.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_SET_VIEW_FFT_ME,(param)&ssvfa);
}

bool RoomAVLogic::GetViewFFTWav(UInt32 iFromUin)
{
	MutexLocker locker(&m_mutex);//房间线程 
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Get_View_FFT_Wav savf;
	savf.iFromUin = iFromUin;
	pModuleManager->CallService(SERVICE_GET_VIEW_FFT_WAV,(param)&savf);
	return savf.bEnable;
}

bool RoomAVLogic::SetMixerControlTotalVolume(unsigned int nVolume)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Total_Volume smstv;
	smstv.iVolume = nVolume;
	pModuleManager->CallService(SERVICE_MIXER_SET_TOTAL_VOLUME,(param)&smstv);
	return smstv.bRet;
}

bool RoomAVLogic::GetMixerControlTotalVolume(unsigned int& nVolume)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Total_Volume smgtv;
	pModuleManager->CallService(SERVICE_MIXER_GET_TOTAL_VOLUME,(param)&smgtv);
	nVolume = smgtv.iVolume;
	return smgtv.bRet;
}

bool RoomAVLogic::SetMixerControlWavoutVolume(unsigned int nVolume)//设置伴音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Wavout_Volume smswv;
	smswv.iVolume = nVolume;
	pModuleManager->CallService(SERVICE_MIXER_SET_WAVOUT_VOLUME,(param)&smswv);
	return smswv.bRet;

}

bool RoomAVLogic::GetMixerControlWavoutVolume(unsigned int& nVolume)//获取伴音音量
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Wavout_Volume smgwv;
	pModuleManager->CallService(SERVICE_MIXER_GET_WAVOUT_VOLUME,(param)&smgwv);
	nVolume = smgwv.iVolume;
	return smgwv.bRet;
}

bool RoomAVLogic::SetMixerControlOutmicVolume(unsigned int nVolume)//设置音量控制中的麦克风
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Outmic_Volume smsov;
	smsov.iVolume = nVolume;
	pModuleManager->CallService(SERVICE_MIXER_SET_OUTMIC_VOLUME,(param)&smsov);
	return smsov.bRet;
}

bool RoomAVLogic::GetMixerControlOutmicVolume(unsigned int& nVolume)//获取音量控制中的麦克风音量
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Outmic_Volume smgov;
	pModuleManager->CallService(SERVICE_MIXER_GET_OUTMIC_VOLUME,(param)&smgov);
	nVolume = smgov.iVolume;
	return smgov.bRet;
}

//录音控制0~255
bool RoomAVLogic::SetMixerControlMixerVolume(unsigned int nVolume)//立体声混音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Mixer_Volume smsmv;
	smsmv.iVolume = nVolume;
	pModuleManager->CallService(SERVICE_MIXER_SET_MIXER_VOLUME,(param)&smsmv);
	return smsmv.bRet;
}

bool RoomAVLogic::GetMixerControlMixerVolume(unsigned int& nVolume)//获取立体声混音音量
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Mixer_Volume smsgmv;
	pModuleManager->CallService(SERVICE_MIXER_GET_MIXER_VOLUME,(param)&smsgmv);
	nVolume = smsgmv.iVolume;
	return smsgmv.bRet;
}

bool RoomAVLogic::SetMixerControlInmicVolume(unsigned int nVolume)//录音控制中的麦克风
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Inmic_Volume smsiv;
	smsiv.iVolume = nVolume;
	pModuleManager->CallService(SERVICE_MIXER_SET_INMIC_VOLUME,(param)&smsiv);
	return smsiv.bRet;
}

bool RoomAVLogic::GetMixerControlInmicVolume(unsigned int& nVolume)//获取录音控制中的麦克风音量
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Inmic_Volume smgiv;
	pModuleManager->CallService(SERVICE_MIXER_GET_INMIC_VOLUME,(param)&smgiv);
	nVolume = smgiv.iVolume;
	return smgiv.bRet;

}

bool RoomAVLogic::SetMixerControlWavInVolume(unsigned int nVolume)//设置音频输入设备音量
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_WavIn_Volume smsiv;
	smsiv.iVolume = nVolume;
	pModuleManager->CallService(SERVICE_MIXER_SET_WAVIN_VOLUME,(param)&smsiv);
	return smsiv.bRet;
}

bool RoomAVLogic::GetMixerControlWavInVolume(unsigned int& nVolume)//获取音频输入设备音量
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_WavIn_Volume smgiv;
	pModuleManager->CallService(SERVICE_MIXER_GET_WAVIN_VOLUME,(param)&smgiv);
	nVolume = smgiv.iVolume;
	return smgiv.bRet;
}

//静音设置
bool RoomAVLogic::SetMixerControlTotalMute(bool bEnable)    //主音量//设置总音量静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Total_Mute smstm;
	smstm.bEnable =bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_TOTAL_MUTE,(param)&smstm);
	return smstm.bRet;
}

bool RoomAVLogic::GetMixerControlTotalMute(bool& bEnable)//获取总音量是否静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Total_Mute smgtm;
	pModuleManager->CallService(SERVICE_MIXER_GET_TOTAL_MUTE,(param)&smgtm);
	bEnable = smgtm.bEnable;
	return smgtm.bRet;
}

bool RoomAVLogic::SetMixerControlWavoutMute(bool bEnable)   //波形//设置伴音静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Wavout_Mute smswm;
	smswm.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_WAVOUT_MUTE,(param)&smswm);
	return smswm.bRet;
}

bool RoomAVLogic::GetMixerControlWavoutMute(bool& bEnable)//获取伴音是否静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Wavout_Mute smgwm;
	pModuleManager->CallService(SERVICE_MIXER_GET_WAVOUT_MUTE,(param)&smgwm);
	bEnable = smgwm.bEnable;
	return smgwm.bRet;
}

bool RoomAVLogic::SetMixerControlOutmicMute(bool bEnable)  //输入监视器//设置音量控制中的麦克风的静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Outmic_Mute smsom;
	smsom.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_OUTMIC_MUTE,(param)&smsom);
	return smsom.bRet;
}

bool RoomAVLogic::GetMixerControlOutmicMute(bool& bEnable)//获取音量控制中的麦克风的静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Outmic_Mute smgom;
	pModuleManager->CallService(SERVICE_MIXER_GET_OUTMIC_MUTE,(param)&smgom);
	bEnable = smgom.bEnable;
	return smgom.bRet;
}

bool RoomAVLogic::SetMixerControlMixerMute(bool bEnable)   //后线路输入//立体声混音静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Mixer_Mute smsmu;
	smsmu.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_MIXER_MUTE,(param)&smsmu);
	return smsmu.bRet;
}

bool RoomAVLogic::GetMixerControlMixerMute(bool& bEnable)//获取立体声混应是否静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Mixer_Mute smgmu;
	pModuleManager->CallService(SERVICE_MIXER_GET_MIXER_MUTE,(param)&smgmu);
	bEnable = smgmu.bEnable;
	return smgmu.bRet;
}

bool RoomAVLogic::SetMixerControlInmicMute(bool bEnable)  //后麦克风//设置录音控制中的麦克风静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Inmic_Mute smsim;
	smsim.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_INMIC_MUTE,(param)&smsim);
	return smsim.bRet;
}

bool RoomAVLogic::GetMixerControlInmicMute(bool& bEnable)//获取录音控制中的麦克风是否静音
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Inmic_Mute smgim;
	pModuleManager->CallService(SERVICE_MIXER_GET_INMIC_MUTE,(param)&smgim);
	bEnable = smgim.bEnable;
	return smgim.bRet;
}

//初始化混音器
void RoomAVLogic::InitMixerControl(HWND hWnd)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Control_Init smci;
	smci.hWnd = hWnd;
	pModuleManager->CallService(SERVICE_MIXER_CONTROL_INIT,(param)&smci);
}

bool RoomAVLogic::SetMixerControlSingMode(bool bEnable)//设置唱歌模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Sing_Mode smssm;
	smssm.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_SING_MODE,(param)&smssm);
	return smssm.bRet;
}

bool RoomAVLogic::GetMixerControlSingMode(bool& bEnable)//获取是否唱歌模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Sing_Mode smgsm;
	pModuleManager->CallService(SERVICE_MIXER_GET_SING_MODE,(param)&smgsm);
	bEnable = smgsm.bEnable;
	return smgsm.bRet;
}

bool RoomAVLogic::SetMixerControlChatMode(bool bEnable)//设置与人聊天模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Chat_Mode smscm;
	smscm.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_CHAT_MODE,(param)&smscm);
	return smscm.bRet;
}

bool RoomAVLogic::GetMixerControlChatMode(bool& bEnable)//获取是否与人聊天模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Chat_Mode smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_CHAT_MODE,(param)&smgcm);
	bEnable = smgcm.bEnable;
	return smgcm.bRet;
}

bool RoomAVLogic::SetMixerControlPlayMode(bool bEnable)//设置在麦上放歌模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Set_Play_Mode smspm;
	smspm.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_MIXER_SET_PLAY_MODE,(param)&smspm);
	return smspm.bRet;
}

bool RoomAVLogic::GetMixerControlPlayMode(bool& bEnable)//获取是否在麦上唱歌模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Play_Mode smgpm;
	pModuleManager->CallService(SERVICE_MIXER_GET_PLAY_MODE,(param)&smgpm);
	bEnable = smgpm.bEnable;
	return smgpm.bRet;
}

bool RoomAVLogic::GetMixerControlMode(EnMixerControlMode& enMode)//获取系统当前模式
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Curr_Mode smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_CURR_MODE,(param)&smgcm);
	enMode = (EnMixerControlMode)smgcm.iMode;
	return smgcm.bRet;
}

bool RoomAVLogic::SetMixerControlMode(EnMixerControlMode enMode)//设置系统当前模式
{
	switch(enMode)
	{
	case MIXER_CONTROL_SING://唱歌模式
		return SetMixerControlSingMode(true);
		break;
	case MIXER_CONTROL_CHAT://与人聊天模式
		return SetMixerControlChatMode(true);
		break;
	case MIXER_CONTROL_PLAY://放歌模式
		return SetMixerControlPlayMode(true);
		break;
	case MIXER_CONTROL_OTHER://其他模式
		ErrTrace("RoomAVLogic::SetMixerControlMode MIXER_CONTROL_OTHER \n");
		break;
	default:
		ErrTrace("RoomAVLogic::SetMixerControlMode Err Mode \n");
	}
	return false;
}

bool RoomAVLogic::GetMixerControlTotalEnable()//获取当前总音量是否可用
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Total_Enable smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_TOTAL_ENABLE,(param)&smgcm);
	return smgcm.bEnable;
}

bool RoomAVLogic::GetMixerControlWavoutEnable()//获取伴音是否可用
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_WavOut_Enable smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_WAVOUT_ENABLE,(param)&smgcm);
	return smgcm.bEnable;
}

bool RoomAVLogic::GetMixerControlOutmicEnable()//获取音量控制中的麦克风是否可用
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_OutMic_Enable smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_OUTMIC_ENABLE,(param)&smgcm);
	return smgcm.bEnable;
}

bool RoomAVLogic::GetMixerControlMixerEnable()//获取立体声混音是否可用
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_Mixer_Enable smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_MIXER_ENABLE,(param)&smgcm);
	return smgcm.bEnable;
}

bool RoomAVLogic::GetMixerControlInmicEnable()//获取音频输入设备中音量大小
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_InMic_Enable smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_INMIC_ENABLE,(param)&smgcm);
	return smgcm.bEnable;
}

bool RoomAVLogic::GetMixerControlWavInEnable()//获取调整麦克风音量大小是否可用
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Mixer_Get_WavIn_Enable smgcm;
	pModuleManager->CallService(SERVICE_MIXER_GET_WAVIN_ENABLE,(param)&smgcm);
	return smgcm.bEnable;
}

bool RoomAVLogic::SetAudioMode(enuROOM_AV_AUDIO_MODE mode)//设置音频编码参数
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Av_Press_Audio_Cfg smgcm;
	smgcm.mode = mode;
	pModuleManager->CallService(SERVICE_AV_PRESS_AUDIO_CFG,(param)&smgcm);
	return smgcm.bret;
}


Enum_Camera_Status RoomAVLogic::GetCameraStatus(int iDevice)//获取摄像头状态
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Video_Get_Curr_Status svgcs;
	svgcs.iDevice = iDevice;
	svgcs.eStatus = kVideo_Success;
	pModuleManager->CallService(SERVICE_VIDEO_GET_CURR_STATUS,(param)&svgcs);
	return svgcs.eStatus;
}

void RoomAVLogic::SetEnableShapen(bool bEnable)
{
	IModuleManager* pModuleManager = m_module.GetModuleManager();
	assert(pModuleManager);
	Service_Video_Set_Shapen_Enable svsse;
	svsse.bEnable = bEnable;
	pModuleManager->CallService(SERVICE_VIDEO_SET_SHAP_ENABLE,(param)&svsse);
}
