#include "../Common.h"
#include "windows.h"

namespace common
{
namespace utility
{
namespace md5
{
	COMMON_INTERFACE std::string GetStringMd5(std::string const & s);
	COMMON_INTERFACE std::string GetBufferMd5(void const * p,uint32 size);
	COMMON_INTERFACE std::string GetFileMd5(std::wstring const & file);
	COMMON_INTERFACE std::string GetFileMd5(std::wstring const & file ,uint32 offset,uint32 size);
	COMMON_INTERFACE std::string GetFileMd5(HANDLE hFile);
	COMMON_INTERFACE std::string GetFileMd5(HANDLE hFile,uint32 begin,uint32 size); 
}
}
}
