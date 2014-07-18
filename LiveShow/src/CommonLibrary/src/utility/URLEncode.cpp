/*****************************************************************************
Module :     URLEncode.cpp
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: CPP URL Encoder
*****************************************************************************/
///#define _CRTDBG_MAP_ALLOC

///#include "stdafx.h"
#define _CRTDBG_MAP_ALLOC

#include "stdafx.h"
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>
#include <WININET.H>
#include <stdlib.h>
#include <crtdbg.h>
#include <algorithm>

#include "utility/URLEncode.h"
#include "utility/StringHelper.h"

#define MAX_BUFFER_SIZE 4096

using namespace common;

namespace common
{
	// HEX Values array
	char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	// UNSAFE String
	std::string CURLEncode::csUnsafeString= "'\"<>%\\^[]`+$,@:;/!#?=&";

	// PURPOSE OF THIS FUNCTION IS TO CONVERT A GIVEN CHAR TO URL HEX FORM
	std::string CURLEncode::convert(const char &val) 
	{
		std::string csRet;
		csRet += "%";
		csRet += decToHex(val, 16);	
		return csRet;
	}

	// THIS IS A HELPER FUNCTION.
	// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
	std::string CURLEncode::decToHex(const char &num, const int &radix)
	{	
		int temp=0;	
		std::string csTmp;
		int num_char;
		num_char = (int) num;

		// ISO-8859-1 
		// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
		// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
		if (num_char < 0)
		{
			num_char = 256 + num_char;
		}

		while (num_char >= radix)
		{
			temp = num_char % radix;
			num_char = (int)floor((float)(num_char / radix));
			csTmp = hexVals[temp];
		}

		csTmp += hexVals[num_char];

		if(csTmp.length() < 2)
		{
			csTmp += '0';
		}

		std::string strdecToHex(csTmp);
		// Reverse the String
		
		std::reverse( strdecToHex.begin(), strdecToHex.end());

		return strdecToHex;
	}

	// PURPOSE OF THIS FUNCTION IS TO CHECK TO SEE IF A CHAR IS URL UNSAFE.
	// TRUE = UNSAFE, FALSE = SAFE
	bool CURLEncode::isUnsafe(const char &compareChar)
	{
		bool bcharfound = false;
		char tmpsafeChar = 0;
		size_t m_strLen = 0;

		m_strLen = csUnsafeString.length();
		for(size_t ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
		{
			tmpsafeChar = csUnsafeString.at(ichar_pos); 
			if(tmpsafeChar == compareChar)
			{ 
				bcharfound = true;
				break;
			} 
		}

		int char_ascii_value = 0;
		//char_ascii_value = __toascii(compareChar);
		char_ascii_value = (int) compareChar;

		if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
		{
			return false;
		}
		// found no unsafe chars, return false		
		else
		{
			return true;
		}

		//return true;
	}
	// PURPOSE OF THIS FUNCTION IS TO CONVERT A STRING 
	// TO URL ENCODE FORM.
	std::string CURLEncode::URLEncode(const std::string& pcsEncode)
	{	
		size_t ichar_pos;
		std::string csEncode;
		std::string csEncoded;	
		size_t m_length;

		csEncode = pcsEncode;
		//m_length = csEncode.length();
		m_length = csEncode.size();

		for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
		{
			char ch = csEncode.at(ichar_pos);
			if (ch < ' ') 
			{
				ch = ch;
			}		
			if(!isUnsafe(ch))
			{
				// Safe Character
				char strTmp[3] = {0}; 
				sprintf_s(strTmp, sizeof(strTmp) - 1, "%c", ch);
				strTmp[1] = '\0';

				csEncoded += std::string(strTmp);
			}
			else
			{
				// get Hex Value of the Character
				csEncoded += convert(ch);
			}
		}

		return csEncoded;
	}

	std::wstring CURLEncode::URLEncode(const std::wstring& vData)
	{
		std::string inner_str = utility::stringhelper::UnicodeToUTF8( vData);
		return utility::stringhelper::UTF8ToUnicode( URLEncode( inner_str));
	}

	std::wstring CURLEncode::URLEncodeLocalPath(const std::wstring& vData)
	{
		std::wstring str = vData.c_str();

		WCHAR szTemp[][2][6] = 
		{
			L"&", L"&amp;"
			//L"%", L"%25",
			//L"#", L"%23"
		};

		for(int i = 0; i < ARRAYSIZE(szTemp); ++i)
		{
			utility::stringhelper::Replace(str, szTemp[i][0], szTemp[i][1]);
		}	

		return str;
	}

	std::string CURLEncode::URLDecode(const std::string& vData)
	{
		std::string strResult = "";
		size_t inlen = vData.length();
		for(size_t i=0;i<inlen;i++)
		{
			if(vData.at(i)=='%')
			{
				/**bug 5605 用户输入& + ' '时出错
				要判断&字符后是否还有字符，不然就会导致出错
				*/
				if ( (i + 2) < inlen)
				{
					i++;
					char c = fromHex(vData.at(i++));
					c = c << 4;
					c += fromHex(vData.at(i));
					strResult += c;
				}
			}
			else if(vData.at(i)=='+')
				strResult += ' ';
			else
				strResult += vData.at(i);
		}
		return strResult;
	}

	std::wstring CURLEncode::URLDecode(const std::wstring& vData)
	{
		std::wstring str = vData.c_str();

#if 1
		std::string strUTF8 = common::utility::stringhelper::UnicodeToUTF8(str);
		strUTF8 = URLDecode(strUTF8);

		str = common::utility::stringhelper::UTF8ToUnicode(strUTF8);
#else
		wchar_t szTemp[40][2][4] = {		
			L" ",   L"%20",
			L"'",   L"%27",
			L"\"", L"%22",
			L"<", L"%3C",
			L">", L"%3E",
			L"\\", L"%5C",
			L"^", L"%5E",
			L"[",  L"%5B",
			L"]",  L"%5D",
			L"`",  L"%60",
			L"+", L"%2B",
			L"$", L"%24",
			L",", L"%2C",
			L"@", L"%40",
			L":", L"%3A",
			L";", L"%3B",
			L"/", L"%2F",
			L"!", L"%21",
			L"#", L"%23",
			L"?", L"%3F",
			L"=", L"%3D",
			L"&", L"%26",
			L"<", L"%3c",
			L">", L"%3e",
			L"\\", L"%5c",
			L"^", L"%5e",
			L"[",  L"%5b",
			L"]",  L"%5d",
			L"+", L"%2b",
			L",", L"%2c",
			L":", L"%3a",
			L";", L"%3b",
			L"/", L"%2f",
			L"?", L"%3f",
			L"=", L"%3d",
			L"%", L"%25",
			L"{", L"%7b",
			L"}", L"%7d",
			L"{", L"%7B",
			L"}", L"%7D",
		};

		for(int i = 0; i < 40; ++i)
		{
			utility::stringhelper::Replace(str, szTemp[i][1], szTemp[i][0]);
		}
#endif
		return str;

		//"'\"<>%\\^[]`+$,@:;/!#?=&{}"
		//"%27%22%3C%3E%25%5C%5E%5B%5D%60%2B%24%2C%40%3A%3B%2F%21%23%3F%3D%26%7b%7d"
	}
}