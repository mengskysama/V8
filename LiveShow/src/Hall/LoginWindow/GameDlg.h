#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "LoginDlg.h"
#include "../../include/ui/mlhtmlview.h"

class CGameDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(HWND sendMesHWND, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
	enum { IDD = IDD_REGISTER_DLG };

	void SetURL(CString& strURL);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnClick(WPARAM wParam,LPARAM lParam);   //载取点击页面中超链接时的响应函数
	afx_msg LRESULT OnOpenURL(WPARAM wParam,LPARAM lParam);

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnSysCommand(UINT nID,LPARAM lParam);
	
	void RegisterElementSetFocus(); //设置内嵌页面的焦点

	void SetWindowSize(int wndWidth,int wndHeight);//调用此接口以改变窗口大小
	void InitBrowser();

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlGame;
	HWND m_sendMesHWND;	
	CWnd* m_pParent;
	CString m_gameURL;
};
