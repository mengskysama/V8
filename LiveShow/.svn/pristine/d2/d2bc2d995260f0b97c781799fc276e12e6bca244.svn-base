#pragma once
#include <vector>
#include <list>

enum ProcessType 
{
	PROCESS_NAME,
	PROCESS_WINDOWS,
};

class CProcessReport
{
public:
	CProcessReport();
	~CProcessReport(){};

public:
	void OnProcessReport(); //发送对手进程名数据至服务器

private:
	std::wstring GetProcessName(); //获取正在运行的对手进程名
	std::wstring GetProcess(std::vector<std::wstring> &processName,ProcessType pType);  //
	std::wstring FindProcessWindowsText(std::vector<std::wstring> *processName);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam); //枚举所有窗口回调函数

private:
	std::vector<std::wstring> m_OpponentsProcessName; //对手进程名列表
	std::vector<std::wstring> m_OpponentsWindowsText; //对手进程窗口名列表	
	static std::wstring m_processWindows;

};