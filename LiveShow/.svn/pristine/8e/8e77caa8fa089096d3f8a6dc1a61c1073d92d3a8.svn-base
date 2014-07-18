#ifndef _CRC32_H_
#define _CRC32_H_

#include "standard.h"


// Class to perform 32 bit CRC checksumming.
class ClsCrc32
{
public:
	// Constructor/Destructor.
	ClsCrc32();
	ClsCrc32( DWORD dwPolynimial );
	virtual ~ClsCrc32();

	// Implementation
	void CrcInitialize( DWORD dwPolynomial = 0xEDB88320 );
	DWORD CrcAdd( LPBYTE pData, DWORD dwLength );
	DWORD Crc( LPBYTE pData, DWORD dwLength );
	inline void CrcReset() { m_lCRC = 0xFFFFFFFF; };
	inline DWORD CrcGet() const { return ( m_lCRC ^ 0xFFFFFFFF ); }
	inline DWORD CrcTableIndex( int nIndex ) const { _ASSERT( nIndex >= 0 && nIndex <= 255 ); _ASSERT_VALID( m_pCrcTable ); return m_pCrcTable[ nIndex ]; };

	// Operator overloads
	inline BOOL operator==( DWORD dwCrc ) const { return ( BOOL )(( m_lCRC ^ 0xFFFFFFFF ) == dwCrc ); }

protected:
	// Data
	static DWORD	s_dwCrc32[ 256 ];
	DWORD	       *m_pCrcAllocated;
	DWORD	       *m_pCrcTable;
	DWORD		m_lCRC;
};
#endif