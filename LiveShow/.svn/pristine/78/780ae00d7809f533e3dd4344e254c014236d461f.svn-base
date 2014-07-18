#include "utility/StringHelper.h"
#include <Windows.h>
#include <algorithm>
#include <assert.h>

#define arraysize(a) (sizeof(a) / sizeof(*(a)))

#define COMPILER_MSVC

//内部使用
template<typename T>
struct ToUnsigned {
	typedef T Unsigned;
};

template<>
struct ToUnsigned<char> {
	typedef unsigned char Unsigned;
};
template<>
struct ToUnsigned<signed char> {
	typedef unsigned char Unsigned;
};

template<class STR>
static bool IsStringASCII(const STR& str) {
	for (size_t i = 0; i < str.length(); i++) 
	{
		typename ToUnsigned<typename STR::value_type>::Unsigned c = str[i];
		if (c > 0x7F)
		{
			return false;
		}
	}
	return true;
};

namespace common
{
	namespace base 
    {
		inline void va_copy(va_list& a, va_list& b)
		{
	#if defined(COMPILER_GCC)
			::va_copy(a, b);
	#elif defined(COMPILER_MSVC)
			a = b;
	#endif
		}

		bool IsWprintfFormatPortable(const wchar_t* format) 
		{
			for (const wchar_t* position = format; *position != '\0'; ++position) 
			{
				if (*position == '%')
				{
					bool in_specification = true;
					bool modifier_l = false;
					while (in_specification) 
					{
						// Eat up characters until reaching a known specifier.
						if (*++position == '\0') 
						{
							// The format string ended in the middle of a specification.  Call
							// it portable because no unportable specifications were found.  The
							// string is equally broken on all platforms.
							return true;
						}

						if (*position == 'l')
						{
							// 'l' is the only thing that can save the 's' and 'c' specifiers.
							modifier_l = true;
						} 
						else if (((*position == 's' || *position == 'c') && !modifier_l) ||
							*position == 'S' || *position == 'C' || *position == 'F' ||
							*position == 'D' || *position == 'O' || *position == 'U') 
						{
								// Not portable.
								return false;
						}

						if (wcschr(L"diouxXeEfgGaAcspn%", *position)) 
						{
							// Portable, keep scanning the rest of the format string.
							in_specification = false;
						}
					}//end for while
				}//end for wif (*position == '%')
			}
			return true;
		}

		inline int vsnprintf(char* buffer, size_t size,
							 const char* format, va_list arguments) 
		{
			int length = vsnprintf_s(buffer, size, size - 1, format, arguments);
			if (length < 0)
			{
				return _vscprintf(format, arguments);
			}

			return length;
		}

		inline int vswprintf(wchar_t* buffer, size_t size,
							 const wchar_t* format, va_list arguments)
		{
// 			assert(IsWprintfFormatPortable(format));
// 			if (!IsWprintfFormatPortable(format))
// 			{
// 				return 0;
// 			}

			int length = _vsnwprintf_s(buffer, size, size - 1, format, arguments);
			if (length < 0)
			{
				return _vscwprintf(format, arguments);
			}
			return length;
		}
	}
}  // namespace base

//ansi
std::string common::utility::stringhelper::MakeLowerA(std::string& in_str)
{
	std::transform( in_str.begin(), in_str.end(), in_str.begin(),tolower);
	return in_str;
}
//ansi-copy
std::string common::utility::stringhelper::MakeLowerCopyA(const std::string& in_str)
{
	std::string str_op = in_str;
	std::transform( in_str.begin(), in_str.end(), str_op.begin(),tolower);
	return in_str;
}

//unicode
std::wstring common::utility::stringhelper::MakeLower(std::wstring& in_str)
{
	std::transform( in_str.begin(), in_str.end(), in_str.begin(),tolower);
	return in_str;
}
//unicode-copy
std::wstring common::utility::stringhelper::MakeLowerCopy(const std::wstring& in_str)
{
	std::wstring str_op=in_str;
	std::transform( in_str.begin(), in_str.end(), str_op.begin(),tolower);
    return str_op;
}

//ansi
std::string common::utility::stringhelper::MakeUpperA(std::string& in_str)
{
	std::transform( in_str.begin(), in_str.end(), in_str.begin(),toupper);
	return in_str;
}
//ansi-copy
std::string common::utility::stringhelper::MakeUpperCopyA(const std::string& in_str)
{
	std::string str_op = in_str;
	std::transform( in_str.begin(), in_str.end(), str_op.begin(),toupper);
	return in_str;
}

//unicode
std::wstring common::utility::stringhelper::MakeUpper(std::wstring& in_str)
{
	std::transform( in_str.begin(), in_str.end(), in_str.begin(),toupper);
	return in_str;
}
//unicode-copy
std::wstring common::utility::stringhelper::MakeUpperCopy(const std::wstring& in_str)
{
	std::wstring str_op = in_str;
	std::transform( in_str.begin(), in_str.end(), str_op.begin(),toupper);
	return str_op;
}

//ansi版本
std::string common::utility::stringhelper::IntToStringA(int value)
{
	// log10(2) ~= 0.3 bytes needed per bit or per byte log10(2**8) ~= 2.4.
	// So round up to allocate 3 output characters per byte, plus 1 for '-'.
	const int kOutputBufSize = 3 * sizeof(int) + 1;

	// Allocate the whole string right away, we will right back to front, and
	// then return the substr of what we ended up using.
	std::string outbuf(kOutputBufSize, 0);

	bool is_neg = value < 0;
	// Even though is_neg will never be true when INT is parameterized as
	// unsigned, even the presence of the unary operation causes a warning. 
	// eg. 0x80-00-00-00, is negative when param in unsigned  passed
	UINT res = (is_neg) ? static_cast<UINT>(-value) : static_cast<UINT>(value);
	
	for (std::string::iterator it = outbuf.end();;) 
	{
		--it;
		assert(it != outbuf.begin());
		*it = static_cast<std::string::value_type>((res % 10) + '0'); //obtain highest bit position data currently
		res /= 10;   // next bit position

		if (res == 0)// We're done..
		{
			if (is_neg) 
			{
				--it;
				assert(it != outbuf.begin());
				*it = static_cast<std::string::value_type>('-');
			}
			return std::string(it, outbuf.end()); //reverse order
		}
	}
	//return std::string();
}

//wchar版本
std::wstring common::utility::stringhelper::IntToString(int value)
{
	// log10(2) ~= 0.3 bytes needed per bit or per byte log10(2**8) ~= 2.4.
	// So round up to allocate 3 output characters per byte, plus 1 for '-'.
	const int kOutputBufSize = 3 * sizeof(int) + 1;

	// Allocate the whole string right away, we will right back to front, and
	// then return the substr of what we ended up using.
	std::wstring outbuf(kOutputBufSize, 0);

	bool is_neg = value < 0;
	// Even though is_neg will never be true when INT is parameterized as
	// unsigned, even the presence of the unary operation causes a warning. 
	// eg. 0x80-00-00-00, is negative when param in unsigned  passed
	UINT res = (is_neg) ? static_cast<UINT>(-value) : static_cast<UINT>(value);

	for (std::wstring::iterator it = outbuf.end();;) 
	{
		--it;
		assert(it != outbuf.begin());
		*it = static_cast<std::wstring::value_type>((res % 10) + '0'); //obtain highest bit position data currently
		res /= 10;   // next bit position

		if (res == 0)// We're done..
		{
			if (is_neg) 
			{
				--it;
				assert(it != outbuf.begin());
				*it = static_cast<std::wstring::value_type>('-');
			}
			return std::wstring(it, outbuf.end()); //reverse order
		}
	}
	//return std::wstring();
}

//ansi版本
bool common::utility::stringhelper::StringToIntA(const std::string& in_str, int* iRetInt, int kBase)
{
	assert(iRetInt != NULL);
	if (iRetInt == NULL)
	{
		return false;
	}

	//static const int kBase = 10;
	std::string::value_type* endptr = NULL;
    long value = strtol(in_str.c_str(), &endptr, kBase);

	*iRetInt = value;  //复制

	return !in_str.empty() &&
		   in_str.c_str() + in_str.length() == endptr && 
		   !isspace(in_str[0]);
}

//ansi版本
int common::utility::stringhelper::StringToIntA(const std::string& in_str, int kBase)
{
	int i=0;

	bool bRet = StringToIntA(in_str, (int*)&i, kBase);
	if( bRet == true)
		return i;

	return 0;
}


//wchar版本
bool common::utility::stringhelper::StringToInt(const std::wstring& in_str, int* iRetInt, int kBase)
{
	assert(iRetInt != NULL);
	if (iRetInt == NULL)
	{
		return false;
	}

	//by ken
	if(in_str.empty())
	{
		* iRetInt=0;
		return false;
	}

	//static const int kBase = 10;
	std::wstring::value_type* endptr = NULL;
	long value = wcstol(in_str.c_str(), &endptr, kBase);

	*iRetInt = value;  //复制

	return !in_str.empty() &&
		in_str.c_str() + in_str.length() == endptr && 
		!iswspace(in_str[0]);
}

//wchar版本
int common::utility::stringhelper::StringToInt(const std::wstring& in_str, int kBase)
{
	int i=0;

	bool bRet = StringToInt(in_str, (int*)&i, kBase);
	if( bRet == true)
		return i;

	return 0;
}

//unicode=>ansi
std::string common::utility::stringhelper::UnicodeToAscii(const std::wstring& wide)
{
	int nNeedChars = WideCharToMultiByte( CP_ACP, 0, wide.c_str(), -1, 0, 0, 0, 0 );
	if (nNeedChars>0)//再次判断一下
	{	
		std::vector<char> temp(nNeedChars);
		::WideCharToMultiByte( CP_ACP, 0, wide.c_str(), -1, &temp[0], nNeedChars, 0, 0 );
		return std::string(&temp[0]);
	}

	return std::string();
}

//ansi=>unicode
std::wstring common::utility::stringhelper::AsciiToUnicode(const std::string& ascii)
{
	int nNeedWchars = MultiByteToWideChar( CP_ACP, 0, ascii.c_str(), -1, NULL, 0 );
	if (nNeedWchars>0)
	{
		std::vector<wchar_t> temp(nNeedWchars);
		::MultiByteToWideChar( CP_ACP, 0, ascii.c_str(), -1, &temp[0], nNeedWchars );
		return std::wstring(&temp[0]);
	}

	return std::wstring();
}

//utf8=>unicode
std::wstring common::utility::stringhelper::UTF8ToUnicode(const std::string& in_utf8Str)
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

//unicode=>utf8
std::string common::utility::stringhelper::UnicodeToUTF8(const std::wstring& in_wStr)
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

//ansi版本
bool common::utility::stringhelper::IsNumeric(const std::string& in_str)
{
	int iRetInt;
    if (common::utility::stringhelper::StringToIntA(in_str, &iRetInt))
	{
		return true;
	}

	return false;
}

//Unicode版本
bool common::utility::stringhelper::IsNumeric(const std::wstring& in_str)
{
	int iRetInt;
	if (common::utility::stringhelper::StringToInt(in_str, &iRetInt))
	{
		return true;
	}

	return false;
}

bool common::utility::stringhelper::IsCharAscii(const wchar_t in_chr)
{
	ToUnsigned<wchar_t>::Unsigned c = in_chr;
	 if (c > 0x7f)
	 {
		 return false;
	 }

	 return true;
}

size_t common::utility::stringhelper::CalcStringAsciiLength(const std::wstring& in_str)
{
	return CalcStringAsciiLength(in_str.c_str());
}

size_t common::utility::stringhelper::CalcStringAsciiLength(const wchar_t* in_str)
{
	assert(NULL != in_str);
	if (NULL == in_str)
	{
		return 0;
	}

	size_t len  = wcslen(in_str);
    size_t nRet = 0;
	for (size_t i = 0; i < len; i++)
	{
		nRet += IsCharAscii(in_str[i]) ? 1 : 2;
	}

	return nRet;
}

size_t common::utility::stringhelper::GetSubstrInAnsi(const std::wstring& in_str, std::wstring& szRet, size_t nMaxInByte)
{
	size_t nSrcLen= in_str.length();
	size_t count  = 0; //byte counts
	size_t iIndex = 0; //wstring element
	size_t nRet   = 0;
	for (; count<=nMaxInByte; ) 
	{
		nRet = count;  //保存最近一次检测成功的值
		if (nSrcLen == iIndex)
		{
			szRet = in_str;
			return nRet;
		}
		count += (common::utility::stringhelper::IsCharAscii(in_str[iIndex])) ? (1) : (2);	 
		++iIndex;
	}
		
	assert(iIndex>0);
	szRet = in_str.substr(0, iIndex-1);
	return nRet;
}

size_t common::utility::stringhelper::GetSubstrInAnsi(const std::wstring& in_str, std::wstring& szRet, size_t nbeginMinInByte,size_t nMaxInByte)
{
	size_t nSrcLen= in_str.length();
	size_t count  = 0; //byte counts
	size_t iIndex = 0; //wstring element
	size_t nRet   = 0;
	size_t nbegin = 0;	//begin 
	for (; count<=nbeginMinInByte+nMaxInByte; ) 
	{
		nRet = count;  //保存最近一次检测成功的值
		if (nSrcLen == iIndex)
		{	
			szRet = in_str.substr(nbegin);
			return nRet;
		}
		count += (common::utility::stringhelper::IsCharAscii(in_str[iIndex])) ? (1) : (2);	 
		++iIndex;

		if(count < nbeginMinInByte)
		{			
			++nbegin;
		}
		
	}

	assert(iIndex>0);
	assert(nbegin>=0);
	szRet = in_str.substr(nbegin, iIndex-1-nbegin);
	return nRet;
}



inline  bool IsSet(uint32 a,uint32 b)
{
	return a & b;
}


//http://203.208.37.132/search?q=cache:UaNj93sRC5kJ:markmail.org/message/edvbd7piz5cq2f32+%E5%8F%A5%E5%8F%B7+unicode&cd=2&hl=zh-CN&ct=clnk&gl=cn&lr=lang_zh-CN|lang_zh-TW&st_usg=ALhdy2-8MXrJ0YTfac9gMFIYx7x1LKXpVw
//http://blog.minidx.com/2008/12/09/1700.html
bool common::utility::stringhelper::IsInCharacterSet(uint32 const &set ,wchar_t const charcter)
{
	if(IsSet(set,SIMPLE_CHINESE)==true)
	{
		if((charcter>=0x4E00 && charcter<=0x9FBB)
			||
			(charcter>=0x3000 && charcter<=0x303f)
			||
			(charcter>=0xfe00 && charcter<=0xffff)
			||
			(charcter>=0x2000 && charcter<=0x22ff)
			||
			(charcter>=0x3300 && charcter<=0x33ff)
			||
			(charcter>=0x2500 && charcter<=0x26ff)
			||
			(charcter>=0x5100 && charcter<=0x51ff)
			)
			return true;
	}

	// 	if(IsSet(set,TRADITIONAL_CHINESE)==true)
	// 	{	
	// 		if(charcter>=0x4E00 && charcter<=0x9FBB)
	// 			return true;
	// 	}

	if(IsSet(set,ENGLISH)==true)
	{
		if(charcter>=0x00 && charcter<=0x7F)
			return true;
	}

	return false;
}

bool common::utility::stringhelper::IsInCharacterSet(uint32 const &set ,std::wstring const charcters)
{
	for(std::wstring::const_iterator it=charcters.begin();it!=charcters.end();++it)
	{
		if(!IsInCharacterSet(set,*it))
		{
			return false;
		}
	}
	return true;
}

namespace //anonymous  namespace
{
	const wchar_t kWhitespaceWide[] = {
		0x0009,  // <control-0009> to <control-000D>
		0x000A,
		0x000B,
		0x000C,
		0x000D,
		0x0020,  // Space
		0x0085,  // <control-0085>
		0x00A0,  // No-Break Space
		0x1680,  // Ogham Space Mark
		0x180E,  // Mongolian Vowel Separator
		0x2000,  // En Quad to Hair Space
		0x2001,
		0x2002,
		0x2003,
		0x2004,
		0x2005,
		0x2006,
		0x2007,
		0x2008,
		0x2009,
		0x200A,
		0x200C,  // Zero Width Non-Joiner
		0x2028,  // Line Separator
		0x2029,  // Paragraph Separator
		0x202F,  // Narrow No-Break Space
		0x205F,  // Medium Mathematical Space
		0x3000,  // Ideographic Space
		0
	};
	const char kWhitespaceASCII[] = {
		0x09,    // <control-0009> to <control-000D>
		0x0A,
		0x0B,
		0x0C,
		0x0D,
		0x20,    // Space
		0
	};

	enum TrimPositions 
	{
		TRIM_NONE     = 0,
		TRIM_LEADING  = 1 << 0,
		TRIM_TRAILING = 1 << 1,
		TRIM_ALL      = TRIM_LEADING | TRIM_TRAILING,
	};

	template<typename STR>
	TrimPositions TrimStringT(const STR& input,
		                      const typename STR::value_type trim_chars[],
		                      TrimPositions positions,
		                      STR* output)
	{
		// Find the edges of leading/trailing whitespace as desired.
		const typename STR::size_type last_char = input.length() - 1;
		const typename STR::size_type first_good_char = (positions & TRIM_LEADING) ?
				                                         input.find_first_not_of(trim_chars) : 0;
		const typename STR::size_type last_good_char  = (positions & TRIM_TRAILING) ?
				                                         input.find_last_not_of(trim_chars) : last_char;

		// When the string was all whitespace, report that we stripped off whitespace
		// from whichever position the caller was interested in.  For empty input, we
		// stripped no whitespace, but we still need to clear |output|.
		if (input.empty() || (first_good_char == STR::npos) || (last_good_char == STR::npos)) 
		{
			bool input_was_empty = input.empty();  // in case output == &input
			output->clear();
			return input_was_empty ? TRIM_NONE : positions;
		}

		// Trim the whitespace.
		*output = input.substr(first_good_char, last_good_char - first_good_char + 1);

		// Return where we trimmed from.
		return static_cast<TrimPositions>(((first_good_char == 0) ? TRIM_NONE : TRIM_LEADING) |((last_good_char == last_char) ? TRIM_NONE : TRIM_TRAILING));
	}

	TrimPositions TrimWhitespace(const std::wstring& input,
		                         TrimPositions positions,
		                         std::wstring* output)
	{
		return TrimStringT(input, kWhitespaceWide, positions, output);
	}

	TrimPositions TrimWhitespaceASCII(const std::string& input,
		                              TrimPositions positions,
		                              std::string* output) 
	{
		return TrimStringT(input, kWhitespaceASCII, positions, output);
	}

	// This function is only for backward-compatibility.
	// To be removed when all callers are updated.
	TrimPositions TrimWhitespace(const std::string& input,
		                         TrimPositions positions,
		                         std::string* output) 
	{
		return TrimWhitespaceASCII(input, positions, output);
	}

	template<typename STR>
	static void SplitStringT(const STR& str, const typename STR::value_type s, bool trim_whitespace, std::vector<STR>* r)
	{
		size_t last = 0;
		size_t i;
		size_t c = str.size();
		for (i = 0; i <= c; ++i) 
		{
			if (i == c || str[i] == s) 
			{
				size_t len = i - last;
				STR tmp = str.substr(last, len);
				if (trim_whitespace) 
				{
					STR t_tmp;
					TrimWhitespace(tmp, TRIM_ALL, &t_tmp);
					r->push_back(t_tmp);
				} 
				else 
				{
					r->push_back(tmp);
				}
				last = i + 1;
			}
		}
	}
};

//ansi版本
std::string common::utility::stringhelper::TrimA(std::string& in_str)
{
     TrimStringT(in_str, kWhitespaceASCII, TRIM_ALL, &in_str);
	 return in_str;
}

//ansi-copy版本
std::string common::utility::stringhelper::TrimCopyA(const std::string& in_str)
{
	std::string strOutput;
     TrimStringT(in_str, kWhitespaceASCII, TRIM_ALL, &strOutput);
	 return strOutput;
}

//unicode版本
std::wstring common::utility::stringhelper::Trim(std::wstring& in_wStr)
{
	TrimStringT(in_wStr, kWhitespaceWide, TRIM_ALL, &in_wStr);
	return in_wStr;
}

//unicode版本
std::wstring common::utility::stringhelper::TrimCopy(const std::wstring& in_wStr)
{
	std::wstring strOutput;
	TrimStringT(in_wStr, kWhitespaceWide, TRIM_ALL, &strOutput);
	return strOutput;
}

//ansi版本
std::vector<std::string> common::utility::stringhelper::Split_String_by_DelimiterA(const std::string& in_str, char cSplit)
{
     std::vector<std::string> vecRet;
	 SplitStringT(in_str, cSplit, true, &vecRet);

	 return vecRet;
}

//unicode版本
std::vector<std::wstring> common::utility::stringhelper::Split_String_by_Delimiter(const std::wstring& in_wStr, wchar_t cSplit)
{
	std::vector<std::wstring> vecRet;
	SplitStringT(in_wStr, cSplit, true, &vecRet);

	return vecRet;
}

namespace //anonymous namespace
{
	// Overloaded wrappers around vsnprintf and vswprintf. The buf_size parameter
	// is the size of the buffer. These return the number of characters in the
	// formatted string excluding the NUL terminator. If the buffer is not
	// large enough to accommodate the formatted string without truncation, they
	// return the number of characters that would be in the fully-formatted string
	// (vsnprintf, and vswprintf on Windows), or -1 (vswprintf on POSIX platforms).
	inline int vsnprintfT(char* buffer,
		                  size_t buf_size,
		                  const char* format,
		                  va_list argptr)
	{
		return common::base::vsnprintf(buffer, buf_size, format, argptr);
	}

	inline int vsnprintfT(wchar_t* buffer,
		                  size_t buf_size,
		                  const wchar_t* format,
		                  va_list argptr) 
	{
		return common::base::vswprintf(buffer, buf_size, format, argptr);
	}

	// Templatized back-end for StringPrintF/StringAppendF. This does not finalize
	// the va_list, the caller is expected to do that.
	template <class StringType>
	static void StringAppendVT(StringType* dst,
		                       const typename StringType::value_type* format,
		                       va_list ap) 
	{
			// First try with a small fixed size buffer.
			// This buffer size should be kept in sync with StringUtilTest.GrowBoundary
			// and StringUtilTest.StringPrintfBounds.
			typename StringType::value_type stack_buf[1024];

			va_list backup_ap;
			common::base::va_copy(backup_ap, ap);

			int result = vsnprintfT(stack_buf, arraysize(stack_buf), format, backup_ap);
			va_end(backup_ap);

			if (result >= 0 && result < static_cast<int>(arraysize(stack_buf)))
			{
				// It fit.
				dst->append(stack_buf, result);
				return;
			}

			// Repeatedly increase buffer size until it fits.
			int mem_length = arraysize(stack_buf);
			while (true) 
			{
				if (result < 0) 
				{
					// Try doubling the buffer size.
					mem_length *= 2;
				} 
				else 
				{
					// We need exactly "result + 1" characters.
					mem_length = result + 1;
				}

				if (mem_length > 32 * 1024 * 1024) 
				{
					// That should be plenty, don't try anything larger.  This protects
					// against huge allocations when using vsnprintfT implementations that
					// return -1 for reasons other than overflow without setting errno.
					printf("Unable to printf the requested string due to size.");
					return;
				}

				std::vector<typename StringType::value_type> mem_buf(mem_length);

				// Restore the va_list before we use it again.
				common::base::va_copy(backup_ap, ap);

				result = vsnprintfT(&mem_buf[0], mem_length, format, ap);
				va_end(backup_ap);

				if ((result >= 0) && (result < mem_length)) 
				{
					// It fit.
					dst->append(&mem_buf[0], result);
					return;
				}
			}
	}

    void StringAppendV(std::string* dst, const char* format, va_list ap) 
	{
	     StringAppendVT(dst, format, ap);
    }

    void StringAppendV(std::wstring* dst, const wchar_t* format, va_list ap)
	{
	     StringAppendVT(dst, format, ap);
    }
};


//ansi版本
std::string common::utility::stringhelper::FormatA(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	std::string result;
	StringAppendV(&result, format, ap);
	va_end(ap);
	return result;
}

//ansi版本
std::wstring common::utility::stringhelper::Format(const wchar_t* format, ...)
{
	va_list ap;
	va_start(ap, format);
	std::wstring result;
	StringAppendV(&result, format, ap);
	va_end(ap);
	return result;
}

//ansi转换为hex，以可以串联到url的参数中
std::string common::utility::stringhelper::HexEncodeCopy(const char* in_str, size_t size)
{
	if (in_str == NULL || size==0)
	{
		return std::string();
	}
	
	return HexEncodeCopy(std::string(in_str, in_str+size));
}

//参数为string
std::string common::utility::stringhelper::HexEncodeCopy(const std::string& in_str)
{
	static const char kHexChars[] = "0123456789ABCDEF";

	// Each input byte creates two output hex characters.
	size_t iSize = in_str.size();
	std::string ret(iSize*2, '\0'); //\0结束

	for (size_t i = 0; i < iSize; ++i) 
	{
		char b = in_str[i];
		ret[(i * 2)] = kHexChars[(b >> 4) & 0xf];
		ret[(i * 2) + 1] = kHexChars[b & 0xf];
	}
	return ret;
}

namespace
{
	template<class StringType>
	void DoReplaceSubstringsAfterOffset(StringType* str,
										typename StringType::size_type start_offset,
										const StringType& find_this,
										const StringType& replace_with,
										bool replace_all) 
	{
		if ((start_offset == StringType::npos) || (start_offset >= str->length()))
			return;

		assert(!find_this.empty());
		if (find_this.empty())
		{
			return;
		}
		
		for (typename StringType::size_type offs(str->find(find_this, start_offset));
											offs != StringType::npos; 
											offs = str->find(find_this, offs)) 
		{
			str->replace(offs, find_this.length(), replace_with);
			offs += replace_with.length();

			if (!replace_all)
			{
				break;
			}
		}
	}
};

std::string common::utility::stringhelper::XMLEncodeA(const std::string& in_xmlStr)
{
	std::string src_str = in_xmlStr;
	std::string src_substr = "&";
	std::string rep_substr = "&amp;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = ">";
	rep_substr = "&gt;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "<";
	rep_substr = "&lt;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "'";
	rep_substr = "&apos;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "\"";
	rep_substr = "&quot;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	return src_str;
}

std::wstring common::utility::stringhelper::XMLEncode(const std::wstring& in_xmlStr)
{
	std::wstring src_str = in_xmlStr;
	std::wstring src_substr = L"&";
	std::wstring rep_substr = L"&amp;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L">";
	rep_substr = L"&gt;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"<";
	rep_substr = L"&lt;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"'";
	rep_substr = L"&apos;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"\"";
	rep_substr = L"&quot;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	return src_str;
}

std::string common::utility::stringhelper::XMLDecodeA(const std::string& in_xmlStr)
{
	std::string src_str = in_xmlStr;
	std::string src_substr = "&amp;";
	std::string rep_substr = "&";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "&gt;";
	rep_substr = ">";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "&lt;";
	rep_substr = "<";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "&apos;";
	rep_substr = "'";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = "&quot;";
	rep_substr = "\"";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	return src_str;
}

std::wstring common::utility::stringhelper:: XMLDecode(const std::wstring& in_xmlStr)
{
	std::wstring src_str = in_xmlStr;
	std::wstring src_substr = L"&amp;";
	std::wstring rep_substr = L"&";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"&gt;";
	rep_substr = L">";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"<";
	rep_substr = L"&lt;";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"&apos;";
	rep_substr = L"'";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	src_substr = L"&quot;";
	rep_substr = L"\"";
	DoReplaceSubstringsAfterOffset(&src_str, 0, src_substr, rep_substr, true);

	return src_str;
}

void common::utility::stringhelper::Replace(
	std::wstring &str, 
	const std::wstring &strSrc, 
	const std::wstring &strDst
	)
{
	return DoReplaceSubstringsAfterOffset(&str, 0, strSrc, strDst, true);
}

void common::utility::stringhelper::Replace(
	std::string &str, 
	const std::string &strSrc, 
	const std::string &strDst
	)
{
	return DoReplaceSubstringsAfterOffset(&str, 0, strSrc, strDst, true);
}

std::string common::utility::stringhelper::UrlDecode( const std::string& encoded_url )
{
	std::string url;
	for (int i = 0; i < encoded_url.length(); ++i)
	{
		if (encoded_url[i] == '+') 
		{
			url.push_back(' ');
		}
		else if (encoded_url[i] == '%' && i + 2 < encoded_url.length() 
			&& isxdigit(encoded_url[i+1]) && isxdigit(encoded_url[i+2]))
		{
			int value = (char2hex(encoded_url[i+1]) << 4) + char2hex(encoded_url[i+2]);
			url.push_back((char)value);
			i += 2;
		}
		else
		{
			url.push_back(encoded_url[i]);
		}
	}
	return url;
}

std::wstring common::utility::stringhelper::UrlDecodeW( const std::wstring& encoded_url )
{
	std::string sUrl = UnicodeToAscii(encoded_url);
	std::string srs = UrlDecode(sUrl);
	return AsciiToUnicode(srs);
}

std::string common::utility::stringhelper::UrlEncode( const std::string& url )
{
	const static char hex_map[] = "0123456789ABCDEF";
	std::string encoded_url;
	for (int i = 0; i < url.length(); ++i)
	{
		char c = url[i];
		// is alpha
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9')
		{
			encoded_url.push_back(c);
		}
		else
		{
			encoded_url.push_back('%');
			encoded_url.push_back(hex_map[(c&0xFF)>>4]);
			encoded_url.push_back(hex_map[c&0x0F]);
		}
	}
	return encoded_url;
}

std::wstring common::utility::stringhelper::UrlEncodeW( const std::wstring& url )
{
	std::string sUrl = UnicodeToAscii(url);
	std::string srs = UrlEncode(sUrl);
	return AsciiToUnicode(srs);
}


std::string common::utility::stringhelper::UrlUTF8Encode(const std::string& url)
{
	const static char hex_map[] = "0123456789ABCDEF";
	std::string encoded_url;
	for (int i = 0; i < url.length(); ++i)
	{
		char c = url[i];
		// is alpha
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9')
		{
			encoded_url.push_back(c);
		}
		else if(isSpace(c))
		{
			encoded_url.push_back('+');
		}
		else
		{
			encoded_url.push_back('%');
			encoded_url.push_back(hex_map[(c&0xFF)>>4]);
			encoded_url.push_back(hex_map[c&0x0F]);
		}
	}
	return encoded_url;

}
std::wstring common::utility::stringhelper::UrlUTF8EncodeW(const std::wstring& url)
{
	std::string sUrl = UnicodeToUTF8(url);
	std::string srs = UrlUTF8Encode(sUrl);
	return AsciiToUnicode(srs);

}

int common::utility::stringhelper:: isSpace(int c)
{
	switch (c)
	{
	  case 0x20:
	  case 0xD:
	  case 0xA:
	  case 0x9:
	  return 1;
	}
	return 0;
}