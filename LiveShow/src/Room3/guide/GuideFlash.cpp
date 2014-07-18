#include "StdAfx.h"
#include "GuideFlash.h"

CGuideFlash::CGuideFlash(CString strParam)
: m_strParam(strParam)
{
}

CGuideFlash::~CGuideFlash(void)
{
}

void CGuideFlash::FlashCall( const CString& request )
{
	if(request.Find(L"<string>0</string>") >= 0)
	{
		std::vector<CString> vFuncParams;
		vFuncParams.push_back(m_strParam);
		CallFunction(L"SendInitData", vFuncParams);
	}

	if (request.Find(L"<string>1</string>") >= 0)
	{
		Finish();
	}
}

BOOL CGuideFlash::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_SPACE:
			{
				Finish();
				return TRUE;
			}
			break;
		default:
			break;
		}	
	}

	return CFlashWnd::PreTranslateMessage(pMsg);
}
