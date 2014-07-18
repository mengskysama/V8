#include "utility\expat++\expat_str.h"

#include <stdio.h>
#include "utility\expat++\expat_document.h"
#include "utility\expat++\expat_tree.h"
#include "utility\expat++\expat_element.h"
#include "utility\stringhelper.h"

namespace common { namespace utility {

void
xmldecl_handler(void* data, XML_Char const* version, XML_Char const* encoding,
				int standalone)
{
	expat_document* pdocument = (expat_document*) data;

	/* Avoid warning: standalone is unused */
	(void) standalone;

	if (pdocument == NULL)
	{
		return;
	}

	if (pdocument->m_tree == NULL)
	{
		pdocument->m_tree = new expat_tree();
	}

	if (version != NULL)
	{
		pdocument->m_tree->m_version = version;
	}

	if (encoding != NULL)
	{
		pdocument->m_tree->m_encoding = encoding;
	}

	pdocument->m_tree->m_standalone = standalone;
}

void
start_handler(void* data, XML_Char const* elem, XML_Char const** attr)
{
	//int i = 0;
	expat_document* pdocument = (expat_document*) data;

	if (pdocument == NULL)
	{
		return;
	}

	if ((pdocument->m_tree == NULL) || pdocument->m_tree->get_root() == NULL)
	{
		if (pdocument->m_tree == NULL)
		{
			pdocument->m_tree = new expat_tree();
		}

		pdocument->m_current = pdocument->m_tree->add_root(elem);
	}
	else
	{
		stack_push(&pdocument->m_stack, pdocument->m_current);
		pdocument->m_current = pdocument->m_current->append_child_element(elem);
	}

	for (int i = 0; attr[i]; i += 2)
	{
		pdocument->m_current->add_attribute( attr[i], attr[i + 1]);
	}
}

void
end_handler(void* data, XML_Char const* elem)
{
	expat_document* pdocument = (expat_document*) data;

	/* Avoid warning: elem is unused */
	(void) elem;

	if (pdocument == NULL)
		return;

	expat_element* current = pdocument->m_current;
	if (current != NULL)
	{
		if ( pdocument->m_ignore_whitespaces)
		{
			if (current->m_contents.length() == 0)
			{
			}
		}
		else
		{
			XML_String contents = current->m_contents;
			if ( contents.length() == 0)
			{
				current->m_contents = _XT("");
			}
		}
	}

	pdocument->m_current = stack_pop(&pdocument->m_stack);
}

void
char_handler(void* data, XML_Char const* s, int len)
{
	//size_t total = 0;
	//size_t total_old = 0;
	expat_document* pdocument = (expat_document*) data;

	if (pdocument == NULL)
		return;

	expat_element* current = pdocument->m_current;

	if (current == NULL)
		return;

	XML_String dst;
	dst.assign( s, len);

#ifdef XML_UNICODE_WCHAR_T
	common::utility::stringhelper::Replace( dst, _XT("\n"), _XT("\r\n") );
#else
	common::utility::stringhelper::Replace( dst, _XT("\n"), _XT("\r\n") );
#endif

	current->m_contents += dst;
}


}}