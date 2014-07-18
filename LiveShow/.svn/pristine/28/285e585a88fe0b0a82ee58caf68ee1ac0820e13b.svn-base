#pragma once

#include "SkinHeader.h"

//skin macros

#define WM_IMSKIN_NOTIFY	(WM_USER + 2001)
#define WM_IM_NOTIFY		(WM_USER + 2002)
#define WM_CTRL_CLICK_3		(WM_USER + 2003)
#define WM_SKIN_MOUSEENTER	(WM_USER + 0x9874)
#define WM_SKIN_MOUSELEAVE	(WM_USER + 0x9875)
#define WM_SKIN_PAINT		(WM_USER + 0x9876)
#define WM_SKIN_ERASEBKGND	(WM_USER + 0x9877)
#define ON_SKIN_MESSAGE		ON_MESSAGE(WM_IMSKIN_NOTIFY, OnSkinMessage)
#define NOTIFY_SKIN_HELPER(wParam, lParam) NotifySkinHelper(GetSafeHwnd(), wParam, lParam)
#define SKIN_MESSAGE_CASE(msg, handler) \
    case msg: \
        handler(); \
        break; 

//skin helper functions
LRESULT UTILITY_API NotifySkinHelper(HWND hWnd, WPARAM wParam, LONG num);
LRESULT UTILITY_API NotifySkinHelper(HWND hWnd, WPARAM wParam, HWND h);
LRESULT UTILITY_API NotifySkinHelper(HWND hWnd, WPARAM wParam, CString str);

#include "C51SkinMessageBox.h"
