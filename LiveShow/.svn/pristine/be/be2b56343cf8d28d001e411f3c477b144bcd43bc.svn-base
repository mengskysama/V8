#pragma once

#include "common.h"
#include "expat++\expat_document.h"
#include <string>
#include <vector>
#include <map>

namespace common
{
namespace utility
{
	class COMMON_INTERFACE XmlConfig
	{
	public:
		bool LoadFile(std::wstring& file);
		bool ParserXml(char const* p);
		typedef std::vector<std::map<std::wstring, std::wstring>> ConfigResult;
		bool LoadConfig(std::wstring parents, std::wstring brothers, bool multi, ConfigResult & result, bool emptyOk=false);
		
		common::utility::expat_document xmlDoc;
		void PreHandle(std::string& strInput);//ÌØÊâ×Ö·û´¦Àí
		void PostHandle(std::wstring& strInput);
	};
}
}
