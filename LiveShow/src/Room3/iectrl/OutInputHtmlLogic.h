#pragma once
#include "..\..\HTMLEdit/HtmlEditLogic.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>
#include <boost/format.hpp>

class CRoomDlg ;

class COutInputHtmlLogic:
	public HtmlEditLogic
{
public:
	COutInputHtmlLogic(CRoomDlg* pRoomDlg );
	virtual ~COutInputHtmlLogic(void);
	void ClearOutput() ;	
private:
	CRoomDlg* m_pRoomDlg ;
};