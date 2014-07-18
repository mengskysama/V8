// SkinClass.cpp : implementation file
//

#include "stdafx.h"
#include "Skin.h"
#include "SkinClass.h"

using namespace std;

// CSkinClass

IMPLEMENT_DYNCREATE(CSkinClass, CXmlCmdTarget)


CSkinClass::CSkinClass()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
}

CSkinClass::~CSkinClass()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CSkinClass::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSkinClass, CXmlCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CSkinClass, CXmlCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ISkinClass to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {166E53A9-7AA5-4E7B-BF06-C3306BF46224}
static const IID IID_ISkinClass =
{ 0x166E53A9, 0x7AA5, 0x4E7B, { 0xBF, 0x6, 0xC3, 0x30, 0x6B, 0xF4, 0x62, 0x24 } };

BEGIN_INTERFACE_MAP(CSkinClass, CXmlCmdTarget)
	INTERFACE_PART(CSkinClass, IID_ISkinClass, Dispatch)
END_INTERFACE_MAP()

// {C0106EC3-E7D3-4DEA-99F3-97C6BEB4DAAB}
IMPLEMENT_OLECREATE_FLAGS(CSkinClass, "Skin.SkinClass", afxRegApartmentThreading, 0xc0106ec3, 0xe7d3, 0x4dea, 0x99, 0xf3, 0x97, 0xc6, 0xbe, 0xb4, 0xda, 0xab)


// CSkinClass message handlers
CString CSkinClass::GetClassName()
{
    return m_strClassName;
}

void CSkinClass::SetClassName(LPCTSTR className)
{
    m_strClassName = className;
}

BOOL CSkinClass::GetProp(LPCTSTR propName, CString& propValue)
{
    CString strPropName = propName;
    strPropName.MakeLower();
    m_mapPropPair[strPropName] = propValue;
    map<CString, CString>::iterator finded = m_mapPropPair.find(strPropName);
    if (finded == m_mapPropPair.end())
        return FALSE;

    propValue = finded->second;
    return TRUE;
};

void CSkinClass::SetProp(LPCTSTR propName, LPCTSTR propValue)
{
    CString strPropName = propName;
    strPropName.MakeLower();
    m_mapPropPair[strPropName] = propValue;
};

BOOL CSkinClass::DelProp(LPCTSTR propName)
{
    return m_mapPropPair.erase(propName) != 0;
};

void CSkinClass::ApplyProp(CXmlCmdTarget* pTarget)
{
    map<CString, CString>::iterator iter = m_mapPropPair.begin(),
        last = m_mapPropPair.end();
    for (; iter!=last; iter++) {
        CComBSTR bstrName = iter->first;
        CComVariant varValue = iter->second;
        pTarget->SetProperty(bstrName, varValue);
    }
};

BOOL CSkinClass::LoadPropertyFromXml(IXMLDOMNodePtr ptrNode)
{
    IXMLDOMNodePtr pNode = NULL;
    IXMLDOMNamedNodeMapPtr pNamedNodeMap = NULL;
    DOMNodeType nodeType = NODE_INVALID;

    long len = 0;
    BOOL allSuc = TRUE;
    ptrNode->get_attributes(&pNamedNodeMap);
    pNamedNodeMap->get_length(&len);
    for (int i=0; i<len; i++){
        pNamedNodeMap->get_item(i, &pNode);
        pNode->get_nodeType(&nodeType);
        if (nodeType != NODE_ATTRIBUTE)
            continue;

        CComBSTR bstrName;
        CComVariant varValue;
        pNode->get_nodeName(&bstrName);
        pNode->get_nodeValue(&varValue);
        if (varValue.vt != VT_BSTR)
            continue;

        CString strName = bstrName;
        CString strValue = varValue.bstrVal;
        if (strName.CompareNoCase(_T("name")) != 0) {
            SetProp(strName, strValue);
        } else {
            ASSERT(m_strClassName.IsEmpty());
            SetClassName(strValue);
        }
    }

    return allSuc;
}
