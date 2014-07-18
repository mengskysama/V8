#pragma once

class CKeyboardHooker
{
public:
	CKeyboardHooker(void);
	~CKeyboardHooker(void);
public:
	BOOL Hook(HWND hWnd);
	BOOL Unhook();
protected:
	static LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
protected:
	HHOOK m_hHook;
};
