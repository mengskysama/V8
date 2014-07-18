
#pragma once

#include "SkinItem.h"

// CSkinItemStatic command target
class CImageBase;
class CSkinItemStatic : public CSkinItem
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinItemStatic)

protected:
	CSkinItemStatic();
	virtual ~CSkinItemStatic();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemStatic)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	enum 
	{
		SKINBASE_DISPIDS(CSkinItemStatic),
		SKINITEM_DISPIDS(CSkinItemStatic),
	};

public:
	virtual BOOL UpdateSkinItemProperty();

	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

};


