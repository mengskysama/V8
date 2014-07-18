
#include "..\include\Types.h"
#ifdef WIN32
#include <winsock2.h>
#endif

/* assembly-optimize these, for example use multiple registers and 
 * convert blocks to avoid stalling the cpu pipeline
 */

void htons_buf( UINT16* pnbuf, const void* phbuf, UINT cnt )
{
    register UINT16 tmp;
    while( cnt )
    {
        memcpy( &tmp, phbuf, 2 );
        *pnbuf = htons( tmp );
        phbuf = (char*)phbuf + 2; pnbuf++; cnt--;
    }
}

void ntohs_buf( UINT16* phbuf, const void* pnbuf, UINT cnt )
{
    register UINT16 tmp;
    while( cnt )
    {
        memcpy( &tmp, pnbuf, 2 );
        *phbuf = ntohs( tmp );
        pnbuf = (char*)pnbuf + 2; phbuf++; cnt--;
    }
}

void htonl_buf( UINT32* pnbuf, const void* phbuf, UINT cnt )
{
    register UINT32 tmp;
    while( cnt )
    {
        memcpy( &tmp, phbuf, 4 );
        *pnbuf = htonl( tmp );
        phbuf = (char*)phbuf + 4; pnbuf++; cnt--;
    }
}

void ntohl_buf( UINT32* phbuf, const void* pnbuf, UINT cnt )
{
    register UINT32 tmp;
    while( cnt )
    {
        memcpy( &tmp, pnbuf, 4 );
        *phbuf = ntohl( tmp );
        pnbuf = (char*)pnbuf + 4; phbuf++; cnt--;
    }
}
