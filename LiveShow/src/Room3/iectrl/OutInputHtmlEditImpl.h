#pragma once 
#include "..\..\HTMLEdit\HtmlEditCore2.h"

class COutInputHtmlEditImpl:public HtmlEditImpl
{
public:
	COutInputHtmlEditImpl() ;
	virtual ~COutInputHtmlEditImpl() ;
	virtual void InsertText(CString s, bool bText = false);
	virtual void GetContent(CString & content);
};