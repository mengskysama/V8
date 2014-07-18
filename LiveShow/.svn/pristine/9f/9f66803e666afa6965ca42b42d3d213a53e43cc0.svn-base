#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "usermanager\iusermanagersink.h"
#include "..\..\include\ui\mlhtmlview.h"
#include "RoomBusiness.h"
#include "avlogic\RoomAVLogic.h"
#include "UserMessageDefine.h"
#include "screeneffect\ScreenEffectManager.h"
#include "..\CommonLibrary\include\ui\TickerStatic.h"
#include "afxwin.h"
#include "flash/flashWnd.h"
#include "statreport\StatReport.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"
#include "..\CommonLibrary\include\ui\GifButton.h"
#include "..\CommonLibrary\include\ui\PngButton.h"
#include "..\CommonLibrary\include\ui\FlashFrameWnd.h"
#include "guide\GuideFlash.h"
#include "fans\AnchorFansCardDlg.h"
#include "sofa\SofaManager.h"
#include "iectrl\OutputHtmlEditImpl.h"
#include "loudspeaker\SpeakerOutputLogic.h"
#include "loudspeaker\LoudSpeakerManager.h"

// CRoomParentDlg 对话框

//DragonZ:CRoomLeftDlg抽象为CRoomTopManager
//class CRoomLeftDlg;

class CRoomMidDlg;
class CRoomRightDlg;
class CStampDlg ;
class CEmotionDlg ;
class CPngDlg ;
class CPrivateMicDlg ;
class CP2PChatDlg ;
class CScrollBtnDlg ;
class CSpeakerDlg ;
class CWishSpeakerDlg ;
class CKickDlg ;
class CPaodaoBtnDlg ;
class CChangeVoiceDlg;
class CFlyingTextInputDlg;
class CUserInfoCardDlg ;
class CSysSettingDlg ;
class COtherUserInfoDlg ;
class CShowAudioTestDlg;
class CGuestRegisterDlg;
class CCoverWnd ;
class CGiftWordsDlg ;
class CModifyNameTipWnd;
//class CTaskDlg;
class CEnterRoomGuide;
class COrderSongDlg;
class CRoomBottomLeftDlg;
class CPublicMicDlg;
class CRoomTopManager;
class CPaodaoStaitc;
class CIconIntroductionDlg;
class CPayDlg;
class CRoomParentDlg : public CDialog, public IUserManagerSink, public IRoomBusinessSink, public ISofaManagerSink
{
	DECLARE_DYNAMIC(CRoomParentDlg)

public:
	CRoomParentDlg(uint32 uRoomID, RoomOption * pRoomOption, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomParentDlg();

public:
	//业务逻辑
	void SetRoomBusiness(RoomBusiness * pRoomBusiness);
	RoomBusiness * GetRoomBusiness();
	HtmlOutPutManager* GetHtmlOutPutManager(){ return m_pHtmlOutPutManager; } ;

	//用户列表通知消息
	virtual void OnGetUserList(std::map<uint32, IUserItem*> & mapAllUser);													
	virtual void OnGetAdminList(std::map<uint32, IUserItem*> & mapAdminUser);
	virtual void OnGetWaitingList(std::map<uint32, IUserItem*> & mapWaitingUser);
	virtual void OnAddUser(IUserItem * pUserItem );
	virtual void OnUpdateUser(IUserItem * pUserItem);
	virtual void OnDeleteUser(IUserItem * pUserItem);
	virtual void OnAddAdminUser(IUserItem * pUserItem);
	virtual void OnDeleteAdminUser(IUserItem * pUserItem);
	virtual void OnAddWaitingUser(IUserItem * pUserItem);
	virtual void OnDeleteWaitingUser(IUserItem * pUserItem);
	virtual void OnUserListCountChange();
	void OnGetUserListBegin();
	void OnGetUserListComplete();

	//房间业务通知消息
	virtual void OnEnterRoom(IUserItem * pUserItem);
	virtual void OnEnterRoomGetUserList_LuckyStar(IUserItem* pUserItem ) ;			//幸运星
	virtual void OnEnterRoomGetUserList_VIPLevelTop( IUserItem* pUserItem ) ;		//至尊皇冠进场
	virtual void OnEnterRoomGetUserList_VIPLevelBlackDiamond(IUserItem * pUserItem);//黑钻皇冠进场
	virtual void OnEnterRoomGetUserList_VIPLevelDiamond(IUserItem* pUserItem);		//钻石皇冠进场  
	virtual void OnEnterRoomGetUserList_Blink(IUserItem* pUserItem ) ;				//闪亮登场
	virtual void OnEnterRoomGetUserList_GiftStar( IUserItem* pUserItem ) ;			//礼物之星
	virtual void OnEnterRoomGetUserList_Crest(IUserItem * pUserItem);				//冠用户进场
	virtual void OnEnterRoomGetUserList_Event(IUserItem * pUserItem, uint8 unIndex);//特殊活动进场 
	virtual void OnKickUserRsp(uint32 unDstUIN);
	virtual void OnKickUserNotify(uint32 unSrcUIN, uint32 unDstUIN);
	virtual void OnAddBlackRsp(uint32 unDstUIN);
	virtual void OnAddBlackNotify(uint32 unSrcUIN, uint32 unDstUIN);
	virtual void OnLockIpRsp(uint32 unDstUIN);
	virtual void OnLockIpNotify(uint32 unSrcUIN, uint32 unDstUIN);

	//沙发通知消息
	virtual void OnBuySofaNotify(CSofaItem * pSofaItem);
	virtual void OnSofaInfoUpdate(std::vector<CSofaItem*> vecSofaItem);

// 对话框数据
	enum { IDD = IDD_ROOM_PARENT_DLG };

public:
	//子窗口消息相应
	afx_msg LRESULT OnClickStamp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickSpeaker(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickWishSong(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickUserList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDBClickUserList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickStampList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickChangeVoice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickChangeVoiceList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickFlyingText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickSofaName(WPARAM wParam, LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose() ;
	afx_msg void OnTimer(UINT_PTR nIDEvent) ;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) ;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnPostPaint(WPARAM wParam, LPARAM lParam);//来自皮肤消息
	afx_msg LRESULT OnModifyNameClickClose( WPARAM wParam ,LPARAM lParam ) ;
	//afx_msg LRESULT OnBtnTaskClick( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnBtnFansHover( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnBtnFansLeave( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nHitTest, CPoint point);
	
	void NotifyCreateSkinItem();

	void OnClickBtnCollectRoom();          //点击收藏房间

	//void DrawGifButton();
	//void MoveGifButton(BOOL bShow);

	void ShowFans(int nIndex);  //显示某麦上对应主播粉丝排行榜
	void MoveBtnFans(BOOL bShow); 
	void MoveBtnUnDealSong(BOOL bShow);//移动未处理歌曲数目的位置
	void SetUnDealSongBtn(int nIndex, int songCount);//设置某麦上未处理歌曲数目

	DECLARE_MESSAGE_MAP()
public:
	void OnClickStamp() ;
	void OnClickEmotion( CWnd* pParentWnd ) ;
	void SetNetState() ;//设置自己的网路状态
	RoomAVLogic* GetRoomAvLogic(){ return m_pRoomAvLogic ;} ;

	void OnClickHerfLink( CString& strName , CString& strUUID , CString& str179ID , int nType , CString strIndex = L"") ;
	void OnClickStampLink( CString& strName , CString& strUUID ) ;						//输入框，点击了盖章
	void OnClickNameLink( CString& strName , CString& strUUID ) ;						//输入、输出聊天框，点击了 名字
	void OnClickPrivateLink( CString& strName , CString& strUUID , CString& str179ID ) ;					//点击查看私麦链接
	void OnClickP2PAccept( CString& strName ,CString& strUUID ) ;						//同意一对一私聊
	void OnClickP2PRefuse( CString& strName ,CString& strUUID ) ;						//拒绝一对一私聊
	void OnClickTempNotice( CString& strTempNotice , CString& strUUID ) ;				//转发到临时公告
	void OnClickVoiceLink( CString& strName , CString& strUUID ) ;						//点击选择变声卡
	void OnClickNoRemind(CString& strUUID);
	void OnClickPublicAccept(CString& strName, CString& strUUID, CString& strIndex);
	void OnClickPublicRefuse(CString& strName, CString& strUUID, CString& strIndex);
	void OnClickHrefMark();
	void OnOutPutHtmlMouseOver( CRect& CWindowRect , ENM_HTML_TYPE nHtmlType ) ;					//bSpeaker为ture时为喇叭区
	void OnOutPutHtmlMouseOut( CRect& CWindowRect , ENM_HTML_TYPE nHtmlType ) ;
	void OnSendMsg() ;
	void OnSendAutoMsg(uint32 unDstUin);
	//void OnClickBtnStatus();//点击状态
	//void OnClickBtnAudioSet();//点击音频箭头
	void SetMyStatus(BOOL bOnLine);
	//afx_msg LRESULT OnClickBtnAutoReplyMsg(WPARAM wParam, LPARAM lParam);//点击自动回复某项菜单 
	afx_msg void OnMenuOnline();
	//afx_msg LRESULT OnClickMenuAudioSet(WPARAM wParam, LPARAM lParam);//点击音频菜单 

	void OnClickSpeakerShortCut(std::wstring strName, bool bBigSpeaker = false) ;
	void OnClickWishShortCut(std::wstring strName) ;
	void OnClickRoomShortCut(uint32 unRoomID) ;
	void OnClickGameReward(std::wstring strUrl);

	void CreateSpeakerDlg(CString strInitContent, core::ENM_LoudSpeakerType enmType) ;
	void CreateWishSpeakerDlg(CString strInitContent) ;

	void SendOpenPrivate( uint32 unDstUin ) ;
	void SendRequestP2PMic( uint32 unDstUin ) ;
	void OnResponseP2PRspAccept( uint32 dstuin ) ;
	void OnSetMicStatus( int16 nIndex , int16 nStatus ) ;
	void CleanAVLogic() ;
	void DrawBtnFans(int nIndex); //绘制某麦上粉丝按扭
	void DeleteFansBtn(uint32 unSrc); //删除某麦上粉丝按扭	
	void DrawBtnUnDealSong(int nIndex, int songCount);//绘制未处理歌曲数目，设置数目调用这个
	void DeleteUnDealSongBtn(uint32 unSrc);//删除某麦上未处理歌曲按钮
public:
	IUserItem* m_pSelectUserItem ;
	uint32 GetRoomID() { return m_uRoomID; }
	RoomOption * GetRoomOption() { return m_pRoomOption; }
	void FangMai( bool bTips ) ; //放麦
	void ShowKickDlg(uint8 unType) ;
	bool InitAVLogic( uint32 unRoomID ,uint32 uRelayIP ,uint32 uRelayPort , uint32 iKey , RoomModule& Module ) ;
	void RestRelayServer( uint32 uRelayIP ,uint32 uRelayPort ) ;
	void SetRoomMicVolume( uint32 unMicVolume ) ;
	void CloseMic() ;
	void OpenMic() ;
	void SetAllVideoCfg( int iDevice ) ;
	void OnUpdateMoney( uint32 unMoney , BOOL bFirstInit = FALSE )	;		//更新钱币
	void InitPublicMic( std::vector<room_user_info>& vecPubMic ) ;  //进房时公麦列表
	void OnUserInfo( IUserItem* pUserItem ) ;			//用户信息
	void OnOtherUserInfo(uint32 unUIN,bool isMan,bool IsActor = false);  //其他用户信息
	void OnAgentInfo(IUserItem* pUserItem);        //销售信息
	void OnHallHeadImageUpdate(uint32 unUIN) ;
	void OnGetFansRsp(Event const&evt); //获取粉丝排行数据返回函数

	void AddGiftInfo( uint32 srcuin , uint32 dstuin , uint32 unCount , 
		uint16 unGiftid, uint16 unType, uint32 time )  ;

	void CreateLuckyInfoList( uint32 srcuin ,uint32 dstuin ,uint32 unCount ,uint16 unGiftid ,
		std::vector<SendGiftLuckyInfo>& vecLuckyInfo, uint32 time ) ;
	
	void OnNotifyPlutusAward( uint32 unSrcUIN , uint32 un179ID ,std::wstring& strDecodeName ,
		std::wstring strRoomName ,std::vector<uint16>& vecMoneys , uint32 unGiftID ) ;

	int GetSenToComboInfo(vector<SenToComboInfo>& vSenToComboInfoList ) ;
	uint32 GetGiftBaseListCount( uint32 unDstuin ,uint32 unGiftid ) ;
	void AdjustGiftBaseListCountAndGiftInfoList( uint32 unDstuin ,uint32 unGiftid ) ;
	void OnLuckyMessage(uint32 unSrc179ID, uint16 unLuckyTimes, std::wstring strUserName, std::wstring strRoomName);
	void OnClickNeedGiftWord( IGiftItem * pGiftItem , IUserItem *pUserItem , uint16 unCount ) ;
public:
	void OnCycleTrigger();
	uint32 GetPulicMicUINByIndex( int nIndex ) ;
	void SetSendObj( CString strName , uint32 unUIN ) ;
	void SetChatObj( CString strName , uint32 unUIN ) ;
	void BackMessage( UINT32 dstuin , MessageFont& stFont , std::wstring strText , ENM_SendMessageType enmType , bool bAutoMsg = FALSE) ;
	bool GetAMessageBack( SendMessageEvent& message ) ;
	void SetPublicOutputGotoEndAble( BOOL bAble ) ;
	void SetPrivateOutputGotoEndAble( BOOL bAble ) ;
	void SetSpeakerOutputGotoEndAble( BOOL bAble ) ;
	void CleanPublicOutput();
	void CleanPrivateOutput();
	void CleanSpeakerOutput();

	void OnRoomRightSendToChange( CString& strSelText ) ;
	void OnPaodaoSendToChange( CString& strSelText ) ;

	void InsertPaoDaoText(CString strSrcName , CString strDstName , CString strGiftWord ,uint32 unSrc179ID , 
	uint32 unDst179ID ,  uint16 unGiftID ,uint16 unCount , CString& strTime ) ;
	void InsertPrivateText(CString& strText ) ; //插入私屏且窗口最小化时闪烁窗口
	void InsertPublicText(CString& strText ) ;
	void InsertSpeakerText(CString& strText ) ;
	void InsertSystemSpeaker(CString& strText ) ;
	void ClearSpeakerMsg();
	void FlashRoomWindow() ;
	void InsertRoomMidInputDefaultText(CString& strText);

	void OnRequestShow_NeedSelect() ;
	void OnRequestShow_Failed() ;
	void OnRequestShow_Public( uint8 unIndex , uint32 unTime ) ;//自己排上公麦
	void OnRequestShow_WaitingList() ;
	void OnRequestShow_PublicNotify( uint32 unSrcUIN , uint8 unIndex , uint16 unTime ) ;//别人排上公麦
	void OnRequestShow_Private() ;
	void OnExitShowNotify( uint32 srcuin , int nType ) ;
	void OnOpenPrivate( uint32 unDstUIN ) ;
	void OnPushOnShow_Success( uint32 unDstUIN , uint8 unIndex ) ;//邀请上麦成功
	void OnPullOffShowRsp_Success( uint32 unDstUIN ) ;
	void OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN ) ;
	void OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) ;
	void OnRespnseP2PNotifyAccept( uint32 unSrcUIN ) ;  //一对一私聊
	void OnNotifyPrivateShow( std::vector<uint32>& vecUIN) ;

	void OnSignature(   ) ;
	void OutPutNoticeMsg( ) ; //进房间时的房间公告
	void OnWelecomeWords( ) ;
	void OnGuestRegister();
	BOOL IsCoverWndVisible() ;
	BOOL IsGuideWndVisible();

	void ClosedRegisterDlg( ) ;
	void OnClickRegister();

	void OnSetRoomInfo( uint32 unSrcUIN ) ;

	int GetFlyTextEffect();
	void SetFlyTextEffect(int nType);
	//void UpdateAudioMode();
	ScreenEffectManager * GetEffectManager();
	//DragonZ:
	//CRoomLeftDlg* GetRoomLeftDlg(){ return m_pRoomLeftDlg ;} ;
	CRoomTopManager* GetRoomTopManager(){ return m_pRoomTopManager; };
	CRoomMidDlg* GetRoomMidDlg(){return m_pRoomMidDlg ;};
	CRoomRightDlg* GetRoomRightDlg(){return m_pRoomRightDlg ;};

	CLoudSpeakerManager * GetLoudSpeakerManager() { return m_pLoudSpeakerManager; }

	long GetPlutusAwardLeftMoney() ;	//获取财神降临时未加的剩余金币
	long GetLuckLeftMoney() ;
	void AddBottomUserInfo();
	CStatReport& GetStatReportInstance()
	{
		return m_ipStatReport;
	}

	void ShowGuide();
	
	void ReloadGiftData();
	
	void ShowEnterRoomGuide();
	void InitSofaList();
	void UpdateSofa(uint32 unIndex);
	void OnSofaChat(uint32 unSrcUIN, uint32 unDstUIN, CString strText, bool bPublic);
	void ClearSofaList();

	void OnClickOrderSong(uint32 unIndex = 0);
	void ShowOrderSongDlg();
	void UpdateSongList(uint32 unIndex, uint32 unSongerID, bool bRefresh = false, bool bSort = true);
	void UpdateOrderList(uint32 unSelfID, bool bActor);
	void SendGetSongList();
	void OnOrderSongRsp(uint32 unSongerID, bool bSuccess);
	void UpdateUndealCount();
	void OnSongTakeOrderRsp(uint32 unResult);
	void UpdateMarkList(uint32 unSelfID);
	void OnInitRoomActor();
private:
	afx_msg LRESULT OnPrivateMicDlgClose( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnP2PMicDlgClose( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnIpStatReport( WPARAM wParam, LPARAM lParam );  //获取本地IP地址响应函数
	void PlutusAward( ) ;

	//返回是否可以追加输出一次
	bool OutPutGift() ;

	void CreatePrivateMic(CString strSrcName , uint32 srcuin ,uint32 un179ID , uint16 nUserStatus ) ;
	void CreateP2PMic( uint32 UIN ) ;
	void CloseMicActive( IUserItem* pMyUserItem ) ;
	void CloseMicPassive( IUserItem* pSrcUserItem  ) ; 
	void CloseP2PPassive( IUserItem* pSrcUserItem ) ;

	void ClosePublicMic(uint32 UIN ) ;
	BOOL ClosePrivateMic(uint32 UIN ) ;
	void CloseP2PMic( uint32 UIN ) ;
	
	void SetRoomName() ;
	void InitMicSlider() ;

	//void OnClickBtnCloseSound() ;				//关闭音量
	//void OnClickBtnOpenSound() ;

	//void ShowAudioTest();

	void MoveTicker();  //跑马灯位置调整
	void OnMyHeadImgUpdate();

	void MoveModifyNameTipWnd();  //修改昵称位置调整
	//void SetRichLevel(IUserItem * pUserItem);
	//void SetRichLevelTip();

	//void MoveTaskDlg();
	//void MoveBtnTask(BOOL bShow);
	//void DrawBtnTask();

	void UpdateSongMarkDlg(int nIndex = 0);			//主播上下麦切换评价区界面
	uint32 CheckActorOnMic();						//麦上是否有主播

//	void DrawChatWithMeTip();
//	void DrawFlashFrame();
//	void MoveFlashFrame();
//	void MoveChatWithTip();
	void MovePaoDao();
	void OnClickBtnIconIntro();

	void OnActorSpeak();
	void OnAgentSpeak();
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
//	CVDCComboBox m_comboAudioMode;

	common::ui::CTickerStatic m_RoomTitleTicker;
    //DragonZ:
	//CRoomLeftDlg *  m_pRoomLeftDlg;			//房间左子对话框
	CRoomTopManager* m_pRoomTopManager;     //房间上半部分逻辑管理类

	CRoomMidDlg *   m_pRoomMidDlg;			//房间中子对话框
	CRoomRightDlg * m_pRoomRightDlg;		//房间右子对话框
	CRoomBottomLeftDlg* m_pRoomBottomLeftDlg;//房间底部左子对话框

	CStampDlg * m_pStampDlg ;				//盖章对话框
	CEmotionDlg *m_pEmotionDlg ;			//表情对话框
	//CPngDlg* m_pPngDlg ;					//画图对话框
	CChangeVoiceDlg * m_pChangeVoiceDlg;	//变声卡对话框
	CFlyingTextInputDlg * m_pFlyingTextInputDlg;	//飞屏输入对话框

	//CShowAudioTestDlg *m_pAudioTest;		//音频测试对话框

	RoomBusiness * m_pRoomBusiness;
	HtmlOutPutManager* m_pHtmlOutPutManager ;

	map<uint32 , CPrivateMicDlg*> m_mPrivateMicDlg ;  // <UUID ,CPrivateMicDlg*>  私麦
	CP2PChatDlg* m_pP2PChatDlg ;						  //一对一私聊窗口
	CScrollBtnDlg *m_pScrollBtnDlg;
	RoomAVLogic *m_pRoomAvLogic ;
	RoomOption * m_pRoomOption ;						//房间属性和RoomBusiness共有，为显示用户列表中的房间名，今后要去掉

	CSpeakerDlg * m_pSpeakerDlg ;
	CWishSpeakerDlg * m_pWishSpeakDlg ;
	CKickDlg * m_pKickDlg ;

	CUserInfoCardDlg* m_pUserInfoCardDlg ;
	CSysSettingDlg* m_pUserInfoDlg ;  //个人信息
	COtherUserInfoDlg* m_pOtherUserInfoDlg ; //他人信息
	CAnchorFansCardDlg* m_pAnchorFansCardDlg; //粉丝排行卡片
	std::map<int, common::ui::PngButton *> m_pMapFansBtn;//麦上粉丝按扭
	std::map<int, common::ui::PngButton *> m_pMapUnDealSongBtn;//未处理歌曲按钮
	
	uint32 m_uRoomID;
	uint32 m_nOutGiftIndex ;
	std::list<GiftInfo> m_vGiftInfoList ;
	std::map<uint32, std::list<LuckyInfo>> m_vLuckyInfoList ;
	std::vector<PlutusAwardInfo> m_vPlutusAwardMoneys ;		//财神送钱信息
	ScreenEffectManager * m_pEffectManager;
	map<uint32 ,map<uint32 ,RecvGiftCountInfo > > m_mGiftBaseList ;	

	//CSliderCtrl m_MicSliderCtrl;
	//CSliderCtrl m_SpeakerSliderCtrl;

	CCoverWnd* m_pCoverWnd ;
	CStatReport m_ipStatReport;
	CGiftWordsDlg* m_pGiftWordsDlg ;

	//修改昵称提示框
	CModifyNameTipWnd* m_pModifyNameTipWnd;

	//common::ui::GifButton *m_pRichLevelBtn;
	bool m_bShowRichLevel;
	
	//CTaskDlg* m_pTaskDlg;
	//common::ui::PngButton *m_pBtnTask;

	CGuideFlash * m_pGuideFlash;

	CEnterRoomGuide * m_pEnterRoomGuideDlg;
	//窗口闪闪
	//common::ui::FlashFrameWnd * m_pFlashFrameWnd;
	//CModifyNameTipWnd* m_pChatWithMeTip;
	
	//点歌
	COrderSongDlg* m_pOrderSongDlg;

	map<int , CPublicMicDlg*> m_mPublicMicDlg ;
	CPaodaoStaitc *m_pPaodaoStatic ;
	CIconIntroductionDlg* m_pIconIntroducDlg;

	COutputHtmlEditImpl* m_pSpeakerHtml;										//喇叭输出控件
	CSpeakerOutputLogic m_SpeakerLogic;											//喇叭输出逻辑
	CLoudSpeakerManager * m_pLoudSpeakerManager;								//喇叭管理逻辑

	CPayDlg* m_pPayDlg;

public:
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnCbnSelchangeComboRoomBottomAudiomode();	
	afx_msg void OnMove(int x, int y);

	void GetFleshGuidRect(CString& strRect);
	//更新任务系统
	//DragonZ:V3.0去除房间右下角的任务
	//void UpdateTaskInfo();
	//void TaskToRegister();//由任务打开的注册
	//void TaskToClose();//点击任务右上角隐藏
	
	//CSelfInfoAndSetDlg里有调用
	void ShowSelfUserInfo();
	CComPtr<ISkinEngine> GetSkinEngine(){ return m_pSkinEngine; };
	void OnClickNameLink( CString& strName , uint32 unUUID );

	void SetCollectRoom(BOOL bCollected);//设置房间是否收藏
	void OnClickRoomPay(CString strUrl); //点击房间里的充值 
	void OnClickGotoRegister();//点击提示框的快去注册吧
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
