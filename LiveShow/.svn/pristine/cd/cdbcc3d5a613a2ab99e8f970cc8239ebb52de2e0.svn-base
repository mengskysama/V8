#pragma once

#include "wtypes.h"
#include "51ktv.h"

#define MAX_FONTNAME_LENGTH 64

enum FONT_EFFECT_TYPE
{
	FONT_TYPE_BOLD	     =	0x0001,
	FONT_TYPE_UNDERLINE  =	0x0002,
	FONT_TYPE_ITALIC	 =	0x0004,
	FONT_TYPE_STRIKEOUT	 =	0x0008,
	FONT_TYPE_LINK		 =	0x0010,
	//自定义字体,如火星文等
};

struct SChatFont
{
	unsigned char unFontSize;
	wchar_t strFontName[LF_FACESIZE+1];
	COLORREF      crColor;
	unsigned int  unFontType;//FONT_EFFECT_TYPE
};

enum PICTURE_TAG_TYPE
{
	PICTURE_TAG_SEND_RECEIVE,
	PICTURE_TAG_TEMP,
};

/*
消息发送方式的设置
*/
enum
{
	KEY_ENTER = 0x02,								//按enter键发送
	KEY_ENTER_CTRL = 0X01,							//按enter+ctrl发送 
};

#define PICTURE_TAG_SEND_RECEIVE_STRING  _T("c_i")
#define PICTURE_TAG_TEMP_STRING          _T("j_i")

#define PICTURE_DEFAULT_FOLDER          _T("Face")
#define PICTURE_CUSTOM_FOLDER           _T("Emotions_Send")

#define SYSTEM_INFO_IMAGE _T("config\\im_info.bmp")
#define DOWNLOADING_IMAGE _T("config/imageloading.gif")

#define EDITABLE_OUT		0		//输出框
#define EDITABLE_INPUT		1		//输入框
#define EDITABLE_OUT_INPUT	2		//即可输出又可输出框