#pragma once

namespace Hall
{
	enum 
	{
		MESSAGE_VALUE_HALL_MODULE_BEGIN = MESSAGE_VALUE_HALL_BEGIN,

		MESSAGE_VALUE_HALL_OPENSOUND,//是否打开声音  param0:  1:打开声音  0;关闭声音
		MESSAGE_VALUE_HALL_HEAD_IMAGE_UPDATE,  //最新头像图片  param0: unUIN
		MESSAGE_VALUE_HALL_RES_UPDATE, //配置文件已更新

		MESSAGE_VALUE_HALL_MODULE_END,
	};

	enum
	{
		SERVICE_VALUE_HALL_MODULE_BEGIN = SERVICE_VALUE_HALL_BEGIN,

		SERVICE_VALUE_HALL_GET_WEBINTERFACEURL,  //&WebInterfaceUrl
		SERVICE_VALUE_HALL_GET_TOKEN_URL,  //&std::wstring
		SERVICE_VALUE_HALL_OPEN_TOKEN_URL,  //&std::wstring
		SERVICE_MSGBOX_SHOW,  //&MsgBox179 
		SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH,  //&HeadImage    注：HeadImage.m_isGuest为true时 HeadImage.m_unUIN、HeadImage.m_bMan无效，可不填 
		SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH,  //&HeadImage 注：HeadImage.m_isGuest为true时 HeadImage.m_unUIN、HeadImage.m_bMan无效，可不填 

		SERVICE_VALUE_HALL_END,

	};

	enum E_HallEventValue
	{

		event_value_hall_module_begin = EVENT_VALUE_HALL_BEGIN,

		EVENT_VALUE_HALL_CREATEANDSHOW, //创建大厅

		EVENT_VALUE_HALL_ENTER_ROOM_REQUEST,  //申请进房 param0: roomId
		EVENT_VALUE_HALL_HEAD_UPLOAD,         //头像上传 param0: &HeadSetting
		EVENT_VALUE_HALL_ACTIVATE_SHOW,       //大厅激活显示
		EVENT_VALUE_HALL_InLineURL_SHOW,      //大厅右侧打开内嵌链接  (大厅右侧内嵌页面)  param0: &InLineUrl

		EVENT_VALUE_HALL_END,
	};
		

	struct WebInterfaceUrl
	{
		//SERVICE_VALUE_HALL_GET_WEBINTERFACEURL

		std::wstring m_WebInterfaceKeyName;  //请求的web接口键的名字  请参考SysSet.ini中[WebInterfaceUrl]下键的名字
		bool m_WebWithPara;   //是否在web接口之后带参,  true为带参(url之后加参数uin,tokentype,token)   ， false为不带
		std::wstring m_WebInterfaceUrl;  //返回的url , 
		WebInterfaceUrl():m_WebWithPara(true)
		{
		}
	};

	struct HeadImage
	{
		//SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH
		//SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH

		uint32 m_unUIN;   //请求头像图片的unUIN
		bool m_bMan;      //请求头像图片的用户性别  true为男  false为女
		bool m_isGuest;   //是否为游客
		std::wstring m_HeadImagePath;  //返回的头像图片路径
	};	
	
	struct MsgBox179
	{
		//SERVICE_MSGBOX_SHOW		

		enum  BoxStyle
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
		};

		enum BoxICO
		{
			//图标的几种类型
			ICONTYPE_WARNING                = 80,          //警告图标
			ICONTYPE_BINGO,								   //打钩图标
			ICONTYPE_ERROR,								   //打钗图标
		};

		std::wstring m_sMessage1;  //提示内容
		std::wstring m_sMessage2;  //提示内容   (提示框类型为带2行时有效)
		std::wstring m_sMessage3;  //提示内容   (提示框类型为带3行时有效)
		std::wstring m_linkText;  //链接内容   (提示框类型为带链接时有效)
		std::wstring m_linkLocal; //链接地址   (提示框类型为带链接时有效)			
		BoxStyle m_boxStype; //提示框类型
		BoxICO m_boxICO;     //提示框图标  (提示框类型为带图标时有效)
		std::wstring m_sTitle;    //提示框标题
	};

	struct HeadSetting : public ExtraInfo
	{
		//EVENT_VALUE_HALL_HEAD_UPLOAD

		HWND m_parentHwnd;
	};

	struct InLineUrl : public ExtraInfo
	{
		//EVENT_VALUE_HALL_InLineURL_SHOW
		std::wstring m_InLineUrl;    //大厅右侧打开内嵌链接url
	};
	
};