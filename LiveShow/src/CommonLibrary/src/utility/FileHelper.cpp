
#include "utility\FileHelper.h"
#include <assert.h>
#include <io.h>

namespace common 
{
	namespace utility
	{
		namespace filehelper{

		// 获得路径上的最后一个文件/目录名
		std::wstring GetLastName(const std::wstring& sPath)
		{
			std::wstring sRet(L"");
			GetParentDir(sPath,&sRet);
			return sRet;
		}

		// 获取文件扩展名
		std::wstring GetFileExt(const std::wstring& sPath)
		{
			return PathFindExtension(sPath.c_str());
		}

		/// 获得上级父目录
		std::wstring GetParentDir(const std::wstring& sPath, std::wstring* pCStringLastName )	
		{
			std::wstring sRet(L"");
			if(sPath.length()==0)
			{
				return sRet;
			}
			std::wstring::size_type index=-1;
			if((index=sPath.rfind(L"\\")) < 0)
			{
				index=sPath.rfind(L"/");
			}
			if(index>=0)
			{
				if(pCStringLastName!=NULL && index<(sPath.length()-1))
				{
					*pCStringLastName=sPath.substr(index+1,sPath.length()-1);
				}
				sRet=sPath.substr(0,index);
			}
			return sRet;

		}

		/// 获得当前进程的exe文件路径
		std::wstring GetCurPath(std::wstring* pCStringFileName )
		{
			WCHAR szPath[MAX_PATH]={0};
			GetModuleFileName(NULL, szPath, MAX_PATH);

			std::wstring sRet(szPath);

			if (pCStringFileName)
			{
				*pCStringFileName = GetLastName(sRet);
			}
			return sRet;
			
		}

		BOOL IsExist(LPCTSTR szFilePath)
		{
			return PathFileExists(szFilePath);
		}

		BOOL IsDirExist(LPCTSTR szFilePath)
		{
			return PathIsDirectory(szFilePath);
		}

		BOOL IsFileExist(LPCTSTR szFilePath)
		{
			return IsExist(szFilePath) && !IsDirExist(szFilePath);
		}

		/// 创建此路径上的所有目录
		BOOL CreateAllDirectory(std::wstring strPath)
		{
			if(strPath.length()==0)
			{
				return FALSE;
			}
			for (size_t i=0;i<strPath.length();i++)
			{
				std::wstring::reference r=strPath.at(i);
				if(r==L'/')
				{
					r=L'\\';
				}
			}
			if(strPath.at(strPath.length()-1)!=L'\\')
			{
				strPath+=L"\\";
			}

			std::wstring::size_type nPos=0;
			for(nPos = 0; nPos < strPath.length(); ++nPos)
			{
				nPos=strPath.find(L"\\",nPos);
				if(nPos<0)
				{
					break;
				}
				std::wstring strDir=strPath.substr(0,nPos);
				CreateDirectory(strDir.c_str(),NULL);
			}
			return TRUE;
		}

		std::wstring GetSysTempDir()
		{
			wchar_t szTempDir[MAX_PATH+1] = {0};
			::GetTempPath(MAX_PATH, szTempDir);

			wchar_t szTempLongDir[1024] = {0};
			GetLongPathName(szTempDir, szTempLongDir, 1024);

			return szTempLongDir;
		}

		BOOL DeleteDir(const std::wstring& sDir)
		{
			std::wstring sDirectory=sDir;
			if(sDirectory.at(sDirectory.length()-1)!=L'\\')
			{
				sDirectory=sDirectory.append(L"\\");
			}
			sDirectory=sDirectory.append(L"*.*");

			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;

			hFind = FindFirstFile(sDirectory.c_str(), &FindFileData);

			if (hFind == INVALID_HANDLE_VALUE) 
			{
				return TRUE;
			} 
			
			while (FindNextFile(hFind, &FindFileData) != 0)
			{
				bool bIsDirectory=((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0);
				if(bIsDirectory && (wcscmp(FindFileData.cFileName,L".")==0 || wcscmp(FindFileData.cFileName,L"..")==0))
				{
					continue;
				}
				
				std::wstring sFileName=sDir;
				if(sFileName.at(sFileName.length()-1)!=L'\\')
				{
					sFileName=sFileName.append(L"\\");
				}
				sFileName.append(FindFileData.cFileName);

				if(bIsDirectory)
				{
					if(!DeleteDir(sFileName.c_str()))
					{
						return FALSE;
					}
					if (!RemoveDirectory(sFileName.c_str()))
					{
						 return FALSE;
					}
				}
				else
				{
					if(!DeleteFile(sFileName.c_str()))
					{
						return FALSE;
					}
				}

			}
			FindClose(hFind);
			return TRUE;
		}
		
		BOOL IsFileWriteable(LPCTSTR szFilePath)
		{
			if (!IsFileExist(szFilePath))
			{
				return FALSE;
			}

			HANDLE hFile = CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return FALSE;
			}
			else
			{
				CloseHandle(hFile);
			}
			return TRUE;
		}

		BOOL IsFileReadable(LPCTSTR szFilePath)
		{
			if (!IsFileExist(szFilePath))
			{
				return FALSE;
			}

			HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return FALSE;
			}
			else
			{
				CloseHandle(hFile);
			}
			return TRUE;
		}

		DWORD GetFileSize(LPCTSTR szFilePath, DWORD* pdwHigh/* = NULL*/)
		{
			DWORD dwRet = -1;

			HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return 0;
			}
			else
			{
				dwRet = ::GetFileSize(hFile, pdwHigh);
				if (INVALID_FILE_SIZE == dwRet)
				{
					//DWORD dwError=GetLastError();
				}

				CloseHandle(hFile);
			}

			return dwRet;
		}

		DWORD GetDirSize(const std::wstring& sDirPath)
		{
			DWORD dwRet = 0;

			std::wstring sDirectory=sDirPath;
			if(sDirectory.at(sDirectory.length()-1)!=L'\\')
			{
				sDirectory=sDirectory.append(L"\\");
			}
			sDirectory=sDirectory.append(L"*.*");

			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;

			hFind = FindFirstFile(sDirectory.c_str(), &FindFileData);

			if (hFind == INVALID_HANDLE_VALUE) 
			{
				return 0;
			} 

			while (FindNextFile(hFind, &FindFileData) != 0)
			{
				bool bIsDirectory=((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0);
				if(bIsDirectory && (wcscmp(FindFileData.cFileName,L".")==0 || wcscmp(FindFileData.cFileName,L"..")==0))
				{
					continue;
				}

				std::wstring sFileName=sDirPath;
				if(sFileName.at(sDirPath.length()-1)!=L'\\')
				{
					sFileName=sFileName.append(L"\\");
				}
				sFileName.append(FindFileData.cFileName);

				if(bIsDirectory)
				{
					dwRet+=GetDirSize(sFileName.c_str());
				}
				else
				{
					dwRet+=GetFileSize(sFileName.c_str());
				}

			}
			FindClose(hFind);

			return dwRet;
		}

		BOOL GetFileTime(const std::wstring& sPath, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
		{
			BOOL bRet = FALSE;

			HANDLE hFile = ::CreateFile(sPath.c_str(),
				GENERIC_READ,          // 
				FILE_SHARE_READ,     //
				NULL,                   // 缺省安全属性。
				OPEN_EXISTING,          // 
				FILE_ATTRIBUTE_NORMAL, // 一般的文件。      
				NULL);                 // 模板文件为空。

			if (hFile != INVALID_HANDLE_VALUE)
			{
				::GetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
				CloseHandle(hFile);

				bRet = TRUE;
			}

			return bRet;
		}

		BOOL  SetFileTime(const std::wstring& sPath, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
		{
			WIN32_FIND_DATA FindFileData;	

			HANDLE hFile = CreateFile(sPath.c_str(), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return FALSE ;
			}
			else
			{	
				
				SetFileTime(hFile,lpCreationTime,lpLastAccessTime,lpLastWriteTime);
				CloseHandle(hFile);
				return TRUE;

			}
		}

		HICON GetFileIcon(LPCTSTR szFilePath,BOOL bDir)
		{
			std::wstring strExt(L"");
			if (! bDir)
			{
				std::wstring strTmp = szFilePath;
				std::wstring::size_type dotPos = strTmp.rfind(L'.');
				if (dotPos!=std::wstring::npos)
				{
					strExt.append(L"*");
					strExt.append( strTmp.substr(dotPos,strTmp.length()-dotPos));

				}
			}
			SHFILEINFO fileInfo = {0};
			if (
				!SHGetFileInfo(strExt.c_str(),
				0,
				&fileInfo,
				sizeof(fileInfo),
				SHGFI_LARGEICON | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES
				)
				)
				return NULL;

			return fileInfo.hIcon;
		}

		BOOL  WriteFile(BYTE* pBuf, int nBufLen, const std::wstring& sFile)
		{
			uint32 result = TRUE;
			//
			HANDLE hFile = ::CreateFile(sFile.c_str(),
				GENERIC_WRITE,          // 
				FILE_SHARE_READ,        // 不共享写
				NULL,                   // 缺省安全属性。
				OPEN_ALWAYS,/*OPEN_EXISTING*/          // 
				FILE_ATTRIBUTE_NORMAL,  // 一般的文件。      
				NULL);                  // 模板文件为空。

			if (hFile == INVALID_HANDLE_VALUE)
			{
				assert(0);
				goto error;
			}

			DWORD realWrite;
			BOOL re= ::WriteFile(hFile,pBuf,nBufLen,&realWrite,NULL);
			FlushFileBuffers(hFile);

			if(re==TRUE)
			{
				goto final;
			}

	error:
			result= FALSE;
	final:
			::CloseHandle(hFile);
			return result;
		}

		//CFileHelper
		bool FileAccessW(std::wstring const & path,int mode)
		{
			return _waccess( path.c_str(), mode)==0?true:false ;
		}
		bool FileAccessA(std::string const & path,int mode)
		{
			return _access( path.c_str(), mode)==0?true:false ;
		}


		CommonResult ReadFileW(std::wstring const & name,std::vector<char> & vec,DWORD & realRead,uint32 offset)
		{
			CommonResult result = COMMON_SUCCESS;
			//
			HANDLE hFile = ::CreateFile(name.c_str(),
				GENERIC_READ,          // 
				FILE_SHARE_READ,     //
				NULL,                   // 缺省安全属性。
				OPEN_EXISTING,          // 
				FILE_ATTRIBUTE_NORMAL, // 一般的文件。      
				NULL);                 // 模板文件为空。

			if (hFile == INVALID_HANDLE_VALUE)
			{
				assert(0);
				goto error;
			}


			DWORD dwPtr = SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
			if (dwPtr == INVALID_SET_FILE_POINTER)
			{
				assert(0);
				goto error;
			}
			BOOL re= ::ReadFile(hFile,&vec[0],vec.size(),&realRead,NULL);
			if(re==TRUE)
			{
				goto final;
			}

	error:
			result= COMMON_FAILED ;
	final:
			::CloseHandle(hFile);
			return result;
		}

		CommonResult WriteFileW(std::wstring const & name,std::vector<char> const & vec,DWORD & realWrite,uint32 offset)
		{
			CommonResult result = COMMON_SUCCESS;
			//
			HANDLE hFile = ::CreateFile(name.c_str(),
				GENERIC_WRITE,          // 
				FILE_SHARE_READ,        // 不共享写
				NULL,                   // 缺省安全属性。
				OPEN_EXISTING,/*OPEN_ALWAYS*/          // 
				FILE_ATTRIBUTE_NORMAL,  // 一般的文件。      
				NULL);                  // 模板文件为空。

			if (hFile == INVALID_HANDLE_VALUE)
			{
				assert(0);
				goto error;
			}

			DWORD dwPtr = SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
			if (dwPtr == INVALID_SET_FILE_POINTER)
			{
				assert(0);
				goto error;
			}

			BOOL re= ::WriteFile(hFile,&vec[0],realWrite/*vec.size()*/,&realWrite,NULL);
			FlushFileBuffers(hFile);

			if(re==TRUE)
			{
				goto final;
			}

	error:
			result= COMMON_FAILED ;
	final:
			::CloseHandle(hFile);
			return result;
		}



		CommonResult ReadFileH(HANDLE hFile,std::vector<char> & vec,DWORD & realRead,uint32 offset)
		{//以句柄打开的话，不能关句柄，由调用者管理句柄
			CommonResult result = COMMON_SUCCESS;

			if (hFile == INVALID_HANDLE_VALUE)
			{
				assert(0);
				goto error;
			}


			DWORD dwPtr = SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
			if (dwPtr == INVALID_SET_FILE_POINTER)
			{
				assert(0);
				goto error;
			}
			BOOL re= ::ReadFile(hFile,&vec[0],vec.size(),&realRead,NULL);
			if(re==TRUE)
			{
				goto final;
			}

	error:
			result= COMMON_FAILED;
	final:
			return result;
		}

		CommonResult WriteFileH(HANDLE hFile,std::vector<char> const & vec,DWORD & realWrite,uint32 offset)
		{//以句柄打开的话，不能关句柄，由调用者管理句柄
			CommonResult result = COMMON_SUCCESS;

			if (hFile == INVALID_HANDLE_VALUE)
			{
				assert(0);
				goto error;
			}

			DWORD dwPtr = SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
			if (dwPtr == INVALID_SET_FILE_POINTER)
			{
				assert(0);
				goto error;
			}

			BOOL re= ::WriteFile(hFile,&vec[0],vec.size(),&realWrite,NULL);
			FlushFileBuffers(hFile);

			if(re==TRUE)
			{
				goto final;
			}

	error:
			result= COMMON_FAILED;
	final:
			return result;
		}

		CommonResult GetFileInformationW(std::wstring name ,BY_HANDLE_FILE_INFORMATION & BY_HANDLE_FILE_INFORMATION_)
		{
			CommonResult result = COMMON_SUCCESS;
			std::string md5;
			HANDLE hFile = ::CreateFile(name.c_str(),
				GENERIC_READ,         
				FILE_SHARE_READ,    
				NULL,                   
				OPEN_EXISTING,         
				FILE_ATTRIBUTE_NORMAL,    
				NULL);                 

			if (hFile == INVALID_HANDLE_VALUE)
			{
				assert(0);
				goto error;
			}

			BOOL re=GetFileInformationByHandle(hFile,&BY_HANDLE_FILE_INFORMATION_);
			if(re!=TRUE)
			{
				assert(0);
				goto error;
			}
			else
			{
				goto final;
			}

	error:
			result= COMMON_FAILED;
	final:
			::CloseHandle(hFile);
			return result;

		}


		 CommonResult GetFileInformationH(HANDLE hFile, BY_HANDLE_FILE_INFORMATION & BY_HANDLE_FILE_INFORMATION_)
		 {
			 CommonResult result = COMMON_SUCCESS;

			 if (hFile == INVALID_HANDLE_VALUE)
			 {
				 assert(0);
				 goto error;
			 }

			 BOOL re=GetFileInformationByHandle(hFile,&BY_HANDLE_FILE_INFORMATION_);
			 if(re!=TRUE)
			 {
				 assert(0);
				 goto error;
			 }
			 else
			 {
				goto final;
			 }

error:
			 result= COMMON_FAILED;
final:
			 return result;
		 }


		 //创建Unicode编码文件，
		 // arways 为true时覆盖已有文件，否则不覆盖
		 bool CreateUnicodeFile(LPCTSTR szFilePath,bool arways)
		 {
			 bool result = true;
			 DWORD openArways;
			 if(arways)
			 {
				 openArways = CREATE_ALWAYS;
			 }
			 else
			 {
				 openArways = OPEN_ALWAYS; //如果文件不存在则创建
			 }
			 HANDLE hFile = ::CreateFile(szFilePath,
				 GENERIC_WRITE,          // 
				 FILE_SHARE_READ,        // 不共享写
				 NULL,                   // 缺省安全属性。
				 openArways,		     //
				 FILE_ATTRIBUTE_NORMAL,  // 一般的文件。      
				 NULL);                  // 模板文件为空。

			 if (hFile == INVALID_HANDLE_VALUE)   
			 {
				 assert(0);
				 result = false;
			 }

			 WORD wSignature = 0xFEFF;

			 DWORD wlen = 2;
			 BOOL re= ::WriteFile(hFile,&wSignature,wlen,&wlen,NULL);
			 FlushFileBuffers(hFile);

			 if(re==TRUE)
			 {
				result = true;
			 }
			 else
			 {
				 result = false;
			 }

			 ::CloseHandle(hFile);
			 return result;


		 }

		 //FILETIME 转 time_t
		 void FileTimeToUinxTime(LPFILETIME pft,time_t *t)
		 {
			 LONGLONG ll;
			 ULARGE_INTEGER ui;
			 ui.LowPart = pft->dwLowDateTime;
			 ui.HighPart = pft->dwHighDateTime;

			 ll = pft->dwHighDateTime << 32 + pft->dwLowDateTime;
			 *t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
		 }

		 //time_t 转 FILETIME
		 void UinxTimeToFileTime(time_t t,LPFILETIME pft)
		 {
			 LONGLONG ll;
			 ll = Int32x32To64(t,10000000) + 116444736000000000;
			 pft->dwLowDateTime = (DWORD)ll;
			 pft->dwHighDateTime = (DWORD)(ll >> 32);
		 }


	}
	}
}

