/**
*
* @file     expat_document.h
* @author   tingya zhang <flydish@gmail.com>
* @date     Thu Dec 25, 2008 00:39
* @brief    Expat++ document type declaration
*
*
* @if copyright
*
* Copyright (C) 2002, 2003
*
* Expat++ is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* Expat++ is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* @endif
*
* Attribute related functions. Expat++ provides functions to access and
* manipulate the attributes of all the elements in a tree. XML element
* attributes are basically a name-value pair.
*
* The Expat++ is the C++ version of Expat.
*/

#pragma once

#include "../../../thirdlibrary/expat/expat.h"
#include <stdio.h>
#include <stdlib.h>
#include "expat_error.h"
#include "expat_tree.h"
#include "expat_element.h"
#include <string>
#include "expat_str.h"
#include "../../Common.h"

namespace common { namespace utility {

using namespace std;

class expat_document;
class expat_error;

/* Stack to keep previous parsed elements */
typedef struct _stack_element
{
	expat_element* element;
	struct _stack_element* prev;

} stack_element;

/* Pushes an element into the stack. */
stack_element*
stack_push(stack_element** stack, expat_element* element);

/* Pops an element from the stack. */
expat_element*
stack_pop(stack_element** stack);

/**
* Callback function type.
*
* @return 1 if callback call had no errors, 0 otherwise.
*/
typedef unsigned int EXPAT_CALLBACK(expat_document* document);


class COMMON_INTERFACE expat_document
{
public:
	expat_document();
	virtual ~expat_document();

	/**
	 *	load xml from file
	 *	@w_file_name [in]	the file name 
	 *  @return if success,return 1 or return 0
	 */
	int load_from_file( const wchar_t* w_file_name);

	/**
	 *	load xml from file
	 *	@fp [in]		the FILE desc 
	 *  @return if success,return 1 or return 0
	 */
	int	load_from_fp( FILE*	fp, unsigned int file_format);

	/**
	 * load xml from buffer   
	 * @param	buffer [in]	xml buffer 
	 */
	int load_from_buffer( const char* buffer, unsigned int	buffer_size);

	/**
	 *  save the xml tree to a file
	 */
	bool save_to_file( const char* file_name);
	bool save_to_file( const wchar_t* w_file_name);

	/**
	 *	get the root element of current document
	 */
	expat_element*	get_root();
	
	/**
	 *  add an element named root_name as the root element     
	 *	@param	root_name	the element name that will be added as the root
	 */ 
	expat_element*	add_root( XML_Char const* root_name);

	/**
	 * when parse failed, all the error msg save in expat_error,
	 *	the method return the error msg;
	 */
	expat_error*	get_error();

private:

	void	print( FILE* out);

public:

	expat_tree*		m_tree;
	XML_Parser		m_parser;
	int				m_ignore_whitespaces; /* 1 if ignore white spaces, 0 otherwise */
	EXPAT_CALLBACK*	m_stream_callback; /* Function to call while reading streams */
	expat_element*	m_current;  /* Current parsed element */
	stack_element*	m_stack;	/* Current parsed element stack, when a element such as <a> meet , the element a 
								 * will be put into the stack , and when </a> meet, the <a> in stack will be poped
								 */
	static XML_String		m_innerxml;	

private:

	expat_error*	m_error;
};

}}