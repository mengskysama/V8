#pragma once
#include "logindlg.h"


class CLoginVerifyHander	
{
public:
	CLoginVerifyHander(HWND hwndLogin);
	~CLoginVerifyHander(void);

public:
	void GetVerifyCode();               //请求获取验证码图片
	CString OnGetVerifyCodeRep(Event const& evt);          //请求获取验证码图片返回响应函数，返回验证码图片路径

	void RefreshCode();					//刷新验证码
	void VerifyCode(CString strVerifyCode);	
	string GetKey();
	CString GetStrVerifyCode();
	
private:	
	string m_strKey;//服务器传给客户端的一个key
	CString m_strVerifyCode;//用户输入的明文的验证码
	uint32 m_respId;								//验证码图片的请求Id	
	HWND m_loginHwnd;
};
