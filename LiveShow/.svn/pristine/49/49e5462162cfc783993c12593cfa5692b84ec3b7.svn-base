#include "StdAfx.h"
#include "utility\expat++\expat_error.h"
#include "utility\expat++\expat_document.h"
#include "assert.h"
#include "utility\expat++\expat_str.h"

namespace common { namespace utility {

expat_error::expat_error(expat_document*	pdocument)
{
	m_document = pdocument;
	assert( m_document != NULL);
}

#ifndef _MT
/* single-threaded version */
expat_error::~expat_error(void)
{
}


static expat_error_e last_error = expat_error_none;

void
expat_error::set_last_error( expat_error_e code)
{
	last_error = code;
}

expat_error_e
expat_error::get_last_error()
{
	return last_error;
}

#else /* _MT */

#ifdef _MSC_VER

expat_error::~expat_error(void)
{

}

void	expat_error::set_last_error(expat_error_e code)
{
	if (last_error_key == TLS_OUT_OF_INDEXES)
	{
		last_error_key = TlsAlloc();
	}

	TlsSetValue(last_error_key, (LPVOID)code);
}

expat_error_e	expat_error::get_last_error()
{
	if (last_error_key == TLS_OUT_OF_INDEXES)
	{
		return expat_error_none;
	}

	return (expat_error_e)(DWORD)TlsGetValue(last_error_key);
}

#else
/* pthread multi-threaded version */

#include <pthread.h>

static pthread_key_t key_error;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

void
create_keys()
{
	expat_error_e* code = NULL;

	pthread_key_create(&key_error, free);

	code = new expat_error_e;
	*code = expat_error_none;
	pthread_setspecific(key_error, code);
}

void
expat_error::set_last_error(expat_error_e code)
{
	expat_error_e* old_code = NULL;
	expat_error_e* new_code = NULL;

	/* Initialize error code per thread */
	pthread_once(&key_once, create_keys);

	old_code = (expat_error_e*) pthread_getspecific(key_error);
	new_code = new expat_error_e;
	*new_code = code;
	free(old_code);
	pthread_setspecific(key_error, new_code);
}

expat_error_e
expat_error::get_last_error()
{
	expat_error_e* code = NULL;

	/* Initialize error code per thread */
	pthread_once(&key_once, create_keys);

	code = (expat_error_e*) pthread_getspecific(key_error);
	if (code == NULL)
	{
		return expat_error_none;
	}
	return *code;
}


#endif /* _MSC_VER */

#endif /* _MT */


/* get the last error we set */
expat_error_e	
expat_error::get_error()
{
	return get_last_error();
}

/* get the last error msg we set*/
XML_String const	
expat_error::get_error_string( expat_error_e code)
{
	static const XML_Char *message[] = {
		_XT("No error"),
		_XT("Out of memory"),
		_XT("Input/Output error"),
		_XT("Callback error"),
		_XT("Internal Expat parser error")
	};

	assert(sizeof(message) / sizeof(message[0]) == expat_error_count);

	if ((code < 0) || (code > expat_error_count))
	{
		// This is not thread safe. Even though, no one should get in
		// here.
		static XML_Char unk_message[35];
		expat_sprintf(unk_message, 35, _XT("Unknown error code (%d)"), code);
		return unk_message;
	}
	else
	{
		return message[code];
	}
}

void	
expat_error::set_error( expat_error_e code)
{
	set_last_error( code);
}

/* get the last error expat set internal*/
enum XML_Error	
expat_error::get_expat_error()
{
	return XML_GetErrorCode( m_document->m_parser);
}

/* get the last error msg expat set internal*/
XML_String const	
expat_error::get_expat_error_string(enum XML_Error code)
{
	return XML_ErrorString(code);
}

/* get the error line in expat internal*/
unsigned int 
expat_error::get_expat_error_line()
{
	return XML_GetCurrentLineNumber( m_document->m_parser);
}

/* get the error column in expat internal*/
unsigned int 
expat_error::get_expat_error_column()
{
	return XML_GetCurrentColumnNumber( m_document->m_parser);
}
}}