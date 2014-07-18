#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

// CAnnouncementDlg 对话框
class CTempAnnounceDlg;
class CRoomAnnounceDlg;
class CRoomParentDlg ;
class CAnnouncementDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnnouncementDlg)

public:
	CAnnouncementDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnnouncementDlg();

	void Refresh();

// 对话框数据
	enum { IDD = IDD_ANNOUNCEMENT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void NotifyCreateSkinItem();

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	CTempAnnounceDlg * m_pTempAnnounceDlg;
	CRoomAnnounceDlg * m_pRoomAnnounceDlg;

	CRoomParentDlg* m_pParentDlg ;

public:
	void TurnToTempAnnouncePanel( CString& strText ) ;
};
