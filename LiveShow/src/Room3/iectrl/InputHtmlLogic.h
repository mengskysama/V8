#pragma once
#include "..\..\HTMLEdit/HtmlEditLogic.h"
#include "..\GlobalRoomDefine.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>
#include <boost/format.hpp>

using namespace boost::xpressive;

class CRoomParentDlg ;

class CInputHtmlLogic :
	public HtmlEditLogic
{
public:
	CInputHtmlLogic( CRoomParentDlg* pParentDlg , ENM_HTML_TYPE nHtmlType );
	virtual ~CInputHtmlLogic(void);

public:
	bool OnInput(HtmlEditImpl* pHtmlEditImp);
	void OnEditCopy(HtmlEditImpl* pHtmlEditImp) ;		//复制
	void OnSendMessage(HtmlEditImpl* pHtmlEditImp);		//按 CTRL+ENTER 键发送消息

	void GetOutputString(CString& strText, CString& strImgText);
	void GetOutputStringWithoutPic(CString & strText);
	void ReplaceImgForInput(CString& strText) ;
	void MsgFormatInput(CString& strText) ;
	void ClearInput() ;			//清屏
	uint32 GetPictureNum(CString& strText);	
	uint32 GetPictureNum() ;
	uint32 GetContentLen() ;
	void TranslateMsgForLimit(CString& strText) ;
	void OnOutputReady(HtmlEditImpl* pHtmlEditImp);
	virtual void OnTrackHrefLinkMenu( uint32 unDstUin ) ;
public:
	void SetFontSize(UINT fontsize ) ;
	void SetFontBold( int checked ) ;
	void SetFontItalic( int checked ) ;
	void SetFontUnderLine( int checked )  ;
	void SetFontName(CString& strFontName ) ;
	void SetFontColor(COLORREF cf ) ;

	MessageFont GetFontInfo() ;		//得到字体信息(大小、颜色、字体名)
private:
	CRoomParentDlg* m_pParentDlg ;
	wsregex m_regex[5];
	ENM_HTML_TYPE m_nHtmlType ;
};
