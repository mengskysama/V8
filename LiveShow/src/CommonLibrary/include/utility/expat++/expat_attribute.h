/**
*
* @file     expat_attribute.h
* @author   tingya zhang <flydish@gmail.com>
* @date     Thu Dec 25, 2008 00:39
* @brief    Expat++ attribute type declaration
*
*
* @if copyright
*
* Copyright (C) 2008-2009,51.com
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
#include "expat_str.h"
#include <string>
#include "../../Common.h"

namespace common { namespace utility {
//==================================================================
//            expat_attribute class
//==================================================================
class expat_element;
class COMMON_INTERFACE expat_attribute
{
public:
	expat_attribute(void);
	expat_attribute( XML_String const name,  XML_String const value, expat_element* element);
	~expat_attribute(void);

	/*
	 * set the attribute name
	 * @param	name [in]	the attribute_name
	 * @return	no return
	 */
	void	set_attribute_name( const XML_String name);

	/**
	 *	set the attribute value
	 *	@param	value [in]	the attribute value 
	 *	@return	no return;
	 */
	void	set_attribute_value( const XML_String value);

	/**
	 *	set the element of the attribute
	 *	@param	element	[in]	the element which the attribute owns
	 */
	void	set_element( expat_element* element);

	/**
	 *	get the name of current attribute 
	 */
	const XML_String	get_attribute_name();

	/** 
	 *	get the value of current attribute
	 */
	const XML_String	get_attribute_value();

	/** 
	 *	get the element of current attribute
	 */
	expat_element*	get_element();	

	/** 
	 *	print the attribute in file
	 */
	void	print( FILE* out );


	expat_attribute*	prev;
	expat_attribute*	next;

private:

	XML_String m_name;			
	XML_String m_value;

	expat_element*		m_elememt;
};


//==================================================================
//            expat_attribute_list class
//==================================================================

class COMMON_INTERFACE expat_attribute_list
{
public:
	expat_attribute_list();
	~expat_attribute_list();

	/**
	 *	add a new attribute to list
	 *	@param	attribute [in]	the attribute to be added to list
	 *	@return	return NULL if add failed, or return the added attribute;
	 */
	expat_attribute*	add_attribute( expat_attribute*	attribute);

	/**
	 *	delete the attribute from the attribute list   
	 *	@param	name [in]	the attribute name that will be deleted
	 */
	void	delete_attribute( XML_String const name);

	/**
	 *	get the attribute in the list by index
	 *	@param	idx	[in]	the attribute index ,based from 0;
	 *  @return	if no attribute, return NULL
	 */
	expat_attribute*	get_attribute_by_index( unsigned int idx);

	/**
	 *	get the first attribute in the list by name
	 *	@param	name [in]	the attribute name 
	 *  @return	if no attribute, return NULL
	 */
 
	expat_attribute*	get_attribute_by_name( XML_String const  name);

	/**
	 *	get the account of current attribute list  
	 *	@return	the count of attribute in current list
	 */
	unsigned int	get_attribute_count();

	/**
	 *	print the attribute list into file out   
	 */
	void	print( FILE* out);

private:

	unsigned int size;
	expat_attribute* first;
	expat_attribute* last;
};

}}