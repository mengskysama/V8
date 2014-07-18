
#pragma once
#include "LoginVerifyHander.h"
#include "resource.h"
#include "Skin_i.h"

#define VERIFY_TEXT_DEFAULT _T("抱歉，密码多次输入错误，为保障\n您的账号安全，请输入验证码")  //密码多次出错，为保证账号安全，请输入验证码
#define VERIFY_TEXT_EXPIRED _T("        验证码已过期")
#define VERIFY_TEXT_EMPTY _T("          请输入验证码")
#define VERIFY_TEXT_ERROR _T("          验证码输入有误")
#define VERIFY_TEXT_VERIFYING _T("正在验证...")

// CVerifyDlg 对话框

class  CVerifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CVerifyDlg)

public:
	CVerifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVerifyDlg();

// 对话框数据
	enum { IDD = IDD_VERIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CLoginVerifyHander m_pVerifyHander;

public:
	void GetVerifyCode();	//请求获取验证码图片
	void OnGetVerifyCodeRep(Event const& evt);          //请求获取验证码图片返回响应函数
	void OnCodeError(CString errorText);					//验证码错误

	void OnClickRefreshCode();			//点击更新验证码
	void OnClickOk();					//用户确认
	void OnClickClose();
	void OnClickCancel();

	void NotifySkinCode(CString strCodePath);				//通知皮肤验证码
	void NotifySkinText(CString strText);
	void NotifySkinColor(LONG color);
	
	void OnCodeSuccess();	
	void OnOK();
	void OnCancel();	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();

	uint32  m_LastVerfiyTime;
};

