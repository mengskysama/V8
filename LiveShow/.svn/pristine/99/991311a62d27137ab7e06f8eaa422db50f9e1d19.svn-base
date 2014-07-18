#pragma once

#include "SysNotifyDlg.h"
#include "51ktv.h"
#include "coredefine.h"
#include <list>

#define HALL_SYSTEMMESSAGE_TIMEOUT 5  //定时器间隔   系统弹窗间隔时间（单位：秒）

struct SystemMessage
{
	enum SystemMessageType
	{
		SYSTEMMESSAGETYPE_SYSTEM		=	0x00,//系统消息
		SYSTEMMESSAGETYPE_TEXT			=	0x01,//纯文本(单条)
		SYSTEMMESSAGETYPE_WITHOUT_TITLE	=	0x20,//客户端加上“尊敬的***”(单条)

		//以上消息类型要与 code::NotifySystemMessageEvent::ENM_SystemMessageType 的消息类型一至


		SYSTEMMESSAGETYPE_MAGNATE       =   0x80,//富豪等级升级消息
	};

	struct MessageInfo
	{
		std::wstring strMessage;
		std::wstring strTipsText;
		std::wstring strTipsURL;
	};

	std::wstring strWindowTitle;//窗口title
	SystemMessageType enmType;
	std::vector<MessageInfo> strMessage;
};


class CSysMessagePopup 
{
public:
	CSysMessagePopup();
	virtual ~CSysMessagePopup(){};

private:
	void CreateSysMessageTimer(uint32 interval);  //创建系统弹窗定时器
	void ClearSysMessageTimer();  //清除系统弹窗定时器
	void OnSysMessageTimeOut(uintptr,bool);  //系统弹窗定时器回调函数
	void CreateSysMessageDlg(SystemMessage *pSysMes); //创建系统弹窗对话框
	void CloseSysMessageDlg(); //关闭系统弹窗对话框	
	void NextSysMessage();  //若m_SysMessList有存储的系统消息，则弹出
	void CopySysMesFormEvent(std::vector<core::NotifySystemMessageEvent::MessageInfo> &msgSec,std::vector<SystemMessage::MessageInfo> &msgDec);
	void CopyWithoutTitleFormEvent(std::vector<core::NotifySystemMessageEvent::MessageInfo> &msgSec,std::vector<SystemMessage::MessageInfo> &msgDec);
	void CopyHtmlSysMesFormEvent(std::vector<core::NotifySystemMessageEvent::MessageInfo> &msgSec,std::vector<SystemMessage::MessageInfo> &msgDec);


	void CreateMagnateSysMessage();  //创建富豪等级升级消息进系统消息列表
	CString GetSysMessageHtmlTitle(CString strTitle=_T("")); //获取标题的html格式内容
	CString GetMagnateHtmlContent();  //获取富豪等级升级消息的html格式内容
	void GetCurNickNameVip(CString &NickName, CString &vipPath);  //获取当前用户昵称、vip等级路径

private:
	std::list<SystemMessage*> m_SysMessList;  //系统消息列表
	CDialog* m_pSysNotifyDlg; //系统弹窗
	int32 m_SysMessageTimer;
	SystemMessage* m_CurSysMessage;  //当前系统消息

public:
	void OnSystemMessageEvent(Event const& evt); //系统弹窗响应函数
	void OnSystemMessageOther(SystemMessage::SystemMessageType sysMesOther);
	void ClearSysMessage();  //清除系统弹窗消息
};