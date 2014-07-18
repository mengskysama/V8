
#pragma once

#include "XmlCmdTarget.h"

// CSkinImageStyle ÃüÁîÄ¿±ê

class CSkinImageStyle : public CXmlCmdTarget
{
	DECLARE_DYNCREATE(CSkinImageStyle)

public:
	CSkinImageStyle();
	virtual ~CSkinImageStyle();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinImageStyle)
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


