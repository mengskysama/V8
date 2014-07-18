#include "stdafx.h"
#include "HtmlBuild.h"
#include "gift\GiftManager.h"
#include "userlist\ListIconManager.h"
#include "GlobalRoomDefine.h"
#include "GlobalManager.h"
#include "roomsetting\RoomOption.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\utility\URLEncode.h"
#include "RoomDefine.h"
#include "RoomModule.h"
#include "globaluidefine.h"
#include "Room.h"
#include "HallDefine.h"
#include "RoomBusiness.h"
#include "personalsetting\PersonalConfig.h"
#include "autoreply\AutoReplyConfig.h"
#include "richlevel\RichLevelManager.h"
#include "sofa/SofaItem.h"

#define MAX_SHOW_179ID	999999

HtmlBuild::HtmlBuild( RoomBusiness* pRoomBusiness )
:m_pRoomBusiness( pRoomBusiness )
{

}

CString HtmlBuild::GetRequestP2PNotify( IUserItem* pSrcUserItem ) 
{
	CString strName ;	
	strName.Format( L"%s邀请您进行一对一视频，您是" ,GetNameAndID(pSrcUserItem) ) ;
	CString strAcceptHref ;
	strAcceptHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\">同意</a>" ,
		L"" , pSrcUserItem->GetDecodeUserName().c_str() , pSrcUserItem->GetUserUIN() ,CLICK_TYPE_P2P_ACCEPT ) ;
	CString strRefuseHref ;
	strRefuseHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\">拒绝</a>" ,
		L"" , pSrcUserItem->GetDecodeUserName().c_str() , pSrcUserItem->GetUserUIN() , CLICK_TYPE_P2P_REFUSE ) ;

	return  GetPrivateTips( L"【提示】： " ,strName + L" " + strAcceptHref+L" 还是 "+strRefuseHref+L"？" , COLOR_OPERATE  ) ;
}

CString HtmlBuild::GetOpenPrivateRspNoPermission() 
{
	CString strHref  ;
	strHref.Format( L"<a href=\"%s\" target=\"_blank\">立即升级<a>" , m_pRoomBusiness->GetUpdateVipUrl() ) ;
	
	return GetPrivateTips( L"【提示】： "  , L"抱歉，只有荣耀VIP以上用户可观看私麦 "+ strHref  , COLOR_OPERATE ) ; ;
}

CString HtmlBuild::GetOpenPrivateRspIsFull() 
{
	CString strHref  ;
	strHref.Format( L"<a href=\"%s\" target=\"_blank\">立即升级<a>" , m_pRoomBusiness->GetUpdateVipUrl() ) ;
	return GetPrivateTips( L"【提示】： "  , L"抱歉！您当前可打开的私麦数量已达上限 "+ strHref , COLOR_OPERATE ) ; ;
}

CString HtmlBuild::GetPublicStamp( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem , uint16 unStampID ) 
{
	//发送者	
	CString strSrcNameHref = ChatUserItem( pSrcUserItem ) ;
	CString strDstNameHref = StampRightUserItem( pDstUserItem ) ;
	CString strImg ;
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchStampByID( unStampID ) ;
	if ( pGiftItem != NULL )
	{
		strImg = Img( pGiftItem->GetSmallImagePath().c_str() ) ;
	}
	CString strTemp = Span( 12 , strSrcNameHref+L" 调皮地鉴定 "+strDstNameHref+L" 为： "+strImg )	;

	strTemp += L"</BR>" ;

	return strTemp ;
}

CString HtmlBuild::GetPrivateStamp( IUserItem* pSrcUserItem , uint16 unStampID ) 
{

	CString strImg ;
	//图片
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchStampByID( unStampID ) ;
	if ( pGiftItem != NULL )
	{
		strImg = Img( pGiftItem->GetSmallImagePath().c_str() ) ;
	}
	CString strHref ;
	strHref.Format( _T("<a href=\"%s\" name=\"%s\" type=\"%d\" uuid=\"%d\">印章道具</a>") ,
		L"" ,pSrcUserItem->GetDecodeUserName().c_str() ,
		CLICK_TYPE_STAMP ,pSrcUserItem->GetUserUIN() ) ;
	return GetPrivateTips( L"【提示】： " , L"您被"+ PrivateUserItem( pSrcUserItem ) +L"鉴定为： " +strImg +
		L" 来而不往非礼也，选择"+strHref+ L"给他盖回去～" ,COLOR_GIFT) ;
}

CString HtmlBuild::GetPrivateMicNotify( IUserItem* pSrcUserItem  ) 
{
	CString strHref ;
	CString strName ;
	strName.Format( L"%s" ,GetNameAndID(pSrcUserItem) ) ;

	strHref.Format(_T("<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\" 179id=\"%u\" >点击观看</a>") ,
		L"" , pSrcUserItem->GetDecodeUserName().c_str() ,
		pSrcUserItem->GetUserUIN() , CLICK_TYPE_PRIVATEVIDEO , pSrcUserItem->GetUser179ID() ) ;

	return GetPrivateTips( L"【提示】： " , strName + L"已上私麦，荣耀VIP以上用户可 "+strHref ,COLOR_OPERATE ) ;
}


CString HtmlBuild::GetPrivateFireWork( IUserItem* pSrcUserItem ) 
{
	CString strHref , strInfo , strLinkInfo ;

	strInfo.Format( L" 体恤管理员工作辛苦，为“%s”房间所有管理员绽放了一束烟花 "  ,m_pRoomBusiness->GetRoomOption()->GetRoomName().c_str() ) ;
	strLinkInfo.Format( L"%s体恤管理员工作辛苦，为“%s”房间所有管理员绽放了一束烟花 " , GetNameAndID(pSrcUserItem) ,
		m_pRoomBusiness->GetRoomOption()->GetRoomName().c_str() ) ;
	strHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\">转发到临时公告</a>" ,L"" ,strLinkInfo , 
		pSrcUserItem->GetUserUIN() , CLICK_TYPE_TEMP_NOTICE ) ;	

	return GetPrivateTips( L"【提示】： " ,ChatUserItem( pSrcUserItem )+strInfo+strHref , COLOR_GIFT)  ;
}

CString HtmlBuild::GetPrivateSlavo( IUserItem* pSrcUserItem ) 
{

	CString strHref , strInfo , strLinkInfo ;	
	 	
	strInfo.Format( L" 出手阔绰，为“%s”房间所有用户鸣放了一门礼炮 "  ,m_pRoomBusiness->GetRoomOption()->GetRoomName().c_str() ) ;	
	strLinkInfo.Format( L"%s出手阔绰，为“%s”房间所有用户鸣放了一门礼炮 "  , GetNameAndID(pSrcUserItem) ,
		m_pRoomBusiness->GetRoomOption()->GetRoomName().c_str() ) ;	
	strHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\">转发到临时公告</a>" ,L"" ,strLinkInfo , 
		pSrcUserItem->GetUserUIN() , CLICK_TYPE_TEMP_NOTICE ) ;

	return GetPrivateTips( L"【提示】： " ,ChatUserItem( pSrcUserItem )+ strInfo + strHref ,COLOR_GIFT)  ;
}

CString HtmlBuild::GetPublicFireWork( IUserItem* pSrcUserItem) 
{	
	CString strTemp ;
	strTemp.Format( L"%s 体恤管理员工作辛苦，为“%s”房间所有管理员绽放了一束烟花" , 
		ChatUserItem( pSrcUserItem ) , m_pRoomBusiness->GetRoomOption()->GetRoomName().c_str() ) ;
	return GetPublicTips( L"【提示】： " , strTemp ,COLOR_GIFT ) ;
}

CString HtmlBuild::GetPublicSlavo( IUserItem* pSrcUserItem ) 
{
	CString strTemp ;
	strTemp.Format( L"%s 出手阔绰，为“%s”房间所有用户鸣放了一门礼炮" , 
		ChatUserItem( pSrcUserItem ) , m_pRoomBusiness->GetRoomOption()->GetRoomName().c_str() ) ;
	return GetPublicTips( L"【提示】： " , strTemp ,COLOR_GIFT ) ;
}

CString HtmlBuild::GetGiftLucky( uint32 unSrcUIN ,uint32 src179ID ,CString& strSrcHead ,
								std::wstring strSrcName ,uint16 unGiftID , uint16 unLuckyTimes ,uint32 unLuckyMoney ) 
{
	IUserItem* pUserItem =  m_pRoomBusiness->GetUserManager()->SearchUserByUIN( unSrcUIN ) ;
	if ( pUserItem != NULL )
	{
		strSrcHead = GetUserHead( pUserItem ) ;
	}

	CString strTemp ;
	IUserItem* pSelfUserItem = m_pRoomBusiness->GetUserManager()->GetSelfUserItem() ;
	if ( pSelfUserItem == NULL )
	{
		return strTemp ;
	}

	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	if ( pGiftItem == NULL )
	{
		return L"";
	}

	CString strCoin ;
	strCoin.Format( L"<B>%d</B>" , unLuckyMoney ) ;
	if ( unLuckyTimes >= 500 )
	{
		strCoin = Span( 16 , strCoin ) ;
	}
	CString strImgPath = Img( pGiftItem->GetSmallImagePath().c_str() );
	if ( unSrcUIN ==  pSelfUserItem->GetUserUIN() )
	{
		//自己中奖
		CString strInfo ;
		CString strMultipliPngPath = GetLuckyTimesPngPath( unLuckyTimes ) ;
		if ( strMultipliPngPath.IsEmpty() )
		{
			strInfo.Format( L"恭喜！您送出的 %s%s 带来了好运，得到<B>%d</B>倍奖金共%s币！" , pGiftItem->GetName().c_str() ,strImgPath ,
				unLuckyTimes , strCoin ) ;
		}
		else
		{
			strInfo.Format( L"恭喜！您送出的 %s%s 带来了好运，得到%s倍奖金共%s币！" , pGiftItem->GetName().c_str() ,strImgPath ,
				Img( strMultipliPngPath )  , strCoin ) ;
		}
		if ( ( pSelfUserItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER) && unLuckyTimes >= 100  )
		{
			//管理员
			CString strHref ;				
			//if ( pGiftItem->IsNeedTempNotice() )
			//{
				CString strHrefName ;
				CString strSrcNameAndID;
				if (src179ID > MAX_SHOW_179ID)
				{
					strSrcNameAndID = strSrcName.c_str();
				}
				else
				{
					strSrcNameAndID.Format(L"%s(%u)", strSrcName.c_str(), src179ID);
				}
				strHrefName.Format( L"恭喜！%s送出的%s带来了好运，得到%d倍奖金共%d币！" , strSrcNameAndID ,
					pGiftItem->GetName().c_str() ,unLuckyTimes ,unLuckyMoney ) ;
				strHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%u\" type=\"%d\">转发到临时公告</a>" ,L"" ,strHrefName , 
					unSrcUIN , CLICK_TYPE_TEMP_NOTICE ) ;
			//}				

			strTemp = strInfo+strHref ;
		}
		else
		{
			strTemp = strInfo  ;
		}
	}
	else
	{			

		CString strName ;
		strName.Format( L"%s%s (%d)" , strSrcHead ,strSrcName.c_str() ,src179ID ) ;

		CString strInfo ;

		CString strMultipliPngPath = GetLuckyTimesPngPath( unLuckyTimes ) ;
		if ( strMultipliPngPath.IsEmpty() )
		{
			strInfo.Format( L"恭喜！%s送出 %s%s 带来了好运，得到<B>%d</B>倍奖金共%s币，送出幸运礼物，礼金翻倍来！" ,strName ,
				pGiftItem->GetName().c_str() ,strImgPath , unLuckyTimes , strCoin ) ;
		}
		else
		{
			strInfo.Format( L"恭喜！%s送出 %s%s 带来了好运，得到%s倍奖金共%s币，送出幸运礼物，礼金翻倍来！" ,strName ,
				pGiftItem->GetName().c_str() ,strImgPath , Img( strMultipliPngPath ) , strCoin ) ;

		}
		
		if ( (pSelfUserItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER) && unLuckyTimes >= 100 && pGiftItem->IsNeedTempNotice() )
		{
			//管理员
			CString strHref ;				
			CString strHrefName ;	
			CString strSrcNameAndID;
			if (src179ID > MAX_SHOW_179ID)
			{
				strSrcNameAndID = strSrcName.c_str();
			}
			else
			{
				strSrcNameAndID.Format(L"%s(%u)", strSrcName.c_str(), src179ID);
			}
			strHrefName.Format( L"恭喜！%s送出%s带来了好运，得到%d倍奖金共%d币，送出幸运礼物，礼金翻倍来！" ,
				strSrcNameAndID, pGiftItem->GetName().c_str() , unLuckyTimes , unLuckyMoney) ;
			strHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%u\" type=\"%d\">转发到临时公告</a>" ,L"" ,strHrefName , 
				unSrcUIN , CLICK_TYPE_TEMP_NOTICE ) ;
			strTemp = strInfo+strHref ;
			
		}
		else
		{
			strTemp = strInfo ;
		}
	}
	return GetPrivateTips( L"【提示】： " ,strTemp , COLOR_GIFT) ;
}

CString HtmlBuild::GetPrivateNormalGift( IUserItem* pSrcUserItem ,uint16 unGiftID , uint16 unCount ) 
{	
	CString strTemp ;
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	if ( pGiftItem != NULL )
	{	
		CString strCount ;
		strCount.Format(_T(" %d"),unCount) ;

		CString strImgPath = Img( pGiftItem->GetSmallImagePath().c_str() ) ;

		strTemp = GetPrivateTips(  L"【提示】： " , ChatUserItem(pSrcUserItem) + L" 本次共送给您 "+CString(pGiftItem->GetName().c_str()) + 
			strImgPath +strCount + CString(pGiftItem->GetUnitName().c_str())  , COLOR_GIFT) ;
	}

	return strTemp ;
}

CString HtmlBuild::GetPublicNormalGift( GiftInfo& giftinfo , uint32 unGiftCount ) 
{
	IUserItem* pSrcUserItem =  m_pRoomBusiness->GetUserManager()->SearchUserByUIN( giftinfo.srcuin ) ;
	if ( pSrcUserItem != NULL )
	{
		giftinfo.strSrcHead = GetUserHead( pSrcUserItem ) ;
	}
	IUserItem* pDstUserItem =  m_pRoomBusiness->GetUserManager()->SearchUserByUIN( giftinfo.dstuin ) ;
	if ( pDstUserItem != NULL )
	{
		giftinfo.strDstHead = GetUserHead( pDstUserItem ) ;
	}

	CString strTemp ;
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( giftinfo.unGiftid ) ;
	if ( pGiftItem != NULL )
	{	

		CString strGift ;
		strGift.Format( L" %s %s，共%d%s" , pGiftItem->GetName().c_str() , 
			Img( pGiftItem->GetLargeImagePath().c_str() ) , unGiftCount , pGiftItem->GetUnitName().c_str() ) ;

		//发送着名称
		CString strNameHrefLeft ;
		CString strSrcNameAndID;
		if (giftinfo.src179ID > MAX_SHOW_179ID)
		{
			strSrcNameAndID = giftinfo.strSrcName.c_str();
		}
		else
		{
			strSrcNameAndID.Format(L"%s(%u)", giftinfo.strSrcName.c_str(), giftinfo.src179ID);
		}
		strNameHrefLeft.Format(_T("<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\" title=\"%d\" class=\"user\" >%s</a>") , 
			L"" , giftinfo.strSrcName.c_str() ,
			giftinfo.srcuin , CLICK_TYPE_NAME ,giftinfo.src179ID ,
			strSrcNameAndID ) ;

		//接受者
		CString strNameHrefRight; 
		CString strDstNameAndID;
		if (giftinfo.dst179ID > MAX_SHOW_179ID)
		{
			strDstNameAndID = giftinfo.strDstName.c_str();
		}
		else
		{
			strDstNameAndID.Format(L"%s(%u)", giftinfo.strDstName.c_str(), giftinfo.dst179ID);
		}
		strNameHrefRight.Format(_T("<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\" title=\"%d\" class=\"user\" >%s</a>") , 
			L"" , giftinfo.strDstName.c_str() ,
			giftinfo.dstuin , CLICK_TYPE_NAME ,giftinfo.dst179ID ,
			strDstNameAndID ) ;

		strTemp = Font(L"宋体", COLOR_NORMAL ,Span(12,giftinfo.strSrcHead + strNameHrefLeft+L" 送给 "+giftinfo.strDstHead + strNameHrefRight+strGift) ) ;
		strTemp += L"<BR>" ;
	}

	return strTemp ;
}


CString HtmlBuild::GetPublicSheChiGift( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem , uint16 unGiftID , uint32 unCount ) 
{
	CString strTemp ;
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	if ( pGiftItem != NULL )
	{		
		CString strGift ;
		//strGift.Format( L" %s %s，共%d%s" , pGiftItem->GetName().c_str() , 
		//	Img( pGiftItem->GetLargeImagePath().c_str() ) , unCount , pGiftItem->GetUnitName().c_str() ) ;
		strGift.Format( L" %s %s，共%d%s" , pGiftItem->GetRichName().c_str() , 
			Img( pGiftItem->GetLargeImagePath().c_str() ) , unCount , pGiftItem->GetUnitName().c_str() ) ;

		CString strChatLeft = ChatUserItem( pSrcUserItem  ) ;
		CString strChatRight = ChatUserItem(pDstUserItem  ) ;
		strTemp = Font( L"宋体" , COLOR_SHECHI , Span( 14 , strChatLeft+L" "+ CString(pGiftItem->GetStrSendDesc().c_str())+ L" "+strChatRight +strGift) ) ;
		strTemp += L"<BR>" ;
	}

	return strTemp ;
}

CString HtmlBuild::GetChat( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem ,
						   CString& strText ,MessageFont& stFont , bool bPublic , bool bReplaceSelf , bool bAutoMsg) 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	CString strTemp ;

	CString strChatLeft = ChatUser( myCurInfo.unUIN , pSrcUserItem , bReplaceSelf ) ;
	CString strChatRight = ChatUser( myCurInfo.unUIN , pDstUserItem ,bReplaceSelf ) ;
	if ( bPublic )
	{
		if ( pDstUserItem == NULL )
		{
			//对所有人
			strTemp =Font( L"宋体",L"#000000" ,Span(12 ,strChatLeft + L" 说：") +ChatMessage(strText,stFont,pSrcUserItem->GetUserUIN() ) ) ;
		}
		else
		{
			if ( strChatLeft == L"您" )
			{
				strTemp =Font( L"宋体",L"#000000" ,Span(12 ,strChatLeft + L"对 "+strChatRight+L" 说：") +ChatMessage(strText,stFont,pSrcUserItem->GetUserUIN() ) ) ;
			}
			else if ( strChatRight == L"您")
			{
				strTemp =Font( L"宋体",L"#000000" ,Span(12 ,strChatLeft + L" 对"+strChatRight+L"说：") +ChatMessage(strText,stFont,pSrcUserItem->GetUserUIN() ) ) ;
			}
			else
			{
				strTemp =Font( L"宋体",L"#000000" ,Span(12 ,strChatLeft + L" 对 "+strChatRight+L" 说：") +ChatMessage(strText,stFont,pSrcUserItem->GetUserUIN() ) ) ;
			}			
		}
		
	}
	else
	{
		if ( strChatLeft == L"您" )
		{
			strTemp =Font( L"宋体",L"#000000" ,Span(12,strChatLeft+L"对 "+strChatRight+L" 悄悄地说："+ ChatMessage(strText,stFont,pSrcUserItem->GetUserUIN(),bAutoMsg))) ;
		}
		else if ( strChatRight == L"您" )
		{
			strTemp =Font( L"宋体",L"#000000" ,Span(12,strChatLeft+L" 对"+strChatRight+L"悄悄地说："+ ChatMessage(strText,stFont,pSrcUserItem->GetUserUIN(),bAutoMsg))) ;
		}		
	}

	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetPrivateTips( const CString& strTitle , const CString& strValue ,  const CString& strColor )
{
	CString strTemp = Font( L"宋体" , strColor , Span(12,strTitle+strValue) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetPublicTips( const CString& strTitle , const CString& strValue ,  const CString& strColor ) 
{
	CString strTemp = Font( L"宋体" , strColor , Span(12,strTitle+strValue) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetOfficialNocice( CString& strValue ) 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strOfficeNoticePngPath = strModulePath + IMG_OFFICE_NOTICE_PNG_PATH;	

	CString strTemp = Img( strOfficeNoticePngPath.c_str() ) ;
	strTemp += Font( L"宋体" , COLOR_NORMAL ,Span(12 , L" "+strValue) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetNotice( const CString& strValue ) 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strNoticePngPath = strModulePath + IMG_NOTICE_PNG_PATH;	

	CString strTemp = Img( strNoticePngPath.c_str() ) ;
	strTemp += Font( L"宋体" , COLOR_NORMAL ,Span(12 , strValue) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetNotice(const CString& strValue ,MessageFont& stFont) 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strNoticePngPath = strModulePath + IMG_NOTICE_PNG_PATH;	

	CString strTemp = Img( strNoticePngPath.c_str() ) ;
	strTemp += Font( stFont.font_type.c_str() ,GetColorString(stFont.font_color) ,Span(stFont.font_size , strValue) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetTempNotice( uint32 un179ID ,CString strSrcDecodeName ,const CString& strValue , MessageFont& stFont ) 
{
	CString strName ;
	strName.Format( L"--【%s】(%u)发布",strSrcDecodeName ,un179ID ) ;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strTempNoticePngPath = strModulePath + IMG_TEMPNOTICE_PNG_PATH;

	CString strTemp ;
	strTemp = Img( strTempNoticePngPath.c_str() ) ;
	strTemp += Font( stFont.font_type.c_str() ,GetColorString(stFont.font_color) ,Span(stFont.font_size ,  GetFontText( stFont , strValue ) ) ) ;
	strTemp += Font( L"宋体" ,COLOR_GIFT ,Span(12,strName)) ;
	strTemp += L"<BR>" ;

	return strTemp ;
}

CString HtmlBuild::GetEnterRoomGetUserList_Blink( IUserItem * pUserItem ) 
{
	CString strHtml ;
	//IGiftItem* pBlinkItem = GiftManager::GetInstance()->GetBlinkEnterItem() ;
	IGiftItem * pBlinkItem = GiftManager::GetInstance()->SearchBlinkEnterByID(pUserItem->GetBlinkEnterID());
	if ( pUserItem->IsBlinkEnter() && pBlinkItem != NULL )
	{
		CString strImgPath = Img( pBlinkItem->GetLargeImagePath().c_str());
		CString strTemp ;
		CString strCrestImgPath , strStampImgPath, strRichLevelImgPath = L"";	
		if ( pUserItem->GetUserStampID() != 0 )
		{
			//盖章 图片
			IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchStampByID( pUserItem->GetUserStampID() ) ;
			if ( pGiftItem != NULL )
			{
				strStampImgPath = Img( pGiftItem->GetSmallImagePath().c_str() ) ;
			}
		}
		//if (pUserItem->GetUserRichLevel() > 0)
		//{
		//	std::wstring richLevelIconPath = RichLevelManager::GetRichLevelImgPath(pUserItem);
		//	if ( !richLevelIconPath.empty() )
		//	{
		//		strRichLevelImgPath = Img( richLevelIconPath.c_str() );
		//	}
		//}
		if ( pUserItem->GetUserVipLevel() >= core::VIPLEVEL_SILVER )
		{
			//皇冠用户/官方代理 图片
			std::wstring strIconPath = ListIconManager::GetOutputIcon(pUserItem) ;
			if ( !strIconPath.empty() )
			{
				strCrestImgPath = Img( strIconPath.c_str() );
			}	
			
			//盖章图片 冠图标 冠名称 昵称 账号
			strTemp.Format( L"%s%s，%s%s %s %s %s %s" ,strImgPath , pBlinkItem->GetStrSendDesc().c_str(), strStampImgPath ,
				strCrestImgPath , GetVipName( pUserItem->GetUserVipLevel() ) , strRichLevelImgPath , GetNameAndID(pUserItem), pBlinkItem->GetName().c_str() ) ;
		}
		else
		{
			strTemp.Format( L"%s%s，%s %s %s %s" ,strImgPath  , pBlinkItem->GetStrSendDesc().c_str(),
				strStampImgPath, strRichLevelImgPath , GetNameAndID(pUserItem), pBlinkItem->GetName().c_str() ) ;
		}

		strHtml = Font( L"宋体" , COLOR_GIFT ,Span( 12,Bold(strTemp) ) ) ;
		strHtml += L"<BR>" ;
	}
	return strHtml ;
}

//至尊皇冠
CString HtmlBuild::GetEnterRoomGetUserList_VIPLevelTop( IUserItem* pUserItem ) 
{
	CString strHtml ;
	if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() == core::VIPLEVEL_TOP )
	{
		strHtml = EnterMgr->GetEnterHtmlByVipLevel( pUserItem->GetUserVipLevel() ) ;
		if ( !strHtml.IsEmpty() )
		{
			CString strTemp ;
			strTemp.Format( L" %s "  , GetNameAndID(pUserItem) ) ;
			strHtml.Replace( L"VipImage" , GetUserItemHeadImg( pUserItem ) ) ;
			//strHtml.Replace( L"RichLevelImage" , GetUserItemRichLevelImg( pUserItem ) ) ;
			strHtml.Replace( L"name" , strTemp ) ;
			strHtml += L"<BR>" ;

		}
	}
	return strHtml ;
}

//钻石皇冠
CString HtmlBuild::GetEnterRoomGetUserList_VIPLevelDiamond( IUserItem* pUserItem ) 
{
	CString strHtml ;
	if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() == core::VIPLEVEL_DIAMOND )
	{
		strHtml = EnterMgr->GetEnterHtmlByVipLevel( pUserItem->GetUserVipLevel() ) ;
		if ( !strHtml.IsEmpty() )
		{
			CString strTemp ;
			strTemp.Format( L" %s "  , GetNameAndID(pUserItem) ) ;
			strHtml.Replace( L"VipImage" , GetUserItemHeadImg( pUserItem ) ) ;
			//strHtml.Replace(L"RichLevelImage", GetUserItemRichLevelImg(pUserItem));
			strHtml.Replace( L"name" , strTemp ) ;
			strHtml += L"<BR>" ;
		}
	}
	return strHtml ;
}

//冠用户
CString HtmlBuild::GetEnterRoomGetUserList_Crest( IUserItem * pUserItem ) 
{
	CString strHtml ;
	if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() > core::VIPLEVEL_HONOR && pUserItem->GetUserVipLevel() < core::VIPLEVEL_DIAMOND)
	{
		strHtml = EnterMgr->GetEnterHtmlByVipLevel( pUserItem->GetUserVipLevel() ) ;
		if ( !strHtml.IsEmpty() )
		{
			CString strTemp ;
			strTemp.Format( L" %s "  , GetNameAndID(pUserItem) ) ;
			strHtml.Replace( L"VipImage" , GetUserItemHeadImg( pUserItem ) ) ;
			//strHtml.Replace( L"RichLevelImage" , GetUserItemRichLevelImg( pUserItem ) ) ;
			strHtml.Replace( L"name" , strTemp ) ;
			strHtml += L"<BR>" ;

		}
	}
	return strHtml ;
}


CString HtmlBuild::GetEnterRoomGetUserList_LuckyStar( IUserItem* pUserItem ) 
{
	return GetLuckyStarEnterRoom( pUserItem ) ;
}

CString HtmlBuild::GetGiftStar(IUserItem* pUserItem, uint16 unGiftID)
{
	CString strHtml ;
	room_user_info* userInfo = pUserItem->GetUserInfo();

	IGiftItem* pGiftItem = NULL;
	pGiftItem = GiftManager::GetInstance()->SearchGiftByID(unGiftID);
	if (pGiftItem)
	{
		CString strImgPath = Img( pGiftItem->GetLargeImagePath().c_str());
		CString strGiftName = pGiftItem->GetName().c_str();
		CString strTemp;
		strTemp.Format(L"%s%s之星 %s 光彩夺目，令房间蓬荜生辉", strImgPath, strGiftName, GetNameAndID(pUserItem));
		strHtml = GetPublicTips( L"【提示】： " , strTemp ,COLOR_GIFT ); 
	}

	return strHtml ;
}

CString HtmlBuild::GetEnterRoom( IUserItem * pUserItem )
{
	//CString strHtml = GetLuckyStarEnterRoom( pUserItem );
	CString strHtml = L"";
	CString strTemp ;
	//IGiftItem* pBlinkItem = GiftManager::GetInstance()->GetBlinkEnterItem() ;
	IGiftItem * pBlinkItem = GiftManager::GetInstance()->SearchBlinkEnterByID(pUserItem->GetBlinkEnterID());
	if ( pUserItem->IsBlinkEnter() && pBlinkItem != NULL )
	{
		strHtml = GetLuckyStarEnterRoom(pUserItem);
		CString strImgPath = Img( pBlinkItem->GetLargeImagePath().c_str());	
		//std::wstring strRichLevelIcon = L"";
		CString strRichLevelPath = L"";
		//if (pUserItem->GetUserRichLevel() > 0)
		//{
		//	strRichLevelIcon = RichLevelManager::GetRichLevelImgPath(pUserItem);
		//	strRichLevelPath = Img(strRichLevelIcon.c_str());
		//}
		strTemp.Format( L"%s%s，%s%s %s%s" ,strImgPath , pBlinkItem->GetStrSendDesc().c_str(), GetUserItemHeadImg( pUserItem ), strRichLevelPath , GetNameAndID(pUserItem), pBlinkItem->GetName().c_str()) ;
		strHtml +=Font( L"宋体" , COLOR_GIFT ,Span(12 ,Bold(strTemp) )  ) ;
		strHtml += L"<BR>" ;
	}
	else
	{
		if ( pUserItem->GetUserVipLevel() >= VIPLEVEL_SILVER )
		{
			CString strHtmlVip = EnterMgr->GetEnterHtmlByVipLevel( pUserItem->GetUserVipLevel() ) ;
			if ( !strHtmlVip.IsEmpty() )
			{				 
				CString strTemp ;
				strTemp.Format( L" %s "  , GetNameAndID(pUserItem) ) ;
				strHtmlVip.Replace( L"VipImage" , GetUserItemHeadImg( pUserItem ) ) ;
				//strHtmlVip.Replace(L"RichLevelImage", GetUserItemRichLevelImg(pUserItem));
				strHtmlVip.Replace( L"name" , strTemp ) ;
				strHtmlVip += L"<BR>" ;
				strHtml += strHtmlVip ;
			}
		}
		else if ( m_pRoomBusiness->GetRoomOption()->IsShowUserInOut() && !CPersonalConfig::GetInstance()->GetIsBlockUserInOutMsg() )
		{			
			strTemp.Format( L"%s 进入房间" , GetNameAndID(pUserItem) ) ;
			strHtml += GetPublicTips( L"【提示】： " , strTemp ,COLOR_GIFT ); 
		}
		strHtml += GetLuckyStarEnterRoom(pUserItem);
	}

	uint8 unEventEnterIndex = EventEnterMgr->GetEventIndex(pUserItem->GetUser179ID());
	if (unEventEnterIndex != 0)
	{
		strHtml += GetEnterRoomGetUserList_Event( pUserItem, unEventEnterIndex ) ;
	}

	std::vector<core::GiftStarInfo> vecInfo = pUserItem->GetGiftStarInfo();
    for (std::vector<core::GiftStarInfo>::iterator iter = vecInfo.begin(); iter != vecInfo.end(); ++iter)
    {
		IGiftItem* pGiftItem = NULL;
		pGiftItem = GiftManager::GetInstance()->SearchGiftByID(iter->unGiftID);
		if (pGiftItem)
		{
			CString strImgPath = Img( pGiftItem->GetLargeImagePath().c_str());
			strTemp.Format(L"%s%s之星 %s 光彩夺目，令房间蓬荜生辉", strImgPath, pGiftItem->GetName().c_str(), GetNameAndID(pUserItem));
			strHtml += GetPublicTips( L"【提示】： " , strTemp ,COLOR_GIFT ); 
		}
	}

	return strHtml ;
}

CString HtmlBuild::GetLeaveRoom( IUserItem* pUserItem ) 
{

	CString strTemp ;
	strTemp.Format( L"%s %s 离开本房间" , GetUserItemHeadImg( pUserItem ) , GetNameAndID(pUserItem) ) ;
	return GetPublicTips( L"【提示】：  " , strTemp ,COLOR_GIFT );
}

CString HtmlBuild::ChatUser( uint32 nSelfUUID , IUserItem* pUserItem , bool bReplaceSelf  )
{
	CString strTemp ;
	if ( pUserItem == NULL )
	{
		return strTemp ;
	}
	else if ( pUserItem->GetUserUIN() == nSelfUUID && bReplaceSelf )
	{
		strTemp = L"您"  ;
	}
	else
	{
		strTemp = ChatUserItem( pUserItem  ) ;
	}

	return strTemp ;
}

CString HtmlBuild::ChatUserItem( IUserItem* pUserItem ) 
{
	CString strTemp ;
	if ( pUserItem != NULL )
	{
		strTemp = GetUserHead( pUserItem ) ;		
		CString strNameHref ;
		strNameHref.Format(_T("<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\" title=\"%d\" class=\"user\" >%s</a>") , 
			L"" , pUserItem->GetDecodeUserName().c_str() ,
			pUserItem->GetUserUIN() , CLICK_TYPE_NAME ,pUserItem->GetUser179ID() ,
			GetNameAndID(pUserItem) ) ;
		strTemp += strNameHref ;
	}
	return strTemp ;
}

CString HtmlBuild::StampRightUserItem( IUserItem* pUserItem ) 
{
	CString strTemp ;
	if (!pUserItem->IsOfficial())
	{
		std::wstring strVipPath = ListIconManager::GetVIPIcon(pUserItem);
		if ( !strVipPath.empty() )
		{
			strTemp += Img( strVipPath.c_str() ) ;
		}
	}

	if ( pUserItem->IsActor() || pUserItem->IsOfficial())
	{
		std::wstring strIconPath = ListIconManager::GetOutputIcon(pUserItem) ;
		if ( !strIconPath.empty() )
		{
			//皇冠用户/官方代理 图片
			strTemp += Img( strIconPath.c_str() ) ;
		}
	}
	//if ( pUserItem->GetUserRichLevel() > 0 )
	//{
	//	wstring strIcon = RichLevelManager::GetRichLevelImgPath(pUserItem);
	//	if ( !strIcon.empty() )
	//	{
	//		strTemp += Img( strIcon.c_str() ) ;
	//	}
	//}
	if ( pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_TEMP_MANAGER && !(pUserItem->IsActor() || pUserItem->IsOfficial()))
	{
		wstring strIcon = ListIconManager::GetThirdColumnIcon( pUserItem ) ;
		if ( !strIcon.empty() )
		{
			strTemp += Img( strIcon.c_str() ) ;
		}
	}
	CString strNameHref ;
	strNameHref.Format(_T("<a href=\"%s\" name=\"%s\" uuid=\"%u\" type=\"%d\" title=\"%d\" class=\"user\" > %s</a>") , 
		L"" , pUserItem->GetDecodeUserName().c_str() ,
		pUserItem->GetUserUIN() , CLICK_TYPE_NAME ,pUserItem->GetUser179ID() ,
		GetNameAndID(pUserItem) ) ;
	strTemp += strNameHref ;

	return strTemp ;
}

CString HtmlBuild::PrivateUserItem( IUserItem* pUserItem , BOOL bStamp ) 
{
	CString strTemp ,strTitleImg;
	if ( pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_TEMP_MANAGER && !(pUserItem->IsActor() || pUserItem->IsOfficial()))
	{
		wstring strIcon = ListIconManager::GetThirdColumnIcon( pUserItem ) ;
		if ( !strIcon.empty() )
		{
			strTitleImg += Img( strIcon.c_str() ) ;
		}
	}
	strTemp.Format( L"%s%s%s %s " , GetUserItemHeadImg( pUserItem , bStamp ) ,GetUserItemRichLevelImg(pUserItem) , strTitleImg, GetNameAndID(pUserItem) ) ;
	return strTemp ;
}

CString HtmlBuild::GetUserItemHeadImg( IUserItem* pUserItem , BOOL bStamp ) //获得用户名前的章和冠图片
{
	CString strTemp ;
	if ( pUserItem != NULL )
	{
		if ( bStamp && pUserItem->GetUserStampID() != 0 )
		{
			//盖章 图片
			IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchStampByID( pUserItem->GetUserStampID() ) ;
			if ( pGiftItem != NULL )
			{
				strTemp += Img( pGiftItem->GetSmallImagePath().c_str() ) ;
			}
		}

		if (!pUserItem->IsOfficial())
		{
			std::wstring strVipPath = ListIconManager::GetVIPIcon(pUserItem);
			if ( !strVipPath.empty() )
			{
				strTemp += Img( strVipPath.c_str() ) ;
			}
		}

		if ( pUserItem->IsActor() || pUserItem->IsOfficial() )
		{
			std::wstring strIconPath = ListIconManager::GetOutputIcon(pUserItem) ;
			if ( !strIconPath.empty() )
			{
				//皇冠用户/官方代理 图片
				strTemp += Img( strIconPath.c_str() ) ;
			}
		}
	}
	return strTemp ;
}

CString HtmlBuild::GetUserItemRichLevelImg(IUserItem* pUserItem) //获得用户富豪等级图标
{
	return L"";
	//CString strTemp ;
	//if ( pUserItem != NULL )
	//{
	//	if ( pUserItem->GetUserRichLevel() > 0 )
	//	{
	//		std::wstring strIconPath = RichLevelManager::GetRichLevelImgPath(pUserItem);
	//		if (!strIconPath.empty())
	//		{
	//			strTemp += Img(strIconPath.c_str());
	//		}
	//	}
	//}
	//return strTemp ;
}

CString HtmlBuild::GetAutoWelcome( IUserItem* pUserItem , CString& strText ) 
{
	MessageFont stFont ;
	stFont.font_type = L"宋体" ;
	stFont.font_color = 0 ;
	stFont.font_size = 12 ;
	stFont.font_style = core::FONTSTYLETYPE_NONE ;

	CString strTemp =Font( L"宋体", COLOR_NORMAL ,Span( 12 , ChatUserItem( pUserItem )+ L" 说： ") + ChatMessage(strText,stFont,pUserItem->GetUserUIN()) ) ;
	strTemp += L"<BR>" ;

	return strTemp ;
}

CString HtmlBuild::GetVisistorRegistor( IUserItem* pUserItem , CString& strText ) 
{

	MessageFont stFont ;
	stFont.font_type = L"宋体" ;
	stFont.font_color = 0 ;
	stFont.font_size = 12 ;
	stFont.font_style = core::FONTSTYLETYPE_NONE ;

	CString strTemp =Font( L"宋体", COLOR_NORMAL ,Span( 12 , ChatUserItem( pUserItem )+ L" 对您悄悄地说： ") + ChatMessage(strText,stFont,pUserItem->GetUserUIN()) ) ;
	strTemp += L"<BR>" ;

	return strTemp ;
}

CString HtmlBuild::ChatMessage( CString& strText , MessageFont& stFont , uint32 srcUin, bool bAutoMsg) 
{
	CString strTemp ;
	if ( stFont.font_style == FONTSTYLETYPE_COLORTEXT )
	{
		//彩字
		CaiZiItem *pCaiZiItem = CaiZiMgr->GetCaiZiItemByIndex( _wtoi( strText ) ) ;
		if ( pCaiZiItem != NULL )
		{
			strTemp = Span( 12 , pCaiZiItem->GetCaiZiHtml().c_str() ) ;
		}
	}
	else
	{
		strTemp = Font( stFont.font_type.c_str() ,GetColorString(stFont.font_color) ,Span(stFont.font_size , GetFontText( stFont , strText ) ) ) ;
		CString strNoRemind;
		if (bAutoMsg)
		{
			strNoRemind.Format( L"<SPAN> <a href=\"%s\" name=\"%s\" type=\"%d\" uuid=\"%d\" >不再提醒</a></SPAN>",L"" ,L"autoReply", CLICK_TYPE_NO_REMIND,srcUin);
			strTemp += strNoRemind;
		}
		strTemp += Span( 12 , _T("#333333") ,GetCurrentTimeString() ) ;
	}
	return strTemp ;
}

CString HtmlBuild::Img( CString strImgPath ) 
{
	CString strTemp ;
	strTemp.Format( L"<IMG src=\"%s\">" ,strImgPath ) ;
	return strTemp ;
}

CString HtmlBuild::ItaLic( const CString& strValue ) 
{
	CString strTemp ;
	strTemp.Format(_T("<I>%s</I>") , strValue ) ;
	return strTemp ;
}

CString HtmlBuild::UnderLine(const CString& strValue )
{
	CString strTemp ;
	strTemp.Format(_T("<U>%s</U>") , strValue ) ;
	return strTemp ;
}

CString HtmlBuild::Bold(const CString& strValue ) 
{
	CString strTemp ;
	strTemp.Format(_T("<B>%s</B>") , strValue ) ;
	return strTemp ;
}

CString HtmlBuild::Span(int nFontSize , const CString& strColor ,const CString& strValue ) 
{
	CString strTemp ;
	strTemp.Format(_T("<SPAN style=FONT-SIZE:%dpx;color:%s>%s</SPAN>"),nFontSize ,strColor,strValue) ;
	return strTemp ;
}

CString HtmlBuild::Span(int nFontSize ,const CString& strValue ) 
{
	CString strTemp ;
	strTemp.Format(_T("<SPAN style=FONT-SIZE:%dpx>%s</SPAN>") ,nFontSize , strValue ) ;
	return strTemp ;
}

CString HtmlBuild::Font(const CString& strFont , const CString& strColor , const CString& strValue ) 
{
	CString strTemp ;
	strTemp.Format(_T("<FONT face=%s color=%s>%s</FONT>"),strFont , strColor , strValue ) ;
	return strTemp ;
}

CString HtmlBuild::Marquee(const CString& strValue ) 
{
	CString strTemp ;
	strTemp.Format(_T("<marquee id=content direction=left scrollamount=6 onmouseover=this.stop() onmouseout=this.start()>%s</marquee>") ,strValue ) ;
	return strTemp ;
}

//颜色转换成字符串
CString HtmlBuild::GetColorString( COLORREF cf ) 
{
	CString strColor ;
	strColor.Format( _T("#%02X%02X%02X") , GetRValue(cf),GetGValue(cf) , GetBValue(cf) ) ;
	return strColor ;
}

//得到时间
CString HtmlBuild::GetCurrentTimeString() 
{
	CString strTime ;
	CTime mtime = CTime::GetCurrentTime() ;
	strTime.Format( L" (%.2d:%.2d:%.2d)" , mtime.GetHour() , mtime.GetMinute() , mtime.GetSecond() ) ;
	return strTime ;
}

CString HtmlBuild::GetGiftDateString( uint32 unTime ) 
{
	CString strTime ;
	time_t szTime = unTime ;
	struct tm *pTime = localtime( &szTime ) ;
	if ( pTime != NULL )
	{
		strTime.Format( L"【%.2d.%.2d %.2d:%.2d】" ,pTime->tm_mon+1 ,pTime->tm_mday , pTime->tm_hour ,pTime->tm_min ) ;
	}
	return strTime ;
}

CString HtmlBuild::GetFontText( MessageFont& stFont ,const CString& strText ) 
{
	CString strTextTemp = strText ;
	if(stFont.font_style & core::FONTSTYLETYPE_BOLD)
	{
		strTextTemp = Bold( strTextTemp ) ;
	}
	if( stFont.font_style & core::FONTSTYLETYPE_ITALICS)
	{
		strTextTemp = ItaLic( strTextTemp ) ;
	}
	if(stFont.font_style & core::FONTSTYLETYPE_UNDERLINE)
	{
		strTextTemp = UnderLine( strTextTemp ) ;
	}
	return strTextTemp ;
}

CString HtmlBuild::GetLuckyTimesPngPath( uint16 unLuckyTimes ) 
{
	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	std::wstring strMultiplePngPath ;
	switch( unLuckyTimes )
	{
	case 100:
		{
			strMultiplePngPath = strModulePath + IMG_100MULTIPLE_GIF_PATH;
		}
		break;
	case 500:
		{
			strMultiplePngPath = strModulePath + IMG_500MULTIPLE_GIF_PATH;	
		}
		break;
	case 800:
		{
			strMultiplePngPath = strModulePath + IMG_800MULTIPLE_PNG_PATH;	
		}
		break;
	case 1000:
		{
			strMultiplePngPath = strModulePath + IMG_1000MULTIPLE_GIF_PATH;	
		}
		break;
	case 1500:
		{
			strMultiplePngPath = strModulePath + IMG_1500MULTIPLE_PNG_PATH;	
		}
		break;
	case 2000:
		{
			strMultiplePngPath = strModulePath + IMG_2000MULTIPLE_PNG_PATH ;
		}
		break;
	case 2500:
		{
			strMultiplePngPath = strModulePath + IMG_2500MULTIPLE_PNG_PATH;	
		}
		break;
	case 3000:
		{
			strMultiplePngPath = strModulePath + IMG_3000MULTIPLE_PNG_PATH;	
		}
		break;
	case 5000:
		{
			strMultiplePngPath = strModulePath + IMG_5000MULTIPLE_GIF_PATH;				
		}
		break;
	default:break ;
	}	
	return strMultiplePngPath.c_str() ;
}


CString HtmlBuild::GetVipName( uint8 nVipLevel ) 
{
	CString strVipName ;
	switch( nVipLevel )
	{
	case core::VIPLEVEL_VIP:
		{
			strVipName = L"VIP" ;
		}
		break;
	case core::VIPLEVEL_HONOR:
		{
			strVipName = L"荣耀VIP" ;
		}
		break;
	case core::VIPLEVEL_SILVER:
		{
			strVipName = L"银色皇冠" ;
		}
		break;
	case core::VIPLEVEL_GOLDEN:
		{
			strVipName = L"金色皇冠" ;
		}
		break;
	case core::VIPLEVEL_CRYSTAL:
		{
			strVipName = L"水晶皇冠" ;
		}
		break;
	case core::VIPLEVEL_DIAMOND:
		{
			strVipName = L"钻石皇冠" ;
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:
		{
			strVipName = L"黑钻皇冠" ;
		}
		break;
	case core::VIPLEVEL_TOP:
		{
			strVipName = L"至尊皇冠" ;
		}
		break;
	default:break;
	}

	return strVipName ;
}

CString HtmlBuild::GetTitleName( uint16 nTitleType ) 
{
	CString strTitle ;
	switch( nTitleType )
	{
	case core::TITLELEVEL_TEMP_MANAGER:
		{
			strTitle = L"临时管理员" ;
		}
		break; 
	case core::TITLELEVEL_MANAGER:
		{
			strTitle = L"管理员" ;
		}
		break;
	case core::TITLELEVEL_SECOND_OWNER:
		{
			strTitle = L"副室主" ;
		}
		break;
	case core::TITLELEVEL_OWNER:
		{
			strTitle = L"室主" ;
		}
		break;
	case core::TITLELEVEL_SUPER_MANAGER:
		{
			strTitle = L"超级管理员" ;
		}
		break;
	default: break;
	}
	return strTitle ;
}

CString HtmlBuild::GetSpeaker( CString& strText, MessageFont& stFont, SpeakerInfo& stInfo )
{
	return SpeakerUserItem(stInfo) + L":<BR>" + SpeakerMessage(strText, stFont);
}

//房主在线时的欢迎辞
CString HtmlBuild::GetWelecomeWords( IUserItem* pOwnerItem , CString& strText , MessageFont& stFont ) 
{
	CString strTemp = Font( L"宋体" ,COLOR_NORMAL , Span(12 ,ChatUserItem(pOwnerItem)+
		L" 对您悄悄地说： "+ChatMessage(strText,stFont,pOwnerItem->GetUserUIN()) ) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetActorAutoSpeakWords(IUserItem* pUserItem, CString& strText)
{
	CString strTemp = Font( L"宋体" , L"#000000" , Span(12, ChatUserItem(pUserItem) + L" 对您悄悄地说： " + strText) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetAgentAutoSpeakWords(IUserItem* pUserItem, CString& strText)
{
	CString strTemp = Font( L"宋体" , L"#000000" , Span(12, ChatUserItem(pUserItem) + L" 对您悄悄地说： " + strText) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::OnNotifyPlutusAward( uint32 srcuin , uint32 un179ID , 
									   std::wstring& strDecodeName ,std::wstring strRoomName ,
									   uint16 nSelfTitleLevel , int nNumber ,uint16 unGiftID ) 
{
	CString strTemp ;
	if ( nSelfTitleLevel > core::TITLELEVEL_TEMP_MANAGER )
	{
		//管理员		
		CString strHref ;
		IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
		if ( pGiftItem != NULL )
		{
			CString strImgPath = Img( pGiftItem->GetSmallImagePath().c_str() ) ;

			CString strNameAndID;
			if (un179ID > MAX_SHOW_179ID)
			{
				strNameAndID = strDecodeName.c_str();
			}
			else
			{
				strNameAndID.Format(L"%s(%u)", strDecodeName.c_str(), un179ID);
			}

			CString strHrefName ;
			strHrefName.Format( L"%s%s的诚心感动了财神，10秒后财神下凡派发元宝，每人都有%d次获得赏赐的机会，黄金满地，快来抢啊！" ,
				strRoomName.c_str(),strNameAndID ,nNumber ) ;
			strHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%u\" type=\"%d\">转发到临时公告</a>" ,
				L"" ,strHrefName , srcuin , CLICK_TYPE_TEMP_NOTICE) ;

			CString strInfo ;
			strInfo.Format( L"%s%s的诚心感动了 财神%s，10秒后财神下凡派发元宝，每人都有%d次获得赏赐的机会，黄金满地，快来抢啊！" ,
				strRoomName.c_str() ,strNameAndID ,strImgPath ,nNumber ) ;
			strTemp = Font( L"宋体" ,COLOR_GIFT ,Span(12,L"【提示】： ")+Span( 12,strInfo)+ Span(12 ,strHref)  ) ;
			strTemp += L"<BR>" ;
		}
	}
	else
	{		
		//非管理员
		CString strInfo ;
		CString strImgPath ;
		IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
		if ( pGiftItem != NULL )
		{

			CString strNameAndID;
			if (un179ID > MAX_SHOW_179ID)
			{
				strNameAndID = strDecodeName.c_str();
			}
			else
			{
				strNameAndID.Format(L"%s(%u)", strDecodeName.c_str(), un179ID);
			}
			strInfo.Format( L"%s的诚心感动了 财神%s，10秒后财神下凡派发元宝，每人都有%d次获得赏赐的机会，黄金满地，快来抢啊！" ,
				strNameAndID, strImgPath, nNumber ) ;
			strTemp = Font( L"宋体" ,COLOR_GIFT , Span(12,L"【提示】： "+strInfo) ) ;
			strTemp += L"<BR>" ;
		}
	}
	return strTemp ;
}

//获取显示名字前的信息
CString HtmlBuild::GetUserHead( IUserItem* pUserItem ) 
{
	CString strTemp = GetUserItemHeadImg( pUserItem ) ;	

	//if ( pUserItem->GetUserRichLevel() > 0 )
	//{
	//	wstring strIcon = RichLevelManager::GetRichLevelImgPath(pUserItem);
	//	if ( !strIcon.empty() )
	//	{
	//		strTemp += Img( strIcon.c_str() ) ;
	//	}
	//}

	if ( pUserItem->GetUserTitleLevel() >= core::TITLELEVEL_TEMP_MANAGER && !(pUserItem->IsActor() || pUserItem->IsOfficial()))
	{
		wstring strIcon = ListIconManager::GetThirdColumnIcon( pUserItem ) ;
		if ( !strIcon.empty() )
		{
			strTemp += Img( strIcon.c_str() ) ;
		}
	}
	strTemp +=L" " ;
	return strTemp ;
}

CString HtmlBuild::SpeakerUserItem( CString& strSrcName , CString& strSrcHead , uint32 un179ID ,COLORREF cf ) 
{
	CString strTemp = L"";
	CString strName ;
	
	IUserItem* pUserItem = m_pRoomBusiness->GetUserManager()->SearchUserBy179ID(un179ID);
	//if (pUserItem != NULL && !pUserItem->IsOfficial())
	//{
	//	std::wstring strRichLevelImg = RichLevelManager::GetRichLevelImgPath(pUserItem);
	//	strTemp = Img(strRichLevelImg.c_str());
	//}
	CString strSrcNameAndID;
	if (un179ID > MAX_SHOW_179ID)
	{
		strSrcNameAndID = strSrcName;
	}
	else
	{
		strSrcNameAndID.Format(L"%s(%u)", strSrcName, un179ID);
	}
	strName.Format( L"%s%s" ,strTemp, strSrcNameAndID ) ;
	strName = Font(_T("宋体") ,GetColorString(cf), Span(13,strName) );
	return strSrcHead + strName ;
}

CString HtmlBuild::SpeakerUserItem( SpeakerInfo& stInfo, COLORREF cf/* = RGB(0,99,255)*/ )
{
	CString strTemp ;

	if ( stInfo.unStampID != 0 )
	{
		//盖章 图片
		IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchStampByID( stInfo.unStampID ) ;
		if ( pGiftItem != NULL )
		{
			strTemp += Img( pGiftItem->GetSmallImagePath().c_str() ) ;
		}
	}
	if ( (core::ENM_VipLevel)stInfo.unVipLevel>= core::VIPLEVEL_SILVER)
	{
		strTemp += Img( ListIconManager::GetIconPathByVipLevel(stInfo.unVipLevel).c_str() );
	}	

	//if (stInfo.unRichLevel > 0 )
	//{
	//	std::wstring strIcon = RichLevelManager::GetRichLevelImgPathByLevel(stInfo.unRichLevel);
	//	if ( !strIcon.empty())
	//	{
	//		strTemp += Img( strIcon.c_str() ) ;
	//	}
	//}
	//发送者名称
	CString strName;
	CString strTitleName;
	if (stInfo.un179ID > MAX_SHOW_179ID)
	{
		strTitleName = common::utility::stringhelper::XMLEncode(stInfo.strName).c_str();
	}
	else
	{
		strTitleName.Format(L"%s(%u)", common::utility::stringhelper::XMLEncode(stInfo.strName).c_str(), stInfo.un179ID);
	}
	strName.Format(L"%s", strTitleName);
	strName = Font(_T("宋体") ,GetColorString(cf), Span(13,strName) );
	strTemp += strName ;

	return strTemp ;
}

CString HtmlBuild::SpeakerMessage( CString& strText , MessageFont& stFont )
{
	CString strTemp;
	strTemp = Font( stFont.font_type.c_str() ,GetColorString(stFont.font_color) ,Span(stFont.font_size , strText) ) ;
	strTemp += L"<BR>" ;

	return strTemp;
}

CString HtmlBuild::SpeakerSystemMessage( CString& strText )
{
	CString strTemp;
	strTemp = Font( L"宋体" , _T("#0000FF"), Span(13, strText) ) ;
	strTemp += L"<BR>" ;

	return strTemp;
}

CString HtmlBuild::SpeakerLuckyMessage( CString& strText )
{
	CString strTemp;
	strTemp = Font( L"宋体" , _T("#FF0000"), Span(12, strText) ) ;
	strTemp += L"<BR>" ;

	return strTemp;
}

CString HtmlBuild::GetSpeakerLucky( CString& strSrcName ,CString& strSrcLuckyHead , uint32 src179ID ,
								   uint16	unLuckyTimes , uint32 unLuckyMoney ,uint16 unGiftid ) 
{
	IUserItem* pUserItem =  m_pRoomBusiness->GetUserManager()->SearchUserBy179ID( src179ID ) ;
	if ( pUserItem != NULL )
	{
		strSrcLuckyHead = GetUserItemHeadImg( pUserItem ) ;
	}
	CString strTemp , strGiftName ;
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftid ) ;
	if (pGiftItem != NULL)
	{
		strGiftName = pGiftItem->GetName().c_str();
	}
	strTemp.Format( L"送出的 %s 为他赢取了<B>%u</B>倍奖金共<B>%u</B>币" ,strGiftName , unLuckyTimes , unLuckyMoney ) ;
	return SystemMessageTitle() + SpeakerUserItem( strSrcName ,strSrcLuckyHead ,src179ID ) + SpeakerLuckyMessage(strTemp) ;
}


CString HtmlBuild::GetSpeakerLucky( CString& strText, SpeakerInfo& stInfo, uint16 unGiftID )
{
	CString strTimes, strMoney, strName;
	int nPos = strText.Find(L",");
	strTimes = strText.Left(nPos);
	strMoney = strText.Right(strText.GetLength() - nPos - 1);
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	if (pGiftItem != NULL)
	{
		strName = pGiftItem->GetName().c_str();
	}
	CString strTemp;
	strTemp.Format(L"送出的 %s 为他赢取了<B>%s</B>倍奖金共<B>%s</B>币", strName, strTimes, strMoney);
	return SystemMessageTitle() + SpeakerUserItem(stInfo) + SpeakerLuckyMessage(strTemp);
}

CString HtmlBuild::GetSpeakerPlutusAward(CString& strText, SpeakerInfo& stInfo, uint16 unGiftID)
{
	int nPos = strText.Find( L"," ) ;
	CString strRoomName = strText.Left( nPos );
	strText = strText.Right( strText.GetLength() - nPos -1 ) ;
	nPos = strText.Find( L",") ;
	CString strRoomID = strText.Left( nPos ) ;
	CString strNumber = strText.Right( strText.GetLength() - nPos - 1 );
	CString strTemp ;
	strTemp.Format( L"的诚心感动了 财神，<B>“%s(%s)”</B>内每人都有%s次获得赏赐的机会，黄金满地，快来抢呀！" , strRoomName , strRoomID , strNumber ) ;
	return SystemMessageTitle() + SpeakerUserItem(stInfo) + SpeakerLuckyMessage(strTemp);
}

CString HtmlBuild::GetSpeakerSystem( CString& strText )
{
	return SpeakerSystemMessage(strText);
}

CString HtmlBuild::SystemMessageTitle()
{
	CString strTemp;
	strTemp = Img( ListIconManager::GetIconPathByIndex(USERLIST_ICON_SYSTEMMSG).c_str() );
	strTemp += L"<BR>";

	return strTemp;
}

CString HtmlBuild::GetSpeakerFirework( CString& strText,SpeakerInfo& stInfo )
{
	CString strTemp;
	strTemp.Format(L"为\"%s\"房间所有管理员绽放了一束 烟花，\"%s\"房间姹紫嫣红，美不胜收", strText, strText);

	return  SpeakerFireworkTitle() + SpeakerUserItem(stInfo) + SpeakerLuckyMessage(strTemp);
}

CString HtmlBuild::GetSpeakerSalute( CString& strText, SpeakerInfo& stInfo )
{
	CString strTemp;
	strTemp.Format(L"为\"%s\"房间所有用户鸣放了一门 礼炮，\"%s\"房间气氛热烈，盛况空前", strText, strText);

	return SpeakerSaluteTitle() + SpeakerUserItem(stInfo) + SpeakerLuckyMessage(strTemp);
}

CString HtmlBuild::SpeakerFireworkTitle()
{
	return Img( ListIconManager::GetIconPathByIndex(USERLIST_ICON_SMALLSPEAKER).c_str()) + Img( ListIconManager::GetIconPathByIndex(USERLIST_ICON_FIREWORK).c_str());

}

CString HtmlBuild::SpeakerSaluteTitle()
{
	return Img( ListIconManager::GetIconPathByIndex(USERLIST_ICON_SMALLSPEAKER).c_str()) + Img( ListIconManager::GetIconPathByIndex(USERLIST_ICON_SALUTE).c_str());
}

CString HtmlBuild::GetPublicChangeVoice( IUserItem* pSrcUserItem ,IUserItem* pDstUserItem , uint16 unChangeVoiceID )
{
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unChangeVoiceID ) ;
	CString strGiftNameAndImg ;
	if ( pGiftItem != NULL )
	{
		strGiftNameAndImg = pGiftItem->GetName().c_str();
		strGiftNameAndImg += Img( pGiftItem->GetLargeImagePath().c_str() ) ;
	}

	CString strTemp = Font( L"宋体" ,COLOR_NORMAL ,Span(12, ChatUserItem( pSrcUserItem ) +L" 使用“变声之术”将 "+ChatUserItem( pDstUserItem )+L" 变成：" + strGiftNameAndImg) ) ;
	strTemp += L"</BR>" ;
	
	return strTemp ;
}

CString HtmlBuild::GetPrivateChangeVoice( IUserItem* pSrcUserItem , uint16 unChangeVoiceID )
{
	CString strGiftName ;
	//图片
	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unChangeVoiceID ) ;
	if ( pGiftItem != NULL )
	{
		strGiftName =  pGiftItem->GetName().c_str();
	}
	CString strHref ;
	strHref.Format( _T("<a href=\"%s\" name=\"%s\" type=\"%d\" uuid=\"%d\">变声卡</a>") ,
		L"" , pSrcUserItem->GetDecodeUserName().c_str() , CLICK_TYPE_VOICE, pSrcUserItem->GetUserUIN() ) ;
	return GetPrivateTips( L"【提示】： " , L"您被"+PrivateUserItem( pSrcUserItem)+L"变成"+strGiftName+
		L"了，此仇不报非君子，选择"+strHref+L"报仇吧~" ,COLOR_GIFT) ;
}

CString HtmlBuild::GetPublicFlyText( IUserItem * pUserItem, CString strText )
{
	return GetPublicTips( L"", ChatUserItem(pUserItem)+L" 使用飞屏说："+strText+GetCurrentTimeString() );
}

CString HtmlBuild::GetLuckyStarEnterRoom( IUserItem* pUserItem ) 
{
	CString strHtml = L"";
	if ( pUserItem->IsLuckyStar() )
	{
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		common::CURLEncode urlEncode;
		strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
		std::wstring strLuckyStarGifPath = strModulePath + IMG_LUCKYSTAR_GIF_PATH;
		CString strTemp ;
		strTemp.Format( L"%s“超级幸运星” %s 从天而降，驾临本房间。" ,
			Img( strLuckyStarGifPath.c_str() ) , GetNameAndID(pUserItem) ) ;
		strHtml = Font( L"宋体" , COLOR_GIFT , Span(12, Bold(L"【提示】： "+strTemp)) ) ;
		strHtml += L"<BR>" ;
	}
	return strHtml ;
}

//超级幸运星诞生
CString HtmlBuild::GetLuckyStarBirth() 
{	
	return GetPrivateTips( L"【提示】： " , L"恭喜您荣获“超级幸运星”称号，系统赠送给您超炫进场特效，快去看看吧！" ,COLOR_GIFT )  ;
}

CString HtmlBuild::GetPrivateChangeVoiceRsp( IUserItem* pDstUserItem, uint16 unChangeVoiceID )
{
	CString strTemp = L"您使用“变声之术”将 " ;
	strTemp += PrivateUserItem( pDstUserItem ) ;
	strTemp += L" 变成" ;

	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchGiftByID( unChangeVoiceID ) ;
	if ( pGiftItem != NULL )
	{
		strTemp += pGiftItem->GetName().c_str();
		strTemp += Img( pGiftItem->GetLargeImagePath().c_str() ) ;
	}

	return GetPrivateTips( L"【提示】： " , strTemp , COLOR_GIFT ) ;
}

CString HtmlBuild::GetPrivateStampRsp( IUserItem* pDstUserItem, uint16 unStampID )
{

	IGiftItem* pGiftItem = GiftManager::GetInstance()->SearchStampByID( unStampID ) ;
	CString strImg ;
	if ( pGiftItem != NULL )
	{
		strImg = Img( pGiftItem->GetSmallImagePath().c_str() ) ;
	}
	return GetPrivateTips( L"【提示】： " , L"您鉴定 "+ PrivateUserItem(pDstUserItem,FALSE) + L" 为："+ strImg  ,COLOR_GIFT ) ;
}

CString HtmlBuild::GetRequestPublicMicNotify( IUserItem * pSrcUserItem, uint8 unIndex )
{
	CString strName ;	
	strName.Format( L"%s邀请您上公麦，您是" ,GetNameAndID(pSrcUserItem) ) ;
	CString strAcceptHref ;
	strAcceptHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" uindex=\"%d\" type=\"%d\">同意</a>" ,
		L"" , pSrcUserItem->GetDecodeUserName().c_str() , pSrcUserItem->GetUserUIN() , unIndex, CLICK_TYPE_PUBLIC_ACCEPT ) ;
	CString strRefuseHref ;
	strRefuseHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" uindex=\"%d\" type=\"%d\">拒绝</a>" ,
		L"" , pSrcUserItem->GetDecodeUserName().c_str() , pSrcUserItem->GetUserUIN() , unIndex, CLICK_TYPE_PUBLIC_REFUSE ) ;

	return  GetPrivateTips( L"【提示】： " ,strName + L" " + strAcceptHref+L" 还是 "+strRefuseHref+L"？" , COLOR_OPERATE  ) ;
}

CString HtmlBuild::GetSofaUserHead( CSofaItem * pSofaItem )
{
	CString strVip = ListIconManager::GetIconPathByVipLevel(pSofaItem->GetLastVipLevel()).c_str();
	if (!strVip.IsEmpty())
	{
		strVip = Img(strVip);
	}

	CString strRich = L"";
	if (pSofaItem->GetLastRichLevel() > 0)
	{
		strRich = RichLevelManager::GetRichLevelImgPathByLevel(pSofaItem->GetLastRichLevel()).c_str();
		if (!strRich.IsEmpty())
		{
			strRich = Img(strRich);
		}
	}
	return strVip + strRich;
}

CString HtmlBuild::ChatSofaUserItem( CSofaItem * pSofaItem )
{
	CString strTemp ;
	if ( pSofaItem != NULL )
	{
		strTemp = GetSofaUserHead( pSofaItem ) ;		
		CString strNameHref ;
		CString strSofaNameAndID;
		if (pSofaItem->GetLast179ID() > MAX_SHOW_179ID)
		{
			strSofaNameAndID = common::utility::stringhelper::XMLEncode(pSofaItem->GetLastUserName()).c_str();
		}
		else
		{
			strSofaNameAndID.Format(L"%s(%u)", common::utility::stringhelper::XMLEncode(pSofaItem->GetLastUserName()).c_str(), pSofaItem->GetLast179ID());
		}
		strNameHref.Format(_T("<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\" title=\"%d\" class=\"user\" >%s</a>") , 
			L"" , common::utility::stringhelper::XMLEncode(pSofaItem->GetLastUserName()).c_str() ,
			pSofaItem->GetLastUserUIN() , CLICK_TYPE_NAME ,pSofaItem->GetLast179ID() ,
			strSofaNameAndID ) ;
		strTemp += strNameHref ;
	}
	return strTemp ;
}

CString HtmlBuild::GetEnterRoomGetUserList_Event( IUserItem * pUserItem, uint8 unIndex )
{
	CString strHtml ;
	strHtml = EventEnterMgr->GetEnterHtmlByIndex(unIndex);
	if ( !strHtml.IsEmpty() )
	{
		CString strTemp ;
		strTemp.Format( L" %s "  , GetNameAndID(pUserItem) ) ;
		strHtml.Replace( L"name" , strTemp ) ;
		strHtml += L"<BR>" ;

	}

	return strHtml ;
}

CString HtmlBuild::GetNameAndID( IUserItem * pUserItem )
{
	CString strTemp;
	if (pUserItem->GetUser179ID() > MAX_SHOW_179ID)
	{
		strTemp = pUserItem->GetDecodeUserName().c_str();
	}
	else
	{
		strTemp.Format(L"%s(%u)", pUserItem->GetDecodeUserName().c_str(), pUserItem->GetUser179ID());
	}

	return strTemp;
}

CString HtmlBuild::GetEnterRoomGetUserList_VIPLevelBlackDiamond( IUserItem * pUserItem )
{
	CString strHtml ;
	if (!pUserItem->IsBlinkEnter() && pUserItem->GetUserVipLevel() == core::VIPLEVEL_BLACKDIAMOND )
	{
		strHtml = EnterMgr->GetEnterHtmlByVipLevel( pUserItem->GetUserVipLevel() ) ;
		if ( !strHtml.IsEmpty() )
		{
			CString strTemp ;
			strTemp.Format( L" %s "  , GetNameAndID(pUserItem) ) ;
			strHtml.Replace( L"VipImage" , GetUserItemHeadImg( pUserItem ) ) ;
			//strHtml.Replace(L"RichLevelImage", GetUserItemRichLevelImg(pUserItem));
			strHtml.Replace( L"name" , strTemp ) ;
			strHtml += L"<BR>" ;
		}
	}
	return strHtml ;
}

CString HtmlBuild::GetSpeakerGame( CString& strText )
{
	CString strTemp;
	strTemp = Font( L"宋体" , _T("#0000FF"), Span(13, strText) ) ;
	strTemp += L"<BR>" ;

	return SystemMessageTitle() + strTemp;
}

CString HtmlBuild::GetRoomMidInputDefaultText( CString& strDefault )
{
	CString strTemp = Font( L"宋体" , L"#C2C2C2" , Span(12, strDefault) ) ;
	return strTemp ;
}

CString HtmlBuild::GetMarkInvite( CString & strSongerName )
{
	CString strTitle = Font( L"宋体" , L"#EF427B" , Span(13, Bold(CString(L"【点歌评价】 "))) ) ;
	CString strHref;
	strHref.Format( L"<a href=\"%s\" name=\"%s\" uuid=\"%d\" type=\"%d\">去评价</a>" ,L"" ,strSongerName , GetCurInfo().unUIN, CLICK_TYPE_MARK );

	CString strTemp;
	strTemp.Format(L"主播%s邀请您对她演出进行评价 ", strSongerName);
	CString strRet = Font( L"宋体" , L"#EF427B" , Span(13, strTemp + strHref) ) ;

	return strTitle + strRet; 
}

CString HtmlBuild::GetMarkTips( const CString& strTitle , const CString& strValue , const CString& strColor/*=COLOR_NORMAL */ )
{
	CString strTemp = Font( L"宋体" , strColor , Span(13,Bold(strTitle)+strValue) ) ;
	strTemp += L"<BR>" ;
	return strTemp ;
}

CString HtmlBuild::GetSpeakerLuxury( SpeakerInfo& stSrcInfo, SpeakerInfo& stDstInfo, CString& strRoomName, uint16 unGiftID, uint16 unCount )
{
	IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(unGiftID);
	if (pGiftItem != NULL)
	{
		CString strGiftName = pGiftItem->GetName().c_str();
		CString strUnit = pGiftItem->GetUnitName().c_str();

		CString strMessage;
		strMessage.Format(L"%d%s%s", unCount, strUnit, strGiftName);

		CString strRoom;
		strRoom.Format(L"\"%s\"房间的", strRoomName);

		return SystemMessageTitle() + SpeakerLuckyMessage(strRoom) + SpeakerUserItem(stSrcInfo) 
			+ SpeakerLuckyMessage(CString(L"送给")) + SpeakerUserItem(stDstInfo) + SpeakerLuckyMessage(strMessage);
	}

	return L"";
}