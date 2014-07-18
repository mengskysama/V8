#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\include\ui\mlhtmlview.h"
#include "..\usermanager\UserManager.h"
#include "fansValue.h"
// CAchorFansCardDlg 对话框


class CRoomParentDlg ;
class CAnchorFansCardDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnchorFansCardDlg)

public:
	CAnchorFansCardDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnchorFansCardDlg();

	// 对话框数据
	enum { IDD = IDD_ANCHORFANS_CARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog() ;
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam) ;	
	
	//void OnGetFans();  //获取粉丝数据
	DECLARE_MESSAGE_MAP()
public:
	void Show(IUserItem * pUserItem );
	void Hide();
	void OnGetFansRsp(Event const&evt);  //获取粉丝数据返回响应函数	
	
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	std::wstring  m_PansHtmlPath ;
	CMLHtmlView m_htmlFansCard;
	CFansValue m_fansValue;
	uint32 m_unAnchorUin;
};
