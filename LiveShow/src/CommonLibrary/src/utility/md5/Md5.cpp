#include "base/md5.h"
#include <string>
#include "utility\md5.h"
#include "utility/FileHelper.h"


namespace common
{
namespace utility
{
namespace md5
{
	namespace imply
	{

		template <typename T>
		struct CallReadFile
		{
		};

		template <>
		struct CallReadFile<std::wstring >
		{
			typedef CommonResult (*ReadFile)(std::wstring const & ,std::vector<char> & ,DWORD & ,uint32 ) ;
			static ReadFile pReadFile;
		};

		template <>
		struct CallReadFile<HANDLE >
		{
			typedef CommonResult (*ReadFile)(HANDLE ,std::vector<char> & ,DWORD & ,uint32 ) ;
			static ReadFile pReadFile;
		};

		CallReadFile<std::wstring >::ReadFile CallReadFile<std::wstring >::pReadFile=filehelper::ReadFileW;
		CallReadFile<HANDLE >::ReadFile CallReadFile<HANDLE>::pReadFile=filehelper::ReadFileH;

		template <typename T>
		std::string GetFileMd5Imply(T const & file,std::vector<char> & content,uint32 offset)
		{
			DWORD realRead;
			if(CallReadFile<T>::pReadFile(file,content,realRead,offset)==COMMON_SUCCESS)
			{
				MD5Digest digest;
				MD5Sum(&content[0], content.size(), &digest);
				return MD5DigestToBase16(digest);
			}
			else
				assert(0);
		
		return std::string();
		}
	}
	std::string GetStringMd5(std::string const & s)
	{
		return MD5String(s);
	}

	std::string GetBufferMd5(void const * p,uint32 size)
	{	
		if(p==NULL || size==0)
		{
			assert(0);
			return std::string();
		}

		MD5Digest digest;
		MD5Sum(p, size, &digest);
		return MD5DigestToBase16(digest);
	}

	std::string GetFileMd5(std::wstring const & file)
	{
		std::vector<char> content;
		//DWORD realRead;

		BY_HANDLE_FILE_INFORMATION bY_HANDLE_FILE_INFORMATION;
		if(filehelper::GetFileInformationW(file,bY_HANDLE_FILE_INFORMATION)==COMMON_SUCCESS)
		{
			content.resize(bY_HANDLE_FILE_INFORMATION.nFileSizeLow);//只取low，没取high，本函数不适合4g以上文件。。。
			return imply::GetFileMd5Imply(file,content,0);
		}
		else
			assert(0);

		return std::string();
	}

	std::string GetFileMd5(std::wstring const & file ,uint32 offset,uint32 size)
	{
		std::vector<char> content;
		//DWORD realRead;

		content.resize(size);//本函数不适合4g以上文件。。。
		return imply::GetFileMd5Imply(file,content,offset);

	}


	std::string GetFileMd5(HANDLE hFile)
	{
		std::vector<char> content;
		//DWORD realRead;

		BY_HANDLE_FILE_INFORMATION bY_HANDLE_FILE_INFORMATION;
		if(filehelper::GetFileInformationH(hFile,bY_HANDLE_FILE_INFORMATION)==COMMON_SUCCESS)
		{
			content.resize(bY_HANDLE_FILE_INFORMATION.nFileSizeLow);//只取low，没取high，本函数不适合4g以上文件。。。
			return imply::GetFileMd5Imply(hFile,content,0);
		}
		else
			assert(0);

		return std::string();
	}
	std::string GetFileMd5(HANDLE hFile,uint32 offset,uint32 size)
	{
		std::vector<char> content;
		//DWORD realRead;

		content.resize(size);//本函数不适合4g以上文件。。。
		return imply::GetFileMd5Imply(hFile,content,offset);

	}

}
}
}