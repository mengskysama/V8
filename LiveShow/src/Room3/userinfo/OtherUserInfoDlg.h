#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "..\..\include\ui\mlhtmlview.h"
#include "..\fans\fansValue.h"

// COtherUserInfoDlg 对话框


enum OtherUserInfoDlgType
{
	Show_Other_UserInfo   ,  //其他人信息	
	Show_Other_ItsRoom    ,  //TA的房间
	Show_Other_ItsFans		  ,  //TA的粉丝
	Show_Other_ItsBankAccount ,  //银行账号
};

struct OtherUserInfo
{
	uint32 m_UserUin;        //用户uin
	uint32 m_UserAccount;    //用户帐号	
	uint32 m_VipLevel;       //用户身份
	uint8 m_RichLevel;       //用户富豪等级
	bool m_Gender;           //用户性别  false为女，true为男
	bool m_IsActor;         //是否为艺人
	bool m_IsAgent;         //是否为销售人员
	std::wstring m_UserRoleName;  //用户昵称

	OtherUserInfo()
	{
		m_UserUin = 0;
		m_UserAccount = 0;
		m_VipLevel = 0;
		m_RichLevel = 0;
		m_IsActor = 0;
		m_IsAgent = 0;
	}
};

class COtherUserInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(COtherUserInfoDlg)

public:
	COtherUserInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COtherUserInfoDlg();

// 对话框数据
	enum { IDD = IDD_OTHER_USER_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEnterRoom(WPARAM wParam,LPARAM lParam);  //内嵌页面中接口发送的进房请求


	DECLARE_MESSAGE_MAP()

	void InitUserInfoBrowser(); //用户信息页面
	void InitUserItsRoom();		//用户管理的房间页面
	void InitUserItsFans();     //用户粉丝的页面(本地页面)
	void InitUserBankAccount();		//用户银行账号页面
	void OnClickClose();
	void OnClickUserInfo();
	void OnClickItsRoom();	
	void OnClickItsFans();	
	void OnClickBankAccount();	
	bool IsCurFansDlg();  //当前显示是否为TA的粉丝
	

public:
	void SetUserInfo(OtherUserInfo &userInfo);
	void OnGetHeadImage(uint32 unUIN);
	void HeadImageUpdate(uint32 unUIN);
	void OnGetFansRsp(Event const&evt);//获取粉丝排行数据返回函数
	void OpenOtherDlg(OtherUserInfoDlgType dlgType);

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlOtherUserInfo;
	OtherUserInfo m_userInfo;
	CFansValue m_fansValue;


	
};
