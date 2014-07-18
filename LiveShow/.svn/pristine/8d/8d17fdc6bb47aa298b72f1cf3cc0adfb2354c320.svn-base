#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
// CUserInfoCardDlg 对话框
#include "..\usermanager\UserManager.h"

class CRoomParentDlg ;
class CUserInfoCardDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserInfoCardDlg)

public:
	CUserInfoCardDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserInfoCardDlg();

// 对话框数据
	enum { IDD = IDD_USERINFO_CARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog() ;

	void GetVIPPath(uint8 vipLevel,CString &imgVip) ;
	void SetRichLevel(IUserItem * pUserItem);
	DECLARE_MESSAGE_MAP()
public:
	void Show(IUserItem * pUserItem );
	void Hide();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam) ;
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);
	void OnClickDetail() ;
	void OnClickPhoto() ;
	void OnClickSelfVerify();
	void OnHallHeadImageUpdate() ;
	uint32 m_unUserUIN ;
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	CString m_ResListiconPath ;
};
