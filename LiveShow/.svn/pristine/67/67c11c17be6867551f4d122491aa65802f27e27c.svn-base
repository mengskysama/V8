

#pragma once

#include <afxole.h>

// 拖动类型
#define DRAGTYPE_NONE			0
#define DRAGTYPE_FILE			1		// 文件

class CMyOleDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CMyOleDropTarget)
	DECLARE_INTERFACE_MAP()
	DECLARE_MESSAGE_MAP()

public:
	CMyOleDropTarget();
	virtual ~CMyOleDropTarget();

protected:
	LPDROPTARGET m_pIEDropTarget;

public:
	void SetIEDropTarget(LPDROPTARGET pDropTarget);
	void ReleaseIEDropTarget();

public:
	BEGIN_INTERFACE_PART(MyDropTarget, IDropTarget)
		INIT_INTERFACE_PART(CMyOleDropTarget, MyDropTarget)
		STDMETHOD(DragEnter)(LPDATAOBJECT, DWORD, POINTL, LPDWORD);
		STDMETHOD(DragOver)(DWORD, POINTL, LPDWORD);
		STDMETHOD(DragLeave)();
		STDMETHOD(Drop)(LPDATAOBJECT, DWORD, POINTL pt, LPDWORD);
	END_INTERFACE_PART(MyDropTarget)
};
