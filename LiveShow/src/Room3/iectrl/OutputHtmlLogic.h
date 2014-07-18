#pragma  once

#include "..\..\HTMLEdit\HtmlEditLogic.h"
#include "..\GlobalRoomDefine.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>
#include <boost/format.hpp>

using namespace boost::xpressive;

class CRoomParentDlg ;

class COutputHtmlLogic:
	public HtmlEditLogic
{
public:
	COutputHtmlLogic(CRoomParentDlg* pParentDlg , ENM_HTML_TYPE nHtmlType );
	virtual ~COutputHtmlLogic(void);
public:
	void TranslateMsgForInput(CString& strText) ;

	BOOL OnClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj);
	BOOL OnMouseOut(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) ;
	BOOL OnMouseMove(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) ;
	void OnOutputReady(HtmlEditImpl* pHtmlEditImp);
	virtual void OnTrackHrefLinkMenu( uint32 unDstUin ) ;
	void SetInitText( CString& strText ) ;
private:
	CString GetValue(CComPtr<IHTMLElement> & p,CString key) ;

private:
	CRoomParentDlg* m_pParentDlg ;
	CString m_strInitText ;
	ENM_HTML_TYPE m_nHtmlType ;
};