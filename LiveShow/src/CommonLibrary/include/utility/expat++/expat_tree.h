#pragma once

#include "../../../thirdlibrary/expat/expat.h"
#include "expat_element.h"

namespace common { namespace utility {

class expat_tree
{
public:
	expat_tree(void);
	~expat_tree(void);

	/* set the version of current xml tree*/
	void	set_version( XML_String const pvsersion);
	XML_String const	get_version();

	/* set the encoding of current xml tree*/
	void	set_encoding( XML_String const pencoding);
	XML_String const	get_encoding();

	/* set the preamble of current xml tree*/
	void	set_preamble( XML_String const	preamble);
	XML_String const	get_preamble();

	/* set the standalone of current xml tree*/
	void	set_standalone( unsigned int nstandalone);
	unsigned int get_standalone();

	expat_element*	get_root();
	expat_element*	add_root( XML_String const	name);

public:
	XML_String m_version;
	XML_String m_encoding;
	XML_String m_preamble;
	int m_standalone;
	expat_element* m_root;
};


}}