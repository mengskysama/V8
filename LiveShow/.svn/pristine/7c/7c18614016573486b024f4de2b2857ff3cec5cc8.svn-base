#pragma once

#include "51ktv.h"

#include "imodulemanager.h"
#include "log.h"
#include <boost/circular_buffer.hpp>
#include <boost/bimap.hpp>
#include "../CommonLibrary/include/utility/Lock.h"
#include "../ErrorReport/include/crashreport.h"

enum
{
 	MAXER = (sizeof(Event)>sizeof(Message)?sizeof(Event):sizeof(Message))
};

EXTERN_LOG()

struct BufUnit
{
	BufUnit(){}
	BufUnit(Event const & r)
	{
		Event & event = *reinterpret_cast<Event*>(buf);
		event = r;
	}
	BufUnit(Message const & r)
	{
		Message & message = *reinterpret_cast<Message*>(buf);
		message = r;
	}
	char buf[MAXER];
};

class ModuleManagerImpl : IModuleManager
{
public:
	ModuleManagerImpl();
	~ModuleManagerImpl();

	virtual bool  PushEvent(Event const&);
	virtual bool  PushMessage(Message const&);
	virtual bool  PushAsynEvent(Event const&);
	virtual bool  PushAsynMessage(Message const&);
	virtual int32 CallService(ServiceValue const, param);

	void OnCycleTrigger();
	void Construct();
	void Destruct();
	void run();

private:

	bool InnerRun(MSG & msg);
	void ModuleManagerProcess();

	void Init();
	void destroy();
	void DestroyCycleWnd();
	void CreatCycleWnd();

	bool ProcessPreLogoutInner(Message & message,ModuleId id,IModule*  pIModule,BOOL & bAllSuc);
	void ProcessPreLogout(Message & message);
	
	void Exit();
	void InnerExchange();

private:

	boost::circular_buffer<BufUnit> m_eventMsgBuf;
	HWND s_hMsgWindow;
	bool m_run;
	uint32 m_exitCounter;

private:

	LPVOID m_CrashRpt;
	DumpNodeBuffer m_DumpNodeSysCmd;
	DumpNodeBuffer m_DumpNodeNetCmd;

	void StatNetCmd(int netCmd);
	void StatSysCmd(int sysCmd);
	void DumpStamp(DumpNodeBuffer& dumpNodeSysCmd, std::wstring & out);
	void DumpLog();

private:

	common::utility::CSWrapper lock_m;

	typedef std::vector<BufUnit> BufUnitVec;

	BufUnitVec m_outVec;
	BufUnitVec m_inVec;

private:

	typedef IModule*  (*GetModuleFunc)();
	typedef void  (*ReleaseModuleFunc)();

	struct ModuleInterface
	{
		ModuleInterface():m_GetModuleFunc(NULL),m_ReleaseModuleFunc(NULL),m_IModule(NULL){}
		ModuleInterface(GetModuleFunc p1, ReleaseModuleFunc p2)
			:m_GetModuleFunc(p1),
			m_ReleaseModuleFunc(p2),m_IModule(NULL)
		{}
		GetModuleFunc m_GetModuleFunc;
		ReleaseModuleFunc m_ReleaseModuleFunc;
		std::wstring m_strModule;
		IModule* m_IModule;
	};

	typedef std::map<ModuleId, ModuleInterface> ModuleInterfaceC;
	ModuleInterfaceC m_ModuleInterfaceC;
};