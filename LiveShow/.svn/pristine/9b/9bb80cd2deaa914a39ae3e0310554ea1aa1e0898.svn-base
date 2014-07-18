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
* Copyright (C) 2008-2009, www.51.com, rainbow project
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

#include "expat_str.h"
#include "expat_attribute.h"
#include "../../Common.h"

namespace common { namespace utility {

class expat_document;

class COMMON_INTERFACE expat_element
{
public:
	expat_element(void);
	expat_element( XML_String const name);
	~expat_element(void);

	/* 
	 *	get the element name of current
	 *	@return name of current element
	 */
	XML_String const	get_element_name();

	/* 
	 *	get the element contents of current
	 *	@return	the contents of element
	 */
	XML_String const	get_element_contents();

	/* 
	 *	set the element name of current
	 *	@param	name [in] name of element
	 */
	void	set_element_name( XML_String const	name);

	/*
	 *	set the element contents of current
	 *	@param contents [in] the contents of current element
	 */
	void	set_element_contents( XML_String const contents);

	/* 
	 *	get the child elements count of current element
	 */
	unsigned int	get_elements_count();

	/* 
	 *	get the first child element of current element
	 *	@return	if no child element, return NULL
	 */
	expat_element*	get_first_child_element();

	/* 
	 *	get the first child element of current element with name
	 *	@param	name [in]	the name of child element 
	 *	@return	if no child ,return NULL;
	 */
	expat_element*	get_first_child_element( XML_String const name);

	/* 
	 *	get the next sibling element of current element, even the element name is
	 *	not same with current element
	 */
	expat_element*	get_sibling_element();

	/* 
	 *  get the next sibling element of current element, the element name is
	 *  same with current element
	 *  @param name	[in] the name of sibling element
	 *	@return	if no element,return NULL
	 */
	expat_element*	get_sibling_element( XML_String const	name);

	/* 
	 *	get the child element which index is idx, idx is base on 0 
	 *	@param	idx [in]	the index of element you should get
	 *	@return	if no element, return NULL
	 */
	expat_element*	get_element_by_index( unsigned int idx);

	/* 
	 *	get the first child element with the name
	 *	@param	name [in]	the name of element you should get
	 *	@param	if no element, return NULL
	 */
	expat_element*	get_element_by_name( XML_String const	name);

	/* 
	 *	get the child element's contents of current element
	 *	the function only return the first child with the name
	 *	@param	name [in]	the name of child element
	 *	@return	the contents of child element, if no element, return ""
	 */
	XML_String const	get_child_element_contents_by_name( XML_String const name);

	/* 
	 *	get the child element's attribute of current element
	 *	the function only return the first child with the name
	 *	@param	ename [in]	the name of child element
	 *	@param	attr_name [in]	the attrbiute_name of child_element
	 *	@return	the attribute value, if no attribute, return ""
	 */
	XML_String const get_child_element_attribute_value_by_name( XML_String const ename, XML_String const attr_name);

	/* 
	 *	get the next element
	 *	@param	bchild [in] if bchild = true, get the first child child, or get sibling element
	 *	@return	if no element ,return NULL
	 */
	expat_element*	get_next_element( bool bchild);

	/** 
	 *	append a new element as the child of current element,
	 *	the new_element will be at the end of all these child elements
	 *	@param	new_element	[in]	the element appended
	 *	@return	if append failed, return NULL, or return point to the new element
	 */
	expat_element*	append_child_element( expat_element* new_element);

	/** 
	 *	append a new element as the child of current element,
	 *	the new_element will be at the end of all these child elements
	 *	@param	name	[in]	the name of element appended
	 *	@return	if append failed, return NULL, or return point to the new element
	 */
	expat_element*	append_child_element( XML_String const name);

	/* 
	 *	add the new element as the first child of current element
	 *	@param	new_element	[in]	the new element to be added as first child
	 *	@return	if	add failed, return NULL, or return point to the new element
	 */
	expat_element*	add_child_element( expat_element*	new_element);

	/* 
	 *	add the new element as the first child of current element
	 *	@param	new_element	[in]	the name of new element to be added as first child
	 *	@return	if	add failed, return NULL, or return point to the new element
	 */
	expat_element*	add_child_element( XML_String const name);

	/* 
	 *	delete all the child element of current element
	 */
	void	delete_element( );

	/* 
	 *	delete the element with index idx of all the child of current elements
	 */
	void	delete_element_by_index( unsigned int idx);

	/*	
	 *	delete the first child element with the name pname of current element
	 *	@param	name [in]	the name of element to be deleted
	 */
	void	delete_element_by_name( XML_String const	name);

	/* 
	 *	delete all the child element with pname of current element
	 *	@param	name [in]	the name of element to be deleted
	 */
	void	delete_all_elements_by_name( XML_String const  name);

	/* 
	 *	get attribute list associated with current element
	 */
	expat_attribute_list*	get_attribute_list();

	/* 
	 *	get the attribute by name associated with current element 
	 *	@param	attributename [in]	name of attribute to be get
	 *	@return	if no such attribute, return NULL
	 */
	expat_attribute*	get_attribute_by_name( XML_String const attributename);

	/* 
	 *	get the attribute by index associated with current element
	 *	@param	idx	[in] index of attribute in current element
	 *	@return	if no such attribute, return NULL
	 */
	expat_attribute*	get_attribute_by_index( unsigned int idx);

	/* 
	 *	get the attribute value of current element with attribute name
	 *	@param	attribute_name [in]	attribute name to be get
	 *	@return	attribute value
	 */
	XML_String const	get_attribute_value( XML_String const attribute_name);

	/* 
	 *	add a new attribute to current element as first attribute
	 *	@param	pattribute [in]	add pattribute as the first attribute in attribute list of current element
	 *	@return	the point [in] to attribute added
	 */
	expat_attribute*	add_attribute( expat_attribute* pattribute);

	/* 
	 *	add a new attribute to current element as first attribute
	 *	@param	name [in]	the name of attribute to be added
	 *	@param	value [in]	the value of attrbute	to	be added
	 *	@return	the point to attribute added
	 */
	expat_attribute*	add_attribute( XML_String const name, XML_String const value);

	/* print the element into file out*/
	void	element_print( FILE* out, unsigned int indent);

private:
	
	void	indent_print( FILE* out, unsigned int indent);


public:
	XML_String	m_name;
	XML_String	m_contents;
	expat_attribute_list*	m_attribute_list;

	unsigned int m_nchildren;

	/* Immediately neighbour pointers */
	expat_element* m_parent;
	expat_element* m_child;
	expat_element* m_left;
	expat_element* m_right;

	/* Last element in list */
	expat_element* m_last_child;
};
}}