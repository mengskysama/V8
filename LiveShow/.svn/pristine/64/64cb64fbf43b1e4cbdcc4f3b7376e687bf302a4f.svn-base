#pragma once

#include "utility/systemhelper.h"
#include "utility/FileHelper.h"

class BlacklistHelper
{

private:

	static std::wstring getBlacklistPath()
	{
		std::wstring strPath;

		int iOSVersion = common::utility::systemhelper::GetOsVersion();

		if (iOSVersion >= common::utility::systemhelper::OS_VISTA)
		{
			//http://msdn.microsoft.com/en-us/library/bb762584(VS.85).aspx
			GUID FOLDERID_RoamingAppData = {0x3EB685DB, 0x65F9, 0x4CF6, {0xA0, 0x3A, 0xE3, 0xEF, 0x65, 0x72, 0x9F, 0x3D}};
			//GUID FOLDERID_ProgramData = {0x62AB5D82, 0xFDC1, 0x4DC3, {0xA9, 0xDD, 0x07, 0x0D, 0x1D, 0x49, 0x5D, 0x97}};
			typedef UINT (CALLBACK* LPFNDLLFUNC) (GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath); 

			HINSTANCE hInst; 
			LPFNDLLFUNC pGetKnownFldPathFnPtr;    // Function pointer 
			hInst = LoadLibrary(_T("shell32.dll")); 
			if ( hInst == NULL ) 
				return std::wstring();

			pGetKnownFldPathFnPtr = (LPFNDLLFUNC)GetProcAddress(hInst, "SHGetKnownFolderPath"); 

			if ( pGetKnownFldPathFnPtr ) 
			{
				PWSTR path = NULL;

				if ( SUCCEEDED(pGetKnownFldPathFnPtr( FOLDERID_RoamingAppData, 0, NULL, &path )) ) 
				{ 
					strPath = path;
					CoTaskMemFree( path ); 
					FreeLibrary( hInst ); 
					strPath.append(L"\\Microsoft\\");
					CreateDirectory(strPath.c_str(), NULL);
					strPath.append(L"Windows\\");
					CreateDirectory(strPath.c_str(), NULL);
					
					if(common::utility::filehelper::IsDirExist(strPath.c_str()))
					{
						return strPath;
					}
				} 
				else
				{
					FreeLibrary( hInst );
				}
			}
		}

		wchar_t szFilePath[MAX_PATH] ;
		GetWindowsDirectory(szFilePath, MAX_PATH);

		strPath = szFilePath;
		strPath.append(L"\\system32\\");

		return strPath;
	}

public:

	static void TickCount()
	{
		std::wstring strPath = getBlacklistPath();

		std::wstring strPath2 = strPath;
		strPath2.append(L"winplus.dll");

		if(common::utility::filehelper::IsFileExist(strPath2.c_str()))
		{
			strPath2 = strPath;
			strPath2.append(L"wins32p.dll");

			HANDLE hFile = ::CreateFile(strPath2.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
			if(hFile != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hFile);
			}
		}
		else
		{
			HANDLE hFile = ::CreateFile(strPath2.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
			if(hFile != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hFile);
			}
		}
	}

	static void CheckTick()
	{
		std::wstring strPath = getBlacklistPath();
		strPath.append(L"wins32p.dll");

		if(common::utility::filehelper::IsFileExist(strPath.c_str()))
		{
			CoreModule::GetCurInfo().bInBlacklist = true;
		}
	}
};