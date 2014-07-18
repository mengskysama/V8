#include "stdafx.h"
#include "flashWnd.h"
#include "UIActiveXBase.h"
#include "UIActiveX.h"
#include <algorithm>
#include <assert.h>

#define FLASH_DRAW_TIMER	30022
#define FLASH_VERSION	0x90000

std::vector<CFlashWnd*> CFlashWnd::ms_vecFlashWnd;

CFlashWnd::CFlashWnd()
:m_flash(this)
,m_version(0)
{
	ms_vecFlashWnd.push_back(this);
	m_bFinish = true;
	m_bPermanent = false;
}

CFlashWnd::~CFlashWnd()
{
	std::vector<CFlashWnd*>::iterator it = find(ms_vecFlashWnd.begin(), 
		ms_vecFlashWnd.end(), this);
	if(it != ms_vecFlashWnd.end())
	{
		ms_vecFlashWnd.erase(it);
	}
}

BEGIN_MESSAGE_MAP(CFlashWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

bool CFlashWnd::Play(int width, int height, int playTime, int playCircle, CString strFlashPath, CWnd * pParentWnd)
{
	CRect rc(0,0,width,height);

	//WS_EX_LAYERED 和 WS_CHILD冲突，导致在Win7和vista下调用SetParent后不能正常显示，去掉WS_CHILD
	CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE/*|WS_EX_TOPMOST*/, AfxRegisterWndClass(0), _T("UIActiveX"), 
		/*WS_CHILD |*/ /*WS_VISIBLE |*/ WS_CLIPCHILDREN | WS_POPUP , rc, pParentWnd, 0);

	bool bSucc = m_flash.CreateControl(this, ShockwaveFlashObjects::CLSID_ShockwaveFlash, rc);

	try
	{
		m_version = m_flash.GetVersion();
	}
	catch(...)
	{
		m_version = 0;
	}	

	if (!bSucc || m_version < FLASH_VERSION)
	{
		m_flash.ReleaseControl();
		m_bFinish = true;
		return false;
	}

	m_flash.Play(strFlashPath.GetBuffer());

	m_count = playTime/playCircle;

	m_bFinish = false;
	if (playTime == 0)
	{
		m_bPermanent = true;
	}

	CWnd::SetTimer(FLASH_DRAW_TIMER, playCircle, NULL);

	return true;
}

long CFlashWnd::GetVersion()
{
	return m_version;
}

void CFlashWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (!m_bPermanent)
	{
		if(m_count-- >= 0)
		{
			CRect rc;
			GetWindowRect(&rc);
			m_flash.DoPaint(GetDC()->GetSafeHdc(), rc);
		}
		else
		{
			m_flash.ReleaseControl();

			m_bFinish = true;

			KillTimer(nIDEvent);

			//DestroyWindow();
		}
	}
	else
	{
		CRect rc;
		GetWindowRect(&rc);
		m_flash.DoPaint(GetDC()->GetSafeHdc(), rc);
	}

	CWnd::OnTimer(nIDEvent);
}

void CFlashWnd::OnDestroy()
{
	CWnd::OnDestroy();

	//delete this;	
}

bool CFlashWnd::IsSupportFlash()
{
	LPCLASSFACTORY pClassFactory = NULL;
	HRESULT hr = ::CoGetClassObject(ShockwaveFlashObjects::CLSID_ShockwaveFlash, CLSCTX_ALL, NULL, IID_IClassFactory, (void**) &pClassFactory);
	if(SUCCEEDED(hr))
	{
		CString strResult;
		strResult.Format(L"CFlashWnd::IsSupportFlash() Sucess");
		OutputDebugString(strResult);
		if(pClassFactory)
		{
			pClassFactory->Release();
			return true;
		}
	}
	CString strResult;
	strResult.Format(L"CFlashWnd::IsSupportFlash() result = %d", hr);
	OutputDebugString(strResult);
	return false;
}

void CFlashWnd::StopAllFlash()
{
	int count = ms_vecFlashWnd.size();

	while(count > 0)
	{
		std::vector<CFlashWnd*>::iterator it = ms_vecFlashWnd.begin();
		if(it == ms_vecFlashWnd.end())
		{
			break;
		}

		CFlashWnd* pWnd = *it;
		pWnd->m_flash.ReleaseControl();
		pWnd->SetFinish(true);
		pWnd->DestroyWindow();
		delete pWnd;
		pWnd = NULL;

		if(count == ms_vecFlashWnd.size())
		{
			//assert(0);
			break;
		}

		count--;
	}
}

void CFlashWnd::CallFunction( CString func, std::vector<CString> vecParams )
{
	m_flash.CallFunction(func, vecParams);
}

void CFlashWnd::Finish()
{
	m_flash.ReleaseControl();

	m_bFinish = true;

	if (m_hWnd != NULL)
	{
		KillTimer(FLASH_DRAW_TIMER);
	}
}

void CFlashWnd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//__super::OnClose();
}

void CFlashWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = cx;
	rc.bottom = cy;

	m_flash.SetPos(rc);
}
