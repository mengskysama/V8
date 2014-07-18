/*
@file:		GGWebObj.h
@purpose:	用于web交互的自动化对象
@version:	1.0 2007.7.12 Danhq create
*/

// GGWebObj.h : Declaration of the CGGWebObj

#pragma once
#include "resource.h"       // main symbols

#include "x179.h"
#include <atlctl.h>
#include <atlstr.h>
#include <utility>

class ATL_NO_VTABLE CxPluginObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CxPluginObj, &CLSID_xPluginObj>,
	public IDispatchImpl<I179Plugin, &IID_I179Plugin, &LIBID_xPluginLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<CxPluginObj, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	CxPluginObj()
	{
    }

    ~CxPluginObj()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_179PluginOBJ)

BEGIN_COM_MAP(CxPluginObj)
	COM_INTERFACE_ENTRY(I179Plugin)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	
	// 获取版本
	STDMETHOD(GetVersion)(UINT* pbOk);
};

OBJECT_ENTRY_AUTO(__uuidof(xPluginObj), CxPluginObj)
