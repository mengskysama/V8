#include "StdAfx.h"
#include "UserListMenu.h"
#include "coredefine.h"
#include "..\Resource.h"
#include "..\..\CommonLibrary\include\ui\MenuEx.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "..\RoomParentDlg.h"
#include "..\roomsetting\RoomOption.h"
#include "..\shortcut\SelfDefSentenceDlg.h"
#include "..\GlobalRoomDefine.h"
#include "..\GlobalManager.h"
#include "KickDlg.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"

#include "..\RoomMidDlg.h"
#include "..\SendEventManager.h"

#include "..\autoreply\AutoReplyConfig.h"

UserListMenu::UserListMenu(CWnd * pWnd)
: m_pWnd(pWnd)
{
}

UserListMenu::~UserListMenu(void)
{
}

void UserListMenu::PopupMenu( IUserItem * pMyItem, IUserItem * pDstItem )
{
	common::ui::CMenuEx * pm = new common::ui::CMenuEx();

	pm->CreatePopupMenu();

//////DragonZ:菜单加图标代码，除了这个地方，在AppendMenu时，最后一个参数是对应到ImageList的第几个图标，注意要添上，否则找不到对应图标
	/*CImageList tempImageList;
	tempImageList.Create(20,20,ILC_COLOR24 | ILC_MASK,50,50);
	HBITMAP hbmp = (HBITMAP)LoadImage(NULL,
		L"D:\\VDC\\51ktv\\bin\\debug\\resource\\halltree\\179chat.bmp",
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE);
	CBitmap bm;
	VERIFY(bm.Attach(hbmp));
	for (int i = 0; i < 20; ++i)
	{
		tempImageList.Add(&bm,RGB(0,0,0));
	}
	bm.Detach();
	pm->SetImageList(&tempImageList);*/

	//自己
	if (pMyItem->GetUserUIN() == pDstItem->GetUserUIN())
	{
		//有管理权限的
		if (pMyItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
		{
			CRoomParentDlg* pRoomParentDlg = (CRoomParentDlg*)m_pWnd;
			if (pMyItem->GetUserTitleLevel() > TITLELEVEL_TEMP_MANAGER)
			{
				if (pRoomParentDlg != NULL)
				{
					if (pRoomParentDlg->GetPulicMicUINByIndex(1) == 0)
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_INVITETOFIRSTMIC,L"邀请到一麦");
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOFIRSTMIC,L"邀请到一麦");
					}

					if (pRoomParentDlg->GetPulicMicUINByIndex(2) == 0)
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_INVITETOSECONDMIC,L"邀请到二麦");
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOSECONDMIC,L"邀请到二麦");
					}

					if (pRoomParentDlg->GetPulicMicUINByIndex(3) == 0)
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_INVITETOTHIRDMIC,L"邀请到三麦");
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOTHIRDMIC,L"邀请到三麦");
					}
				}
			}

			if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_OWNER)
			{
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_SECONDOWNER,L"任命房间副室主");
			}
			if ( pMyItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER )
			{
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_MANAGER,L"任命房间管理员");
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_SETTEMPMANAGER,L"任命临时管理员");
			}			
			if (pMyItem->GetUserTitleLevel() > core::TITLELEVEL_OWNER)
			{
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDLOCKIP,L"封停IP");
			}
			if ( pMyItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER )
			{
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDBLACK,L"加入黑名单");
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
			}			
			pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
			pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_SHUTUP,L"禁止在本房间发言");
			pm->AppendMenuEx(MF_SEPARATOR);
			pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
			pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			pm->AppendMenuEx(MF_SEPARATOR);			
			//pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_BLOCKTOCHAWITHME,L"禁止和我聊天");//todo
		}
		//已注册的非管理
		else if (pMyItem->GetUserVipLevel() > core::VIPLEVEL_NONE)
		{
			pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
			pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
			pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			//pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
		}
		//游客
		else 
		{
			delete pm;
			pm = NULL;
			return;
			//pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_USERINFO,L"用户信息");
			//pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
			//pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_COPYID,L"复制ID");
			//pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
		}
	}
	else
	{
		//目标用户非游客
		if (pDstItem->GetUserVipLevel() > core::VIPLEVEL_NONE)
		{
			//自己是管理员
			if (pMyItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
			{
				//邀请上麦
				if (pMyItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER)
				{
					if (pDstItem->GetUserTitleLevel() < pMyItem->GetUserTitleLevel())
					{
						CRoomParentDlg* pParentDlg = (CRoomParentDlg*)m_pWnd;					
						if (pParentDlg != NULL)
						{
							if (pParentDlg->GetPulicMicUINByIndex(1) == 0)
							{
								pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_INVITETOFIRSTMIC,L"邀请到一麦"/*,0*/);//最后一个参数是对应ImageList里的图标位置
							}
							else
							{
								pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOFIRSTMIC,L"邀请到一麦"/*,0*/);
							}
		
							if (pParentDlg->GetPulicMicUINByIndex(2) == 0)
							{
								pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_INVITETOSECONDMIC,L"邀请到二麦"/*,1*/);
							}
							else
							{
								pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOSECONDMIC,L"邀请到二麦"/*,1*/);
							}
		
							if (pParentDlg->GetPulicMicUINByIndex(3) == 0)
							{
								pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_INVITETOTHIRDMIC,L"邀请到三麦"/*,2*/);
							}
							else
							{
								pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOTHIRDMIC,L"邀请到三麦"/*,2*/);
							}
	
						}
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOFIRSTMIC,L"邀请到一麦");
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOSECONDMIC,L"邀请到二麦");
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_INVITETOTHIRDMIC,L"邀请到三麦");
					}
				}
		
				//任命副室主
				if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_OWNER)
				{
					if (pDstItem->GetUserTitleLevel() < core::TITLELEVEL_SECOND_OWNER)
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SECONDOWNER,L"任命房间副室主");
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_SECONDOWNER,L"任命房间副室主");
					}
				}
	
				//任命正管
				if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_SECOND_OWNER && pDstItem->GetUserTitleLevel() < core::TITLELEVEL_MANAGER)
				{
					pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_MANAGER,L"任命房间管理员");
				}
				else if (pMyItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER)
				{
					pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_MANAGER,L"任命房间管理员");
				}
	
				//任命临管
				if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
				{
					if (pDstItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_SETTEMPMANAGER,L"任命临时管理员");
					}
					else if (pDstItem->GetUserTitleLevel() == core::TITLELEVEL_TEMP_MANAGER)
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_REMOVETEMPMANAGER,L"取消临时管理员身份");
					}
					else
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETTEMPMANAGER,L"任命临时管理员");
					}
				}

				if (pMyItem->GetUserTitleLevel() == core::TITLELEVEL_SUPER_MANAGER)
				{
					if (pDstItem->GetUserTitleLevel() < pMyItem->GetUserTitleLevel())
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUTANDLOCKIP,L"封停IP");
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUTANDBLACK,L"加入黑名单");
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDLOCKIP,L"封停IP");
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDBLACK,L"加入黑名单");
					}
				}
				else
				{
					//踢出房间并封IP
					//if (pMyItem->GetUserTitleLevel() == core::TITLELEVEL_OWNER)
					//{
					//	if (pDstItem->GetUserTitleLevel() < core::TITLELEVEL_MANAGER)
					//	{
					//		pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUTANDLOCKIP,L"踢出房间并封IP");
					//	}
					//	else
					//	{
					//		pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDLOCKIP,L"踢出房间并封IP");
					//	}
					//}

					//踢出房间
					if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
					{
						//pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
						if ( ( pDstItem->GetUserTitleLevel() < pMyItem->GetUserTitleLevel() ) && ( !pDstItem->IsAgent() ))
						{
							pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
						}
						else
						{
							pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
						}
					}
					//else if (pMyItem->GetUserVipLevel() >= core::VIPLEVEL_SILVER)
					//{
					//	if (pDstItem->GetUserVipLevel() < pMyItem->GetUserVipLevel() && pDstItem->GetUserTitleLevel() < core::TITLELEVEL_TEMP_MANAGER)
					//	{
					//		pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
					//	}
					//	else
					//	{
					//		pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
					//	}
					//}

					//踢出房间并加黑
					if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_OWNER/* && pDstItem->GetUserTitleLevel() < core::TITLELEVEL_MANAGER*/)
					{
						if ( pDstItem->GetUserTitleLevel() < core::TITLELEVEL_MANAGER && (!pDstItem->IsAgent()) )
						{
							pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUTANDBLACK,L"加入黑名单");
						}
						else
						{
							pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDBLACK,L"加入黑名单");
						}
					}
					else if (pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER)
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTANDBLACK,L"加入黑名单");
					}
				}
	
				//禁言
				if (pDstItem->GetUserTitleLevel() < pMyItem->GetUserTitleLevel())
				{
					if (!pDstItem->IsForbidden())
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SHUTUP,L"禁止在本房间发言");
					}
					else
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_UNLOCKSHUTUP,L"取消房间禁言");
					}
				}
				else
				{
					pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_SHUTUP,L"禁止在本房间发言");
				}

				if (pMyItem->GetUserVipLevel() >= core::VIPLEVEL_SILVER && pMyItem->GetUserTitleLevel() == core::TITLELEVEL_TEMP_MANAGER)
				{
					if (pDstItem->IsAgent())
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
					}else
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
					}
				}

				pm->AppendMenuEx(MF_SEPARATOR);
			}
			else
			{
				if (pMyItem->GetUserVipLevel() >= core::VIPLEVEL_SILVER)
				{
					if (pDstItem->IsAgent())
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
					}else
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
					}
					
					pm->AppendMenuEx(MF_SEPARATOR);
				}
			}
			
			//自己是非游客
			if (pMyItem->GetUserVipLevel() > core::VIPLEVEL_NONE)
			{							
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
				pm->AppendMenuEx(MF_SEPARATOR);
				if (pDstItem->GetUserTitleLevel() > core::TITLELEVEL_NONE)
				{
					if (pMyItem->GetUserVipLevel() > core::VIPLEVEL_NONE && pDstItem->IsPrivate())
					{
						pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_PRIVATEMIC,L"观看私麦视频");
					}
					else
					{
						pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_PRIVATEMIC,L"观看私麦视频");
					}
				}
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_P2PMIC,L"一对一视频");     //lfp
				pm->AppendMenuEx(MF_SEPARATOR);
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
				pm->AppendMenuEx(MF_SEPARATOR);
				CRoomParentDlg* pRoomParentDlg = (CRoomParentDlg*)m_pWnd;
				if ( pRoomParentDlg->GetRoomBusiness()->IsBlockChat( pDstItem->GetUserUIN() ) )
				{
					pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_UNBLOCKCHATTOME,L"允许和我聊天");
				}
				else
				{
					pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_BLOCKTOCHAWITHME,L"禁止和我聊天");
				}
			}
			else
			{
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			}
		}
		else
		{
			if (pMyItem->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER)
			{
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
				//if (!pDstItem->IsForbidden())
				//{
				//	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SHUTUP,L"禁止在本房间发言");
				//}
				//else
				//{
				//	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_UNLOCKSHUTUP,L"取消房间禁言");
				//}
				//pm->AppendMenuEx(MF_SEPARATOR);
			}
			if (pMyItem->GetUserVipLevel() > core::VIPLEVEL_NONE)
			{
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			}
			//if (pDstItem->IsBlockChatToMe())
			//{
			//	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_UNBLOCKCHATTOME,L"允许和我聊天");
			//}
			//else
			//{
			//	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_BLOCKTOCHAWITHME,L"禁止和我聊天");
			//}
		}
	}

	common::ui::CCursorPos pt;
	uint32 nReturn = pm->TrackPopupMenu( TPM_RETURNCMD , pt.x, pt.y, m_pWnd);
	if ( nReturn != 0 )
	{
		OnCommand( nReturn ) ;
	}
	pm->DestroyMenu();

	delete pm;
	pm = NULL;

}


void UserListMenu::PopupMenu( IUserItem * pDstItem ) 
{
	common::ui::CMenuEx * pm = new common::ui::CMenuEx();
	pm->CreatePopupMenu();
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
	//pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
	pm->AppendMenuEx(MF_SEPARATOR);
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_PRIVATEMIC,L"观看私麦视频");
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");	
	//pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_P2PMIC,L"一对一私聊");
	pm->AppendMenuEx(MF_SEPARATOR);
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
	pm->AppendMenuEx(MF_SEPARATOR);
	pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_BLOCKTOCHAWITHME,L"禁止和我聊天");
	//pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
	if (  !(pDstItem->GetUserStatus()& core::USERSTATUSTYPE_PRIVATE) )
	{
		pm->EnableMenuItem( ID_USERLISTMENU_PRIVATEMIC , MF_GRAYED ) ;
	}
	//if ( !(pDstItem->GetUserStatus()& core::USERSTATUSTYPE_VIDEO) )
	//{
	//	pm->EnableMenuItem( ID_USERLISTMENU_P2PMIC , MF_GRAYED ) ;
	//}
	common::ui::CCursorPos pt;
	uint32 nReturn = pm->TrackPopupMenu(TPM_RETURNCMD, pt.x, pt.y, m_pWnd);
	if ( nReturn != 0 )
	{
		OnCommand( nReturn ) ;
	}
	pm->DestroyMenu();

	delete pm;
	pm = NULL;
}

void UserListMenu::OnCommand( UINT32 nCommand ) 
{
	CRoomParentDlg* pParentDlg = (CRoomParentDlg*)m_pWnd ;
	switch ( nCommand )
	{
	case ID_USERLISTMENU_INVITETOFIRSTMIC:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendPushOnEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), 1);
			}
		}
		break;
	case ID_USERLISTMENU_INVITETOSECONDMIC:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendPushOnEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), 2);
			}
		}
		break;
	case ID_USERLISTMENU_INVITETOTHIRDMIC:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendPushOnEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), 3);
			}
		}
		break;
	case ID_USERLISTMENU_SECONDOWNER:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendAddTitleEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), core::TITLELEVEL_SECOND_OWNER);
			}
		}
		break;
	case ID_USERLISTMENU_MANAGER:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendAddTitleEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), core::TITLELEVEL_MANAGER);
			}
		}
		break;
	case ID_USERLISTMENU_SETTEMPMANAGER:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendAddTitleEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), core::TITLELEVEL_TEMP_MANAGER);
			}
		}
		break;
	case ID_USERLISTMENU_REMOVETEMPMANAGER:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendRemoveTitleEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN(), core::TITLELEVEL_TEMP_MANAGER);
			}
		}
		break;
	case ID_USERLISTMENU_KICKOUTANDLOCKIP:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				pParentDlg->ShowKickDlg(1);
			}
		}
		break;
	case ID_USERLISTMENU_KICKOUTANDBLACK:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CString strTip1;
				strTip1.Format(L"您确定要将%s", pParentDlg->m_pSelectUserItem->GetUserName().c_str());
				CString strTip2 ;
				strTip2.Format( L"(%u)加入黑名单？" , pParentDlg->m_pSelectUserItem->GetUser179ID() ) ;
				if ( C179MsgBox::Show( pParentDlg, NewMsgBox::MSGBOX_ICO_OK_CANCEL_TWO_STRING , NewMsgBox::ICONTYPE_WARNING , strTip1, strTip2 ) == RET_OK)
				{
					if ( pParentDlg->m_pSelectUserItem != NULL )
					{
						CSendEventManager::SendAddBlackEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN());
					}					
				}
			}
		}
		break;
	case ID_USERLISTMENU_KICKOUT:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				pParentDlg->ShowKickDlg(0);
			}
		}
		break;
	case ID_USERLISTMENU_KICKOUTALLGUEST:
		{
			if (pParentDlg != NULL)
			{
				if ( C179MsgBox::Show( pParentDlg, NewMsgBox::MSGBOX_ICO_OK_CANCEL_ONE_STRING ,L"您确定踢出房间所有游客？" ) == RET_OK )
				{
					CSendEventManager::SendKickOutAllGuestEvent(pParentDlg->GetRoomID());
				}
			}
		}
		break;
	case ID_USERLISTMENU_SHUTUP:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendShutUpEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN());
			}
		}
		break;
	case ID_USERLISTMENU_UNLOCKSHUTUP:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendUnlockShutUpEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN());
			}
		}
		break;
	case ID_USERLISTMENU_CHECKCPUANDMEMORY:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendCheckCPUandMemEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN());
			}
		}
		break;
	case ID_USERLISTMENU_SHOWJIAZU:
		{

		}
		break;
	case ID_USERLISTMENU_NOTSHOWJIAZU:
		{

		}
		break;
	case ID_USERLISTMENU_STAMP:
		{
			if ( pParentDlg->m_pSelectUserItem != NULL )
			{
				CString strName = pParentDlg->m_pSelectUserItem->GetUserName().c_str() ;
				CString strUUID ;
				strUUID.Format( L"%d" , pParentDlg->m_pSelectUserItem->GetUserUIN() ) ;
				pParentDlg->OnClickNameLink( strName , strUUID ) ;
				pParentDlg->OnClickStamp() ;
			}
		}
		break;
	case ID_USERLISTMENU_PRIVATEMIC:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				pParentDlg->SendOpenPrivate( pParentDlg->m_pSelectUserItem->GetUserUIN() ) ;
			}
		}
		break;
	case ID_USERLISTMENU_P2PMIC:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				pParentDlg->SendRequestP2PMic(pParentDlg->m_pSelectUserItem->GetUserUIN());
			}
		}
		break;
	case ID_USERLISTMENU_BLOCKTOCHAWITHME:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				pParentDlg->GetRoomBusiness()->BlockChat( pParentDlg->m_pSelectUserItem->GetUserUIN() ) ;
				pParentDlg->m_pSelectUserItem->SetBlockChatToMe(true);

				pParentDlg->GetHtmlOutPutManager()->OnBlockChatToMe(pParentDlg->m_pSelectUserItem->GetUserName() , pParentDlg->m_pSelectUserItem->GetUser179ID() );
			}
		}
		break;
	case ID_USERLISTMENU_UNBLOCKCHATTOME:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				pParentDlg->GetRoomBusiness()->UnBlockChat( pParentDlg->m_pSelectUserItem->GetUserUIN() ) ;
				pParentDlg->m_pSelectUserItem->SetBlockChatToMe(false);

				pParentDlg->GetHtmlOutPutManager()->OnUnBlockChatToMe(pParentDlg->m_pSelectUserItem->GetUserName() , pParentDlg->m_pSelectUserItem->GetUser179ID() );
			}
		}
		break;
	case ID_USERLISTMENU_SETSENDOBJ:
		{
			//选中聊天对象
			if ( pParentDlg->m_pSelectUserItem != NULL )
			{
				pParentDlg->SetSendObj( pParentDlg->m_pSelectUserItem->GetUserName().c_str() , pParentDlg->m_pSelectUserItem->GetUserUIN() ) ;
			}
		}
		break;
	case ID_USERLISTMENU_SETCHATOBJ:
		{
			//选中赠送对象
			if ( pParentDlg->m_pSelectUserItem != NULL )
			{
				pParentDlg->SetChatObj( pParentDlg->m_pSelectUserItem->GetUserName().c_str() , pParentDlg->m_pSelectUserItem->GetUserUIN() ) ;
			}
		}
		break;
	case ID_USERLISTMENU_USERINFO:
		{
			if ( pParentDlg->m_pSelectUserItem != NULL )
			{
				pParentDlg->OnUserInfo( pParentDlg->m_pSelectUserItem ) ;
			}
		}
		break;
	case ID_USERLISTMENU_COPYNICKNAME:
		{
			if ( pParentDlg->m_pSelectUserItem != NULL )
			{
				common::utility::systemhelper::CopyDataToClipbord( pParentDlg->m_pSelectUserItem->GetUserName() ) ;
			}
		}
		break;
	case ID_USERLISTMENU_COPYID:
		{
			if ( pParentDlg->m_pSelectUserItem != NULL )
			{
				CString strTemp ;
				strTemp.Format( L"%d" , pParentDlg->m_pSelectUserItem->GetUser179ID() ) ;
				common::utility::systemhelper::CopyDataToClipbord( strTemp.GetBuffer() ) ;
			}
		}
		break;
	case ID_USERLISTMENU_CHECKNETSPEED:
		{
			if (pParentDlg->m_pSelectUserItem != NULL)
			{
				CSendEventManager::SendCheckNetSpeedEvent(pParentDlg->GetRoomID(), pParentDlg->m_pSelectUserItem->GetUserUIN());
			}
		}
		break;
	default:
		break;
	}
}

void UserListMenu::PopupGuestMenu()
{
	common::ui::CMenuEx * pm = new common::ui::CMenuEx();
	pm->CreatePopupMenu();

	CRoomParentDlg* pRoomParentDlg = (CRoomParentDlg*)m_pWnd;
	if (pRoomParentDlg != NULL && pRoomParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() != NULL)
	{
		if (pRoomParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem()->GetUserTitleLevel() > core::TITLELEVEL_TEMP_MANAGER)
		{
			if (pRoomParentDlg->GetRoomBusiness()->GetUserManager()->GetGuestCount() > 0)
			{
				pm->AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUTALLGUEST,L"踢出全部游客");
			}
			else
			{
				pm->AppendMenuEx(MF_GRAYED,ID_USERLISTMENU_KICKOUTALLGUEST,L"踢出全部游客");
			}
		}
	}

	common::ui::CCursorPos pt;
	uint32 nReturn = pm->TrackPopupMenu( TPM_RETURNCMD , pt.x, pt.y, m_pWnd);
	if ( nReturn != 0 )
	{
		OnCommand( nReturn ) ;
	}
	pm->DestroyMenu();

	delete pm;
	pm = NULL;
}

CaiZiMenu::CaiZiMenu( CWnd * pWnd )
:m_pWnd( pWnd )
{
}

CaiZiMenu::~CaiZiMenu()
{

}
void CaiZiMenu::PopupMenu()
{
	common::ui::CMenuEx * pm = new common::ui::CMenuEx();
	pm->CreatePopupMenu();
	vector<CaiZiItem*>::iterator iter ;
	for ( iter = CaiZiMgr->GetCaiZiList().begin() ; iter != CaiZiMgr->GetCaiZiList().end() ; ++iter )
	{
		CaiZiItem *pCaiZiItem = *iter ;
		pm->AppendMenuEx( MF_STRING , pCaiZiItem->GetCaiZiIndex() , pCaiZiItem->GetCaiZiName().c_str() ) ;
	}
	common::ui::CCursorPos pt;
	int nReturn = pm->TrackPopupMenu( TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_RETURNCMD, pt.x, pt.y-5,  CWnd::FromHandle(m_pWnd->GetSafeHwnd()) );
	m_pWnd->SendMessage( WM_CLICK_CAIZI , 0 , nReturn ) ;
	pm->DestroyMenu();

	delete pm;
	pm = NULL;
}

ShortCutMsgMenu::ShortCutMsgMenu( CWnd * pWnd )
:m_pWnd( pWnd )
{
}

ShortCutMsgMenu::~ShortCutMsgMenu()
{
}

void ShortCutMsgMenu::PopupMenu() 
{
	common::ui::CMenuEx shortcutmenu ;
	shortcutmenu.CreatePopupMenu() ;
	vector<wstring>::iterator iter ;
	int nIndex = 1 ;
	for ( iter = ShortCutMsgMgr->GetShortCutMsgList().begin() ; iter != ShortCutMsgMgr->GetShortCutMsgList().end() ; ++iter ,++nIndex )
	{
		shortcutmenu.AppendMenuEx( MF_STRING , nIndex , iter->c_str() ) ;
	}
	shortcutmenu.AppendMenuEx( MF_SEPARATOR) ;
	shortcutmenu.AppendMenuEx( MF_STRING , SELF_DEFINE_SHORCUT ,L"自定义编辑语句" ) ;
	common::ui::CCursorPos pt;
	int nreturn = shortcutmenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD , pt.x, pt.y, /*CWnd::FromHandle(m_pRoomDlg->GetSafeHwnd()) */m_pWnd);
	m_pWnd->SendMessage( WM_CLICK_SHORTCUT , 0 , nreturn ) ;
	shortcutmenu.DestroyMenu();
}

AutoReplyMenu::AutoReplyMenu(CWnd *pWnd)
:m_pWnd(pWnd)
{
	m_pMenu = NULL;
}

AutoReplyMenu::~AutoReplyMenu()
{
	DeleteReplyMenu();
}

void AutoReplyMenu::PopupMenu()
{
	common::ui::CMenuEx autoReplyMenu;
	autoReplyMenu.CreatePopupMenu();
	vector<stAutoReplyMenuItem>::iterator iter ;
	int nIndex = 1 ;
	for (iter = AutoReplyMgr->GetAutoReplyList().begin(); iter != AutoReplyMgr->GetAutoReplyList().end(); ++iter, ++nIndex )
	{
		stAutoReplyMenuItem stItem = *iter;
		if (stItem.bChecked)
		{
			autoReplyMenu.AppendMenuEx(MF_STRING | MF_CHECKED, nIndex, stItem.strMenuItem.c_str());
		}else
		{
			autoReplyMenu.AppendMenuEx(MF_STRING, nIndex, stItem.strMenuItem.c_str());
		}
		
	}
	autoReplyMenu.AppendMenuEx(MF_SEPARATOR);
	
	if(AutoReplyMgr->IsAutoReply())
	{
		autoReplyMenu.AppendMenuEx(MF_STRING, CANCEL_AUTOREPLY_MENU, L"无回复消息");
	}else
	{
		autoReplyMenu.AppendMenuEx(MF_STRING | MF_CHECKED, CANCEL_AUTOREPLY_MENU, L"无回复消息");
	}
	autoReplyMenu.AppendMenuEx(MF_STRING , SET_AUTOREPLY_MENU, L"自定义...");

	common::ui::CCursorPos pt;
	int nreturn = autoReplyMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, pt.x, pt.y,m_pWnd);
	m_pWnd->SendMessage( WM_CLICK_AUTOREPLY_MENU, 0, nreturn );
	autoReplyMenu.DestroyMenu();
}

common::ui::CMenuEx *  AutoReplyMenu::GetAutoReplyMenu()
{
	m_pMenu = new common::ui::CMenuEx();
	m_pMenu->CreatePopupMenu();

	vector<stAutoReplyMenuItem>::iterator iter ;
	int nIndex = 1 ;
	for (iter = AutoReplyMgr->GetAutoReplyList().begin(); iter != AutoReplyMgr->GetAutoReplyList().end(); ++iter, ++nIndex )
	{
		stAutoReplyMenuItem stItem = *iter;
		if (stItem.bChecked)
		{
			m_pMenu->AppendMenuEx(MF_STRING | MF_CHECKED, nIndex, stItem.strMenuItem.c_str());
		}else
		{
			m_pMenu->AppendMenuEx(MF_STRING, nIndex, stItem.strMenuItem.c_str());
		}

	}
	m_pMenu->AppendMenuEx(MF_SEPARATOR);
	if(AutoReplyMgr->IsAutoReply())
	{
		m_pMenu->AppendMenuEx(MF_STRING, CANCEL_AUTOREPLY_MENU, L"无回复消息");
	}else
	{
		
		m_pMenu->AppendMenuEx(MF_STRING | MF_CHECKED, CANCEL_AUTOREPLY_MENU, L"无回复消息");
	}
	m_pMenu->AppendMenuEx(MF_STRING , SET_AUTOREPLY_MENU, L"自定义...");

	//common::ui::CCursorPos pt;
	//int nreturn = m_pMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, pt.x, pt.y,m_pWnd);
	//m_pWnd->SendMessage( WM_CLICK_AUTOREPLY_MENU, 0, nreturn );
	//m_pMenu->DestroyMenu();

	return m_pMenu;
}

void AutoReplyMenu::DeleteReplyMenu()
{
	if (m_pMenu != NULL)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
		m_pMenu = NULL;
	}
}

MicDlgMenu::MicDlgMenu(  CRoomParentDlg* pParentDlg ) 
:m_pParentDlg( pParentDlg )
{

}

MicDlgMenu::~MicDlgMenu() 
{

}

void MicDlgMenu::PopupMenu( IUserItem * pMyItem, int16 nIndex ,IUserItem * pDstItem )
{
	common::ui::CMenuEx micdlgmenu;
	micdlgmenu.CreatePopupMenu() ;
	UINT32 unDstUIN = pDstItem->GetUserUIN() ;
	if (  pMyItem->GetPublicMicIndex() != 0 && pMyItem->GetUserUIN() == unDstUIN )
	{
		micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_XIAMAI,L"下麦");
	}
	else
	{
		switch (pMyItem->GetUserTitleLevel())
		{
		case core::TITLELEVEL_SUPER_MANAGER:
			{
				//自己是超管
				micdlgmenu.AppendMenuEx(MF_ENABLED , ID_USERLISTMENU_PULLOFF ,L"取消主持") ;
				if ( pDstItem->GetUserTitleLevel() >= core::TITLELEVEL_SUPER_MANAGER )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
				}
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
				//micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
				if ( m_pParentDlg->GetRoomBusiness()->GetRoomOption()->GetPublicMicStatus( nIndex ) == PUBLIC_MIC_STATUS_OPEN )
				{
					micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_CLOSE , L"限制使用此麦") ;
				}
				else
				{
					micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_OPEN , L"取消限制使用此麦") ;
				}
			}
			break;
		case core::TITLELEVEL_OWNER:
			{
				//自己是室主
				micdlgmenu.AppendMenuEx(MF_ENABLED , ID_USERLISTMENU_PULLOFF ,L"取消主持") ;
				if ( pDstItem->GetUserTitleLevel() >= core::TITLELEVEL_OWNER )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
				}
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
				//micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
				if ( pMyItem->GetUserTitleLevel() > pDstItem->GetUserTitleLevel() )
				{
					if ( m_pParentDlg->GetRoomBusiness()->GetRoomOption()->GetPublicMicStatus( nIndex ) == PUBLIC_MIC_STATUS_OPEN )
					{
						micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_CLOSE , L"限制使用此麦") ;
					}
					else
					{
						micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_OPEN , L"取消限制使用此麦") ;
					}
				}	
				else
				{
					micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_CLOSE , L"限制使用此麦") ;
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
				}
				//	micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
			}
			break;
		case core::TITLELEVEL_SECOND_OWNER:
			{
				//自己是副室主
				micdlgmenu.AppendMenuEx(MF_ENABLED , ID_USERLISTMENU_PULLOFF ,L"取消主持") ;
				if ( pDstItem->GetUserTitleLevel() >= core::TITLELEVEL_SECOND_OWNER )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
				}
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				if ( pDstItem->GetUserTitleLevel() > pMyItem->GetUserTitleLevel() )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
					//micdlgmenu.EnableMenuItem( ID_USERLISTMENU_KICKOUT , MF_GRAYED ) ;
				}
				//else if ( pDstItem->GetUserTitleLevel() == pMyItem->GetUserTitleLevel() )
				//{
					//micdlgmenu.EnableMenuItem( ID_USERLISTMENU_KICKOUT , MF_GRAYED ) ;
				//}
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
				//micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			//	micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
			}
			break;
		case core::TITLELEVEL_MANAGER:
			{
				//自己是正管
				micdlgmenu.AppendMenuEx(MF_ENABLED , ID_USERLISTMENU_PULLOFF ,L"取消主持") ;
				if ( pDstItem->GetUserTitleLevel() >= core::TITLELEVEL_MANAGER )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
				}
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_KICKOUT,L"踢出房间");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				if ( pDstItem->GetUserTitleLevel() > pMyItem->GetUserTitleLevel() )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
					//micdlgmenu.EnableMenuItem( ID_USERLISTMENU_KICKOUT , MF_GRAYED ) ;
				}
				//else if ( pDstItem->GetUserTitleLevel() == pMyItem->GetUserTitleLevel() )
				//{
				//	micdlgmenu.EnableMenuItem( ID_USERLISTMENU_KICKOUT , MF_GRAYED ) ;
				//}
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
				//micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			//	micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
			}
			break;
		case core::TITLELEVEL_TEMP_MANAGER:
			{
				//自己是临管
				micdlgmenu.AppendMenuEx(MF_ENABLED , ID_USERLISTMENU_PULLOFF ,L"取消主持") ;
				if ( pDstItem->GetUserTitleLevel() >= core::TITLELEVEL_TEMP_MANAGER )
				{
					micdlgmenu.EnableMenuItem( ID_USERLISTMENU_PULLOFF , MF_GRAYED ) ;
				}				
			}
		case core::TITLELEVEL_NONE:
			{
				//自己是普通用户
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETSENDOBJ,L"选中赠送对象");
				micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_SETCHATOBJ,L"选中聊天对象");
				if ( pMyItem->GetUserVipLevel() != core::VIPLEVEL_NONE )
				{
					micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_STAMP,L"盖章");
					micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_USERINFO,L"用户信息");
					//micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				}								
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYNICKNAME,L"复制昵称");
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_COPYID,L"复制ID");
			//	micdlgmenu.AppendMenuEx(MF_SEPARATOR);
				//micdlgmenu.AppendMenuEx(MF_ENABLED,ID_USERLISTMENU_CHECKNETSPEED,L"查看他的网速");
			}
			break;
		default:break;
		}
	}
	common::ui::CCursorPos pt;
	int nreturn = micdlgmenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN|TPM_RETURNCMD , pt.x, pt.y, CWnd::FromHandle(m_pParentDlg->GetSafeHwnd()) );
	if ( nreturn != 0 )
	{
		OnCommand( nreturn ,unDstUIN , nIndex ) ;
	}
	micdlgmenu.DestroyMenu() ;
}

void MicDlgMenu::PopupMenu( IUserItem* pMyItem , int16 nIndex ) 
{
	if ( pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_TEMP_MANAGER )
	{
		common::ui::CMenuEx micdlgmenu;
		micdlgmenu.CreatePopupMenu() ;
		int nPublicMicStatus = m_pParentDlg->GetRoomOption()->GetPublicMicStatus( nIndex ) ;
		micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SHANGMAI ,L"上麦" ) ;
		if ( nPublicMicStatus != PUBLIC_MIC_STATUS_OPEN )
		{
			micdlgmenu.EnableMenuItem( ID_USERLISTMENU_SHANGMAI , MF_GRAYED ) ;
		}
		if ( pMyItem->GetUserTitleLevel() >= core::TITLELEVEL_OWNER )
		{
			if ( nPublicMicStatus == PUBLIC_MIC_STATUS_OPEN )
			{
				micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_CLOSE , L"限制使用此麦") ;
			}
			else
			{
				micdlgmenu.AppendMenuEx( MF_ENABLED , ID_USERLISTMENU_SET_MIC_OPEN , L"取消限制使用此麦") ;
			}
		}

		common::ui::CCursorPos pt;
		int nreturn = micdlgmenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN|TPM_RETURNCMD , pt.x, pt.y, CWnd::FromHandle(m_pParentDlg->GetSafeHwnd()) );
		if ( nreturn != 0 )
		{
			OnCommand( nreturn , nIndex ) ;
		}
		micdlgmenu.DestroyMenu() ;
	}
}

void MicDlgMenu::OnCommand( UINT32 nCommand ,uint32 unDstUIN , int16 nIndex ) 
{
	IUserItem * pDstItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unDstUIN ) ;
	if ( pDstItem == NULL && nCommand != ID_USERLISTMENU_SET_MIC_OPEN && nCommand != ID_USERLISTMENU_SET_MIC_CLOSE )
	{
		return ;
	}
	switch( nCommand )
	{
	case ID_USERLISTMENU_XIAMAI:
		{
			//下麦
			m_pParentDlg->FangMai( true ) ;
		}
		break;
	case ID_USERLISTMENU_PULLOFF:
		{
			//取消主持
			CSendEventManager::SendPushOffEvent(m_pParentDlg->GetRoomID(), pDstItem->GetUserUIN());
		}
		break;
	case ID_USERLISTMENU_SETSENDOBJ:
		{
			//选中聊天对象
			m_pParentDlg->SetSendObj( pDstItem->GetUserName().c_str() , pDstItem->GetUserUIN() ) ;
		}
		break;
	case ID_USERLISTMENU_SETCHATOBJ:
		{
			//选中赠送对象
			m_pParentDlg->SetChatObj( pDstItem->GetUserName().c_str() , pDstItem->GetUserUIN() ) ;
		}
		break;
	case ID_USERLISTMENU_KICKOUT:
		{
			//踢出房间
			m_pParentDlg->m_pSelectUserItem = pDstItem ;
			m_pParentDlg->ShowKickDlg(0);
		}
		break;
	case ID_USERLISTMENU_USERINFO:
		{
			//用户信息
			m_pParentDlg->m_pSelectUserItem = pDstItem ;
			m_pParentDlg->OnUserInfo( m_pParentDlg->m_pSelectUserItem ) ;
		}
		break;
	case ID_USERLISTMENU_STAMP:
		{
			//盖章
			CString strUUID ;
			strUUID.Format( L"%u" , pDstItem->GetUserUIN() ) ;			
			m_pParentDlg->OnClickStampLink( CString(pDstItem->GetUserName().c_str()) , strUUID ) ;
		}
		break;
	case ID_USERLISTMENU_COPYNICKNAME:
		{
			common::utility::systemhelper::CopyDataToClipbord( pDstItem->GetUserName() ) ;
		}
		break;
	case ID_USERLISTMENU_COPYID:
		{
			CString strTemp ;
			strTemp.Format( L"%u" , pDstItem->GetUser179ID() ) ;
			common::utility::systemhelper::CopyDataToClipbord( strTemp.GetBuffer() ) ;
		}
		break;
	case ID_USERLISTMENU_CHECKNETSPEED:
		{

		}
		break;
	case ID_USERLISTMENU_SET_MIC_OPEN:
		{
			CSendEventManager::SendSetMicStatus( m_pParentDlg->GetRoomID() , nIndex , PUBLIC_MIC_STATUS_OPEN ) ;
		}	
		break;
	case ID_USERLISTMENU_SET_MIC_CLOSE:
		{
			CSendEventManager::SendSetMicStatus( m_pParentDlg->GetRoomID() , nIndex , PUBLIC_MIC_STATUS_CLOSE ) ;
		}
		break;
	default:break;
	}
}

void MicDlgMenu::OnCommand( UINT32 nCommand ,int16 nIndex ) 
{
	switch( nCommand )
	{
	case ID_USERLISTMENU_SET_MIC_OPEN:
		{
			CSendEventManager::SendSetMicStatus( m_pParentDlg->GetRoomID() , nIndex , PUBLIC_MIC_STATUS_OPEN ) ;
		}	
		break;
	case ID_USERLISTMENU_SET_MIC_CLOSE:
		{
			CSendEventManager::SendSetMicStatus( m_pParentDlg->GetRoomID() , nIndex , PUBLIC_MIC_STATUS_CLOSE ) ;
		}
		break;
	case ID_USERLISTMENU_SHANGMAI:
		{
			core::CurInfo myCurInfo = GetCurInfo() ;
			CSendEventManager::SendPushOnEvent(m_pParentDlg->GetRoomID(), myCurInfo.unUIN , nIndex ) ;
		}
		break;
	default:break;
	}
}

FlyTextMenu::FlyTextMenu( CWnd * pWnd, int nType )
	: m_pWnd(pWnd),
	  m_nType(nType)
{

}

FlyTextMenu::~FlyTextMenu()
{

}

int FlyTextMenu::PopupMenu()
{
	common::ui::CMenuEx menu;
	menu.CreatePopupMenu();

	if (m_nType == 0)
	{
		menu.AppendMenuEx(MF_STRING | MF_CHECKED, FLYINT_TEXT_EFFECT_RANDOM, L"随机");
	}
	else
	{
		menu.AppendMenuEx(MF_STRING, FLYINT_TEXT_EFFECT_RANDOM, L"随机");
	}
	if (m_nType == 1)
	{
		menu.AppendMenuEx(MF_STRING | MF_CHECKED, 1, L"火箭");
	}
	else
	{
		menu.AppendMenuEx(MF_STRING, 1, L"火箭");
	}
	if (m_nType == 2)
	{
		menu.AppendMenuEx(MF_STRING | MF_CHECKED, 2, L"飞机");
	}
	else
	{
		menu.AppendMenuEx(MF_STRING, 2, L"飞机");
	}
	if (m_nType == 3)
	{
		menu.AppendMenuEx(MF_STRING | MF_CHECKED, 3, L"羽毛");
	}
	else
	{
		menu.AppendMenuEx(MF_STRING, 3, L"羽毛");
	}
	if (m_nType == 4)
	{
		menu.AppendMenuEx(MF_STRING | MF_CHECKED, 4, L"纸飞机");
	}
	else
	{
		menu.AppendMenuEx(MF_STRING, 4, L"纸飞机");
	}

	common::ui::CCursorPos pt;
	int nRet = menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, pt.x, pt.y, m_pWnd);

	menu.DestroyMenu();
	return nRet;
}