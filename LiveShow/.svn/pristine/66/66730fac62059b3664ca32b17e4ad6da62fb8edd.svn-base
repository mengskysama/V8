#include "StdAfx.h"
#include "utility\expat++\expat_tree.h"
#include "utility\expat++\expat_str.h"
#include "utility\expat++\expat_element.h"

namespace common { namespace utility {

expat_tree::expat_tree(void)
{
	m_encoding = _XT("utf-8");
	m_version = _XT("2.0");
	m_root = NULL;
}

expat_tree::~expat_tree(void)
{
	if( m_root)
		m_root->delete_element();
}

void	
expat_tree::set_version( XML_String const pvsersion)
{
	if( pvsersion == _XT(""))
		return;

	m_version = pvsersion;
}

XML_String const	
expat_tree::get_version()
{
	return m_version;
}

/* set the encoding of current xml tree*/
void	
expat_tree::set_encoding( XML_String const pencoding)
{
	if( pencoding == _XT(""))
		return;

	m_encoding =  pencoding;
}

XML_String const	
expat_tree::get_encoding()
{
	return m_encoding;
}

/* set the preamble of current xml tree*/
void	expat_tree::set_preamble( XML_String const	preamble)
{
	if( preamble == _XT(""))
		return;

	m_preamble =  preamble;
}

XML_String const	expat_tree::get_preamble()
{
	return m_preamble;
}

/* set the standalone of current xml tree*/
void	
expat_tree::set_standalone( unsigned int nstandalone)
{
	m_standalone = nstandalone;
}

unsigned int 
expat_tree::get_standalone()
{
	return m_standalone;
}

expat_element*	expat_tree::get_root()
{
	return m_root;
}

expat_element*	expat_tree::add_root( XML_String const	name)
{
	if( name == _XT(""))
		return NULL;

	expat_element* root = new expat_element();
	m_root = root;

	root->set_element_name( name);

	return m_root;
}
}}