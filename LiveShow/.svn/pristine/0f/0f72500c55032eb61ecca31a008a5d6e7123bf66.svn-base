#pragma  once
#include <mshtml.h>
#include <mshtmdid.h>
#include "afxhtml.h"


class HtmlControlSite : public COleControlSite
{
public:
	HtmlControlSite(COleControlContainer* pParentWnd);
	~HtmlControlSite();
	

	CHtmlView* GetView() const;

	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*);
		STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT,
			LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(OnDocWindowActivate)(BOOL);
		STDMETHOD(OnFrameWindowActivate)(BOOL);
		STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD);
		STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD);
		STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*);
		STDMETHOD(GetExternal)(LPDISPATCH*);
		STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **);
		STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*);
		void EndStateSwitch(HtmlControlSite*);
		void BeginStateSwitch(HtmlControlSite*);
	END_INTERFACE_PART(DocHostUIHandler)
		DECLARE_INTERFACE_MAP()
private:
	AFX_MODULE_STATE * AFX_MODULE_STATE_m;
// 	AFX_MODULE_THREAD_STATE AFX_MODULE_THREAD_STATE_m;
// 	AFX_MODULE_THREAD_STATE backup_mts;
	char AFX_MODULE_THREAD_STATE_m[sizeof(AFX_MODULE_THREAD_STATE)];
	char backup_mts[sizeof(AFX_MODULE_THREAD_STATE)];
	AFX_MODULE_THREAD_STATE* mts;
};

