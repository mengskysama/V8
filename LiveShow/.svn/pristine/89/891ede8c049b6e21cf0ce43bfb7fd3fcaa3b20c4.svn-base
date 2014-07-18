#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\gift\GiftComboBox.h"
#include "..\usermanager\useritem.h"

// CPaodaoDlg 对话框
class CRoomParentDlg ;
class CGiftListCtrl ;

class CPaodaoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaodaoDlg)

public:
	CPaodaoDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaodaoDlg();

	// 对话框数据
	enum { IDD = IDD_PAODAO_DLG };
	void InitLuxuryList() ;
	void InitSendToCombo() ;
	void OnClickNameLink( CString& strName , uint32 unUUID ) ;
	void SetSendObj( CString strName , uint32 unUIN ) ;
	void OnDeleteUser(IUserItem * pUserItem) ;
	void OnRoomRightSendToChange( CString& strSelText ) ;
	void UpdateGiftList();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnClickGiftList(WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	CGiftComboBox m_GiftCountCombo;												//礼物数量控件
	CComboBox m_GiftSendToCombo;												//礼物赠送对象
	CGiftListCtrl* m_pListCtrl ;
public:
	afx_msg void OnCbnSelchangeComboGiveto();
};
