#include "stdafx.h"
#include "ModuleManagerMain.h"
#include "ModuleManager.h"
#include "HallDefine.h"
#include "utility/SystemHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
END_MESSAGE_MAP()

CMainApp::CMainApp()
{
	m_hMutex = NULL;
	m_hMapFile = NULL;
}

CMainApp theApp;

void PurecallHandler(void)
{
	MessageBox(NULL,L"发生纯虚函数调用，请叫开发人员，thx！！！",L"错误",MB_OK);
	if(IsDebuggerPresent())
	{
		DebugBreak();
	}
}

// CMainApp 初始化

BOOL CMainApp::CheckSingleInstance()
{
	HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, FALSE, _T("Global\\51KTVUpdateCopy"));
	if(hMutex != NULL)
	{
		CloseHandle(hMutex);
		hMutex = NULL;
		AfxMessageBox(_T("当前正在升级，白兔KTV不能启动！"));
		return FALSE;
	}

	m_hMutex = ::CreateMutex(NULL, FALSE, _T("Global\\51KTVMainApp"));
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hMutex, INFINITE))
		{
			HANDLE hMapFile = ::OpenFileMapping(FILE_MAP_READ, FALSE, _T("Local\\51KTVMainFileMap"));
			if(hMapFile == NULL)
			{
				ReleaseMutex(m_hMutex);
				CloseHandle(m_hMutex);
				m_hMutex = NULL;
				return FALSE;
			}

			ShareMemInfo* pMemInfo = (ShareMemInfo*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, sizeof(ShareMemInfo));
			if(pMemInfo == NULL)
			{
				CloseHandle(hMapFile);
				ReleaseMutex(m_hMutex);
				CloseHandle(m_hMutex);
				m_hMutex = NULL;
				return FALSE;
			}

			if(pMemInfo->unVersion == SHARE_MEMORY_INFO_VERSION)
			{
				if(::IsWindowEnabled(pMemInfo->hwndHall))
				{
					if(::IsIconic(pMemInfo->hwndHall))
					{
						::SendMessage(pMemInfo->hwndHall,WM_SYSCOMMAND, SC_RESTORE, 0);
					}
					else
					{
						::SetForegroundWindow(pMemInfo->hwndHall);	
					}
				}
				else if(::IsIconic(pMemInfo->hwndHall))
				{
					::EnableWindow(pMemInfo->hwndHall, TRUE);						
					::SendMessage(pMemInfo->hwndHall, WM_SYSCOMMAND, SC_RESTORE, 0);
					::SendMessage(pMemInfo->hwndLogin, WM_SYSCOMMAND, SC_RESTORE, 0);
					::EnableWindow(pMemInfo->hwndHall, FALSE);
				}
			}

			UnmapViewOfFile(pMemInfo);
			CloseHandle(hMapFile);
			ReleaseMutex(m_hMutex);
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
			return FALSE;
		}

		return FALSE;
	}
	else if(m_hMutex != NULL)
	{
		common::utility::systemhelper::SetSecurityLevel(m_hMutex);

		m_hMapFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 
			sizeof(ShareMemInfo), _T("Local\\51KTVMainFileMap"));

		if(m_hMapFile != NULL)
		{
			common::utility::systemhelper::SetSecurityLevel(m_hMapFile);
		}		
	}

	return TRUE;
}

BOOL CMainApp::InitInstance()
{
	if(!CheckSingleInstance())
	{
		return FALSE;
	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//虚函数调用捕获
	_set_purecall_handler(PurecallHandler);

	module_manager.PushEvent(MakeEvent<MODULE_ID_CORE>()(Hall::EVENT_VALUE_HALL_CREATEANDSHOW,MODULE_ID_HALL));

	//module_manager.PushEvent(MakeEvent<MODULE_ID_ROOM>()(Room::EVENT_VALUE_TEST,MODULE_ID_ROOM));

    //模块管理单元启动
	module_manager.Construct();

	module_manager.run();

	module_manager.Destruct();

	return FALSE;
}

int CMainApp::ExitInstance() 
{
	if (m_hMapFile)
	{
		CloseHandle(m_hMapFile);
	}

	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
	}

	return CWinApp::ExitInstance();
}