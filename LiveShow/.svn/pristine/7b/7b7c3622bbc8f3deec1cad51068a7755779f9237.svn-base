#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "ModuleManager.h"

class CMainApp : public CWinApp
{
public:
	CMainApp();

	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void CMainApp::OnCycleTrigger(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	void CreatCycleWnd();
	void DestroyCycleWnd();

private:

	BOOL CheckSingleInstance();

	ModuleManagerImpl module_manager;
	HANDLE m_hMutex;
	HANDLE m_hMapFile;
};

extern CMainApp theApp;