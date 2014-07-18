#pragma once
#include <tchar.h>

int DecodeBase64Char2(UINT ch)
{
	// returns -1 if the character is invalid
	// or should be skipped
	// otherwise, returns the 6-bit code for the character
	// from the encoding table
	if (ch >= 'A' && ch <= 'Z')
	{
		return ch - 'A' + 0;	// 0 range starts at 'A'
	}

	if (ch >= 'a' && ch <= 'z')
	{
		return ch - 'a' + 26;	// 26 range starts at 'a'
	}

	if (ch >= '0' && ch <= '9')
	{
		return ch - '0' + 52;	// 52 range starts at '0'
	}

	if (ch == '+')
	{
		return 62;
	}

	if (ch == '/')
	{
		return 63;
	}

	return -1;
}


BOOL Base64Decode2(LPCTSTR lpszSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)
{
	// walk the source buffer
	// each four character sequence is converted to 3 bytes
	// CRLFs and =, and any characters not in the encoding table
	// are skiped
	
	if (!lpszSrc || !pbDest || !pnDestLen)
	{
		return FALSE;
	}
	
	LPCTSTR lpszSrcEnd = lpszSrc + nSrcLen;
	int nWritten = 0;

	while (lpszSrc < lpszSrcEnd)
	{
		DWORD dwCurr = 0;
		int i;
		int nBits = 0;

		for (i=0; i<4; i++)
		{
			if (lpszSrc >= lpszSrcEnd)
				break;

			int nCh = DecodeBase64Char2(*lpszSrc);

			lpszSrc++;
			if (nCh == -1)
			{
				// skip this char
				i--;
				continue;
			}

			dwCurr <<= 6;
			dwCurr |= nCh;
			nBits += 6;
		}

		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		dwCurr <<= 24-nBits;

		for (i=0; i<nBits/8; i++)
		{
			*pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
			dwCurr <<= 8;
			pbDest++;
			nWritten++;
		}
	}
	
	*pnDestLen = nWritten;
	return TRUE;
}

int Base64Encode(LPTSTR lpszBuffer, int nLen, LPTSTR lpszEncodeBuffer)
{
	int nLength = -1;
	
	if (lpszEncodeBuffer != NULL) 
	{
		TCHAR szBase64Table[] 
			= _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
		// 123456 78 1234 5678 12 345678  12345678
		
		UINT nLeaveValue = 0;	// 上一字符剩下的bit值
		int nLeaveBit = 0;		// 上一字符剩下的bit数
		
		for (int i = 0; i < nLen; i++)
		{
			_TUCHAR uchCur = NULL;
			uchCur = *(lpszBuffer + i);
			
			// nLeaveBit 必须小于6, nLeaveBit 的取值为0、2、4
			switch(nLeaveBit)
			{
			case 0:
				{
					nLeaveValue = uchCur >> 2;
					_tcsncat(lpszEncodeBuffer, (szBase64Table + nLeaveValue), 1);
					nLength++;

					nLeaveBit = 2;
					nLeaveValue = *(lpszBuffer + i) & 0x03;
				}
				break;
				
			case 2:
				{
					nLeaveValue <<= 4;
					nLeaveValue |= (uchCur >> 4);
					_tcsncat(lpszEncodeBuffer, (szBase64Table + nLeaveValue), 1);
					nLength++;

					nLeaveBit = 4;
					nLeaveValue = *(lpszBuffer + i) & 0x0f;
				}
				break;
				
			case 4:
				{
					nLeaveValue <<= 2;
					nLeaveValue |= (uchCur >> 6);
					_tcsncat(lpszEncodeBuffer, (szBase64Table + nLeaveValue), 1);
					nLength++;
					
					nLeaveValue = uchCur & 0x3f;
					_tcsncat(lpszEncodeBuffer, (szBase64Table + nLeaveValue), 1);
					nLength++;
					
					nLeaveBit = 0;
					nLeaveValue = 0;
				}
				break;
				
			default:
				break;
				
			} // End switch
			
		} // End For
		
		// nLeftBit必须小于6, nLeftBit的取值为0、2、4
		// 根据nLeftBit填充'='号
		switch(nLeaveBit)
		{
		case 0:
			break;
			
		case 2:
			{
				//2+8+8= 2+4 +4+2 +6
				nLeaveValue <<= 4; //填充4个0
				_tcsncat(lpszEncodeBuffer, (szBase64Table + nLeaveValue), 1);
				_tcscat(lpszEncodeBuffer, _T("==")); // 填充2个'='号
				nLength += 3;
			}
			break;
			
		case 4:
			{
				//4+8= 4+2 +6
				nLeaveValue <<= 2; // 填充2个0
				_tcsncat(lpszEncodeBuffer, (szBase64Table + nLeaveValue), 1);
				_tcscat(lpszEncodeBuffer, _T("=")); // 填充1个'='号
				nLength += 2;
			}
			break;
			
		default:
			break;
			
		} // End Switch
		
	}// End If
	
	return nLength;
}

