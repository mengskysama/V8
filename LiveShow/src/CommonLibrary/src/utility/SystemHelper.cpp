#include "stdafx.h"
#include "utility/systemhelper.h"
#include "assert.h"
#include "utility/stringhelper.h"
#include "sddl.h"
#include "aclapi.h"
#include "utility/UrlOpenOp.h"
#include "utility\crypt\blowfish.h"
#include "boost\algorithm\string.hpp"
#include<TlHelp32.h>
#include <algorithm>
#include <boost/bind.hpp>
#include "utility/registerHelper.h"
#include "utility/filehelper.h"
#include <afxadv.h>

namespace common
{
namespace utility
{
namespace systemhelper
{	
	//SwitchResource
	SwitchResource::SwitchResource(char const * const dllName)
	{		
		std::string tempStr=dllName;
		Init(tempStr);
	}

	SwitchResource::SwitchResource(std::wstring const & dllName)
	{
		Init(dllName);
	}

	SwitchResource::SwitchResource(wchar_t const * const dllName)
	{
		std::wstring tempStr=dllName;
		Init(tempStr);
	}

	SwitchResource::SwitchResource(std::string const & dllName)
	{
		Init(dllName);
	}

	void SwitchResource::Init(std::string const & dllName)
	{
		std::string  tempStr=dllName;	
		oldHINSTANCE = AfxGetResourceHandle();	// return
		HINSTANCE hResNew = NULL;


#ifdef _DEBUG
		hResNew = GetModuleHandleA(tempStr.append("d.dll").c_str());
#else
		hResNew = GetModuleHandleA(tempStr.append(".dll").c_str());
#endif

		if(hResNew)
			AfxSetResourceHandle(hResNew);
		else
			assert(0);

	}

	void SwitchResource::Init(std::wstring const & dllName)
	{
		std::wstring  tempStr=dllName;	
		oldHINSTANCE = AfxGetResourceHandle();	// return
		HINSTANCE hResNew = NULL;


#ifdef _DEBUG
		hResNew = GetModuleHandleW(tempStr.append(L"d.dll").c_str());
#else
		hResNew = GetModuleHandleW(tempStr.append(L".dll").c_str());
#endif

		if(hResNew)
			AfxSetResourceHandle(hResNew);
		else
			assert(0);

	}

	SwitchResource::~SwitchResource()
	{
		if(oldHINSTANCE)
			AfxSetResourceHandle(oldHINSTANCE);
	}

	/*
	* 获取系统版本参数
	* @param [in] 一个OSVERSIONINFO结构的引用
	* @return TRUE表示成功，FALSE表示失败
	*/
	COMMON_INTERFACE BOOL GetOsVersion(OSVERSIONINFO & osInfo)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		return GetVersionEx(&osInfo);
	}

	/*
	* 判断当前系统是否等于某个版本
	* @param 无
	* @return OS_VERSION枚举中的一个值
	*/
	COMMON_INTERFACE OS_VERSION GetOsVersion()
	{
		OSVERSIONINFO osVer;
		if(GetOsVersion(osVer)==TRUE)
		{
			DWORD majorVer=osVer.dwMajorVersion;
			DWORD minorVer=osVer.dwMinorVersion;

			if( majorVer == 5 && minorVer == 0)
			{
				return  OS_WIN2000;
			}
			else if( majorVer == 5 && minorVer == 1)
			{
				return OS_WINXP;
			}
			else if( majorVer == 5 && minorVer == 2)
			{
				return OS_WIN2003;
			}
			else if( majorVer == 6 && minorVer == 0)
			{
				return OS_VISTA;
			}
			else if(majorVer == 6 && minorVer == 1)
			{
				return OS_WIN7;
			}
			else if(majorVer == 6 && minorVer == 2)
			{
				return OS_WIN8;
			}
			else
			{
				return OS_UNKNOWN;
			}
		}
		else
		{
			assert(0);
			return OS_UNKNOWN;
		}
	}


	/*
	* 判断当前系统是否等于某个版本
	* @param [in] 一个OS_VERSION枚举值
	* @return TRUE表示相等，FALSE表示不等,或者没有取道版本信息
	*/
	COMMON_INTERFACE BOOL IsWindowsVersionEqual(OS_VERSION const osv)
	{
		OSVERSIONINFO osInfo;
		if(GetOsVersion(osInfo)==TRUE)
		{
			switch(osInfo.dwMajorVersion)
			{
			case 5://2000 xp server2003
				{
					if(osInfo.dwMinorVersion==0
						&& osv==OS_WIN2000)
						return TRUE;
					else if(osInfo.dwMinorVersion==1
						&& osv==OS_WINXP)
						return TRUE;
					else if(osInfo.dwMinorVersion==2
						&& osv==OS_WIN2003)
						return TRUE;
					else return FALSE;
					break;
				}
			case 6://vista 2008 win7
				{
					if(osInfo.dwMinorVersion==0
						&& (osv==OS_VISTA || osv==OS_WIN2008)
					  )
						return TRUE;
					else if(osInfo.dwMinorVersion==1
						&& (osv==OS_WIN2008R2 || osv==OS_WIN7)
						)
						return TRUE;
					else return FALSE;
					break;
				}
			default:
				{
					//unknown
					return FALSE;
				}
			}
		}
		else
		{
			assert(0);
			return FALSE;
		}
	}

	/*
	* 判断当前系统是否等于某个版本或大于某个版本
	* @param [in] 一个OS_VERSION枚举值
	* @return TRUE表示相等或大于，FALSE表示不等或不大于
	*/
	COMMON_INTERFACE BOOL IsWindowsVersionEqualOrLater(OS_VERSION const osv)
	{
		OSVERSIONINFO osInfo;
		if(GetOsVersion(osInfo)==TRUE)
		{
			if(osInfo.dwMajorVersion>=7)
				return TRUE;
			else if(osInfo.dwMajorVersion==6)
			{
				if( (osv==OS_WIN7 || osv==OS_WIN2008R2)
					&& osInfo.dwMinorVersion>=1)
					return TRUE;
				else if( (osv==OS_VISTA || osv==OS_WIN2008)
					&& osInfo.dwMinorVersion>=0)
					return TRUE;
				else return FALSE;
			}
			else if(osInfo.dwMajorVersion==5)
			{
				if( osv==OS_WIN2003
					&& osInfo.dwMinorVersion>=2)
					return TRUE;
				else if( osv==OS_WINXP
					&& osInfo.dwMinorVersion>=1)
					return TRUE;
				else if( osv==OS_WIN2000
				&& osInfo.dwMinorVersion>=0)
					return TRUE;
				else return FALSE;
			}
			else return FALSE;
		}
		else
		{
			assert(0);
			return FALSE;
		}
	}

	std::wstring Get179AppPath()
	{
		static std::wstring strPath;			

		int iOSVersion = GetOsVersion();

		if (iOSVersion >= OS_VISTA)
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
					strPath.append(L"\\179\\");
					BOOL bDir = common::utility::filehelper::IsDirExist(strPath.c_str());
					if(bDir)
					{
						return strPath;
					}
					else
					{
						return GetModulePath();
					}					
				} 
				else
				{
					FreeLibrary( hInst );
				}
			}     
		}

		return GetModulePath();	
	}

	//////////////////////////////////////////////////////////////////////////

	//加密过滤词库
	void EncodeFilterWords(std::wstring& strSrc, std::wstring& strDst)
	{
		DWORD size = common::utility::filehelper::GetFileSize(strSrc.c_str());
		if(size <= 2)
		{
			return;
		}
		size -= 2;//去掉 ff fe		
		int left = 0;
		if(size%8!=0)
			left = 8-size%8;
		byte* pBuffer = new byte[size+left];
		FILE* fp = _wfopen(strSrc.c_str(), L"rb");
		if(fp != NULL)
		{
			fseek(fp, 2, SEEK_SET);//去掉 ff fe
			fread(pBuffer, size, 1, fp);
			fclose(fp);
		}
		else
		{
			delete[] pBuffer;
			return;
		}
		for(int i=0;i<left;i++)
		{
			pBuffer[size+i] = ' ';
		}
		byte key[] = "$~179^%<>/.com-=";
		common::CBlowFish blowFish;
		blowFish.Initialize(key, 16);
		int length = blowFish.Encode(pBuffer, pBuffer, size+left);

		fp = _wfopen(strDst.c_str(), L"wb");
		if(fp != NULL)
		{
			fwrite(pBuffer, length, 1, fp);
			fclose(fp);
		}
		delete[] pBuffer;
	}

	//解密过滤词库
	void DecodeFilterWords(std::wstring& strPath, std::vector<std::wstring>& vecFilterWords)
	{	
		DWORD size = common::utility::filehelper::GetFileSize(strPath.c_str());
		if(size <= 0)
		{
			return;
		}
		wchar_t* pBuffer = new wchar_t[size/2];
		FILE* fp = _wfopen(strPath.c_str(), L"rb");
		if(fp != NULL)
		{
			fread(pBuffer, size, 1, fp);
			fclose(fp);
		}
		else
		{
			delete[] pBuffer;
			return;
		}
		byte key[] = "$~179^%<>/.com-=";
		common::CBlowFish blowFish;
		blowFish.Initialize(key, 16);
		blowFish.Decode((unsigned char*)pBuffer, (unsigned char*)pBuffer, size);
		std::wstring strWord;
		for(int i=0;i<size/2;i++)
		{
			wchar_t sz = pBuffer[i];
			if(pBuffer[i] == ',')
			{
				if(strWord.size() > 0)
				{
					vecFilterWords.push_back(strWord);
					strWord.clear();
				}
			}
			else
			{
				strWord.push_back(pBuffer[i]);
			}
		}
		delete[] pBuffer;
	}

	std::vector<std::wstring>& GetFilterWords()
	{
		static bool bLoadFilterWords = false;
		static std::vector<std::wstring> vecFilterWords;

		if(!bLoadFilterWords)
		{
			std::wstring strSrc = Get179AppPath() + L"config\\filter.txt";
			std::wstring strDst = Get179AppPath() + L"config\\filter.dat";
			EncodeFilterWords(strSrc, strDst);

			std::wstring strPath = Get179AppPath() + L"config\\filter.dat";
			DecodeFilterWords(strPath, vecFilterWords);
			bLoadFilterWords = true;
		}

		return vecFilterWords;
	}

	std::vector<std::wstring>& GetFilterNoticeWords()
	{
		static bool bLoadFilterNoticeWords = false;
		static std::vector<std::wstring> vecFilterNoticeWords;

		if(!bLoadFilterNoticeWords)
		{
			std::wstring strSrc = Get179AppPath() + L"config\\filter_notice.txt";
			std::wstring strDst = Get179AppPath() + L"config\\filter_notice.dat";
			EncodeFilterWords(strSrc, strDst);

			std::wstring strPath = Get179AppPath() + L"config\\filter_notice.dat";
			DecodeFilterWords(strPath, vecFilterNoticeWords);
			bLoadFilterNoticeWords = true;
		}

		return vecFilterNoticeWords;
	}

	void FilterDangerousWords(std::wstring& strText)
	{
		static bool bLoadFilterName = false;
		static std::vector<std::wstring> vecFilterName;
		static std::vector<std::wstring> vecFilterChar;

		if(!bLoadFilterName)
		{
			std::wstring strSrc = Get179AppPath() + L"config\\filter_name.txt";
			std::wstring strDst = Get179AppPath() + L"config\\filter_name.dat";
			EncodeFilterWords(strSrc, strDst);

			strSrc = Get179AppPath() + L"config\\filter_char.txt";
			strDst = Get179AppPath() + L"config\\filter_char.dat";
			EncodeFilterWords(strSrc, strDst);

			std::wstring strPath = Get179AppPath() + L"config\\filter_name.dat";
			DecodeFilterWords(strPath, vecFilterName);

			strPath = Get179AppPath() + L"config\\filter_char.dat";
			DecodeFilterWords(strPath, vecFilterChar);
			vecFilterChar.push_back(L",");
			vecFilterChar.push_back(L"，");
			vecFilterChar.push_back(L" ");
			vecFilterChar.push_back(L".");
			vecFilterChar.push_back(L"。");

			bLoadFilterName = true;
		}

		//去掉字符
		std::wstring strText2 = strText;
		int size=vecFilterChar.size();
		while(size-- > 0)
		{
			common::utility::stringhelper::Replace(strText2, vecFilterChar[size], L"");
		}

		bool bContain = false;

		//扫描名字禁用库
		size=vecFilterName.size();
		while(size-- > 0)
		{
			if(strText2.find(vecFilterName[size].c_str()) != -1)
			{
				bContain=true;
				break;
			}
		}

		if(!bContain)
		{
			//扫描过滤库
			std::vector<std::wstring>& vecFilterWords = GetFilterWords();
			size=vecFilterWords.size();
			while(size-- > 0)
			{
				if(strText2.find(vecFilterWords[size].c_str()) != -1)
				{
					bContain=true;
					break;
				}
			}
		}

		if(bContain)
		{
			strText = L"***";
		}
	}

	void FilterWebCharacter(std::wstring& strText)
	{
		std::wstring words1[2] = {L"&lt;", L"&gt;"};
		std::wstring words2[2] = {L"<", L">"};

		for(int i=0;i<2;i++)
		{
			stringhelper::Replace(strText, words1[i], words2[i]);
		}
	}

	void FilterNickName(std::wstring& strText)
	{
		FilterWebCharacter(strText);
		FilterDangerousWords(strText);	
	}

	void FilterWords(std::wstring& strText)
	{
		std::vector<std::wstring>& vecFilterWords = GetFilterWords();

		int size=vecFilterWords.size();
		std::wstring strDst = L"***";
		while(size-- > 0)
		{
			common::utility::stringhelper::Replace(strText, vecFilterWords[size], strDst);
		}
	}

	void FilterNoticeWords(std::wstring& strText)
	{
		std::vector<std::wstring>& vecFilterWords = GetFilterNoticeWords();

		int size=vecFilterWords.size();
		std::wstring strDst = L"***";
		while(size-- > 0)
		{
			common::utility::stringhelper::Replace(strText, vecFilterWords[size], strDst);
		}
	}

	std::wstring GetCommandLineCommand()
	{
		char* lpCommandLine = GetCommandLineA();

		char* pPos = strchr(lpCommandLine, '\"');
		if(NULL == pPos || *(pPos+1) == 0)
		{
			return L"";
		}

		pPos = strchr(pPos+1, '\"');
		if(NULL == pPos || *(pPos+1) == 0 || *(pPos+2) == 0)
		{
			return L"";
		}

		char szCode[MAX_PATH] = {0};
		sscanf(pPos+2, "%s", szCode);

		std::string strCode = szCode;
		return stringhelper::AsciiToUnicode(strCode);
	}

	namespace imply
	{
		std::vector<DWORD> GetProcessIdImply(std::wstring const & nameExe)
		{
			std::vector<DWORD> reVec;

			std::wstring name(nameExe);
			//name.replace(L"/", L"\\");
			//name.replace('/','\\');
			boost::algorithm::replace_all(name,L"/", L"\\");

			//DWORD dwRet = 0;

			//
			std::wstring sExeFileName;
			if (name.size())
			{
				TCHAR szDrive[MAX_PATH] = {0}, szDir[MAX_PATH] = {0}, szFileTitle[MAX_PATH] = {0}, szFileExt[MAX_PATH] = {0};
				_tsplitpath(name.c_str(), szDrive, szDir, szFileTitle, szFileExt);

				sExeFileName = szFileTitle ;
				sExeFileName+= szFileExt;
			}
			else
			{
				sExeFileName = name;
			}

			//
			PROCESSENTRY32 pe = {sizeof pe};
			HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (INVALID_HANDLE_VALUE == hSnapShot)
			{
				ASSERT(FALSE);
				return reVec;
			}

			BOOL bContinue = Process32First(hSnapShot, &pe);
			while (bContinue)
			{
				
				if(boost::algorithm::iequals(sExeFileName,pe.szExeFile))
				{
					if (name.size())
					{
						HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe.th32ProcessID);

						MODULEENTRY32 me = {sizeof me};
						BOOL bContinue = Module32First(hSnapShot, &me);

						while (bContinue)
						{	
                            std::wstring name2(me.szExePath);
                            name2=common::utility::filehelper::GetLastName(name2);
							if (name==name2)
							{
								reVec.push_back(pe.th32ProcessID);
								break;
							}

							bContinue = Module32Next(hSnapShot, &me);
						}
					}
					else
					{
						reVec.push_back(pe.th32ProcessID);
					}
				}

				bContinue = Process32Next(hSnapShot, &pe);
			}

			CloseHandle(hSnapShot);

			return reVec;
		}		
	}

	/*
	* 判断某个exe有几个运行实例
	* @param [in] exe的名字，如caihong.exe
	* @return 运行实例的数目
	*/
	COMMON_INTERFACE uint32 GetProcessCount(std::wstring const & name)
	{
		return imply::GetProcessIdImply(name).size();
	}


	COMMON_INTERFACE bool RainbowProcessAlive(DWORD id)
	{
		//BOOL re;
		std::vector<DWORD> processIdVec;
		
#ifdef _DEBUG
		processIdVec=imply::GetProcessIdImply(L"caihongd.exe");
#else
		processIdVec=imply::GetProcessIdImply(L"caihong.exe");
#endif
	
		for(int i=0;i<processIdVec.size();i++)
		{
			if (processIdVec.at(i) == id)
				return true;
		}
		return false;
	}

	/*
	* 终止某个exe的所有运行实例
	* @param [in] exe的名字，如caihong.exe
	* @return 是否成功终止，最后一个成功返回TRUE，最后一个失败返回FALSE
	*/
	COMMON_INTERFACE BOOL KillProcess(std::wstring const & name)
	{
		BOOL re=FALSE;
		std::vector<DWORD> processIdVec(imply::GetProcessIdImply(name));
		for(int i=0;i<processIdVec.size();i++)
			re=boost::bind<BOOL>(&TerminateProcess,
			boost::bind<HANDLE>(&OpenProcess,PROCESS_TERMINATE, FALSE, boost::arg<1>())
			,1)(processIdVec[i]);
			
	
		return re;//...
	}

	/*
	* 从资源载入字符串
	* @param [in] 资源id名字。本函数自动将资源句柄切换到GGres.dll
	* @return 对应id的字符串。如果没有对应，返回空串
	*/
	COMMON_INTERFACE std::wstring LoadResourceString(uint32 id)
	{
		SwitchResource sr("GGRes");
		CString sRet;
		sRet.LoadString(id);
		return std::wstring((LPCTSTR)sRet);
	}

	/*
	* 获取commonlibrary.dll所在目录的路径
	* @param [in] 无
	* @return 所在路径
	*/
	COMMON_INTERFACE std::wstring  GetModulePath( )
	{
		std::wstring str_path;
		wchar_t temp[MAX_PATH+1];
		::GetModuleFileNameW(NULL, temp, MAX_PATH);
		str_path = temp;
		std::wstring::size_type index = str_path.rfind('\\');
		str_path = str_path.substr(0,index+1);

		return str_path;
	}

	/*
	* 创建目录。如果路径上任何一级目录没有都将创建
	* @param [in] 需要创建的目录路径
	* @return 如果全部创建成功返回TRUE，否则返回FALSE
	*/
#if 0
	COMMON_INTERFACE BOOL CreateAllDirectory(std::wstring const & strPathOrgin)
	{
		std::wstring strPath(strPathOrgin);
		boost::algorithm::replace_all(strPath,L"/", L"\\");

		if (strPath.size()>=2 && strPath[ strPath.size() - 2] != L'\\')
		{
			strPath.append( L"\\");
		}

		bool bRet = false;

		for(size_t nPos = 0; nPos < strPath.size(); ++nPos)
		{		
			nPos = strPath.find('\\', nPos);
			if(nPos < 0)
			{
				break;
			}
			std::wstring sDir;
			sDir.assign( strPath.c_str(), nPos);
			bRet = CreateDirectoryW(sDir.c_str(), NULL);		
		}

		return bRet;
	}
#endif

	/*
	*  判断一个键是否被按下
	* @param [in] 一个键
	* @return 是返回TRUE，否返回FALSE
	*/
	COMMON_INTERFACE BOOL IsKeyDown(int nVirtKey)
	{
		return GetKeyState(nVirtKey) < 0;
	}


	COMMON_INTERFACE BOOL IsDigit( std::wstring wstr)
	{
		for   (int  i = 0;i < wstr.length(); i++)   
		{     
			if(wstr[i] > L'9'|| wstr[i] < L'0')   
			{   
				return FALSE;   
			}   
		}   
		return TRUE; 
	}

	/*
	* 获取ie版本
	* @param [in] 无
	* @return ie版本
	*/
	COMMON_INTERFACE std::wstring GetIEVersion()
	{
		return urlopenimply::GetIEVersionImply();
	}

	/*
	* 打开一个url，使用默认浏览器，如果找不到使用ie。
	* @param [in] url地址
	* @return 如果shellexecute返回的instance不为null，返回TRUE，否则返回FALSE
	*/
	COMMON_INTERFACE BOOL OpenUrl(std::wstring const & wstrURL)
	{
		return urlopenimply::OpenUrlImply(wstrURL);
	}

	/*
	* 获取数字月份对应的农历月份字符串
	* @param [in] 数字月份，可取值范围1~12
	* @return 返回对应数字的农历月份字符串，如果没有匹配值，返回空串
	*/
	COMMON_INTERFACE std::wstring GetNongliMonthName( uint32 nMonth)
	{
		std::wstring wstrMonth;

		wchar_t * month[]={L"正",L"二",L"三",L"四",L"五",L"六",L"七",L"八",L"九",L"十",L"十一",L"腊"};

		if( nMonth >= 1 && nMonth<=12)
		{
			wstrMonth=month[nMonth-1];
			wstrMonth += L"月";
		}
		return wstrMonth;
	}

	/*
	* 获取数字日期对应的农历日期字符串
	* @param [in] 数字月份，可取值范围1~31
	* @return 返回对应数字的农历日期字符串，如果没有匹配值，返回空串
	*/
	COMMON_INTERFACE std::wstring GetNongliDayName( uint32 nDay)
	{
		std::wstring wstrDay ;
		wchar_t * day[]={ L"初一",L"初二", L"初三", L"初四",L"初五",L"初六",L"初七", L"初八", L"初九",L"初十",
		L"十一",L"十二", L"十三", L"十四",L"十五",L"十六", L"十七",L"十八", L"十九", L"二十",L"廿一", L"廿二",
		L"廿三",L"廿四", L"廿五",L"廿六", L"廿七", L"廿八",L"廿九",L"三十"};

		if(nDay>=1 && nDay<=30)
			wstrDay=day[nDay-1];
		
		return wstrDay;
	}

	
	/*
	* 调整某个句柄的SecurityLevel
	* @param [in] 要被调整的句柄
	* @return 成功返回TRUE，失败返回FALSE
	*/
	COMMON_INTERFACE BOOL SetSecurityLevel(HANDLE const & hObject)
	{
#ifndef LABEL_SECURITY_INFORMATION
#define LABEL_SECURITY_INFORMATION (0x010L)
#endif

	// LABEL_SECURITY_INFORMATION  SDDL  SACL被设为低完整性级别
	LPCWSTR LOW_INTEGRITY_SDDL_SACL_W = L"S:(ML;;NW;;;LW)";

	bool bRet = false;
	DWORD dwErr = ERROR_SUCCESS;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pSacl = NULL;
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;

	if ( ConvertStringSecurityDescriptorToSecurityDescriptorW (
		LOW_INTEGRITY_SDDL_SACL_W, SDDL_REVISION_1, &pSD, NULL ) )
	{
		if ( GetSecurityDescriptorSacl (
			pSD, &fSaclPresent, &pSacl, &fSaclDefaulted ) )
		{
			//以下代码需要新的sdk
			dwErr = SetSecurityInfo (
				hObject, SE_KERNEL_OBJECT, LABEL_SECURITY_INFORMATION,
				NULL, NULL, NULL, pSacl );

			bRet = (ERROR_SUCCESS == dwErr);
		}

		LocalFree ( pSD );
	}

	return bRet;
}



	/*
	* 接受一个组合键，返回组合键的字符串形式
	* @param [in] 一个组合键
	* @return 对应的字符串
	*/
	COMMON_INTERFACE std::wstring HotkeyToString(DWORD dwHotKey)
	{
			
		WORD wModifier = HIWORD(dwHotKey);
		WORD wKey = LOWORD(dwHotKey);

		if (wModifier == 0)
		{
			return stringhelper::Format(_T("Num %d"),dwHotKey - 96);
		}

		std::wstring strHotKey;

		if (wModifier & HOTKEYF_CONTROL )
		{
			strHotKey.append(_T("Ctrl"));
		}
		if (wModifier & HOTKEYF_SHIFT)
		{
			if (strHotKey.size()>0)
			{
				strHotKey.append(_T("+Shift"));
			}
			else
			{
				strHotKey.append(_T("Shift"));
			}
		}
		if (wModifier & HOTKEYF_ALT)
		{
			if (strHotKey.size() > 0)
			{
				strHotKey.append(_T("+Alt"));
			}
			else
			{
				strHotKey.append(_T("Alt"));
			}
		}
		std::wstring strKey(stringhelper::Format(_T("%c"),LOBYTE(wKey)));
		strHotKey.append(_T("+"));
		strHotKey.append(strKey);
		return strHotKey;
	}




	std::map<CHighPerformanceTimeCounter::ETimeCounterEvent, CHighPerformanceTimeCounter::STimePiece*> CHighPerformanceTimeCounter::m_mapTimeCounter;

CHighPerformanceTimeCounter::STimePiece* CHighPerformanceTimeCounter::GetTimePiece(ETimeCounterEvent eTCEvent)
{
	STimePiece* p = m_mapTimeCounter[eTCEvent];
	if (!p)
	{
		p = new STimePiece;
		m_mapTimeCounter[eTCEvent] = p;
	}

	return p;
}

bool CHighPerformanceTimeCounter::IsInCounting(ETimeCounterEvent eTCEvent)
{
	return m_mapTimeCounter.end() != m_mapTimeCounter.find(eTCEvent);
}

void CHighPerformanceTimeCounter::StartCountEvent(ETimeCounterEvent eTCEvent)
{
	QueryPerformanceCounter(&GetTimePiece(eTCEvent)->liStart);
}

std::wstring CHighPerformanceTimeCounter::Enum2String(ETimeCounterEvent eTCEvent)
{
	switch (eTCEvent)
	{
	case eTCEvent_Null:
		return L"eTCEvent_Null";
	case eTCEvent_PickMsgFromTray:
		return L"eTCEvent_PickMsgFromTray";
	case eTCEvent_PickMsgFromFriendList:
		return L"eTCEvent_PickMsgFromFriendList";
	case eTCEvent_PickMsgFromHotKey:
		return L"eTCEvent_PickMsgFromHotKey";
	case eTCEvent_Accumulate_processmessage_mf:
		return L"eTCEvent_Accumulate_processmessage_mf";
	default: 
		return stringhelper::Format(L"ETimeCounterEvent%d", eTCEvent);
	}

	//return L"";
}

double CHighPerformanceTimeCounter::StopCountEvent(ETimeCounterEvent eTCEvent, bool bTrace/* = true*/)
{
	if (!IsInCounting(eTCEvent))
	{
		return 0;
	}

	STimePiece* p = GetTimePiece(eTCEvent);
	QueryPerformanceCounter(&p->liEnd);

	double llEllapsed = p->liEnd.QuadPart - p->liStart.QuadPart;

	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);

	double d = (llEllapsed / liFrequency.QuadPart) * 1000 * 1000;

	if (bTrace)
	{
		wchar_t sDebug[500];
		wsprintf(sDebug, L"%s %u microsecond\n", Enum2String(eTCEvent).c_str(), (UINT)d);
		OutputDebugStringW(sDebug);
	}

	delete m_mapTimeCounter[eTCEvent];
	m_mapTimeCounter.erase(eTCEvent);

	return d;
}

void CHighPerformanceTimeCounter::Start()
{
	QueryPerformanceCounter(&m_liCounterStart);
}

double CHighPerformanceTimeCounter::Stop()
{
	LARGE_INTEGER liEnd;
	QueryPerformanceCounter(&liEnd);

	double llEllapsed = liEnd.QuadPart - m_liCounterStart.QuadPart;

	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);

	double d = (llEllapsed / liFrequency.QuadPart) * 1000 * 1000;

	return d;
}

CHighPerformanceTimeCounter::CHighPerformanceTimeCounter(wchar_t* szComment/* = NULL*/)
{
	m_eTCEventAccumulate = eTCEvent_Null;
	m_bAccumulateMode = false;
	this->szComment = szComment;

	Start();
}

CHighPerformanceTimeCounter::CHighPerformanceTimeCounter(ETimeCounterEvent eTCEvent, wchar_t* szComment/* = NULL*/)
{
	m_eTCEventAccumulate = eTCEvent;
	m_bAccumulateMode = true;
	StartAccumulateCountEvent(eTCEvent, szComment);
}

void CHighPerformanceTimeCounter::StartAccumulateCountEvent(ETimeCounterEvent eTCEvent, wchar_t* szComment/* = NULL*/)
{
	QueryPerformanceCounter(&GetTimePiece(eTCEvent)->liStart);
}

double CHighPerformanceTimeCounter::StopAccumulateCountEvent(ETimeCounterEvent eTCEvent, bool bTrace/* = true*/)
{
	if (!IsInCounting(eTCEvent))
	{
		return 0;
	}

	STimePiece* p = GetTimePiece(eTCEvent);
	QueryPerformanceCounter(&p->liEnd);

	double llEllapsed = p->liEnd.QuadPart - p->liStart.QuadPart;

	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);

	double d = (llEllapsed / liFrequency.QuadPart) * 1000 * 1000;

	p->dEllapse = d;
	p->dEllapseTotal += d;
	p->cnt++;

	if (bTrace)
	{
		wchar_t sDebug[500];
		wsprintf(sDebug, L"%s %u microsecond, totaltime %d microsecond, 第%d次\n", Enum2String(eTCEvent).c_str(), (UINT)p->dEllapse, (UINT)p->dEllapseTotal, p->cnt);
		OutputDebugStringW(sDebug);
	}

	return p->dEllapseTotal;
}

CHighPerformanceTimeCounter::~CHighPerformanceTimeCounter()
{
	if (m_bAccumulateMode)
	{
		StopAccumulateCountEvent(m_eTCEventAccumulate);
	}
	else
	{
		double d = Stop();
		UINT u = d;

		if (szComment)
		{
			wchar_t sDebug[500];
			wsprintf(sDebug, L"%s %u microsecond\n", szComment, u);
			OutputDebugStringW(sDebug);
		}
	}
}

int CALLBACK MyEnumFontProc( ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* lpntm, DWORD nFontType, LONG lParam)
{
	if(lParam == 0)
	{
		return 1;
	}

	std::vector<std::wstring>* pvecResult = (std::vector<std::wstring>*)lParam ;
	
	if ( !__isascii( lpelf->elfLogFont.lfFaceName[0]) )
	{
		pvecResult->push_back(std::wstring(lpelf->elfLogFont.lfFaceName));
	}

	return 1;
}

void GetChineseFontNameList(std::vector<std::wstring>& vector)
{
	static std::vector<std::wstring> s_vector;

	if(s_vector.size() == 0)
	{
		LOGFONT lf = {0};
		lf.lfCharSet = GB2312_CHARSET ;
		EnumFontFamiliesEx(CClientDC(CWnd::GetDesktopWindow()), &lf, (FONTENUMPROC)MyEnumFontProc, (LPARAM)&s_vector, 0) ;
	}

	vector = s_vector;	
}

void GetDefaultFontName(std::wstring& defaulName)
{
	static std::wstring s_defaulName = L"";

	if(s_defaulName.empty())
	{
		std::vector<std::wstring> vector;
		GetChineseFontNameList(vector);

		for(std::vector<std::wstring>::iterator it = vector.begin();
			it != vector.end(); ++it)
		{
			if((*it).compare(L"微软雅黑") == 0)
			{
				s_defaulName = L"微软雅黑";
				break;
			}
		}

		if(s_defaulName.empty())
		{
			s_defaulName = L"宋体";
		}
	}

	defaulName = s_defaulName;	 
}

void CopyDataToClipbord( std::wstring strText )  
{
	bool flag = false ;
	COleDataSource* pSource = new COleDataSource ;
	if ( pSource != NULL )
	{
		if(strText.length() > 0)
		{

			CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
			sf.Write(strText.c_str(), 2*strText.length());

			HGLOBAL hMem = sf.Detach();
			if(hMem == NULL) return;

			pSource->CacheGlobalData(CF_UNICODETEXT, hMem);

			flag = true;
		}

		if(flag)
		{
			pSource->SetClipboard();
		}
	}
}
	namespace imply
	{
		/// Gb2312 charset range check ///////////////////////////////////////////////////////////////////////

		BOOL IsGB2312BytePre(BYTE byToCheck)
		{
			return (byToCheck >= 0xa1) && (byToCheck < 0xb0);
		}

		BOOL IsGB2312ByteMid(BYTE byToCheck)
		{
			return (byToCheck >= 0xb0) && (byToCheck <= 0xf7);
		}

		BOOL IsGB2312BytePost(BYTE byToCheck)
		{
			return (byToCheck > 0xf7) && (byToCheck <= 0xfe);
		}

		/// LeadByte 是 FollowByte 的字集
		BOOL IsGB2312LeadByte(BYTE byToCheck)
		{
			return IsGB2312ByteMid(byToCheck);
		}

		BOOL IsGB2312FollowByte(BYTE byToCheck)
		{
			return IsGB2312BytePre(byToCheck) || IsGB2312ByteMid(byToCheck) || IsGB2312BytePost(byToCheck);
		}

		BOOL IsGB2312Byte(BYTE byToCheck)
		{
			return IsGB2312LeadByte(byToCheck) || IsGB2312FollowByte(byToCheck);
		}

		BOOL IsGB2312Byte_Strictly(BYTE byToCheck, BYTE byBefore/* = 0*/, BYTE byAfter/* = 0*/)
		{
			BOOL bRet = false;

			if (IsGB2312BytePre(byToCheck) || IsGB2312BytePost(byToCheck))	// low byte
			{
				bRet = IsGB2312LeadByte(byBefore);
			}
			else if (IsGB2312ByteMid(byToCheck))
			{
				bRet = IsGB2312FollowByte(byBefore) || IsGB2312FollowByte(byAfter);
			}
			else
			{
				//
			}

			return bRet;
		}
	}

	char GetFirstPinYin(const char * cs)
		{
			static const char str[26][3]={"啊","芭","擦","搭","蛾","发","噶","哈","\1","击","喀","垃","妈","拿","哦","啪","期","然","撒","塌","\1","\1","挖","昔","压","匝"};
			static const char SpecialWords[][4] = {"缪m", "闵m"};

			if(cs==NULL) return 0;

			if(cs[0]>0)
			{
				char c=0;
				if(cs[0]>=65 && cs[0]<=90) c=cs[0]+32;
				else if(cs[0]>=97 && cs[0]<=122) c=cs[0];

				return c;
			}

			for(int i=1;i<26;i++)
			{
				if(str[i][0]!=1)
				{
					for (int k = 0; k < (sizeof(SpecialWords) / sizeof(SpecialWords[0])); k++)
					{
						if (strncmp(cs, SpecialWords[k], 2) == 0)
						{
							return *(SpecialWords[k] + 2);
						}
					}

					if (!imply::IsGB2312LeadByte(cs[0]))
					{
						return '`';
					}
					if(memcmp(cs,str[i],2)<0)
					{
						while(str[i-1][0]==1) i--;
						return 'a'+(char)(i-1);
					}
				}
			}
			return 'z';
		}

	
}
}
}