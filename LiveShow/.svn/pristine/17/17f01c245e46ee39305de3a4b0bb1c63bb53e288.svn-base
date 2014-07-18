
#pragma once

// CSkinEngine command target

#include <map>
#include <set>
#include <vector>

#include "Skin_i.h"

#include "FileRepository.h"
#include "ImageRepository.h"
#include "KeyboardHooker.h"
#include "MouseHooker.h"
#include "ImageTransformParam.h"
#include "ModulVer.h"

#define WM_SKINENGINE_CHANGECOLOR (WM_USER+1)
#define SKINENGINE_IDLE_TIMER (65535)

#define SKIN_TYPE_LIST(V) \
    V(CSkinBase) \
    V(CSkinProxy) \
    V(CSkinItemDialog) \
    V(CSkinButton) \
    V(CSkinText) \
    V(CSkinImage) \
    V(CSkinRoundImage) \
    V(CSkinGroup) \
    V(CSkinProgress) \
    V(CSkinSplitter) \
    V(CSkinLibretto) \
    V(CSkinTab) \
    V(CSkinItem) \
    V(CSkinItemCombo) \
    V(CSkinItemList) \
    V(CSkinItemStatic) \
    V(CSkinItemButton) \
    V(CSkinItemEdit) \
    V(CSkinItemSlider) \
    V(CSkinItemProgress) \
    V(CSkinItemGroup) \
    V(CSkinItemTab) \
    V(CSkinItemSplitter) \
    V(CSkinItemHeader) \
    V(CSkinItemTree) \

#define SKIN_NAME_TYPE_MAP(V) \
    V(Skin_Base, CSkinBase) \
    V(Skin_Proxy, CSkinProxy) \
    V(Skin_Splitter, CSkinSplitter) \
    V(Skin_Group, CSkinGroup) \
    V(Skin_Progress, CSkinProgress) \
    V(Skin_Libretto, CSkinLibretto) \
    V(Skin_Tab, CSkinTab) \
    V(Skin_Button, CSkinButton) \
    V(Skin_Text, CSkinText) \
    V(Skin_Image, CSkinImage) \
    V(Skin_Round_Image, CSkinRoundImage) \
    V(Skin_Item, CSkinItem) \
    V(Skin_Window, CSkinItemDialog) \
    V(Skin_Item_Dialog, CSkinItemDialog) \
    V(Skin_Item_Combo, CSkinItemCombo) \
    V(Skin_Item_List, CSkinItemList) \
    V(Skin_Item_Static, CSkinItemStatic) \
    V(Skin_Item_Button, CSkinItemButton) \
    V(Skin_Item_Edit, CSkinItemEdit) \
    V(Skin_Item_Slider, CSkinItemSlider) \
    V(Skin_Item_Progress, CSkinItemProgress) \
    V(Skin_Item_Group, CSkinItemGroup) \
    V(Skin_Item_Tab, CSkinItemTab) \
    V(Skin_Item_Splitter, CSkinItemSplitter) \
    V(Skin_Item_Header, CSkinItemHeader) \
    V(Skin_Item_Tree, CSkinItemTree) \

typedef CObject* (PASCAL *SKIN_CREATOR)();

struct SKINMAP_ENTRY
{
    LPCTSTR lpszClassName;
    SKIN_CREATOR pfnSkinCreator;
#ifdef _DEBUG
    UINT sizeOfClass;
#endif
};

struct SKINNAMETYPEMAP_ENTRY
{
    LPCTSTR lpszXmlName;
    LPCTSTR lpszClassName;
};

class CImageRepository;
class CSkinBase;
class CSkinItemDialog;
class ISkinJSEngine;
class CSkinClass;
class CSkinEngine : public CCmdTarget
{
	friend class CSkinBase;
	DECLARE_DYNCREATE(CSkinEngine)

public:
	CSkinEngine();
	virtual ~CSkinEngine();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSkinEngine)
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	BEGIN_DUAL_INTERFACE_PART(SkinEngine, ISkinEngine)
		STDMETHOD(LoadSkin)(HWND hWnd, BSTR bstrSkinPath);
		STDMETHOD(UnloadSkin)(HWND hWnd);
		STDMETHOD(IsSkined)(HWND varHwnd, VARIANT_BOOL* pBool);
		STDMETHOD(GetSkinPath)(BSTR* pBstrSkinPath);
		STDMETHOD(SetSkinPath)(BSTR bstrSkinPath);
		STDMETHOD(LoadSkinPack)(BSTR bstrSkinPack);
		STDMETHOD(SetModuleState)(LONG lpModuleState);
		STDMETHOD(GetSubSkinProp)(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT* pVarValue);
		STDMETHOD(SetSubSkinProp)(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT varValue);
		STDMETHOD(CallSubSkinMethod)(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams);
		STDMETHOD(GetSubSkinPos)(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG* pLeft, LONG* pTop, LONG* pRight, LONG* pBottom);
		STDMETHOD(InvalidateSubSkinRect)(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG left, LONG top, LONG width, LONG height, LONG bErase);
		STDMETHOD(GetDefaultImageTransformParam)(UINT* pTransformType, LONG* pWidth, LONG* pHeight, LONG* pHue, LONG* pSat, FLOAT* pBlend, LONG* pR, LONG* pG, LONG* pB);
		STDMETHOD(SetDefaultImageTransformParam)(UINT uTransformType, LONG width, LONG height, LONG hue, LONG sat, FLOAT blend, LONG r, LONG g, LONG b);
		STDMETHOD(GetTransformedColor)(LONG color, LONG* pTransformedColor, UINT uTransformType, LONG hue, LONG sat, FLOAT blend, LONG r, LONG g, LONG b);
        STDMETHOD(TransformColor)(LONG color, LONG* pTransformedColor);
        STDMETHOD(PreTranslateMessage)(HWND hWnd, UINT uMsg, LONG wParam, LONG lParam, DWORD time, LONG x, LONG y, LONG bFilterIMM);
        STDMETHOD(CreateJSEngine)(IUnknown** ppJSEngine);
        STDMETHOD(SetGlobalMessageReceiver)(HWND hWndReceiver);
    END_DUAL_INTERFACE_PART(SkinEngine)

public:
	//////////////////////////////////////////////////////////////////////////
	//Interface Method
	//////////////////////////////////////////////////////////////////////////
	HRESULT LoadSkin(HWND hWnd, BSTR bstrSkinName);
	HRESULT UnloadSkin(HWND hWnd);
	HRESULT IsSkined(HWND varHwnd, VARIANT_BOOL* pBool) const;
	HRESULT GetSkinPath(BSTR* pBstrSkinPath) const;
	HRESULT SetSkinPath(BSTR bstrSkinPath);
	HRESULT LoadSkinPack(BSTR bstrSkinPack);
	HRESULT SetModuleState(LONG lpModuleState);
	HRESULT GetSubSkinProp(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT* pVarValue);
	HRESULT SetSubSkinProp(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT varValue);
	HRESULT CallSubSkinMethod(HWND skinWindowHwnd, BSTR bstrSubSkinName, BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams);
	HRESULT GetSubSkinPos(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG* pLeft, LONG* pTop, LONG* pRight, LONG* pBottom);
	HRESULT InvalidateSubSkinRect(HWND skinWindowHwnd, BSTR bstrSubSkinName, LONG left, LONG top, LONG width, LONG height, LONG bErase);
	HRESULT GetDefaultImageTransformParam(UINT* pTransformType, LONG* pWidth, LONG* pHeight, LONG* pHue, LONG* pSat, FLOAT* pBlend, LONG* pR, LONG* pG, LONG* pB);
	HRESULT SetDefaultImageTransformParam(UINT uTransformType, LONG width, LONG height, LONG hue, LONG sat, FLOAT blend, LONG r, LONG g, LONG b);
	HRESULT GetTransformedColor(LONG color, LONG* pTransformedColor, UINT uTransformType, LONG hue, LONG sat, FLOAT blend, LONG r, LONG g, LONG b);
    HRESULT TransformColor(LONG color, LONG* pTransformedColor);
	HRESULT PreTranslateMessage(HWND hWnd, UINT uMsg, LONG wParam, LONG lParam, DWORD time, LONG x, LONG y, LONG bFilterIMM = TRUE);
    HRESULT CreateJSEngine(IUnknown** ppJSEngine);
    HRESULT SetGlobalMessageReceiver(HWND hWndReceiver);

	//////////////////////////////////////////////////////////////////////////
	//Initialize and Uninitialize
	//////////////////////////////////////////////////////////////////////////
protected:
	BOOL Initialize();
	BOOL Uninitialize();

	//////////////////////////////////////////////////////////////////////////
	//Path of Skin Files
	//////////////////////////////////////////////////////////////////////////
public:
	CString GetSkinPath() const;
protected:
	CString	m_strSkinEnginePath;


	//////////////////////////////////////////////////////////////////////////
	//CSkinItemDialog Configuration
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL LoadSkin(HWND hWnd, LPCTSTR configFile);
	BOOL UnLoadSkin(HWND hWnd);
	BOOL ReLoadSkin(HWND hWnd);
	//BOOL GetSkinConfig(HWND hWnd, CString& configFileName) const;
protected:
	std::map<HWND, CString> m_mapSkinConfig;


	//////////////////////////////////////////////////////////////////////////
	//Map HWND to CSkinItemDialog
	//////////////////////////////////////////////////////////////////////////
protected:
	std::map<HWND, CSkinItemDialog*> m_mapSkinItemDialog;


	//////////////////////////////////////////////////////////////////////////
	//File Repository
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL GetFile(LPCTSTR filePathName, CFileData& fileData);
protected:
	CFileRepository* m_pFileRepository;


	//////////////////////////////////////////////////////////////////////////
	//Image Repository
	//////////////////////////////////////////////////////////////////////////
public:
	CImageBase* LoadImage(LPCTSTR filePathName);
	CImageBase* LoadImageInSkinPath(LPCTSTR fileName);
protected:
	CImageRepository* m_pImageRepository;

#ifdef USE_GDIPLUS
	//////////////////////////////////////////////////////////////////////////
	//Gdiplus
	//////////////////////////////////////////////////////////////////////////
protected:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
#endif //USE_GDIPLUS

	//////////////////////////////////////////////////////////////////////////
	//SkinEngine Message
	//////////////////////////////////////////////////////////////////////////
protected:
	static LRESULT CALLBACK _SkinEngineWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT SkinEngineWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	LRESULT SendEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL PostEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT SendBroadcastEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL PostBroadcastEngineMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	class CSkinEngineWndMap : public std::map<HWND, CSkinEngine*>
	{
	public:
		CSkinEngineWndMap();
		~CSkinEngineWndMap();
		CSkinEngine* Find(HWND hWnd);
		BOOL AddPair(HWND hWnd, CSkinEngine* pEngine);
		BOOL RemovePair(HWND hWnd, CSkinEngine* pEngine);
		LRESULT SendBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL PostBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
#ifdef MULTI_THREAD_SUPPORT
        CCSWrapper m_csSkinEngineWndMap;
#endif
	};
	HWND m_hEngineWnd;
	WNDPROC m_pOldProc;
	const bool m_bSkinEngineWndMapLoad;
	static CSkinEngineWndMap s_mapSkinEngineWnd;


	//////////////////////////////////////////////////////////////////////////
	//Timer Service
	//////////////////////////////////////////////////////////////////////////
public:
	UINT_PTR SetTimer(UINT nIDEvent, UINT nEclapse, CSkinBase* pSkin);
	BOOL KillTimer(UINT nIDEvent, CSkinBase* pSkin);
protected:
    struct SkinTimerStruct
    {
        CSkinBase* pSkin;
        UINT uIDEvent;
        bool operator < (const SkinTimerStruct& right) const {
            if (pSkin != right.pSkin)
                return pSkin < right.pSkin;

            return uIDEvent < right.uIDEvent;
        }
        bool operator == (const SkinTimerStruct& right) const {
            return (pSkin == right.pSkin) && (uIDEvent == right.uIDEvent);
        }
    };
	//¶¨Ê±Æ÷Ó³Éä±í
    std::map<UINT, SkinTimerStruct> m_mapSkinTimer;
    std::map<SkinTimerStruct, UINT> m_mapSkinTimerReverse;
    UINT GenerateTimerID();
	virtual BOOL OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);


	//////////////////////////////////////////////////////////////////////////
	//SkinEngine Set
	//////////////////////////////////////////////////////////////////////////
protected:
	class CSkinEngineSet : public std::set<CSkinEngine*>
	{
	public:
		CSkinEngineSet();
		~CSkinEngineSet();
		BOOL Add(CSkinEngine* pEngine);
		BOOL Remove(CSkinEngine* pEngine);
	protected:
#ifdef MULTI_THREAD_SUPPORT
        CCSWrapper m_csSkinEngineSet;
#endif
	};
	const bool m_bSkinEngineSetLoad;
	static CSkinEngineSet s_setAllSkinEngine;

	//////////////////////////////////////////////////////////////////////////
	//Default Image Transform Parameter
	//////////////////////////////////////////////////////////////////////////
public:
	const CImageTransformParam* GetDefaultImageParam() const;
	BOOL SetDefaultImageParam(const CImageTransformParam& param);
	BOOL TransformColor(LONG color, LONG& transformColor);
protected:
	CImageTransformParam m_defaultImageParam;

	//////////////////////////////////////////////////////////////////////////
	//Change Color
	//////////////////////////////////////////////////////////////////////////
protected:
	virtual BOOL OnChangeColor(UINT uMsg, WPARAM wParam, LPARAM lParam);

    //////////////////////////////////////////////////////////////////////////
    //Global Hook
    //////////////////////////////////////////////////////////////////////////
protected:
    virtual BOOL OnGlobalMouseHook(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnGlobalKeyboardHook(UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL m_bGlobalMouseHook;
    BOOL m_bGlobalKeyboardHook;

	//////////////////////////////////////////////////////////////////////////
	//Skin Factory
	//////////////////////////////////////////////////////////////////////////
public:
    static const SKINMAP_ENTRY* GetSkinMapEntry(LPCTSTR className);
    static LPCTSTR GetClassName(LPCTSTR xmlName);
	CSkinBase* CreateSkinByClassName(LPCTSTR className);
	CSkinBase* CreateSkinByXmlName(LPCTSTR xmlName);
	BOOL DestroySkin(CSkinBase* pSkin);
	BOOL IsSkinDestroyed(CSkinBase* pSkin);
private:
	void DestroyAllSkin();
protected:
	std::set<CSkinBase*> m_setAllSkin;
#ifdef MULTI_THREAD_SUPPORT
    CCSWrapper m_cs;
#endif
    static const SKINMAP_ENTRY s_skinMap[];
    static const SKINNAMETYPEMAP_ENTRY s_skinNameTypeMap[];

    //////////////////////////////////////////////////////////////////////////
    //Class
    //////////////////////////////////////////////////////////////////////////
public:
    CSkinClass* GetSkinClass(LPCTSTR className, LPCTSTR skinClassName);
protected:
    BOOL LoadSkinClass(LPCTSTR skinClassFileName);
    BOOL AddSkinClass(LPCTSTR className, LPCTSTR skinClassName, CSkinClass* pSkinClass);
    void RemoveAllSkinClass();
    std::map<CString, std::map<CString, CSkinClass*>> m_mapSkinClass;

    //////////////////////////////////////////////////////////////////////////
    //Script
    //////////////////////////////////////////////////////////////////////////
public:
    BOOL CreateSkinJSEngine(CSkinBase* pBase);
    BOOL CloseSkinJSEngine(CSkinBase* pBase);
    ISkinJSEngine* GetSkinJSEngine(CSkinBase* pBase);
protected:
    std::map<CSkinBase*, ISkinJSEngine*> m_mapSkinJSEngine;

	//////////////////////////////////////////////////////////////////////////
	//monitor memory
	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
protected:
	static int s_iMaxSkinMem;
#endif

	//////////////////////////////////////////////////////////////////////////
	//Keyboard Hooker
	//////////////////////////////////////////////////////////////////////////
protected:
	CKeyboardHooker m_KeyboardHooker;

    //////////////////////////////////////////////////////////////////////////
    //Mouse Hooker
    //////////////////////////////////////////////////////////////////////////
protected:
    CMouseHooker m_MouseHooker;

	//////////////////////////////////////////////////////////////////////////
	//Translate Message
	//////////////////////////////////////////////////////////////////////////
public:
	static BOOL TranslateMessageHelper(HWND hWnd, UINT uMsg, LONG wParam, LONG lParam, DWORD time, LONG x, LONG y, LONG bFilterIMM);

	//////////////////////////////////////////////////////////////////////////
	//Skin Buffer
	//////////////////////////////////////////////////////////////////////////
protected:
	BOOL AddBuffer(LPCTSTR configName);
	BOOL RemoveBuffer(LPCTSTR configName);
	void MaintainSkinBuffer();
	CSkinItemDialog* GetBufferSkin(LPCTSTR configName);
	BOOL ResetBufferSkin(LPCTSTR configName);
	std::map<CString, CSkinItemDialog*> m_mapSkinBuffer;

    //////////////////////////////////////////////////////////////////////////
    //Version Of Common Control
    //////////////////////////////////////////////////////////////////////////
public:
    DWORD GetComctl32Version();
protected:
    DLLVERSIONINFO m_comctlVerInfo;

    //////////////////////////////////////////////////////////////////////////
    //Global Message
    //////////////////////////////////////////////////////////////////////////
public:
    static BOOL SetGlobalMessageReceiverHwnd(HWND hWnd);
protected:
    static LONG m_nCtrlCount;
    static DWORD m_tLastCtrlTime;
    static HWND m_hGlobalMessageReceiver;
};
