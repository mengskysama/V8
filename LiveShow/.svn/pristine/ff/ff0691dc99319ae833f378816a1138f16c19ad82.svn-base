#include "StdAfx.h"
#include "utility/AsynOpenURL.h"
#include "process.h"

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

namespace common 
{
	namespace utility
	{


		CAsynOpenURL::CAsynOpenURL(HWND hRcvWnd, UINT unMessage, UINT unParam, BYTE* pBuffer, DWORD dwBufSize)
			: m_hThread(NULL)
			, m_hRcvWnd(hRcvWnd)
			, m_unMessage(unMessage)
			, m_pBuffer(pBuffer)
			, m_dwBufSize(dwBufSize)
			, m_unParam(unParam)
		{

		}

		CAsynOpenURL::~CAsynOpenURL(void)
		{
			if(WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT)
			{
				TerminateThread(m_hThread, 0);
			}	

			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		void CAsynOpenURL::OpenURL(CString strURL)
		{
			m_strURL = strURL;

			UINT dwThreadID = 0;
			m_hThread  = (HANDLE)_beginthreadex(NULL, 0, OpenURL, LPVOID(this), 0, &dwThreadID);
		}

		void CAsynOpenURL::PostMessage(ENM_URLOpenResult enmURLOpenResult)
		{
			if(m_hRcvWnd != NULL)
				::SendMessage(m_hRcvWnd, m_unMessage, (int)enmURLOpenResult, m_unParam);
		}

		UINT CAsynOpenURL::OpenURL(LPVOID lParam)
		{
			CAsynOpenURL* pThis = (CAsynOpenURL*)lParam;

			HINTERNET handle = InternetOpen(L"179", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);

			if(handle == NULL)
			{
				return 1;
			}

			HINTERNET internetopenurl = 
				InternetOpenUrl(handle, pThis->m_strURL.GetBuffer(), NULL, 0, 
				INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE, 0);

			if(internetopenurl != NULL)
			{
				DWORD dwReadBytes = 0;

				DWORD dwTotal = 0;

				while(InternetReadFile(internetopenurl, &pThis->m_pBuffer[dwTotal], pThis->m_dwBufSize-dwTotal, &dwReadBytes)
					&& dwReadBytes > 0)
				{
					dwTotal += dwReadBytes;

					dwReadBytes = 0;

					if(pThis->m_dwBufSize-dwTotal <= 0)
					{
						break;
					}
				}

				pThis->m_pBuffer[dwTotal] = 0;

				InternetCloseHandle(internetopenurl);

				if(dwTotal > 0)
				{
					pThis->PostMessage(enmOpenURLResult_success);
				}
				else
				{
					pThis->PostMessage(enmOpenURLResult_failed);
				}
			}
			else
			{
				pThis->PostMessage(enmOpenURLResult_failed);
			}

			InternetCloseHandle(handle);

			return 2;
		}

	}
}