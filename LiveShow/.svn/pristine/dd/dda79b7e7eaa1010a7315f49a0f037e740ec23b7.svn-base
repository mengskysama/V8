/********************************************************************
	created:	
	created:	
	file base:	
	file ext:	h
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#ifndef __DIRECTUI_X_H__
#define __DIRECTUI_X_H__


//////////////////////////////////////////////////////////////////////////
class CActiveXCtrl;


//////////////////////////////////////////////////////////////////////////
class CActiveXWnd : public CWnd
{
public:
	HWND Init(CActiveXCtrl* pOwner, HWND hWndParent, int width, int height);
	
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool &bHandled);
	
protected:
	void DoVerb(LONG iVerb);
	
	LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	
protected:
	CActiveXCtrl* m_pOwner;
};


#endif  //__DIRECTUI_X_H__