#include "stdafx.h"
#include <assert.h>
#include "TEAEncryptor.h"


#define DELTA 0x9e3779b9 /* sqr(5)-1 * 2^31 */

/// TEA加/解密单元
/*
@param	rounds IN	轮数，大于0时，用于加密，小于0时，用于解密	
@param	in IN		要加/解密的64bit数据
@param	out OUT		存放加/解密结果的64bit
@param	key	IN		加/解密的key,128bit
@remark	该函数是TEA算法的核心，一次只能加/解密64bit的数据，结果也是64bit的数据
*/
static void TEACore(UINT32 in[2], UINT32 out[2], UINT32 key[4], long rounds)
{
	UINT32 y = in[0], z = in[1];
	UINT32 limit = 0, sum = 0;

	if(rounds > 0)	// encrypt
	{
		limit = DELTA * rounds;
		while(sum != limit)
		{
			y += ((z<<4)^(z>>5)) + (z^sum) + key[sum&3];
			sum += DELTA;
			z += ((y<<4)^(y>>5)) + (y^sum) + key[(sum>>11)&3];
		}
	}
	else	// decrypt
	{
		sum = DELTA * (-rounds);
		while(sum)
		{
			z -= ((y<<4)^(y>>5)) + (y^sum) + key[(sum>>11)&3];
			sum -= DELTA;
			y -= ((z<<4)^(z>>5)) + (z^sum) + key[sum&3];
		}
	}

	out[0] = y; out[1] = z;
}

BYTE TEAEncryptor::DEF_KEY[16] = {'A','3','4','F','9','7','7','1','E','C','1','0','B','C','A','5'};

BOOL TEAEncryptor::TEAEncrypt(BYTE* pInBuffer, int nInBufferLen, BYTE* pOutBuffer, int* pnOutBufferLen, BYTE pKey[16]/* = DEF_KEY*/, UINT uRounds/* = 16*/)
{
	if (nInBufferLen < 1 || !pnOutBufferLen)
	{
		assert(FALSE);
		return FALSE;
	}

	/// padding
	const int nPadLen = ((nInBufferLen % 8) == 0 ? 0 : (8 - (nInBufferLen % 8)));

	int nBufferLenToEncrypt = nInBufferLen + nPadLen;
	assert(nBufferLenToEncrypt % 8 == 0);

	if (*pnOutBufferLen < (nBufferLenToEncrypt + 1))	// check: *pnOutBufferLen is length enough
	{
		assert(FALSE);
		return FALSE;
	}

	BYTE* pBufferToEncrypt = new BYTE[nBufferLenToEncrypt];
	memcpy(pBufferToEncrypt, pInBuffer, nInBufferLen);
	for (int i = 0; i < nPadLen; i++)	// pad with 0
	{
		pBufferToEncrypt[nInBufferLen + i] = 0;
	}

	/// core
	for (int i = 0; i < nBufferLenToEncrypt; i += 8)
	{
		TEACore((UINT32*)(pBufferToEncrypt + i), (UINT32*)(pOutBuffer + i + 1), (UINT32*)pKey, uRounds);
	}

	pOutBuffer[0] = nPadLen;
	if (pnOutBufferLen)
	{
		*pnOutBufferLen = nBufferLenToEncrypt + 1;
	}

	delete[] pBufferToEncrypt;
	pBufferToEncrypt = NULL;

	return TRUE;
}

BOOL TEAEncryptor::TEADecrypt(BYTE* pInBuffer, int nInBufferLen, BYTE* pOutBuffer, int* pnOutBufferLen, BYTE pKey[16]/* = DEF_KEY*/, UINT uRounds/* = 16*/)
{
	if (nInBufferLen < 9 || ((nInBufferLen - 1) % 8) != 0 || !pnOutBufferLen || *pnOutBufferLen < (nInBufferLen - 1))
	{
		assert(FALSE);
		return FALSE;
	}

	for (int i = 1; i < nInBufferLen; i += 8)
	{
		TEACore((UINT32*)(pInBuffer + i), (UINT32*)(pOutBuffer + i - 1), (UINT32*)pKey, -1 * uRounds);
	}

	const int nPadLen = pInBuffer[0];
	if (nPadLen > 0)
	{
		for (int i = 0; i < nPadLen; i++)
		{
			if (pOutBuffer[nInBufferLen - 1 - nPadLen + i] != 0)	// check: padding BYTEs MUST equal 0
			{
				assert(FALSE);
				return FALSE;
			}
		}
	}

	if (*pnOutBufferLen)
	{
		*pnOutBufferLen = nInBufferLen - 1 - nPadLen;
	}

	return TRUE;
}
