#ifndef COOLSB_DETOURS_INCLUDED
#define COOLSB_DETOURS_INCLUDED

#ifdef COOLSB_EXPORTS
#define COOLSB_API __declspec(dllexport) WINAPI
#else
#define COOLSB_API __declspec(dllimport) WINAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif

BOOL COOLSB_API Detour_EnableScrollBar(HWND hwnd, int wSBflags, UINT wArrows);
BOOL COOLSB_API Detour_GetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi);
int  COOLSB_API Detour_GetScrollPos(HWND hwnd, int nBar);
BOOL COOLSB_API Detour_GetScrollRange(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos);
int  COOLSB_API Detour_SetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi, BOOL fRedraw);
int  COOLSB_API Detour_SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL fRedraw);
int  COOLSB_API Detour_SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw);
BOOL COOLSB_API Detour_ShowScrollBar(HWND hwnd, int wBar, BOOL fShow);
UINT_PTR COOLSB_API Detour_SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
HGDIOBJ COOLSB_API Detour_SelectObject(HDC hDC, HGDIOBJ hObj);

BOOL COOLSB_API CoolSB_InitializeApp(void);
BOOL COOLSB_API CoolSB_UninitializeApp(void);

#ifdef __cplusplus
}
#endif

#endif