/**
 *
 * @file     str.c
 * @author   Aleix Conchillo Flaque <aleix@member.fsf.org>
 * @date     Mon Nov 25, 2002 23:32
 * @brief    expat string functions
 *
 * $Id: str.c,v 1.1 2004/01/28 00:43:21 aleix Exp $
 *
 * @if copyright
 *
 * Copyright (C) 2002, 2003 Aleix Conchillo Flaque
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
#include <stdio.h>
#include "utility\expat++\expat_str.h"
#include <assert.h>
#include <stdlib.h>
#include <string>
#include "utility\stringhelper.h"

namespace common { namespace utility {

using namespace std;



XML_Char*
expat_strdup(XML_Char const* src)
{
    unsigned int len = 0;
    XML_Char* out = NULL;

    assert(src != NULL);

    len = (unsigned int)expat_strlen(src);
    out = (XML_Char*) calloc( len + 1, sizeof(XML_Char));
    return (XML_Char*) expat_memcpy(out, (XML_Char*) src, len);
}

void
expat_strtrim(XML_Char* src)
{
    unsigned int start = 0;
    unsigned int end = 0;
    unsigned int total = 0;

    assert(src != NULL);

    end = (unsigned int)expat_strlen(src);

    /* strip trailing whitespace */
    while (end > 0 && expat_isspace(src[end-1]))
    {
        src[--end] = '\0';
    }

    /* strip leading whitespace */
    start = (unsigned int)expat_strspn(src, _XT(" \n\r\t\v") );
    total = end - start;
    expat_memmove(src, &src[start], total);
    src[total] = _XT('\0');
}


unsigned int	
expat_get_file_format( XML_Char const*	w_file_name)
{
	FILE* in = NULL;
	unsigned int n;

	if( w_file_name == NULL)
		return EXPAT_UNKNOWN;

	in = _wfopen( w_file_name, L"rb");
	if ( in == NULL)
		return EXPAT_UNKNOWN;

	n =  expat_get_file_format_from_fp(in);
	fclose(in);

	return n;
}

unsigned int	
expat_get_file_format_from_fp( FILE*	fp)
{
	char buffer[10];
	size_t len;

	memset(buffer, 0x0, 10);

	len = fread(buffer, 1, 10, fp);
	if (ferror(fp))
		return EXPAT_UNKNOWN;

	/* first three bytes  EF BB BF          UTF-8*/
	if( len >= 3 && (unsigned char)buffer[0] == 0xEF 
		&& (unsigned char)buffer[1] == 0xBB 
		&& (unsigned char)buffer[2] == 0xBF)
		return UTF8;

	/* FE EF UTF-16/UCS-2, little endian*/
	if( len >= 2 && (unsigned char)buffer[0] == 0xFE && (unsigned char)buffer[1] == 0xFF)
		return UTF16_LE;

	/* FF FE UTF-16/UCS-2, big endian*/
	if( len >= 2 && (unsigned char)buffer[0] == 0xFF && (unsigned char)buffer[1] == 0xFE)
		return UTF16_BE;

	/* FF FE 00 00 UTF-32/UCS-4, little endian*/
	if( len >= 4 && (unsigned char)buffer[0] == 0xFF && (unsigned char)buffer[1] == 0xFE 
		&& (unsigned char)buffer[2] == 0 && (unsigned char)buffer[3] == 0)
		return UTF32_LE;

	/* 00 00 FE FF UTF-32/UCS-4, big endian*/
	if( len >= 4 && (unsigned char)buffer[0] == 0x00 && (unsigned char)buffer[1] == 0x00 
		&& (unsigned char)buffer[2] == 0xFE && (unsigned char)buffer[3] == 0xFF)
		return UTF32_BE;

	return EXPAT_UNKNOWN;
}

XML_String
expat_strreplace( const XML_Char* src, XML_Char* replaced_str, XML_Char* replace_str)
{
	XML_String strSrc = src;

#ifdef XML_UNICODE_WCHAR_T
	common::utility::stringhelper::Replace( strSrc, replaced_str, replace_str);
#else
	common::utility::stringhelper::Replace( strSrc, replaced_str, replace_str);
#endif

	return strSrc;
}
}}