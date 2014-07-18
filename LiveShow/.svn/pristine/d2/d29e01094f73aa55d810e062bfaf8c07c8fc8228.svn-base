#include "StdAfx.h"
#include "SkinEngine.h"
#include "SkinItem.h"
#include "KeyboardHooker.h"

CKeyboardHooker::CKeyboardHooker(void)
:m_hHook(NULL)
{
}

CKeyboardHooker::~CKeyboardHooker(void)
{
	Unhook();
}

BOOL CKeyboardHooker::Hook(HWND hWnd)
{
	if (m_hHook != NULL)
		return FALSE;

	if (! ::IsWindow(hWnd))
		return FALSE;

	m_hHook = ::SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardHookProc, NULL, GetWindowThreadProcessId(hWnd, NULL));
	ASSERT(m_hHook != NULL);
	return (m_hHook != NULL);
}

BOOL CKeyboardHooker::Unhook()
{
	if (m_hHook != NULL) {
		BOOL bRet = ::UnhookWindowsHookEx(m_hHook);
		if (bRet) {
			m_hHook = NULL;
		} else {
			ASSERT(0 && _T("hook释放失败！"));
		}
		return bRet;
	}
	return FALSE;
}

LRESULT CKeyboardHooker::KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0) {

        //调用下一钩子
        return CallNextHookEx(NULL, code, wParam, lParam);

    } else if (code == HC_ACTION && (lParam & 0x80000000) == 0) {	//KEYDOWN

		//获取获得消息的窗口
		HWND hwndForeground = ::GetForegroundWindow();
		DWORD dwCurrentThreadId = ::GetCurrentThreadId();
		DWORD dwWindowThreadId = ::GetWindowThreadProcessId(hwndForeground, NULL); 
		::AttachThreadInput(dwCurrentThreadId, dwWindowThreadId, TRUE);
		HWND hwndFocus = ::GetFocus();
		HWND hWnd = hwndFocus ? hwndFocus : hwndForeground;

		//处理消息
		if (CSkinEngine::TranslateMessageHelper(hWnd, WM_KEYDOWN, (LONG)wParam, (LONG)lParam, 0, 0, 0, TRUE))
			return TRUE;

    } else if (code == HC_ACTION && (lParam & 0x80000000) != 0) {   //KEYUP

        //获取获得消息的窗口
        HWND hwndForeground = ::GetForegroundWindow();
        DWORD dwCurrentThreadId = ::GetCurrentThreadId();
        DWORD dwWindowThreadId = ::GetWindowThreadProcessId(hwndForeground, NULL); 
        ::AttachThreadInput(dwCurrentThreadId, dwWindowThreadId, TRUE);
        HWND hwndFocus = ::GetFocus();
        HWND hWnd = hwndFocus ? hwndFocus : hwndForeground;

        //处理消息
        if (CSkinEngine::TranslateMessageHelper(hWnd, WM_KEYUP, (LONG)wParam, (LONG)lParam, 0, 0, 0, TRUE))
            return TRUE;

    }

	return CallNextHookEx(NULL, code, wParam, lParam);
}
