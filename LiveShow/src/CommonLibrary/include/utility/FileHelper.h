/*
@file:		FileHelper.h
@purpose:	文件的帮助函数等
@version:	1.0 2007.3.28 create zhangjb
*/

#pragma once

#include <string>
#include <vector>
#include "../Common.h"

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#include "winver.h"
#pragma comment(lib, "version.lib")

namespace common 
{
	namespace utility
	{
		namespace filehelper{
		/*
		/*function 获得路径上的最后一个文件/目录名
		/*@param [in] sPath  输入的全路径
		/*return 返回值为文件最后一个文件/目录名
		*/
		COMMON_INTERFACE std::wstring GetLastName(const std::wstring& sPath);

		/*
		/*function 获取文件扩展名
		/*@param [in] sPath 输入的文件全路径
		/*return 返回文件的扩展名
		*/
		COMMON_INTERFACE std::wstring GetFileExt(const std::wstring& sPath);

		
		/*
		/*function  获得上级父目录
		/*@param [in] sPath 输入的文件全路径
		/*@param [out] pCStringLastName 返回最后一个文件名/目录
		/*return 返回上级目录的路径
		*/
		COMMON_INTERFACE std::wstring GetParentDir(const std::wstring& sPath, std::wstring* pCStringLastName = NULL);

		
		/*
		/*function 获得当前进程的exe文件路径
		/*@param [out] 返回当前进程的exe的文件名
		/*return 返回当前进程的exe的文件路径
		*/
		COMMON_INTERFACE std::wstring GetCurPath(std::wstring* pCStringFileName = NULL);

		/*
		/*function 获得当前进程的exe文件所在目录
		/*@param [out] 返回当前进程的exe的文件名
		/*return 返回当前进程的exe所在的目录
		*/
		COMMON_INTERFACE std::wstring GetCurDir(std::wstring* pCStringDirName = NULL);

		/*
		/*function 文件/目录是否存在
		/*@param [in] szFilePath 文件/目录的路径
		/*return 存在返回TRUE，反之返回FALSE
		*/
		COMMON_INTERFACE BOOL IsExist(LPCTSTR szFilePath);

		/*
		/*function 目录是否存在
		/*@param [in] szFilePath 目录的路径
		/*return 存在返回TRUE，反之返回FALSE
		*/
		COMMON_INTERFACE BOOL IsDirExist(LPCTSTR szFilePath);

		/*
		/*function 文件是否存在
		/*@param [in] szFilePath 文件路径
		/*return 存在返回TRUE，反之返回FALSE
		*/
		COMMON_INTERFACE BOOL IsFileExist(LPCTSTR szFilePath);

		/*
		/*function 创建此路径上的所有目录
		/*@param [in] strPath 目录路径
		/*return 创建成功返回TRUE,反之返回FALSE
		*/
		COMMON_INTERFACE BOOL CreateAllDirectory(std::wstring strPath);

		//
		/*
		/*function 获取系统目录临时路径，注意，是短路径
		/*@param void
		/*return 返回系统临时目录的路径
		*/
		COMMON_INTERFACE std::wstring GetSysTempDir();

		/*
		/*function 删除目录（包括目录下的所有文件和目录）
		/*@param [in] sDir 需要删除的目录路径
		/*return 删除成功返回TRUE,反之返回FALSE
		*/
		COMMON_INTERFACE BOOL DeleteDir(const std::wstring& sDir);


		/*
		/*function 文件是否可读(即是否被别的程序占用)
		/*@param [in] szFilePath 文件全路径
		/*return 文件可读返回TRUE,反之返回 FALSE
		*/
		COMMON_INTERFACE BOOL IsFileReadable(LPCTSTR szFilePath);

		/*
		/*function 文件是否可写(即是否被别的程序占用)
		/*@param [in] szFilePath 文件全路径
		/*return 文件可写返回TRUE,反之返回 FALSE
		*/
		COMMON_INTERFACE BOOL IsFileWriteable(LPCTSTR szFilePath);

		/*
		/*function 获取文件的大小(单位:字节，获取的大小在0xFFFFFFFF之内)
		/*@param [in] szFilePath 文件路径
		/*@param [out] pdwHigh 文件大小的高端部分值,可以为NULL
		/*return 返回文件的大小(单位:字节)
		*/
		COMMON_INTERFACE DWORD GetFileSize(LPCTSTR szFilePath, DWORD* pdwHigh = NULL);

		/*
		/*function 获取目录的大小（目录中所有文件大小的和）
		/*@param [in] sDirPath 目录路径
		/*return 返回目录的大小(单位：字节)
		*/
		COMMON_INTERFACE DWORD GetDirSize(const std::wstring& sDirPath);

		/*
		/*function 获取文件的创建时间，最后修改时间，最后访问时间 
		/*@param [in] 文件路径
		/*@param [out] lpCreateTime 文件的创建时间
		/*@param [out] lpLastAccessTime 文件的访问时间
		/*@param [out] lpLastWriteTime 文件的修改时间
		/*return 成功返回TRUE,失败返回FALSE
		*/
		COMMON_INTERFACE BOOL  GetFileTime(const std::wstring& sPath, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);

		/*
		/*function 设置文件的创建时间，最后修改时间，最后访问时间 
		/*@param [in] 文件路径
		/*@param [in] lpCreateTime 文件的创建时间
		/*@param [in] lpLastAccessTime 文件的访问时间
		/*@param [in] lpLastWriteTime 文件的修改时间
		/*return 成功返回TRUE,失败返回FALSE
		*/
		COMMON_INTERFACE BOOL  SetFileTime(const std::wstring& sPath, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);

		/*
		/*function 获取文件的ICON图标 
		/*@param [in] 文件路径
		/*@param [in] 是否是目录
		/*return 成功返回文件ICON的句柄，失败返回FALSE
		*/
		COMMON_INTERFACE HICON GetFileIcon(LPCTSTR szFilePath,BOOL bDir);

		/*
		/*function 将数据写入文件
		/*@param [in] pBuf 数据的内存起始地址
		/*@param [in] nBufLen 数据的长度(单位:字节)
		/*@param [in] sFile   文件路径
		/*return 成功返回TRUE,失败返回FALSE
		*/
		COMMON_INTERFACE BOOL  WriteFile(BYTE* pBuf, int nBufLen, const std::wstring& sFile);


		//////////////////////////////////////////////////////////////////////////
		enum Mode
		{
			Exist=00,
			Write=02,
			Read=04,
			ReadWrite=06,
		};

		/*
		/*function 判断文件是否具有的指定的访问属性 
		/*@param [in] path 文件路径  (宽字符)
		/*@param [in] mode 文件访问属性
		/*return 如果具有指定的属性，返回true,反之返回false
		*/
		COMMON_INTERFACE bool FileAccessW(std::wstring const &path,int mode);
		
		/*
		/*function 判断文件是否具有的指定的访问属性 
		/*@param [in] path 文件路径 
		/*@param [in] mode 文件访问属性
		/*return 如果具有指定的属性，返回true,反之返回false
		*/
		COMMON_INTERFACE bool FileAccessA(std::string const &path,int mode);


		/*
		/*function 从文件中读取数据
		/*@param [in] name 文件路径
		/*@param [out] vec 存放数据的容器 ,注意:要读取数据的长度=vec.size()
		/*@param [out] realRead 实际读取数据的大小(单位：字节)
		/*@param [in] offset  文件数据的起始位置(单位:字节)
		/*return 成功返回COMMON_SUCCESS ，失败返回COMMON_FAILED
		*/
		COMMON_INTERFACE CommonResult ReadFileW(std::wstring const & name,std::vector<char> & vec,DWORD & realRead,uint32 offset=0);
		
		/*
		/*function 写入数据到文件
		/*@param [in] name 文件路径
		/*@param [in] vec 存放数据的容器 ，注意：要写入数据的长度=vec.size()
		/*@param [out] realRead 实际写入数据的大小(单位：字节)
		/*@param [in] offset  文件数据的起始位置(单位:字节)
		/*return 成功返回COMMON_SUCCESS ，失败返回COMMON_FAILED
		*/
		COMMON_INTERFACE CommonResult WriteFileW(std::wstring const & name,std::vector<char> const & vec,DWORD & realWrite,uint32 offset=0);
		
		/*
		/*function 从文件中读取数据
		/*@param [in] hFile 文件句柄
		/*@param [out] vec 存放数据的容器 ,注意:要读取数据的长度=vec.size()
		/*@param [out] realRead 实际读取数据的大小(单位：字节)
		/*@param [in] offset  文件数据的起始位置(单位:字节)
		/*return 成功返回COMMON_SUCCESS ，失败返回COMMON_FAILED
		*/
		COMMON_INTERFACE CommonResult ReadFileH(HANDLE hFile,std::vector<char> & vec,DWORD & realRead,uint32 offset);
		
		/*
		/*function 写入数据到文件
		/*@param [in] hFile 文件句柄
		/*@param [in] vec 存放数据的容器,注意：要写入数据的长度=vec.size() 
		/*@param [out] realRead 实际写入数据的大小(单位：字节)
		/*@param [in] offset  文件数据的起始位置(单位:字节)
		/*return 成功返回COMMON_SUCCESS ，失败返回COMMON_FAILED
		*/
		COMMON_INTERFACE CommonResult WriteFileH(HANDLE hFile,std::vector<char> const & vec,DWORD & realWrite,uint32 offset);
		
		/*
		/*function 获取文件的相关信息
		/*@param [in] name 文件路径
		/*@param [out] BY_HANDLE_FILE_INFORMATION_ 文件的信息
		/*return  成功返回COMMON_SUCCESS ，失败返回COMMON_FAILED
		*/
		COMMON_INTERFACE CommonResult GetFileInformationW(std::wstring name ,BY_HANDLE_FILE_INFORMATION & BY_HANDLE_FILE_INFORMATION_);

		COMMON_INTERFACE CommonResult GetFileInformationH(HANDLE hFile, BY_HANDLE_FILE_INFORMATION & BY_HANDLE_FILE_INFORMATION_);

		COMMON_INTERFACE bool CreateUnicodeFile(LPCTSTR szFilePath,bool arways = false);

		//FILETIME 转 time_t
		COMMON_INTERFACE void FileTimeToUinxTime(LPFILETIME pft,time_t *t); 

	    //time_t 转 FILETIME
	    COMMON_INTERFACE void UinxTimeToFileTime(time_t t,LPFILETIME pft);

	}
	}
}

