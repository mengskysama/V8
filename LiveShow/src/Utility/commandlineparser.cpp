#include "stdafx.h"
#include "commandlineparser.h"
#include "string_helper.h"
#include "base64hash.h"
#include "utility.h"
#include "blowfish.h"

ENM_COMMANDLINE_TYPE GetCommandLineType()
{
    LPWSTR lpCommandLine = GetCommandLine();

    wchar_t * pPos = wcschr(lpCommandLine, L'\"');

    if(NULL == pPos || *(pPos+1) == 0) return ENM_COMMANDLINE_TYPE_NULL;

    pPos = wcschr(pPos+1, L'\"');

    if(NULL == pPos || *(pPos+1) == 0 || *(pPos+2) == 0) return ENM_COMMANDLINE_TYPE_NULL;

    wchar_t szCode[MAX_PATH*2] = {0};

    swscanf(pPos+2, L"%s", szCode);

	szCode[wcslen(szCode)-1] = 0;

    if(wcscmp(szCode, COMMANDLINE_CODE_WEBLOGIN) == 0)
    {
        return ENM_COMMANDLINE_TYPE_WEBLOGIN;
    }
	else if(wcscmp(szCode, COMMANDLINE_CODE_WEBTRAYLOGIN) == 0)
	{
		return ENM_COMMANDLINE_TYPE_WEBTRAYLOGIN;
	}

    return ENM_COMMANDLINE_TYPE_NULL;
}

void BlowFishDecode(byte* key, int nBufLen, byte* pBuff)
{
    CBlowFish blow;
    blow.Initialize((byte*)key,16);
    int nlength = nBufLen;
    int nDifference = nlength % 8;
    int nNewLength = nlength;
    if( nDifference	!= 0 )
    nNewLength = (nlength - nDifference) + 8;
    blow.Decode(pBuff,pBuff,nNewLength);
}

BOOL GetWebLoginParam(LPWSTR lpszAccount, int nAccLen, LPWSTR lpszPswd, int nPswdLen, bool& bOnline)
{
    LPWSTR lpCommandLine = GetCommandLine();

    wchar_t* pPos = wcsstr(lpCommandLine, COMMANDLINE_CODE_WEBLOGIN);

    if(NULL != pPos)
	{
		pPos += wcslen(COMMANDLINE_CODE_WEBLOGIN);
	}
	else
	{
		pPos = wcsstr(lpCommandLine, COMMANDLINE_CODE_WEBTRAYLOGIN);

		if(pPos == NULL) return FALSE;

		pPos += wcslen(COMMANDLINE_CODE_WEBTRAYLOGIN);
	}    

    wchar_t szOnline[10] = {0};

    swscanf(pPos, L"%s", szOnline);

    if(wcscmp(szOnline, L"1") == 0)
    {
        bOnline = true;
    }
    else
    {
        bOnline = false;
    }

    pPos += wcslen(szOnline);

    pPos++;

    wchar_t szAccount[MAX_PATH] = {0};
    swscanf(pPos, L"%s", szAccount);

     int nLen = _tcslen(szAccount);

     if(0 == nLen ||  nAccLen <=  nLen) return FALSE;

     wcscpy(lpszAccount, szAccount);
     
     pPos += nLen;

    while(nLen < 16)
    {
        if(nLen > 8)
        {
            memcpy(&szAccount[nLen], szAccount, sizeof(wchar_t)*(16-nLen));
        }
        else
        {
            memcpy(&szAccount[nLen], szAccount, sizeof(wchar_t)*nLen);
        }

        nLen = _tcslen(szAccount);
    }

    wchar_t szPswd[MAX_PATH] = {0}; 

    pPos++;

    swscanf(pPos, L"%s", szPswd);

    if(0 == _tcslen(szPswd)) return FALSE;

    BYTE byTemp[MAX_PATH] = {0};
    int nDstLen = MAX_PATH;

    if(!Base64Decode2(szPswd, _tcslen(szPswd), byTemp, &nDstLen))
    {
        return FALSE;
    }

    string	strAccount2 = string_helper::from( wstring(szAccount));

    BlowFishDecode((byte*)strAccount2.c_str(), nDstLen, byTemp);

    wstring strPswdMd5 = string_helper::from( string((char*)byTemp));   

    if(strPswdMd5.length() < nPswdLen)
    {
        wcscpy(lpszPswd, strPswdMd5.c_str());

        return TRUE;
    }
    else
    {
        return FALSE;
    }  
}
