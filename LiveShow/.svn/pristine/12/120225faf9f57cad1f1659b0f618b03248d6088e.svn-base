#include "StdAfx.h"
#include "LoudSpeakerManager.h"
#include "SpeakerMsg.h"
#include "..\GlobalRoomDefine.h"
#include "..\RoomParentDlg.h"
#include "..\personalsetting\PersonalConfig.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define MIN_SPEAKER_SHOW_TIME		1000 * 5
#define MAX_SPEAKER_SHOW_TIME		1000 * 60 * 5

CLoudSpeakerManager::CLoudSpeakerManager(CRoomParentDlg * pRoomParentDlg)
:	m_pRoomParentDlg(pRoomParentDlg),
	m_bHaveSpeaker(false),
	m_unCurSpeakerTime(0)
{
}

CLoudSpeakerManager::~CLoudSpeakerManager(void)
{
}

void CLoudSpeakerManager::OnCycleTrigger()
{
	if (m_bHaveSpeaker)
	{
		m_unCurSpeakerTime += 100;
		if (m_unCurSpeakerTime > MAX_SPEAKER_SHOW_TIME)
		{
			//É¾³ýµ±Ç°À®°È
			m_pRoomParentDlg->ClearSpeakerMsg();

			m_bHaveSpeaker = false;
		}
		if (m_unCurSpeakerTime > 5000 && m_listSpeakerInfo.size() != 0)
		{
			//É¾³ýµ±Ç°À®°È
			m_pRoomParentDlg->ClearSpeakerMsg();

			m_bHaveSpeaker = false;
		}
	}
	else
	{
		if (m_listSpeakerInfo.size() != 0)
		{
			TopSpeakerInfo stInfo = m_listSpeakerInfo.front();
			m_listSpeakerInfo.pop_front();

			ShowLoudSpeaker(stInfo.enmType, stInfo.strMessage);
			m_bHaveSpeaker = true;
			m_unCurSpeakerTime = 0;
		}
	}
}

void CLoudSpeakerManager::ProcessLoudSpeaker( core::ENM_LoudSpeakerType enmType, std::wstring strMessage )
{
	if (m_bHaveSpeaker)
	{
		TopSpeakerInfo stInfo;
		stInfo.enmType = enmType;
		stInfo.strMessage = strMessage;
		m_listSpeakerInfo.push_back(stInfo);

		return;
	}

	ShowLoudSpeaker(enmType, strMessage);
	m_bHaveSpeaker = true;
}

void CLoudSpeakerManager::ShowLoudSpeaker( core::ENM_LoudSpeakerType enmType, std::wstring strMessage )
{
	std::wstring strTemp = strMessage;
	core::MessageFont stFont;
	SpeakerInfo stInfo;
	if(CSpeakerMsg::DecodeMessage(strTemp, stFont, stInfo))
	{	
		//stInfo.unRichLevel = pInfo->unRichLevel;
		CString strSpeaker = strTemp.c_str();
		m_pRoomParentDlg->GetHtmlOutPutManager()->OnSpeakerMsg( enmType , strSpeaker, stFont, stInfo);
	}

	if ( enmType >= core::LOUDSPEAKERTYPE_BIG && !CPersonalConfig::GetInstance()->GetIsBlockSpeakerSound())
	{
		std::wstring strPath = common::utility::systemhelper::Get179AppPath();
		strPath += L"resource\\sound\\msg.wav";
		::PlaySound(strPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	}

	m_unCurSpeakerTime = 0;
}