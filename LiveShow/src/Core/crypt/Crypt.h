/********************************************************************

purpose:	
*********************************************************************/

#pragma once

namespace Auth51
{
class CXTEA
{
public:
	static int Encrypt(char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16]);
	static int Decrypt(char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16]);
};
}
