/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLAttributes.h - Defines CLiteHTMLAttributes and 
 *	                       CLiteHTMLElemAttr
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
 *
 *	HISTORY:
 *
 *	Version 1.0				Gurmeet S. Kochar (GSK)
 *	Mar 18, 2004			First release version.
 */
#ifndef __LITEHTMLATTRIBUTES_H__
#define __LITEHTMLATTRIBUTES_H__

/*
 * Conditional Includes
 */
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif	// !__AFXTEMPL_H__

#ifndef __LITEHTMLCOMMON_H__
#	include "LiteHTMLCommon.h"
#endif	// !__LITEHTMLCOMMON_H__

#ifndef __LITEHTMLENTITYRESOLVER_H__
#	include "LiteHTMLEntityResolver.h"
#endif	// !__LITEHTMLENTITYRESOLVER_H__

#pragma warning(push, 4)
#pragma warning (disable : 4290)	// C++ Exception Specification ignored

class CLiteHTMLAttributes;	// forward declaration

/**
 * CLiteHTMLElemAttr
 *
 * @version 1.0 (Mar 18, 2004)
 * @author Gurmeet S. Kochar
 */
class   CLiteHTMLElemAttr
{
	// Friends
	friend class CLiteHTMLAttributes;

// Constructors
public:
	CLiteHTMLElemAttr(LPCTSTR lpszAttribName = NULL, 
					  LPCTSTR lpszAttribValue = NULL)
	{
		Init();
		m_strAttrName = lpszAttribName;
		m_strAttrValue = lpszAttribValue;
	}

	CLiteHTMLElemAttr(const CLiteHTMLElemAttr &rSource)
	{
		Init();
		m_strAttrName = rSource.m_strAttrName;
		m_strAttrValue = rSource.m_strAttrValue;
	}

// Initialization Helpers
private:
	static __declspec(dllexport)  void Init(void);
// Attributes
public:
	/**
	 * @return name of this CLiteHTMLElemAttr
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getName(void) const
		{ return (m_strAttrName); }

	/**
	 * @return value of this CLiteHTMLElemAttr
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getValue(void) const
		{ return (m_strAttrValue); }

	/**
	 * Determines if the attribute value is a named color value
	 * @return true if attribute value is a named color, otherwise, false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isNamedColorValue(void)const;
	/**
	 * Determines if the attribute value is a named system color value
	 * @return true if value is a named system color, false otherwise
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isSysColorValue(void)const;
	/**
	 * Determines if the attribute value is a color value in 
	 * hexadecimal format
	 * @return true if attribute value is a color value, otherwise, false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isHexColorValue(void) const
	{
		// zero-length attribute value?
		if (m_strAttrValue.IsEmpty())
			return (false);

		if (m_strAttrValue[0] == _T('#'))
		{
			if (m_strAttrValue.GetLength() > 1)
			{
				for (int i = 1; i < m_strAttrValue.GetLength(); i++)
				{
					if (!::_istxdigit(m_strAttrValue[i]))
						return (false);
				}
				return (true);
			}
		}

		return (false);
	}

	/**
	 * Determines if the attribute value contains a color reference
	 * @return true, if attribute value is color value, false otherwise.
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isColorValue(void) const
		{ return (isNamedColorValue() || isHexColorValue()); }

	/**
	 * Returns the color value of the attribute
	 * @return a COLORREF representing the color
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	COLORREF getColorValue(void)const;
	
	/**
	 * Returns the RGB value of the attribute in hexadecimal format
	 * @return hexadecimal string representing the color value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getColorHexValue(void)const;
	/**
	 * Checks to see if the attribute contains a percent value
	 * @return true if value is a percent value, otherwise, false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isPercentValue(void) const
		{ return (m_strAttrValue.Right(1) == _T("%") ? true : false); }

	/**
	 * Returns a percent value of the attribute
	 * @return percentage value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	unsigned short getPercentValue(unsigned short max = _percentMax) const
	{
		ASSERT(max > 0);
		if (!isPercentValue())	return (0);
		unsigned short	percentVal = (unsigned short)((short)*this);
		return ((percentVal > max ? max : percentVal));
	}
	
	/**
	 * Parses a length value from the attribute/value 
	 * and identifies its length unit also
	 *
	 * @param rUnit - this will receive the type of the length unit
	 *
	 * @return an integer value of the attribute 
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	enum LengthUnitsEnum { em, ex, px, per, in, cm, mm, pt, pc };
	short getLengthValue(LengthUnitsEnum &rUnit) const
	{
		static const TCHAR	_szUnits[][4] = 
		{
			/** relative length units */
			_T("em"), _T("ex"), _T("px"), _T("%"), 
			/** absolute length units */
			_T("in"), _T("cm"), _T("mm"), _T("pt"), _T("pc") 
		};

		if (m_strAttrValue.IsEmpty())
			return (0);

		int i = 0;
		for (; i < sizeof(_szUnits)/sizeof(_szUnits[0]); i++)
		{
			if (m_strAttrValue.Right(::lstrlen(_szUnits[i])). \
				CompareNoCase(_szUnits[i]) == 0)
			{
				rUnit = (LengthUnitsEnum)i;
				break;
			}
		}
		if (i == sizeof(_szUnits)/sizeof(_szUnits[0]))
			return (0);
		return (*this);
	}

// Operators
public:
	/**
	 * Converts attribute value to bool
	 * @return true or false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator bool() const
	{
		if (!m_strAttrValue.CompareNoCase(_T("true")))
			return (true);
		if (!m_strAttrValue.CompareNoCase(_T("false")))
			return (false);
		return (((short)*this ? true : false));
	}

	/**
	 * Converts attribute value to BYTE (unsigned char)
	 * @return the left-most character of attribute value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator BYTE() const
		{ return ((BYTE)(m_strAttrValue.GetLength() ? m_strAttrValue[0] : 0)); }
	
	/**
	 * Converts attribute value to double
	 * @return 0.00 on failure, otherwise, a numeric value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator double() const
		{ return (::_tcstod(m_strAttrValue, NULL)); }
	
	/**
	 * Converts attribute value to signed short int
	 * @return 0 on failure, otherwise, an integer value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator short() const
		{ return ((short)::_ttoi(m_strAttrValue)); }
	
	/**
	 * @return attribute value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator LPCTSTR() const
		{ return (m_strAttrValue); }

// Private Operations
private:
	/**
	 * Sets the value of an attribute. Takes care of the following:
	 *  1. Ignores leading and trailing white-space characters
	 *  2. Replaces character entities with appropriate characters.
	 *  3. Ignores line feeds (LF).
	 *  4. Replaces each carriage-return (CR) or tab with a single space.
	 *
	 * @param lpszValue - new attribute value
	 * 
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	void putValue(LPCTSTR lpszValue)
	{
		ASSERT(AfxIsValidString(lpszValue));

		m_strAttrValue = lpszValue;
		
		// ignore leading white-spaces
		m_strAttrValue.TrimLeft();

		// ignore trailing white-spaces
		m_strAttrValue.TrimRight();

		// ignore line feeds
		m_strAttrValue.Remove(_T('\n'));

		// replace tab and carriage-return with a single space
		m_strAttrValue.Replace(_T('\r'), _T(' '));
		m_strAttrValue.Replace(_T('\t'), _T(' '));

		/** resolve entity reference(s) */
		int		iCurPos = -1, iParseLen = 0;
		TCHAR	chSubst = 0;
		do
		{
			if ((iCurPos = m_strAttrValue.Find(_T('&'), ++iCurPos)) == -1)
				break;
			
			iParseLen = CLiteHTMLEntityResolver::resolveEntity(m_strAttrValue.Mid(iCurPos), chSubst);
			if (iParseLen)
			{
				m_strAttrValue.Replace
				(
					m_strAttrValue.Mid(iCurPos, iParseLen), 
					CString(chSubst)
				);
			}
		}
		while (true);
	}

// Parsing Helpers
public:
	// parses an attribute/value pair from the given string
	UINT parseFromStr(LPCTSTR lpszString);

// Data Members
public:
	static const COLORREF		_clrInvalid;	// an invalid color
	static const unsigned short	_percentMax;	// maximum allowable percentage value
private:
	typedef CMap<CString, LPCTSTR, COLORREF, COLORREF>	CNamedColors;

	static CNamedColors		__declspec(dllexport)  _namedColors;	// collection of named colors
	CString					m_strAttrName,  // attribute name
							m_strAttrValue; // attribute value
};

/**
 * CLiteHTMLElemAttr::parseFromStr
 *
 * @param lpszString - string to parse
 *
 * @return number of TCHARs successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
inline UINT CLiteHTMLElemAttr::parseFromStr(LPCTSTR lpszString)
{
	ASSERT(AfxIsValidString(lpszString));

	LPCTSTR	lpszBegin = lpszString;
	LPCTSTR	lpszEnd;
	TCHAR	ch = 0;

	// skip leading white-space characters
	while (::_istspace(*lpszBegin))
		lpszBegin = ::_tcsinc(lpszBegin);

	// name doesn't begin with an alphabet?
	if (!::_istalpha(*lpszBegin))
		return (0U);

	lpszEnd = lpszBegin;
	do
	{
		// attribute name may contain letters (a-z, A-Z), digits (0-9), 
		// underscores '_', hyphen '-', colons ':', and periods '.'
		if ( (!::_istalnum(*lpszEnd)) && 
			 (*lpszEnd != _T('-')) && (*lpszEnd != _T(':')) && 
			 (*lpszEnd != _T('_')) && (*lpszEnd != _T('.')) )
		{
			ASSERT(lpszEnd != lpszBegin);

			// only white-space characters, a null-character, an 
			// equal-sign, a greater-than symbol, or a forward-slash 
			// can act as the separator between an attribute and its 
			// value
			if (*lpszEnd == NULL || ::_istspace(*lpszEnd) || 
				*lpszEnd == _T('=') || 
				*lpszEnd == _T('>') || *lpszEnd == _T('/'))
			{
				break;
			}

			return (0U);	// any other character will fail parsing process
		}

		lpszEnd = ::_tcsinc(lpszEnd);
	}
	while (true);

	// extract attribute name
	CString	strAttrName(lpszBegin, lpszEnd - lpszBegin);
	
	if (*lpszEnd != _T('='))
	{
		m_strAttrName = strAttrName;
		m_strAttrValue.Empty();
		return (lpszEnd - lpszString);
	}
	else
	{
		// skip white-space characters after equal-sign 
		// and the equal-sign itself
		do {
			lpszEnd = ::_tcsinc(lpszEnd);
		} while (::_istspace(*lpszEnd));

		lpszBegin = lpszEnd;
		ch = *lpszEnd;
		
		// is attribute value wrapped in quotes?
		if (ch == _T('\'') || ch == _T('\"'))
		{
			lpszBegin = ::_tcsinc(lpszBegin);	// skip quote symbol
			do
			{
				lpszEnd = ::_tcsinc(lpszEnd);
			}
			// Loop until we find the same quote character that 
			// was used at the starting of the attribute value.
			// Anything within these quotes is considered valid!
			// NOTE that the entity references are resolved later.
			while (*lpszEnd != NULL && *lpszEnd != ch);
		}

		// open attribute value i.e. not wrapped in quotes?
		else
		{
			do 
			{ 
				lpszEnd = ::_tcsinc(lpszEnd);
			}
			// loop until we find a tag ending delimeter or any 
			// white-space character, or until we reach at the 
			// end of the string buffer
			while (*lpszEnd != NULL && !::_istspace(*lpszEnd) && 
				   *lpszEnd != _T('/') && *lpszEnd != _T('>'));
		}

		m_strAttrName = strAttrName;
		if (lpszEnd == lpszBegin)	// empty attribute value?
			m_strAttrValue.Empty();
		else
			// use putValue() instead of direct assignment; 
			// this will automatically normalize data before 
			// assigning according to the specs and will 
			// also resolve entity references!!!
			putValue(CString(lpszBegin, lpszEnd - lpszBegin));

		// calculate and return the count of characters successfully parsed
		return ((lpszEnd - lpszString) + 
				(ch == _T('\'') || ch == _T('\"') ? 1 : 0) );
	}

	return (0U);
}

/**
 * CLiteHTMLAttributes
 *
 * @version 1.0 (Mar 18, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLAttributes
{
// Construction/Destruction
public:
	CLiteHTMLAttributes() 
		: m_parrAttrib(NULL)
	{ }

	/**
	 * @param bCopy - if true, this CLiteHTMLAttributes makes a copy 
	 * of the encapsulated pointer. if false, this constructor takes 
	 * ownership of the encapsulated pointer.
	 *
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLAttributes(CLiteHTMLAttributes &rSource, bool bCopy = false) throw(CMemoryException) 
		: m_parrAttrib(NULL)
	{
		if (!bCopy)
		{
			m_parrAttrib = rSource.m_parrAttrib;
			rSource.m_parrAttrib = NULL;
		}
		else
		{
			const int	nElemCount = rSource.getCount();
			if (nElemCount)
			{
				if ((m_parrAttrib = new CElemAttrArray) == NULL)
					AfxThrowMemoryException();
				
				CLiteHTMLElemAttr	*pItem = NULL;
				m_parrAttrib->SetSize(nElemCount);
				
				/** DEEP COPY BEGIN */
				for (int iElem = 0; iElem < nElemCount; iElem++)
				{
					if ((pItem = new CLiteHTMLElemAttr(rSource[iElem])) == NULL)
					{
						removeAll();
						AfxThrowMemoryException();
						return;
					}
					
					(*m_parrAttrib)[iElem] = pItem;
					pItem = NULL;
				}
				/** DEEP COPY END */
			}
		}
	}

	virtual ~CLiteHTMLAttributes()
		{ removeAll(); }

// Initialization
public:
	// parses attribute/value pairs from the given string
	UINT parseFromStr(LPCTSTR lpszString);

// Attributes
public:
	/**
	 * Returns the count of CLiteHTMLElemAttr items in this collection
	 * @return number of items
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	int getCount(void) const
	{
		if (m_parrAttrib != NULL)
			return (m_parrAttrib->GetUpperBound() + 1);
		return (0);
	}

	/**
	 * Look up the index of an attribute given its name.
	 * If more than one attribute with the same name exist, 
	 * this will return the index of the first match.
	 *
	 * @param lpszAttributeName - name of the attribute
	 *
	 * @return zero-based index of an attribute, or -1 if not found
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	int getIndexFromName(LPCTSTR lpszAttributeName) const
	{
		ASSERT(AfxIsValidString(lpszAttributeName));
		CLiteHTMLElemAttr	*pItem = NULL;
		for (int iElem = 0; iElem < getCount(); iElem++)
		{
			if ((pItem = (*m_parrAttrib)[iElem]) == NULL)	// just in case
				continue;
			
			// perform a CASE-INSENSITIVE search
			if (pItem->m_strAttrName.CompareNoCase(lpszAttributeName) == 0)
				return (iElem);
		}
		return (-1);
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute's index
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr operator[](int nIndex) const
	{
		if (!(nIndex >= 0 && nIndex < getCount()))
		{
			//ASSERT(FALSE);
			return (CLiteHTMLElemAttr());
		}
		return ( *((*m_parrAttrib)[nIndex]) );
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute name
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr operator[](LPCTSTR lpszIndex) const
	{
		ASSERT(AfxIsValidString(lpszIndex));
		return ((*this)[getIndexFromName(lpszIndex)]);
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute's index
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr getAttribute(int nIndex) const
		{ return ((*this)[nIndex]); }

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute name
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr getAttribute(LPCTSTR lpszIndex) const
	{
		ASSERT(AfxIsValidString(lpszIndex));
		return ((*this)[getIndexFromName(lpszIndex)]);
	}

	/**
	 * Returns the name of an attribute given its index
	 *
	 * @return name of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getName(int nIndex) const
		{ return ((*this)[nIndex].m_strAttrName); }

	/**
	 * Returns the value of an attribute given its index
	 *
	 * @return value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getValue(int nIndex) const
		{ return ((*this)[nIndex].m_strAttrValue); }

	/**
	 * Returns the value of an attribute given its name
	 *
	 * @return value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getValueFromName(LPCTSTR lpszAttributeName) const
		{ return ((*this)[lpszAttributeName].m_strAttrValue); }

// Operations
public:
	/**
	 * Adds a new CLiteHTMLElemAttr item to the collection
	 *
	 * @param lpszName - attribute name (serves as the key to the item)
	 * @param lpszValue - attribute value
	 * @param bReplaceOld - If an item with the same name as specified 
	 *        by lpszName already exists in the collection, this 
	 *        parameter is used to determine whether to replace the 
	 *        existing item or add a new one
	 *
	 * @return pointer to a CLiteHTMLElemAttr
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr* addAttribute(LPCTSTR lpszName, LPCTSTR lpszValue)
	{
		ASSERT(AfxIsValidString(lpszName));
		ASSERT(AfxIsValidString(lpszValue));

		CLiteHTMLElemAttr	*pItem = new CLiteHTMLElemAttr(lpszName, lpszValue);
		if (pItem != NULL)
		{
			if (m_parrAttrib == NULL)
			{
				if ((m_parrAttrib = new CElemAttrArray) == NULL)
				{
					SAFE_DELETE_POINTER(pItem);
					TRACE0("(Error) CLiteHTMLAttributes::addAttribute: Out of memory.\n");
					return (NULL);
				}
			}
			
			VERIFY(m_parrAttrib->Add(pItem) >= 0);
		}
		return (pItem);
	}
	
	/**
	 * Removes an CLiteHTMLElemAttr item from the collection
	 *
	 * @param lpszName - attribute to remove
	 *
	 * @return true if successful, false otherwise
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool removeAttribute(int nIndex)
	{
		if (!(nIndex >= 0 && nIndex < getCount()))
			return (false);
		CLiteHTMLElemAttr	*pItem = NULL;
		VERIFY(((*m_parrAttrib)[nIndex]) != NULL);
		SAFE_DELETE_POINTER(pItem);
		return (true);
	}
	
	/**
	 * Removes all CLiteHTMLElemAttr items from the collection
	 * @return true if successful, false otherwise
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool removeAll(void)
	{
		CLiteHTMLElemAttr	*pItem = NULL;
		for (int iElem = 0; iElem < getCount(); iElem++)
		{
			VERIFY((pItem = (*m_parrAttrib)[iElem]) != NULL);
			SAFE_DELETE_POINTER(pItem);
		}
		SAFE_DELETE_POINTER(m_parrAttrib);
		return (true);
	}

// Data Members
private:
	typedef CTypedPtrArray<CPtrArray, CLiteHTMLElemAttr*>	CElemAttrArray;
	CElemAttrArray	*m_parrAttrib;	// array of attributes/value pairs
};

/**
 * CLiteHTMLAttributes::parseFromStr
 *
 * @param lpszString - string to parse. It can contain pairs such as: 
 *
 *          1. NAME
 *          2. NAME=VALUE
 *          3. NAME='VALUE'
 *          4. NAME="VALUE"
 *
 *        NAME consist of letters, digits, underscores, 
 *        colons, hyphens, and periods
 *
 *        NOTE that white-spaces between NAME and equal-sign AND 
 *        equal-sign and VALUE is allowed.
 *
 * @return number of TCHARs successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
inline UINT CLiteHTMLAttributes::parseFromStr(LPCTSTR lpszString)
{
	ASSERT(AfxIsValidString(lpszString));

	CElemAttrArray		*pcoll = NULL;
	CLiteHTMLElemAttr	oElemAttr;
	const UINT			nStrLen = ::_tcslen(lpszString);
	UINT				nRetVal = 0U, 
						nTemp = 0U;

	do
	{
		// try to parse an attribute/value 
		// pair from the rest of the string
		if (!(nTemp = oElemAttr.parseFromStr(&lpszString[nRetVal])))
		{
			if (!nRetVal)
				goto LError;
			break;
		}

		// collection has not been instantiated until now?
		if (pcoll == NULL)
		{
			// instantiate now
			if ((pcoll = new CElemAttrArray) == NULL)
				// out of memory?
			{
				TRACE0("(Error) CLiteHTMLAttributes::parseFromStr: Out of memory.\n");
				goto LError;
			}
		}

		// add attribute/value pair to collection
		if (pcoll->Add(new CLiteHTMLElemAttr(oElemAttr)) < 0)
			goto LError;

		// advance seek pointer
		nRetVal += nTemp;
	}
	
	// do we still have something in the buffer to parse?
	while (nRetVal < nStrLen);

	// collection was never instantiated?
	if (pcoll == NULL)
		goto LError;
	// collection is empty?
	if (pcoll->GetUpperBound() == -1)
		goto LError;
	// current collection could not be emptied?
	if (!removeAll())
		goto LError;

	m_parrAttrib = pcoll;
	pcoll = NULL;
	goto LCleanExit;	// success!

LError:
	SAFE_DELETE_POINTER(pcoll);
	nRetVal = 0U;

LCleanExit:
	return (nRetVal);
}

#pragma warning(default : 4290)
#pragma warning(pop)

#endif	// !__LITEHTMLATTRIBUTES_H__
