#include "StdAfx.h"
#include "RoomActor.h"
#include <algorithm>
#include "HallDefine.h"
#include "..\Room.h"
#include "coredefine.h"
#include "..\RoomParentDlg.h"
#include "..\..\include\utility\SystemHelper.h"
#include "SkinUtil.h"

#define COLUMN_MAX 4

CRoomActor::CRoomActor(CRoomParentDlg* pParentDlg)
{

	CString strTemp;
	if(m_pParentDlg != NULL)
	{
		strTemp.Format(_T("\n-----------CRoomActor::m_pParentDlg :%0x"),pParentDlg->GetSafeHwnd());
	}
	else
	{
		strTemp.Format(_T("\n-----------CRoomActor::m_pParentDlg :000000"));
	}
	OutputDebugString(strTemp);

	m_pParentDlg = pParentDlg;
	m_pRoomParentSkinEngine = NULL;
	m_pRoomParentSkinEngine = m_pParentDlg->GetSkinEngine();
	ASSERT(m_pRoomParentSkinEngine != NULL);

	m_IsInInit = false;
	m_curSkinPage = 0;
	m_maxSkinPage = 0;
	m_RoomActorIsDeploy = false;
	
	pActorMenuDlg = NULL;
	SwitchResourceToModule(_T("Room"));	
	pActorMenuDlg = new CRoomActorMenuDlg(m_pParentDlg,this) ;
	pActorMenuDlg->Create( CRoomActorMenuDlg::IDD , m_pParentDlg ) ;
	pActorMenuDlg->Hide();

	
	
}

CRoomActor::~CRoomActor()
{
	actorClear();
	if(pActorMenuDlg != NULL)
	{
		pActorMenuDlg->DestroyWindow();
		delete pActorMenuDlg;
		pActorMenuDlg = NULL;
	}
}

void  CRoomActor::InitInRoomActor()
{
	m_IsInInit = true;

	UserManager * pUserManager = m_pParentDlg->GetRoomBusiness()->GetUserManager();
	ASSERT(pUserManager != NULL);

	std::vector<IUserItem*> vecActorList;
	pUserManager->GetAllActors(vecActorList);  //获得房间所有在线主播	

	for(int i=0; i<vecActorList.size();i++)
	{
		ActorInfo *InActor = new ActorInfo;
		IUserItem* pIUserIterm = vecActorList[i]; 
		if(pIUserIterm == NULL)
		{
			continue;
		}

		InActor->m_ActorUin = pIUserIterm->GetUserUIN();
		InActor->m_VipLevel = pIUserIterm->GetUserVipLevel();
		InActor->m_RichLevel = pIUserIterm->GetUserRichLevel();
		InActor->m_IsLine = true;  //在线 (即在房间)
		InActor->m_IsMan = pIUserIterm->IsMale();
		InActor->m_ActorRoleName = pIUserIterm->GetUserName();
		LoadActorImage(InActor);
		m_ActorSet.insert(InActor);

	}	
	
	FreshRoomActor();



}

//申请离线主播列表
void  CRoomActor::InitOffRoomAcotrRequest()
{
	uint32 iRoomID = m_pParentDlg->GetRoomID();
	Event _event = MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_GET_OFFLINE_ACTOR_REQ, 
		MODULE_ID_CORE,iRoomID,0,0);

	g_RoomModule->m_pModuleManager->PushEvent(_event);		

}

//申请离线主播列表返回响应函数
void  CRoomActor::OnInitOffRoomAcotrRsp(Event const& evt)
{
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::GetOfflineActorRspEvent *pInfo = (core::GetOfflineActorRspEvent*)(evt.m_pstExtraInfo) ;

	for(int i=0;i<pInfo->offlineActor.size();i++)
	{
		core::room_user_info &offLine = pInfo->offlineActor[i];

		if( ((offLine.unBusiness&core::BUSINESSTYPE_SONGER) 
			|| (offLine.unBusiness&core::BUSINESSTYPE_DANCER) 
			|| (offLine.unBusiness&core::BUSINESSTYPE_HOST)) && offLine.unTitleLevel > core::TITLELEVEL_TEMP_MANAGER)    //为艺人且为管理员
		{
			ActorInfo *offActor = new ActorInfo;
			offActor->m_ActorUin = offLine.unUIN;
			offActor->m_VipLevel = offLine.unVipLevel;
			offActor->m_RichLevel = offLine.unRichLevel;
			offActor->m_IsLine = false;  //离线 (即不在房间)
			offActor->m_IsMan = offLine.bMan;		
			offActor->m_ActorRoleName = offLine.strName;

			LoadActorImage(offActor);
			m_ActorSet.insert(offActor);
		}

	}	
	
	FreshRoomActor();

}

void CRoomActor::actorInOffRoom(ActorInfo *pActor)
{
	if(pActor == NULL)
		return;

	std::set<ActorInfo*,compare_actor>::iterator it;
	for(it = m_ActorSet.begin(); it != m_ActorSet.end();)
	{
		ActorInfo* pActorInfo = *it;
		if(pActorInfo->m_ActorUin == pActor->m_ActorUin)
		{
			it = m_ActorSet.erase(it);  //删除旧的uin主播数据
			if(pActorInfo != NULL )
			{
				delete pActorInfo;
				pActorInfo = NULL;
			}				

		}
		else
		{
			it++;
		}

	}	

	m_ActorSet.insert(pActor);
}


//删除列表
void CRoomActor::actorClear()
{
	std::set<ActorInfo*,compare_actor>::iterator iter;
	for(iter = m_ActorSet.begin();iter != m_ActorSet.end(); iter++)
	{
		ActorInfo* pDelActor = *iter;
		if(pDelActor != NULL )
		{
			delete pDelActor;
			pDelActor = NULL;
		}
	}

	m_ActorSet.clear();

}

//获取m_ActorSet列表中第index个迭代器
bool CRoomActor::GetActorSetIterator(int index,std::set<ActorInfo*,compare_actor>::iterator &indexIter)
{
	if(index <=0 )
	{
		indexIter = m_ActorSet.end();
		return false;
	}


	int num = 0;
	std::set<ActorInfo*,compare_actor>::iterator iter;
	for(iter = m_ActorSet.begin();iter != m_ActorSet.end(); iter++)
	{
		num++;
		if(num == index)
		{
			indexIter = iter;
			return true;
		}		

	}

	indexIter = m_ActorSet.end();
	return false;

}

//获取m_ActorSet列表中uin主播的迭代器
bool CRoomActor::GetActorUin(uint32 UIN,std::set<ActorInfo*,compare_actor>::iterator &indexIter)
{	
	std::set<ActorInfo*,compare_actor>::iterator iter;
	for(iter = m_ActorSet.begin();iter != m_ActorSet.end(); iter++)
	{		
		ActorInfo* pActor = *iter;
		if(pActor->m_ActorUin == UIN)
		{
			indexIter = iter;
			return true;
		}		

	}

	indexIter = m_ActorSet.end();
	return false;
}

//刷新房间主播列表数据
void CRoomActor::FreshRoomActor()
{
	m_curSkinPage = 1;  //显示第1页
	if(m_ActorSet.size() <= 0)
	{
		m_maxSkinPage = 0;
	}
	else
	{
		m_maxSkinPage = (m_ActorSet.size() -1 )/COLUMN_MAX + 1;   //计算最多可显示页  
	}	


	ShowSkinRoomActor();

}

//主播进房(上线)
void CRoomActor::ActorInRoom(IUserItem* pUserItem)
{
	if(pUserItem == NULL || !m_IsInInit)  //需先进行InInit主播列表初始化
		return;

	if(pUserItem->IsActor() && pUserItem->GetUserTitleLevel()> core::TITLELEVEL_TEMP_MANAGER)  //为艺人且为管理员
	{
		ActorInfo *InActor = new ActorInfo;
		InActor->m_ActorUin = pUserItem->GetUserUIN();
		InActor->m_VipLevel = pUserItem->GetUserVipLevel();
		InActor->m_RichLevel = pUserItem->GetUserRichLevel();
		InActor->m_IsLine = true;  //在线 (即在房间)
		InActor->m_IsMan = pUserItem->IsMale();
		InActor->m_ActorRoleName = pUserItem->GetUserName();
		LoadActorImage(InActor);
		actorInOffRoom(InActor);
		FreshRoomActor();

	}



}

//主播退房(离线)
void CRoomActor::ActorOffRoom(IUserItem* pUserItem)
{
	if(pUserItem == NULL)
		return;

	if(pUserItem->IsActor() && pUserItem->GetUserTitleLevel()> core::TITLELEVEL_TEMP_MANAGER)  //为艺人且为管理员
	{
		ActorInfo *offActor = new ActorInfo;
		offActor->m_ActorUin = pUserItem->GetUserUIN();
		offActor->m_VipLevel = pUserItem->GetUserVipLevel();
		offActor->m_RichLevel = pUserItem->GetUserRichLevel();
		offActor->m_IsLine = false;  //离线 (即不在房间)
		offActor->m_IsMan = pUserItem->IsMale();
		offActor->m_ActorRoleName = pUserItem->GetUserName();
		LoadActorImage(offActor);
		actorInOffRoom(offActor);
		FreshRoomActor();


	}


}

//最新头像图片响应函数
void CRoomActor::OnActorHeadImageUpdate(uint32 UIN)
{		

	std::set<ActorInfo*,compare_actor>::iterator actorIter;
	if(!GetActorUin(UIN,actorIter))
	{
		return;
	}
	
	ActorInfo *pActor = *actorIter;	
	LoadActorImage(pActor,false);
	FreshRoomActor();

	
}


//其他主播在房间时的vip等级、富豪等级更新
void CRoomActor::ProcessNotifyUpdateActorLevel(Event const& evt)
{	
	ASSERT(evt.m_pstExtraInfo != NULL);
	core::NotifyUserInfoUpdateEvent * pInfo = (core::NotifyUserInfoUpdateEvent*)evt.m_pstExtraInfo;

	IUserItem* pUserItem = NULL;
	//目前只用在更新用户vip等级
	if (pInfo->map_key2uin8.size() != 0)
	{
		if (pInfo->map_key2uin8.find(core::INFOTYPE_VIP_LEVEL) != pInfo->map_key2uin8.end())  //VIP等级更新
		{
			pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( pInfo->unSrcUIN ) ;
		}

		if (pInfo->map_key2uin8.find(core::INFOTYPE_RICH_LEVEL) != pInfo->map_key2uin8.end()) //富豪等级更新
		{
			pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( pInfo->unSrcUIN ) ;
		}
	}

	if(pUserItem == NULL)
		return;

	if(pUserItem->IsActor() && pUserItem->GetUserTitleLevel()> core::TITLELEVEL_TEMP_MANAGER)  //为艺人且为管理员
	{
		ActorInRoom(pUserItem);

	}



}

//主播自已在房间时的vip等级、富豪等级更新
void CRoomActor::ProcessMyActorLevelUpdate()
{
	uint32 unUIN =  GetCurInfo().unUIN;
	IUserItem* pUserItem =  m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unUIN ) ;

	if(pUserItem == NULL)
		return;

	if(pUserItem->IsActor() && pUserItem->GetUserTitleLevel()> core::TITLELEVEL_TEMP_MANAGER)  //为艺人且为管理员
	{
		ActorInRoom(pUserItem);		
	}
}

void CRoomActor::OnActorMouseMove(UINT nHitTest,CPoint point)
{
	CPoint CurPoint ;
	GetCursorPos( &CurPoint ) ;  //鼠标当前point
	m_pParentDlg->ScreenToClient(&CurPoint);
	if(CurPoint == pointOld)
	{
		return;
	}

	pointOld = CurPoint;

	int headIndex;
	if(!CurPointIsActorRect(headIndex))  //当前鼠标是否在房间主播头像中
	{
		ActorMouseLevel();
		return;  //当前鼠标不在房间主播头像中
	}

	CString strHead;
	CComVariant varVisible = FALSE;		
	CComVariant varValue; 

	strHead.Format(_T("imgActorHead%dbk"),headIndex);	
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), strHead.GetBuffer(), L"visible", &varVisible);  //第i个主播头像是否有显示
	if(varVisible.boolVal)
	{
		//移动并显示主播头像框
		m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), strHead.GetBuffer(), L"top", &varValue);
		varValue.intVal = varValue.intVal-1;
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadCtrlBack_H", L"top", varValue);
		m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), strHead.GetBuffer(), L"left", &varValue);
		varValue.intVal = varValue.intVal-1;
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadCtrlBack_H", L"left", varValue);

		varValue = TRUE;
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadCtrlBack_H", L"visible", varValue);	
		
		if(pActorMenuDlg != NULL)
		{
			pActorMenuDlg->SetTimer(ROOMACTOR_MOUSE_TIMER_IDEVENT,ROOMACTOR_MOUSE_TIMER_INTERVALTIME,NULL);				
		}

	}
	strHead.ReleaseBuffer();	

}

//在房间主播框中点击鼠标
void CRoomActor::OnClickBtnActorHead(int headIndex)
{
	int curIndex = (m_curSkinPage - 1) * COLUMN_MAX + headIndex;
	std::set<ActorInfo*,compare_actor>::iterator itIndex;
	if(!GetActorSetIterator(curIndex,itIndex))
	{
		return;
	}	

	if(pActorMenuDlg != NULL)
	{
		ActorInfo *pActor = *itIndex;
		if(pActor != NULL && pActor->m_IsLine)
		{
			pActorMenuDlg->SetActorInfo(pActor);
			pActorMenuDlg->OnClickbtnSelTalk();
			pActorMenuDlg->OnClickbtnSelSendGifts();
		}
	}
}

void CRoomActor::ActorMouseLevel()
{
	CPoint point ;
	GetCursorPos( &point ) ;  //鼠标当前point
	m_pParentDlg->ScreenToClient(&point);	

	if(pActorMenuDlg != NULL && pActorMenuDlg->GetSafeHwnd() != NULL)
	{		

		CRect rect ;
		pActorMenuDlg->GetWindowRect(&rect) ;
		if (!rect.PtInRect(point) )
		{
			pActorMenuDlg->Hide();

			CComVariant varValue = FALSE;
			m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadCtrlBack_H", L"visible", varValue);
		}
	}
}

//当前鼠标是否在房间主播头像上
//curActorIndex  返回当前鼠标所在的主播头像index
bool CRoomActor::CurPointIsActorRect(int &curActorIndex)
{
	curActorIndex = 0;
	CPoint point ;
	GetCursorPos( &point ) ;  //鼠标当前point
	m_pParentDlg->ScreenToClient(&point);	

	CRect rcHeadFramg(0,0,0,0);
	m_pRoomParentSkinEngine->GetSubSkinPos(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadBk", &(rcHeadFramg.left), &(rcHeadFramg.top), &(rcHeadFramg.right), &(rcHeadFramg.bottom));	


	if (PtInRect(&rcHeadFramg, point))  //鼠标在房间主播列表内
	{
		CRect rcHead;
		CString strHead;

		CComVariant varVisible = FALSE;
		CComVariant varValue; 
		for(int i=1;i<= COLUMN_MAX;i++)
		{
			strHead.Format(_T("imgActorHead%dbk"),i);	
			m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), strHead.GetBuffer(), L"visible", &varVisible);  //第i个主播头像是否有显示
			if(varVisible.boolVal)
			{
				m_pRoomParentSkinEngine->GetSubSkinPos(m_pParentDlg->GetSafeHwnd(), strHead.GetBuffer(), &(rcHead.left), &(rcHead.top), &(rcHead.right), &(rcHead.bottom));	
				if (PtInRect(&rcHead, point))  //当前鼠标是否在第i个主播头像框内
				{ 	
					curActorIndex = i;				
					break;
				}

			}
			strHead.ReleaseBuffer();
		}

		if(curActorIndex == 0)  //鼠标不在某个主播头像上
		{			
			OutputDebugString(_T("不在头像中"));
			return false;
		}
		else  //鼠标在某个主播头像上
		{			
			return true;
		}

	}
	else //鼠标不在房间主播列表上
	{		
		OutputDebugString(_T("不在列表中"));
		return false;
	}
}

void CRoomActor::ShowActorMenuDlg()
{
	
	int headIndex;
	if(!CurPointIsActorRect(headIndex))  //当前鼠标是否在房间主播头像中
	{			
		return;  //当前鼠标不在房间主播头像中
	}		

	int curIndex = (m_curSkinPage - 1) * COLUMN_MAX + headIndex;
	std::set<ActorInfo*,compare_actor>::iterator itIndex;
	if(!GetActorSetIterator(curIndex,itIndex))
	{
		return;
	}	

	
	ActorInfo* pActor = *itIndex;	

	CString headName;
	headName.Format(_T("imgActorHead%d"),headIndex);	
	CRect rcTemp;
	m_pRoomParentSkinEngine->GetSubSkinPos(m_pParentDlg->GetSafeHwnd(),headName.GetBuffer(),&(rcTemp.left),&(rcTemp.top),&(rcTemp.right),&(rcTemp.bottom));	
	headName.ReleaseBuffer();
	m_pParentDlg->ClientToScreen(&rcTemp);
	
	if(pActor->m_IsLine)
	{
		pActorMenuDlg->MoveWindow( rcTemp.right-5, rcTemp.top+8 , 110 , 168 ) ;
	}
	else
	{
		pActorMenuDlg->MoveWindow( rcTemp.right-5, rcTemp.top+8 , 110 , 60 ) ;
	}
	
	pActorMenuDlg->Show( pActor ) ;

}

void CRoomActor::VisibleBtnHeadUpDown()
{
	CComVariant varVisible = FALSE;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadUp", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadDown", L"visible", varVisible);

	if(m_RoomActorIsDeploy)
	{
		if(m_curSkinPage >= 2)
		{
			//有上一页按扭
			CComVariant varVisible = TRUE;
			m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadUp", L"visible", varVisible);		
		}
		if(m_curSkinPage < m_maxSkinPage)
		{
			//有下一页按扭
			CComVariant varVisible = TRUE;
			m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadDown", L"visible", varVisible);

		}

	}

}

//上一页
void CRoomActor::OnClickBtnRoomActorUp()
{
	m_curSkinPage-=1;

	if(m_curSkinPage < 1)
	{		
		m_curSkinPage = 1;

	}	

	ShowSkinRoomActor();

}



//下一页
void CRoomActor::OnClickBtnRoomActorDown()
{
	m_curSkinPage+=1;

	if(m_curSkinPage > m_maxSkinPage)
	{		
		m_curSkinPage = m_maxSkinPage;

	}	

	ShowSkinRoomActor();

}

//点击标题中的向下或关闭按扭的响应函数,以便展开或收缩房间主播列表
void CRoomActor::OnClickBtnRoomActorTitle()
{
	if(m_RoomActorIsDeploy)
	{
		m_RoomActorIsDeploy = false;
	}
	else
	{
		m_RoomActorIsDeploy = true;
		m_curSkinPage = 1;
		FreshImage();
	}
	
	ShowSkinRoomActor();

}

//房间主播收缩
void CRoomActor::ContractRoomActor()
{
	//房间主播收缩

	//调整房间主播框位置
	CComVariant varVisible = TRUE;		
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTopTitle", L"visible", varVisible);		
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadTopTitleDown", L"visible", varVisible);		


	varVisible = FALSE;	
	CString acthorHead,acthorHeadbk;
	for(int i=1; i<=COLUMN_MAX ;i++ )   
	{		
		acthorHead.Format(_T("imgActorHead%d"),i);
		acthorHeadbk.Format(_T("imgActorHead%dbk"),i);
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHead.GetBuffer(), L"visible", varVisible);
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHeadbk.GetBuffer(), L"visible", varVisible);
		acthorHead.ReleaseBuffer();	
	}
	
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTop", L"visible", varVisible);	
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadBk", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadUp", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadDown", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTitle", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadTitleClose", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadCtrlBack_H", L"visible", varVisible);

	VisibleBtnHeadUpDown();  //设置上页下页按扭是否显示
}

void CRoomActor::ShowSkinRoomActor()
{	
	if(!m_RoomActorIsDeploy)
	{
		//房间主播收缩
		ContractRoomActor();

		return;
	}


	//房间主播展开
	int beginIndex = (m_curSkinPage-1) * COLUMN_MAX + 1;
	int headIndex = 1;
	CComVariant varHeadPath;
	CString acthorHead,acthorHeadBk;

	std::set<ActorInfo*,compare_actor>::iterator itIndex;
	GetActorSetIterator(beginIndex,itIndex);

	CComVariant varVisible = TRUE;
	for(itIndex; itIndex != m_ActorSet.end();itIndex++ )
	{

		//给房间主播列表设置头像显示
		ActorInfo* pActor =*itIndex;
		if(pActor == NULL)
		{
			continue;
		}
		acthorHead.Format(_T("imgActorHead%d"),headIndex);
		acthorHeadBk.Format(_T("imgActorHead%dbk"),headIndex);
		if(pActor->m_IsLine)
		{
			varHeadPath = (LONG)((IUnknown*)pActor->m_HeadImage);
		}
		else
		{
			varHeadPath = (LONG)((IUnknown*)pActor->m_HeadImageGray);
		}

		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHead.GetBuffer(), L"externalImage", varHeadPath);
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHead.GetBuffer(), L"visible", varVisible);
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHeadBk.GetBuffer(), L"visible", varVisible);
		acthorHead.ReleaseBuffer();		

		headIndex++;
		if(headIndex > COLUMN_MAX)
		{
			break; //房间主播列表最多显示数 COLUMN_MAX
		}
	}

	CComVariant varTop,varWidth,varHeight;
	CComVariant varValue1,varValue2;
	if(headIndex - 1 > 0)
	{
		acthorHead.Format(_T("imgActorHead%d"),headIndex-1);

	}
	else
	{
		acthorHead.Format(_T("imgActorHead1"));
	}

	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTop", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadBk", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTitle", L"visible", varVisible);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadTitleClose", L"visible", varVisible);
	VisibleBtnHeadUpDown();  //设置上页下页按扭是否显示


	varVisible = FALSE;	
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTopTitle", L"visible", varVisible);		
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadTopTitleDown", L"visible", varVisible);	

	//调整房间主播向下按扭位置  
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHead.GetBuffer(), L"top", &varValue1);
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHead.GetBuffer(), L"height", &varValue2);
	varTop = varValue1.intVal + varValue2.intVal + 6;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadDown", L"top", varTop);
	
	//调整房间主播标题位置
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadDown", L"height", &varValue2);
	varTop = varTop.intVal + varValue2.intVal;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTitle", L"top", varTop);
	varTop = varTop.intVal + 20;
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"btnActorHeadTitleClose", L"top", varTop);

	//调整房间主播背景位置
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTitle", L"top", &varTop);
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTop", L"top", &varValue1);	
	m_pRoomParentSkinEngine->GetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadTop", L"height", &varValue2);	
	varHeight =  varTop.intVal - varValue1.intVal - varValue2.intVal ;	
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadBk", L"height", varHeight);
	m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), L"imgActorHeadCtrlBack_H", L"visible", varVisible);
	


	//房间主播列表未显示完成时，剩除头像位置不显示	
	for(int i=headIndex; i<=COLUMN_MAX ;i++ )   
	{		
		acthorHead.Format(_T("imgActorHead%d"),i);
		acthorHeadBk.Format(_T("imgActorHead%dbk"),i);
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHead.GetBuffer(), L"visible", varVisible);
		m_pRoomParentSkinEngine->SetSubSkinProp(m_pParentDlg->GetSafeHwnd(), acthorHeadBk.GetBuffer(), L"visible", varVisible);
		acthorHead.ReleaseBuffer();	
	}


}

//刷新图片
void CRoomActor::FreshImage()
{	

	std::set<ActorInfo*,compare_actor>::iterator iter;
	for(iter = m_ActorSet.begin();iter != m_ActorSet.end(); iter++)
	{
		ActorInfo* pActor = *iter;
		if(pActor != NULL )
		{
			Hall::HeadImage headImage;
			headImage.m_bMan = pActor->m_IsMan;
			headImage.m_isGuest = false;
			headImage.m_unUIN = pActor->m_ActorUin;	
			g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
		}
	}

}

void CRoomActor::LoadActorImage(ActorInfo *pActor,bool isUpdate /* = true */)
{
	if(pActor == NULL)
		return;

	Hall::HeadImage headImage;
	headImage.m_bMan = pActor->m_IsMan;
	headImage.m_isGuest = false;
	headImage.m_unUIN = pActor->m_ActorUin;
	if(isUpdate)
	{
		g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);

	}
	else
	{
		g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH, (param)&headImage);
	}

	CString headPath;
	headPath.Format(_T("%s"),headImage.m_HeadImagePath.c_str());

	if(pActor->m_HeadImage == NULL)
	{
		pActor->m_HeadImage = new common::ui::CImageImpl;
	}
	if(pActor->m_HeadImageGray == NULL)
	{
		pActor->m_HeadImageGray = new common::ui::CImageImpl;
	}	

	pActor->m_HeadImage->Load(headImage.m_HeadImagePath.c_str());	
	pActor->m_HeadImageGray->Load(headImage.m_HeadImagePath.c_str());
	pActor->m_HeadImageGray->Gray(); //图像灰度化

	LONG lValue;
	lValue = (LONG)((IUnknown*)pActor->m_HeadImage);
	CComVariant varImage =lValue;	

}

//新手引导
void CRoomActor::ShowGuide()
{
	if(pActorMenuDlg != NULL)
	{
		pActorMenuDlg->SetTimer(ROOMACTOR_GIDUE_TIMER_IDEVENT,ROOMACTOR_GIDUE_TIMER_INTERVALTIME,NULL);
		OnClickBtnRoomActorTitle();
	}
}

//引导结束
void CRoomActor::ShowGuideEnd()
{
	if(pActorMenuDlg != NULL)
	{
		if(m_RoomActorIsDeploy)
		{
			OnClickBtnRoomActorTitle();
		}
	}

}
