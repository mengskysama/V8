/*
@file:		HtmlEditCore2.h
@purpose:	ie编辑器
@feature:	
@version:	1.0 2008.01.17 created by ken
*/
#pragma  once

#define _WTL_NO_AUTOMATIC_NAMESPACE
#include <atlbase.h>
#include <ExDispid.h>
#include <ComDef.h>
#include "stdlib.h"
#include "atlstr.h"
#include "afxhtml.h"
#include <MsHTML.h>//定义了IHTMLDocument2等接口
#include <mshtmdid.h>//定义了HTMLDocumentEvents2接口的方法DISPID
#include <afxole.h>
#include <afxcoll.h>
#include <memory>
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <stack>
#include <utility>
#include "HtmlEditSetting.h"
#include "HtmlEditLogic.h"

#ifdef _UNICODE
#include  <xstring>
#include <sstream>
#ifdef TCHAR
#undef TCHAR
#define TCHAR wchar_t
#endif
#else
#include <string>
#include <sstream>
#ifdef TCHAR
#undef TCHAR
#define TCHAR char
#endif
#endif

#ifdef HTMLEDIT_EXPORTS
#define HTMLEDIT_API __declspec(dllexport)
#else
#define HTMLEDIT_API __declspec(dllimport)
#endif

using namespace std;

class HtmlEditLogic;

class HTMLEDIT_API HtmlEditImpl: public CHtmlEditView
{	
	DECLARE_DISPATCH_MAP()
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(HtmlEditImpl)

public:

	HtmlEditImpl();
	virtual ~HtmlEditImpl(void);

	void SetHtmlEditLogic(HtmlEditLogic* );
	HtmlEditLogic* GetHtmlEditLogic();

	virtual void InsertText(CString text);
	virtual void InsertDivText(CString text, CString div);

	void InsertNewLine();
	void PutBodyInnerHTML(CString s);
	virtual void ClearMsg();	
	void ClearDiv(std::wstring const & const id);

	BOOL Create(CWnd* pParent,UINT nID,CRect& rcIE, DWORD style);	
	
	void SetGotoEndAble() ;   //允许主动滚屏
	void SetUnGotoEnd() ;
	void GotoEnd();		
	
	long GetScrollPos();
	long GetScrollHeight();
	long GetClientHeight();

	void GetContent(CString & content);
	long GetContentLen();
	bool HasContent();
	bool HasContentSelect();//是否有内容被选中	
	CString GetContentText();
	unsigned long CountInnterTextLen(CString & const input);

	void SetUnEditAble();				//设置是否可以编辑 add by xiaoyi
	void SetEditAble();
	
	void SetBgColor(CString color);
	void InsertAtSelectionArea(CString & content);
	void UpdateMy3();//用于字体改变后更新设置
	void UpdateMy4();
	void _SetFocus();	
	BOOL IsReady();
	void GetHtmlElement(CString id,IHTMLElement ** pElem2);	

	void OnEditCopy(CString & strHtmlText,CString & strText);
	void OnEditCut(CString & strHtmlText,CString & strText);
	int OnEditPaste(CString & strHtmlText,long type);

	//test
	void OutputDebugInfo();
	void OnNavigateComplete2(LPDISPATCH pDisp, VARIANT FAR* URL);
    void OnHtmlDownloadComplete();

	void DeleteDom(CString strParent, CString strToDel);

	void OnEditCopyWord( CString& strText ) ;		//复制文字
	void OnEditCopyHtml( CString& strHtmlText) ;	//复制HTML代码	

	CString GetValue(CComPtr<IHTMLElement> & p,CString key);	

	BOOL CallJScript(const CString strFunc, const CStringArray& paramArray,_variant_t* pVarResult);

	CComPtr<IHTMLDocument2> GetHtmlDocument2(){ return m_pDoc2 ; } ;
	virtual HRESULT OnGetDropTarget(LPDROPTARGET pDropTarget,LPDROPTARGET* ppDropTarget );
	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) ;
private:

	// ON_MESSAGE

	LRESULT OnGetFocus(WPARAM wParam, LPARAM lParam);			

	// dispatch

	BOOL OnKeyDown(IHTMLEventObj *pEvtObj);
	BOOL onClick(IHTMLEventObj *pEvtObj);
	BOOL OnMouseOut(IHTMLEventObj *pEvtObj) ;
	BOOL OnMouseMove(IHTMLEventObj *pEvtObj) ;
	BOOL OnControlSelect(IHTMLEventObj *pEvtObj) ;
	//用于显示默认字
	virtual BOOL OnHtmlFocusIn(IHTMLEventObj *pEvtObj);
	virtual BOOL OnHtmlFocusOut(IHTMLEventObj *pEvtObj);

	// ON_EVENT_REFLECT

	void OnProgressChange(long lProgress, long lProgressMax);

	// CWnd

	int OnCreate(LPCREATESTRUCT p);
	afx_msg void OnDestroy();

	// menu

	void OnEditCopyWord() ;		//复制文字
	void OnEditCleanScreen() ;	//清屏
	void OnEditPasteWrapper();	//粘贴

	virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved,	LPDISPATCH pdispReserved);
	void OnTrackMenuResult( int nreturn , CString& strName , CString& strUUID ) ;

private:
	
	LPCTSTR GetStartDocument();
	CString GetURLFromResource(CString nRes);	
	void  UpdateMy2(CMapStringToString & font );
	BOOL IsClipbordAvailable();

	void BuildEnviromentInput();
	void BuildEnviromentOutput();
			
	void InsertTextDiv(CString s);

	int GetFontColor();
	void ConstructScrollColor();
	void InnerGotoEnd();
	virtual void OnOutputReady();	

protected:

	HtmlEditLogic* m_pLogic;
	CComPtr<IHTMLDocument2> m_pDoc2;
	COleDropTarget m_DropTarget;	
private:

	bool m_bReady;
	DWORD m_dwCookie;	
public:
	bool m_bIsNeedGotoEnd;
	WNDPROC m_pOrgIEProc ;
	static std::map<HWND,CWnd*> m_staTable ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage( MSG* pMsg ) ;
};





