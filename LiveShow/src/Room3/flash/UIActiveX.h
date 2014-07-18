/********************************************************************
	created:	2006/04/12
	created:	12:4:2006   16:41
	file base:	UIString
	file ext:	cpp
	author:		Garret Lian
	
	purpose:	
*********************************************************************/
#ifndef __DIRECTUI_ACTIVEX_H__
#define __DIRECTUI_ACTIVEX_H__

#include "..\stdafx.h"
#include "UIActiveXBase.h"

//////////////////////////////////////////////////////////////////////////
class CActiveXUI : public IActivXOwner
{
public:
	CActiveXUI();
	~CActiveXUI();

public:
	LPCTSTR		GetClass() const;
	LPVOID		QueryInterface(LPCTSTR pstrName);

	bool		CreateControl(const CLSID clsid);
	bool		CreateControl(LPCTSTR pstrCLSID);

	SIZE		EstimateSize(SIZE szAvailable);

	void		SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	LRESULT		MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	//IActivXOwner
	HWND		AxGetHWND();
	void		AxSetFocus(bool bFocus);

protected:
	virtual void ReleaseControl();
	virtual bool DelayedControlCreation();

	void		SetPos(RECT rc);
	void		DoPaint(HDC hDC, const RECT& rcPaint);

protected:
	CLSID m_clsid;
	bool m_bCreated;

	CActiveXCtrl *m_pControl;
};


#endif // __DIRECTUI_ACTIVEX_H__
