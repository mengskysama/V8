#pragma once

#include "../../Common.h"
#include "../../../thirdlibrary/expat/expat.h"
#include "expat_str.h"

namespace common { namespace utility {

#ifndef _MT
#else /* _MT */

#ifdef _MSC_VER

#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
static DWORD last_error_key = TLS_OUT_OF_INDEXES;

#else
/* pthread multi-threaded version */

#include <pthread.h>
static pthread_key_t key_error;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
#endif /* _MSC_VER */

#endif /* _MT */


typedef enum
{
	/** No error has occured. */
	expat_error_none,
	/** No more memory available. */
	expat_error_no_memory,
	/** General Input/Output error. */
	expat_error_io,
	/** Callback error. */
	expat_error_callback,
	/** Expat parser error. */
	expat_error_parse,
	/** end of list marker */
	expat_error_count
} expat_error_e;


class expat_document;

class COMMON_INTERFACE expat_error
{
public:
	expat_error( expat_document* pdocument);
	~expat_error(void);

	/* get the last error we set */
	expat_error_e	get_error();

	/* get the last error msg we set*/
	XML_String const	get_error_string( expat_error_e code);

	/* set the last error when meet a application error*/
	void	set_error( expat_error_e code);

	/* get the last error expat set internal*/
	enum XML_Error	get_expat_error();

	/* get the last error msg expat set internal*/
	XML_String const	get_expat_error_string(enum XML_Error code);

	/* get the error line in expat internal*/
	unsigned int get_expat_error_line();

	/* get the error column in expat internal*/
	unsigned int get_expat_error_column();


private:

	void	set_last_error(expat_error_e code);
	expat_error_e	get_last_error();

	expat_document*	m_document;
};

}}