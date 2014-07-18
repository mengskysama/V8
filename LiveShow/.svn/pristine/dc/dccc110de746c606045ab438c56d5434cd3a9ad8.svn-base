#include "StdAfx.h"
#include "SofaChatFlash.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"

CSofaChatFlash::CSofaChatFlash(void)
{
}

CSofaChatFlash::~CSofaChatFlash(void)
{
}

void CSofaChatFlash::OnSofaChat( CString strChatParam, bool bAll/* = false*/ )
{
	std::vector<CString> vecParams;
	vecParams.push_back(strChatParam);
	if (bAll)
	{
		vecParams.push_back(L"1");
	}
	else
	{
		vecParams.push_back(L"0");
	}
	CallFunction(L"sendSpeaker", vecParams);
}

void CSofaChatFlash::OnSofaKick( uint8 unIndex )
{
	std::vector<CString> vecParams;
	vecParams.push_back(common::utility::stringhelper::IntToString(unIndex).c_str());
	CallFunction(L"kickUser", vecParams);
}

void CSofaChatFlash::UpdateSize( int cx )
{
	std::vector<CString> vecParams;
	vecParams.push_back(common::utility::stringhelper::IntToString(cx).c_str());
	CallFunction(L"moveXY", vecParams);
}

void CSofaChatFlash::HideChatDlg( uint8 unIndex, bool bAll /*= false*/ )
{
	std::vector<CString> vecParams;
	vecParams.push_back(common::utility::stringhelper::IntToString(unIndex).c_str());
	if (bAll)
	{
		vecParams.push_back(L"1");
	}
	else
	{
		vecParams.push_back(L"0");
	}
	CallFunction(L"hideDialoge", vecParams);
}