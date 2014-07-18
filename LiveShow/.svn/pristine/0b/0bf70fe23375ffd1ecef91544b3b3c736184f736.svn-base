#include "utility\expat++\expat_document.h"
#include "utility\expat++\expat_handler.h"
#include "assert.h"
#include "utility\expat++\expat_error.h"
#include "utility\expat++\expat_str.h"
#include "utility\stringhelper.h"

namespace common { namespace utility {

#define SAFEDELETE(p) \
	if(p) \
	{  \
		delete p;  \
		p = NULL;  \
	}


/* element stack*/

/*
push a new element before element as the first element
of the stack
*/
stack_element*
stack_push(stack_element** stack, expat_element* element)
{
	stack_element* new_elem = new stack_element();

	if (new_elem != NULL)
	{
		new_elem->element = element;
		if (stack != NULL)
		{
			new_elem->prev = *stack;
		}
		*stack = new_elem;
	}

	return new_elem;
}

expat_element*
stack_pop(stack_element** stack)
{
	expat_element* element = NULL;
	stack_element* sk_elem = NULL;

	if (stack != NULL)
	{
		sk_elem = *stack;
		if (sk_elem != NULL)
		{
			*stack = sk_elem->prev;
			element = sk_elem->element;
		}
	}

	/* delete the element from stack*/
	delete sk_elem;
	sk_elem = NULL;

	return element;
}


#define MAX_BUFFER_SIZE 102400

XML_String expat_document::m_innerxml = _XT("");

expat_document::expat_document()
{
	m_parser = XML_ParserCreate(NULL);
	assert(m_parser != NULL);

	/* initialize Expat handlers */
	XML_SetXmlDeclHandler(m_parser, xmldecl_handler);
	XML_SetElementHandler(m_parser, start_handler, end_handler);
	XML_SetCharacterDataHandler(m_parser, char_handler);
	XML_SetUserData(m_parser, this);

	/* ignore white spaces by default */
	m_ignore_whitespaces = 1;

	/* no callback by default */
	m_stream_callback = NULL;
	m_error = new expat_error( this);
	m_tree = NULL;
	m_stack = NULL;
	m_current = NULL;
}

expat_document::~expat_document()
{
	if( m_parser)
	{
		XML_ParserFree( m_parser);
		m_parser = NULL;
	}

	SAFEDELETE( m_error);
	SAFEDELETE( m_tree);

	/* iter all the stack and delete them*/
	stack_element* sk_elem = NULL;
	while (m_stack != NULL)
	{
		sk_elem = m_stack;
		if (sk_elem != NULL)
		{
			m_stack = sk_elem->prev;

			SAFEDELETE( sk_elem);
		}
	}

	SAFEDELETE( m_stack);
}

expat_error*	
expat_document::get_error()
{
	return m_error;
}

int	expat_document::load_from_fp( FILE*	fp, unsigned int file_format)
{
	size_t len = 0;
	size_t done = 0;
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0x0, MAX_BUFFER_SIZE);

	assert(m_parser != NULL);

	while (!done)
	{
		len = fread(buffer, 1, MAX_BUFFER_SIZE, fp);
		if (ferror(fp))
		{
			m_error->set_error(expat_error_io);
			return 0;
		}

		done = feof(fp);
		
		/* turn buffer to utf-8*/
		if( file_format != UTF8)
		{
			string	strUtf8 = common::utility::stringhelper::UnicodeToAscii( common::utility::stringhelper::AsciiToUnicode(buffer));
			if (!XML_Parse( m_parser, strUtf8.c_str(), (int)strUtf8.size(), done))
			{
				m_error->set_error(expat_error_parse);
				return 0;
			}
		}
		else
		{
			if (!XML_Parse( m_parser, buffer, (int)len, done))
			{
				m_error->set_error(expat_error_parse);
				return 0;
			}
		}
	}

	return 1;
}

int 
expat_document::load_from_file( const wchar_t* w_file_name)
{
	unsigned int n  = expat_get_file_format( w_file_name);

	FILE* in = NULL;
	unsigned int res = 0;

	assert( w_file_name != NULL);

	in = _wfopen( w_file_name, L"rb");
	if (in == NULL)
	{
		m_error->set_error( expat_error_io);
		return 0;
	}

	res = load_from_fp(in, n);
	fclose(in);

	return res;
}

/* load xml from buffer*/
int 
expat_document::load_from_buffer( const char* buffer, unsigned int	buffer_size)
{
	if(buffer == NULL)
		return 0;

	if (!XML_Parse( m_parser, buffer, buffer_size, 1))
	{
		m_error->set_error(expat_error_parse);
		return 0;
	}

	return 1;
}

expat_element*	
expat_document::get_root()
{
	return m_tree->get_root();
}

expat_element*	
expat_document::add_root( XML_Char const* root_name)
{
	if( m_tree == NULL)
		m_tree = new expat_tree();

	expat_element* pelement = m_tree->get_root();
	if( pelement == NULL)
	{
		return m_tree->add_root( root_name);
	}

	return NULL;
}

void	
expat_document::print( FILE* out)
{
	static XML_Char const* version = _XT("1.0");
#ifdef XML_UNICODE_WCHAR_T
	static XML_Char const* encoding = _XT("UTF-16");
#else
	static XML_Char const* encoding = _XT("UTF-8");
#endif /* XML_UNICODE_WCHAR_T */

	m_innerxml = _XT("<?xml version=\"");
	m_innerxml += ( m_tree->m_version == _XT("") ? version : m_tree->m_version ) ;
	m_innerxml += _XT("\" ");
	m_innerxml += _XT("encoding=\"");
	m_innerxml += ( m_tree->m_encoding == _XT("") ? encoding : m_tree->m_encoding ); 
	m_innerxml += _XT("\" ");
	m_innerxml += _XT("standalone=\"");
	m_innerxml += m_tree->m_standalone > 0 ? _XT("yes") : _XT("no");
	m_innerxml += _XT("\" ?>");

	
	get_root()->element_print(out, 0);
}

bool 
expat_document::save_to_file( const char* file_name)
{
	FILE* out = NULL;

	assert( m_tree != NULL);
	assert( file_name != NULL);

	out = fopen(file_name, "w");
	if (out == NULL)
	{
		m_error->set_error( expat_error_io);
		return false;
	}

	print(out);

	/* turn the m_innerxml into utf-8*/
	wstring	strInnerXml = common::utility::stringhelper::AsciiToUnicode(common::utility::stringhelper::UnicodeToAscii( m_innerxml));
	expat_fprintf(out,strInnerXml.c_str());

	fclose(out);

	return true;
}

bool 
expat_document::save_to_file( const wchar_t* w_file_name)
{
	FILE* out = NULL;

	assert( m_tree != NULL);
	assert( w_file_name != NULL);

	out = _wfopen( w_file_name, L"w");
	if (out == NULL)
	{
		m_error->set_error( expat_error_io);
		return false;
	}

	print(out);
	/* turn the m_innerxml into utf-8*/
	wstring	strInnerXml = common::utility::stringhelper::AsciiToUnicode(common::utility::stringhelper::UnicodeToAscii( m_innerxml));
	expat_fprintf(out,strInnerXml.c_str());

	fclose(out);

	return true;
}

}}