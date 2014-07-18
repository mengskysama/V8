#pragma once

class CMouseHooker
{
public:
    CMouseHooker(void);
    ~CMouseHooker(void);
public:
    BOOL Hook(HWND hWnd);
    BOOL Unhook();
protected:
    static LRESULT CALLBACK MouseHookProc(int code, WPARAM wParam, LPARAM lParam);
protected:
    HHOOK m_hHook;
};
