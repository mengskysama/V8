/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLAttributes.cpp
 *	
 *	Written By Gurmeet S. Kochar <gomzygotit@hotmail.com>
 *	Copyright (c) 2004. All rights reserved.
 *
 *	This code may be used in compiled form in any way you desire 
 *	(including commercial use). The code may be redistributed 
 *	unmodified by any means PROVIDING it is not sold for profit 
 *	without the authors written consent, and providing that this 
 *	notice and the authors name and all copyright notices remains 
 *	intact. However, this file and the accompanying source code may 
 *	not be hosted on a website or bulletin board without the authors 
 *	written permission.
 *	
 *	This file is provided "AS IS" with no expressed or implied warranty.
 *	The author accepts no liability for any damage/loss of business that
 *	this product may cause.
 *
 *	Although it is not necessary, but if you use this code in any of 
 *	your application (commercial or non-commercial), please INFORM me 
 *	so that I may know how useful this library is. This will encourage 
 *	me to keep updating it.
 */
#include "stdafx.h"
#include "LiteHTMLAttributes.h"  

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG

#pragma warning(push, 4)

const COLORREF CLiteHTMLElemAttr::_clrInvalid = (COLORREF)0xFFFFFFFF;
const unsigned short CLiteHTMLElemAttr::_percentMax = USHRT_MAX;

// the reason behind setting the block size of our collection 
// to 166 is that we have a total of 166 known named colors
 CLiteHTMLElemAttr::CNamedColors CLiteHTMLElemAttr::_namedColors(166 /* block size */);

void CLiteHTMLElemAttr::Init(void)
{
	if (_namedColors.GetCount())
		return;

	/** 28 system colors */
	_namedColors[_T("activeborder")]		= (COLORREF)0x8000000A;
	_namedColors[_T("activecaption")]		= (COLORREF)0x80000002;
	_namedColors[_T("appworkspace")]		= (COLORREF)0x8000000C;
	_namedColors[_T("background")]			= (COLORREF)0x80000001;
	_namedColors[_T("buttonface")]			= (COLORREF)0x8000000F;
	_namedColors[_T("buttonhighlight")]		= (COLORREF)0x80000014;
	_namedColors[_T("buttonshadow")]		= (COLORREF)0x80000010;
	_namedColors[_T("buttontext")]			= (COLORREF)0x80000012;
	_namedColors[_T("captiontext")]			= (COLORREF)0x80000009;
	_namedColors[_T("graytext")]			= (COLORREF)0x80000011;
	_namedColors[_T("highlight")]			= (COLORREF)0x8000000D;
	_namedColors[_T("highlighttext")]		= (COLORREF)0x8000000E;
	_namedColors[_T("inactiveborder")]		= (COLORREF)0x8000000B;
	_namedColors[_T("inactivecaption")]		= (COLORREF)0x80000003;
	_namedColors[_T("inactivecaptiontext")]	= (COLORREF)0x80000013;
	_namedColors[_T("infobackground")]		= (COLORREF)0x80000018;
	_namedColors[_T("infotext")]			= (COLORREF)0x80000017;
	_namedColors[_T("menu")]				= (COLORREF)0x80000004;
	_namedColors[_T("menutext")]			= (COLORREF)0x80000007;
	_namedColors[_T("scrollbar")]			= (COLORREF)0x80000000;
	_namedColors[_T("threeddarkshadow")]	= (COLORREF)0x80000015;
	_namedColors[_T("threedface")]			= (COLORREF)0x8000000F;
	_namedColors[_T("threedhighlight")]		= (COLORREF)0x80000014;
	_namedColors[_T("threedlightshadow")]	= (COLORREF)0x80000016;
	_namedColors[_T("threedshadow")]		= (COLORREF)0x80000010;
	_namedColors[_T("window")]				= (COLORREF)0x80000005;
	_namedColors[_T("windowframe")]			= (COLORREF)0x80000006;
	_namedColors[_T("windowtext")]			= (COLORREF)0x80000008;

	/** 16 basic colors */
	_namedColors[_T("black")]				= RGB(0x00, 0x00, 0x00);
	_namedColors[_T("gray")]				= RGB(0x80, 0x80, 0x80);
	_namedColors[_T("silver")]				= RGB(0xC0, 0xC0, 0xC0);
	_namedColors[_T("white")]				= RGB(0xFF, 0xFF, 0xFF);
	_namedColors[_T("yellow")]				= RGB(0xFF, 0xFF, 0x00);
	_namedColors[_T("olive")]				= RGB(0x80, 0x80, 0x00);
	_namedColors[_T("red")]					= RGB(0xFF, 0x00, 0x00);
	_namedColors[_T("maroon")]				= RGB(0x80, 0x00, 0x00);
	_namedColors[_T("fuchsia")]				= RGB(0xFF, 0x00, 0xFF);
	_namedColors[_T("purple")]				= RGB(0x80, 0x00, 0x80);
	_namedColors[_T("blue")]				= RGB(0x00, 0x00, 0xFF);
	_namedColors[_T("navy")]				= RGB(0x00, 0x00, 0x80);
	_namedColors[_T("aqua")]				= RGB(0x00, 0xFF, 0xFF);
	_namedColors[_T("teal")]				= RGB(0x00, 0x80, 0x80);
	_namedColors[_T("lime")]				= RGB(0x00, 0xFF, 0x00);
	_namedColors[_T("green")]				= RGB(0x00, 0x80, 0xFF);

	/** additional named colors */
	_namedColors[_T("darkolivegreen")]		= RGB(0x55, 0x6B, 0x2F);
	_namedColors[_T("olivedrab")]			= RGB(0x6B, 0x8E, 0x23);
	_namedColors[_T("yellowgreen")]			= RGB(0x9A, 0xCD, 0x32);
	_namedColors[_T("lawngreen")]			= RGB(0x7C, 0xFC, 0x00);
	_namedColors[_T("chartreuse")]			= RGB(0x7F, 0xFF, 0x00);
	_namedColors[_T("greenyellow")]			= RGB(0xAD, 0xFF, 0x2F);
	_namedColors[_T("palegreen")]			= RGB(0x98, 0xFB, 0x98);
	_namedColors[_T("lightgreen")]			= RGB(0x90, 0xEE, 0x90);
	_namedColors[_T("darkgreen")]			= RGB(0x00, 0x64, 0x00);
	_namedColors[_T("forestgreen")]			= RGB(0x22, 0x8B, 0x22);
	_namedColors[_T("seagreen")]			= RGB(0x2E, 0x8B, 0x57);
	_namedColors[_T("mediumseagreen")]		= RGB(0x3C, 0xB3, 0x71);
	_namedColors[_T("limegreen")]			= RGB(0x32, 0xCD, 0x32);
	_namedColors[_T("darkseagreen")]		= RGB(0x8F, 0xBC, 0x8B);
	_namedColors[_T("springgreen")]			= RGB(0x00, 0xFF, 0x7F);
	_namedColors[_T("mediumspringgreen")]	= RGB(0x00, 0xFA, 0x99);
	_namedColors[_T("darkslategray")]		= RGB(0x2F, 0x4F, 0x4F);
	_namedColors[_T("darkcyan")]			= RGB(0x00, 0x8B, 0x8B);
	_namedColors[_T("cadetblue")]			= RGB(0x5F, 0x9E, 0xA0);
	_namedColors[_T("lightseagreen")]		= RGB(0x20, 0xB2, 0xAA);
	_namedColors[_T("mediumaquamarine")]	= RGB(0x66, 0xCD, 0xAA);
	_namedColors[_T("turquoise")]			= RGB(0x40, 0xE0, 0xD0);
	_namedColors[_T("aquamarine")]			= RGB(0x7F, 0xFF, 0xD4);
	_namedColors[_T("paleturquoise")]		= RGB(0xAF, 0xEE, 0xEE);
	_namedColors[_T("slategray")]			= RGB(0x70, 0x80, 0x90);
	_namedColors[_T("lightslategray")]		= RGB(0x77, 0x88, 0x99);
	_namedColors[_T("steelblue")]			= RGB(0x46, 0x82, 0xB4);
	_namedColors[_T("deepskyblue")]			= RGB(0x00, 0xBF, 0xFF);
	_namedColors[_T("darkturquoise")]		= RGB(0x00, 0xCE, 0xD1);
	_namedColors[_T("mediumturquoise")]		= RGB(0x48, 0xD1, 0xCC);
	_namedColors[_T("powderblue")]			= RGB(0xB0, 0xE0, 0xE6);
	_namedColors[_T("lightcyan")]			= RGB(0xE0, 0xFF, 0xFF);
	_namedColors[_T("darkblue")]			= RGB(0x00, 0x00, 0x8B);
	_namedColors[_T("mediumblue")]			= RGB(0x00, 0x00, 0xCD);
	_namedColors[_T("royalblue")]			= RGB(0x41, 0x69, 0xe1);
	_namedColors[_T("dodgerblue")]			= RGB(0x1E, 0x90, 0xFF);
	_namedColors[_T("cornflowerblue")]		= RGB(0x64, 0x95, 0xED);
	_namedColors[_T("skyblue")]				= RGB(0x87, 0xCE, 0xEB);
	_namedColors[_T("lightskyblue")]		= RGB(0x87, 0xCE, 0xFA);
	_namedColors[_T("lightblue")]			= RGB(0xAD, 0xD8, 0xE6);
	_namedColors[_T("midnightblue")]		= RGB(0x19, 0x19, 0x70);
	_namedColors[_T("darkslateblue")]		= RGB(0x48, 0x3D, 0x8B);
	_namedColors[_T("blueviolet")]			= RGB(0x8A, 0x2B, 0xE2);
	_namedColors[_T("slateblue")]			= RGB(0x6A, 0x5A, 0xCD);
	_namedColors[_T("mediumslateblue")]		= RGB(0x7B, 0x68, 0xEE);
	_namedColors[_T("mediumpurple")]		= RGB(0x93, 0x70, 0xDB);
	_namedColors[_T("lightsteelblue")]		= RGB(0xB0, 0xC4, 0xDE);
	_namedColors[_T("lavender")]			= RGB(0xE6, 0xE6, 0xFA);
	_namedColors[_T("indigo")]				= RGB(0x4B, 0x00, 0x82);
	_namedColors[_T("darkviolet")]			= RGB(0x94, 0x00, 0xD3);
	_namedColors[_T("darkorchid")]			= RGB(0x99, 0x32, 0xCC);
	_namedColors[_T("mediumorchid")]		= RGB(0xBA, 0x55, 0xD3);
	_namedColors[_T("orchid")]				= RGB(0xDA, 0x70, 0xD6);
	_namedColors[_T("violet")]				= RGB(0xEE, 0x82, 0xEE);
	_namedColors[_T("plum")]				= RGB(0xDD, 0xA0, 0xDD);
	_namedColors[_T("thistle")]				= RGB(0xD8, 0xDF, 0xD8);
	_namedColors[_T("darkmagenta")]			= RGB(0x8B, 0x00, 0x8B);
	_namedColors[_T("mediumvioletred")]		= RGB(0xC7, 0x15, 0x85);
	_namedColors[_T("deeppink")]			= RGB(0xFF, 0x14, 0x93);
	_namedColors[_T("palmvioletred")]		= RGB(0xDB, 0x70, 0x93);
	_namedColors[_T("hotpink")]				= RGB(0xFF, 0x69, 0xB4);
	_namedColors[_T("lightpink")]			= RGB(0xFF, 0xB6, 0xC1);
	_namedColors[_T("pink")]				= RGB(0xFF, 0xC0, 0xCB);
	_namedColors[_T("mistyrose")]			= RGB(0xFF, 0xE4, 0xE1);
	_namedColors[_T("brown")]				= RGB(0xA5, 0x2A, 0x2A);
	_namedColors[_T("indianred")]			= RGB(0xCD, 0x5C, 0x5C);
	_namedColors[_T("rosybrown")]			= RGB(0xBC, 0x8F, 0x8F);
	_namedColors[_T("salmon")]				= RGB(0xFA, 0x80, 0x72);
	_namedColors[_T("lightcoral")]			= RGB(0xF0, 0x80, 0x80);
	_namedColors[_T("darksalmon")]			= RGB(0xE9, 0x96, 0x7A);
	_namedColors[_T("lightsalmon")]			= RGB(0xFF, 0xA0, 0x7A);
	_namedColors[_T("peachpuff")]			= RGB(0xFF, 0xDA, 0xB9);
	_namedColors[_T("darkred")]				= RGB(0x8B, 0x00, 0x00);
	_namedColors[_T("firebrick")]			= RGB(0xB2, 0x22, 0x22);
	_namedColors[_T("crimson")]				= RGB(0xDC, 0x14, 0x3C);
	_namedColors[_T("orangered")]			= RGB(0xFF, 0x45, 0x00);
	_namedColors[_T("tomato")]				= RGB(0xFF, 0x63, 0x47);
	_namedColors[_T("coral")]				= RGB(0xFF, 0x7F, 0x50);
	_namedColors[_T("wheat")]				= RGB(0xF5, 0xDE, 0xB3);
	_namedColors[L"papayawhip"]			= RGB(0xFF, 0xEF, 0xD5);
	_namedColors[L"sienna"]				= RGB(0xA0, 0x52, 0x2D);
	_namedColors[L"chocolate"]			= RGB(0xD2, 0x69, 0x1E);
	_namedColors[L"darkorange"]			= RGB(0xFF, 0x8C, 0x00);
	_namedColors[L"sandybrown"]			= RGB(0xF4, 0xA4, 0x60);
	_namedColors[L"orange"]				= RGB(0xFF, 0xA5, 0x00);
	_namedColors[L"navajowhite"]			= RGB(0xFF, 0xDE, 0xAD);
	_namedColors[L"moccasin"]			= RGB(0xFF, 0xE4, 0xB5);
	_namedColors[L"saddlebrown"]			= RGB(0x8B, 0x45, 0x13);
	_namedColors[L"peru"]				= RGB(0xCD, 0x85, 0x3F);
	_namedColors[L"burlywood"]			= RGB(0xDE, 0xB8, 0x87);
	_namedColors[L"tan"]					= RGB(0xD2, 0xB4, 0x8C);
	_namedColors[L"bisque"]				= RGB(0xFF, 0xE4, 0xC4);
	_namedColors[L"blanchedalmond"]		= RGB(0xFF, 0xEB, 0xCD);
	_namedColors[L"antiquewhite"]		= RGB(0xFA, 0xEB, 0xD7);
	_namedColors[L"darkgoldenrod"]		= RGB(0xB8, 0x86, 0x0B);
	_namedColors[L"goldenrod"]			= RGB(0xDA, 0xA5, 0x20);
	_namedColors[L"darkkhaki"]			= RGB(0xBD, 0xB7, 0x6B);
	_namedColors[L"gold"]				= RGB(0xFF, 0xD7, 0x00);
	_namedColors[L"khaki"]				= RGB(0xF0, 0xE6, 0x8C);
	_namedColors[L"palegoldenrod"]		= RGB(0xEE, 0xE8, 0xAA);
	_namedColors[L"lemonchiffon"]		= RGB(0xFF, 0xFA, 0xCD);
	_namedColors[L"beige"]				= RGB(0xF5, 0xF5, 0xDC);
	_namedColors[L"lightgoldenrodyellow"]= RGB(0xFA, 0xFA, 0xD2);
	_namedColors[L"lightyellow"]			= RGB(0xFF, 0xFF, 0xE0);
	_namedColors[L"ivory"]				= RGB(0xFF, 0xFF, 0x00);
	_namedColors[L"cornsilk"]			= RGB(0xFF, 0xF8, 0xDC);
	_namedColors[L"oldlace"]				= RGB(0xFD, 0xF5, 0xE6);
	_namedColors[L"florawhite"]			= RGB(0xFF, 0xFA, 0xF0);
	_namedColors[L"honeydew"]			= RGB(0xF0, 0xFF, 0xF0);
	_namedColors[L"mintcream"]			= RGB(0xF5, 0xFF, 0xFA);
	_namedColors[L"azure"]				= RGB(0xF0, 0xFF, 0xFF);
	_namedColors[L"ghostwhite"]			= RGB(0xF8, 0xF8, 0xFF);
	_namedColors[L"linen"]				= RGB(0xFA, 0xF0, 0xE6);
	_namedColors[L"seashell"]			= RGB(0xFF, 0xF5, 0xEE);
	_namedColors[L"snow"]				= RGB(0xFF, 0xFA, 0xFA);
	_namedColors[L"dimgray"]				= RGB(0x69, 0x69, 0x69);
	_namedColors[L"darkgray"]			= RGB(0xA9, 0xA9, 0xA9);
	_namedColors[L"lightgray"]			= RGB(0xD3, 0xD3, 0xD3);
	_namedColors[L"gainsboro"]			= RGB(0xDC, 0xDC, 0xDC);
	_namedColors[L"whitesmoke"]			= RGB(0xF5, 0xF5, 0xF5);
	_namedColors[L"ghostwhite"]			= RGB(0xF8, 0xF8, 0xFF);
	_namedColors[L"aliceblue"]			= RGB(0xF0, 0xF8, 0xFF);
}


bool CLiteHTMLElemAttr::isNamedColorValue(void) const
{
	if ( (m_strAttrValue.GetLength()) && (::_istalpha(m_strAttrValue[0])) )
	{
		COLORREF crTemp = _clrInvalid;
		CString		strKey(m_strAttrValue);

		strKey.MakeLower();
		if (_namedColors.Lookup(m_strAttrValue, crTemp))
			return (true);
	}
	return (false);
}

bool CLiteHTMLElemAttr::isSysColorValue(void) const
{
	if ( (m_strAttrValue.GetLength()) && (::_istalpha(m_strAttrValue[0])) )
	{
		COLORREF	crTemp = _clrInvalid;
		CString		strKey(m_strAttrValue);

		strKey.MakeLower();
		if (_namedColors.Lookup(strKey, crTemp))
			return (crTemp >= 0x80000000 && crTemp <= 0x80000018);
	}
	return (false);
}

COLORREF CLiteHTMLElemAttr::getColorValue(void) const
{
	COLORREF crTemp = _clrInvalid;
	if (isNamedColorValue())
	{
		CString	strKey(m_strAttrValue);
		strKey.MakeLower();
		if (_namedColors.Lookup(strKey, crTemp))
		{
			// is this a system named color value?
			if (crTemp >= 0x80000000 && crTemp <= 0x80000018)
				crTemp = ::GetSysColor(crTemp & 0x7FFFFFFF);
		}
	}
	else if (isHexColorValue())
		crTemp = ::_tcstoul(m_strAttrValue.Mid(1), NULL, 16);
	return (crTemp);
}
CString CLiteHTMLElemAttr::getColorHexValue(void) const
{
	CString	strColorHex;
	if (isHexColorValue())
		strColorHex = m_strAttrValue.Mid(1);
	else
	{
		COLORREF crTemp = getColorValue();
		if (crTemp != _clrInvalid)
			strColorHex.Format(_T("#%06x"), crTemp);
	}
	return (strColorHex);
}

#pragma warning(pop)
