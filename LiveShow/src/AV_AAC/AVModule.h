#pragma once
#include "51ktv.h"
#include "imodule.h"
#include "CircleBuffer.h"
#include "Audio\\mixer\\MixerManager.h"
#include "AVManager.h"

class AVModule :
	public IModule
{
public:
	AVModule(void){}
	~AVModule(void){}

	virtual void Load(IModuleManager *);
	virtual void Unload();
	virtual wchar_t const * const  GetModuleName();
	
	//模块外通知模块内一个事件，该事件处理为异步
	virtual void ProcessEvent(Event const&);	
	//不使用该接口通知消息，仅仅作为一个异步处理Event的方法。Model内------->ModelManager
	virtual void ProcessMessage(Message const&);
	//处理一个消息，以及可以返回消息的处理结果，为实时
	virtual int32 Invoke(ServiceValue const serviceValue, param);
	//提供个异步调用的发送事件的接口，先把事件存入BUF 然后统一调用m_pModuleManager发送   Model内------->ModelManager
	bool   sendEvent(void* pEvent, uint32 iType);
	
	IModuleManager*	GetModuleManager(){return m_pModuleManager;}

private:
	void   processCircleBuffer();

	IModuleManager* m_pModuleManager;
	CircleBuffer<void*>  m_CircleBuffer;

	AVManager	  m_avManager;
	CMixerManager m_mixerManager;
};
