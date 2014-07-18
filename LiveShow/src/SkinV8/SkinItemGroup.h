
#pragma once


// CSkinItemGroup ÃüÁîÄ¿±ê

class CSkinItemGroup : public CSkinItem
{
	DECLARE_DYNCREATE(CSkinItemGroup)

public:
	CSkinItemGroup();
	virtual ~CSkinItemGroup();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinItemGroup)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


