#pragma once

#include "GlobalRoomDefine.h"
#include "..\CommonLibrary\include\ui\MenuEx.h"
#include "coredefine.h"
#include "usermanager\UserItem.h"
#include "micwindow\MicToolBarDlg.h"
#include "iectrl\OutputHtmlEditImpl.h"
#include "loudspeaker\SpeakerOutputLogic.h"
#include "roomactor\RoomActor.h"


class CPublicMicDlg;
class CRoomParentDlg ;
class CRoomTopManager
{
public:
	CRoomTopManager(CRoomParentDlg* pParentDlg);
	~CRoomTopManager(void);

public:
	void InitPublicMic( std::vector<core::room_user_info>& vecPubMic ) ;  //进房时公麦列表	
	void ClosePublicMic(uint32 UIN ) ;
	void DisableBtnPaiMai() ;
	void EnablePaiMai() ;
	void SetPaiMaiBtnStatus( ENM_PaiMaiBtn_Status nStatus ) ;
	void CloseMic() ;
	void OpenMic() ;
	uint32 GetPulicMicUINByIndex( int nIndex ) ;

	void OnRequestShow_NeedSelect() ;
	void OnRequestShow_Failed() ;
	void OnRequestShow_Public(uint8 unIndex , uint32 unTime ) ;
	void OnRequestShow_WaitingList() ;
	void OnRequestShow_PublicNotify( uint32 unSrcUIN , uint8 unIndex , uint16 unTime ) ;
	void OnRequestShow_PublicNotifySelf() ;
	void OnRequestShow_Private() ;
	void OnExitShowNotify( uint32 srcuin ) ;
	void OnPushOnShow_Success( uint32 unDstUIN , uint8 unIndex ) ;
	void OnPullOffShowRsp_Success( IUserItem* pDstUserItem ) ;
	void OnOpVideoAudioRsp( uint16 nIndex ,uint32 nOpAVType ,uint32 unUIN ) ;
	void OnOpVideoAudioNotify( uint16 nIndex , uint32 nOpAVType ,uint32 unUIN ) ;

	void OnSetMicStatus( int16 nIndex , int16 nStatus ) ;

	void SetPublicMicEffect(int nMic, BOOL bOnMic);//设置公麦的状态.参数一表示第几个公麦，参数二为TRUE表示正在主播
	void CleanAVLogic() ;

	void CloseVideo() ;		//关闭公屏上的视频
	void OpenVideo() ;

	void GetPublicMicRect(int micIndex, CRect& rcMic);
	void SetAllPublicMic(map<int, CPublicMicDlg*>& mapPublicMicDlg);
    void OnClickPaiMic();                       //点击排麦或放麦

	void SetSpeakerHtml(COutputHtmlEditImpl * pSpeakerHtml);

	void OnAddUser(IUserItem* pUserItem);  //用户进房
	void OnDeleteUser(IUserItem* pUserItem); //用户退房
	void OnHallHeadImageUpdate(uint32 UIN);  //最新头像图片响应函数
	void ProcessNotifyUserInfoUpdate(Event const& evt);  //其他用户的vip等级、富豪等级改变
	void ProcessMyVipLevelUpdate(); //用户自已的vip等级更新
	void ProcessMyRichLevelUpdate(); //用户自已的富豪等级更新
	void OnMouseMove(UINT nHitTest,CPoint point);	

	//主播头像列表
	void InitRoomActor(); //初始化主播列表	
	void ProcessInitOffRoomActorRsp(Event const& evt);
	void OnClickBtnRoomActorUp();
	void OnClickBtnRoomActorDown();
	void OnClickBtnRoomActorTitle();//点击主播主像列表上标题中的向下或关闭按扭的响应函数
	void OnClickBtnActorHead(int headIndex);//点击主播头像
	void ShowSkinRoomActor();
	void ShowGuide();	
	//主播头像列表
	
	
private:
	CComPtr<ISkinEngine> m_pRoomParentSkinEngine;
	common::ui::CMenuEx m_menuPaiMai ;
	map<int , CPublicMicDlg*> m_mPublicMicDlg ;
	CRoomParentDlg* m_pParentDlg ;
	ENM_PaiMaiBtn_Status m_nPaiMaiBtn_Status ;
	COutputHtmlEditImpl* m_pSpeakerHtml;							//喇叭输出控件
	CRoomActor *m_RoomActor; //房间所有主播
};
