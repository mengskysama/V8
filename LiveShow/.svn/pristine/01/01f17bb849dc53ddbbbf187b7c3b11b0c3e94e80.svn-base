
#pragma once


// CSkinWndBase ÃüÁîÄ¿±ê

class CSkinWndBase : public CCmdTarget
{
	DECLARE_DYNAMIC(CSkinWndBase)

public:
	CSkinWndBase();
	virtual ~CSkinWndBase();

	virtual void OnFinalRelease();
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinWndBase)
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	//Assert
	virtual void SkinAssert(BOOL bAssert = TRUE);
	//Attributes
	DWORD GetStyle() const;
	DWORD GetExStyle() const;
	//Message
	LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT SendNotifyMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	//Window Text Functions
	int GetWindowTextLength() const;
	int GetWindowText(CString& strText) const;
	int SetWindowText(const CString& strText);

	//Font Functions
	HFONT GetFont() const;
	void SetFont(HFONT hFont, BOOL bRedraw = TRUE);

	//Menu Functions
	HMENU GetMenu() const;
	BOOL SetMenu(HMENU hMenu);
	//BOOL DrawMenuBar();
	//HMENU GetSystemMenu(BOOL bRevert);
	//BOOL HiliteMenuItem(HMENU hMenu, UINT uItemHilite,UINT uItem);

	//Windows Size and Position Functions
	BOOL IsIconic() const;
	BOOL IsZoomed() const;
	BOOL MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	//BOOL SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UINT nFlags);
	//UINT ArrangeIconicWindows();
	//BOOL BringWindowToTop();
	BOOL GetWindowRect(LPRECT lpRect) const;
	BOOL GetClientRect(LPRECT lpRect) const;
	
};

#define SKINASSERT(b) SkinAssert(b)
