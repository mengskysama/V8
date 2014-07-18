// SkinItemStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBase.h"
#include "SkinItemStatic.h"


// CSkinItemStatic

IMPLEMENT_DYNCREATE(CSkinItemStatic, CSkinItem)
CSkinItemStatic::CSkinItemStatic()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.

	AfxOleLockApp();
}

CSkinItemStatic::~CSkinItemStatic()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CSkinItemStatic::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinItemStatic, CSkinItem)
END_MESSAGE_MAP()


BEGIN_SKIN_DISPATCH_MAP(CSkinItemStatic, CSkinItem)
	SKINBASE_DISP_PROPERTIES(CSkinItemStatic)
	SKINITEM_DISP_PROPERTIES(CSkinItemStatic)

END_SKIN_DISPATCH_MAP(CSkinItemStatic)

// Note: we add support for IID_ISkinItemStatic to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {6EC370AA-63E2-4589-870C-A65CDCB68CEA}
static const IID IID_ISkinItemStatic =
{ 0x6EC370AA, 0x63E2, 0x4589, { 0x87, 0xC, 0xA6, 0x5C, 0xDC, 0xB6, 0x8C, 0xEA } };

BEGIN_INTERFACE_MAP(CSkinItemStatic, CSkinItem)
	INTERFACE_PART(CSkinItemStatic, IID_ISkinItemStatic, Dispatch)
END_INTERFACE_MAP()

// {14C821C6-073E-4CB1-9E48-0E0BE236C0B7}
IMPLEMENT_OLECREATE_FLAGS(CSkinItemStatic, "Skin.SkinItemStatic", afxRegApartmentThreading, 0x14c821c6, 0x73e, 0x4cb1, 0x9e, 0x48, 0xe, 0xb, 0xe2, 0x36, 0xc0, 0xb7)


// CSkinItemStatic message handlers

BOOL CSkinItemStatic::UpdateSkinItemProperty()
{
	if (! CSkinItem::UpdateSkinItemProperty())
		return FALSE;
	return TRUE;
}

BOOL CSkinItemStatic::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	*lpResult = 0;

	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	if (pWnd == NULL)
		return FALSE;

	CPaintDC dc(pWnd);
	PAINTSTRUCT paintStruct = dc.m_ps;

	CRect rect = GetSkinRect();
	if (m_pbkImageDisabled != NULL){
		m_pbkImageDisabled->Draw(dc.GetSafeHdc(), &rect);
	}

	//¼ÆËã»æÖÆÎÄ×Ö
	CRect textRect = GetSkinAbsRect();

	m_textStyleNormal.DrawText(dc.GetSafeHdc(), m_strText, &textRect);

	return !GetselfOnPaint();
}
