/**
 *
 * @file     str.h
 * @author   Aleix Conchillo Flaque <aleix@member.fsf.org>
 * @date     Sun Dec 01, 2002 13:05
 * @brief    expat string functions
 *
 * $Id: str.h,v 1.2 2004/01/29 00:34:38 aleix Exp $
 *
 * @if copyright
 *
 * Copyright (C) 2002, 2003, 2004 Aleix Conchillo Flaque
 *
 * expat is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * expat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @endif
 */


#pragma once

#include "../../../thirdlibrary/expat/expat.h"
#include <string>

namespace common { namespace utility {

using namespace std;

#define expat_memcpy(dst,src,n)  memcpy(dst,src,sizeof(XML_Char)*(n))
#define expat_memmove(dst,src,n) memmove(dst,src,sizeof(XML_Char)*(n))

#ifdef XML_UNICODE_WCHAR_T

#include <wchar.h>
#include <wctype.h>

#define _XT(str)                L##str
#define XML_String				std::wstring

#define expat_fprintf            fwprintf
#define expat_sprintf            swprintf

#define expat_strcmp(s1,s2)      wcscmp(s1,s2)
#define expat_stricmp(s1,s2)	 _wcsicmp(s1,s2)
#define expat_strspn(s1,s2)      wcsspn(s1,s2)
#define expat_strcpy(s1,s1_n,s2,s2_n)	wcsncpy_s(s1,s1_n,s2, s2_n)
#define expat_strcat(s1,s2)				wcscat_s(s1,s1_n,s2)
#define expat_strncpy(s1,s1_n,s2,s2_n)	wcsncpy_s(s1,s1_n,s2,s2_n)
#define expat_strncat(s1,s1_n,s2,s2_n)	wcsncat_s(s1,s1_n,s2,s2_n)
#define expat_strlen(s)					wcslen(s)

#define expat_isalnum(c)         iswalnum((c))
#define expat_isalpha(c)         iswalpha((c))
#define expat_iscntrl(c)         iswcntrl((c))
#define expat_isdigit(c)         iswdigit((c))
#define expat_isxdigit(c)        iswxdigit((c))
#define expat_isgraph(c)         iswgraph((c))
#define expat_islower(c)         iswlower((c))
#define expat_isupper(c)         iswupper((c))
#define expat_isprint(c)         iswprint((c))
#define expat_ispunct(c)         iswpunct((c))
#define expat_isspace(c)         iswspace((c))

#else /* not XML_UNICODE_WCHAR_T */

#include <ctype.h>

#define _XT(str)                str
#define XML_String				std::string

#define expat_fprintf            fprintf
#define expat_sprintf            sprintf

#define expat_strcmp(s1,s2)      strcmp(s1,s2)
#define expat_stricmp(s1,s2)	 _stricmp(s1,s2)
#define expat_strspn(s1,s2)      strspn(s1,s2)
#define expat_strcpy(s1,s1_n,s2,s2_n)	strcpy_s(s1,s1_n,s2,s2_n)
#define expat_strcat(s1,s1_n,s2,s2_n)	strcat_s(s1,s1_n,s2,s2_n)
#define expat_strncpy(s1,s1_n,s2,s2_n)	strncpy_s(s1,s1_n,s2,s2_n)
#define expat_strncat(s1,s1_n,s2,s2_n)	strncat_s(s1,s1_n,s2,s2_n)
#define expat_strlen(s)          strlen(s)

#define expat_isalnum(c)         isalnum((unsigned char)(c))
#define expat_isalpha(c)         isalpha((unsigned char)(c))
#define expat_iscntrl(c)         iscntrl((unsigned char)(c))
#define expat_isdigit(c)         isdigit((unsigned char)(c))
#define expat_isxdigit(c)        isxdigit((unsigned char)(c))
#define expat_isgraph(c)         isgraph((unsigned char)(c))
#define expat_islower(c)         islower((unsigned char)(c))
#define expat_isupper(c)         isupper((unsigned char)(c))
#define expat_isprint(c)         isprint((unsigned char)(c))
#define expat_ispunct(c)         ispunct((unsigned char)(c))
#define expat_isspace(c)         isspace((unsigned char)(c))

#endif /* XML_UNICODE_WCHAR_T */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Creates a new copy of the given string. Client must free it.
 */
extern XML_Char*
expat_strdup(XML_Char const* src);

/**
 * Trims off extra spaces from the beginning and end of a string.
 */
extern void
expat_strtrim(XML_Char* src);

/**
* the file format   
*/
#define EXPAT_UNKNOWN		0
#define UTF8		1		//	UTF-8
#define UTF16_LE	2		//	UTF-16/UCS-2	little endian
#define UTF16_BE	3		//	UTF-16/UCS-2	big endian
#define UTF32_LE	4		//	UTF-32/UCS-4	little endian
#define UTF32_BE	5		//	UTF-32/UCS-4	big endian

extern unsigned int	
expat_get_file_format(XML_Char const*	w_file_name);

extern unsigned int	
expat_get_file_format_from_fp( FILE*	fp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

}}