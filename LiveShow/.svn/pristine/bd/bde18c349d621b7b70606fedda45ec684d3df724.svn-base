#pragma once
#include "..\..\HTMLEdit\HtmlEditCore2.h"
#include "..\GlobalRoomDefine.h"

#define  WM_OUTPUTHTMLEDITIMPLM_FOCUS_IN            WM_USER+3344
#define  WM_OUTPUTHTMLEDITIMPLM_FOCUS_OUT			 WM_USER+3345

class COutputHtmlEditImpl : 	public HtmlEditImpl
{
public:
	COutputHtmlEditImpl(uint32 nHoldLine , ENM_HTML_TYPE nHtmlType );
	virtual ~COutputHtmlEditImpl(void);
	virtual void InsertText(CString& s) ;
	virtual void ClearMsg() ;

	//用于显示默认字
	virtual BOOL OnHtmlFocusIn(IHTMLEventObj *pEvtObj);
	virtual BOOL OnHtmlFocusOut(IHTMLEventObj *pEvtObj);
private:
	void DeleteDom(CString& strToDel);
	uint32 m_nLine;
	uint32 m_nHoldLine;
	CComPtr<IHTMLElement> m_pDisplay;
	ENM_HTML_TYPE m_nHtmlType ;
};