#pragma once
#include "stdafx.h"
#include "audio_pch.h"
class string_helper
{
public:

	static std::wstring UTF8ToUnicode(const std::string& in_utf8Str)
	{
		int nNeedWchars = MultiByteToWideChar( CP_UTF8, 0, in_utf8Str.c_str(), -1, NULL, 0 );
		if (nNeedWchars>0)
		{
			std::vector<wchar_t> temp(nNeedWchars);
			::MultiByteToWideChar( CP_UTF8, 0, in_utf8Str.c_str(), -1, &temp[0], nNeedWchars );
			return std::wstring(&temp[0]);
		}

		return std::wstring();
	}


	static std::string UnicodeToUTF8(const std::wstring& in_wStr)
	{
		int nNeedChars = WideCharToMultiByte( CP_UTF8, 0, in_wStr.c_str(), -1, 0, 0, 0, 0 );
		if (nNeedChars>0)//再次判断一下
		{	
			std::vector<char> temp(nNeedChars);
			::WideCharToMultiByte( CP_UTF8, 0, in_wStr.c_str(), -1, &temp[0], nNeedChars, 0, 0 );
			return std::string(&temp[0]);
		}

		return std::string();
	}

};