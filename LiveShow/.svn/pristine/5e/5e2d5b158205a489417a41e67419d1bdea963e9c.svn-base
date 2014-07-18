// GGListDropSource.cpp : 实现文件
//

#include "../../StdAfx.h"
#include "../../include/ui/DropSource.h"

namespace common { namespace ui
{

// CDropSource

IMPLEMENT_DYNAMIC(CDropSource, COleDropSource)


CDropSource::CDropSource()
{
	m_pwnd = NULL;
}

CDropSource::~CDropSource()
{
}


BEGIN_MESSAGE_MAP(CDropSource, COleDropSource)
END_MESSAGE_MAP()



// CDropSource 消息处理程序

BOOL CDropSource::OnBeginDrag(CWnd* pWnd,CRect rcItem)
{
	ASSERT_VALID(this);

	m_bDragStarted = FALSE;
	m_bLBtnUp = FALSE;

	// opposite button cancels drag operation
	m_dwButtonCancel = 0;
	m_dwButtonDrop = 0;
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		m_dwButtonDrop |= MK_LBUTTON;
		m_dwButtonCancel |= MK_RBUTTON;
	}
	else if (GetKeyState(VK_RBUTTON) < 0)
	{
		m_dwButtonDrop |= MK_RBUTTON;
		m_dwButtonCancel |= MK_LBUTTON;
	}

	DWORD dwLastTick = GetTickCount();
	pWnd->SetCapture();

	while (!m_bDragStarted)
	{
		// some applications steal capture away at random times
		if (CWnd::GetCapture() != pWnd)
			break;

		// peek for next input message
		MSG msg;
		if (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE) ||
			PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		{
			// check for button cancellation (any button down will cancel)
			if (msg.message == WM_LBUTTONUP || msg.message == WM_RBUTTONUP ||
				msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN)
			{
				if(msg.message == WM_LBUTTONUP)
					m_bLBtnUp = TRUE;
				break;
			}
			// check for keyboard cancellation
			if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
				break;
			// check for drag start transition
			m_bDragStarted = !rcItem.PtInRect(msg.pt);
		}

		// if the user sits here long enough, we eventually start the drag
		if (GetTickCount() - dwLastTick > 500)
			m_bDragStarted = TRUE;
	}
	ReleaseCapture();

	return m_bDragStarted;
}

SCODE CDropSource::GiveFeedback(DROPEFFECT /*dropEffect*/)
{
	static int i= 10000;
	TRACE("GiveFeedback %d\n", i++);
	ASSERT_VALID(this);

	CPoint pt;
	GetCursorPos(&pt);
	HWND h = ::WindowFromPoint(pt);
	DWORD pid = -1;
	GetWindowThreadProcessId(h, &pid);
	bool bInMyProcess = (pid == GetCurrentProcessId());
	//return m_bDragStarted ? DRAGDROP_S_USEDEFAULTCURSORS : S_OK;
	if (bInMyProcess)
	{
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}
	else
	{
		HCURSOR h = LoadCursor(NULL, IDC_NO);
		SetCursor(h);
		return S_OK;
	}
}

SCODE CDropSource::QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState)
{
	static int i= 40000;
	TRACE("QueryContinueDrag %d dwKeyState=%d bEscapePressed=%d\n", i++, dwKeyState, bEscapePressed);

	SCODE scRet = __super::QueryContinueDrag(bEscapePressed, dwKeyState);
	
	if (bEscapePressed)
	{
		m_pwnd->SendMessage(WM_DropSource_Canceled);
	}
	else if (dwKeyState == 0)
	{
		m_pwnd->SendMessage(WM_DropSource_Droped);
	}

	
	return scRet;
}

void CDropSource::Register(CWnd* pWnd)
{
	m_pwnd = pWnd;
}

} } //namespace common::ui
