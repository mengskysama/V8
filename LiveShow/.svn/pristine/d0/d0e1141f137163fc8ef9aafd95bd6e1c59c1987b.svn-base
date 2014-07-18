#pragma once

#include <string>
#include <vector>
#include "../Common.h"

namespace common
{
	namespace utility
	{
		/*
		* 对于字符串操作的一些封装，全部大写，全部小写，宽字符转ansi，ansi转宽字符等。
		*/
		namespace stringhelper
		{
			/*
			/*ansi的使字符串全部小写
			* @param [in] in_str 输入字符串
			* @return 返回值将输入字符串变小写直接返回
			*/
			COMMON_INTERFACE std::string MakeLowerA(std::string& in_str);

			/*
			/*ansi使字符串全部小写
			* @param [in] in_str 输入字符串，输入串不变更
			* @return 返回值将输入字符串变小写到新串，返回新串
			*/
			COMMON_INTERFACE std::string MakeLowerCopyA(const std::string& in_str);

			/*
			/*宽字符的使字符串全部小写
			* @param [in] in_str 输入字符串
			* @return 返回值将输入字符串变小写直接返回
			*/
			COMMON_INTERFACE std::wstring MakeLower(std::wstring& in_str);

			/*
			/*宽字符的使字符串全部小写
			* @param [in] in_str 输入字符串，输入串不变更
			* @return 返回值将输入字符串变小写到新串，返回新串
			*/
			COMMON_INTERFACE std::wstring MakeLowerCopy(const std::wstring& in_str);

			/*
			/*ansi的使字符串全部大写
			* @param [in] in_str 输入字符串
			* @return 返回值将输入字符串变大写直接返回
			*/
			COMMON_INTERFACE std::string MakeUpperA(std::string& in_str);

			/*
			/*ansi的使字符串全部大写
			* @param [in] in_str 输入字符串，输入串不变更
			* @return 返回值将输入字符串变大写到新串，返回新串
			*/
			COMMON_INTERFACE std::string MakeUpperCopyA(const std::string& in_str);

			/*
			/*宽字符的使字符串全部大写
			* @param [in] in_str 输入字符串
			* @return 返回值将输入字符串变大写到新串，返回新串
			*/
			COMMON_INTERFACE std::wstring MakeUpper(std::wstring& in_str);

			/*
			/*返回值将输入字符串变小写到新串，返回新串
			* @param [in] in_str 输入字符串，输入串不变更
			* @return 返回值将输入字符串变大写直接返回
			*/
			COMMON_INTERFACE std::wstring MakeUpperCopy(const std::wstring& in_str);

			/*
			* 从字符串转为整型
			* @param [in] in_str 输入的源字符串，内容不修改
			* @param [out] iRetInt 返回的整数值，返回值正确时，启用之
			* @param [in] kBase 输入的字符串进制，默认为十进制
			* @return 返回true表示成功，false失败，失败原因字符串为空等 
			*/
			COMMON_INTERFACE bool StringToIntA(const std::string& in_str, int* iRetInt, int kBase = 10);

			/*
			* 从字符串转为整型
			* @param [in] in_str 输入的源字符串，内容不修改
			* @param [in] kBase 输入的字符串进制，默认为十进制
			* @return 转换失败返回为0 
			*/
			COMMON_INTERFACE int StringToIntA(const std::string& in_str, int kBase = 10);

			/*
			* 从宽字符串转为整型
			* @param [in] in_str 输入的源宽字符串，内容不修改
			* @param [out] bRetInt 返回的整数值，返回值正确时，启用之
			* @param [in] kBase 输入的字符串进制，默认为十进制
			* @return 返回true表示成功，false失败，失败原因字符串为空等 
			*/
			COMMON_INTERFACE bool StringToInt(const std::wstring& in_str, int* bRetInt, int kBase = 10);

			/*
			* 从字符串转为整型
			* @param [in] in_str 输入的源字符串，内容不修改
			* @param [in] kBase 输入的字符串进制，默认为十进制
			* @return 转换失败返回为0 
			*/
			COMMON_INTERFACE int StringToInt(const std::wstring& in_str, int kBase = 10);

			/*
			* 从字符串转为整型
			* @param [in] inNUM 输入的整型值，可能为负数（输出一个负号）
			* @return 返回为NULL时，转换失败
			*/
			COMMON_INTERFACE std::string IntToStringA(int inNUM);

			/*
			* 从宽字符串转为整型
			* @param [in] inNUM 输入的整型值，可能为负数（输出一个负号）
			* @return 返回为NULL时，转换失败
			*/
			COMMON_INTERFACE std::wstring IntToString(int inNUM);

			/*
			* 从宽字符串转为ansi字符串
			* @param [in] in_str 为输入的源ansi字符
			* @return 返回值为宽字符串
			*/
			COMMON_INTERFACE std::string UnicodeToAscii(const std::wstring& in_str);

			/*
			* 从ansi字符串转为宽字符串
			* @param [in]  in_str 为输入的源ansi字符
			* @return 返回值为ansi字符串
			*/
			COMMON_INTERFACE std::wstring AsciiToUnicode(const std::string& in_str);

			/*
			* 从utf8字符串转为宽字符串
			* @param [in] in_utf8Str 输入UTF8字符串
			* @return 返回值为UNICODE字符串，如果转换失败，返回NULL
			*/
			COMMON_INTERFACE std::wstring UTF8ToUnicode(const std::string& in_utf8Str);

			/*
			* 从宽字符串转为utf8字符串
			* @param [in] in_wStr 输入宽字符串
			* @return 返回值为UTF8字符串，如果转换失败，返回NULL
			*/		
			COMMON_INTERFACE std::string UnicodeToUTF8(const std::wstring& in_wStr);

			/*
			* ansi版本的判断字符串是否全部数字组成
			* @param [in] in_str 输入的字符串
			* @return 返回值为true时，则表明是全部数字组成的字符串，否则返回false
			*/	
			COMMON_INTERFACE bool IsNumeric(const std::string& in_str);

			/*
			* UNICODE版本的判断字符串是否全部数字组成
			* @param [in] in_str 输入的字符串
			* @return 返回值为true时，则表明是全部数字组成的字符串，否则返回false
			*/	
			COMMON_INTERFACE bool IsNumeric(const std::wstring& in_str);


			/*
			* UNICODE版本的判断字符是否属于ascii字符，统计为一个字符
			* @param [in] in_chr 输入的宽字符
			* @return 返回值为true时，则表明属于ascii字符，否则返回false
			*/	
			COMMON_INTERFACE bool IsCharAscii(const wchar_t in_chr);

			/*
			* UNICODE版本的统计字符个数，宽字符计算为2，anscii计算为1
			* @param [in] in_str 输入的宽字符串
			* @return 返回值为ansi，则表明属于ascii字符，否则返回false
			*/
			COMMON_INTERFACE size_t CalcStringAsciiLength(const wchar_t* in_str);	

			/*
			* UNICODE版本的统计字符个数，宽字符计算为2，anscii计算为1
			* @param [in] in_str 输入的宽字符串
			* @return 返回值为ansi，则表明属于ascii字符，否则返回false
			*/
			COMMON_INTERFACE size_t CalcStringAsciiLength(const std::wstring& in_str);	

			/*
			* 获取字符串中指定数目的anscii码的字串
			* @param [in] in_str 输入的宽字符串
			* @param [in][out] szRet 返回的子字符串
			* @param [in] nMaxInByte 所需的子字符串的最大字符数目（宽字符计算为2，anscii计算为1）
			* @return 返回值为子字符串中包含的anscii码数目
			*/
			COMMON_INTERFACE size_t GetSubstrInAnsi(const std::wstring& in_str, std::wstring& szRet, size_t nMaxInByte);

			/*
			* 获取字符串中指定位置开始数目的anscii码的字串
			* @param [in] in_str 输入的宽字符串
			* @param [in][out] szRet 返回的子字符串
			* @param [in] nMaxInByte 所需的子字符串的最大字符数目（宽字符计算为2，anscii计算为1）
			* @return 返回值为子字符串在字符串中所在anscii码位置
			*/
			COMMON_INTERFACE size_t GetSubstrInAnsi(const std::wstring& in_str, std::wstring& szRet, size_t nbeginMinInByte,size_t nMaxInByte);

			/*
			* 将字符串的首尾空白字符移除，包括空格' ' 换行\n 回车\r 制表符\等！！！
			* @param [in] in_str 输入字符串
			* @return 返回值为删除空白串的字符串，直接对输入串操作
			*/	
			COMMON_INTERFACE std::string TrimA(std::string& in_str);

			/*
			* 将字符串的首尾空白字符移除
			* @param [in] in_Str 输入字符串
			* @return 返回值为删除空白串的新字符串
			*/	
			COMMON_INTERFACE std::string TrimCopyA(const std::string& in_str);

			/*
			* 将宽字符串的首尾空白字符移除
			* @param [in] in_wStr 输入宽字符串
			* @return 返回值为删除空白串的宽字符串，直接对输入串操作
			*/	
			COMMON_INTERFACE std::wstring Trim(std::wstring& in_wStr);

			/*
			* 将宽字符串的首尾空白字符移除
			* @param [in] in_wStr 输入宽字符串
			* @return 返回值为删除空白串的新宽字符串
			*/	
			COMMON_INTERFACE std::wstring TrimCopy(const std::wstring& in_str);

			/*
			* 从字符串中根据截断字符进行截断（注意：删除了首尾的空白字符）
			* @param [in] in_str 输入字符串
			* @param [in] cSplit 输入的截断字符
			* @return 返回值截断的各个字串，如果字符串中没有截断字符的话，vector大小为0
			*/	
			COMMON_INTERFACE std::vector<std::string> Split_String_by_DelimiterA(const std::string& in_str, char cSplit);

			/*
			* 从宽字符串中根据截断字符进行截断（注意：删除了首尾的空白字符）
			* @param [in] in_wStr 输入宽字符串
			* @param [in] cSplit 输入的截断宽字符
			* @return 返回值截断的各个子宽字符串，如果字符串中没有截断字符的话，vector大小为0
			*/	
			COMMON_INTERFACE std::vector<std::wstring> Split_String_by_Delimiter(const std::wstring& in_wStr, wchar_t cSplit);

			/*
			* format字符串
			* @param [in] format 格式化字符串
			* @return 返回为格式化字符串，错误格式化形式时，则输出NULL
			*/
			COMMON_INTERFACE std::string FormatA(const char* format, ...);

			/*
			* 从宽字符串转为utf8字符串
			* @param [in] format 宽字符的格式化字符串
			* @return 返回为格式化字符串，错误格式化形式时，则输出NULL
			*/
			COMMON_INTERFACE std::wstring Format(const wchar_t* format, ...);

			/*将ascii码字串转换为hex字串，通常用作url的参数，（替代get_token_string，AsciiToHex函数）
			* 从宽字符串转为utf8字符串
			* @param [in] token 输入字符串
			* @return 返回为Hex的字符串
			*/
			COMMON_INTERFACE std::string HexEncodeCopy(const std::string& in_str);

			/*将ascii码字串转换为hex字串，通常用作url的参数，（替代get_token_string，AsciiToHex函数）
			* 从宽字符串转为utf8字符串
			* @param [in] in_str 输入字符串指针
			* @param [in] size 输入字符串指针所指的字串长度
			* @return 返回为Hex的字符串，输入参数错误时，返回为NULL
			*/
			COMMON_INTERFACE std::string HexEncodeCopy(const char* in_str, size_t size);

			/*
			/*XML文本中对应的ascii码字符串编码
			* @param [in] in_xmlStr 输入压缩前的字符串
			* @return 返回为压缩后的字符串，返回新串
			*/
			COMMON_INTERFACE std::string XMLEncodeA(const std::string& in_xmlStr);

			/*
			/*XML文本中对应的unicode码字符串编码
			* @param [in] in_xmlStr 输入压缩前的字符串
			* @return 返回为压缩后的字符串，返回新串
			*/
			COMMON_INTERFACE std::wstring XMLEncode(const std::wstring& in_xmlStr);

			/*
			/*XML文本中对应的ascii码字符串解码
			* 从宽字符串转为utf8字符串
			* @param [in] in_xmlStr 输入压缩后字符串
			* @return 返回为解压缩后的字符串，返回新串
			*/
			COMMON_INTERFACE std::string XMLDecodeA(const std::string& in_xmlStr);

			/*
			/*XML文本中对应的unicode码字符串解码
			* @param [in] in_xmlStr 输入压缩后字符串
			* @return 返回为解压缩后的字符串，返回新串
			*/
			COMMON_INTERFACE std::wstring XMLDecode(const std::wstring& in_xmlStr);

			/** 对std::wstring进行Replace操作
			* @param [in][out] std::wstring &str,待替换的字符串
			* @param [in] const std::wstring &strSrc,要被替换的源字符子串
			* @param [in] const std::wstring &strDst,要被替换的目的字符子串
			* @return void.
			*/
			COMMON_INTERFACE void Replace(std::wstring &str, const std::wstring &strSrc, const std::wstring &strDst);
			COMMON_INTERFACE void Replace(std::string &str, const std::string &strSrc, const std::string &strDst);

			//字符集的枚举定义
			enum E_CHARACTER_SET
			{
				SIMPLE_CHINESE       = 0X01,
				TRADITIONAL_CHINESE  = 0X02,
				CHINESE              = SIMPLE_CHINESE | TRADITIONAL_CHINESE,
				ENGLISH              = 0X04,
				KOREA                = 0X08,
				JAPANESE             = 0X10,
				SPANISH              = 0x20,
				FRANCE               = 0x40,
				GERMAN               = 0x80,
				RUSIA                = 0x100,
			};

			/** 判断字符输入字符集函数
			* @param [in] set, 字符集的枚举值，可以多项结合，用“|”来连接
			* @param [in] charcter, 待判断的字符
			* @return 返回为true时，表示字符串指定的字符集
			*/
			COMMON_INTERFACE bool IsInCharacterSet(uint32 const &set ,wchar_t const charcter);

			/** 判断字符输入字符串集函数
			* @param [in] set, 字符集的枚举值，可以多项结合，用“|”来连接
			* @param [in] charcters, 待判断的字符串
			* @return 返回为true时，表示字符串属于指定的字符集
			*/
			COMMON_INTERFACE bool IsInCharacterSet(uint32 const &set ,std::wstring const charcters);

			inline int char2hex(char c)
			{
				if (c >= '0' && c <= '9') return c - '0';
				if (c >= 'a' && c <= 'f') return c - 'a' + 10;
				if (c >= 'A' && c <= 'F') return c - 'A' + 10;
				return c;
			}	
			
			int isSpace(int c);			

			COMMON_INTERFACE std::string UrlDecode(const std::string& encoded_url);

			COMMON_INTERFACE std::wstring UrlDecodeW(const std::wstring& encoded_url); //GB2312 的UrlDecode解码

			COMMON_INTERFACE std::string UrlEncode(const std::string& url);

			COMMON_INTERFACE std::wstring UrlEncodeW(const std::wstring& url);	//GB2312 的UrlEncode编码

			COMMON_INTERFACE std::string UrlUTF8Encode(const std::string& url);	 //UTF-8 的UrlEncode编码

			COMMON_INTERFACE std::wstring UrlUTF8EncodeW(const std::wstring& url);
			

        };
	};	
};