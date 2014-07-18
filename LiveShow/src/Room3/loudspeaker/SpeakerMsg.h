#pragma once
#include "coredefine.h"
#include "..\usermanager\useritem.h"
#include "..\GlobalRoomDefine.h"

class CSpeakerMsg
{
public:
	CSpeakerMsg(void);
	~CSpeakerMsg(void);

	static bool EncodeMessage(std::wstring & strMessage, core::MessageFont stFont, IUserItem * pUserItem, 
		std::wstring& strRoomName , uint32 unRoomID , bool bWish = false);
	static bool DecodeMessage(std::wstring & strMessage, core::MessageFont & stFont, SpeakerInfo & stInfo);
	static CString GetRoomShortCut(uint32 unRoomID, std::wstring strRoomName);
	static CString GetRoomShortCutFromHtml( uint32 unRoomID, std::wstring strRoomName ) ;
	static CString GetSpeakerShortCut(core::ENM_LoudSpeakerType enmType, SpeakerInfo stInfo);
	static void DisableRoomShortcut(CString & strMessage);
};
