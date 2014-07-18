#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "..\usermanager\UserItem.h"
#include "..\..\CommonLibrary\include\ui\GifButton.h"
#include "Roomdefine.h"

// CSelfInfoAndSetDlg 对话框
class CRoomParentDlg;
class CRoomManagerDlg;
class CSysSettingDlg;
class CShowAudioTestDlg;
class CVeritySliderDlg;
class CSelfInfoAndSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfInfoAndSetDlg)

public:
	CSelfInfoAndSetDlg(CRoomParentDlg* pRoomParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelfInfoAndSetDlg();
	

// 对话框数据
	enum { IDD = IDD_ROOMBOTTOMLEFT_SELFINFO_CTRL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnClickBtnAutoReplyMsg(WPARAM wParam, LPARAM lParam);//点击自动回复某项菜单 
	afx_msg LRESULT OnClickMenuAudioSet(WPARAM wParam, LPARAM lParam);//点击音频菜单 
	afx_msg LRESULT OnVScrollSlider( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);

	void CreateSysSettingDlg();

	void OnClickHead();
	void OnClickUserLevel();
	void OnClickUserRole();
	void OnClickSysSetting();
	void OnClickRoomManager();
	void OnClickOnOffLine();
	void OnClickBtnAudioSet();

	void SetUserHead(IUserItem* pUserItem);    //设置头像
	void SetUserLevel(IUserItem* pUserItem);   //设置VIP等级
	void SetUserRole(IUserItem* pUserItem);    //设置管理等级
	void SetUserName(IUserItem* pUserItem);    //设置昵称
	//富豪等级相关
	void SetRichLevel(IUserItem* pUserItem);   //设置富豪等级
	void DrawBtnRichLevel(); 
	
	void OnMenuOnline();
	void ShowAudioTest();

	void InitSlider(); 

	DECLARE_MESSAGE_MAP()
public:
	void SetBottomUserInfo(IUserItem* pUserItem);
	void MoveBtnRichLevel(BOOL bShow);
	void ShowSysSetting(SysSettingDlgType enmType);
	void OnAddUser(IUserItem * pUserItem);
	void OnUpdateUser(IUserItem * pUserItem);
	void SetMyStatus(BOOL bOnLine);
	void CloseMic();
	void OpenMic();
	void SetRichLevelTip();                    //设置富豪等级tip
	void OnMyHeadUpdate();                     //头像更换

	void OnClickBtnCloseSound();
	void OnClickBtnOpenSound();

	void SetRoomMicVolume(uint32 unMicVolume);
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pRoomParentDlg;
	common::ui::GifButton *m_pRichLevelBtn;
	CRoomManagerDlg* m_pRoomManagerDlg;
	CSysSettingDlg* m_pSysSettingDlg;
	CShowAudioTestDlg *m_pAudioTest;		//音频测试对话框
	CVeritySliderDlg * m_pMicSliderDlg;
	CVeritySliderDlg * m_pSpeakerSliderDlg;
};
