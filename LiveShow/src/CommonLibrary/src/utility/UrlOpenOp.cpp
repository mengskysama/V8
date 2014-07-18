#pragma once;

#include <string>
#include <xstring>
#include "utility/stringhelper.h"
#include "windows.h"
#include <algorithm>
#include "utility/systemhelper.h"
#include "utility/registerhelper.h"
#include "boost\algorithm\string.hpp"
#include "utility/UrlOpenOp.h"
#include <atlbase.h>

namespace common
{
namespace utility
{
namespace systemhelper
{
namespace urlopenimply
{
	std::wstring GetIEVersionImply()
	{		
		TCHAR szVersion[128] = {0};
		DWORD dwSize = 128;   
		CRegKey keyVersion;   
		LONG lResult = keyVersion.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer"));
		if(ERROR_SUCCESS == lResult)
		{
			keyVersion.QueryStringValue(_T("Version"), szVersion, &dwSize);
			keyVersion.Close();
		}

		return std::wstring(szVersion);
	}


	template<typename T>
	struct TraitsCommand
	{
		// typedef UrlCommandW value;
		static std::wstring ProcessParam(T const & t)
		{
			return t;
		}
	};

	template<>
	struct TraitsCommand<std::string>
	{
		//typedef UrlCommand value;
		static std::wstring ProcessParam(std::string const & t)
		{
			std::wstring re;
			re=stringhelper::UTF8ToUnicode(t);
			return re;
		}
	};


	BOOL OpenUrlWithIe(std::wstring const & url,std::wstring const & ie)
	{
		HINSTANCE re;
		std::wstring strIE = GetIEVersionImply();
		float fIEVersion = _tstof(strIE.c_str());
		if(fIEVersion>6.99)
		{
			re = ShellExecute(NULL, L"open", url.c_str(), NULL,NULL, SW_SHOWNOACTIVATE);
		}
		else
		{
			re = ShellExecute(NULL, L"open", ie.c_str(),url.c_str(), NULL, SW_SHOWNOACTIVATE);
		}
		return re!=NULL?TRUE:FALSE;
	}


	void GenIePath(std::wstring  & out)
	{

		std::transform(out.begin(),
			out.end(),
			out.begin(),
			(int(*)(int))tolower);

		if(out.find(L"internet explorer")!=std::wstring::npos)
		{
			if(out.size()>1)
			{
				if(out[out.size()-1]==L';')
					out.erase(out.begin()+out.size()-1);
			}
			out.append(L"\\iexplore.exe");
		}

	}


	//BOOL OpenUrlImply(std::wstring const & url);

	template<typename stringT>
	BOOL OpenUrl(stringT const & url)
	{
		std::wstring urlResult(TraitsCommand<stringT>::ProcessParam(url));
		return OpenUrlImply(urlResult);
	}
	BOOL OpenUrlImply(std::wstring const & url)
	{
		HINSTANCE reHINSTANCE=NULL;
		if(::common::utility::systemhelper::GetOsVersion()>=::common::utility::systemhelper::OS_VISTA)
		{
			reHINSTANCE=ShellExecute(NULL, L"open", url.c_str(), NULL,NULL, SW_SHOWNOACTIVATE);
			return reHINSTANCE!=NULL?TRUE:FALSE;
		}
		else
		{
			//static const wchar_t * pCommand=L"http\\shell\\open\\command";

			Register key(HKEY_CLASSES_ROOT, L"http\\shell\\open\\command", KEY_READ);
			std::wstring exePath;
			if (key.Valid() && key.ReadValue(L"", &exePath))
			{
				boost::iterator_range<std::wstring::const_iterator> irBegin;
				irBegin=boost::algorithm::find_first(exePath,L"\""); 
				if(irBegin.begin()!=exePath.end())
				{
					std::wstring newStr(irBegin.end(),exePath.end());
					boost::iterator_range<std::wstring::const_iterator> irEnd;
					irEnd=boost::algorithm::find_first(newStr,L"\""); 
					if(irEnd.begin()!=newStr.end())
					{
						std::wstring realExePath(newStr.begin(),irEnd.begin());
						int re=_waccess( realExePath.c_str(), 00);
						if(re==0)
						{
							std::transform(realExePath.begin(),
								realExePath.end(),
								realExePath.begin(),
								(int(*)(int))tolower);

							if(realExePath.find(L"iexplore.exe")!=std::wstring::npos)
							{
								return OpenUrlWithIe(url,realExePath);
							}
							else
							{
								reHINSTANCE=ShellExecute(NULL, L"open", url.c_str(), NULL,NULL, SW_SHOWNOACTIVATE);
								return reHINSTANCE!=NULL?TRUE:FALSE;
							}
						}
					}
				}
			}

FinalWay :

			//static const wchar_t * pIe=L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE";

			RegKey keyIe(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", KEY_READ);
			std::wstring iePath;
			if (keyIe.Valid() && keyIe.ReadValue(L"Path", &iePath))
			{
				GenIePath(iePath);
				return OpenUrlWithIe(url,iePath);
			}
			else
			{
				assert(0);//上帝也没辙了。。。
				return FALSE;
			}
		}
	}




}
}
}
}
