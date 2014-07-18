#include "StdAfx.h"
#include "Room.h"
#include "RoomTopManager.h"
#include "micwindow\PublicMicDlg.h"
#include "RoomParentDlg.h"
#include "personalsetting/PersonalConfig.h"
#include "SendEventManager.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"


CRoomTopManager::CRoomTopManager(CRoomParentDlg* pParentDlg)
: m_pParentDlg(pParentDlg)
{
	CString strTemp;
	strTemp.Format(_T("\n-----------CRoomTopManager::m_pParentDlg :%0x"),pParentDlg->GetSafeHwnd());
	OutputDebugString(strTemp);

	m_RoomActor = new CRoomActor(pParentDlg);

	m_nPaiMaiBtn_Status = PAIMAIBTN_ANY ;
	m_pRoomParentSkinEngine = NULL;
	m_pRoomParentSkinEngine = m_pParentDlg->GetSkinEngine();
	ASSERT(m_pRoomParentSkinEngine != NULL);

	m_menuPaiMai.CreatePopupMenu() ;
	m_menuPaiMai.AppendMenuEx( MF_STRING , ROOMDLG_PAIMAI_PUBLIC ,_T("公麦") ) ;
	m_menuPaiMai.AppendMenuEx( MF_STRING , ROIMDLG_PAIMAI_PRIVATE , _T("私麦") ) ;

	

}

CRoomTopManager::~CRoomTopManager(void)
{
	if(m_RoomActor != NULL)
	{
		delete m_RoomActor;
	}

	
	m_menuPaiMai.DestroyMenu();
	
}

void CRoomTopManager::OnClickPaiMic()
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( m_pParentDlg, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能排麦，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return ;
	}	
	switch( m_nPaiMaiBtn_Status)
	{
	case PAIMAIBTN_ANY:
		{
			DisableBtnPaiMai() ;
			CSendEventManager::SendRequestShow(m_pParentDlg->GetRoomID(), REQUESTSHOWTYPE_ANY);
		}
		break;
	case PAIMAIBTN_FANGMAI:
		{
			m_pParentDlg->FangMai( true ) ;
		}
		break;
	default:break ;
	}
}


//进房时公麦列表
void CRoomTopManager::InitPublicMic( std::vector<core::room_user_info>& vecPubMic ) 
{
	std::vector<core::room_user_info>::iterator iter ;
	for ( iter = vecPubMic.begin() ; iter != vecPubMic.end() ; ++iter )
	{			
		uint32 micIndex = 0;
		BOOL bActor = (   (iter->unBusiness & core::BUSINESSTYPE_SONGER) 
			|| (iter->unBusiness & core::BUSINESSTYPE_DANCER) 
			|| (iter->unBusiness & core::BUSINESSTYPE_HOST) );//是否是艺人
		if ( iter->unStatus & core::USERSTATUSTYPE_PUBLIC_1 )
		{			
			m_mPublicMicDlg[1]->Init( iter->unUIN , iter->un179id , iter->unStatus , iter->strName.c_str() , 0, bActor) ;
			micIndex = 1;
		}
		else if ( iter->unStatus & core::USERSTATUSTYPE_PUBLIC_2 )
		{
			m_mPublicMicDlg[2]->Init( iter->unUIN , iter->un179id ,iter->unStatus ,  iter->strName.c_str() , 0, bActor) ;
			micIndex = 2;
		}
		else if ( iter->unStatus & core::USERSTATUSTYPE_PUBLIC_3 )
		{
			m_mPublicMicDlg[3]->Init( iter->unUIN , iter->un179id , iter->unStatus , iter->strName.c_str() , 0, bActor) ;
			micIndex = 3;
		}


		if ( micIndex != 0 && ( (iter->unBusiness & core::BUSINESSTYPE_SONGER) 
			|| (iter->unBusiness & core::BUSINESSTYPE_DANCER) 
			|| (iter->unBusiness & core::BUSINESSTYPE_HOST) ) )//如果公麦上的是艺人
		{
			m_pParentDlg->DrawBtnFans(micIndex);
		}
	}
}

//初始化主播列表
void CRoomTopManager::InitRoomActor()
{	
	m_RoomActor->InitOffRoomAcotrRequest(); //申请主播离线列表
	m_RoomActor->InitInRoomActor(); //初始化主播在线列表
	
	
}
void CRoomTopManager::ProcessInitOffRoomActorRsp(Event const& evt)
{
	m_RoomActor->OnInitOffRoomAcotrRsp(evt);
}

//用户进房
void CRoomTopManager::OnAddUser(IUserItem* pUserItem)
{
	ASSERT(pUserItem != NULL);
	
	m_RoomActor->ActorInRoom(pUserItem);  //主播进房	

}

//用户退房
void CRoomTopManager::OnDeleteUser(IUserItem* pUserItem)
{
	ASSERT(pUserItem != NULL);

	m_RoomActor->ActorOffRoom(pUserItem); //主播退房	

}

//最新头像图片响应函数
void CRoomTopManager::OnHallHeadImageUpdate(uint32 UIN)
{	
	
	m_RoomActor->OnActorHeadImageUpdate(UIN);	

}

 //其他用户的vip等级、富豪等级改变
void CRoomTopManager::ProcessNotifyUserInfoUpdate(Event const& evt)
{
	m_RoomActor->ProcessNotifyUpdateActorLevel(evt);	
}

//用户自已的vip等级更新
void CRoomTopManager::ProcessMyVipLevelUpdate()
{
	m_RoomActor->ProcessMyActorLevelUpdate();

}

//用户自已的富豪等级更新
void CRoomTopManager::ProcessMyRichLevelUpdate()
{
	m_RoomActor->ProcessMyActorLevelUpdate();	

}

void CRoomTopManager::OnMouseMove(UINT nHitTest,CPoint point)
{
	m_RoomActor->OnActorMouseMove(nHitTest,point);	
}

void CRoomTopManager::OnClickBtnRoomActorUp()
{
	m_RoomActor->OnClickBtnRoomActorUp();

}

void CRoomTopManager::OnClickBtnRoomActorDown()
{
	m_RoomActor->OnClickBtnRoomActorDown();
}

void CRoomTopManager::OnClickBtnRoomActorTitle()
{
	m_RoomActor->OnClickBtnRoomActorTitle();
}
//点击主播头像
void CRoomTopManager::OnClickBtnActorHead(int headIndex)
{
	m_RoomActor->OnClickBtnActorHead(headIndex);
}
void CRoomTopManager::ShowSkinRoomActor()
{
	m_RoomActor->ShowSkinRoomActor();
}

void CRoomTopManager::ShowGuide()
{	

	if(m_RoomActor != NULL)
	{
		m_RoomActor->ShowGuide();
	}

}

void CRoomTopManager::ClosePublicMic(uint32 UIN ) 
{
	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( UIN ) ;
	if ( pUserItem == NULL )
	{
		return ;
	}
	uint16 nIndex = pUserItem->GetPublicMicIndex() ; 
	if ( nIndex != 0 )
	{
		m_mPublicMicDlg[nIndex]->CleanAVLogic() ;
		m_mPublicMicDlg[nIndex]->SetDefault() ;
		m_mPublicMicDlg[nIndex]->SetBtnOrderSongVisible(false);
		m_pParentDlg->DeleteFansBtn(UIN);
		m_pParentDlg->DeleteUnDealSongBtn(UIN);
	}
}

void CRoomTopManager::DisableBtnPaiMai() 
{
	CComVariant var = true ;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(),L"btnPaiMic",L"disabled",var );
}

void CRoomTopManager::EnablePaiMai() 
{
	CComVariant var = false ;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(),L"btnPaiMic",L"disabled",var );
}

void CRoomTopManager::SetPaiMaiBtnStatus( ENM_PaiMaiBtn_Status nStatus ) 
{
	CComVariant varPicture ;
	CComVariant varText ;
	switch( nStatus)
	{
	case PAIMAIBTN_ANY:
		{
			varPicture = TRUE;
			varText = L"排麦" ;			
		}
		break;
	case PAIMAIBTN_PAIMAI:
		{
			varPicture = TRUE;
			varText = L"排麦" ;
		}
		break;
	case PAIMAIBTN_FANGMAI:
		{
			varPicture = FALSE;
			varText = L"放麦" ;
		}
		break;
	default:break;
	}
	m_nPaiMaiBtn_Status = nStatus ;

	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(),L"btnPaiMic",L"visible",varPicture);
	CComVariant varTemp = !varPicture.boolVal;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(),L"btnFangMic",L"visible",varTemp);

	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(),L"btnPaiMic",L"tip",varText);
}

void CRoomTopManager::CloseMic() 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	IUserItem* pUserIterm = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( pUserIterm != NULL )
	{
		int unIndex = pUserIterm->GetPublicMicIndex() ;
		if ( unIndex>=1 && unIndex<= 3 )
		{
			m_mPublicMicDlg[unIndex]->ChangeAudio( myCurInfo.unUIN , BTN_STATUS_OPEN ) ;
		}
	}
}

void CRoomTopManager::OpenMic() 
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	IUserItem* pUserIterm = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( pUserIterm != NULL )
	{
		int unIndex = pUserIterm->GetPublicMicIndex() ;
		if ( unIndex>=1 && unIndex<= 3 )
		{
			m_mPublicMicDlg[unIndex]->ChangeAudio( myCurInfo.unUIN , BTN_STATUS_CLOSE ) ;
		}
	}
}

uint32 CRoomTopManager::GetPulicMicUINByIndex( int nIndex )
{
	if ( nIndex>=1 && nIndex<= 3 )
	{
		return m_mPublicMicDlg[nIndex]->GetUserUIN() ;
	}
	else
	{
		return 0 ;
	}
}

void CRoomTopManager::OnRequestShow_NeedSelect()
{
	CRect rcTemp;
	m_pRoomParentSkinEngine->GetSubSkinPos(m_pParentDlg->GetSafeHwnd(),L"btnPaiMic",&(rcTemp.left),&(rcTemp.top),
		&(rcTemp.right),&(rcTemp.bottom));
	//ClientToScreen( &rcTemp );
	m_pParentDlg->ClientToScreen(&rcTemp);
	int nreturn = m_menuPaiMai.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN|TPM_RETURNCMD  ,rcTemp.right-10 ,rcTemp.top+30 , m_pParentDlg ) ;  //排麦下拉菜单
	if ( nreturn != 0 )
	{			
		if ( nreturn == ROOMDLG_PAIMAI_PUBLIC )
		{
			CSendEventManager::SendRequestShow( m_pParentDlg->GetRoomID(), REQUESTSHOWTYPE_PUBLIC ) ;
		}
		else if ( nreturn == ROIMDLG_PAIMAI_PRIVATE )
		{
			CSendEventManager::SendRequestShow( m_pParentDlg->GetRoomID(), REQUESTSHOWTYPE_PRIVATE ) ;
		}		
	}
	EnablePaiMai() ;
}

void CRoomTopManager::OnRequestShow_Failed()
{
	SetPaiMaiBtnStatus( PAIMAIBTN_ANY ) ;
	EnablePaiMai() ;
}

void CRoomTopManager::OnRequestShow_Public(uint8 unIndex , uint32 unTime )
{
	core::CurInfo myCurInfo = GetCurInfo() ; 
	IUserItem* pUserIterm = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;
	if ( unIndex>=1 && unIndex<=3 && pUserIterm != NULL )
	{
		m_mPublicMicDlg[unIndex]->Init( pUserIterm->GetUserUIN() ,pUserIterm->GetUser179ID() , pUserIterm->GetUserStatus() ,pUserIterm->GetUserName().c_str() , unTime , pUserIterm->IsActor()) ;
		SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
		EnablePaiMai() ;
	}
}

void CRoomTopManager::OnRequestShow_WaitingList()
{
	SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
	EnablePaiMai() ;
}


void CRoomTopManager::OnRequestShow_PublicNotify( uint32 unSrcUIN , uint8 unIndex , uint16 unTime )
{
	IUserItem* pSrcUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unSrcUIN ) ;
	if ( pSrcUserItem == NULL )
	{
		return ;
	}
	if (  unIndex>=1 && unIndex<= 3 )
	{
		m_mPublicMicDlg[unIndex]->Init( unSrcUIN  , pSrcUserItem->GetUser179ID() ,pSrcUserItem->GetUserStatus() , pSrcUserItem->GetUserName().c_str() , unTime , pSrcUserItem->IsActor()) ;
	}
	core::CurInfo myCurInfo = GetCurInfo() ; 
	if ( myCurInfo.unUIN == unSrcUIN )
	{
		OnRequestShow_PublicNotifySelf() ;  //从麦序中主动上麦时会收到Notify消息
	}
}

void CRoomTopManager::OnRequestShow_PublicNotifySelf() 
{
	SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
	EnablePaiMai() ;
}

void CRoomTopManager::OnRequestShow_Private()
{
	SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
	EnablePaiMai() ;
}

void CRoomTopManager::OnExitShowNotify( uint32 srcuin )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.unUIN == srcuin )
	{		
		SetPaiMaiBtnStatus( PAIMAIBTN_ANY ) ;
		EnablePaiMai() ;
	}
}

void CRoomTopManager::OnPushOnShow_Success( uint32 unDstUIN , uint8 unIndex )
{
	IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unDstUIN ) ;
	if ( pDstUserItem == NULL )
	{
		return ;
	}
	core::CurInfo myCurInfo = GetCurInfo() ;
	if (  unIndex>=1 && unIndex<= 3 )
	{
		m_mPublicMicDlg[unIndex]->Init( unDstUIN  , pDstUserItem->GetUser179ID() ,pDstUserItem->GetUserStatus() , pDstUserItem->GetUserName().c_str() , 0, pDstUserItem->IsActor()) ;
		if ( myCurInfo.unUIN == unDstUIN )
		{
			SetPaiMaiBtnStatus( PAIMAIBTN_FANGMAI ) ;
		}
	}
}

void CRoomTopManager::OnPullOffShowRsp_Success( IUserItem* pDstUserItem )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.unUIN == pDstUserItem->GetUserUIN() && pDstUserItem->GetPublicMicIndex() != 0 )
	{		
		SetPaiMaiBtnStatus( PAIMAIBTN_ANY ) ;
		DisableBtnPaiMai() ;
	}
}

void CRoomTopManager::OnOpVideoAudioRsp( uint16 nIndex ,uint32 nOpAVType ,uint32 unUIN )
{
	m_mPublicMicDlg[nIndex]->OnOpVideoAudioRsp( nOpAVType , unUIN ) ;
}

void CRoomTopManager::OnOpVideoAudioNotify( uint16 nIndex , uint32 nOpAVType ,uint32 unUIN ) 
{
	m_mPublicMicDlg[nIndex]->OnOpVideoAudioNotify( nOpAVType , unUIN ) ;
}

void CRoomTopManager::OnSetMicStatus( int16 nIndex , int16 nStatus )
{
	if ( nIndex >=1 && nIndex<=3 && nStatus == PUBLIC_MIC_STATUS_CLOSE )
	{
		uint32 unDstUIN = GetPulicMicUINByIndex( nIndex ) ;
		if (unDstUIN != 0 )
		{
			ClosePublicMic( unDstUIN ) ;
			m_pParentDlg->GetRoomBusiness()->OnUserStatusChange( unDstUIN , USERSTATUSTYPE_PUBLIC_1|USERSTATUSTYPE_PUBLIC_2|
				USERSTATUSTYPE_PUBLIC_3 ,false ) ;
			OnExitShowNotify( unDstUIN ) ;
		}
	}
}

void CRoomTopManager::SetPublicMicEffect(int nMic, BOOL bOnMic)
{
	if (nMic >=1 && nMic <= 3)
	{
		CString strOnMic = L"";
		CString strOutMic = L"";
		strOnMic.Format(L"imgPublicOnMic%dBk", nMic);
		strOutMic.Format(L"imgPublicOutMic%dBk", nMic);
		CComVariant var = bOnMic;
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), strOnMic.GetBuffer(), L"visible", var);
		var = !bOnMic;
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), strOutMic.GetBuffer(), L"visible", var);
	}
}

void CRoomTopManager::CleanAVLogic() 
{
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		pPublicMicDlg->CleanAVLogic() ;
	}
}

//关闭公屏上的视频
void CRoomTopManager::CloseVideo() 
{
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		pPublicMicDlg->CloseVideo() ;
	}
}

void CRoomTopManager::OpenVideo() 
{
	map<int ,CPublicMicDlg*>::iterator iter_public ;
	for ( iter_public = m_mPublicMicDlg.begin() ; iter_public != m_mPublicMicDlg.end() ; ++iter_public )
	{
		CPublicMicDlg* pPublicMicDlg = iter_public->second ;
		pPublicMicDlg->OpenVideo() ;
	}
}

void CRoomTopManager::GetPublicMicRect(int micIndex, CRect &rcMic)
{
	if (micIndex >= 1 && micIndex <= 3)
	{
		CString strTemp;
		strTemp.Format(L"itemPublicMic%d", micIndex);
		m_pRoomParentSkinEngine->GetSubSkinPos(m_pParentDlg->GetSafeHwnd(), strTemp.GetBuffer(), &(rcMic.left), &(rcMic.top), &(rcMic.right), &(rcMic.bottom));
		m_pParentDlg->ClientToScreen(&rcMic);
	}
}

void CRoomTopManager::SetAllPublicMic( map<int, CPublicMicDlg*>& mapPublicMicDlg)
{
	m_mPublicMicDlg = mapPublicMicDlg;
}

void CRoomTopManager::SetSpeakerHtml( COutputHtmlEditImpl * pSpeakerHtml )
{
	m_pSpeakerHtml = pSpeakerHtml;
}