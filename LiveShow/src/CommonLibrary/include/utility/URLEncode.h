/*****************************************************************************
Module :     URLEncode.h
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: H URL Encoder
			URL Encode/Decode 类CURLEncode的声明头文件                                                    
			进行URL Encode 编码如空格将转换为 %20和URL Decode解码 如将%20转换为空格 
			支持std::string和std::wstring两种类型的参数.
*****************************************************************************/
#ifndef __CURLENCODE_H_
#define __CURLENCODE_H_

#include <string>
#include "../Common.h"

namespace common 
{
	/*
	* URL Encode/Decode 类CURLEncode
	* 进行URL Encode 编码如空格将转换为 %20和URL Decode解码 如将%20转换为空格
	* 支持std::string和std::wstring两种类型的参数.
	*/
	class COMMON_INTERFACE CURLEncode
	{
	public:
		CURLEncode() { };
		virtual ~CURLEncode() { };
		
		/*
		* 对传入vData进行URL Encode 编码如空格将转换为 %20
		* 和IE上输入的URL转换类似
		* @param [in] const std::string& vData 待转换的字符串
		* @return std::string 转换后的字符串
		*/
		std::string URLEncode(const std::string& vData);

		/*
		* 对传入vData进行URL Encode 编码如空格将转换为 %20
		* 和IE上输入的URL转换类似
		* @param [in] const std::wstring& vData 待转换的字符串
		* @return std::wstring 转换后的字符串
		*/
		std::wstring URLEncode(const std::wstring& vData);
		
		/*
		* 对传入vData进行URL Decode 解码如%20将转换为 空格
		* 和IE上输入的URL转换类似
		* @param [in] const std::wstring& vData 待转换的字符串
		* @return std::wstring 转换后的字符串
		*/
		std::wstring URLDecode(const std::wstring& vData);
		
		/*
		* 对传入vData进行URL Decode 解码如%20将转换为 空格
		* 和IE上输入的URL转换类似
		* @param [in] const std::string& vData 待转换的字符串
		* @return std::string 转换后的字符串
		*/
		std::string URLDecode(const std::string& vData);
		
		/** 对本地路径进行URLEncode,主要用于聊天框中的加载本地html用
		* 这里不需要进行所有字符的Encode只需要进行少数字符的Encode
		* 如:%,#等
		* @param [in] const std::wstring& vData 待转换的字符串
		* @return std::wstring.
		*/
		std::wstring URLEncodeLocalPath(const std::wstring& vData);

	private:
		static std::string csUnsafeString;
		std::string decToHex(const char &num, const int &radix);
		bool isUnsafe(const char &compareChar);
		std::string convert(const char &val);

		inline BYTE toHex(const BYTE &x){ return x > 9 ? x + 55: x + 48; }
		inline BYTE fromHex(const BYTE &x){ return x > 64 ? x - 55 : x - 48; }
	};
}

#endif //__CURLENCODE_H_