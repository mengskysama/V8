#pragma once

#include "../CommonLibrary/include/ui/MenuEx.h"
#include "globaluidefine.h"
#include "coredefine.h"
using namespace core ;
enum HTMLEDITIMPL
{
	HTMLEDITIMPL_EDIT_GET_FOCUS = (WM_USER+700),
	HTMLEDITIMPL_EDIT_COPY_WORD,			//复制文字
	HTMLEDITIMPL_EDIT_CLEAN_SCREEN,			//清屏
	HTMLEDITIMPL_EDIT_PASTE,				//粘贴
};

class HtmlEditImplSetting
{
	friend class HtmlEditImpl;
public:
	HtmlEditImplSetting()
	{
		m_stChatFont.font_type = _T("宋体") ;
		m_stChatFont.font_size = 12;
		m_stChatFont.font_style = 0;
		m_stChatFont.font_color = RGB(0,0,0);

		m_nEditable = EDITABLE_INPUT;

	/*	m_strVcolorScroll=_T("#E0F2FF");
		m_strVcolorScrollBorder=_T("#519BFF");
		m_strVcolorScrollArrow=_T("#0141C7");*/

		m_strVcolorScrollTrackColor = _T("E6E6E6");
		m_strVcolorScroll3dLightColor = _T("4A85BD");
		m_strVcolorScrollArrowColor = _T("3465A4");
		m_strVcolorScrollShadowColor = _T("E7EFF7");
		m_strVcolorScrollFaceColor = _T("7DA6D7");
		m_strVcolorScrollHighlightColor = _T("E7EFF7");
		m_strVcolorScrollDarkShadowColor = _T("3465A4");

		m_strDocument = _T("about:blank");

		m_bEnableContextMenu = false;
		m_bDefaultScrollStyle = false;
		m_bMale = false;
		m_keySend = KEY_ENTER_CTRL;				//add by xiaoyi 默认为Ctrl+Enter表示发送
		m_bTribe = false;

	}

	~HtmlEditImplSetting()
	{
		m_menuText.DestroyMenu();
	}

	void Construct()
	{
		m_menuText.CreatePopupMenu();
		m_menuText.SetWidth(180);		
		//m_nEditable 1、2为可编辑
		CString strTemp;
		strTemp.Format( _T("%s"),_T("复制文字(&C)")) ;
		m_menuText.AppendMenuEx( MF_STRING , HTMLEDITIMPL_EDIT_COPY_WORD , strTemp ) ;
		if ( m_nEditable != EDITABLE_OUT )
		{ //输出框,粘贴不可用
			strTemp.Format( _T("%s") , _T("粘贴(&V)") ) ;
			m_menuText.AppendMenuEx( MF_STRING , HTMLEDITIMPL_EDIT_PASTE , strTemp ) ;
		}
		strTemp.Format(_T("%s"),_T("清屏"), strTemp ) ;
		m_menuText.AppendMenuEx( MF_STRING , HTMLEDITIMPL_EDIT_CLEAN_SCREEN , strTemp ) ;

	}

	HtmlEditImplSetting(HtmlEditImplSetting & r)
	{
		m_nSelfUin = r.m_nSelfUin;

		m_nEditable = r.m_nEditable;

		m_stChatFont.font_type= r.m_stChatFont.font_type;

	/*	m_strVcolorScroll = r.m_strVcolorScroll;
		m_strVcolorScrollBorder = r.m_strVcolorScrollBorder;
		m_strVcolorScrollArrow = r.m_strVcolorScrollArrow;*/
		m_bDefaultScrollStyle = r.m_bDefaultScrollStyle;
		m_bMale = r.m_bMale;
		m_bTribe = r.m_bTribe;
	}

	unsigned long m_nSelfUin;//only
	MessageFont m_stChatFont;//only
	//CString m_strVcolorScroll;//only
	//CString m_strVcolorScrollBorder;//only
	//CString m_strVcolorScrollArrow;//only

	CString	m_strVcolorScrollTrackColor;
	CString	m_strVcolorScrollHighlightColor;
	CString	m_strVcolorScrollShadowColor;
	CString	m_strVcolorScroll3dLightColor;
	CString	m_strVcolorScrollArrowColor;
	CString	m_strVcolorScrollDarkShadowColor;
	CString	m_strVcolorScrollFaceColor;
	unsigned int m_keySend;		//Enter发送或者Ctrl+Enter发送 add by xiaoyi

	CString m_strDocument;

	common::ui::CMenuEx m_menuText;//复制，粘贴...
	common::ui::CMenuEx	m_menuMember;//选中赠送用户，选中聊天对象,用户信息、加好友...

	bool m_bEnableContextMenu;//是否启用右键菜单
	unsigned int m_unSelectUIN;

	unsigned int m_nEditable;
	bool m_bTribe;

	bool m_bDefaultScrollStyle;//2008-5-20 add by plushu
	bool m_bMale;

	CString   str_ShowUin;			//靓号   add by xiaoyi
};