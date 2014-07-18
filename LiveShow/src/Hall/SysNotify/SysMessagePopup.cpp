#include "stdafx.h"
#include "SysMessagePopup.h"
#include "../HallModule.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "utility\timers.h"
#include <boost/bind.hpp>
#include "SysHtmlNotifyDlg.h"
#include "Roomdefine.h"
#include "../Hall.h"

#define MESSAGE_TITLE_HTML _T("%s<a name=\"closemes\"href=\"#\"></a>")
#define MESSAGE_TOP_MES_HREF_HTML  _T("<p class=\"black\" >%s</p><p style=\"margin:0;padding:6px 0;line-height:14px;\">&nbsp;&nbsp;&nbsp;&nbsp;%s</p><p style=\"position:absolute;bottom:0;left:0;margin:0;width:248px;padding:0 0 14px; text-align:center\"> <a target=\"_blank\" href=\"%s\"> %s </a></p>")
#define MESSAGE_TOP_MES_HTML  _T("<p class=\"black\">%s</p><p style=\"margin:0;padding:6px 0;line-height:14px;\">&nbsp;&nbsp;&nbsp;&nbsp;%s</p>")
#define MESSAGE_IMG_HTML _T("<img src=\"%s\"/>")
#define MESSAGE_TEXT_HTML _T("<p style=\"line-height:14px;\">&nbsp;&nbsp;%s</p>")


CSysMessagePopup::CSysMessagePopup()
{
	m_pSysNotifyDlg = NULL;
	m_SysMessageTimer = -1;
	m_CurSysMessage = NULL;
}

void CSysMessagePopup::CopyWithoutTitleFormEvent(std::vector<core::NotifySystemMessageEvent::MessageInfo> &msgSec,std::vector<SystemMessage::MessageInfo> &msgDec)
{
	if(msgSec.size() <=0)
	{
		return ;
	}

	CString NameUIN,vipPath,topMesHTML,vipImgHTML,MagnateHTML,tempHtml;	
	GetCurNickNameVip(NameUIN,vipPath);	

	SystemMessage::MessageInfo msgInfo;
	core::NotifySystemMessageEvent::MessageInfo &msgInfoSec = msgSec[0];	

	vipImgHTML.Format(MESSAGE_IMG_HTML,vipPath);
	topMesHTML.Format(_T("尊敬的 %s %s"),vipImgHTML,NameUIN);
	if(msgInfoSec.strTipsText.size()>0)
	{
		MagnateHTML.Format(MESSAGE_TOP_MES_HREF_HTML,topMesHTML,msgInfoSec.strMessage.c_str(),msgInfoSec.strTipsURL.c_str(),msgInfoSec.strTipsText.c_str());		
	}
	else
	{
		MagnateHTML.Format(MESSAGE_TOP_MES_HTML,topMesHTML,msgInfoSec.strMessage.c_str());
	}
	msgInfo.strMessage = MagnateHTML;

	msgDec.push_back(msgInfo);


}
void CSysMessagePopup::CopyHtmlSysMesFormEvent(std::vector<core::NotifySystemMessageEvent::MessageInfo> &msgSec,std::vector<SystemMessage::MessageInfo> &msgDec)
{
	if(msgSec.size() <=0)
	{
		return ;
	}

	CString NameUIN,vipPath,topMesHTML,vipImgHTML,MagnateHTML,tempHtml;
	SystemMessage::MessageInfo msgInfo;
	core::NotifySystemMessageEvent::MessageInfo &msgInfoSec = msgSec[0];	
	
	MagnateHTML.Format(MESSAGE_TEXT_HTML,msgInfoSec.strMessage.c_str());	
	msgInfo.strMessage = MagnateHTML;

	msgDec.push_back(msgInfo);

}

void CSysMessagePopup::CopySysMesFormEvent(std::vector<core::NotifySystemMessageEvent::MessageInfo> &msgSec,std::vector<SystemMessage::MessageInfo> &msgDec)
{		

	for(int i=0;i<msgSec.size();i++)
	{
		SystemMessage::MessageInfo msgInfo;
		core::NotifySystemMessageEvent::MessageInfo &msgInfoSec = msgSec[i];
		msgInfo.strMessage = msgInfoSec.strMessage;
		msgInfo.strTipsText = msgInfoSec.strTipsText;
		msgInfo.strTipsURL = msgInfoSec.strTipsURL;		


		msgDec.push_back(msgInfo);
	}
}

void CSysMessagePopup::OnSystemMessageOther(SystemMessage::SystemMessageType sysMesOther)
{
	bool bMes = true;
	switch(sysMesOther)
	{
	case SystemMessage::SystemMessageType::SYSTEMMESSAGETYPE_MAGNATE:  //富豪等级升级消息
		{
			CreateMagnateSysMessage(); //创建富豪等级升级消息进系统消息列表
		}
		break;

	default: 
		bMes = false;		
		break;
	}

	if(bMes && m_pSysNotifyDlg == NULL )
	{
		CreateSysMessageTimer(1);  //若当前没有系统弹窗，则1秒后显示
	}

}
//系统弹窗响应函数
void CSysMessagePopup::OnSystemMessageEvent(Event const& evt)
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::NotifySystemMessageEvent *pEvent=static_cast<core::NotifySystemMessageEvent*>(evt.m_pstExtraInfo);

	//保存系统消息进列表	
	SystemMessage *sysMes = new SystemMessage;
	sysMes->strWindowTitle = GetSysMessageHtmlTitle(pEvent->strWindowTitle.c_str());

	sysMes->enmType = (SystemMessage::SystemMessageType)pEvent->enmType;

	if(sysMes->enmType == SystemMessage::SYSTEMMESSAGETYPE_SYSTEM)
	{
		CopySysMesFormEvent(pEvent->strMessage,sysMes->strMessage);
	}
	else if(sysMes->enmType == SystemMessage::SYSTEMMESSAGETYPE_WITHOUT_TITLE)
	{
		CopyWithoutTitleFormEvent(pEvent->strMessage,sysMes->strMessage);  		
	}
	else
	{
		CopyHtmlSysMesFormEvent(pEvent->strMessage,sysMes->strMessage);
	}

	m_SysMessList.push_back(sysMes); 	

	if(m_pSysNotifyDlg == NULL)
	{
		CreateSysMessageTimer(1);  //若当前没有系统弹窗，则1秒后显示
	}


}

//清除系统弹窗消息
void CSysMessagePopup::ClearSysMessage()
{
	ClearSysMessageTimer();
	CloseSysMessageDlg();

	std::list<SystemMessage*>::iterator iter;
	for(iter = m_SysMessList.begin(); iter != m_SysMessList.end(); iter++)
	{
		SystemMessage* pSysMes = *iter;
		if(pSysMes != NULL)
		{
			delete pSysMes;
			pSysMes = NULL;
		}

	}
	m_SysMessList.clear();
}

void CSysMessagePopup::CreateSysMessageTimer(uint32 interval)
{
	if(m_SysMessageTimer == -1)
	{
		m_SysMessageTimer = common::utility::_Timers::instance().CreateTimer(interval, 
			common::utility::_Timers::TimerCallback(
			boost::bind(&CSysMessagePopup::OnSysMessageTimeOut,this,boost::arg<1>(),boost::arg<2>())), 
			common::utility::_Timers::ENM_TIMER_HALL_SYSTEMMESSAGE);
	}

}

void CSysMessagePopup::ClearSysMessageTimer()
{
	if(m_SysMessageTimer != -1)
	{
		common::utility::_Timers::instance().DestroyTimer(m_SysMessageTimer);
		m_SysMessageTimer = -1;
	}
}

//系统弹窗定时器回调函数
void CSysMessagePopup::OnSysMessageTimeOut(uintptr,bool)
{
	if(m_pSysNotifyDlg == NULL )  //若当前没有系统弹窗(即首次弹窗) 则清除定时器后弹窗
	{
		ClearSysMessageTimer();
		CloseSysMessageDlg();	
		NextSysMessage();

	}
	else
	{
		BOOL mouseLeave; //当前鼠标是否不在弹窗内
		if(m_CurSysMessage->enmType == SystemMessage::SYSTEMMESSAGETYPE_SYSTEM) //系统消息			
		{
			CSysNotifyDlg *pSysNotifyDlg = (CSysNotifyDlg*)m_pSysNotifyDlg;
			mouseLeave = pSysNotifyDlg->IfMouseLeave();
		}		
		else
		{
			CSysHtmlNotifyDlg *pSysNotifyDlg = (CSysHtmlNotifyDlg*)m_pSysNotifyDlg;
			mouseLeave = pSysNotifyDlg->IfMouseLeave();
		}	

		if(m_SysMessList.size() == 0 && mouseLeave) //所有系统弹窗都已显示完成且鼠标不在弹窗内时，清除定时器
		{
			ClearSysMessageTimer();
		}
		if(mouseLeave) //鼠标不在弹窗内时 关闭系统弹窗
		{
			CloseSysMessageDlg();	
			NextSysMessage();			
		}

	}


}

//创建系统弹窗对话框
void CSysMessagePopup::CreateSysMessageDlg(SystemMessage *pSysMes)
{
	ASSERT(pSysMes != NULL);
	switch(pSysMes->enmType)
	{
	case SystemMessage::SYSTEMMESSAGETYPE_SYSTEM:   //系统消息
		{
			CSysNotifyDlg *pSysNotifyDlg = new CSysNotifyDlg;  //系统消息弹窗
			if (pSysNotifyDlg)
			{					
				SwitchResourceToModule(_T("Hall"));
				pSysNotifyDlg->Create(CSysNotifyDlg::IDD,NULL);

				SYSMSGMAP mapSysMsg;
				for (int i = 0; i < pSysMes->strMessage.size(); ++i)
				{
					stSysMsg* pSysMsg = new stSysMsg;
					pSysMsg->strContent.Format(_T("%s"),pSysMes->strMessage[i].strMessage.c_str());
					pSysMsg->strHyperLinkLabel.Format(_T("%s"),pSysMes->strMessage[i].strTipsText.c_str());
					pSysMsg->strHyperLingAddr.Format(_T("%s"),pSysMes->strMessage[i].strTipsURL.c_str());

					mapSysMsg.insert(std::pair<int,stSysMsg*>(i,pSysMsg));
				}				

				pSysNotifyDlg->SetSysMsg(mapSysMsg,pSysMes->strWindowTitle.c_str());
				pSysNotifyDlg->ShowWindow(SW_SHOWNOACTIVATE);
				m_pSysNotifyDlg = pSysNotifyDlg;
			}

		}
		break;

	case SystemMessage::SYSTEMMESSAGETYPE_TEXT:  //纯文本
	case SystemMessage::SYSTEMMESSAGETYPE_WITHOUT_TITLE:  //(服务器发送来的消息不包含“尊敬的***”部分，客户端已有该部分)
	case SystemMessage::SYSTEMMESSAGETYPE_MAGNATE:  //富豪等级升级消息
		{

			CSysHtmlNotifyDlg *pSysNotifyDlg = new CSysHtmlNotifyDlg;  

			if (pSysNotifyDlg)
			{					
				SwitchResourceToModule(_T("Hall"));
				pSysNotifyDlg->Create(CSysHtmlNotifyDlg::IDD);
				pSysNotifyDlg->SetSystemMessage(m_CurSysMessage);
				pSysNotifyDlg->ShowWindow(SW_SHOWNOACTIVATE);
				m_pSysNotifyDlg = pSysNotifyDlg;
			}	
		}
		break;	

	default: 
		break;
	}


	CreateSysMessageTimer(HALL_SYSTEMMESSAGE_TIMEOUT); //系统弹窗5秒之后自动关闭
}
//关闭系统弹窗对话框
void CSysMessagePopup::CloseSysMessageDlg()
{
	if(m_pSysNotifyDlg != NULL)
	{		
		m_pSysNotifyDlg->DestroyWindow();
		delete m_pSysNotifyDlg;
		m_pSysNotifyDlg = NULL;
	}
	if(m_CurSysMessage != NULL)
	{
		delete m_CurSysMessage;
		m_CurSysMessage = NULL;
	}
}

//若m_SysMessList有存储的系统消息，则弹出
void CSysMessagePopup::NextSysMessage()
{	
	if(m_SysMessList.size() > 0)
	{
		m_CurSysMessage = m_SysMessList.front();
		m_SysMessList.pop_front();
		CreateSysMessageDlg(m_CurSysMessage);

	}


}

//获取标题的html格式内容
CString CSysMessagePopup::GetSysMessageHtmlTitle(CString strTitle)
{
	CString titleHtml;

	if(!strTitle.IsEmpty())
	{		
		titleHtml.Format(MESSAGE_TITLE_HTML,strTitle);
	}
	else
	{
		titleHtml.Format(MESSAGE_TITLE_HTML,_T("系统消息"));
	}

	return titleHtml;

}

// 获取富豪等级升级消息的html格式内容
CString CSysMessagePopup::GetMagnateHtmlContent()
{
	core::CurInfo m_MyCurInfo = GetCurInfo();
	Room_Get_Rich_Level_Info info;
	info.unRichLevel = m_MyCurInfo.unRichLevel;
	g_HallModule->m_pModuleManager->CallService(Room::SERVICE_ROOM_GET_RICHLEVEL_INFO, (param)&info);

	CString NameUIN,vipPath,topMesHTML,vipImgHTML,MagnateHTML,tempHtml;	

	GetCurNickNameVip(NameUIN,vipPath);	
	vipImgHTML.Format(MESSAGE_IMG_HTML,vipPath);
	topMesHTML.Format(_T("尊敬的 %s %s"),vipImgHTML,NameUIN);
	tempHtml.Format(_T("恭喜您!您的富豪等级已升级为<span style=\"color:red\">%s</span>"),info.strLevelName.c_str());
	MagnateHTML.Format(MESSAGE_TOP_MES_HTML,topMesHTML,tempHtml);

	return MagnateHTML;

}

//创建富豪等级升级消息进系统消息列表
void CSysMessagePopup::CreateMagnateSysMessage()
{
	//保存系统消息进列表
	SystemMessage *sysMes = new SystemMessage;		
	sysMes->strWindowTitle = GetSysMessageHtmlTitle();	

	sysMes->enmType = SystemMessage::SystemMessageType::SYSTEMMESSAGETYPE_MAGNATE;		

	CString MagnateMes = GetMagnateHtmlContent();	

	SystemMessage::MessageInfo msgInfo;	
	msgInfo.strMessage = MagnateMes;
	msgInfo.strTipsText = _T("");
	msgInfo.strTipsURL = _T("");
	sysMes->strMessage.push_back(msgInfo);
	m_SysMessList.push_back(sysMes); 	
}


//获取当前用户昵称、vip等级
void CSysMessagePopup::GetCurNickNameVip(CString &NickName,CString &vipPath)
{	
	core::CurInfo m_MyCurInfo = GetCurInfo();
	if(m_MyCurInfo.strName.size() == 0)  
	{
		NickName.Format(_T("%d(%d)"),m_MyCurInfo.un179id, m_MyCurInfo.un179id);
	}
	else
	{
		NickName.Format(_T("%s(%d)"),m_MyCurInfo.strName.c_str(), m_MyCurInfo.un179id);
	}	

	CString strModulePath,m_ResListiconPath;
	strModulePath = common::utility::systemhelper::Get179AppPath().c_str();		
	m_ResListiconPath.Format(_T("%sresource\\listicon\\"),strModulePath);	

	switch(m_MyCurInfo.enmVIPLevel)
	{
	case core::VIPLEVEL_NONE:  //未注册
		{
			vipPath.Format(_T("%sLevelVipNone.png"),m_ResListiconPath);			

		}
		break;
	case core::VIPLEVEL_REGISTER:  //已注册
		{
			vipPath.Format(_T("%sLevelVipNone.png"),m_ResListiconPath);				
		}
		break;
	case core::VIPLEVEL_VIP:   //vip
		{
			vipPath.Format(_T("%sLevelVip.png"),m_ResListiconPath);		

		}
		break;
	case core::VIPLEVEL_HONOR:  //荣耀vip
		{
			vipPath.Format(_T("%sLevelVipHonor.png"),m_ResListiconPath);		

		}
		break;
	case core::VIPLEVEL_SILVER:  //银冠
		{
			vipPath.Format(_T("%sLevelSilver.png"),m_ResListiconPath);		

		}
		break;
	case core::VIPLEVEL_GOLDEN:  //金冠
		{
			vipPath.Format(_T("%sLevelGolden.png"),m_ResListiconPath);			

		}
		break;
	case core::VIPLEVEL_CRYSTAL:  //水晶皇冠
		{
			vipPath.Format(_T("%sLevelCrystal.png"),m_ResListiconPath);			

		}
		break;
	case core::VIPLEVEL_DIAMOND:  //钻石皇冠
		{
			vipPath.Format(_T("%slevelDiamond.png"),m_ResListiconPath);			

		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:  //黑钻皇冠
		{
			vipPath.Format(_T("%sLevelBlackDiamond.png"),m_ResListiconPath);			

		}
		break;
	case core::VIPLEVEL_TOP:  //至尊皇冠
		{
			vipPath.Format(_T("%sLevelTop.png"),m_ResListiconPath);			

		}
		break;

	default: 
		vipPath.Format(_T("%sLevelVipNone.png"),m_ResListiconPath);	
		break;
	}	

}