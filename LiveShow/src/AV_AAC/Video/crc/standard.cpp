#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif

#include "standard.h"

#ifdef _DEBUG
// Replacements for the new and delete operators. These will report the
// file name and line number where blocks of unfreed memory where
// allocated.
#undef new
void *operator new( size_t nSize, const char *lpszFileName, int nLine )
{
	// Allocate the data.
	void *pData = _malloc_dbg( nSize, _CLIENT_BLOCK, lpszFileName, nLine );

	// Failed?
	if ( pData == NULL )
	{
		// Call the new handler which, in the case
		// of this library, will throw an exception.
		_PNH nh = _set_new_handler( 0 );
		( *nh )( nSize );
		_set_new_handler( nh );
	}
	// Return pointer or NULL.
	return pData;
}

void operator delete( void *pData, const char *lpszFileName, int nLine )
{
	// Free the data.
	_free_dbg( pData, _CLIENT_BLOCK );
}

void ClsTrace( LPCTSTR pszFormatStr, ... )
{
	TCHAR  szBuffer[ 512 ]; // 512 chars is max.

	// Format the output.
	va_list  args;
	va_start( args, pszFormatStr );
	::wvsprintf( szBuffer, pszFormatStr, args );
	va_end( args );

	// Allocate a buffer to fit.
	::OutputDebugString( szBuffer );
}
#endif