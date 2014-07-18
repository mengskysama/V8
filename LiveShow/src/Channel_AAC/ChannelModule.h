#pragma once
#include "Channel.h"
#include "51ktv.h"

class ChannelModule : public IModule
{
public:
	ChannelModule() : m_pModuleManager(NULL){}
	~ChannelModule(){}

	virtual void Load(IModuleManager *);
	virtual void Unload();
	virtual wchar_t const * const GetModuleName();
	virtual void ProcessEvent(Event const&);
	virtual void ProcessMessage(Message const&);
	virtual int32 Invoke(param const, param lparam);
	virtual void PaybackExtraInfo(uint32, void*);
	
	IModuleManager* GetModuleManager()
	{
		return m_pModuleManager;
	}
              

private:
	IModuleManager*        m_pModuleManager;
};




