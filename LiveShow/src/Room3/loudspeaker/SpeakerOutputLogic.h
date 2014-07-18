#pragma once
#include "..\..\HTMLEdit\HtmlEditLogic.h"

class CRoomParentDlg;
class CSpeakerOutputLogic :
	public HtmlEditLogic
{
public:
	CSpeakerOutputLogic( CRoomParentDlg* pParentDlg, bool bScrollBtn );
	virtual ~CSpeakerOutputLogic(void);

	BOOL OnClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj);
	BOOL OnMouseOut(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) ;
	BOOL OnMouseMove(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) ;

	void OnOutputReady(HtmlEditImpl* pHtmlEditImp);
private:
	CString GetValue(CComPtr<IHTMLElement> & p,CString key);

private:
	CRoomParentDlg* m_pParentDlg ;
	bool m_bScrollBtn;

};
