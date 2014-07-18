#pragma once
#include "..\..\HTMLEdit/HtmlEditLogic.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>
#include <boost/format.hpp>

using namespace boost::xpressive;

class CSpeakerDlg ;

class CHtmlInputLogic :
	public HtmlEditLogic
{
public:
	CHtmlInputLogic(int iEditable = EDITABLE_INPUT);
	virtual ~CHtmlInputLogic(void);

public:
	virtual bool OnInput(HtmlEditImpl* pHtmlEditImp);
	void OnEditCopy(HtmlEditImpl* pHtmlEditImp) ;		//复制
	void OnSendMessage(HtmlEditImpl* pHtmlEditImp);		//按 CTRL+ENTER 键发送消息

	void GetOutputString(CString& strText, CString& strImgText);
	void ReplaceImgForInput(CString& strText) ;
	void MsgFormatInput(CString& strText) ;
	void ClearInput() ;			//清屏
	uint32 GetPictureNum(CString& strText);
	uint32 GetPictureNum() ;  //获取输入框中图片数量
	void OnOutputReady(HtmlEditImpl* pHtmlEditImp);
	void TranslateMsgForLimit(CString& strText) ;
	void SetInitText( CString& strText ) ;
public:
	void SetFontSize(UINT fontsize ) ;
	void SetFontBold( int checked ) ;
	void SetFontItalic( int checked ) ;
	void SetFontUnderLine( int checked )  ;
	void SetFontName(CString& strFontName ) ;
	void SetFontColor(COLORREF cf ) ;

	MessageFont GetFontInfo() ;		//得到字体信息(大小、颜色、字体名)
private:
	wsregex m_regex[5];
	CString m_strInitText ;
};
