#pragma once

#include "stdafx.h"
#include "Skin_i.h"
#include "Resource.h"
#include "afxwin.h"
// CEnterRoomPwdDlg 对话框

class CEnterRoomRequest;

class CEnterRoomPwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnterRoomPwdDlg)

public:
	CEnterRoomPwdDlg(CEnterRoomRequest *enterRoom,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEnterRoomPwdDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void OnClickBtnOK();
	void OnClickBtnClose();

	DECLARE_MESSAGE_MAP()

private:	
	uint32 m_roomId;
	CEnterRoomRequest *m_enterRoom;	
	CEdit m_password;
	CWnd *m_pParent;

public:
	CComPtr<ISkinEngine>  m_pSkinEngine;

	void SetEnterRoomID(int roomID);
};
