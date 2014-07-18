#include "StdAfx.h"
#include "VideoOutputEngine.h"
#include "TimersManager.h"
#include "RC.h"
#include "AVEngine.h"
#include "./crc/Crc32.h"
#include "Global.h"
#define H264

Mutex VideoOutputEngine::m_Mutex_Static;//网络线程 房间线程 m_VOutputEngineMap 
std::map<UInt32, RCPtr<VideoOutputEngine> >   VideoOutputEngine::m_VOutputEngineMap;
bool VideoOutputEngine::m_bViewFFTALL = TRUE;
VideoOutputEngine::VideoOutputEngine(void)
{
	Trace("VideoOutputEngine::VideoOutputEngine()初始化 time = %I64d \n",GetTickCount());
	m_iLastActiveTime = 0;
	m_unTotalPacket = 0;
	m_unLostPacket = 0;
	fLostRate = 0.0f;
}

VideoOutputEngine::~VideoOutputEngine(void)
{
	Trace("VideoOutputEngine::~VideoOutputEngine()销毁 time = %I64d \n",GetTickCount());
	Destroy();
}

void  VideoOutputEngine::Destroy()
{
	Trace("VideoOutputEngine: 销毁VideoOutputEngine time = %I64d \n",GetTickCount());

	{
		//清除Partner的窗口资源
		std::map<UInt32, std::vector<stPartner> >::iterator iter;
		MutexLocker  locker(&m_Mutex); //房间线程 网络线程 m_PartnerWnds 
		for (iter=m_PartnerWnds.begin(); iter!=m_PartnerWnds.end(); ++iter)
		{
			for (UInt32 i=0; i<iter->second.size(); ++i)
			{
				delete iter->second[i].pDibSection;
				iter->second[i].pDibSection = NULL;
			}
			iter->second.clear();
		}
		m_PartnerWnds.clear();
		m_PartnerFFT.clear();
		m_PartnerFFTcoder.clear();

		//清除Partner的解码器
		std::map<UInt32, h264Codec*>::iterator iterDecoder;
		for (iterDecoder=m_PartnerDecoder.begin(); iterDecoder!=m_PartnerDecoder.end(); ++iterDecoder)
		{
			delete iterDecoder->second;
			iterDecoder->second = NULL;
		}
		m_PartnerDecoder.clear();
	}
}


void  VideoOutputEngine::AddPartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, SInt32 iQuality,UInt32 iVideoMode,enuVIDEO_TYPE enType)
{
	Trace("VideoOutputEngine::AddPartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	MutexLocker  locker(&m_Mutex);//网络线程 房间线程 m_VOutputEngineMap 
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	std::map<UInt32,RCPtr<CAnalyserProcess>>::iterator it;
	
	Trace("VideoEngine::AddPartnerHwnd 创建[DesUin = %d, width=%d, iHeight=%d, iQuality=%d]的解码器\n", iDesUin, iWidth, iHeight, iQuality);
	{
		//此时删除对应的解码器
		std::map<UInt32, h264Codec*>::iterator iterDecoder = m_PartnerDecoder.find(iDesUin);
		if (iterDecoder!=m_PartnerDecoder.end())
		{
			Trace("VideoEngine::DelPartnerHwnd() 删除[desuin = %d]的解码器 \n", iDesUin);
			delete iterDecoder->second;
			m_PartnerDecoder.erase(iterDecoder);
		}
	}

	iter = m_PartnerWnds.find(iDesUin);
	if (iter!=m_PartnerWnds.end())
	{
		std::vector<stPartner>::iterator iterDib;
		for (iterDib=iter->second.begin();iterDib!=iter->second.end();)
		{
			DIBSection* pDibSection = (*iterDib).pDibSection;
			delete pDibSection;
			iterDib = iter->second.erase(iterDib);
		}
		m_PartnerWnds.erase(iter);
	}

	DIBSection* pDibSection = new DIBSection;
	pDibSection->Create(hPartnerWnd,iWidth,iHeight,3);//pDibSection->Create(hPartnerWnd,H264_CODEC_WIDTH,H264_CODEC_HEIGHT,3);
	stPartner st;
	st.pDibSection = pDibSection;
	st.bPause = false;
	m_PartnerWnds[iDesUin].push_back(st);
	
	h264Codec* pDecoder = new h264Codec;
	pDecoder->Init(1, iWidth, iHeight, iQuality,enType,(enuROOM_AV_VIDEO_MODE)iVideoMode);
	m_PartnerDecoder[iDesUin] = pDecoder;

	it = m_PartnerFFTcoder.find(iDesUin);
	if(it==m_PartnerFFTcoder.end())//找不到FFT变换处理
	{
		RCPtr<CAnalyserProcess> pAnalyserProcess(new CAnalyserProcess(hPartnerWnd));
		m_PartnerFFTcoder[iDesUin] = pAnalyserProcess;
	}
}

bool  VideoOutputEngine::ChangePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd, UInt32 iWidth, UInt32 iHeight, HWND hOldWnd)
{
	Trace("VideoOutputEngine::ChangePartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	iter = m_PartnerWnds.find(iDesUin);
	if (iter==m_PartnerWnds.end()) return false;
	DIBSection* pDibSection = new DIBSection;
	pDibSection->Create(hPartnerWnd,iWidth,iHeight,3);
	stPartner st;
	st.pDibSection = pDibSection;
	st.bPause = false;
	m_PartnerWnds[iDesUin].push_back(st);
	
	DelPartnerHwnd(iDesUin,hOldWnd);

	m_PartnerDecoder[iDesUin]->DeChangeInit(iWidth,iHeight);
	return true;
}

void VideoOutputEngine::ClearPartnerHwnd(UInt32 iDesUin)
{
	Trace("VideoOutputEngine::ClearPartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	//删除对应句柄关系
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	iter = m_PartnerWnds.find(iDesUin);
	if (iter!=m_PartnerWnds.end())
	{
		std::vector<stPartner>::iterator iterDib;
		for (iterDib=iter->second.begin();iterDib!=iter->second.end();)
		{
			DIBSection* pDibSection = (*iterDib).pDibSection;
			delete pDibSection;
			iterDib = iter->second.erase(iterDib);
		}
		m_PartnerWnds.erase(iter);
	}
	
	//此时删除对应的解码器
	std::map<UInt32, h264Codec*>::iterator iterDecoder = m_PartnerDecoder.find(iDesUin);
	if (iterDecoder!=m_PartnerDecoder.end())
	{
		Trace("VideoEngine::DelPartnerHwnd() 删除[desuin = %d]的解码器 \n", iDesUin);
		delete iterDecoder->second;
		m_PartnerDecoder.erase(iterDecoder);
	}

	//删除对应的FFT资源
	if (m_PartnerFFTcoder.find(iDesUin)!=m_PartnerFFTcoder.end())
	{
		m_PartnerFFTcoder.erase(m_PartnerFFTcoder.find(iDesUin));
	}
}

void VideoOutputEngine::ClearPartnerHwndRoom()
{
	Trace("VideoOutputEngine::ClearPartnerHwndRoom() time = %I64d \n",GetTickCount());
	Destroy();
}

bool VideoOutputEngine::PaintRemote(UInt32 iFromUin,HWND hWnd)
{
	Trace("VideoOutputEngine::PaintRemote() iFromUin=%d time = %I64d \n",iFromUin,GetTickCount());
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	std::map<UInt32,BOOL>::iterator it;
	std::map<UInt32,RCPtr<CAnalyserProcess>>::iterator iterFFT;
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	iter = m_PartnerWnds.find(iFromUin);
	it = m_PartnerFFT.find(iFromUin);
	iterFFT = m_PartnerFFTcoder.find(iFromUin);
	if (iter!=m_PartnerWnds.end())
	{
		for (UInt32 i=0; i<iter->second.size(); ++i)
		{
			DIBSection* pDibSection = iter->second[i].pDibSection;
			if (hWnd==pDibSection->GetHwnd() && !iter->second[i].bPause)
			{
				if (!pDibSection->IsPhotoable()) return false;

				HDC hdc = ::GetWindowDC(pDibSection->GetHwnd());
				pDibSection->Paint(hdc, 0, 0);
				if (it==m_PartnerFFT.end() || it->second) //不存在 或为TRUE 执行操作
				{
					if (iterFFT!=m_PartnerFFTcoder.end()) //存在
						iterFFT->second->Draw(pDibSection->GetHwnd(),hdc);
				}
				::ReleaseDC(pDibSection->GetHwnd(), hdc);
				return true;
			}
		}
	}

	return false;
}

void  VideoOutputEngine::DelPartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd)
{
	Trace("VideoOutputEngine::DelPartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 

	bool  bParnterEmpty = true;
	std::vector<stPartner>::iterator iterTmp;
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	iter = m_PartnerWnds.find(iDesUin);
	if (iter!=m_PartnerWnds.end())
	{
		for(iterTmp=iter->second.begin();iterTmp!=iter->second.end();)
		{
			DIBSection* pDibSection = (*iterTmp).pDibSection;
			if (hPartnerWnd==pDibSection->GetHwnd())
			{
				iterTmp = iter->second.erase(iterTmp);
				delete pDibSection;
				break;
			}
			else
			{
				iterTmp++;
			}
		}

		if (iter->second.empty())
		{
			m_PartnerWnds.erase(iter);
		}
		else
		{
			bParnterEmpty = false;
		}
	}

	//此时删除对应的解码器
	if(bParnterEmpty )
	{
		std::map<UInt32, h264Codec*>::iterator iterDecoder = m_PartnerDecoder.find(iDesUin);
		if (iterDecoder!=m_PartnerDecoder.end())
		{
			Trace("VideoEngine::DelPartnerHwnd() 删除[desuin = %d]的解码器 \n", iDesUin);
			delete iterDecoder->second;
			m_PartnerDecoder.erase(iterDecoder);
		}

		if (m_PartnerFFTcoder.find(iDesUin)!=m_PartnerFFTcoder.end())
		{
			m_PartnerFFTcoder.erase(m_PartnerFFTcoder.find(iDesUin));
		}
	}
}

void  VideoOutputEngine::PausePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd)
{
	Trace("VideoOutputEngine::PausePartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	iter = m_PartnerWnds.find(iDesUin);
	if (iter!=m_PartnerWnds.end())
	{
		//找到对应的HWnd, 并取出
		for (UInt32 i=0; i<iter->second.size(); ++i)
		{
			if (hPartnerWnd==iter->second[i].pDibSection->GetHwnd())
			{
				iter->second[i].bPause = true;
				break;
			}
		}

		if (iter->second.empty())
		{
			m_PartnerWnds.erase(iter);
		}
	}
}

void  VideoOutputEngine::ContinuePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd)
{
	Trace("VideoOutputEngine::ContinuePartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	DIBSection* pDibSection = NULL;
	std::map<UInt32, std::vector<stPartner> >::iterator iterPause;
	iterPause = m_PartnerWnds.find(iDesUin);
	if (iterPause!=m_PartnerWnds.end())
	{
		//找到对应的HWnd
		for (UInt32 i=0; i<iterPause->second.size(); ++i)
		{
			pDibSection = iterPause->second[i].pDibSection;
			if (hPartnerWnd==pDibSection->GetHwnd())
			{
				iterPause->second[i].bPause = false;
				break;
			}
		}

		if (iterPause->second.empty())
		{
			m_PartnerWnds.erase(iterPause);
		}
	}
}


bool  VideoOutputEngine::IsPhotoablePartnerHwnd(UInt32 iDesUin, HWND hPartnerWnd)
{
	Trace("VideoOutputEngine::IsPhotoablePartnerHwnd() iDesUin=%d time = %I64d \n",iDesUin,GetTickCount());
	std::map<UInt32, std::vector<stPartner> >::iterator iter;
	MutexLocker  locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	iter = m_PartnerWnds.find(iDesUin);
	if (iter!=m_PartnerWnds.end())
	{
		for (UInt32 i=0; i<iter->second.size(); ++i)
		{
			if (hPartnerWnd==iter->second[i].pDibSection->GetHwnd() && iter->second[i].pDibSection->IsPhotoable())
			{
				//已经拥有该HWND，直接返回
				return true;
			}
		}
	}

	return false;
}

bool VideoOutputEngine::GetShowFFTWav(UInt32 iFromUin)
{
	Trace("VideoOutputEngine::GetShowFFTWav() iFromUin=%d time = %I64d \n",iFromUin,GetTickCount());
	std::map<UInt32,BOOL>::iterator iter = m_PartnerFFT.find(iFromUin);
	if (iter!=m_PartnerFFT.end())
	{
		return iter->second;
	}
	return TRUE;
}

void VideoOutputEngine::SetShowFFTWav(UInt32 iFromUin,bool bEnable)
{
	MutexLocker  locker(&m_Mutex);
	Trace("VideoOutputEngine::SetShowFFTWav() iFromUin=%d time = %I64d \n",iFromUin,GetTickCount());
	m_PartnerFFT[iFromUin] = bEnable;
}

void VideoOutputEngine::PrepareWav(UInt32 iFromUin,byte* pData,UInt32 iLen)
{
	if (!GetShowFFTWav(iFromUin))
	{
		return;
	}
	MutexLocker  locker(&m_Mutex);
	std::map<UInt32,RCPtr<CAnalyserProcess>>::iterator iter = m_PartnerFFTcoder.find(iFromUin);
	if (iter!=m_PartnerFFTcoder.end())
	{
		iter->second->Prepare(pData,iLen);
	}
	else
	{
		Assert(0);
	}
}

void  VideoOutputEngine::DecodeVideoData(UInt32 iDesUin, UInt8* pData, UInt32 iDataLen, UInt8 cKey, UInt32 iSeqNum, UInt32 iKeySeqNum)
{
	Trace("VideoOutputEngine::DecodeVideoData cKey= %d, iSeqNum=%d, time = %I64d \n", (unsigned int)(cKey & KEY_VALUE), iSeqNum,GetTickCount());
	MutexLocker locker(&m_Mutex);//房间线程 网络线程 m_PartnerWnds 
	static unsigned int s_VideoPacketCount = 0;
	static unsigned int s_VideoBadCount = 0;
	s_VideoPacketCount++;
	/*
	CRC 校验代码   cygnus
	*/
#ifndef IGNOR_CRC_CHECK
	DWORD dwCrc = *(DWORD*)(pData+iDataLen-sizeof(DWORD));
	static ClsCrc32 s_Crc32;
	if (s_Crc32.Crc(pData,iDataLen-sizeof(DWORD)) != dwCrc)
	{
		s_VideoBadCount++;
		Trace("数据包-CRC校验失败，OldCrc=%d,NewCrc=%d,iDesUin=%d,s_VideoPacketCount=%d,s_VideoBadCount=%d,pecent=%f time = %I64d \n",dwCrc,s_Crc32.CrcGet(),iDesUin,s_VideoPacketCount,s_VideoBadCount,(float)s_VideoBadCount/s_VideoPacketCount,GetTickCount());

		__asm
		{
			Assert(0);
#ifndef IGNOR_INT_3
			_asm int 3;
#endif
		}
		return ;
	}	
	iDataLen = iDataLen - sizeof(DWORD);
#endif
	
	std::map<UInt32, h264Codec*>::iterator iterDecoder = m_PartnerDecoder.find(iDesUin);

	if (iterDecoder == m_PartnerDecoder.end())
	{
		ErrTrace("收到无效用户数据包iDesUin=%d\n",iDesUin);
		return;
	}

	h264Codec* pDecoder = iterDecoder->second;
	if (!pDecoder)
	{
		ErrTrace("VideoOutputEngine::DecodeVideoData.....pDecoder=NULL.....\n");
		return;
	}

	if (pDecoder->GetUsedType() != (cKey>>4))
	{
		ErrTrace("VideoOutputEngine::DecodeVideoData.....收到无效类型数据包 Type=%d.  UsedType=%d....\n",(cKey>>4), pDecoder->GetUsedType());
		return;
	}

	UInt64 iTickTime = ::GetTickCount();

	if (iTickTime - m_iLastActiveTime > VIDEO_LOST_PACKET_TIME_INTERVAL)
	{
		m_iLastActiveTime = iTickTime;
		if (m_unTotalPacket + m_unLostPacket == 0)
		{
			fLostRate = 0.0f;
		}
		else
		{
			fLostRate = (float)m_unLostPacket / (m_unTotalPacket + m_unLostPacket);
		}
	}

	if (iSeqNum == pDecoder->m_iCurSeqNum+1)
	{
		++m_unTotalPacket;
	}
	else
	{
		m_unLostPacket += (iSeqNum - pDecoder->m_iCurSeqNum-1)<10?(iSeqNum - pDecoder->m_iCurSeqNum-1):1;
	}

	if (pDecoder->m_iCurSeqNum==0)
	{
		pDecoder->m_iCurSeqNum = iSeqNum-1;
	}

	if (iSeqNum - pDecoder->m_iCurSeqNum>5)
	{
		pDecoder->m_iCurSeqNum = iSeqNum-1;
		pDecoder->SetWaittingForKeyFrame(true);
	}

	if (iSeqNum <= pDecoder->m_iCurSeqNum)
	{
		CocTrace("plushub 收到过期帧，丢弃 %d -> %d time = %I64d \n", pDecoder->m_iCurSeqNum, iSeqNum,GetTickCount());
		pDecoder->m_iCurSeqNum = iSeqNum-1;
		pDecoder->SetWaittingForKeyFrame(true);
	}


	/////////数据过滤 转换为不同大小的尺寸


	if ((cKey & KEY_VALUE)==KEY_VALUE)
	{
		if (iSeqNum != pDecoder->m_iCurSeqNum+1)
		{	
			//CocTrace("plushub 当前关键帧不连续 %d -> %d time = %I64d\n", pDecoder->m_iCurSeqNum, iSeqNum,GetTickCount());
		}

		pDecoder->m_iCurKeySeqNum = iKeySeqNum;
		pDecoder->m_iCurSeqNum = iSeqNum;

		pDecoder->SetWaittingForKeyFrame(false);
	}
	else
	{
		if(pDecoder->IsWaittingForKeyFrame())
		{
			//CocTrace("plushub 正在等待关键帧，当前非关键帧丢弃 %d -> %d time = %I64d\n", pDecoder->m_iCurSeqNum, iSeqNum,GetTickCount());
			pDecoder->m_iCurSeqNum = iSeqNum;
			return;
		}
		else
		{
			if (iSeqNum != pDecoder->m_iCurSeqNum+1 && iSeqNum - pDecoder->m_iCurSeqNum > (cKey & KEY_VALUE))   //修复p帧是否已经丢失  出现马赛克问题  ----- 2013-5-28日 修改
			{
				CocTrace("当前非关键帧不连续，丢失P帧 cKey=%d, %d -> %d time = %I64d\n", (cKey & KEY_VALUE),pDecoder->m_iCurSeqNum, iSeqNum,GetTickCount());
				pDecoder->m_iCurSeqNum = iSeqNum;
				pDecoder->SetWaittingForKeyFrame(true);
				return;
			}

			pDecoder->m_iCurSeqNum = iSeqNum;
		}		
	}

	BAD_MEM_READ(pData,iDataLen,"pData .....")

	int iOutLen = 0;
	bool bForceKey = false;

	try
	{
		Trace("VideoOutputEngine::DecodeVideoData...00\n");
		std::map<UInt32, std::vector<stPartner> >::iterator iter = m_PartnerWnds.begin();

		m_dibRectCalc.Initialize();
		if (iter!=m_PartnerWnds.end())
		{
			for (UInt32 i=0; i<iter->second.size(); ++i)
			{
				DIBSection* pDibSection = iter->second[i].pDibSection;
				m_dibRectCalc.AddHWnd(pDibSection->GetHwnd());
			}
		}

		//按照实际大小放大图片
		CRect rcWindow = m_dibRectCalc.GetRectInfo();
		AssertVS(rcWindow.Width()>=200 && rcWindow.Height()>=150,iDesUin,"VideoOutputEngine::DecodeVideoData");
		if (rcWindow.Width()<224 || rcWindow.Height()<168){
			ErrTrace("rcWindow.Width()=%d,rcWindow.Height()=%d\n",rcWindow.Width(),rcWindow.Height());
		}
		pDecoder->DeChangeInit(rcWindow.Width(),rcWindow.Height());

		Trace("VideoOutputEngine::DecodeVideoData...01\n");

		BAD_MEM_WRITE(pDecoder->GetBuffer(),rcWindow.Width()*rcWindow.Height()*3,"VideoOutputEngine::DecodeVideoData...pDecoder->GetBuffer()")
		
		int err = 0;
		if(pDecoder->DecodeVideoData(pData, iDataLen, pDecoder->GetBuffer(), iOutLen,err, bForceKey))
		{
			Trace("VideoOutputEngine::DecodeVideoData...11\n");
			std::map<UInt32,BOOL>::iterator it;
			std::map<UInt32,RCPtr<CAnalyserProcess>>::iterator iterFFT;
			iter = m_PartnerWnds.find(iDesUin);
			it = m_PartnerFFT.find(iDesUin);
			iterFFT = m_PartnerFFTcoder.find(iDesUin);
			if (it==m_PartnerFFT.end() || it->second) //不存在 或为TRUE 执行操作
			{
				if (iterFFT!=m_PartnerFFTcoder.end()) //存在
					iterFFT->second->Process();
			}
			
			Trace("VideoOutputEngine::DecodeVideoData...22\n");

			if (iter!=m_PartnerWnds.end())
			{
				for (UInt32 i=0; i<iter->second.size(); ++i)
				{
					if (iter->second[i].bPause) continue;
					DIBSection* pDibSection = iter->second[i].pDibSection;
					pDibSection->SetImage(pDecoder->GetBuffer(), pDecoder->GetWidth(), pDecoder->GetHeight(), 3);
					HDC hdc = ::GetWindowDC(pDibSection->GetHwnd());
					pDibSection->Paint(hdc, 0, 0);
					if (it==m_PartnerFFT.end() || it->second) //不存在 或为TRUE 执行操作
					{
						if (iterFFT!=m_PartnerFFTcoder.end()) //存在
							iterFFT->second->Draw(pDibSection->GetHwnd(),hdc);
					}
					::ReleaseDC(pDibSection->GetHwnd(), hdc);
				}
			}

			Trace("VideoOutputEngine::DecodeVideoData...33\n");
		}
		else
		{
			CocTrace("plushub 解码失败 %d -> %d time = %I64d\n", pDecoder->m_iCurSeqNum, iSeqNum,GetTickCount());
			if (err==1) pDecoder->SetWaittingForKeyFrame(true);
		}
	}
	catch(...)
	{
		Assert(0);
		__asm
		{
			int 3;
		}
	}
}

RCPtr<VideoOutputEngine> VideoOutputEngine::Create(SInt32 iRoomId)
{
	Trace("VideoOutputEngine::Create() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	std::map<UInt32, RCPtr<VideoOutputEngine> >::iterator iter;
	MutexLocker locker(&m_Mutex_Static); //网络线程 房间线程 m_VOutputEngineMap 
	if (m_VOutputEngineMap.find(iRoomId)!=m_VOutputEngineMap.end())
	{
		iter = m_VOutputEngineMap.find(iRoomId);
		return iter->second;
	}
	else
	{
		RCPtr<VideoOutputEngine> pVInput(new VideoOutputEngine());
		m_VOutputEngineMap.insert(voutput_pair(iRoomId, pVInput));
		return pVInput;

	}
}

void VideoOutputEngine::RemoveOutputEngine(SInt32 iRoomId)
{
	Trace("RemoveOutputEngine::Create() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_Mutex_Static); //网络线程 房间线程 m_VOutputEngineMap 
	if (m_VOutputEngineMap.find(iRoomId)!=m_VOutputEngineMap.end())
	{
		m_VOutputEngineMap.erase(m_VOutputEngineMap.find(iRoomId));
	}
}

RCPtr<VideoOutputEngine>  VideoOutputEngine::FindOutputEngine(SInt32 iRoomId)
{
	//Trace("RemoveOutputEngine::FindOutputEngine() iRoomId=%d time = %I64d \n",iRoomId,GetTickCount());
	MutexLocker locker(&m_Mutex_Static); //网络线程 房间线程 m_VOutputEngineMap 
	RCPtr<VideoOutputEngine> pVInput(NULL);
	std::map<UInt32, RCPtr<VideoOutputEngine> >::iterator iter = m_VOutputEngineMap.find(iRoomId);
	if (iter!=m_VOutputEngineMap.end())
	{
		return iter->second;
	}
	return pVInput;
}

void VideoOutputEngine::RemoveAllOutputEngine()
{
	Trace("RemoveOutputEngine::RemoveAllOutputEngine()  time = %I64d \n",GetTickCount());
	MutexLocker locker(&m_Mutex_Static); //网络线程 房间线程 m_VOutputEngineMap 
	m_VOutputEngineMap.clear();
}

float VideoOutputEngine::GetLostPacketRate()
{
	return this->fLostRate;
}

UInt32 VideoOutputEngine::GetTotalPackets()
{
	return this->m_unTotalPacket;
}