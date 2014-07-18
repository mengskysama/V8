#pragma once

#include "XmlCmdTargetDefine.h"

class CSkinBase;
typedef CCmdTargetPtr<CSkinBase> CSkinBasePtr;

#define DECLARE_SKINBASE_PROPERTIES()	\
public:									\
	BSTR Getname();						\
	void Setname(LPCTSTR newVal);		\
    BSTR GetskinClass();				\
    void SetskinClass(LPCTSTR newVal);	\
    BSTR GetpreLoad();				\
    void SetpreLoad(LPCTSTR newVal);	\
	BSTR Gettext();						\
	void Settext(LPCTSTR newVal);		\
	LONG Getleft();						\
	void Setleft(LONG newVal);			\
	LONG Gettop();						\
	void Settop(LONG newVal);			\
	LONG Getwidth();					\
	void Setwidth(LONG newVal);			\
	LONG Getheight();					\
	void Setheight(LONG newVal);		\
    VARIANT_BOOL GetsizeDelay();				\
    void SetsizeDelay(VARIANT_BOOL newVal);		\
	VARIANT_BOOL Getvisible();					\
	void Setvisible(VARIANT_BOOL newVal);		\
	VARIANT_BOOL Getdisabled();					\
	void Setdisabled(VARIANT_BOOL newVal);		\
	VARIANT_BOOL GettabStop();					\
	void SettabStop(VARIANT_BOOL newVal);		\
	VARIANT_BOOL Getgroup();					\
	void Setgroup(VARIANT_BOOL newVal);			\
	VARIANT_BOOL Getbackground();				\
	void Setbackground(VARIANT_BOOL newVal);	\
    VARIANT_BOOL GetmouseEvent();				\
    void SetmouseEvent(VARIANT_BOOL newVal);	\
	VARIANT_BOOL Gettransparent();				\
	void Settransparent(VARIANT_BOOL newVal);	\
	VARIANT_BOOL GetcolorTransform();			\
	void SetcolorTransform(VARIANT_BOOL newVal);\
    VARIANT_BOOL Getalpha();			        \
    void Setalpha(VARIANT_BOOL newVal);         \
	BSTR Getcursor();							\
	void Setcursor(LPCTSTR newVal);				\
    BSTR GetsysCursor();						\
    void SetsysCursor(LPCTSTR newVal);			\
	LONG GetupCornerRadius(void);				\
	void SetupCornerRadius(LONG newVal);		\
	LONG GetdownCornerRadius(void);				\
	void SetdownCornerRadius(LONG newVal);		\
	LONG GetborderLineWidth(void);				\
	void SetborderLineWidth(LONG newVal);		\
	LONG GetborderLineColor(void);				\
	void SetborderLineColor(LONG newVal);		\
	LONG GettransparentColor(void);				\
	void SettransparentColor(LONG newVal);		\
    VARIANT_BOOL GetscriptHandleEvent();				\
    void SetscriptHandleEvent(VARIANT_BOOL newVal);		\


#define IMPLEMENT_SKINBASE_PROPERTIES(theClass) \
	BSTR theClass::Getname(){					\
	return GetName();						    \
	};											\
	void theClass::Setname(LPCTSTR newVal){		\
	SetName(newVal);						    \
	};											\
    BSTR theClass::GetskinClass(){				\
    return GetSkinClass();						\
    };											\
    void theClass::SetskinClass(LPCTSTR newVal){\
    SetSkinClass(newVal);						\
    };											\
    BSTR theClass::GetpreLoad(){				\
    return GetPreLoad();						\
    };											\
    void theClass::SetpreLoad(LPCTSTR newVal){  \
    SetPreLoad(newVal);						    \
    };											\
	BSTR theClass::Gettext(){					\
	return GetText();						    \
	};											\
	void theClass::Settext(LPCTSTR newVal){		\
	SetText(newVal);						    \
	};											\
	LONG theClass::Getleft(){					\
	return GetLeft();					    	\
	};											\
	void theClass::Setleft(LONG newVal){		\
	SetLeft(newVal);						    \
	};											\
	LONG theClass::Gettop(){					\
	return GetTop();						    \
	};											\
	void theClass::Settop(LONG newVal){			\
	SetTop(newVal);							    \
	};											\
	LONG theClass::Getwidth(){					\
	return GetWidth();						    \
	};											\
	void theClass::Setwidth(LONG newVal){		\
	SetWidth(newVal);						    \
	};											\
	LONG theClass::Getheight(){					\
	return GetHeight();						    \
	};											\
	void theClass::Setheight(LONG newVal){		\
	SetHeight(newVal);						    \
	};											\
    VARIANT_BOOL theClass::GetsizeDelay(){				\
    return GetSizeDelay() ? (VARIANT_TRUE) : (VARIANT_FALSE);	    \
    };													\
    void theClass::SetsizeDelay(VARIANT_BOOL newVal){	\
    SetSizeDelay(newVal);								\
    };													\
    VARIANT_BOOL theClass::Getvisible(){				\
	return GetVisible() ? (VARIANT_TRUE) : (VARIANT_FALSE);		    \
	};													\
	void theClass::Setvisible(VARIANT_BOOL newVal){		\
	SetVisible(newVal);								    \
	};													\
	VARIANT_BOOL theClass::Getdisabled(){				\
	return GetDisabled() ? (VARIANT_TRUE) : (VARIANT_FALSE);		\
	};													\
	void theClass::Setdisabled(VARIANT_BOOL newVal){	\
	SetDisabled(newVal);							    \
	};													\
	VARIANT_BOOL theClass::GettabStop(){				\
	return GetTabStop() ? (VARIANT_TRUE) : (VARIANT_FALSE);			\
	};													\
	void theClass::SettabStop(VARIANT_BOOL newVal){		\
	SetTabStop(newVal);								\
	};													\
	VARIANT_BOOL theClass::Getgroup(){					\
	return GetGroup() ? (VARIANT_TRUE) : (VARIANT_FALSE);			\
	};													\
	void theClass::Setgroup(VARIANT_BOOL newVal){		\
	SetGroup(newVal);								\
	};													\
	VARIANT_BOOL theClass::Getbackground(){				\
	return GetBackground() ? (VARIANT_TRUE) : (VARIANT_FALSE);		\
	};													\
	void theClass::Setbackground(VARIANT_BOOL newVal){	\
	SetBackground(newVal);							\
	};													\
    VARIANT_BOOL theClass::GetmouseEvent(){				\
    return GetMouseEvent() ? (VARIANT_TRUE) : (VARIANT_FALSE);		\
    };													\
    void theClass::SetmouseEvent(VARIANT_BOOL newVal){	\
    SetMouseEvent(newVal);							\
    };													\
	VARIANT_BOOL theClass::Gettransparent(){				\
	return GetTransparent() ? (VARIANT_TRUE) : (VARIANT_FALSE);		\
	};													\
	void theClass::Settransparent(VARIANT_BOOL newVal){	\
	SetTransparent(newVal);							\
	};													\
	VARIANT_BOOL theClass::GetcolorTransform(){				\
	return GetColorTransform() ? (VARIANT_TRUE) : (VARIANT_FALSE);		\
	};													\
	void theClass::SetcolorTransform(VARIANT_BOOL newVal){	\
	SetColorTransform(newVal);							\
	};													\
    VARIANT_BOOL theClass::Getalpha(){				\
    return GetAlpha() ? (VARIANT_TRUE) : (VARIANT_FALSE);		\
    };													\
    void theClass::Setalpha(VARIANT_BOOL newVal){	    \
    SetAlpha(newVal);							        \
    };													\
	BSTR theClass::Getcursor(){							\
	return GetCursor();								\
	};													\
	void theClass::Setcursor(LPCTSTR newVal){			\
	SetCursor(newVal);								\
	};													\
    BSTR theClass::GetsysCursor(){							\
    return GetSysCursor();								\
    };													\
    void theClass::SetsysCursor(LPCTSTR newVal){			\
    SetSysCursor(newVal);								\
    };													\
	LONG theClass::GetupCornerRadius(){					\
	return GetUpCornerRadius();						\
	};													\
	void theClass::SetupCornerRadius(LONG newVal){		\
	SetUpCornerRadius(newVal);						\
	};													\
	LONG theClass::GetdownCornerRadius(){				\
	return GetDownCornerRadius();					\
	};													\
	void theClass::SetdownCornerRadius(LONG newVal){	\
	SetDownCornerRadius(newVal);					\
	};													\
	LONG theClass::GetborderLineWidth(void){			\
	return GetBorderLineWidth();					\
	};													\
	void theClass::SetborderLineWidth(LONG newVal){		\
	SetBorderLineWidth(newVal);						\
	};													\
	LONG theClass::GetborderLineColor(void){			\
	return GetBorderLineColor();					\
	};													\
	void theClass::SetborderLineColor(LONG newVal){		\
	SetBorderLineColor(newVal);						\
	};													\
	LONG theClass::GettransparentColor(void){			\
	return GetTransparentColor();					\
	};													\
	void theClass::SettransparentColor(LONG newVal){	\
	SetTransparentColor(newVal);					\
	};													\
    VARIANT_BOOL theClass::GetscriptHandleEvent(void){			\
    return GetScriptHandleEvent() ? (VARIANT_TRUE) : (VARIANT_FALSE); \
    };													\
    void theClass::SetscriptHandleEvent(VARIANT_BOOL newVal){	\
    SetScriptHandleEvent(newVal);					\
    };													\

//CSkinBase Enum for Dispatch
#define SKINBASE_DISPIDS(theClass)	\
	SKIN_DISPID(theClass, name), \
    SKIN_DISPID(theClass, skinClass), \
    SKIN_DISPID(theClass, preLoad), \
	SKIN_DISPID(theClass, text), \
	SKIN_DISPID(theClass, left), \
	SKIN_DISPID(theClass, top), \
	SKIN_DISPID(theClass, width), \
	SKIN_DISPID(theClass, height), \
    SKIN_DISPID(theClass, sizeDelay), \
	SKIN_DISPID(theClass, visible), \
	SKIN_DISPID(theClass, disabled), \
	SKIN_DISPID(theClass, tabStop), \
	SKIN_DISPID(theClass, group), \
	SKIN_DISPID(theClass, background), \
    SKIN_DISPID(theClass, mouseEvent), \
	SKIN_DISPID(theClass, transparent), \
	SKIN_DISPID(theClass, colorTransform), \
    SKIN_DISPID(theClass, alpha), \
	SKIN_DISPID(theClass, cursor), \
    SKIN_DISPID(theClass, sysCursor), \
	SKIN_DISPID(theClass, upCornerRadius), \
	SKIN_DISPID(theClass, downCornerRadius), \
	SKIN_DISPID(theClass, borderLineWidth), \
	SKIN_DISPID(theClass, borderLineColor), \
	SKIN_DISPID(theClass, transparentColor), \
    SKIN_DISPID(theClass, scriptHandleEvent), \
	SKIN_DISPID(theClass, DebugBreak), \
	SKIN_DISPID(theClass, GetParent), \
	SKIN_DISPID(theClass, GetChild), \
    SKIN_DISPID(theClass, CreateChild), \
	SKIN_DISPID(theClass, AttachEvent), \
	SKIN_DISPID(theClass, DetachEvent), \
	SKIN_DISPID(theClass, SetTimer), \
	SKIN_DISPID(theClass, KillTimer), \
	SKIN_DISPID(theClass, GetActive), \
	SKIN_DISPID(theClass, SetActive), \
	SKIN_DISPID(theClass, GetFocus), \
	SKIN_DISPID(theClass, SetFocus), \
	SKIN_DISPID(theClass, GetCapture), \
	SKIN_DISPID(theClass, SetCapture), \
	SKIN_DISPID(theClass, ReleaseCapture), \
    SKIN_DISPID(theClass, CreateLayout), \
    SKIN_DISPID(theClass, GetLayout), \
    SKIN_DISPID(theClass, DestroyLayout) \


//CSkinBase Dispatch
#define SKINBASE_DISP_PROPERTIES(theClass)		\
	SKIN_DISP_PROPERTY(theClass, name, VT_BSTR) 	\
    SKIN_DISP_PROPERTY(theClass, skinClass, VT_BSTR) 	\
    SKIN_DISP_PROPERTY(theClass, preLoad, VT_BSTR) 	\
	SKIN_DISP_PROPERTY(theClass, text, VT_BSTR) 	\
	SKIN_DISP_PROPERTY(theClass, left, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, top, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, width, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, height, VT_I4) 	\
    SKIN_DISP_PROPERTY(theClass, sizeDelay, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, visible, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, disabled, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, tabStop, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, group, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, background, VT_BOOL) 	\
    SKIN_DISP_PROPERTY(theClass, mouseEvent, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, transparent, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, colorTransform, VT_BOOL) 	\
    SKIN_DISP_PROPERTY(theClass, alpha, VT_BOOL) 	\
	SKIN_DISP_PROPERTY(theClass, cursor, VT_BSTR) 	\
    SKIN_DISP_PROPERTY(theClass, sysCursor, VT_BSTR) 	\
	SKIN_DISP_PROPERTY(theClass, upCornerRadius, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, downCornerRadius, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, borderLineWidth, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, borderLineColor, VT_I4) 	\
	SKIN_DISP_PROPERTY(theClass, transparentColor, VT_I4) 	\
    SKIN_DISP_PROPERTY(theClass, scriptHandleEvent, VT_BOOL) 	\
	SKIN_DISP_FUNCTION(theClass, DebugBreak, VT_EMPTY, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, GetParent, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, GetChild, VT_UNKNOWN, VTS_BSTR)	\
    SKIN_DISP_FUNCTION(theClass, CreateChild, VT_UNKNOWN, VTS_BSTR VTS_BSTR)	\
	SKIN_DISP_FUNCTION(theClass, AttachEvent, VT_BOOL, VTS_BSTR VTS_DISPATCH)	\
	SKIN_DISP_FUNCTION(theClass, DetachEvent, VT_BOOL, VTS_BSTR)	\
	SKIN_DISP_FUNCTION(theClass, SetTimer, VT_UI4, VTS_UI4 VTS_UI4)	\
	SKIN_DISP_FUNCTION(theClass, KillTimer, VT_BOOL, VTS_UI4)	\
	SKIN_DISP_FUNCTION(theClass, GetActive, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, SetActive, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, GetFocus, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, SetFocus, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, GetCapture, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, SetCapture, VT_UNKNOWN, VTS_NONE)	\
	SKIN_DISP_FUNCTION(theClass, ReleaseCapture, VT_BOOL, VTS_NONE)	\
    SKIN_DISP_FUNCTION(theClass, CreateLayout, VT_UNKNOWN, VTS_BSTR VTS_BSTR)	\
    SKIN_DISP_FUNCTION(theClass, GetLayout, VT_UNKNOWN, VTS_BSTR)	\
    SKIN_DISP_FUNCTION(theClass, DestroyLayout, VT_BOOL, VTS_BSTR)	\


#define DECLARE_IMAGE_PROPERTY(theCalss, propName) \
    BSTR Get##propName(); \
    void Set##propName(LPCTSTR newVal); \


#define DECLARE_IMAGE_PROPERTY_VAR(theCalss, propName) \
    CImageBase* m_p##propName; \
    CString m_str##propName; \


#define CLEAR_IMAGE_PROPERTY_VAR(theCalss, propName) \
    m_p##propName = NULL; \
    m_str##propName = _T(""); \


#define IMPLEMENT_IMAGE_PROPERTY(theClass, propName) \
    BSTR theClass::Get##propName() { \
        AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
        return m_str##propName.AllocSysString(); \
    }; \
    void theClass::Set##propName(LPCTSTR newVal) { \
        AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
        SetImageHelper(m_p##propName, m_str##propName, newVal); \
    }; \


