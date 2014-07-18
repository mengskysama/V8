#include "StdAfx.h"
#include "utility\expat++\expat_attribute.h"
#include "utility\expat++\expat_element.h"
#include "utility\expat++\expat_str.h"
#include <assert.h>
#include "utility\expat++\expat_document.h"
#include "utility\StringHelper.h"

using namespace common::utility::stringhelper;

namespace common { namespace utility {

//==================================================================
//            implementation of expat_attribute class
//==================================================================

expat_attribute::expat_attribute(void)
{
	prev = NULL;
	next = NULL;

	set_attribute_name(_XT(""));
	set_attribute_value(_XT(""));
	set_element(NULL);
}

expat_attribute::expat_attribute(  XML_String const name,  XML_String const value, expat_element* element)
{
	m_elememt = NULL;

	prev = NULL;
	next = NULL;

	set_attribute_name( name);
	set_attribute_value( value);
	set_element( element);
}

expat_attribute::~expat_attribute(void)
{
}

void
expat_attribute::set_attribute_name( const XML_String name)
{
	m_name	=	name;
}


const 
XML_String	expat_attribute::get_attribute_name()
{
	return m_name;
}

void	
expat_attribute::set_attribute_value( const XML_String value)
{
	m_value = value;
}

void	
expat_attribute::set_element( expat_element* element)
{
	m_elememt = element;
}

const XML_String	
expat_attribute::get_attribute_value()
{
	return m_value;;
}

expat_element*	
expat_attribute::get_element()
{
	return m_elememt;
}

void	
expat_attribute::print( FILE* out )
{
	expat_document::m_innerxml += _XT(" ");
	expat_document::m_innerxml += m_name;
	expat_document::m_innerxml += _XT("=\"");
	expat_document::m_innerxml += m_value;
	expat_document::m_innerxml += _XT("\" ");
}


//==================================================================
//            implementation of expat_attribute_list class
//==================================================================

expat_attribute_list::expat_attribute_list()
{
	size = 0;
	first = NULL;
	last = NULL;
}

expat_attribute_list::~expat_attribute_list()
{
	expat_attribute* it = NULL;
	expat_attribute* tmp = NULL;

	it = first;
	while (it != NULL)
	{
		tmp = it;
		it = it->next;
		
		delete tmp;
	}
}

expat_attribute*	
expat_attribute_list::add_attribute( expat_attribute*	attribute)
{
	expat_attribute* aux = NULL;

	assert(attribute != NULL);

	/* if the attribute with the name attribute->name exist,
	 * insert it before the attribute ,and delete the old one
	 */
	aux = get_attribute_by_name( attribute->get_attribute_name());
	if (aux != NULL)
	{
		if (aux->prev != NULL)
		{
			aux->prev->next = attribute;
		}
		if (aux->next != NULL)
		{
			aux->next->prev = attribute;
		}
		if ( first == aux)
		{
			first = attribute;
		}
		if ( last == aux)
		{
			last = attribute;
		}
		attribute->prev = aux->prev;
		attribute->next = aux->next;

		delete aux;

		return attribute;
	}

	/*if the attribute with the name attribute->name doesn't exist
	 *insert it direcotyr
	 */
	size++;
	if ( first == NULL)
	{
		first = attribute;
	}
	else
	{
		last->next = attribute;
		attribute->prev = last;
	}
	last = attribute;

	return attribute;
}

void	
expat_attribute_list::delete_attribute( XML_String const name)
{
	expat_attribute* attribute = NULL;
	expat_attribute* tmp_prev = NULL;
	expat_attribute* tmp_next = NULL;

	if (name == _XT(""))
	{
		return;
	}

	attribute = get_attribute_by_name( name);

	if (attribute != NULL)
	{
		tmp_prev = attribute->prev;
		tmp_next = attribute->next;
		if (tmp_prev != NULL)
		{
			tmp_prev->next = tmp_next;
		}
		if (tmp_next != NULL)
		{
			tmp_next->prev = tmp_prev;
		}

		if (attribute == first)
		{
			first = tmp_next;
		}

		if (attribute == last)
		{
			last = tmp_prev;
		}

		delete attribute;
		size--;
	}
}

expat_attribute*	
expat_attribute_list::get_attribute_by_index( unsigned int idx)
{
	if(idx >= size)
		return NULL;

	expat_attribute* attribute = first;
	for (unsigned int i = 0; (i < idx) && (attribute != NULL); ++i)
	{
		attribute = attribute->next;
	}

	return attribute;
}

expat_attribute*	
expat_attribute_list::get_attribute_by_name( XML_String const name)
{
	if (name == _XT(""))
	{
		return NULL;
	}

	expat_attribute* attribute = first;
	while (attribute && ( stringhelper::MakeLowerCopy(attribute->get_attribute_name()) != stringhelper::MakeLowerCopy(name)))
	{
		attribute = attribute->next;
	}

	return attribute;
}

unsigned int	
expat_attribute_list::get_attribute_count()
{
	return size;	
}

void	
expat_attribute_list::print( FILE* out)
{
	for( unsigned int i = 0; i < size; i++)
	{
		expat_attribute* pattribute = get_attribute_by_index(i);
		if( pattribute)
		{
			pattribute->print( out);
		}
	}
}

}}