#pragma once

#include "..\resource.h"
#include "Skin_i.h"
#include <map>
#include "SysSettingIniKey.h"

// CSysSettingDlg 对话框

#define WM_NOTIFY_ROOM_SYSSET_CLICKOK WM_USER + 210       //点击了"确定"按扭时的消息
#define WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI WM_USER + 211    //点击了"应用"按扭时的消息
#define WM_NOTIFY_ROOM_SYSSET_MODIFY WM_USER + 212		  //设置对话框有修改时的消息
#define WM_NOTIFY_ROOM_SYSSET_MODIFY_OK WM_USER + 213     //关闭对话框

#define MAPOFDLG map<UINT, CDialog*>

//enum SysSettingDlgType
//{
//	Show_User_Info_Dlg      =  0,  //基本资料	
//	Show_Basic_Setting_Dlg      ,  //常规设置
//	Show_Audio_Setting_Dlg      ,  //视频设置
//	Show_PwdSafe_Setting_Dlg    ,  //密码安全
//	Show_MgrRoom_Setting_Dlg    ,  //我的房间
//	Show_My_Account_Dlg         ,  //我的账户
//	Show_Sound_Setting_Dlg      ,  //音频设置
//	Show_Message_Setting_Dlg    ,  //消息设置
//};


class CSysSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysSettingDlg)

public:
	CSysSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysSettingDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SYS_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	void OnTimer(UINT_PTR nIDEvent);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);


    void OnClickTabPersonalSetting(int iTab);
	void OnClickTabSystemSetting(int iTab);

	void OnClickBtnChangeHead();//更换头像
	void OnClickClocse(); //点击取消
    void OnClickBtnApply();//点击应用
	void OnClickBtnOK(); //点击确定
	void SetApplyEffect(BOOL bEffect);

	void NotifyCreateSkinItem();

	CDialog* CreateShowPersonalDlg();  //创建或获取个人资料	
	CDialog* CreateShowBasicSettingDlg(); //创建或获取基本设置
	CDialog* CreateShowAudioSettingDlg(); //创建或获取视频设置	
	CDialog* CreateShowPwdSafeDlg();  //创建或获取密码安全
	CDialog* CreateShowMyRoomDlg();  //创建或获取我的房间
	CDialog* CreateShowBankAccountDlg();  //创建或获取银行账号
	CDialog* CreateShowMyAccountDlg();//创建或获取我的账号
	CDialog* CreateShowFansDlg(); //创建或获取粉丝排行
	CDialog* CreateShowMessageSettingDlg();//创建或获取消息设置
	CDialog* CreateShowSoundSettingDlg();//创建或获取音频设置


	bool PwdSafeDlgIsShow(CDialog *&pwdSafeDlg);  //当前显示对话框是否为密码安全
	

	DECLARE_MESSAGE_MAP()
public:
	void OpenDlg(SysSettingDlgType type);
	void SetToPersonalSetting();
	void SetToSysSetting();
	void OnGetHeadImage(uint32 unUIN);
	void BtnApplyEnable(bool isModify);  //应用按扭是否可用 ( 设置对话框有修改时按扭可用)
	void OnGetFansRsp(Event const&evt);  //获取粉丝数据返回响应函数	

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	MAPOFDLG m_mShowDlg;
	CDialog * curShowDlg;
	CString m_strCurItemBk;
	int m_nCurPersonalItem;
	int m_nCurSysItem;
	bool m_isClose;  //是否关闭设置对话框 


	
};





