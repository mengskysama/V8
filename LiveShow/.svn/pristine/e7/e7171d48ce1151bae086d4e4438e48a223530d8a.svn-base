#pragma once
#include "../Common.h"

namespace common 
{
	namespace utility
	{
		

		class COMMON_INTERFACE CAsynOpenURL
		{
		public:

			enum ENM_URLOpenResult
			{
				enmOpenURLResult_failed,
				enmOpenURLResult_success,
			};			

			//hRcvWnd会收到unMessage的消息，lParam为unParam, wParam为ENM_URLOpenResult
			CAsynOpenURL(HWND hRcvWnd, UINT unMessage, UINT unParam, BYTE* pBuffer, DWORD dwBufSize);
			~CAsynOpenURL(void);

			
			void OpenURL(CString strURL);

		private:

			void PostMessage(ENM_URLOpenResult enmURLOpenResult);

			static UINT _stdcall OpenURL(LPVOID lParam);

			HANDLE m_hThread;
			HWND m_hRcvWnd;
			UINT m_unMessage;
			UINT m_unParam;
			CString m_strURL;

			BYTE* m_pBuffer;
			DWORD m_dwBufSize;	
		};
	}
}
