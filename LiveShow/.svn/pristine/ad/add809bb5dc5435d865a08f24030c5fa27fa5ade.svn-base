// Skin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Skin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _USRDLL

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CSkinApp

BEGIN_MESSAGE_MAP(CSkinApp, CWinApp)
END_MESSAGE_MAP()


// CSkinApp construction

CSkinApp::CSkinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CSkinApp::~CSkinApp()
{
}

// The one and only CSkinApp object

CSkinApp theApp;

// CSkinApp initialization

BOOL CSkinApp::InitInstance()
{
	CWinApp::InitInstance();

	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

int CSkinApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::ExitInstance();
}

#endif // _USRDLL


#ifdef _AFXEXT

#include <afxdllx.h>
#ifdef _MANAGED
#error Please read instructions in TestDll3.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

static AFX_EXTENSION_MODULE SkinDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("Skin.DLL 正在初始化!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(SkinDLL, hInstance))
            return 0;

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.

        new CDynLinkLibrary(SkinDLL);

        //COleObjectFactory::RegisterAll();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("Skin.DLL 正在终止1!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(SkinDLL);

		TRACE0("Skin.DLL 正在终止2!\n");
    }
    return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif //_AFXEXT


const GUID CDECL BASED_CODE _tlid =
{ 0x17EC7DDD, 0x4730, 0x4E8C, { 0xBC, 0x85, 0xCF, 0xAE, 0x1B, 0x18, 0x9, 0x75 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

#ifdef _AFXEXT

STDAPI AfxExtDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    *ppv = NULL;

    // search factories defined in the application
    AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
    //AfxLockGlobals(CRIT_OBJECTFACTORYLIST);
    COleObjectFactory* pFactory;
    for (pFactory = pModuleState->m_factoryList;
        pFactory != NULL; pFactory = pFactory->m_pNextFactory)
    {
        REFCLSID rcFactoryClass = pFactory->GetClassID();
        if (//pFactory->IsRegistered() &&
            rclsid.Data1 == rcFactoryClass.Data1 &&
            ((DWORD*)&rclsid)[1] == ((DWORD*)&rcFactoryClass)[1] &&
            ((DWORD*)&rclsid)[2] == ((DWORD*)&rcFactoryClass)[2] &&
            ((DWORD*)&rclsid)[3] == ((DWORD*)&rcFactoryClass)[3])
        {
            // found suitable class factory -- query for correct interface
            SCODE sc = pFactory->InternalQueryInterface(&riid, ppv);
            //AfxUnlockGlobals(CRIT_OBJECTFACTORYLIST);
            return sc;
        }
    }
    //AfxUnlockGlobals(CRIT_OBJECTFACTORYLIST);
#ifdef _AFXDLL
    //AfxLockGlobals(CRIT_DYNLINKLIST);
    // search factories defined in extension DLLs
    CDynLinkLibrary* pDLL;
    for (pDLL = pModuleState->m_libraryList; pDLL != NULL;
        pDLL = pDLL->m_pNextDLL)
    {
        for (COleObjectFactory* pDLLFactory = pDLL->m_factoryList;
            pDLLFactory != NULL; pDLLFactory = pDLLFactory->m_pNextFactory)
        {
            REFCLSID rcFactoryClass = pDLLFactory->GetClassID();
            if (//pDLLFactory->IsRegistered() &&
                rclsid.Data1 == rcFactoryClass.Data1 &&
                ((DWORD*)&rclsid)[1] == ((DWORD*)&rcFactoryClass)[1] &&
                ((DWORD*)&rclsid)[2] == ((DWORD*)&rcFactoryClass)[2] &&
                ((DWORD*)&rclsid)[3] == ((DWORD*)&rcFactoryClass)[3])
            {
                // found suitable class factory -- query for correct interface
                SCODE sc = pDLLFactory->InternalQueryInterface(&riid, ppv);
                //AfxUnlockGlobals(CRIT_DYNLINKLIST);
                return sc;
            }
        }
    }
    //AfxUnlockGlobals(CRIT_DYNLINKLIST);
#endif

    // factory not registered -- return error
    return CLASS_E_CLASSNOTAVAILABLE;
}

#endif //_AFXEXT

// DllGetClassObject - Returns class factory

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _USRDLL
    return AfxDllGetClassObject(rclsid, riid, ppv);
#endif //_USRDLL

#ifdef _AFXEXT
    return AfxExtDllGetClassObject(rclsid, riid, ppv);
#endif //_AFXEXTS

}


// DllCanUnloadNow - Allows COM to unload DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return AfxDllCanUnloadNow();
}


// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _USRDLL
    if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
        return SELFREG_E_TYPELIB;
#endif //_USRDLL

#ifdef _AFXEXT
    if (!AfxOleRegisterTypeLib(SkinDLL.hModule, _tlid))
        return SELFREG_E_TYPELIB;
#endif //_AFXEXT

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}
