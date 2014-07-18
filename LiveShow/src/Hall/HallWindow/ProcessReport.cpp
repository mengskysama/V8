#include "stdafx.h"
#include "ProcessReport.h"
#include <algorithm>
#include "tlhelp32.h"
#include "..\Hall.h"


std::wstring CProcessReport::m_processWindows = _T("");
CProcessReport::CProcessReport()
{
	m_OpponentsProcessName.push_back(_T("yy.exe"));
	m_OpponentsProcessName.push_back(_T("9158.exe"));
	m_OpponentsProcessName.push_back(_T("sinashow.exe"));
	m_OpponentsProcessName.push_back(_T("chathall.exe"));
	m_OpponentsProcessName.push_back(_T("guagua.exe"));
	m_OpponentsProcessName.push_back(_T("eph.exe"));
	m_OpponentsProcessName.push_back(_T("99lover.exe"));
	m_OpponentsProcessName.push_back(_T("293show.exe"));

	m_OpponentsWindowsText.push_back(_T("六间房秀场"));
	m_OpponentsWindowsText.push_back(_T("xiu.56.com"));
	m_OpponentsWindowsText.push_back(_T("主播间"));
	m_OpponentsWindowsText.push_back(_T("呱呱视频社区"));
	m_OpponentsWindowsText.push_back(_T("新浪show"));
	m_OpponentsWindowsText.push_back(_T("搜狗繁星"));
	m_OpponentsWindowsText.push_back(_T("ibo秀场"));
	m_OpponentsWindowsText.push_back(_T("蜜糖视频交友网"));
	m_OpponentsWindowsText.push_back(_T("搜狐秀场"));
	m_OpponentsWindowsText.push_back(_T("5see真人秀场"));
	m_OpponentsWindowsText.push_back(_T("激动星秀"));
	
}
//发送对手进程名数据至服务器
void CProcessReport::OnProcessReport()
{
	std::wstring strRunProcName = GetProcessName();  //查询获取正在运行的对手进程名
	if(strRunProcName == _T(""))
	{
		return;
	}

	//上传对手进程名数据至服务器

	core::ReportCollectInfoEvent *runProcEvent = new core::ReportCollectInfoEvent();	
	core::ReportCollectInfoEvent::Key2Value runProc;
	runProc.key = COLLECT_INFO_PROCESS;
	runProc.value = strRunProcName;
	runProcEvent->vecCollecInfo.push_back(runProc);

	Event _event = MakeEvent<MODULE_ID_HALL>()(core::EVENT_VALUE_CORE_REPORT_COLLECT_INFO, 
		MODULE_ID_CORE,0,0,0,runProcEvent);
	g_HallModule->m_pModuleManager->PushEvent(_event);
}

//获取正在运行的对手进程名
std::wstring CProcessReport::GetProcessName()
{
	std::wstring runProcName;
	runProcName = GetProcess(m_OpponentsProcessName,ProcessType::PROCESS_NAME );  //使用进程名列表查找正在运行的对手进程
	if(runProcName == _T("")) //未找到
	{
		runProcName = GetProcess(m_OpponentsWindowsText,ProcessType::PROCESS_WINDOWS ); //使用窗口名列表查找正在运行的对手窗口标题
	}

	return runProcName;
}

std::wstring CProcessReport::GetProcess(std::vector<std::wstring> &processName,ProcessType pType)
{
	std::wstring procName;
	if(pType == PROCESS_WINDOWS)
	{
		procName = FindProcessWindowsText(&processName);
		return procName;
	}


	HANDLE toolhelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(toolhelp == NULL)
	{
		return procName;
	}

	PROCESSENTRY32 processinfo;
	processinfo.dwSize   =   sizeof(PROCESSENTRY32);
	BOOL start = Process32First(toolhelp, &processinfo); //获取第一个正在运行的进程			

	while(start)
	{

		std::wstring runProcName = processinfo.szExeFile;
		std::transform(runProcName.begin(),runProcName.end(),runProcName.begin(),tolower);  //转小写


		vector<std::wstring>::iterator itfind;
		itfind = find(processName.begin(),processName.end(),runProcName);
		if(itfind != processName.end()) //找到
		{	
			procName = runProcName;
			break;
		}		

		start = Process32Next(toolhelp, &processinfo); //下一个正在运行的进程

	}

	CloseHandle(toolhelp);

	return procName;

}



std::wstring CProcessReport::FindProcessWindowsText(std::vector<std::wstring> *processName)
{
	//枚举所有窗口标题
	std::wstring winName;
	m_processWindows = _T("");	
	EnumWindows(EnumWindowsProc,(LPARAM)processName);  //枚举所有窗口
	OutputDebugString(_T("FindProcessWindowsText"));
	OutputDebugString(m_processWindows.c_str());  

	return m_processWindows;

}

//枚举所有窗口回调函数
BOOL CProcessReport::EnumWindowsProc(HWND hwnd,LPARAM lparam)
{
	std::vector<std::wstring> *processName = (std::vector<std::wstring>*)lparam;
	TCHAR windowName[1024];
	memset(windowName, 0, 1024 * sizeof(TCHAR));
	GetWindowText(hwnd,windowName,1024);	//取得窗口标题
	std::wstring strName = windowName;
	std::wstring strProcessName;
	if(strName != _T(""))
	{
		std::transform(strName.begin(),strName.end(),strName.begin(),tolower);  //转小写

		for(int i=0;i<processName->size();i++)
		{
			strProcessName = processName->at(i);
			if(strName.find(strProcessName) != -1)  //部分查找窗口名中包含指定对手名的标题名
			{
				m_processWindows = strProcessName;			
				return false;  //已找到，不用再继续枚举
			}
		}
	}	

	return true;  //继续枚举

}
