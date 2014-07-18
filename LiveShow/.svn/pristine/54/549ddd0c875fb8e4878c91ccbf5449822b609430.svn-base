#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"
#include "usermanager\UserManager.h"
#include "RoomBusiness.h"
#include "gift\PropsListCtrl.h"
#include "gift\GiftComboBox.h"
#include "iectrl\OutputHtmlEditImpl.h"
#include "loudspeaker\SpeakerOutputLogic.h"
#include "gift\GiftTipDlg.h"
#include "gift\MyGoodsListCtrl.h"
#include "flash\flashWnd.h"
#include "screeneffect\ScreenEffectManager.h"
#include "..\CommonLibrary\include\ui\GifButton.h"

// CRoomRightDlg 对话框

class CRoomParentDlg ;
class CPresentDetailDlg ;
class CRoomRightDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomRightDlg)

public:
	CRoomRightDlg(CRoomParentDlg* pParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomRightDlg();

public:
	//用户相关
	void OnAddUser(IUserItem * pUserItem);
	void OnDeleteUser(IUserItem * pUserItem);

	//道具商城相关
	void OnUpdateMoney( uint32 unMoney , BOOL bFirstInit = FALSE )	;										//更新钱币
	bool GetSendToUIN(uint32 & unUIN);											//获取赠送用户UIN
	void ReloadGiftData();

	//喇叭相关
	void CleanSpeakerOutput();

	//业务逻辑
	void SetRoomBusiness(RoomBusiness * pRoomBusiness);

// 对话框数据
	enum { IDD = IDD_ROOM_RIGHT_DLG };
	void SetSendObj( CString strName , uint32 unUIN ) ;
	void SetSpeakerOutputGotoEndAble( BOOL bAble ) ;
	void InsertSpeakerText(CString& strText ) ;
	void OnClickNameLink( CString& strName , uint32 unUUID ) ;
	void OnPaodaoSendToChange( CString& strSelText ) ;
	int GetSenToComboInfo(vector<SenToComboInfo>& vSenToComboInfoList ) ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	//afx_msg LRESULT OnClickGifButton( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);

	//道具商城相关
	void InitGiftListCtrl();													//初始化礼物列表
	void ShowGiftListCtrlByTab(int nTab);										//根据Tab ID显示相应的礼物页
	//void OnClickBtnGiftListDown();                 								//点击向下箭头，礼物列表收缩
	//void OnClickBtnGiftListUp();                   								//点击向上箭头，礼物列表展开
	void OnClickTabGift(int nTab);                         						//点击礼物列表tab
	//void SetAboutGiftVisible(BOOL visible);        								//礼物列表展开或收缩时设置一些控件可见与不可见
	void OnClickPresentDetail();                   								//点击赠收详情
	void OnClickStamp();														//点击弹出印章框
	void OnClickChangeVoice();													//点击弹出变声卡选择框
	void OnClickFlyingText();													//点击弹出飞屏输入框
	void OnClickBtnPayVIP();                                                    //点击充值
	void OnClickBtnGift();														//点击礼物
	void OnClickBtnGame();														//点击游戏

	//包裹系统
	void OnClickBtnMyGoods();													//点击包裹

	//喇叭相关
	void OnClickSpeaker();                         								//点击喇叭
	void OnClickWishSong();														//点击浪漫点歌

	void PlayCoinEffect(uint32 totalMoney, uint32 addMoney);                    //播放加币效果,addMoney大于0时有加币效果，无效果则传<=0的值
	//充值按钮
	//void DrawGifButton();
	//void MoveGifButton(BOOL bShow);

	void InitCoinFlashWnd();

	void OnClickBtnBaiTuService(); //点击联系客服

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnClickUserList(WPARAM wParam, LPARAM lParam);				//点击用户列表
	afx_msg LRESULT OnClickGiftList(WPARAM wParam, LPARAM lParam);				//点击道具列表
	afx_msg LRESULT OnShowGiftTip(WPARAM wParam, LPARAM lParam);				//显示礼物信息卡片
	afx_msg LRESULT OnClickMyGoodsList(WPARAM wParam, LPARAM lParam);			//点击包裹物品

private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	RoomBusiness * m_pRoomBusiness;												//房间业务逻辑
	CRoomParentDlg * m_pParentDlg;												//房间主窗口
	
	//道具商城
	std::map<std::wstring, CGiftListCtrl*> m_mapGiftListCtrl;					//礼物列表控件
	CGiftComboBox m_GiftCountCombo;												//礼物数量控件
	CComboBox m_GiftSendToCombo;												//礼物赠送对象
	CPresentDetailDlg * m_pPresentDetailDlg;									//礼物赠收详情
	CGiftTipDlg * m_pGiftTipDlg;												//礼物信息卡片

	//包裹
	CMyGoodsListCtrl m_MyGoodsListCtrl;											//包裹控件

	//喇叭展现
	COutputHtmlEditImpl* m_pSpeakerHtml;										//喇叭输出控件
	CSpeakerOutputLogic m_SpeakerLogic;											//喇叭输出逻辑

	int m_nGiftTypeCurSel;                                                      //当前礼物类型

	int m_nPayImageIndex;                                                       //充值按钮动画帧
	CFlashWnd * m_pCoinFlashWnd;                                                //179币flash
	uint32 m_unLastMoney ;

	//common::ui::GifButton *m_pGifBtnPayMoney;
public:
	UserManager * GetUserManager();												//获取用户管理
	IUserItem * GetSelfUserItem();												//获取自己信息
	afx_msg void OnCbnSelchangeComboRoomGiveto();
	void ShowCoinFlashWnd();

	void GetGiftItemRect(CRect &rcGiftItem);
	//void MoveThePayMoneyBtn(BOOL bShow);
};
