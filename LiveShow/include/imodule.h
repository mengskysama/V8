#pragma  once

struct IModuleManager;

struct IModule
{
	virtual void  Load(IModuleManager *)            = 0;
	virtual void  Unload()					        = 0;
	virtual void  ProcessEvent(Event const&)	    = 0;
	virtual void  ProcessMessage(Message const&)    = 0;
	virtual int32 Invoke(ServiceValue const, param)	= 0;
	virtual void PaybackExtraInfo(uint32 value, void* p)
	{
		if(IsEventValue(value))
		{
			Event * pstEvent = static_cast<Event*>(p);
			if(pstEvent->m_pstExtraInfo)
				delete pstEvent->m_pstExtraInfo;
		}
		else if(IsMessageValue(value))
		{
			Message* pstMessage = static_cast<Message*>(p);
			if(pstMessage->m_pstExtraInfo)
				delete pstMessage->m_pstExtraInfo;
		}
	}
};

