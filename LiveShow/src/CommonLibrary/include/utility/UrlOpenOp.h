#pragma once

#include "common.h"
#include <string>
#include <xstring>
#include <map>
#include <vector>
//#include <windows.h>




namespace common
{
namespace utility
{
namespace systemhelper
{
namespace urlopenimply
{
	std::wstring GetIEVersionImply();
	BOOL OpenUrlImply(std::wstring const & url);
}
}
}
}