#include "StdAfx.h"
#include "utility\expat++\expat_element.h"
#include "utility\expat++\expat_str.h"
#include "utility\expat++\expat_document.h"
#include "../../include/utility/StringHelper.h"
#include "utility\StringHelper.h"

using namespace common::utility::stringhelper;

namespace common { namespace utility {

static int const indent_size = 3;

expat_element::expat_element(void)
{
	m_attribute_list = NULL;
	m_nchildren = 0;

	m_parent = NULL;
	m_child = NULL;
	m_left = NULL;
	m_right = NULL;
	m_last_child = NULL;
}

expat_element::expat_element( XML_String const name)
{
	m_attribute_list = new expat_attribute_list();
	m_nchildren = 0;

	m_parent = NULL;
	m_child = NULL;
	m_left = NULL;
	m_right = NULL;
	m_last_child = NULL;

	m_name = name;
}

/* delete current element*/
expat_element::~expat_element(void)
{
	if( m_attribute_list)
	{
		delete m_attribute_list;
		m_attribute_list = NULL;
	}
}

XML_String const	
expat_element::get_element_name()
{
	return m_name;
}

XML_String const	
expat_element::get_element_contents()
{
	return m_contents;
}

void	
expat_element::set_element_name( XML_String	const	name)
{
	m_name = name;
}

void	
expat_element::set_element_contents( XML_String const contents)
{
	m_contents = contents;
}

unsigned int	
expat_element::get_elements_count()
{
	if( m_parent)
		return m_parent->m_nchildren;

	return 0;
}

expat_element*	
expat_element::get_first_child_element()
{
	return get_element_by_index( 0);
}

expat_element*	
expat_element::get_first_child_element( XML_String const name)
{
	expat_element*	pelement = get_first_child_element();
	while( pelement)
	{
		if( stringhelper::MakeLowerCopy(pelement->get_element_name()) == stringhelper::MakeLowerCopy(name))
			return pelement;

		pelement = pelement->get_sibling_element();
	}

	return NULL;
}

expat_element*	
expat_element::get_sibling_element()
{
	return m_right;
}

expat_element*	
expat_element::get_sibling_element( XML_String const	name)
{
	expat_element*	pelement = get_sibling_element();
	while( pelement)
	{
		if( stringhelper::MakeLowerCopy(pelement->get_element_name()) == stringhelper::MakeLowerCopy(name ))
			return pelement;

		pelement = pelement->get_sibling_element();
	}

	return NULL;
}

expat_element*	
expat_element::get_element_by_index( unsigned int idx)
{
	unsigned int i = 0;

	if(idx >= m_nchildren)	return NULL;

	expat_element* element = m_child;
	for (i = 0; (i < idx) && (element != NULL); ++i)
	{
		element = element->m_right;
	}

	return element;
}

expat_element*	
expat_element::get_element_by_name( XML_String const name)
{
	//unsigned int i = 0;

	expat_element* element = get_first_child_element();
	while (element && ( stringhelper::MakeLowerCopy(element->m_name) != stringhelper::MakeLowerCopy(name)))
	{
		element = element->get_sibling_element();
	}

	return element;
}

XML_String const	
expat_element::get_child_element_contents_by_name( XML_String const name)
{
	expat_element* pChild = get_element_by_name(name);
	if( pChild == NULL)
		return _XT("");

	return pChild->get_element_contents();
}

XML_String const 
expat_element::get_child_element_attribute_value_by_name( XML_String const ename, XML_String const attr_name)
{
	expat_element* pChild = get_element_by_name(ename);
	if( pChild == NULL)
		return _XT("");

	expat_attribute* pAttr =  pChild->get_attribute_by_name(attr_name);
	if( !pAttr)
		return _XT("");

	return pAttr->get_attribute_value();
}


expat_element*	
expat_element::get_next_element( bool bchild)
{
	if( bchild)
		return m_child;

	return m_right;
}


/* add new_element as the child of current element*/
expat_element*	
expat_element::append_child_element( expat_element* new_element)
{
	if( new_element == NULL)	return NULL;

	m_nchildren++;
	new_element->m_parent = this;

	/* be first child element*/
	if (m_child == NULL)
	{
		m_child = new_element;
	}
	else
	{
		/* append the new element at the end */
		expat_element* current = m_last_child;
		current->m_right = new_element;
		new_element->m_left = current;
	}
	m_last_child = new_element;

	return new_element;
}

expat_element*	
expat_element::append_child_element( XML_String const name)
{
	expat_element* pelement = new expat_element( name);
	if( pelement == NULL )
		return NULL;

	return append_child_element( pelement);
}

expat_element*	
expat_element::add_child_element( expat_element*	new_element)
{
	if (!new_element)	return NULL;

	m_nchildren++;
	new_element->m_parent = this;

	/* be first child element*/
	if (m_child == NULL)
	{
		m_child = new_element;
	}
	else
	{
		/* append the new element at the end */
		expat_element*	current = m_child;

		new_element->m_right = current;
		current->m_left = new_element;

		m_child = new_element;
	}

	return new_element;
}

expat_element*	
expat_element::add_child_element( XML_String const name)
{
	expat_element*	pelement = new expat_element( name);
	if( pelement == NULL)
		return NULL;

	return add_child_element(pelement);
}

void	
expat_element::delete_element()
{
	expat_element* pleft = m_left;
	expat_element* pright = m_right;

	if ( pleft != NULL)
	{
		pleft->m_right = m_right;
	}
	if ( pright != NULL)
	{
		pright->m_left = m_left;
	}

	if ( m_parent != NULL)
	{
		if ( m_parent->m_child == this)
		{
			m_parent->m_child = m_right;
		}
		if ( m_parent->m_last_child == this)
		{
			m_parent->m_last_child = m_left;
		}
		m_parent->m_nchildren--;
	}

	while (m_child != NULL)
	{
		m_child->delete_element();
	}

	delete this;
}

void	
expat_element::delete_element_by_index( unsigned int idx)
{
	expat_element*	pelement = get_element_by_index(idx);
	pelement->delete_element();
}

void	
expat_element::delete_element_by_name( XML_String const	name)
{
	expat_element*	pelement = get_element_by_name( name);
	pelement->delete_element();
}

void	
expat_element::delete_all_elements_by_name( XML_String const name)
{

}

expat_attribute_list*	
expat_element::get_attribute_list()
{
	return m_attribute_list;
}

expat_attribute*	
expat_element::get_attribute_by_name( XML_String const attributename )
{
	expat_attribute_list*	plist = get_attribute_list();
	if( plist == NULL)
		return NULL;

	return plist->get_attribute_by_name( attributename);
}

expat_attribute*	
expat_element::get_attribute_by_index( unsigned int idx)
{
	expat_attribute_list*	plist = get_attribute_list();
	if( plist == NULL)
		return NULL;

	return plist->get_attribute_by_index( idx);
}

XML_String const		
expat_element::get_attribute_value( XML_String const attributename)
{
	expat_attribute*	pattr = get_attribute_by_name(attributename);
	if( !pattr)
		return _XT("");

	return pattr->get_attribute_value();
}

expat_attribute*	
expat_element::add_attribute( expat_attribute* pattribute)
{
	if(m_attribute_list)
	{
		m_attribute_list->add_attribute( pattribute);
		return pattribute;
	}
	else 
		return NULL;
}

expat_attribute*	
expat_element::add_attribute( XML_String const name, XML_String const value )
{
	expat_attribute* pattribute = new expat_attribute(name, value, this);
	if(m_attribute_list)
	{
		m_attribute_list->add_attribute( pattribute);
		return pattribute;
	}
	else 
		return NULL;
}

void	
expat_element::indent_print( FILE* out, unsigned int indent)
{
	unsigned int i = 0;

	for (i = 0; i < indent * indent_size; i++)
	{
		expat_document::m_innerxml += _XT(" ");
	}
}

void	
expat_element::element_print( FILE* out, unsigned int indent)
{
	unsigned int closed = 0;

	indent_print(out, indent);
	expat_document::m_innerxml += _XT("<") ;
	expat_document::m_innerxml += m_name;
	
	/* print all the attribute*/
	expat_attribute_list* attribute_list = get_attribute_list();
	if( attribute_list)
		attribute_list->print(out);

	/* print the contents of current element*/
	XML_String const contents = get_element_contents();
	if ((contents == _XT("")) && ( m_child == NULL)
		&& ( m_parent != NULL))
	{
		expat_document::m_innerxml += _XT("/>\n");

		closed = 1;
	}
	else
	{
		expat_document::m_innerxml += _XT(">");
		if (contents == _XT(""))
		{
			expat_document::m_innerxml += _XT("\n");
		}
	}

	/* print the child element*/

	expat_element* child = get_first_child_element();
	if( child)
		child->element_print(  out, indent + 1);

	expat_element* sibling = child;
	if( sibling)
	{
		while (( sibling = sibling->get_sibling_element()) != NULL)
		{
			sibling->element_print(out, indent + 1);
		}
	}

	if (contents != _XT(""))
	{		
		expat_document::m_innerxml += common::utility::stringhelper::TrimCopy(contents);
	}
	else if (!closed)
	{
		indent_print(out, indent);
	}

	if (!closed)
	{
		expat_document::m_innerxml +=_XT("</");
		expat_document::m_innerxml += m_name;
		expat_document::m_innerxml += _XT(">");
	}
}

}}