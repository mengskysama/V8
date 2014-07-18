#include "stdafx.h"
#include "ChannelModule.h"
#include "RelayManger.h"

const wchar_t* const g_ModuleName = L"ChannelModule";
ChannelModule        g_ChannelModule;
ModuleId             g_ChanneModuleID = MODULE_ID_RELAY;

namespace ChannelSpace
{
	IModule * __stdcall GetModule()
	{
		return &g_ChannelModule;
	}
}

void  ChannelModule::Load(IModuleManager * p)
{
	m_pModuleManager = p;

	//RelayManger::Initialize(p);
}

void  ChannelModule::Unload()
{
	m_pModuleManager=NULL;
	//RelayManger::DestroyInstance();
}

const wchar_t* const  ChannelModule::GetModuleName()
{
	return g_ModuleName;
}

void  ChannelModule::ProcessEvent(const Event& const inEventvalue)
{
	return ;
}

void  ChannelModule::ProcessMessage(const Message& const inMsg)
{

}

int32 ChannelModule::Invoke(param const, param lparam)
{
	return 0;
}

void ChannelModule::PaybackExtraInfo(uint32 value, void* p)
{
	{
		if(IsEventValue(value)) 
		{
			Event* pstEvent=static_cast<Event*>(p);
			if(pstEvent->m_pstExtraInfo)
				delete pstEvent->m_pstExtraInfo;
		}
		else if(IsMessageValue(value))
		{
			Message* pstMessage=static_cast<Message*>(p);
			if(pstMessage->m_pstExtraInfo)
				delete pstMessage->m_pstExtraInfo;
		}
	};
}


