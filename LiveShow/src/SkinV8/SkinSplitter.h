
#pragma once


// CSkinSplitter ÃüÁîÄ¿±ê

class CSkinSplitter : public CSkinBase
{
	DECLARE_DYNCREATE(CSkinSplitter)

public:
	CSkinSplitter();
	virtual ~CSkinSplitter();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinSplitter)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinSplitter),
	};
};


