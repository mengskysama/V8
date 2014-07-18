#pragma once

class CPasswordCtrl;
class CRBPasswordCtrl;
class CVerifyDlg;
class CLoginVerifyHander;
class CHallDlg;
class CQQLoginDlg;
class C179TipCtrl;

#include "../UserInfo.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "imodulemanager.h"
#include "LoginComboBox.h"
#include "TipsButton.h"
#include "RegisterDlg.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"

#define UM_179REGISTER_AUTOLOGIN WM_USER + 201   //179注册成功之后的自动登录消息
#define UM_LOGIN_DLG_SETFOCUS WM_USER + 202
#define UM_QQLOGIN_AUTOLOGIN  WM_USER + 203		 //QQ对应179账号自动登录
#define UM_QQLOGIN_SET		  WM_USER + 204      //设置QQ登录
#define UM_179REGISTER_QQLOGIN  WM_USER + 205    //179注册框中的QQ登录
#define UM_QQLOGIN_MOVEWINDOW  WM_USER + 206     //QQ登录框设置位置
#define WM_AUTO_LOGIN (WM_USER+123)


#define LOGIN_EMPTY_179ACCOUNT_TIP _T("请输入您的白兔KTV账号")
#define LOGIN_PWD_EMPTY_TIP     _T("请输入密码")
#define DELETE_ACCOUNT_TIP _T("是否从列表中删除该账号？")

#define LOGINREP_ERROR_TIP _T("登录失败，连接服务器超时")
#define LOGINREP_PWDERROR_TITLE _T("179安全中心")
#define LOGINREP_PWDERROR_TIP _T("请注意区分大小写，开启小键盘")
#define LOGINREP_PWDERROR_LINKS_TIP _T("找回密码请点击这里")
#define LOGINREP_ACCOUNT_NOEXIST_TIP _T("您输入的账号不存在")
#define LOGINREP_ACCOUNT_LOCKED_TIP _T("您当前的账号已被封停%d天")
#define LOGINREP_ACCOUNT_LOCKED2_TIP _T("将于%s解封")
#define LOGINREP_LOCKED_TIP _T("将于%s解封            ")
#define LOGINREP_ACCOUNT_LOGINED_TIP _T("请勿重复登陆      ")
#define LOGINREP_NET_TYPE_ERROR_TIP _T("获取网络类型失败，请稍候再试")
#define LOGINREP_EMAIL_TIP _T("邮箱未激活，请激活后再登录")
#define LOGINREP_MAINTAN_TIP _T("服务器在维护")
#define LOGINREP_TOKEN_REFUSE _T("抱歉，您的账号登录过于频繁，")	//后面部分在下一个String,汗啊
#define LOGINREP_TOKEN_ERROR _T("对不起，账号认证失败")
#define LOGINREP_TOKEN_LOCKED _T("对不起，您的账号被锁定")
#define LOGINREP_TOKEN_LOCKED2 _T("如有疑问请咨询客服        ")
#define LOGINREP_VERSION_ERROR_TIP _T("抱歉，您当前的版本不可用")
#define LOGINREP_VERSION_LINKS_TIP _T("请去官网下载最新版本")

#define QQ_LOGIN_DLG_WIDTH                  540
#define QQ_LOGIN_DLG_HEIGHT                 340
#define QQ_LOGIN_DLG_BOTTOM_MARGIN          45 //QQ登录框bottom到登录框bottom

enum LoginType
{
	AUTOLOGIN_FIRST =  0,   //首次启动客户端时的自动登录
	AUTOLOGIN_179REGISTER = 1, //注册成功后的自动登录
	AUTOLOGIN_QQLOGIN = 2,   //QQ对应179号自动登录
    CLICK_LOGIN_BUTTON = 3,  //用户点击"登录"按扭进行的登录
	CLICK_GUEST_LOGIN_BUTTON = 4, //用户点击"游客登录"按扭进行的登录
	AUTOLOGIN_FIRST51 = 5,  //51对应179号自动登录

};


// CLoginDlg 对话框
///////////////////////
class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	//CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数

	CLoginDlg(IModuleManager* pModuleManager,CHallDlg *pHallDlg, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CLoginDlg();

	// 对话框数据
	enum { IDD = IDD_LOGIN };

public:
	BOOL OnFirstLogin();  //首次启动客户端
	BOOL OnLoginSuccess();  //登录成功
	void OnLoginResp(Event const &evt); //请求登录返回响应函数	
	void OnVerfiyCodeResp(Event const &evt);//发送验证码请求返回响应函数

	void UIEnable(bool bEnable); //界面上的“账号”“密码”输入框以及登录”“游客登录”“注册”按扭是否可用
	void StopLoginingHallTray(); //停止登录过程中大厅托盘动态切换显示
	void SaveUserAccount();  //保存当前登录成功后的用户信息至配置文件中	
	void ReLogin(); //重新登录
	void CloseReconnectMsgBox(); //关闭m_pReconnectMsgBox非模态对话框
	CWnd* Get179MsgBoxParent();  //获取C179MsgBox显示时所需的父窗口


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClickClose();
	afx_msg void OnClose();	

	//OnSkinMessage函数里面的case SKIN_ON_CLICK_LOGIN
	afx_msg void OnClickLogin();  //点击了登录对话框中的"登录"按扭时的响应函数
	afx_msg void OnClickFindPassword(); //点击了"找回密码"按扭时的响应函数
	afx_msg void OnClickRegister();  //点击了"新注册"按扭时的响应函数	
	afx_msg void OnBnClickedCheckSavePassword(); //点击“保存密码”复选框时的响应函数
	afx_msg void OnBnClickedCheckAutoLogin();  //点击“自动登录”复选框时的响应函数	
	afx_msg void OnClickLoginAd(); //点击登录框广告图片

	
	afx_msg LRESULT OnAutoLogin(WPARAM wParam, LPARAM lParam); //自动登录  WM_AUTO_LOGIN消息的响应函数
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoginDlgSetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On179RegisterLogin(WPARAM wParam, LPARAM lParam);  //179注册成功后自动登录的响应函数
	afx_msg LRESULT On179RegisterQQLogin(WPARAM wParam, LPARAM lParam); //179注册框中点击"QQ登录"按扭的响应函数
	afx_msg HRESULT	OnClickDeleteAccount( WPARAM w, LPARAM l); //点击用户列表控件中"清删记录"时的响应函数
	afx_msg HRESULT OnSelectComboAccount(WPARAM w,LPARAM l); //选取用户下拉列表中某用户时的响应函数
	afx_msg HRESULT OnCbnEditchangeComboAccount(WPARAM wParam, LPARAM lParam); //用户下拉列表输入框中内容被更新	
	afx_msg LRESULT OnWebPluginMessage(WPARAM wParam, LPARAM lParam);  //来自web插件的消息
	afx_msg LRESULT OnQQLogin179Account(WPARAM wParam, LPARAM lParam);  //QQ对应179账号自动登录的响应函数
	afx_msg LRESULT OnSetQQLogin(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCloseReconnectMsgBox(WPARAM wParam, LPARAM lParam);  //用户点击了m_pReconnectMsgBox非模态关闭按扭的响应函数

	afx_msg LRESULT OnQQLoginMoveWindow(WPARAM wParam, LPARAM lParam);  //QQ登录框设置位置的响应函数
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);

	afx_msg HRESULT OnEditPasswordCtrl( WPARAM w, LPARAM l);
	afx_msg HRESULT OnSetFocusPasswordCtrl(WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseHoverPasswordCtrl( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseLeavePasswordCtrl( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseHoverAccountCtrl( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseLeaveAccountCtrl( WPARAM w, LPARAM l);

	afx_msg HRESULT OnTipClickBtn( WPARAM w, LPARAM l) ;
	afx_msg HRESULT OnTipClickClose( WPARAM w, LPARAM l) ;

	DECLARE_MESSAGE_MAP()	

protected:
	void SetQQLogin(BOOL bQQLogin);  //设置登录方式
	void ShowQQLogin(BOOL bShow);	
	void AccountLogin(); //用户登录	
	void OnLoginDlgKeyReturn(); //登录对话框按了回车键的吶应函数	
	bool b51AutoLogin179();//检查是否为从彩红登录白兔KTV

	void NotifyCreateSkinItem();	
	BOOL SendLoginRequest(LPCTSTR account, LPCTSTR password, CLoginVerifyHander *pHander=NULL);  //发送登录请求
	BOOL SendGuestLoginRequest();  //发送游客登录请求	
	
	void ClearPasswordInUI();  //清空界面上的密码框
	void ClearAccountInUI();   //清空界面上的用户名
	BOOL SetPasswordInUI(LPCTSTR passwd);  //设置界面上的密码
	CString GetPasswordInUI();    //获取界面上的密码	
	BOOL GetSavePasswordInUI();  //获取界面上的是否"保存密码"
	BOOL SetSavePasswordInUI(BOOL bTrue = TRUE);  //设置界面上的"保存密码"复选框
	BOOL GetAutoLoginInUI();  //获取界面上的是否"自动登录"
	BOOL SetAutoLoginInUI(BOOL bTrue = TRUE);  //设置界面上的是否"自动登录"复选框
	void OnClickBtnQQLogin();  //点击了"QQ帐户一键登录"按扭
	
	void LoadAccountComboBox();  //给用户列表控件加载179账号名列表	
	void SelectAccountInfoInUI(CString strAccount);  //从配置文件中获取指定用户信息设置至登录界面中	

	void HideLoginAllTip();  //隐藏登录界面上的所有提示标签
	void LoginingHallTray(); //登录过程中大厅托盘动态切换显示
	
	//以下为图片验证
	int OpenVerifyDlg();  //弹出图片验证对话框
	void CloseVerifyDlg(); //关闭图片验证对话框
	void OnVerfiyCodeError(CString errorText); //图片验证错误	
	CVerifyDlg *m_pVerifydg;  //图片验证对话框
	//以上为图片验证

	void ShowAccoutTip(CString TipText);
	void ShowPasswordTip(CString TipText);
	void ShowSelectQQTip();
	void ShowQQLoginTip();
	void HideAccoutTip();
	void HidePasswordTip();
	void HideSelectQQTip();
	void HideQQLoginTip();
	void HideAutoLoginTip();
	void SetQQLoginTip(bool isShow);	

	void LoginRespMaintanError(CString respError1,CString respError2);	//登录时返回服务器正在维修或连接服务器超时的错误时调用的函数处理

private:
	//for skin
	CComPtr<ISkinEngine>	m_pSkinEngine;
	IModuleManager* m_pModuleManager;

	CHallDlg * m_pHallDlg;  //大厅对话框指针
	CRBPasswordCtrl* m_pPasswordCtrl;  //界面上的“密码”框控件
	CButton			m_ctrlSavePassword; //界面上的"保存密码"复选框
	CTipsButton		m_ctrlAutoLogin;    //界面上的"自动登录"复选框
	bool  m_isClear; //是否可以清空界面上的"保存密码"，"自动登录"，"密码"
	
	LoginType m_loginType;   //登录类型	
	bool m_bVerifyOpen;
	bool m_bVerifyCodeError;
	CMsgBox *m_pReconnectMsgBox;

	//DragonZ: for 二期登录效果
	CQQLoginDlg* m_pQQLoginDlg;
	CLoginComboBox	m_testLoginComboBox; 
	C179TipCtrl * m_p179TipCtrl;
	C179TipCtrl * m_pAccoutTip;
	C179TipCtrl * m_pPasswordTip;
	C179TipCtrl * m_pQQLoginTip;
	BOOL m_bCurQQLogin;//当前是否为QQ登录界面
	

};
