#pragma once

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

#define MB_OKCANCEL_UPDATE (MB_CANCELTRYCONTINUE+1)

class UTILITY_API C51SkinMessageBox
{
public:
	static int Show(LPCTSTR pzPrompt);
	static int Show(CWnd* pParent, LPCTSTR pzPrompt);
	static int Show(HWND hParent, LPCTSTR pzPrompt);
	static int Show(LPCTSTR pzPrompt, LPCTSTR pzTitle, UINT uType = MB_OK);
	static int Show(CWnd* pParent, LPCTSTR pzPrompt, LPCTSTR pzTitle, UINT uType = MB_OK);
	static int Show(HWND hParent, LPCTSTR pzPrompt, LPCTSTR pzTitle, UINT uType = MB_OK);
};
