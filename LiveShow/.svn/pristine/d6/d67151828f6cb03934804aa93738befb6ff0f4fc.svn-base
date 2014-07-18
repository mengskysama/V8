#pragma once

#include "../Common.h"
#include <map>
#include <vector>

#define MSGBOX_STYLE	INT
#define MSGBOX_ICONTYPE INT
#define RET_OK          1
#define RET_CANCEL      0

#define COMMONLIB_UM_NONDOMODAL_MSGBOX_CLOSE		      WM_USER + 2323 
#define COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER       WM_USER + 2324
//新提示框类型
namespace NewMsgBox
{
	enum
	{
		//无图标的几个类型 
		MSGBOX_OK_ONE_STRING                    = 50,  //一行文字，确定
		MSGBOX_OK_TWO_STRING,                         //两行文字，确定
		MSGBOX_OK_ONE_STRING_WITH_LINK,				  //一行文字，一个链接，确定
		MSGBOX_OK_TWO_STRING_WITH_LINK,				  //两行文字，一个链接，确定
		//带图标的几个类型
		MSGBOX_ICO_OK_ONE_STRING,					  //图标，一行文字，确定
		MSGBOX_ICO_OK_TWO_STRING,					  //图标，两行文字，确定
		MSGBOX_ICO_OK_CANCEL_ONE_STRING,			  //图标，一行文字，确定，取消
		MSGBOX_ICO_OK_CANCEL_TWO_STRING,			  //图标，两行文字，确定，取消
		//游客操作提示类型
		MSGBOX_GUEST_OPERATE,						  //游客操作提示的普遍样式
		//遗漏的样式继续添加进来
		MSGBOX_OK_THREE_STRING,                       //三行文字，确定
		MSGBOX_OK_THREE_STRING_WITH_LINK,             //三行文字，一个链接，确定
		//带CheckBox
		MSGBOX_OK_CANCEL_ONE_STRING_WITH_CHECKBOX,	  //一行文字，确定，取消，CheckBox
		MSGBOX_OK_CANCEL_TWO_STRING_WITH_CHECKBOX,	  //两行文字，确定，取消，CheckBox
		MSGBOX_OK_CANCEL_THREE_STRING_WITH_CHECKBOX,  //三行文字，确定，取消，CheckBox
		//非模态提示框
		MSGBOX_ICO_OK_ONE_STRING_NONDOMODAL,          //非模态，图标，一行文字
		//用于点歌系统的提示框
		MSGBOX_ORDERSONG_OK_TWO_STRING,               //两行字，确定
		MSGBOX_ORDERSONG_OK_CANCEL_TWO_STRING,        //两行字，确定，取消
		MSGBOX_ORDERSONG_OK_CANCEL_ONE_STRING,        //一行字，确定，取消
	};

	enum
	{
		//图标的几种类型
		ICONTYPE_WARNING                = 80,          //警告图标
		ICONTYPE_BINGO,								   //打钩图标
		ICONTYPE_ERROR,								   //打钗图标
	};
}
class CMsgBox;
class COMMON_INTERFACE C179MsgBox
{
public:
	static BOOL IsMsgBoxVisible();
	static BOOL IsChecked();

//新提示框接口
	//无图标类、游客类、有CheckBox类
	static int Show(CWnd* pParent, MSGBOX_STYLE nStyle, LPCTSTR pzPrompt1=_T(""), LPCTSTR pzPrompt2=_T(""), LPCTSTR pzPrompt3=_T(""),  LPCTSTR linkLabel=_T(""), LPCTSTR linkAddress=_T(""), LPCTSTR pzTitle=_T("提 示"), LPCTSTR pzCheckBox=_T("不再提醒"));
	//有图标类
	static int Show(CWnd* pParent, MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1=_T(""), LPCTSTR pzPrompt2=_T(""), LPCTSTR pzTitle=_T("提 示"));
	//非模态类
	static CMsgBox* ShowNonDomodal(CWnd* pParent, MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1=_T(""), LPCTSTR pzTitle=_T("提 示"));
	//用于点歌系统
	static int ShowForOrderSong(CWnd* pParent, MSGBOX_STYLE nStyle, LPCTSTR pzString1=_T(""), LPCTSTR pzString2=_T(""), LPCTSTR colorString1=_T(""), LPCTSTR colorString2=_T(""), LPCTSTR btnOKLabel=_T("确定"), LPCTSTR btnCancelLabel=_T("取消"), LPCTSTR pzTitle=_T("点歌结果"));
private:

	static std::map<HWND, std::vector<HWND>> s_mapParent2Child;
	static BOOL s_bChecked;
};
