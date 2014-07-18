// SkinEngine.cpp : implementation file
//

#include "stdafx.h"

#include "SkinClass.h"
#include "ImageRepository.h"
#include "SkinBase.h"
#include "SkinProxy.h"
#include "SkinButton.h"
#include "SkinImage.h"
#include "SkinRoundImage.h"
#include "SkinProgress.h"
#include "SkinText.h"
#include "SkinLibretto.h"
#include "SkinTab.h"
#include "SkinGroup.h"
#include "SkinSplitter.h"
#include "SkinItem.h"
#include "SkinItemDialog.h"
#include "SkinItemStatic.h"
#include "SkinItemCombo.h"
#include "SkinItemButton.h"
#include "SkinItemEdit.h"
#include "SkinItemList.h"
#include "SkinItemSlider.h"
#include "SkinItemProgress.h"
#include "SkinItemGroup.h"
#include "SkinItemTab.h"
#include "SkinItemSplitter.h"
#include "SkinItemHeader.h"
#include "SkinItemTree.h"

#include "SkinPackEncoder.h"

#include "SkinJSEngine.h"
#include "SkinJSEngineV8.h"

#include "JSEngine.h"
#include "JSEngineV8.h"

#include "SkinHelper.h"

#ifdef USE_GLOBAL_MOUSE_HOOKER
#include "GlobalMouseHooker.h"
#endif //USE_GLOBAL_MOUSE_HOOKER

#ifdef USE_GLOBAL_KEYBOARD_HOOKER
#include "GlobalKeyboardHooker.h"
#endif //USE_GLOBAL_KEYBOARD_HOOKER

//IME
#include <Imm.h>
#pragma comment(lib, "imm32.lib")

#include ".\skinengine.h"

#include <map>
#include <algorithm>
using namespace std;

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

#pragma warning (disable:4311)
#pragma warning (disable:4312)

// CSkinEngine

IMPLEMENT_DYNCREATE(CSkinEngine, CCmdTarget)
DELEGATE_DUAL_INTERFACE(CSkinEngine, SkinEngine)
CSkinEngine::CSkinEngine()
: 
m_bSkinEngineSetLoad(false),
m_bSkinEngineWndMapLoad(false),
m_hEngineWnd(NULL),
m_pOldProc(NULL)
{
	TRACE(("SkinEngine的构造函数开始\n"));

	::CoInitialize(NULL);

	//记录
	s_setAllSkinEngine.Add(this);

	//
	m_hEngineWnd = ::CreateWindow(_T("STATIC"), NULL, WS_POPUP,
		0, 0, 0, 0, 
		HWND_MESSAGE,	//Message-only
		NULL, NULL, NULL);
	ASSERT(::IsWindow(m_hEngineWnd));
	if (::IsWindow(m_hEngineWnd)){
		m_pOldProc = (WNDPROC)::SetWindowLong(m_hEngineWnd, GWL_WNDPROC, (LONG)_SkinEngineWndProc);
		s_mapSkinEngineWnd.AddPair(m_hEngineWnd, this);
		//AddBuffer(_T("SingleChatDlg"));
		//AddBuffer(_T("TribeChatDlg"));
		::SetTimer(m_hEngineWnd, SKINENGINE_IDLE_TIMER, 300, NULL);
	}

	EnableAutomation();

	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	m_pImageRepository = NULL;
	m_pImageRepository = new CImageRepository;

	m_pFileRepository = NULL;
	m_pFileRepository = new CFileRepository;

	//设置默认图像变换参数
	SetDefaultImageParam(CImageTransformParam::s_globalParam);

#ifdef USE_GDIPLUS
	//初始化Gdiplus
	m_pGdiToken = NULL;
	GdiplusStartup(&m_pGdiToken , &m_gdiplusStartupInput , NULL);
#endif //USE_GDIPLUS

	//
	CSkinEngine::Initialize();

    //获得控件库版本
    memset(&m_comctlVerInfo, 0, sizeof(m_comctlVerInfo));
    CModuleVersion::DllGetVersion(_T("comctl32.dll"), m_comctlVerInfo);

    m_bGlobalMouseHook = FALSE;
    m_bGlobalKeyboardHook = FALSE;

#ifdef USE_GLOBAL_MOUSE_HOOKER
    m_bGlobalMouseHook = CGlobalMouseHooker::StartMouseHooker();
    if (m_bGlobalMouseHook)
        CGlobalMouseHooker::SetMouseReceiver(m_hEngineWnd);
#endif //USE_GLOBAL_MOUSE_HOOKER

#ifdef USE_GLOBAL_KEYBOARD_HOOKER
    m_bGlobalKeyboardHook = CGlobalKeyboardHooker::StartKeyboardHooker();
    if (m_bGlobalKeyboardHook)
        CGlobalKeyboardHooker::SetKeyboardReceiver(m_hEngineWnd);
#endif//USE_GLOBAL_KEYBOARD_HOOKER

	TRACE(("SkinEngine的构造函数结束\n"));
}

CSkinEngine::~CSkinEngine()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	TRACE(("SkinEngine的析构函数开始\n"));

#ifdef USE_GLOBAL_MOUSE_HOOKER
    if (m_bGlobalMouseHook)
        CGlobalMouseHooker::StopMouseHooker();
#endif //USE_GLOBAL_MOUSE_HOOKER

#ifdef USE_GLOBAL_KEYBOARD_HOOKER
    if (m_bGlobalKeyboardHook)
        CGlobalKeyboardHooker::StopKeyboardHooker();
#endif //USE_GLOBAL_KEYBOARD_HOOKER

	DestroyAllSkin();

    RemoveAllSkinClass();

	AfxOleUnlockApp();

	if (m_pImageRepository != NULL){
		delete m_pImageRepository;
		m_pImageRepository = NULL;
	}

	if (m_pFileRepository != NULL){
		delete m_pFileRepository;
		m_pFileRepository = NULL;
	}

	//删除记录
	if (m_bSkinEngineSetLoad)
		s_setAllSkinEngine.Remove(this);

	//
	if (m_bSkinEngineWndMapLoad)
		s_mapSkinEngineWnd.RemovePair(m_hEngineWnd, this);
	ASSERT(::IsWindow(m_hEngineWnd));
	if (::IsWindow(m_hEngineWnd)){
		::SetWindowLong(m_hEngineWnd, GWL_WNDPROC, (LONG)m_pOldProc);
		::DestroyWindow(m_hEngineWnd);
		m_hEngineWnd = NULL;
		m_pOldProc = NULL;
	}

	//
	CSkinEngine::Uninitialize();

	::CoUninitialize();

#ifdef USE_GDIPLUS
	//卸载Gdiplus
	Gdiplus::GdiplusShutdown(m_pGdiToken);
#endif //USE_GDIPLUS

	TRACE(("SkinEngine的析构函数结束\n"));
}


void CSkinEngine::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


//////////////////////////////////////////////////////////////////////////
//Initialize and Uninitialize
//////////////////////////////////////////////////////////////////////////
BOOL CSkinEngine::Initialize()
{
    m_KeyboardHooker.Hook(m_hEngineWnd);
    //m_MouseHooker.Hook(m_hEngineWnd);
	return TRUE;
}

BOOL CSkinEngine::Uninitialize()
{
    m_KeyboardHooker.Unhook();
    //m_MouseHooker.Unhook();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSkinEngine, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CSkinEngine, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ISkinEngine to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {91424951-C708-40A4-A0E3-C0A795A1110A}
static const IID IID_ISkinEngine =
{ 0x91424951, 0xC708, 0x40A4, { 0xA0, 0xE3, 0xC0, 0xA7, 0x95, 0xA1, 0x11, 0xA } };

BEGIN_INTERFACE_MAP(CSkinEngine, CCmdTarget)
	INTERFACE_PART(CSkinEngine, IID_ISkinEngine, SkinEngine)
END_INTERFACE_MAP()

// {C02E7228-4E76-44C5-8652-BC34AEC7948E}
IMPLEMENT_OLECREATE_FLAGS(CSkinEngine, "Skin.SkinEngine", afxRegApartmentThreading, 0xc02e7228, 0x4e76, 0x44c5, 0x86, 0x52, 0xbc, 0x34, 0xae, 0xc7, 0x94, 0x8e)




STDMETHODIMP CSkinEngine::XSkinEngine::LoadSkin(HWND hWnd, BSTR bstrSkinPath)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->LoadSkin(hWnd, bstrSkinPath);
}

STDMETHODIMP CSkinEngine::XSkinEngine::UnloadSkin(HWND hWnd)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->UnloadSkin(hWnd);
}

STDMETHODIMP CSkinEngine::XSkinEngine::IsSkined(HWND hWnd, VARIANT_BOOL* pBool)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->IsSkined(hWnd, pBool);
}

STDMETHODIMP CSkinEngine::XSkinEngine::GetSkinPath(BSTR* pBstrSkinPath)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->GetSkinPath(pBstrSkinPath);
}

STDMETHODIMP CSkinEngine::XSkinEngine::SetSkinPath(BSTR bstrSkinPack)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->SetSkinPath(bstrSkinPack);
}

STDMETHODIMP CSkinEngine::XSkinEngine::LoadSkinPack(BSTR bstrSkinPath)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->LoadSkinPack(bstrSkinPath);
}

STDMETHODIMP CSkinEngine::XSkinEngine::SetModuleState(LONG lpModuleState)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->SetModuleState(lpModuleState);
}

STDMETHODIMP CSkinEngine::XSkinEngine::GetSubSkinProp(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT* pVarValue)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->GetSubSkinProp(skinWindowHwnd, bstrSubSkinName, bstrPropName, pVarValue);
}

STDMETHODIMP CSkinEngine::XSkinEngine::SetSubSkinProp(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT varValue)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->SetSubSkinProp(skinWindowHwnd, bstrSubSkinName, bstrPropName, varValue);
}

STDMETHODIMP CSkinEngine::XSkinEngine::CallSubSkinMethod(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->CallSubSkinMethod( skinWindowHwnd, bstrSubSkinName, bstrFuncName, pVarResult, pDispParams);
}

STDMETHODIMP CSkinEngine::XSkinEngine::GetSubSkinPos(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG* pLeft, LONG* pTop, LONG* pRight, LONG* pBottom)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->GetSubSkinPos(skinWindowHwnd, bstrSubSkinName, pLeft, pTop, pRight, pBottom);
}

STDMETHODIMP CSkinEngine::XSkinEngine::InvalidateSubSkinRect(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG left, LONG top, LONG width, LONG height, LONG bErase)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->InvalidateSubSkinRect(skinWindowHwnd, bstrSubSkinName, left, top, width, height, bErase);
}

STDMETHODIMP CSkinEngine::XSkinEngine::GetDefaultImageTransformParam(UINT* pTransformType, LONG* pWidth, LONG* pHeight,
																	 LONG* pHue, LONG* pSat, FLOAT* pBlend,
																	 LONG* pR, LONG* pG, LONG* pB)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->GetDefaultImageTransformParam(pTransformType, pWidth, pHeight,
		pHue, pSat, pBlend,
		pR, pG, pB);
}

STDMETHODIMP CSkinEngine::XSkinEngine::SetDefaultImageTransformParam(UINT uTransformType, LONG width, LONG height,
																	 LONG hue, LONG sat, FLOAT blend,
																	 LONG r, LONG g, LONG b)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->SetDefaultImageTransformParam(uTransformType, width, height,
												hue, sat, blend,
												r, g, b);
}

STDMETHODIMP CSkinEngine::XSkinEngine::GetTransformedColor(LONG color, LONG* pTransformedColor, UINT uTransformType, LONG hue, LONG sat, FLOAT blend, LONG r, LONG g, LONG b)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->GetTransformedColor(color, pTransformedColor, uTransformType, hue, sat, blend, r, g, b);
}

STDMETHODIMP CSkinEngine::XSkinEngine::TransformColor(LONG color, LONG* pTransformedColor)
{
    METHOD_PROLOGUE(CSkinEngine, SkinEngine);

    return pThis->TransformColor(color, pTransformedColor);
}

STDMETHODIMP CSkinEngine::XSkinEngine::PreTranslateMessage(HWND hWnd, UINT uMsg, LONG wParam, LONG lParam, DWORD time, LONG x, LONG y, LONG bFilterIMM)
{
	METHOD_PROLOGUE(CSkinEngine, SkinEngine);

	return pThis->PreTranslateMessage(hWnd, uMsg, wParam, lParam, time, x, y, bFilterIMM);
}

STDMETHODIMP CSkinEngine::XSkinEngine::CreateJSEngine(IUnknown** ppJSEngine)
{
    METHOD_PROLOGUE(CSkinEngine, SkinEngine);

    return pThis->CreateJSEngine(ppJSEngine);
}

STDMETHODIMP CSkinEngine::XSkinEngine::SetGlobalMessageReceiver(HWND hWndReceiver)
{
    METHOD_PROLOGUE(CSkinEngine, SkinEngine);

    return pThis->SetGlobalMessageReceiver(hWndReceiver);
}

HRESULT CSkinEngine::LoadSkin(HWND hWnd, BSTR bstrSkinConfig)
{
	CString strSkinConfig(bstrSkinConfig);
	if (LoadSkin(hWnd, strSkinConfig))
		return S_OK;
	else 
		return E_FAIL;
}

HRESULT CSkinEngine::UnloadSkin(HWND hWnd)
{
	map<HWND, CSkinItemDialog*>::iterator finded = m_mapSkinItemDialog.find(hWnd);
	if (finded == m_mapSkinItemDialog.end())
		return E_FAIL;

	//删除
	CSkinItemDialog* pWindow = finded->second;
	if ( ! IsSkinDestroyed(pWindow) ){
		DestroySkin(pWindow);
	}

	//清除映射表中的项
	m_mapSkinItemDialog.erase(hWnd);

	//引擎已经无皮肤窗口，则自行销毁
	if (m_mapSkinItemDialog.size() == 0)
		InternalRelease();

	return S_OK;
}

HRESULT CSkinEngine::IsSkined(HWND hWnd, VARIANT_BOOL* pBool) const
{
	if (IsBadWritePtr(pBool, sizeof(VARIANT_BOOL)) != 0)
		return E_POINTER;

	*pBool = VARIANT_FALSE;
	map<HWND, CSkinItemDialog*>::const_iterator finded = m_mapSkinItemDialog.find(hWnd);
	if (finded != m_mapSkinItemDialog.end())
		*pBool = VARIANT_TRUE;

	return S_OK;
}

HRESULT CSkinEngine::GetSkinPath(BSTR* pBstrSkinPath) const
{
	if (IsBadWritePtr(pBstrSkinPath, sizeof(BSTR)) != 0)
		return E_POINTER;

	*pBstrSkinPath = m_strSkinEnginePath.AllocSysString();

	return S_OK;
}

HRESULT CSkinEngine::SetSkinPath(BSTR bstrSkinPath)
{
	m_strSkinEnginePath = bstrSkinPath;

	CString packPathName = m_strSkinEnginePath + _T("\\default.skn");

#ifdef _DEBUG
	//制作Skin Pack
	CSkinPackEncoder pack;
	pack.CreatePack(m_strSkinEnginePath);
	pack.SavePack(packPathName);
#else
	//加载Skin Pack
	m_pFileRepository->LoadFromDBFile(packPathName);
	//加载Skin Pack中图片
	vector<CString> vecFileList;
	m_pFileRepository->GetFileList(vecFileList);
	CFileData fileData;
	for (size_t i=0; i<vecFileList.size(); i++) {
		m_pFileRepository->GetFile(vecFileList[i], fileData);
		m_pImageRepository->LoadImage(vecFileList[i], fileData, fileData.GetLength());
	}
#endif

    if (! LoadSkinClass(_T("class.xml"))) {
        TRACE("加载skinClass配置文件失败！");
    };

	return S_OK;
}

HRESULT CSkinEngine::LoadSkinPack(BSTR bstrSkinPack)
{
	CString strPackFile = m_strSkinEnginePath + _T("\\") + bstrSkinPack;

	if (m_pFileRepository->LoadFromDBFile(strPackFile)) {
		vector<CString> vecFileList;
		m_pFileRepository->GetFileList(vecFileList);
		CFileData fileData;
		for (size_t i=0; i<vecFileList.size(); i++) {
			m_pFileRepository->GetFile(vecFileList[i], fileData);
			m_pImageRepository->LoadImage(vecFileList[i], fileData, fileData.GetLength());
		}
		return S_OK;
	} else {
		return S_FALSE;
	}
}

HRESULT CSkinEngine::SetModuleState(LONG lpModuleState)
{
	//m_pSkinModuleState = (AFX_MODULE_STATE*)lpModuleState;
	return S_OK;
}

HRESULT CSkinEngine::GetSubSkinProp(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT* pVarValue)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(skinWindowHwnd);
	if (pItem == NULL ||
		pItem->GetEngine() != this ||
		!pItem->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)))
	{
		return S_FALSE;
	}
	if (IsBadWritePtr(pVarValue, sizeof(VARIANT)) != 0)
		return E_POINTER;

	CSkinItemDialog* pWindow = (CSkinItemDialog*)pItem;
    if (bstrSubSkinName == NULL) {
        ISkinJSEngine* pJSEngine = GetSkinJSEngine(pWindow);
        if (pJSEngine != NULL) {
            return pJSEngine->GetProperty(bstrPropName, pVarValue);
        }
        return S_FALSE;
    }
	return pWindow->GetSubSkinProp(bstrSubSkinName, bstrPropName, pVarValue);
}

HRESULT CSkinEngine::SetSubSkinProp(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT varValue)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(skinWindowHwnd);
	if (pItem == NULL ||
		pItem->GetEngine() != this ||
		!pItem->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)))
	{
		return S_FALSE;
	}

	CSkinItemDialog* pWindow = (CSkinItemDialog*)pItem;
	return pWindow->SetSubSkinProp(bstrSubSkinName, bstrPropName, varValue);
}

HRESULT CSkinEngine::CallSubSkinMethod(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(skinWindowHwnd);
	if (pItem == NULL ||
		pItem->GetEngine() != this ||
		!pItem->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)))
	{
		return S_FALSE;
	}

	CSkinItemDialog* pWindow = (CSkinItemDialog*)pItem;
    if (bstrSubSkinName == NULL) {
        ISkinJSEngine* pJSEngine = GetSkinJSEngine(pWindow);
        if (pJSEngine != NULL) {
            return pJSEngine->CallFunction(bstrFuncName, pVarResult, pDispParams);
        }
        return S_FALSE;
    }
	return pWindow->CallSubSkinMethod(bstrSubSkinName, bstrFuncName, pVarResult, pDispParams);
}

HRESULT CSkinEngine::GetSubSkinPos(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG* pLeft, LONG* pTop, LONG* pRight, LONG* pBottom)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(skinWindowHwnd);
	if (pItem == NULL ||
		pItem->GetEngine() != this ||
		!pItem->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)))
	{
		return S_FALSE;
	}

	if (IsBadWritePtr(pLeft, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pTop, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pRight, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pBottom, sizeof(LONG)) != 0 )
		return E_POINTER;

	CSkinItemDialog* pWindow = (CSkinItemDialog*)pItem;
	return pWindow->GetSubSkinPos(bstrSubSkinName, pLeft, pTop, pRight, pBottom);
}

HRESULT CSkinEngine::InvalidateSubSkinRect(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG left, LONG top, LONG width, LONG height, LONG bErase)
{
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(skinWindowHwnd);
	if (pItem == NULL ||
		pItem->GetEngine() != this ||
		!pItem->IsKindOf(RUNTIME_CLASS(CSkinItemDialog)))
	{
		return S_FALSE;
	}
	CRect invalidRect(left, top, left+width, top+height);
	pItem->InvalidateRect(&invalidRect, bErase);
	return S_OK;
}

HRESULT CSkinEngine::GetDefaultImageTransformParam(UINT* pTransformType, LONG* pWidth, LONG* pHeight,
																	 LONG* pHue, LONG* pSat, FLOAT* pBlend,
																	 LONG* pR, LONG* pG, LONG* pB)
{
	if (IsBadWritePtr(pTransformType, sizeof(UINT)) != 0 ||
		IsBadWritePtr(pWidth, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pHeight, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pHue, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pSat, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pBlend, sizeof(FLOAT)) != 0 ||
		IsBadWritePtr(pR, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pG, sizeof(LONG)) != 0 ||
		IsBadWritePtr(pB, sizeof(LONG)) != 0
		)
		return E_POINTER;

	*pTransformType = CImageTransformParam::s_globalParam.enTransformType;
	*pWidth = CImageTransformParam::s_globalParam.width;
	*pHeight = CImageTransformParam::s_globalParam.height;
	*pHue = CImageTransformParam::s_globalParam.hue;
	*pSat = CImageTransformParam::s_globalParam.sat;
	*pBlend = CImageTransformParam::s_globalParam.blend;
	*pR = CImageTransformParam::s_globalParam.r;
	*pG = CImageTransformParam::s_globalParam.g;
	*pB = CImageTransformParam::s_globalParam.b;

	return S_OK;
}

HRESULT CSkinEngine::SetDefaultImageTransformParam(UINT uTransformType, LONG width, LONG height,
																	 LONG hue, LONG sat, FLOAT blend,
																	 LONG r, LONG g, LONG b)
{
	CImageTransformParam param;
	switch(uTransformType)
	{
	case TRANSFORM_TYPE_NONE:
		param.enTransformType = TRANSFORM_TYPE_NONE;
		break;
	case TRANSFORM_TYPE_SMOOTH:
		param.enTransformType = TRANSFORM_TYPE_SMOOTH;
		param.width = width;
		param.height = height;
		break;
	case TRANSFORM_TYPE_COLORIZE:
		param.enTransformType = TRANSFORM_TYPE_COLORIZE;
		param.hue = (BYTE)hue;
		param.sat = (BYTE)sat;
		param.blend = blend;
		break;
	case TRANSFORM_TYPE_SHIFTRGB:
		param.enTransformType = TRANSFORM_TYPE_SHIFTRGB;
		param.r = r;
		param.g = g;
		param.b = b;
		break;
	default:
		return S_FALSE;
	}

	CImageTransformParam::s_globalParam = param;

	SendBroadcastEngineMessage(WM_SKINENGINE_CHANGECOLOR, 0, 0);

	return S_OK;
}

HRESULT CSkinEngine::GetTransformedColor(LONG color, LONG* pTransformedColor, UINT uTransformType, LONG hue, LONG sat, FLOAT blend, LONG r, LONG g, LONG b)
{
	if (IsBadWritePtr(pTransformedColor, sizeof(LONG)) != 0 )
		return E_POINTER;

	CImageTransformParam param;
	switch(uTransformType)
	{
	case TRANSFORM_TYPE_NONE:
		param.enTransformType = TRANSFORM_TYPE_NONE;
		break;
	case TRANSFORM_TYPE_SMOOTH:
		param.enTransformType = TRANSFORM_TYPE_SMOOTH;
		param.width = 0;
		param.height = 0;
		break;
	case TRANSFORM_TYPE_COLORIZE:
		param.enTransformType = TRANSFORM_TYPE_COLORIZE;
		param.hue = (BYTE)hue;
		param.sat = (BYTE)sat;
		param.blend = blend;
		break;
	case TRANSFORM_TYPE_SHIFTRGB:
		param.enTransformType = TRANSFORM_TYPE_SHIFTRGB;
		param.r = r;
		param.g = g;
		param.b = b;
		break;
	default:
		return S_FALSE;
	}
	
	return CImageRepository::TransformColor(&param, color, color) ? S_OK : S_FALSE;
}

HRESULT CSkinEngine::TransformColor(LONG color, LONG* pTransformedColor)
{
    return CImageRepository::TransformColor(GetDefaultImageParam(), color, *pTransformedColor) ? S_OK : S_FALSE;
}

BOOL CSkinEngine::TranslateMessageHelper(HWND hWnd, UINT uMsg, LONG wParam, LONG lParam, DWORD time, LONG x, LONG y, LONG bFilterIMM)
{
	if (uMsg == WM_KEYDOWN) {

		switch (wParam) 
		{
		case VK_TAB:
			{
                //判断是否有菜单弹出，有则调用默认的处理
                HWND hMenuOwner = NULL;
                CSkinItem::GetGUIThreadHwndHelper(hWnd, CSkinItem::GUI_THREAD_HWND_MENUOWNER, hMenuOwner);
                if (hMenuOwner != NULL)
                    break;

                //控件是否接收TAB键
                if (::SendMessage(hWnd, WM_GETDLGCODE, 0, 0) & DLGC_WANTTAB)
                    break;

				CSkinItem *pItem = CSkinItem::FromHandlePermanent(hWnd);
				if (pItem == NULL)
					break;

				if (pItem->PreTranslateMessage(WM_KEYDOWN, wParam, lParam))
					return TRUE;
			}
			break;
        case VK_UP:
        case VK_DOWN:
            {
                //判断是否有菜单弹出，有则调用默认的处理
                HWND hMenuOwner = NULL;
                CSkinItem::GetGUIThreadHwndHelper(hWnd, CSkinItem::GUI_THREAD_HWND_MENUOWNER, hMenuOwner);
                if (hMenuOwner != NULL)
                    break;

                //控件是否接收方向键
                if (::SendMessage(hWnd, WM_GETDLGCODE, 0, 0) & DLGC_WANTARROWS)
                    break;

                CSkinItem *pItem = CSkinItem::FromHandlePermanent(hWnd);
                if (pItem == NULL)
                    break;

                if (pItem->PreTranslateMessage(WM_KEYDOWN, wParam, lParam))
                    return TRUE;
            }
            break;
		case VK_RETURN:
			{
                //控件是否接收
                if (::SendMessage(hWnd, WM_GETDLGCODE, 0, 0) & DLGC_WANTALLKEYS)
                    break;

                //判断是否有菜单弹出，有则调用默认的处理
                HWND hMenuOwner = NULL;
                CSkinItem::GetGUIThreadHwndHelper(hWnd, CSkinItem::GUI_THREAD_HWND_MENUOWNER, hMenuOwner);
                if (hMenuOwner != NULL)
                    break;

                CSkinItem *pItem = CSkinItem::FromHandlePermanent(hWnd);
				if (pItem == NULL)
					break;

				if (bFilterIMM) {
				    //获得IME状态，不截获IME的VK_RETURN
				    HIMC hImc = ImmGetContext(hWnd); 
				    if(ImmGetOpenStatus(hImc))
					    break;
				}

				if (pItem->PreTranslateMessage(WM_KEYDOWN, wParam, lParam))
					return TRUE;
			}
			break;
		case VK_CANCEL:
		case VK_PAUSE:
			{
				if ( !(GetKeyState(VK_CONTROL) & 0x8000) )
					break;

				HWND hRoot = ::GetAncestor(hWnd, GA_ROOT);
				CSkinItem *pItem = CSkinItem::FromHandlePermanent(hRoot);
				if (pItem == NULL)
					break;

				if (! pItem->GetrecieveCtrlBreak())
					return TRUE;
			}
            break;
		}

    }

	return FALSE;
}

HRESULT CSkinEngine::PreTranslateMessage(HWND hWnd, UINT uMsg, LONG wParam, LONG lParam, DWORD time, LONG x, LONG y, LONG bFilterIMM)
{
	BOOL bTranslated = TranslateMessageHelper(hWnd, uMsg, wParam, lParam, time, x, y, bFilterIMM);
	return (bTranslated) ? (S_OK) : (S_FALSE);
}

HRESULT CSkinEngine::CreateJSEngine(IUnknown** ppJSEngine)
{
    if (IsBadWritePtr(ppJSEngine, sizeof(IJSEngine*)))
        return E_POINTER;

    *ppJSEngine = NULL;

#ifdef USE_V8_JS_ENGINE
    CJSEngineV8* pEngine = new CJSEngineV8;
#else
    CJSEngine* pEngine = new CJSEngine;
#endif

    *ppJSEngine = pEngine->GetInterface(&IID_IUnknown);
    return S_OK;
}

HRESULT CSkinEngine::SetGlobalMessageReceiver(HWND hWndReceiver)
{
    if (SetGlobalMessageReceiverHwnd(hWndReceiver))
        return S_OK;

    return S_FALSE;
}

CString CSkinEngine::GetSkinPath() const
{
	return m_strSkinEnginePath + _T("\\");
}

const SKINMAP_ENTRY CSkinEngine::s_skinMap[] = {
#ifdef _DEBUG
#define SKINMAP_ITEM(className) \
    {_T(#className), &className::CreateObject, sizeof(className)},
    SKIN_TYPE_LIST(SKINMAP_ITEM)
#undef SKINMAP_ITEM
    {NULL, NULL, 0},
#else
#define SKINMAP_ITEM(className) \
    {_T(#className), &className::CreateObject},
    SKIN_TYPE_LIST(SKINMAP_ITEM)
#undef SKINMAP_ITEM
    {NULL, NULL},
#endif //_DEBUG
};

const SKINNAMETYPEMAP_ENTRY CSkinEngine::s_skinNameTypeMap[] = {
#define SKINNAMETYPEMAP_ITEM(name, type) \
    {_T(#name), _T(#type)},
    SKIN_NAME_TYPE_MAP(SKINNAMETYPEMAP_ITEM)
    {NULL, NULL},
#undef SKINNAMETYPEMAP_ITEM
};

#ifdef _DEBUG
int CSkinEngine::s_iMaxSkinMem = 0;
#endif

const SKINMAP_ENTRY* CSkinEngine::GetSkinMapEntry(LPCTSTR className)
{
    CString strClassName = className;
    const SKINMAP_ENTRY* pMapEntry = s_skinMap;
    for (; pMapEntry->lpszClassName != NULL; pMapEntry++) {
        if (strClassName.CompareNoCase(pMapEntry->lpszClassName) == 0) {
            return pMapEntry;
        }
    }
    return NULL;
}

LPCTSTR CSkinEngine::GetClassName(LPCTSTR xmlName)
{
    CString strXmlName = xmlName;
    const SKINNAMETYPEMAP_ENTRY* pEntry = s_skinNameTypeMap;
    for (; pEntry->lpszXmlName!=NULL; pEntry++) {
        if (strXmlName.CompareNoCase(pEntry->lpszXmlName) == 0) {
            return pEntry->lpszClassName;
        }
    }
    return NULL;
}

CSkinBase* CSkinEngine::CreateSkinByClassName(LPCTSTR className)
{
#ifdef MULTI_THREAD_SUPPORT
    CLockMgr<CCSWrapper> lock(m_cs, TRUE);
#endif

    const SKINMAP_ENTRY* pMapEntry = GetSkinMapEntry(className);
    if (pMapEntry == NULL)
        return NULL;

#ifdef _DEBUG
    s_iMaxSkinMem += pMapEntry->sizeOfClass;
#endif
    //创建
    CSkinBase* pSkin = (CSkinBase*)(*pMapEntry->pfnSkinCreator)();

    //记录
    set<CSkinBase*>::iterator finded = m_setAllSkin.find(pSkin);
    ASSERT(finded == m_setAllSkin.end());
    m_setAllSkin.insert(pSkin);

    //绑定皮肤引擎
    const_cast<CSkinEngine*>(pSkin->m_pEngine) = this;

    //初始化，依赖皮肤引擎，需在绑定皮肤引擎之后进行
    pSkin->Initialize();

    return pSkin;
}

CSkinBase* CSkinEngine::CreateSkinByXmlName(LPCTSTR xmlName)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_cs, TRUE);
#endif

    LPCTSTR className = GetClassName(xmlName);

    return CreateSkinByClassName(className);
}

BOOL CSkinEngine::DestroySkin(CSkinBase* pSkin)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_cs, TRUE);
#endif

	set<CSkinBase*>::iterator finded = m_setAllSkin.find(pSkin);
	ASSERT(finded != m_setAllSkin.end());
	if (finded == m_setAllSkin.end())
		return FALSE;

    CloseSkinJSEngine(pSkin);

	//释放皮肤
	DWORD ref = pSkin->ReleaseSkin();

	//删除记录
	m_setAllSkin.erase(pSkin);

	return TRUE;
}

BOOL CSkinEngine::IsSkinDestroyed(CSkinBase* pSkin)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_cs, TRUE);
#endif

	set<CSkinBase*>::iterator iter = m_setAllSkin.find(pSkin);
	return (iter == m_setAllSkin.end());
}

void CSkinEngine::DestroyAllSkin()
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_cs, TRUE);
#endif

	//调试
	TRACE(("DestroyAllSkin Start! *****************\n"));

	set<CSkinBase*> tempSet = m_setAllSkin;
	set<CSkinBase*>::iterator iter = tempSet.begin(), last = tempSet.end();
	for (; iter!=last; iter++) {
		CSkinBase* pSkin = (*iter);

		if (! IsSkinDestroyed(pSkin)) {
			DestroySkin(pSkin);
		}
	}

	//调试
	TRACE(("DestroyAllSkin end! Skin remain %d, JSEngine remain %d *****************\n"), m_setAllSkin.size(), m_mapSkinJSEngine.size());

	//解除剩余皮肤元素的绑定
	set<CSkinBase*>::iterator iter2 = m_setAllSkin.begin(), last2 = m_setAllSkin.end();
	for (; iter2!=last2; iter2++) {
		CSkinBase* pBase = (*iter2);
		const_cast<CSkinEngine*>(pBase->m_pEngine) = NULL;	//解除绑定
	}
}

CSkinClass* CSkinEngine::GetSkinClass(LPCTSTR className, LPCTSTR skinClassName)
{
    CString strClassName = className;
    strClassName.MakeLower();
    map<CString, map<CString, CSkinClass*>>::iterator finded = m_mapSkinClass.find(strClassName);
    if (finded == m_mapSkinClass.end())
        return NULL;

    CString strSkinClassName = skinClassName;
    strSkinClassName.MakeLower();
    map<CString, CSkinClass*>& mapSkinClass = finded->second;
    map<CString, CSkinClass*>::iterator finded2 = mapSkinClass.find(strSkinClassName);
    if (finded2 == mapSkinClass.end())
        return NULL;

    return finded2->second;
}

BOOL CSkinEngine::LoadSkinClass(LPCTSTR skinClassFileName)
{
    CFileData fileDate;
    CString strskinClassFilePathName = GetSkinPath() + skinClassFileName;
    CFileData fileData;
    if (! GetFile(strskinClassFilePathName, fileData))
        return FALSE;

    IXMLDOMElementPtr ptrRoot;
    if (! CSkinBase::GetXmlRoot(fileData, TRUE, &ptrRoot))
        return FALSE;

    IXMLDOMNodePtr childNode;
    IXMLDOMNodePtr tempNode;
    ptrRoot->get_firstChild(&childNode);
    for (; childNode!=NULL; tempNode=childNode,tempNode->get_nextSibling(&childNode)) {
        //验证节点属性
        DOMNodeType type = NODE_INVALID;
        if (S_OK != childNode->get_nodeType(&type))
            continue;
        if (type != NODE_ELEMENT)
            continue;

        //验证节点名是否为合法的类名
        CComBSTR bstrNodeName;
        if (S_OK != childNode->get_nodeName(&bstrNodeName))
            continue;
        CString strNodeName = bstrNodeName;
        CString strNodeClass = GetClassName(strNodeName);
        if (strNodeClass.IsEmpty())
            continue;

        //加载属性
        CSkinClass* pClass = new CSkinClass;
        if (! pClass->LoadPropertyFromXml(childNode)) {
            delete pClass;
            ASSERT(_T("skinClass节点解析错误！"));
            continue;
        }

        //加入SkinClass表
        if (! AddSkinClass(strNodeClass, pClass->GetClassName(), pClass)) {
            delete pClass;
            ASSERT(_T("skinClass节点添加错误"));
            continue;
        }
    }

    return TRUE;
}

BOOL CSkinEngine::AddSkinClass(LPCTSTR className, LPCTSTR skinClassName, CSkinClass* pSkinClass)
{
    ASSERT(pSkinClass != NULL);
    if (pSkinClass == NULL)
        return FALSE;

    CString strClassName = className;
    strClassName.MakeLower();
    if (strClassName.IsEmpty())
        return FALSE;

    map<CString, map<CString, CSkinClass*>>::iterator finded = m_mapSkinClass.find(strClassName);
    if (finded == m_mapSkinClass.end()) {
        pair<map<CString, map<CString, CSkinClass*>>::iterator, bool> pairib = 
            m_mapSkinClass.insert(pair<CString, map<CString, CSkinClass*>>(strClassName, map<CString, CSkinClass*>()));
        finded = pairib.first;
    }

    CString strSkinClassName = skinClassName;
    strSkinClassName.MakeLower();
    if (strSkinClassName.IsEmpty())
        return FALSE;

    map<CString, CSkinClass*>& mapSkinClass = finded->second;
    map<CString, CSkinClass*>::iterator finded2 = mapSkinClass.find(strSkinClassName);
    if (finded2 != mapSkinClass.end()) {
        delete finded2->second;
    }

    mapSkinClass[strSkinClassName] = pSkinClass;
    return TRUE;
}

void CSkinEngine::RemoveAllSkinClass()
{
    map<CString, map<CString, CSkinClass*>>::iterator iter = m_mapSkinClass.begin(),
        last = m_mapSkinClass.end();
    for (; iter!=last; iter++) {
        map<CString, CSkinClass*>& mapSkinClass = iter->second;
        map<CString, CSkinClass*>::iterator iter2 = mapSkinClass.begin(),
            last2 = mapSkinClass.end();
        for (; iter2!=last2; iter2++) {
            delete iter2->second;
        }
    }

    m_mapSkinClass.clear();
}

BOOL CSkinEngine::GetFile(LPCTSTR filePathName, CFileData& fileData)
{
	ASSERT(m_pFileRepository != NULL);
	if (m_pFileRepository == NULL)
		return NULL;

#ifdef _DEBUG
	//不缓存文件
	return CFileRepository::LoadFileData(filePathName, fileData);
#else
	//缓存文件
	m_pFileRepository->LoadFile(filePathName);
	return m_pFileRepository->GetFile(filePathName, fileData);
#endif

}

CImageBase* CSkinEngine::LoadImage(LPCTSTR filePathName)
{
	ASSERT(m_pImageRepository != NULL);
	if (m_pImageRepository == NULL)
		return NULL;

	CFileData fileData;
	if (! GetFile(filePathName, fileData))
		return NULL;

	return m_pImageRepository->LoadImage(filePathName, fileData, fileData.GetLength());
}

CImageBase* CSkinEngine::LoadImageInSkinPath(LPCTSTR fileName)
{
	CString filePathName = m_strSkinEnginePath + _T("\\") + fileName;
	return LoadImage(filePathName);
}

//////////////////////////////////////////////////////////////////////////
//Script
//////////////////////////////////////////////////////////////////////////
BOOL CSkinEngine::CreateSkinJSEngine(CSkinBase* pBase)
{
    map<CSkinBase*, ISkinJSEngine*>::const_iterator finded = m_mapSkinJSEngine.find(pBase);
    if (finded != m_mapSkinJSEngine.end())
        return FALSE;

    CString strScriptFileName = GetSkinPath() + pBase->GetScriptFileName();
    CFileData fileData;
    if (! GetFile(strScriptFileName, fileData))
        return FALSE;

#ifdef USE_V8_JS_ENGINE
    ISkinJSEngine* pEngine = new CSkinJSEngineV8();
#else
#ifdef USE_MS_JS_ENGINE
    ISkinJSEngine* pEngine = new CSkinJSEngine();
#endif //USE_MS_JS_ENGINE
#endif //USE_V8_JS_ENGINE

    if (! pEngine->Create(pBase)) {
        pEngine->Destroy();
        return FALSE;
    }

    CString strPreLoad = pBase->GetSkinPreLoad();
    if (! strPreLoad.IsEmpty()) {
        CString strPreLoadFileName = strPreLoad + _T(".js");
        CString strPreLoadFilePathName = GetSkinPath() + strPreLoadFileName;
        CFileData preLoadData;
        if (! GetFile(strPreLoadFilePathName, preLoadData)) {
            pEngine->Destroy();
            return FALSE;
        }
        if (! pEngine->Load(preLoadData, strPreLoadFileName)) {
            pEngine->Destroy();
            return FALSE;
        }
    }

    if (! pEngine->Load(fileData, pBase->GetScriptFileName())) {
        pEngine->Destroy();
        return FALSE;
    }

    if (! pEngine->InitGlobalNamespace()) {
        pEngine->Destroy();
        return FALSE;
    }

    if (! pEngine->Start()) {
        pEngine->Destroy();
        return FALSE;
    }

    m_mapSkinJSEngine.insert(pair<CSkinBase*, ISkinJSEngine*>(pBase, pEngine));

    return TRUE;
}

BOOL CSkinEngine::CloseSkinJSEngine(CSkinBase* pBase)
{
    map<CSkinBase*, ISkinJSEngine*>::const_iterator finded = m_mapSkinJSEngine.find(pBase);
    if (finded == m_mapSkinJSEngine.end())
        return FALSE;

    ISkinJSEngine* pEngine = finded->second;
    pEngine->Close();
    pEngine->Destroy();

    m_mapSkinJSEngine.erase(pBase);

    return TRUE;
}

ISkinJSEngine* CSkinEngine::GetSkinJSEngine(CSkinBase* pBase)
{
    map<CSkinBase*, ISkinJSEngine*>::const_iterator finded = m_mapSkinJSEngine.find(pBase);
    if (finded == m_mapSkinJSEngine.end())
        return NULL;

    return finded->second;
}

BOOL CSkinEngine::LoadSkin(HWND hWnd, LPCTSTR configFile)
{
	ASSERT(::IsWindow(hWnd));
	if (!::IsWindow(hWnd))
		return FALSE;

	CString strConfigFile = configFile;
    if (strConfigFile.CompareNoCase(_T("class")) == 0) {
        ASSERT(0 && "不能以 class 为配置文件名！");
        return FALSE;
    }

	//判断句柄的窗口是否已加载皮肤
	map<HWND, CSkinItemDialog*>::iterator finded = m_mapSkinItemDialog.find(hWnd);
	if (finded != m_mapSkinItemDialog.end()){
		TRACE(("已经加载皮肤"));
		return FALSE;
	}

    //查找缓存中是否有预先加载的
	CSkinItemDialog* pBuffer = GetBufferSkin(configFile);
	if (pBuffer != NULL) {

		//初始化
		if (! pBuffer->AttachHwnd(hWnd)) {
			USES_CONVERSION;
			TRACE(("%s :皮肤窗口初始化失败"), T2A(pBuffer->GetXmlFileName()));
			return FALSE;
		}

		if (! pBuffer->LoadPropFromXml()) {
			USES_CONVERSION;
			TRACE(("%s :加载属性错误"), T2A(pBuffer->GetXmlFileName()));
			return FALSE;
		}

		m_mapSkinItemDialog.insert(pair<HWND, CSkinItemDialog*>(hWnd, pBuffer));

		ResetBufferSkin(configFile);
		return TRUE;
	}

	//创建窗口皮肤
	CSkinItemDialog* pWindow = (CSkinItemDialog*)CreateSkinByClassName(_T("CSkinItemDialog"));
	if (pWindow == NULL){
		TRACE(("皮肤窗口创建失败"));
		return FALSE;
	}

	pWindow->SetConfigFileName(configFile);

    //解析XML配置文件
    if (! pWindow->CreateFromXml()) {
        USES_CONVERSION;
        TRACE(("%s :XML解析错误"), T2A(pWindow->GetXmlFileName()));
        return FALSE;
    }

	//初始化
	if (! pWindow->AttachHwnd(hWnd)) {
        USES_CONVERSION;
        TRACE(("皮肤窗口初始化失败"));
        return FALSE;
	}

	//加载Xml属性
    if (! pWindow->LoadPropFromXml()) {
	    USES_CONVERSION;
	    TRACE(("%s :加载属性错误"), T2A(pWindow->GetXmlFileName()));
	    return FALSE;
    }

    //创建脚本引擎
    if (! CreateSkinJSEngine(pWindow)) {
        USES_CONVERSION;
        TRACE(("%s :脚本解析启动错误"), T2A(pWindow->GetScriptFileName()));
        return FALSE;
    }

    m_mapSkinItemDialog.insert(pair<HWND, CSkinItemDialog*>(hWnd, pWindow));

	return TRUE;
}

BOOL CSkinEngine::UnLoadSkin(HWND hWnd)
{
	return FALSE;
}

BOOL CSkinEngine::ReLoadSkin(HWND hWnd)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//SkinEngine record
//////////////////////////////////////////////////////////////////////////
CSkinEngine::CSkinEngineSet::CSkinEngineSet()
{

}

CSkinEngine::CSkinEngineSet::~CSkinEngineSet()
{
	set<CSkinEngine*>::iterator iter = begin(), last = end();
	for (; iter!=last; iter++) {
		CSkinEngine* pEngine = (*iter);
		const_cast<bool&>(pEngine->m_bSkinEngineSetLoad) = false;
	}
}

BOOL CSkinEngine::CSkinEngineSet::Add(CSkinEngine* pEngine)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_csSkinEngineSet, TRUE);
#endif

	ASSERT(pEngine != NULL);
	if (pEngine == NULL)
		return FALSE;

	set<CSkinEngine*>::iterator finded = find(pEngine);
	ASSERT(finded == end());
	if (finded == end()){
		insert(pEngine);
		const_cast<bool&>(pEngine->m_bSkinEngineSetLoad) = true;
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CSkinEngine::CSkinEngineSet::Remove(CSkinEngine* pEngine)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_csSkinEngineSet, TRUE);
#endif

	ASSERT(pEngine != NULL);
	if (pEngine == NULL)
		return FALSE;

	set<CSkinEngine*>::iterator finded = find(pEngine);
	ASSERT(finded != end());
	if (finded != end()){
		erase(finded);
		const_cast<bool&>(pEngine->m_bSkinEngineSetLoad) = false;
		return TRUE;
	} else {
		return FALSE;
	}
}

CSkinEngine::CSkinEngineSet CSkinEngine::s_setAllSkinEngine;

//////////////////////////////////////////////////////////////////////////
//SkinEngine Message
//////////////////////////////////////////////////////////////////////////
CSkinEngine::CSkinEngineWndMap::CSkinEngineWndMap()
{

}

CSkinEngine::CSkinEngineWndMap::~CSkinEngineWndMap()
{
	map<HWND, CSkinEngine*>::iterator iter = begin(), last = end();
	for (; iter!=last; iter++) {
		CSkinEngine* pEngine = iter->second;
		const_cast<bool&>(pEngine->m_bSkinEngineWndMapLoad) = false;
	}
}

CSkinEngine* CSkinEngine::CSkinEngineWndMap::Find(HWND hWnd)
{
	map<HWND, CSkinEngine*>::iterator finded = find(hWnd);
	if (finded != end())
		return finded->second;
	else 
		return NULL;
}

BOOL CSkinEngine::CSkinEngineWndMap::AddPair(HWND hWnd, CSkinEngine* pEngine)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_csSkinEngineWndMap, TRUE);
#endif

	ASSERT(hWnd != NULL && pEngine != NULL);
	if (hWnd == NULL || pEngine == NULL)
		return FALSE;

	map<HWND, CSkinEngine*>::iterator finded = find(hWnd);
	ASSERT(finded == end());
	if (finded == end()){
		insert(pair<HWND, CSkinEngine*>(hWnd, pEngine));
		const_cast<bool&>(pEngine->m_bSkinEngineWndMapLoad) = true;
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CSkinEngine::CSkinEngineWndMap::RemovePair(HWND hWnd, CSkinEngine* pEngine)
{
#ifdef MULTI_THREAD_SUPPORT
	CLockMgr<CCSWrapper> lock(m_csSkinEngineWndMap, TRUE);
#endif

	ASSERT(hWnd != NULL && pEngine != NULL);
	if (hWnd == NULL || pEngine == NULL)
		return FALSE;

	map<HWND, CSkinEngine*>::iterator finded = find(hWnd);
	ASSERT(finded != end() && finded->second == pEngine);
	if (finded != end()){
		erase(finded);
		const_cast<bool&>(pEngine->m_bSkinEngineWndMapLoad) = false;
		return TRUE;
	} else {
		return FALSE;
	}
}

LRESULT CSkinEngine::CSkinEngineWndMap::SendBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	map<HWND, CSkinEngine*>::iterator iter = begin(), last = end();
	for (; iter!=last; iter++) {
		CSkinEngine* pEngine = iter->second;
		result = pEngine->SendEngineMessage(uMsg, wParam, lParam);
	}
	return result;
}

BOOL CSkinEngine::CSkinEngineWndMap::PostBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL allSc = TRUE;
	map<HWND, CSkinEngine*>::iterator iter = begin(), last = end();
	for (; iter!=last; iter++) {
		CSkinEngine* pEngine = iter->second;
		if (! pEngine->PostEngineMessage(uMsg, wParam, lParam))
			allSc = FALSE;
	}
	return allSc;
}

CSkinEngine::CSkinEngineWndMap CSkinEngine::s_mapSkinEngineWnd;

LRESULT CSkinEngine::_SkinEngineWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CSkinEngine* pEngine = s_mapSkinEngineWnd.Find(hWnd);
	ASSERT(pEngine != NULL);
	if (pEngine == NULL)
		return 0;

	return pEngine->SkinEngineWndProc(uMsg, wParam, lParam);
}

LRESULT CSkinEngine::SendEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hEngineWnd));
	if (!::IsWindow(m_hEngineWnd))
		return 0;

	return ::SendMessage(m_hEngineWnd, uMsg, wParam, lParam);
}

BOOL CSkinEngine::PostEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hEngineWnd));
	if (!::IsWindow(m_hEngineWnd))
		return 0;

	return ::PostMessage(m_hEngineWnd, uMsg, wParam, lParam);
}

LRESULT CSkinEngine::SendBroadcastEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return s_mapSkinEngineWnd.SendBroadcast(uMsg, wParam, lParam);
}

BOOL CSkinEngine::PostBroadcastEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return s_mapSkinEngineWnd.PostBroadcast(uMsg, wParam, lParam);
}

LRESULT CSkinEngine::SkinEngineWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
		if (wParam == SKINENGINE_IDLE_TIMER) {
			MaintainSkinBuffer();
			break;
		}
		OnTimer(uMsg, wParam, lParam);
		break;
	case WM_SKINENGINE_CHANGECOLOR:
		OnChangeColor(uMsg, wParam, lParam);
        break;

#ifdef USE_GLOBAL_MOUSE_HOOKER
    case WM_MOUSE_HOOK:
        OnGlobalMouseHook(uMsg, wParam, lParam);
        break;
#endif //USE_GLOBAL_MOUSE_HOOKER

#ifdef USE_GLOBAL_KEYBOARD_HOOKER
    case WM_KEYBORAD_HOOK:
        OnGlobalKeyboardHook(uMsg, wParam, lParam);
        break;
#endif //USE_GLOBAL_KEYBOARD_HOOKER

	default:
		break;
	}
	return S_OK;
}

BOOL CSkinEngine::AddBuffer(LPCTSTR configName)
{
	CString strConfigName = configName;
	strConfigName.MakeLower();

	map<CString, CSkinItemDialog*>::const_iterator finded = m_mapSkinBuffer.find(strConfigName);
	if (finded != m_mapSkinBuffer.end())
		return FALSE;

	m_mapSkinBuffer.insert(pair<CString, CSkinItemDialog*>(strConfigName, NULL));
	return TRUE;
}

BOOL CSkinEngine::RemoveBuffer(LPCTSTR configName)
{
	CString strConfigName = configName;
	strConfigName.MakeLower();

	map<CString, CSkinItemDialog*>::iterator finded = m_mapSkinBuffer.find(strConfigName);
	if (finded == m_mapSkinBuffer.end())
		return FALSE;

	if (finded->second != NULL) {
		DestroySkin(finded->second);
	}
	m_mapSkinBuffer.erase(finded);
	return TRUE;
}

void CSkinEngine::MaintainSkinBuffer()
{
	map<CString, CSkinItemDialog*>::iterator iter = m_mapSkinBuffer.begin(),
		last = m_mapSkinBuffer.end();
	for (; iter!=last; iter++) {
		if (iter->second != NULL)
			continue;

		CSkinItemDialog* pSkinBuffer = (CSkinItemDialog*)CreateSkinByClassName(_T("CSkinItemDialog"));
		pSkinBuffer->SetConfigFileName(iter->first);
		if (! pSkinBuffer->CreateFromXml()) {
			DestroySkin(pSkinBuffer);
			continue;
		}

        if (! CreateSkinJSEngine(pSkinBuffer)) {
            USES_CONVERSION;
            TRACE(("%s :脚本解析启动错误"), T2A(pSkinBuffer->GetScriptFileName()));
            DestroySkin(pSkinBuffer);
            continue;
        }

		iter->second = pSkinBuffer;
	}
}

CSkinItemDialog* CSkinEngine::GetBufferSkin(LPCTSTR configName)
{
	CString strConfigName = configName;
	strConfigName.MakeLower();

	map<CString, CSkinItemDialog*>::const_iterator finded = m_mapSkinBuffer.find(strConfigName);
	if (finded != m_mapSkinBuffer.end()) {
		return finded->second;
	} else {
		return NULL;
	}
}

BOOL CSkinEngine::ResetBufferSkin(LPCTSTR configName)
{
	CString strConfigName = configName;
	strConfigName.MakeLower();

	map<CString, CSkinItemDialog*>::iterator finded = m_mapSkinBuffer.find(strConfigName);
	if (finded != m_mapSkinBuffer.end()) {
		finded->second = NULL;
		return TRUE;
	} else {
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//Timer Service
//////////////////////////////////////////////////////////////////////////
UINT CSkinEngine::GenerateTimerID()
{
    ::srand((unsigned int)::time(NULL));
    do {
        UINT timerID = (UINT)::rand();
        map<UINT, SkinTimerStruct>::iterator finded = m_mapSkinTimer.find(timerID);
        if (finded == m_mapSkinTimer.end())
            return timerID;
    } while (true);

    ASSERT(0);
    return 0;
}

UINT_PTR CSkinEngine::SetTimer(UINT nIDEvent, UINT nEclapse, CSkinBase* pSkin)
{
	ASSERT(::IsWindow(m_hEngineWnd) && pSkin != NULL);
	if (!::IsWindow(m_hEngineWnd) || pSkin == NULL)
		return NULL;

    SkinTimerStruct timerStruct;
    timerStruct.pSkin = pSkin;
    timerStruct.uIDEvent = nIDEvent;

    map<SkinTimerStruct, UINT>::iterator finded = m_mapSkinTimerReverse.find(timerStruct);
    if (finded != m_mapSkinTimerReverse.end()) {
        UINT_PTR timer = ::SetTimer(m_hEngineWnd, finded->second, nEclapse, NULL);
        ASSERT(timer == finded->second);
        return nIDEvent;
    } else {
        UINT newTimerID = GenerateTimerID();
        UINT_PTR timerID = ::SetTimer(m_hEngineWnd, newTimerID, nEclapse, NULL);
        ASSERT(timerID == newTimerID);
        m_mapSkinTimerReverse.insert(pair<SkinTimerStruct, UINT>(timerStruct, newTimerID));
        m_mapSkinTimer.insert(pair<UINT, SkinTimerStruct>(newTimerID, timerStruct));
        return nIDEvent;
    }
}

BOOL CSkinEngine::KillTimer(UINT nIDEvent, CSkinBase* pSkin)
{
    ASSERT(::IsWindow(m_hEngineWnd) && pSkin != NULL);
    if (!::IsWindow(m_hEngineWnd) || pSkin == NULL)
    	return FALSE;

    SkinTimerStruct timerStruct;
    timerStruct.pSkin = pSkin;
    timerStruct.uIDEvent = nIDEvent;

    map<SkinTimerStruct, UINT>::iterator finded = m_mapSkinTimerReverse.find(timerStruct);
    if (finded == m_mapSkinTimerReverse.end())
        return FALSE;

    BOOL bSuc = ::KillTimer(m_hEngineWnd, finded->second);
    ASSERT(bSuc);
    m_mapSkinTimer.erase(finded->second);
    m_mapSkinTimerReverse.erase(finded);
    return bSuc;
}

BOOL CSkinEngine::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UINT nIDEvent = (UINT)wParam;

    map<UINT, SkinTimerStruct>::iterator finded = m_mapSkinTimer.find(nIDEvent);
    if (finded == m_mapSkinTimer.end())
        return FALSE;

    CSkinBase* pSkin = finded->second.pSkin;
    UINT uSkinTimerID = finded->second.uIDEvent;
    if (IsSkinDestroyed(pSkin)) {
        KillTimer(uSkinTimerID, pSkin);
        return FALSE;
    } else {
        pSkin->OnTimer(uSkinTimerID);
        return TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////
//Default Image Transform Parameter
//////////////////////////////////////////////////////////////////////////
const CImageTransformParam* CSkinEngine::GetDefaultImageParam() const
{
	return &m_defaultImageParam;
}

BOOL CSkinEngine::SetDefaultImageParam(const CImageTransformParam& param)
{
	m_defaultImageParam = param;
	return TRUE;
}

BOOL CSkinEngine::TransformColor(LONG color, LONG& transformColor)
{
	return CImageRepository::TransformColor(&m_defaultImageParam, color, transformColor);
}

//////////////////////////////////////////////////////////////////////////
//Change Color
//////////////////////////////////////////////////////////////////////////
class CSkinItemRedrawVisitor :
	public ICSkinItemVisitor
{
public:
	void Visit(CSkinItem* pItem)
	{
		HWND hWnd = pItem->GetSafeHwnd();
		if (!::IsWindow(hWnd))
			return;

		pItem->InvalidateRect(NULL, FALSE);
		::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
};

BOOL CSkinEngine::OnChangeColor(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CImageTransformParam& param = CImageTransformParam::s_globalParam;

	SetDefaultImageParam(param);

	//刷新所有窗口
	map<HWND, CSkinItemDialog*>::const_iterator iter = m_mapSkinItemDialog.begin(), last = m_mapSkinItemDialog.end();
	for (; iter!=last; iter++) {
		HWND hWnd = iter->first;
		if (! ::IsWindow(hWnd))
			continue;
		CSkinItemDialog* pWindow = iter->second;
		if (IsSkinDestroyed(pWindow))
			continue;
		pWindow->InvalidateRect(NULL, FALSE);
	}

	return S_OK;
}

BOOL CSkinEngine::OnGlobalMouseHook(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_MOUSEMOVE) {

        m_nCtrlCount = 0;

    }  else if (uMsg == WM_LBUTTONDOWN) {

         m_nCtrlCount = 0;

    } else if (uMsg == WM_LBUTTONUP) {

        m_nCtrlCount = 0;

    } else if (uMsg == WM_RBUTTONDOWN) {

        m_nCtrlCount = 0;

    }  else if (uMsg == WM_RBUTTONUP) {

        m_nCtrlCount = 0;

    } 

    return 0;
}

BOOL CSkinEngine::OnGlobalKeyboardHook(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (lParam != VK_CONTROL) {
        m_nCtrlCount = 0;
    }

    if (wParam == WM_KEYUP) {

        if (lParam != VK_CONTROL) {
            m_nCtrlCount = 0;
        }

        switch (lParam) 
        {
        case VK_CONTROL:
            {
                DWORD tm = ::GetTickCount();
                LONG diff = tm - m_tLastCtrlTime;
                m_tLastCtrlTime = tm;
                if (abs(diff) < 1000) {
                    m_nCtrlCount ++;
                } else {
                    m_nCtrlCount = 1;
                }
                if (m_nCtrlCount >= 3) {
                    m_nCtrlCount = 0;
                    ::PostMessage(m_hGlobalMessageReceiver, WM_CTRL_CLICK_3, 0, 0);
                }
                return FALSE;
            }
            break;
        default:
            break;
        }

    }
    return 0;
}

DWORD CSkinEngine::GetComctl32Version()
{
    return m_comctlVerInfo.dwMajorVersion;
}

LONG CSkinEngine::m_nCtrlCount = 0;
DWORD CSkinEngine::m_tLastCtrlTime = 0;
HWND CSkinEngine::m_hGlobalMessageReceiver = NULL;

BOOL CSkinEngine::SetGlobalMessageReceiverHwnd(HWND hWnd)
{
    if (! IsWindow(hWnd))
        return FALSE;

    m_hGlobalMessageReceiver = hWnd;
    return TRUE;
}
