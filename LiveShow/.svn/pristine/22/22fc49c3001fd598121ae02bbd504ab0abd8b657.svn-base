#pragma once
#include "common.h"
#include "base/registry.h"

namespace common
{
namespace utility
{
	class COMMON_INTERFACE Register : public RegKey
	{
	public:

		Register(HKEY rootkey = NULL, 
			const wchar_t* subkey = NULL, 
			REGSAM access = KEY_READ)
			:RegKey(rootkey , subkey, access){}
	};
	//COMMON_INTERFACE bool RegKey::ReadValue(const tchar* name, tstr * value) ;

}
}