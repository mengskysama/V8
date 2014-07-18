#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "../../include/ui/mlhtmlview.h"
#include "../CommonLibrary/include/utility/AsynOpenURL.h"
// CQQLoginDlg 对话框

struct QQLogin179Account
{
	CString m_Account;
	CString m_Password;  //MD5加密密码
};

class CQQLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CQQLoginDlg)

public:
	CQQLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQQLoginDlg();

// 对话框数据
	enum { IDD = IDD_QQ_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnSysCommand(UINT nID,LPARAM lParam);

	void OnQQloginGetProLoad();//WM_QQLOGIN_GET_PRELOADQQ 消息的响应函数，预加载QQ登录页面响应函数	
	void OnQQloginClick(LPARAM lParam); //WM_MISSION_HYPERLINK_ONCLICK 消息的响应函数， QQ登录有点击操作
	void OnQQloginGet179AccountPwd();//获取QQ对应的179号和MD5加密密码	
	void OnQQLoginUserCannel(); //QQ登录授权页面中用户取消授权

	void QQloginLoad(); //QQ登录页面加载中
	void QQloginLoadError(); //QQ登录页面加载失败	
	void QQloginAgainLoad(); //再试一次,重新打开QQ登录页面
	void QQloginUse179login(); //使用179账号登录，转至179登录
	void QQloginRegister179Web(); //注册179账号， 使用浏览器打开官方179账号注册页面
	void QQloginOpenURL(UINT unMessage,CString strURL);

	void QQSetHTMLSize();  //设置QQ内嵌页面的大小
	void GetDefineSize();  //获取NewVersion.ini配置文件中QQ登录框的默认大小
	bool IEversion6();  //当前IE版本是否为IE6

	DECLARE_MESSAGE_MAP()

public:
	void Show();
	void Hide();//淡入淡出效果
	void QQloginPreLoad(); //QQ登录页面预加载中
	void QQloginError();  //QQ登录失败
	void GetQQHtmlSize(int &qqWidth,int &qqHeight);

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	BOOL m_bLayered;//是否有分层窗口属性

	CMLHtmlView m_htmlQQLogin;
	//关于窗口透明
	typedef BOOL (WINAPI* FSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD) ;
	FSetLayeredWindowAttributes SetLayeredWindowAttributes ;
	int m_alpha;
	HINSTANCE m_hInst;

	uint32 m_nSeqNo;
	common::utility::CAsynOpenURL *m_QQLoginURL;	
	BYTE m_QQBuffer[1280];	
	bool m_IE6;  //当前是否为ie6版本浏览器
	uint32 m_HtmlHeight;
	uint32 m_HtmlWidth;
	uint32 m_defineHeight;
	uint32 m_defineWidth;
};
