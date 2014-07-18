
#pragma once

#include "SkinItem.h"

// CSkinItemTree ÃüÁîÄ¿±ê

class CSkinItemTree : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemTree)

public:
	CSkinItemTree();
	virtual ~CSkinItemTree();

	virtual void OnFinalRelease();
protected:
    enum 
    {
        SKINBASE_DISPIDS(CSkinItemTree),
        SKINITEM_DISPIDS(CSkinItemTree),
    };

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemTree)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
    virtual BOOL OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
};


