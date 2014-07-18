#include "StdAfx.h"
#include "SkinEngine.h"
#include "SkinItem.h"
#include "MouseHooker.h"

CMouseHooker::CMouseHooker(void)
:m_hHook(NULL)
{
}

CMouseHooker::~CMouseHooker(void)
{
    Unhook();
}

BOOL CMouseHooker::Hook(HWND hWnd)
{
    if (m_hHook != NULL)
        return FALSE;

    if (! ::IsWindow(hWnd))
        return FALSE;

    m_hHook = ::SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseHookProc, NULL, GetWindowThreadProcessId(hWnd, NULL));
    ASSERT(m_hHook != NULL);
    return (m_hHook != NULL);
}

BOOL CMouseHooker::Unhook()
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

LRESULT CMouseHooker::MouseHookProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0) {

        //调用下一钩子
        return CallNextHookEx(NULL, code, wParam, lParam);

    } else if (code == HC_ACTION) {	

        const PMOUSEHOOKSTRUCT pMouseStruct = (PMOUSEHOOKSTRUCT)lParam;
        HWND hWnd = pMouseStruct->hwnd;
        LONG mouseParam = pMouseStruct->pt.x + (pMouseStruct->pt.y << 16);

        if (wParam == WM_MOUSEMOVE) {

            if (CSkinEngine::TranslateMessageHelper(hWnd, WM_MOUSEMOVE, 0, mouseParam, 0, 0, 0, TRUE))
                return TRUE;

        } else if (wParam == WM_LBUTTONDOWN) {

            if (CSkinEngine::TranslateMessageHelper(hWnd, WM_LBUTTONDOWN, 0, mouseParam, 0, 0, 0, TRUE))
                return TRUE;

        } else if (wParam == WM_LBUTTONUP) {

            if (CSkinEngine::TranslateMessageHelper(hWnd, WM_LBUTTONUP, 0, mouseParam, 0, 0, 0, TRUE))
                return TRUE;

        } else if (wParam == WM_RBUTTONDOWN) {

            if (CSkinEngine::TranslateMessageHelper(hWnd, WM_RBUTTONDOWN, 0, mouseParam, 0, 0, 0, TRUE))
                return TRUE;

        }  else if (wParam == WM_RBUTTONUP) {

            if (CSkinEngine::TranslateMessageHelper(hWnd, WM_RBUTTONUP, 0, mouseParam, 0, 0, 0, TRUE))
                return TRUE;

        } 

    } 

    return CallNextHookEx(NULL, code, wParam, lParam);
}
