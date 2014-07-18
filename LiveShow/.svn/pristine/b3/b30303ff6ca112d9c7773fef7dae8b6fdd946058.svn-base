#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\..\include/ui/mlhtmlview.h"
// CShowMyRoomDlg 对话框

class CShowMyRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowMyRoomDlg)

public:
	CShowMyRoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowMyRoomDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_MYROOM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEnterRoom(WPARAM wParam,LPARAM lParam);  //内嵌页面中接口发送的进房请求

	void Init();

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_MyRoom;

};
