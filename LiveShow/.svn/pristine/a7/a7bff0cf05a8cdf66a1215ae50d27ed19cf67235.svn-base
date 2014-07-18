#pragma once
#include <set>
#include <vector>
#include "51ktvtype.h"
#include "..\usermanager\useritem.h"
#include "Skin_i.h"
#include "RoomActorMenuDlg.h"



//按m_IsLine排序之后，再按vip等级、富豪等级、uin排序
struct compare_actor
{
	bool operator()(const ActorInfo *x,const ActorInfo *y) const   //比较函数,用于排序规则
	{
		if(!(x->m_IsLine == y->m_IsLine))  //m_IsLine排序  
		{
			return (x->m_IsLine);
		}

		if(!(x->m_VipLevel == y->m_VipLevel))  //vip等级不一至时按vip等级降序排序
		{
			return x->m_VipLevel > y->m_VipLevel;
		}
		else if(!(x->m_RichLevel == y->m_RichLevel))//vip等级一至且富豪等级不一至时按富豪等级降序排序
		{
			return x->m_RichLevel > y->m_RichLevel;
		}
		else
		{
			return x->m_ActorUin < y->m_ActorUin;  //否则(vip等级、富豪等级都一至时) 按uin升序排序
		}	
	
	}
};


class CRoomParentDlg ;
class CRoomActor    //房间主播
{
public:
	CRoomActor::CRoomActor(CRoomParentDlg* pParentDlg);
	virtual CRoomActor::~CRoomActor();

public:
	void InitInRoomActor();//初始化在线主播列表
	void InitOffRoomAcotrRequest(); //申请离线主播列表
	void OnInitOffRoomAcotrRsp(Event const& evt); //申请离线主播列表返回响应函数
	void ActorInRoom(IUserItem* pUserItem);  //主播进房(上线)
	void ActorOffRoom(IUserItem* pUserItem); //主播退房(离线)
	void ProcessNotifyUpdateActorLevel(Event const& evt); //其他主播在房间时的vip等级、富豪等级更新
	void ProcessMyActorLevelUpdate();//主播自已在房间时的vip等级、富豪等级更新
	void OnActorHeadImageUpdate(uint32 UIN);  //最新头像图片响应函数
	void OnActorMouseMove(UINT nHitTest,CPoint point); //在房间主播框中移动鼠标	
	void OnClickBtnRoomActorUp();  //上一页响应函数
	void OnClickBtnRoomActorDown(); //下一页响应函数
	void OnClickBtnRoomActorTitle(); //点击标题响应函数
	void OnClickBtnActorHead(int headIndex);
	void ShowActorMenuDlg();	
	void ShowSkinRoomActor(); //房间主播展开时 显示主播列表
	void ShowGuide(); //新手引导
	void ShowGuideEnd();//引导结束	

private:
	void actorInOffRoom(ActorInfo *pActor); //主播进退房	
	void actorClear();//删除列表	
	void ContractRoomActor(); //房间主播收缩
	void VisibleBtnHeadUpDown(); //设置上页下页按扭是否显示
	void LoadActorImage(ActorInfo *pActor,bool isUpdate = true);  //加载图片	
	void FreshImage();  //刷新图片	
	bool GetActorSetIterator(int index,std::set<ActorInfo*,compare_actor>::iterator &indexIter);  //获取m_ActorSet列表中第index个迭代器
	bool GetActorUin(uint32 UIN,std::set<ActorInfo*,compare_actor>::iterator &indexIter);  //获取m_ActorSet列表中uin主播的迭代器
	
	void ActorMouseLevel();
	bool CurPointIsActorRect(int &curActorIndex);//当前鼠标是否在房间主播头像中
	void FreshRoomActor(); //刷新房间主播列表数据
	

private:
	std::set<ActorInfo*,compare_actor> m_ActorSet; //房间主播set列表	
	bool m_IsInInit; //是否有过调用InInit初始化主播进房列表
	int m_curSkinPage;  //当前显示的页数(一页最多5个主播头像)
	int m_maxSkinPage; //m_vecRoomActor最多可显示页数
	bool m_RoomActorIsDeploy; //当前房间主播是否为展开  true为展开  false为收缩
	CRoomActorMenuDlg *pActorMenuDlg;
	CPoint pointOld ;

	
	CRoomParentDlg* m_pParentDlg ;
	CComPtr<ISkinEngine> m_pRoomParentSkinEngine;
	

};