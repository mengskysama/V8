#pragma once
#include "XmlCmdTargetDefine.h"

#define SKIN_TEXT_STYLE_PROP_SUFFIX_LIST(V, theClass, propStatus) \
    V(theClass, color, VT_I4, propStatus) \
    V(theClass, bkColor, VT_I4, propStatus) \
    V(theClass, bkMode, VT_BSTR, propStatus) \
    V(theClass, fontSize, VT_I4, propStatus) \
    V(theClass, fontWeight, VT_I4, propStatus) \
    V(theClass, italic, VT_BOOL, propStatus) \
    V(theClass, underline, VT_BOOL, propStatus) \
    V(theClass, strikeOut, VT_BOOL, propStatus) \
    V(theClass, fontFamily, VT_BSTR, propStatus) \
    V(theClass, wordBreak, VT_BOOL, propStatus) \
    V(theClass, endEllipsis, VT_BOOL, propStatus) \
    V(theClass, noPrefix, VT_BOOL, propStatus) \
    V(theClass, singleLine, VT_BOOL, propStatus) \
    V(theClass, horizAlign, VT_BSTR, propStatus) \
    V(theClass, vertiAlign, VT_BSTR, propStatus) \
    V(theClass, shadow, VT_BOOL, propStatus) \
    V(theClass, beginLineMargin, VT_I4, propStatus) \
    V(theClass, lineMargin, VT_I4, propStatus) \
    V(theClass, endLineMargin, VT_I4, propStatus) \
    V(theClass, underlineMargin, VT_I4, propStatus) \

#define SKIN_TEXT_STYLE_PROP_SUFFIX_DISPID(theClass, propName, propType, propStatus) \
    SKIN_DISPID(theClass, propName##propStatus)

#define SKIN_TEXT_STYLE_PROP_SUFFIX_DISPIDS(theClass, propStatus) \
    SKIN_TEXT_STYLE_PROP_SUFFIX_LIST(SKIN_TEXT_STYLE_PROP_SUFFIX_DISPID, theClass, propStatus)

#define SKIN_TEXT_STYLE_PROP_SUFFIX_PROPERTY(theClass, propName, propType, propStatus) \
    SKIN_DISP_PROPERTY(theClass, propName##propStatus, propType)

#define SKIN_TEXT_STYLE_PROP_SUFFIX_PROPERTIES(theClass, propStatus) \
    SKIN_TEXT_STYLE_PROP_SUFFIX_LIST(SKIN_TEXT_STYLE_PROP_SUFFIX_PROPERTY, theClass, propStatus)

#define DECLARE_SKIN_TEXT_STYLE_SUFFIX_PROPERTIES(propStatus) \
    LONG Getcolor##propStatus(void); \
    void Setcolor##propStatus(LONG newVal); \
    LONG GetbkColor##propStatus(); \
    void SetbkColor##propStatus(LONG newVal); \
    BSTR GetbkMode##propStatus(void); \
    void SetbkMode##propStatus(LPCTSTR newVal); \
    LONG GetfontSize##propStatus(void); \
    void SetfontSize##propStatus(LONG newVal); \
    LONG GetfontWeight##propStatus(void); \
    void SetfontWeight##propStatus(LONG newVal); \
    VARIANT_BOOL Getitalic##propStatus(void); \
    void Setitalic##propStatus(VARIANT_BOOL newVal); \
    VARIANT_BOOL Getunderline##propStatus(void); \
    void Setunderline##propStatus(VARIANT_BOOL newVal); \
    VARIANT_BOOL GetstrikeOut##propStatus(void); \
    void SetstrikeOut##propStatus(VARIANT_BOOL newVal); \
    BSTR GetfontFamily##propStatus(void); \
    void SetfontFamily##propStatus(LPCTSTR newVal); \
    VARIANT_BOOL Getshadow##propStatus(void); \
    void Setshadow##propStatus(VARIANT_BOOL newVal); \
    VARIANT_BOOL GetwordBreak##propStatus(void); \
    void SetwordBreak##propStatus(VARIANT_BOOL newVal); \
    VARIANT_BOOL GetendEllipsis##propStatus(void); \
    void SetendEllipsis##propStatus(VARIANT_BOOL newVal); \
    VARIANT_BOOL GetnoPrefix##propStatus(void); \
    void SetnoPrefix##propStatus(VARIANT_BOOL newVal); \
    VARIANT_BOOL GetsingleLine##propStatus(void); \
    void SetsingleLine##propStatus(VARIANT_BOOL newVal); \
    BSTR GethorizAlign##propStatus(void); \
    void SethorizAlign##propStatus(LPCTSTR newVal); \
    BSTR GetvertiAlign##propStatus(void); \
    void SetvertiAlign##propStatus(LPCTSTR newVal); \
    LONG GetbeginLineMargin##propStatus(void); \
    void SetbeginLineMargin##propStatus(LONG newVal); \
    LONG GetlineMargin##propStatus(void); \
    void SetlineMargin##propStatus(LONG newVal); \
    LONG GetendLineMargin##propStatus(void); \
    void SetendLineMargin##propStatus(LONG newVal); \
    LONG GetunderlineMargin##propStatus(void); \
    void SetunderlineMargin##propStatus(LONG newVal); \

#define IMPLEMENT_SKIN_TEXT_STYLE_SUFFIX_PROPERTIES(theClass, propStatus, memberTextStyle) \
LONG theClass::Getcolor##propStatus(void) { \
    return memberTextStyle.Getcolor(); \
} \
void theClass::Setcolor##propStatus(LONG newVal) { \
    memberTextStyle.Setcolor(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetbkColor##propStatus(void) { \
    return memberTextStyle.GetbkColor(); \
} \
void theClass::SetbkColor##propStatus(LONG newVal) { \
    memberTextStyle.SetbkColor(newVal); \
    InvalidateRect(); \
} \
BSTR theClass::GetbkMode##propStatus(void) { \
    return memberTextStyle.GetbkMode(); \
} \
void theClass::SetbkMode##propStatus(LPCTSTR newVal) { \
    memberTextStyle.SetbkMode(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetfontSize##propStatus(void) { \
    return memberTextStyle.GetfontSize(); \
} \
void theClass::SetfontSize##propStatus(LONG newVal) { \
    memberTextStyle.SetfontSize(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetfontWeight##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetfontWeight(); \
} \
void theClass::SetfontWeight##propStatus(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetfontWeight(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::Getitalic##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.Getitalic(); \
} \
void theClass::Setitalic##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.Setitalic(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::Getunderline##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.Getunderline(); \
} \
void theClass::Setunderline##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.Setunderline(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::GetstrikeOut##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetstrikeOut(); \
} \
void theClass::SetstrikeOut##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetstrikeOut(newVal); \
    InvalidateRect(); \
} \
BSTR theClass::GetfontFamily##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetfontFamily(); \
} \
void theClass::SetfontFamily##propStatus(LPCTSTR newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetfontFamily(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::GetwordBreak##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetwordBreak(); \
} \
void theClass::SetwordBreak##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetwordBreak(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::GetendEllipsis##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetendEllipsis(); \
} \
void theClass::SetendEllipsis##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetendEllipsis(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::GetnoPrefix##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetnoPrefix(); \
} \
void theClass::SetnoPrefix##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetnoPrefix(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::GetsingleLine##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetsingleLine(); \
} \
void theClass::SetsingleLine##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetsingleLine(newVal); \
    InvalidateRect(); \
} \
BSTR theClass::GethorizAlign##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GethorizAlign(); \
} \
void theClass::SethorizAlign##propStatus(LPCTSTR newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SethorizAlign(newVal); \
    InvalidateRect(); \
} \
BSTR theClass::GetvertiAlign##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetvertiAlign(); \
} \
void theClass::SetvertiAlign##propStatus(LPCTSTR newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetvertiAlign(newVal); \
    InvalidateRect(); \
} \
VARIANT_BOOL theClass::Getshadow##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.Getshadow(); \
} \
void theClass::Setshadow##propStatus(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.Setshadow(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetbeginLineMargin##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetbeginLineMargin(); \
} \
void theClass::SetbeginLineMargin##propStatus(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetbeginLineMargin(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetlineMargin##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetlineMargin(); \
} \
void theClass::SetlineMargin##propStatus(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetlineMargin(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetendLineMargin##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetendLineMargin(); \
} \
void theClass::SetendLineMargin##propStatus(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetendLineMargin(newVal); \
    InvalidateRect(); \
} \
LONG theClass::GetunderlineMargin##propStatus(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetunderlineMargin(); \
} \
void theClass::SetunderlineMargin##propStatus(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetunderlineMargin(newVal); \
    InvalidateRect(); \
}

#define SKIN_TEXT_STYLE_PROP_PREFIX_LIST(V, theClass, propStatus) \
    V(theClass, Color, VT_I4, propStatus) \
    V(theClass, BkColor, VT_I4, propStatus) \
    V(theClass, BkMode, VT_BSTR, propStatus) \
    V(theClass, FontSize, VT_I4, propStatus) \
    V(theClass, FontWeight, VT_I4, propStatus) \
    V(theClass, Italic, VT_BOOL, propStatus) \
    V(theClass, Underline, VT_BOOL, propStatus) \
    V(theClass, StrikeOut, VT_BOOL, propStatus) \
    V(theClass, FontFamily, VT_BSTR, propStatus) \
    V(theClass, WordBreak, VT_BOOL, propStatus) \
    V(theClass, EndEllipsis, VT_BOOL, propStatus) \
    V(theClass, NoPrefix, VT_BOOL, propStatus) \
    V(theClass, SingleLine, VT_BOOL, propStatus) \
    V(theClass, HorizAlign, VT_BSTR, propStatus) \
    V(theClass, VertiAlign, VT_BSTR, propStatus) \
    V(theClass, Shadow, VT_BOOL, propStatus) \
    V(theClass, BeginLineMargin, VT_I4, propStatus) \
    V(theClass, LineMargin, VT_I4, propStatus) \
    V(theClass, EndLineMargin, VT_I4, propStatus) \
    V(theClass, UnderlineMargin, VT_I4, propStatus) \

#define SKIN_TEXT_STYLE_PROP_PREFIX_DISPID(theClass, propName, propType, propStatus) \
    SKIN_DISPID(theClass, propStatus##propName),

#define SKIN_TEXT_STYLE_PROP_PREFIX_DISPIDS(theClass, propStatus) \
    SKIN_TEXT_STYLE_PROP_PREFIX_LIST(SKIN_TEXT_STYLE_PROP_PREFIX_DISPID, theClass, propStatus)

#define SKIN_TEXT_STYLE_PROP_PREFIX_PROPERTY(theClass, propName, propType, propStatus) \
    SKIN_DISP_PROPERTY(theClass, propStatus##propName, propType)

#define SKIN_TEXT_STYLE_PROP_PREFIX_PROPERTIES(theClass, propStatus) \
    SKIN_TEXT_STYLE_PROP_PREFIX_LIST(SKIN_TEXT_STYLE_PROP_PREFIX_PROPERTY, theClass, propStatus)

#define DECLARE_SKIN_TEXT_STYLE_PREFIX_PROPERTIES(propStatus) \
    LONG Get##propStatus##Color(void); \
    void Set##propStatus##Color(LONG newVal); \
    LONG Get##propStatus##BkColor(); \
    void Set##propStatus##BkColor(LONG newVal); \
    BSTR Get##propStatus##BkMode(void); \
    void Set##propStatus##BkMode(LPCTSTR newVal); \
    LONG Get##propStatus##FontSize(void); \
    void Set##propStatus##FontSize(LONG newVal); \
    LONG Get##propStatus##FontWeight(void); \
    void Set##propStatus##FontWeight(LONG newVal); \
    VARIANT_BOOL Get##propStatus##Italic(void); \
    void Set##propStatus##Italic(VARIANT_BOOL newVal); \
    VARIANT_BOOL Get##propStatus##Underline(void); \
    void Set##propStatus##Underline(VARIANT_BOOL newVal); \
    VARIANT_BOOL Get##propStatus##StrikeOut(void); \
    void Set##propStatus##StrikeOut(VARIANT_BOOL newVal); \
    BSTR Get##propStatus##FontFamily(void); \
    void Set##propStatus##FontFamily(LPCTSTR newVal); \
    VARIANT_BOOL Get##propStatus##Shadow(void); \
    void Set##propStatus##Shadow(VARIANT_BOOL newVal); \
    VARIANT_BOOL Get##propStatus##WordBreak(void); \
    void Set##propStatus##WordBreak(VARIANT_BOOL newVal); \
    VARIANT_BOOL Get##propStatus##EndEllipsis(void); \
    void Set##propStatus##EndEllipsis(VARIANT_BOOL newVal); \
    VARIANT_BOOL Get##propStatus##NoPrefix(void); \
    void Set##propStatus##NoPrefix(VARIANT_BOOL newVal); \
    VARIANT_BOOL Get##propStatus##SingleLine(void); \
    void Set##propStatus##SingleLine(VARIANT_BOOL newVal); \
    BSTR Get##propStatus##HorizAlign(void); \
    void Set##propStatus##HorizAlign(LPCTSTR newVal); \
    BSTR Get##propStatus##VertiAlign(void); \
    void Set##propStatus##VertiAlign(LPCTSTR newVal); \
    LONG Get##propStatus##BeginLineMargin(void); \
    void Set##propStatus##BeginLineMargin(LONG newVal); \
    LONG Get##propStatus##LineMargin(void); \
    void Set##propStatus##LineMargin(LONG newVal); \
    LONG Get##propStatus##EndLineMargin(void); \
    void Set##propStatus##EndLineMargin(LONG newVal); \
    LONG Get##propStatus##UnderlineMargin(void); \
    void Set##propStatus##UnderlineMargin(LONG newVal); \

#define IMPLEMENT_SKIN_TEXT_STYLE_PREFIX_PROPERTIES(theClass, propStatus, memberTextStyle) \
    LONG theClass::Get##propStatus##Color(void) { \
    return memberTextStyle.Getcolor(); \
} \
    void theClass::Set##propStatus##Color(LONG newVal) { \
    memberTextStyle.Setcolor(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##BkColor(void) { \
    return memberTextStyle.GetbkColor(); \
} \
    void theClass::Set##propStatus##BkColor(LONG newVal) { \
    memberTextStyle.SetbkColor(newVal); \
    InvalidateRect(); \
} \
    BSTR theClass::Get##propStatus##BkMode(void) { \
    return memberTextStyle.GetbkMode(); \
} \
    void theClass::Set##propStatus##BkMode(LPCTSTR newVal) { \
    memberTextStyle.SetbkMode(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##FontSize(void) { \
    return memberTextStyle.GetfontSize(); \
} \
    void theClass::Set##propStatus##FontSize(LONG newVal) { \
    memberTextStyle.SetfontSize(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##FontWeight(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetfontWeight(); \
} \
    void theClass::Set##propStatus##FontWeight(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetfontWeight(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##Italic(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.Getitalic(); \
} \
    void theClass::Set##propStatus##Italic(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.Setitalic(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##Underline(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.Getunderline(); \
} \
    void theClass::Set##propStatus##Underline(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.Setunderline(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##StrikeOut(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetstrikeOut(); \
} \
    void theClass::Set##propStatus##StrikeOut(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetstrikeOut(newVal); \
    InvalidateRect(); \
} \
    BSTR theClass::Get##propStatus##FontFamily(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetfontFamily(); \
} \
    void theClass::Set##propStatus##FontFamily(LPCTSTR newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetfontFamily(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##WordBreak(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetwordBreak(); \
} \
    void theClass::Set##propStatus##WordBreak(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetwordBreak(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##EndEllipsis(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetendEllipsis(); \
} \
    void theClass::Set##propStatus##EndEllipsis(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetendEllipsis(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##NoPrefix(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetnoPrefix(); \
} \
    void theClass::Set##propStatus##NoPrefix(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetnoPrefix(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##SingleLine(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetsingleLine(); \
} \
    void theClass::Set##propStatus##SingleLine(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetsingleLine(newVal); \
    InvalidateRect(); \
} \
    BSTR theClass::Get##propStatus##HorizAlign(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GethorizAlign(); \
} \
    void theClass::Set##propStatus##HorizAlign(LPCTSTR newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SethorizAlign(newVal); \
    InvalidateRect(); \
} \
    BSTR theClass::Get##propStatus##VertiAlign(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetvertiAlign(); \
} \
    void theClass::Set##propStatus##VertiAlign(LPCTSTR newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetvertiAlign(newVal); \
    InvalidateRect(); \
} \
    VARIANT_BOOL theClass::Get##propStatus##Shadow(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.Getshadow(); \
} \
    void theClass::Set##propStatus##Shadow(VARIANT_BOOL newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.Setshadow(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##BeginLineMargin(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetbeginLineMargin(); \
} \
    void theClass::Set##propStatus##BeginLineMargin(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetbeginLineMargin(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##LineMargin(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetlineMargin(); \
} \
    void theClass::Set##propStatus##LineMargin(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetlineMargin(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##EndLineMargin(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetendLineMargin(); \
} \
    void theClass::Set##propStatus##EndLineMargin(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetendLineMargin(newVal); \
    InvalidateRect(); \
} \
    LONG theClass::Get##propStatus##UnderlineMargin(void) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    return memberTextStyle.GetunderlineMargin(); \
} \
    void theClass::Set##propStatus##UnderlineMargin(LONG newVal) { \
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
    memberTextStyle.SetunderlineMargin(newVal); \
    InvalidateRect(); \
}
