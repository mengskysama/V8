
#pragma once

#include "XmlCmdTarget.h"
#include <map>

// CSkinClass command target

class CSkinClass : public CXmlCmdTarget
{
	DECLARE_DYNCREATE(CSkinClass)

public:
	CSkinClass();
	virtual ~CSkinClass();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinClass)
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
    CString GetClassName();
    void SetClassName(LPCTSTR className);

    BOOL GetProp(LPCTSTR propName, CString& propValue);
    void SetProp(LPCTSTR propName, LPCTSTR propValue);
    BOOL DelProp(LPCTSTR propName);
    void ApplyProp(CXmlCmdTarget* pTarget);

public:
    virtual BOOL LoadPropertyFromXml(IXMLDOMNodePtr ptrNode);

protected:
    CString m_strClassName;
    // Ù–‘ºØ∫œ
    std::map<CString, CString> m_mapPropPair;
};

