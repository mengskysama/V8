#include "stdafx.h"
#include "ModuleManager.h"
#include <sstream>
#include <boost/format.hpp>
#include "utility\timers.h"
#include "initlist.h"
#include "utility\SystemHelper.h"
#include "..\ErrorReport\include\crashrpt.h"

static InitList g_stInitList;
INSTANCE_LOG(L"ModuleManager")

#define MINI_MEM_SIZE	8*1024*1024
#define MEDIUM_MEM_SIZE 12*1024*1024

#define DUMPNODE_SYSCMD_MAXSIZE	1000
#define DUMPNODE_NETCMD_MAXSIZE	500

DumpNodeBuffer m_DumpNodeSysCmd;
DumpNodeBuffer m_DumpNodeNetCmd;

#define EXIST_MODULE(Y,X) ((Y=m_ModuleInterfaceC.find((ModuleId)(X)))!= m_ModuleInterfaceC.end())

ModuleManagerImpl::ModuleManagerImpl():
m_eventMsgBuf(10240),
m_run(false),
m_exitCounter(0),
m_CrashRpt(NULL)
{	
}

void ModuleManagerImpl::Construct()
{
	m_CrashRpt = InstallCrashRpt();
	SetDumpVecAddr(&m_DumpNodeSysCmd, &m_DumpNodeNetCmd);

	Init();
	OleInitialize(NULL);
}

void ModuleManagerImpl::DumpStamp(DumpNodeBuffer& dumpNodeSysCmd, std::wstring & out)
{
	std::wstringstream buf;
	
	for(DumpNodeBuffer::iterator it = dumpNodeSysCmd.begin();
		it != dumpNodeSysCmd.end(); ++it)
	{
		DumpNode& node = *it;

		CString strTime;
		strTime.Format(L"%2d:%2d:%3d", node.m_wMinute, node.m_wSecond, node.m_wMilliseconds);

		buf<<L"["<<strTime.GetBuffer()<<L","<<std::hex<<node.m_nValue<<L"]\r\n";
	}

	out = buf.str();
}

void ModuleManagerImpl::DumpLog()
{
	std::wstring busCmd;
	std::wstring netCmd;
	DumpStamp(m_DumpNodeSysCmd,busCmd);
	DumpStamp(m_DumpNodeNetCmd,netCmd);

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString name;
	name.Format(L"dump%4d-%02d-%02d-%02d-%02d-%02d.log", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	std::wstring filePath = common::utility::systemhelper::Get179AppPath();
	filePath += L"log\\";
	filePath += name.GetBuffer();

	FILE* fp = _wfopen(filePath.c_str(), L"w");
	if(fp != NULL)
	{
		fwprintf(fp, L"%s\r\n=========================\r\n%s", busCmd.c_str(), netCmd.c_str());
		fclose(fp);
	}
}

ModuleManagerImpl::~ModuleManagerImpl()
{
	DumpLog();
}

void ModuleManagerImpl::Destruct()
{
	UninstallCrashRpt(m_CrashRpt);

	destroy();
	OleUninitialize();
}

void ModuleManagerImpl::Init()
{
	//load every module and get the IModule pointer
	
	for(InitList::DllModuleList::iterator it = g_stInitList.m_stDllModuleList.begin();
		it != g_stInitList.m_stDllModuleList.end(); ++it)
	{
		HMODULE exportmodule;
		
		exportmodule = LoadLibrary(it->first.c_str());
		DWORD dwStatus = GetLastError();
		assert(exportmodule != NULL);
		if(exportmodule == NULL)
		{
			std::wstring temp = it->first;
			temp.append(L"  装载dll失败");
			AfxMessageBox(temp.c_str());
			return;
		}
		
		m_ModuleInterfaceC[it->second] =
			ModuleInterface(reinterpret_cast<GetModuleFunc>(GetProcAddress(exportmodule, "GetModule")),
									reinterpret_cast<ReleaseModuleFunc>(GetProcAddress(exportmodule, "ReleaseModule")));

		if(m_ModuleInterfaceC[it->second].m_GetModuleFunc == NULL)
		{
			std::wstring temp = it->first;
			temp.append(L"  GetModule失败");
			AfxMessageBox(temp.c_str());
			return;
		}
		else
		{
			IModule* pIModule = m_ModuleInterfaceC[it->second].m_GetModuleFunc();
			if(pIModule == NULL)
			{
				assert(0);
				return;
			}
			
			m_ModuleInterfaceC[it->second].m_strModule = it->first;
			m_ModuleInterfaceC[it->second].m_IModule = pIModule;
		}
	}	

	//let every module has a chance to Init itself
	for(ModuleInterfaceC::iterator it=m_ModuleInterfaceC.begin();it!=m_ModuleInterfaceC.end();++it)
	{
		it->second.m_IModule->Load(this);
	}

	Message msg(MESSAGE_VALUE_SYS_ALL_MODULE_LOADED, MODULE_ID_CORE);
	PushMessage(msg);

	/*在Vista下，允许低等级的进程向本进程发送WM_COPYDATA消息*/
#define MSGFLT_ADD		1
#define MSGFLT_REMOVE	2

	HMODULE hUserModule = LoadLibrary(L"user32.dll");
	if( NULL == hUserModule)
		return;

	typedef BOOL (WINAPI *PChangeWindowMessageFilter)(UINT, DWORD);
	PChangeWindowMessageFilter pMsgFilter = (PChangeWindowMessageFilter)GetProcAddress(
		hUserModule, "ChangeWindowMessageFilter");

	if( NULL == pMsgFilter)
		return;

	BOOL bResult = pMsgFilter(WM_COPYDATA, MSGFLT_ADD);
	ASSERT(bResult == TRUE);

	if( hUserModule != NULL)
		FreeLibrary(hUserModule);
}

void ModuleManagerImpl::destroy()
{
	for(ModuleInterfaceC::iterator it = m_ModuleInterfaceC.begin(); it != m_ModuleInterfaceC.end(); ++it)
	{
		it->second.m_IModule->Unload();
	}

	for(ModuleInterfaceC::iterator it = m_ModuleInterfaceC.begin(); it != m_ModuleInterfaceC.end(); ++it)
	{
		it->second.m_ReleaseModuleFunc();
	}

	m_ModuleInterfaceC.clear();
}

void ModuleManagerImpl::ModuleManagerProcess()
{
	// if the loop take too much time and block UI, it will be big problem... but i do not think it will happen
	//first process the event

	if(m_run == false)//如果已经多次尝试退出，不是好事。。。
	{
		m_exitCounter++;
		if(m_exitCounter>10)
		{
			::PostMessage(NULL,WM_QUIT,NULL,NULL);
		}
	}

	InnerExchange();

	for(BufUnitVec::iterator it = m_inVec.begin();it != m_inVec.end(); ++it)
		m_eventMsgBuf.push_back(*it);
	m_inVec.clear();

	while(m_eventMsgBuf.size())
	{
		ModuleInterfaceC::iterator it;

		Event & eventRef = *reinterpret_cast<Event*>(m_eventMsgBuf[0].buf);
		Message & messageRef = *reinterpret_cast<Message*>(m_eventMsgBuf[0].buf);

		StatSysCmd(eventRef.eventValue);

		if(IsEventValue(eventRef.eventValue))
		{
			Event event = eventRef;
			
			m_eventMsgBuf.pop_front();

			if(event.eventValue == EVENT_VALUE_SYS_MAIN_LOOP_EXIT)
			{
				Exit();			
				continue;
			}

			if(EXIST_MODULE(it,event.desMId))
			{
				DWORD dwBegin = GetTickCount();

				//LOG_TRACE(L"ModuleManagerImpl::ModuleManagerProcess event enter %d", event.eventValue);

				it->second.m_IModule->ProcessEvent(event);

				//LOG_TRACE(L"ModuleManagerImpl::ModuleManagerProcess event leave %d", event.eventValue);

				DWORD dwEnd = GetTickCount();

				if(dwEnd - dwBegin > 1000)
				{
					//assert(0);
				}

				if(event.m_pstExtraInfo)
				{
					if(EXIST_MODULE(it,event.srcMId))
						it->second.m_IModule->PaybackExtraInfo(event.eventValue,&event);
					else
						assert(0);
				}
			}
		}
		else if(IsMessageValue(messageRef.messageValue))
		{
			Message message= messageRef;
			m_eventMsgBuf.pop_front();

			if(message.messageValue == MESSAGE_VALUE_SYS_PRELOGOUT)
			{
				ProcessPreLogout(message);
			}
			else
			{
				//LOG_TRACE(L"ModuleManagerImpl::ModuleManagerProcess message enter %d", message.messageValue);

				for(ModuleInterfaceC::iterator it=m_ModuleInterfaceC.begin(); it != m_ModuleInterfaceC.end(); ++it)
				{
					DWORD dwBegin = GetTickCount();

					(*it).second.m_IModule->ProcessMessage(message);

					DWORD dwEnd = GetTickCount();

					if(dwEnd - dwBegin > 1000)
					{
						//assert(0);
					}
				}

				//LOG_TRACE(L"ModuleManagerImpl::ModuleManagerProcess message leave %d", message.messageValue);

				if(message.m_pstExtraInfo)
				{
					if(EXIST_MODULE(it,message.srcMId))
						it->second.m_IModule->PaybackExtraInfo(message.messageValue,&message);
					else
						assert(0);
				}
			}	
		}
		else
		{
			assert(0);
			LOG_ERROR(L"ModuleManagerImpl::ModuleManagerProcess unknown %d", *((uint32*)m_eventMsgBuf[0].buf));
			m_eventMsgBuf.pop_front();
		}		
	}

	
}

void ModuleManagerImpl::ProcessPreLogout(Message & message)
{
	BOOL bAllSuc = TRUE;

	for(ModuleInterfaceC::iterator it = m_ModuleInterfaceC.begin(); it != m_ModuleInterfaceC.end(); ++it)
	{
		if(ProcessPreLogoutInner(message,it->first,it->second.m_IModule,bAllSuc)==true)
			break;
	}
	
	if (bAllSuc)
	{
		Message msg;
		msg.messageValue = MESSAGE_VALUE_SYS_LOGOUT;
		if (message.param0 == 1)
		{
			msg.param0 = 1;
		}
		PushMessage(msg);
	}
}

bool  ModuleManagerImpl::ProcessPreLogoutInner(Message & message,ModuleId id,IModule*  pIModule,BOOL & bAllSuc)
{
	PreLogoutParam myParam;
	if (message.param0 == 1)
	{
		myParam.bOffline = TRUE;
	}
    if (message.param1)
    {
        myParam.bForce = TRUE;
    }
	pIModule->Invoke(MESSAGE_VALUE_SYS_PRELOGOUT, (param)&myParam);
	if (myParam.bSuc == FALSE)
	{
		bAllSuc = FALSE;
		return true;
	}
	return false;

}

void ModuleManagerImpl::run()
{
	CreatCycleWnd();

	int id = 77;
	uintptr timerHandle = SetTimer(s_hMsgWindow,id,SYS_CYCLE_ELAPSE,NULL);

	m_run = true;

	OnCycleTrigger();	

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0) && (m_run!=false))
	{
		InnerRun(msg);
	}

	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE) && m_eventMsgBuf.size() )
	{
		InnerRun(msg);
	}

	KillTimer(NULL, timerHandle);
	DestroyCycleWnd();
}

bool ModuleManagerImpl::InnerRun(MSG & msg)
{
	if(!AfxPreTranslateMessage(&msg))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}

	return true;
}

bool ModuleManagerImpl::PushEvent(Event const& event_)
{
	assert(!(event_.srcMId==(UINT16)MODULE_ID_INVALID) && !(event_.desMId==(UINT16)MODULE_ID_INVALID));
	if(m_eventMsgBuf.size()<=10000)
	{
		if(IsEventValue(event_.eventValue))
		{
			m_eventMsgBuf.push_back(event_);			
			return true;
		}
		else
		{
			assert(0);
			LOG_ERROR(L"ModuleManagerImpl::PushEvent failed wrong value %d", event_.eventValue);
		}
		
	}
	else
	{
		assert(0);
		LOG_ERROR(L"ModuleManagerImpl::PushEvent failed full %d", event_.eventValue);
	}
	
	return false;
}

bool ModuleManagerImpl::PushAsynEvent(Event const& event_)
{
	common::utility::LockMgr<common::utility::CSWrapper> guard(lock_m, TRUE);
	if(IsEventValue(event_.eventValue))
	{
		m_outVec.push_back(event_);
		return true;
	}
	else
	{
		assert(0);
		return false;
	}
}

bool ModuleManagerImpl::PushAsynMessage(Message const & message_)
{
	common::utility::LockMgr<common::utility::CSWrapper> guard(lock_m, TRUE);
	if(	IsMessageValue(message_.messageValue))
	{
		m_outVec.push_back(message_);
		return true;
	}
	else
	{
		assert(0);
		return false;
	}
}

int32 ModuleManagerImpl::CallService(ServiceValue const w, param l)
{
	ModuleInterfaceC::iterator it;

	if(w == SERVICE_VALUE_SYS_STAT_NETCMD)
	{
		StatNetCmd(l);
		return 0;
	}
	
	if(IsServiceValue(w))
	{
		if(EXIST_MODULE(it,MODULIE_ID_FROM_SERVICE_VALUE(w)))
			return it->second.m_IModule->Invoke(w,l);
		else
		{
			assert(0);
		}
	}	
	else
	{
		assert(0);
	}

	return -1;
}

void ModuleManagerImpl::StatNetCmd(int netCmd)
{
	m_DumpNodeNetCmd.push_back(DumpNode(netCmd));

	if(m_DumpNodeNetCmd.size() > DUMPNODE_NETCMD_MAXSIZE)
	{
		m_DumpNodeNetCmd.pop_front();
	}
}

void ModuleManagerImpl::StatSysCmd(int sysCmd)
{
	m_DumpNodeSysCmd.push_back(DumpNode(sysCmd));

	if(m_DumpNodeSysCmd.size() > DUMPNODE_SYSCMD_MAXSIZE)
	{
		m_DumpNodeSysCmd.pop_front();
	}
}

void ModuleManagerImpl::InnerExchange()
{
	assert(m_inVec.size()==0);
	if(m_outVec.size()!=0)
	{
		common::utility::LockMgr<common::utility::CSWrapper> guard(lock_m, TRUE);
		std::swap(m_outVec, m_inVec);
	}
}

bool ModuleManagerImpl::PushMessage(Message const & message_)
{
	if(m_eventMsgBuf.size() <= 10000)
	{		
		if(	IsMessageValue(message_.messageValue))
		{
			m_eventMsgBuf.push_back(message_);
			return true;
		}
		else
		{
			assert(0);
			LOG_ERROR(L"ModuleManagerImpl::PushMessage failed  wrong value %d", message_.messageValue);
		}
	}
	else
	{
		assert(0);
		LOG_ERROR(L"ModuleManagerImpl::PushMessage failed  full %d", message_.messageValue);
	}

	return false;
}

void ModuleManagerImpl::Exit()
{
	LOG_WARN(L"ModuleManagerImpl::Exit()");
	m_run = false; 		
}

void ModuleManagerImpl::OnCycleTrigger()
{
	PushMessage(MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_SYS_CYCLE_TRIGGER));
	
	common::utility::_Timers::instance().OnTimer();

	ModuleManagerProcess();

	static unsigned int iFactor = 0;

	if(iFactor<10*60)  //前一分钟，10s调用一次
	{
		if(0 == (iFactor%(10*10)))
		{
			//::SetProcessWorkingSetSize(::GetCurrentProcess(), MEDIUM_MEM_SIZE, MEDIUM_MEM_SIZE);
		}
	}
	else
	{
		if (0 == (iFactor%(10*60*5))) //100ms 5分钟
		{
			//::SetProcessWorkingSetSize(::GetCurrentProcess(), MEDIUM_MEM_SIZE, MEDIUM_MEM_SIZE);
		}
	}

	++iFactor;
}

#ifndef _M_IX86
#error "The following code only works for x86!"
#endif

void DisableSetUnhandledExceptionFilter()
{
	void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")),
		"SetUnhandledExceptionFilter");

	if (addr)
	{
		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag, dwTempFlag;

		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}

static uint32 s_iWndAtom = (unsigned)time( NULL );
LRESULT CALLBACK CycleProc(HWND inWindow, UINT inMsg, WPARAM inParam, LPARAM inOtherParam);

void ModuleManagerImpl::CreatCycleWnd()
{
	std::wstringstream os;
	os<<L"CycleWnd"<<s_iWndAtom;
	std::wstring className=os.str();

	WNDCLASSEX theWndClass;
	theWndClass.cbSize = sizeof(theWndClass);
	theWndClass.style = 0;
	theWndClass.lpfnWndProc = &CycleProc;
	theWndClass.cbClsExtra = 0;
	theWndClass.cbWndExtra = 0;
	theWndClass.hInstance = NULL;
	theWndClass.hIcon = NULL;
	theWndClass.hCursor = NULL;
	theWndClass.hbrBackground = NULL;
	theWndClass.lpszMenuName = NULL;
	theWndClass.lpszClassName = className.c_str();
	theWndClass.hIconSm = NULL;
	ATOM theWndAtom = ::RegisterClassEx(&theWndClass);

	assert(theWndAtom != NULL);

	s_hMsgWindow = ::CreateWindow( className.c_str(), className.c_str(), WS_POPUP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, NULL, NULL);

	assert(s_hMsgWindow != NULL);

	SetWindowLong(s_hMsgWindow, GWL_USERDATA, (LONG)this);
}

void ModuleManagerImpl::DestroyCycleWnd()
{
	::DestroyWindow(s_hMsgWindow);
	std::wstringstream os;
	os<<L"CycleWnd"<<s_iWndAtom;
	std::wstring className=os.str();
	UnregisterClass(className.c_str(), NULL );
}

static LRESULT CALLBACK CycleProc(HWND inWindow, UINT inMsg, WPARAM inParam, LPARAM inOtherParam)
{
	ModuleManagerImpl * pModuleManagerImpl = (ModuleManagerImpl*)GetWindowLong(inWindow, GWL_USERDATA);
	if(pModuleManagerImpl)
	{
		if(inMsg == WM_TIMER)
		{
			pModuleManagerImpl->OnCycleTrigger();
			return true ;
		}
		else
			return DefWindowProc(inWindow, inMsg, inParam, inOtherParam);
	}
	else 
	{
		if(inMsg == WM_NCCREATE)
			return true;
		else
			return DefWindowProc(inWindow, inMsg, inParam, inOtherParam);
	}
}

#pragma warning (default:4819)
#pragma warning (default:4227)