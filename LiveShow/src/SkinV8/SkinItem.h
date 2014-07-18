
#pragma once

#include "SkinBase.h"
#include "SkinEngine.h"
#include "SkinItemDefine.h"
#include "SkinTextStyle.h"

#include <map>
#include <vector>

class CSkinItem;
class ICSkinItemVisitor
{
public:
	virtual void Visit(CSkinItem*) = 0;
};

//窗口过程调用堆栈栈帧
typedef struct tagSKINITEM_STACK_FRAME {
    HWND hWnd;
    UINT uMsg;
    WPARAM wParam;
    LPARAM lParam;
} SKINITEM_STACK_FRAME, *LPSKINITEM_STACK_FRAME;


#define SKINITEM_GUARD_LIST(V) \
    V(UpdateRgn) \


#define DECLARE_SKINITEM_GUARD(Type) \
    protected: \
    BOOL IsIn##Type() {ASSERT(m_lIn##Type >= 0); return (m_lIn##Type > 0);}; \
    LONG m_lIn##Type; \
    protected: \
    class CIn##Type##Guard { \
    public: \
        CIn##Type##Guard(CSkinItem* pItem, CSkinEngine* pEngine) { \
            m_pSkinItem = pItem; \
            m_pSkinEngine = pEngine; \
            ASSERT(m_pSkinEngine != NULL); \
            if (m_pSkinItem) { \
                m_pSkinItem->m_lIn##Type ++; \
            } \
        }; \
        ~CIn##Type##Guard() { \
            if (m_pSkinItem) { \
                if (! m_pSkinEngine->IsSkinDestroyed(m_pSkinItem)) \
                    m_pSkinItem->m_lIn##Type --; \
                else \
                    ASSERT(0); \
                m_pSkinItem = NULL; \
            } \
        }; \
        void Dispose() { \
            if (m_pSkinItem) { \
                if (! m_pSkinEngine->IsSkinDestroyed(m_pSkinItem)) \
                    m_pSkinItem->m_lIn##Type --; \
                else \
                    ASSERT(0); \
                m_pSkinItem = NULL; \
            } \
        } \
        void Clear() { \
            m_pSkinItem = NULL; \
        } \
    protected: \
        CSkinItem* m_pSkinItem; \
        CSkinEngine* m_pSkinEngine; \
    }; \


#define CLEAR_SKINITEM_GUARD(Type) \
    m_lIn##Type = 0; \


#define SKINITEM_GUARD(Type) \
    CIn##Type##Guard Type##Guard(this, this->GetEngine()); \


// CSkinItem command target
class CImageBase;
class CSkinItemDialog;
class CSkinItem : public CSkinBase
{
	friend class CSkinEngine;
	DECLARE_DYNCREATE(CSkinItem)

protected:
	CSkinItem();
	virtual ~CSkinItem();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_SKIN_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
protected:
	enum 
	{
		SKINBASE_DISPIDS(CSkinItem),
		SKINITEM_DISPIDS(CSkinItem),
	};

protected:
    BSTR GetText();
    void SetText(LPCTSTR newVal);
	BOOL GetVisible();
	void SetVisible(BOOL newVal);
	BOOL GetDisabled();
	void SetDisabled(BOOL newVal);
	BOOL GetTabStop();
	void SetTabStop(BOOL newVal);

public:
	//skin坐标转屏幕坐标
	virtual BOOL SkinToScreen(LONG& x, LONG& y);
	virtual BOOL SkinToScreen(POINT& point);
	virtual BOOL SkinToScreen(RECT& rect);
	//屏幕坐标转skin坐标
	virtual BOOL ScreenToSkin(LONG& x, LONG& y);
	virtual BOOL ScreenToSkin(POINT& point);
	virtual BOOL ScreenToSkin(RECT& rect);

	//更新皮肤
	virtual BOOL UpdateSkinItem();

	//更新皮肤附着的窗口句柄
	virtual BOOL UpdateSkinItemHwnd();

	//更新皮肤属性，在皮肤新附着窗口时调用
	virtual BOOL UpdateSkinItemProperty();

	virtual BOOL MoveWindow(LPCRECT lpRect = NULL, BOOL bRepaint = FALSE);

	virtual BOOL InvalidateRect(LPCRECT lpRects = NULL, BOOL bErase = TRUE);

	virtual BOOL UpdateSkinRgn(LONG width, LONG height, BOOL bReDraw = FALSE);

    virtual BOOL IsFocused();

    virtual BOOL IsHot();

	HWND GetSafeHwnd();

protected:
    //Stack Guard
    static std::vector<SKINITEM_STACK_FRAME> s_vecSkinItemStack;
    static BOOL StackTrace(UINT nResr, SKINITEM_STACK_FRAME& frame);
    static BOOL IsInStack(UINT uMsg);
    static BOOL IsInStack(HWND hWnd, UINT uMsg);
    static BOOL IsOnStackBottom(UINT uMsg);
    static BOOL IsOnStackBottom(HWND hWnd, UINT uMsg);

protected:
    class CSkinItemStackGuard {
    public:
        CSkinItemStackGuard(CSkinItem* pSkinItem, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            m_pSkinItem = pSkinItem;
            ASSERT(m_pSkinItem != NULL);
            HWND hWnd = (m_pSkinItem != NULL) ? (m_pSkinItem->m_hWnd) : (NULL);
            ASSERT(::IsWindow(hWnd));
            if (m_pSkinItem != NULL) {
                SKINITEM_STACK_FRAME frame = {hWnd, uMsg, wParam, lParam};
                CSkinItem::s_vecSkinItemStack.push_back(frame);
            }
        };
        ~CSkinItemStackGuard() {
            ASSERT(m_pSkinItem != NULL);
            if (m_pSkinItem != NULL) {
                CSkinItem::s_vecSkinItemStack.pop_back();
                m_pSkinItem = NULL;
            }
        };
        void Dispose() {
            ASSERT(m_pSkinItem != NULL);
            if (m_pSkinItem != NULL) {
                CSkinItem::s_vecSkinItemStack.pop_back();
                m_pSkinItem = NULL;
            }
        };
    protected:
        CSkinItem* m_pSkinItem;
    };

#define SKINITEM_STACK_GUARD(uMsg, wParam, lParam) \
    CSkinItemStackGuard guard(this, uMsg, wParam, lParam); \

    SKINITEM_GUARD_LIST(DECLARE_SKINITEM_GUARD);

protected:
	void Update(void);
	BOOL UpdateFont();
public:
	BOOL Attach(HWND hWnd);
	HWND Detach(void);

    //
    BOOL MoveSkinItemHelper(const RECT& rect, BOOL bRepaint);

    //
    BOOL GetSkinItemRectHelper(RECT& rect);

    //
    BOOL SetSkinItemRedrawHelper(BOOL bRedraw = TRUE);

    //
    BOOL GetSkinItemTextHelper(CString& strText);
    BOOL SetSkinItemTextHelper(LPCTSTR strText);

	//窗口属性, 窗口句柄不合法时返回FALSE
	BOOL GetSkinItemLongHelper(int nIndex, DWORD nNewLong, BOOL& bVal);
	BOOL SetSkinItemLongHelper(int nIndex, DWORD nNewLong, BOOL bVal);

	BOOL GetSkinItemPlacementHelper(WINDOWPLACEMENT& wndPlace);
	BOOL SetSkinItemPlacementHelper(const WINDOWPLACEMENT& wndPlace);

	//获得子窗口
	struct EnumChildProcStruct
	{
		HWND hParentWnd;
		LONG nID;
		LPCTSTR wndClassName;
		LPCTSTR wndText;
		POINT pt;
		UINT uFindFlag;
		HWND hResultWnd;
	};
	enum ENUM_FIND_HWND
	{
		FIND_HWND_NONE = 0x00000000,
		FIND_HWND_HWND = 0x00000001,
		FIND_HWND_CTRL_ID = 0x00000002,
		FIND_HWND_WND_CLASS_TEXT = 0x00000004,
		FIND_HWND_WND_POS = 0x00000008,
	};
	static HWND GetChildItemHelper(HWND hParentWnd, LONG nID, LPCTSTR wndClassName, LPCTSTR wndText, POINT pt, UINT uFindFlag = FIND_HWND_CTRL_ID);
	static BOOL CALLBACK _EnumChildProc(HWND hWnd, LPARAM lParam);

	static BOOL GetAllChildRgnHelper(HWND hParent, HRGN& hRgn, BOOL bVisible = TRUE);

public:
	virtual BOOL CtlColor(CDC* pDC, CWnd* pWnd, HBRUSH& hBrush);
	virtual void DrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

    //过滤消息
    virtual BOOL PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息响应函数，不需调用原处理函数返回TRUE，否则返回FALSE
	virtual BOOL OnSkinNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnSyncPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnNcRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnGetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnGetTextLength(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnGetText(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnThemeChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnStyleChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnStyleChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnParantNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnEnable(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
	virtual BOOL OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnUpdateUIState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    //Button
    virtual BOOL OnBMSetState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    virtual BOOL OnBMSetCheck(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
    //ListView
    virtual BOOL OnLVMSetItemState(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

#ifdef CUSTOM_SCROLLBAR_SUPPORT
    virtual BOOL OnCoolSBCustomDraw(NMHDR* pNMHDR, LRESULT* lpResult);
#endif
    virtual BOOL OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	//窗口过程相关
	static std::map<HWND, CSkinItem*> s_mapSkinItem;
public:
	static CSkinItem* FromHandlePermanent(HWND hWnd);
	static CSkinItem* FromHandle(HWND hWnd, CSkinEngine* pEngine);
	static void Visit(ICSkinItemVisitor* pVisitor);
protected:
	static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT DefWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	enum GUI_THREAD_HWND
	{
		GUI_THREAD_HWND_ACTIVE,
		GUI_THREAD_HWND_FOCUS,
		GUI_THREAD_HWND_CAPTURE,
		GUI_THREAD_HWND_MENUOWNER,
		GUI_THREAD_HWND_MOVESIZE,
		GUI_THREAD_HWND_CARET,
	};
	static BOOL GetGUIThreadHwndHelper(HWND hCurrentWnd, GUI_THREAD_HWND flag, HWND& hWnd);
	static BOOL GetGUIThreadIDHelper(HWND hWnd, DWORD& threadId);
	static BOOL IsInGUIThreadOf(HWND hWnd);
#ifdef CUSTOM_SCROLLBAR_SUPPORT
    CImageBase* GetScrollBarImageHelper(UINT uBar, UINT uItem, UINT uState);
#endif //CUSTOM_SCROLLBAR_SUPPORT
protected:
	CSkinBase*		m_pPrevMouseInSkin;	//之前鼠标所在区域
	CSkinBase*		m_pSkinActive;		//当前Active元素
	//CSkinBase*		m_pSkinOldActive;	//
	CSkinBase*		m_pSkinFocus;		//当前获得焦点的元素
	//CSkinBase*		m_pSkinOldFocus;	//
	CSkinBase*		m_pSkinCapture;		//当前捕获鼠标的区域
	//CSkinBase*		m_pSkinOldCapture;	//
	CSkinBase*		m_pSkinMenuOwner;
	//CSkinBase*		m_pSkinOldMenuOwner;
	CSkinBase*		m_pSkinMoveSize;
	//CSkinBase*		m_pSkinOldMoveSize;
	CSkinBase*		m_pSkinCaret;
	//CSkinBase*		m_pSkinOldCaret;

	BOOL IsSkinHwndItemOf(CSkinBase* pBase);
public:
	//
	BOOL GetSkinActive(CSkinBase** ppSkin = NULL);
	BOOL GetSubSkinActive(CSkinBase** ppSkin = NULL);
	BOOL SetSkinActive(CSkinBase* pNewSkin);
	BOOL GetSkinFocus(CSkinBase** ppSkin = NULL);
	BOOL GetSubSkinFocus(CSkinBase** ppSkin = NULL);
	BOOL SetSkinFocus(CSkinBase* pNewSkin);
	BOOL GetSkinCapture(CSkinBase** ppSkin = NULL);
	BOOL GetSubSkinCapture(CSkinBase** ppSkin = NULL);
	BOOL SetSkinCapture(CSkinBase* pNewSkin);
	BOOL GetSkinMenuOwner(CSkinBase** ppSkin = NULL);
	BOOL GetSubSkinMenuOwner(CSkinBase** ppSkin = NULL);
	BOOL SetSkinMenuOwner(CSkinBase* pNewSkin);
	BOOL GetSkinMoveSize(CSkinBase** ppSkin = NULL);
	BOOL GetSubSkinMoveSize(CSkinBase** ppSkin = NULL);
	BOOL SetSkinMoveSize(CSkinBase* pNewSkin);
	BOOL GetSkinCaret(CSkinBase** ppSkin = NULL);
	BOOL GetSubSkinCaret(CSkinBase** ppSkin = NULL);
	BOOL SetSkinCaret(CSkinBase* pNewSkin);

public:
	BOOL GetActiveSubSkin(CSkinItem** ppItem, CSkinBase** ppSkin);
	BOOL SetActiveSubSkin(CSkinBase* pSkin, CSkinBase** ppPrevSkin = NULL);
	BOOL GetFocusSubSkin(CSkinItem** ppItem, CSkinBase** ppSkin);
	BOOL SetFocusSubSkin(CSkinBase* pSkin, CSkinBase** ppPrevSkin = NULL);
	BOOL GetCaptureSubSkin(CSkinItem** ppItem, CSkinBase** ppSkin);
	BOOL SetCaptureSubSkin(CSkinBase* pSkin, CSkinBase** ppPrevSkin = NULL);
	BOOL ReleaseCaptureSubSkin(CSkinBase* pSkin);

public:
	//属性
	LONG GetitemID();
	void SetitemID(LONG newVal);
	BSTR GetclassName();
	void SetclassName(LPCTSTR newVal);
	LONG GetfindX();
	void SetfindX(LONG newVal);
	LONG GetfindY();
	void SetfindY(LONG newVal);
    VARIANT_BOOL GetuseSkinPaint();
    void SetuseSkinPaint(VARIANT_BOOL newVal);
	VARIANT_BOOL GetselfOnPaint();
	void SetselfOnPaint(VARIANT_BOOL newVal);
    VARIANT_BOOL GetselfOnNcPaint();
    void SetselfOnNcPaint(VARIANT_BOOL newVal);
	VARIANT_BOOL GetselfOnEraseBkgnd();
	void SetselfOnEraseBkgnd(VARIANT_BOOL newVal);
    VARIANT_BOOL GetselfOnSetFocus();
    void SetselfOnSetFocus(VARIANT_BOOL newVal);
    BSTR GeteraseBkMode();
    void SeteraseBkMode(LPCTSTR newVal);
    VARIANT_BOOL GetselfOnSize();
	void SetselfOnSize(VARIANT_BOOL newVal);
    VARIANT_BOOL GetselfOnCtlColor();
    void SetselfOnCtlColor(VARIANT_BOOL newVal);
    VARIANT_BOOL GetbkColorTransform();
    void SetbkColorTransform(VARIANT_BOOL newVal);
	VARIANT_BOOL GetclipAllChild();
	void SetclipAllChild(VARIANT_BOOL newVal);
	LONG Gethwnd();
	void Sethwnd(LONG newVal);
	LONG GetrestoredWidth();
	void SetrestoredWidth(LONG newVal);
	LONG GetrestoredHeight();
	void SetrestoredHeight(LONG newVal);

	BSTR GetbkMode(void);
	void SetbkMode(LPCTSTR newVal);
	LONG GetbkColorNormal(void);
	void SetbkColorNormal(LONG newVal);
	LONG GetbkColorHover(void);
	void SetbkColorHover(LONG newVal);
	LONG GetbkColorFocus(void);
	void SetbkColorFocus(LONG newVal);
	LONG GetbkColorDisabled(void);
	void SetbkColorDisabled(LONG newVal);

	VARIANT_BOOL GetuseOriginPosition();
	void SetuseOriginPosition(VARIANT_BOOL newVal);
    VARIANT_BOOL GetuseOriginText();
    void SetuseOriginText(VARIANT_BOOL newVal);
	LONG GettextColorNormal(void);
	void SettextColorNormal(LONG newVal);
	LONG GettextColorHover(void);
	void SettextColorHover(LONG newVal);
	LONG GettextColorDown(void);
	void SettextColorDown(LONG newVal);
	LONG GettextColorDisabled(void);
	void SettextColorDisabled(LONG newVal);
	//字体和大小
	LONG GetfontSize(void);
	void SetfontSize(LONG newVal);
	LONG GetfontWeight(void);
	void SetfontWeight(LONG newVal);
	VARIANT_BOOL Getitalic(void);
	void Setitalic(VARIANT_BOOL newVal);
	VARIANT_BOOL Getunderline(void);
	void Setunderline(VARIANT_BOOL newVal);
	VARIANT_BOOL GetstrikeOut(void);
	void SetstrikeOut(VARIANT_BOOL newVal);
	BSTR GetfontFamily(void);
	void SetfontFamily(LPCTSTR newVal);
	VARIANT_BOOL GetwordBreak(void);
	void SetwordBreak(VARIANT_BOOL newVal);
	VARIANT_BOOL GetsingleLine(void);
	void SetsingleLine(VARIANT_BOOL newVal);

	BSTR GettextHorizAlign(void);
	void SettextHorizAlign(LPCTSTR newVal);
	BSTR GettextVertiAlign(void);
	void SettextVertiAlign(LPCTSTR newVal);

    BACKGROUND_IMAGE_LIST(CSkinItem, DECLARE_IMAGE_PROPERTY);

	BSTR GetsmallIcon(void);	//不可用
	void SetsmallIcon(LPCTSTR newVal);
	BSTR GetbigIcon(void);	//不可用
	void SetbigIcon(LPCTSTR newVal);
	VARIANT_BOOL GetshowIcon(void);
	void SetshowIcon(VARIANT_BOOL newVal);

	//透明度相关
	VARIANT_BOOL Getlayered(void);
	void Setlayered(VARIANT_BOOL newVal);
	LONG Getopacity();
	void Setopacity(LONG newVal);

	VARIANT_BOOL GettopMost(void);
	void SettopMost(VARIANT_BOOL newVal);

	VARIANT_BOOL GetrecieveCtrlBreak(void);
	void SetrecieveCtrlBreak(VARIANT_BOOL newVal);

    VARIANT_BOOL GetbkDrag(void);
    void SetbkDrag(VARIANT_BOOL newVal);

    VARIANT_BOOL GettrackMouse(void);
    void SettrackMouse(VARIANT_BOOL newVal);

    VARIANT_BOOL GetskinScrollBar();
    void SetskinScrollBar(VARIANT_BOOL newVal);
    VARIANT_BOOL GetskinScrollStretchPage();
    void SetskinScrollStretchPage(VARIANT_BOOL newVal);
    LONG GetskinScrollBarChild();
    void SetskinScrollBarChild(LONG newVal);
    LONG GetskinScrollBarWidth();
    void SetskinScrollBarWidth(LONG newVal);
    LONG GetskinScrollBarHeight();
    void SetskinScrollBarHeight(LONG newVal);
    LONG GetskinScrollBarSideWidth();
    void SetskinScrollBarSideWidth(LONG newVal);
    LONG GetskinScrollBarSideHeight();
    void SetskinScrollBarSideHeight(LONG newVal);

    SCROLLBAR_IMAGE_LIST(CSkinItem, DECLARE_IMAGE_PROPERTY);

    void ShowToolTip(CString pszTooltip, const RECT& rcTip, LONG maxTipWidth = 0);
    void PopToolTip();

	//Dispatch方法
	LONG SendSkinMessage(LONG lCode, LONG lParam);
	VARIANT_BOOL PostSkinMessage(LONG lCode, LONG lParam);
	void DebugMessage(VARIANT* pvarShow);
	VARIANT_BOOL IsIconic();
	VARIANT_BOOL IsZoomed();
	VARIANT_BOOL IsBinded();
    VARIANT_BOOL IsMenuOwner();
	VARIANT_BOOL Show(LPCTSTR showPara);
	VARIANT_BOOL Invalidate(void);
    LONG GetKeyState(LONG uKey);

#ifdef V8_JS_ENGINE_SUPPORT
    //v8实现
    v8::Handle<v8::Value> V8SendSkinMessage(const v8::Arguments& args);
    v8::Handle<v8::Value> V8PostSkinMessage(const v8::Arguments& args);
    v8::Handle<v8::Value> V8DebugMessage(const v8::Arguments& args);
    v8::Handle<v8::Value> V8IsIconic(const v8::Arguments& args);
    v8::Handle<v8::Value> V8IsZoomed(const v8::Arguments& args);
	v8::Handle<v8::Value> V8IsBinded(const v8::Arguments& args);
    v8::Handle<v8::Value> V8IsMenuOwner(const v8::Arguments& args);
    v8::Handle<v8::Value> V8Show(const v8::Arguments& args);
    v8::Handle<v8::Value> V8Invalidate(const v8::Arguments& args);
    v8::Handle<v8::Value> V8GetKeyState(const v8::Arguments& args);
    virtual v8::Handle<v8::Value> V8InternalCallMethod(const CString& funcName, const v8::Arguments& args);
#endif //V8_JS_ENGINE_SUPPORT

protected:
	//旧的窗口过程函数指针
	WNDPROC	m_pOldWndProc;

    //使用皮肤绘制
    BOOL    m_bUseSkinPaint;

	//调用自身的OnPaint
	BOOL    m_bSelfOnPaint;

    //调用自身的OnNcPaint
    BOOL    m_bSelfOnNcPaint;

	//调用自身的OnEraseBkgnd
	BOOL	m_bSelfOnEraseBkgnd;

    //调用自身的OnSetFocus
    BOOL    m_bSelfOnSetFocus;

    //在OnEraseBkgnd中设置的背景模式
    LONG    m_lEraseBkMode;

	//调用自身的OnSize
	BOOL	m_bSelfOnSize;

    //调用自身的OnCtlColor
    BOOL    m_bSelfOnCtlColor;

    //背景色是否变换
    BOOL    m_bBkColorTransform;


	//Top Most
	BOOL	m_bTopMost;

    BOOL    m_bBkDrag;            //背景区域拖动
    CPoint  m_ptDragPoint;                //拖动点
    CPoint  m_ptOldDragPoint;           //上次的鼠标位置
    BOOL    m_bDragging;                  //是否在拖动

    BOOL    m_bSkinScrollBar;
    BOOL    m_bSkinScrollStretchPage;
    CRect   m_rcSkinScrollPage;
    LONG    m_lSkinScrollBarChild;
    LONG    m_lSkinScrollBarWidth;
    LONG    m_lSkinScrollBarHeight;
    LONG    m_lSkinScrollBarSideWidth;
    LONG    m_lSkinScrollBarSideHeight;

	//
	BOOL	m_bClipAllChild;

	//
	BOOL	m_bRecieveCtrlBreak;

    //
    BOOL    m_bTrackMouse;
    BOOL    m_bMouseEntered;

private:
	//查找窗口的方法
	ENUM_FIND_HWND	m_enFindHwnd;

	//查找的窗口
	HWND	m_hFindHwnd;

	//控件ID，用于查找窗口
	UINT	m_nItemID;

	//窗口的className，用于查找窗口
	CString m_strWndClassName;

	//窗口中的点，用于查找窗口
	CPoint	m_ptFindPoint;

protected:
	//控件句柄
	HWND	m_hWnd;

    //提示框句柄
    HWND    m_hToolTip;


	//背景颜色
	LONG	m_clrBkColorNormal;
	LONG	m_clrBkColorHover;
	LONG	m_clrBkColorFocus;
	LONG	m_clrBkColorDisabled;
	CBrush	m_brushBk;

	//透明度相关
	BOOL	m_bLayered;	//layered属性
	LONG	m_nOpacity;	//不透明度，范围0-100

	//使用默认位置
	BOOL	m_buseOriginPosition;
    BOOL    m_buseOriginText;

	//各种状态下的文本风格
	CSkinTextStyle m_textStyleNormal;
	CSkinTextStyle m_textStyleHover;
	CSkinTextStyle m_textStyleDown;
	CSkinTextStyle m_textStyleDisabled;

	//Icon
	HICON							m_hSmallIcon;
	HICON							m_hBigIcon;
	BOOL							m_bShowIcon;

    BACKGROUND_IMAGE_LIST(CSkinItem, DECLARE_IMAGE_PROPERTY_VAR);
    SCROLLBAR_IMAGE_LIST(CSkinItem, DECLARE_IMAGE_PROPERTY_VAR);
};
