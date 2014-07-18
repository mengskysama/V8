// x179.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "x179.h"
#include "51ktv.h"
#include "../CommonLibrary/include/utility/FileHelper.h"

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cx179App

BEGIN_MESSAGE_MAP(Cx179App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cx179App 构造

Cx179App::Cx179App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

Cx179App theApp;

CString Get179AppPath()
{
#ifdef _DEBUG
	//return "F:\\vdc_dev\\51ktv\\bin\\debug\\baituKTVd.exe";
#endif

	HKEY hKey;
	if(RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\51Corp\\白兔KTV", &hKey) == ERROR_SUCCESS)
	{
		char sz[256];
		DWORD dwSize = sizeof(sz);
		if(RegQueryValueEx(hKey, "Install", NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS)
		{
			CString path;

#ifdef _DEBUG
			path.Format("%s\\baituKTVd.exe", sz);
#else
			path.Format("%s\\baituKTV.exe", sz);
#endif
			if(PathFileExists(path.GetBuffer()))
			{
				return path;
			}
		}
	}

	for(char p = 'C'; p <= 'H'; p++)
	{
		CString path;

#ifdef _DEBUG
		path.Format("%c:\\Program Files\\179\\179ktvd.exe", p);
#else
		path.Format("%c:\\Program Files\\179\\179ktv.exe", p);
#endif
		if(PathFileExists(path.GetBuffer()))
		{
			return path;
		}
	}

	return "";
}

CString GetCommand()
{
	char* lpCommandLine = GetCommandLine();
	OutputDebugStringA("plushu GetCommand()");
	OutputDebugStringA(lpCommandLine);

	char* pPos = strchr(lpCommandLine, '\"');
	if(NULL == pPos || *(pPos+1) == 0)
	{
		return "";
	}

	pPos = strchr(pPos+1, '\"');
	if(NULL == pPos || *(pPos+1) == 0 || *(pPos+2) == 0)
	{
		return "";
	}

	CString code = pPos+2;

	OutputDebugStringA("plushu Command:");
	OutputDebugStringA(code.GetBuffer());

	code.TrimLeft();

	return code;
}

void Open179App()
{
	CString strPath = Get179AppPath();
	if(strPath.IsEmpty())
	{
		return;
	}

	CString strCommand = GetCommand();
	if(strCommand.IsEmpty())
	{
		return;
	}
	
	ShellExecute(NULL, "open", strPath.GetBuffer(), strCommand.GetBuffer(), NULL, SW_SHOWNORMAL);
}

void Invoke179App(HANDLE hMutex)
{
	//客户端已经打开
	if(WAIT_OBJECT_0 == ::WaitForSingleObject(hMutex, INFINITE))
	{
		HANDLE hMapFile = ::OpenFileMapping(FILE_MAP_READ, FALSE, _T("Local\\51KTVMainFileMap"));
		if(hMapFile == NULL)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
			return;
		}

		ShareMemInfo* pMemInfo = (ShareMemInfo*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, sizeof(ShareMemInfo));
		if(pMemInfo == NULL)
		{
			CloseHandle(hMapFile);
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
			return;
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

			CString strCommand = GetCommand();			
			if(!strCommand.IsEmpty())
			{
				HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, strCommand.GetLength()+1);
				char* pDst = (char*)::GlobalLock(hGlobal);
				strcpy(pDst, strCommand.GetBuffer());
				::GlobalUnlock(hGlobal);
				::OpenClipboard(::GetDesktopWindow());
				::SetClipboardData(RegisterClipboardFormat(WEBPLUGIN_DATA_FORMAT), hGlobal);
				::CloseClipboard();
				::SendMessage(pMemInfo->hwndLogin, WM_WEBPLUGIN_MESSAGE, 0, 0);
				::GlobalFree(hGlobal);
			}
		}

		UnmapViewOfFile(pMemInfo);
		CloseHandle(hMapFile);
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
}

void OpenWebPluginRequest(CString& strURL, CString& strResult)
{
	HINTERNET handle = InternetOpen("x179", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);

	if(handle == NULL)
	{
		return;
	}

	HINTERNET internetopenurl = 
		InternetOpenUrl(handle, strURL.GetBuffer(), NULL, 0, 
		INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE, 0);

	if(internetopenurl != NULL)
	{
		byte buffer[10240] = {0};
		DWORD dwReadBytes = 0;
		DWORD dwTotal = 0;

		while(InternetReadFile(internetopenurl, &buffer[dwTotal], 10240-dwTotal, &dwReadBytes)
			&& dwReadBytes > 0)
		{
			dwTotal += dwReadBytes;
			dwReadBytes = 0;
			if(10240-dwTotal <= 0)
				break;
		}

		buffer[dwTotal] = 0;
		strResult = (char*)buffer;

		InternetCloseHandle(internetopenurl);
	}

	InternetCloseHandle(handle);
}

void GetIdPswd(CString cmd, CString& id1, CString& pswd1)
{
	//{"vdcID":"801000514","password":"acfbb43fc3a4484a1bf49996286cd808","error":0}

	if(cmd.Find("\"error\":0") < 0) return;
	
	char sz1[100] = "\"vdcID\":\"";
	char sz2[100] = "\"password\":\"";

	int x = cmd.Find(sz1);
	if(x < 0) return;
	x += strlen(sz1);

	char id[100]={0};
	char pswd[100]={0};

	int i=x;
	for(;i<cmd.GetLength();i++)
	{
		if(cmd.GetAt(i) < '0' || cmd.GetAt(i) > '9')
			break;
		id[i-x] = cmd.GetAt(i);
	}
	if(cmd.GetAt(i) != '"')
		return;
	id[i-x] = 0;

	int y = cmd.Find(sz2);
	if(y < 0) return;
	y += strlen(sz2);

	i=y;
	for(;i<cmd.GetLength();i++)
	{
		if(cmd.GetAt(i) == '"')
			break;
		pswd[i-y] = cmd.GetAt(i);
	}
	if(cmd.GetAt(i) != '"')
		return;
	pswd[i-y] = 0;

	id1 = id;
	pswd1 = pswd;
}

//处理彩虹过来的命令
void ProcessCaihongCmd(CString cmd)
{
	int x = cmd.Find("caihong");
	if(x >= 0)
		cmd = cmd.Right(cmd.GetLength()-x);

	if(cmd.GetLength() < 3)
		return;

	char sz1[100]={0};
	char sz2[100]={0};
	sscanf(cmd.GetBuffer(), "%s%s", sz1, sz2);

	if(strlen(sz2) < 3)
		return;

	for(int i=0;i<strlen(sz2);i++)
	{
		if(sz2[i] < '0' || sz2[i] > '9')
			return;
	}
	
	if(atoi(sz2) <= 100)
		return;

	CString url;
	url.Format("http://www.179.com/qqapi/caihong_callback?code=%s", sz2);
	
	CString temp;
	OpenWebPluginRequest(url, temp);

	CString id, pswd;
	GetIdPswd(temp, id, pswd);

	if(id.GetLength() < 3 || pswd.GetLength() < 5)
		return;

	CString result = "caihong ";
	result += id;
	result += " ";
	result += pswd;

	x = cmd.Find(sz2);
	x += strlen(sz2);

	result += cmd.Right(cmd.GetLength()-x);

	//////////////////////////////////////////////////////////////////////////

	CString strPath = Get179AppPath();
	if(strPath.IsEmpty())
		return;

	ShellExecute(NULL, "open", strPath.GetBuffer(), result.GetBuffer(), NULL, SW_SHOWNORMAL);
}

//处理web过来的命令
void ProcessWebCmd(CString cmd)
{
	int x = cmd.Find("://");
	CString url = "http://www.179.com/room/request?cmd=";
	cmd = cmd.Right(cmd.GetLength()-x-3);
	if(cmd.GetAt(cmd.GetLength()-1) == L'/')
		cmd = cmd.Left(cmd.GetLength()-1);
	url += cmd;

	CString temp;
	OpenWebPluginRequest(url,temp);

	HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, FALSE, _T("Global\\51KTVMainApp"));

	//客户端没有打开
	if(hMutex == NULL)
	{
		OutputDebugStringA("plushu hMutex == NULL");
		Open179App();
	}
	else
	{
		OutputDebugStringA("plushu hMutex != NULL");
		Invoke179App(hMutex);
	}
}

BOOL Cx179App::InitInstance()
{
	CWinApp::InitInstance();
	
	CString cmd = GetCommand();

	OutputDebugStringA(cmd);

	//cmd = L"xxx caihong 123456 room 100001";

	if(cmd.Find("caihong") >= 0)
	{
		ProcessCaihongCmd(cmd);
		return TRUE;
	}

	if(cmd.Find("://") > 0)
	{
		ProcessWebCmd(cmd);
		return TRUE;
	}
		
	return TRUE;
}
