#pragma once


// CTempAnnounceListCtrl

class CTempAnnounceListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTempAnnounceListCtrl)

public:
	CTempAnnounceListCtrl();
	virtual ~CTempAnnounceListCtrl();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) ;
protected:
	DECLARE_MESSAGE_MAP()
};


