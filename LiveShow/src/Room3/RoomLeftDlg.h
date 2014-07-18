#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"
#include "GlobalRoomDefine.h"
#include "usermanager/UserManager.h"
#include "SysSetting\SysSettingDlg.h"
#include "..\CommonLibrary\include\ui\MenuEx.h"
#include "coredefine.h"

// CRoomLeftDlg 对话框

class CPublicMicDlg;
class CRoomParentDlg ;
class CRoomManagerDlg ;
class CRoomLeftDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomLeftDlg)

public:
	CRoomLeftDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomLeftDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_LEFT_DLG };

public:
	void InitPublicMic( std::vector<core::room_user_info>& vecPubMic ) ;  //进房时公麦列表
	//void OnAddUser( IUserItem * pUserItem ) ;
	//void OnUpdateUser(IUserItem * pUserItem) ;
public:
	void ClosePublicMic(uint32 UIN ) ;
	void DisableBtnPaiMai() ;
	void EnablePaiMai() ;
	void SetPaiMaiBtnStatus( ENM_PaiMaiBtn_Status nStatus ) ;
	//void SetRoomMicVolume( uint32 unMicVolume ) ;
	void CloseMic() ;
	void OpenMic() ;
	//void ShowUserSetting( SysSettingDlgType enmType );
	uint32 GetPulicMicUINByIndex( int nIndex ) ;
	//void OnClickTempNotice( CString& strTempNotice , CString& strUUID ) ;

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
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void NotifyCreateSkinItem();
	void SetPublicMicDis();                     //设置公麦间距

	void OnClickPaiMic();                       //点击排麦或放麦
	//void OnClickUserSetting();                  //点击个人设置
	//void OnClickRoomMgr();				        //点击房间管理

	//void OnClickBtnCloseMic() ;					//关闭麦克风
	//void OnClickBtnOpenMic() ;
	//void OnClickBtnCloseSound() ;				//关闭音量
	//void OnClickBtnOpenSound() ;

	//void CreateSysSettingDlg(); //创建设置对话框

	DECLARE_MESSAGE_MAP()
private:
	//void InitMicSlider() ;
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	common::ui::CMenuEx m_menuPaiMai ;
//	CSliderCtrl m_MicSliderCtrl;
//	CSliderCtrl m_SpeakerSliderCtrl;
	map<int , CPublicMicDlg*> m_mPublicMicDlg ;
	CRoomParentDlg* m_pParentDlg ;
	ENM_PaiMaiBtn_Status m_nPaiMaiBtn_Status ;

	//CRoomManagerDlg* m_pRoomManagerDlg ;
	//CSysSettingDlg* m_pSysSettingDlg;
};
