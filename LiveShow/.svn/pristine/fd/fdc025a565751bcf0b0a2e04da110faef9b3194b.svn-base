#pragma once
#include "coredefine.h"

class CRoomParentDlg;
struct TopSpeakerInfo
{
	core::ENM_LoudSpeakerType enmType;		//¿Æ∞»¿‡–Õ£¨–°¿Æ∞»°¢¥Û¿Æ∞»°¢≥¨º∂¿Æ∞»
	std::wstring strMessage;				//¿Æ∞»ƒ⁄»›
};

class CLoudSpeakerManager
{
public:
	CLoudSpeakerManager(CRoomParentDlg * pRoomParentDlg);
	~CLoudSpeakerManager(void);

	void ProcessLoudSpeaker(core::ENM_LoudSpeakerType enmType, std::wstring strMessage);

	void OnCycleTrigger();

private:
	void ShowLoudSpeaker(core::ENM_LoudSpeakerType enmType, std::wstring strMessage);

private:
	std::list<TopSpeakerInfo> m_listSpeakerInfo;
	bool m_bHaveSpeaker;
	uint32 m_unCurSpeakerTime;
	CRoomParentDlg * m_pRoomParentDlg;
};
