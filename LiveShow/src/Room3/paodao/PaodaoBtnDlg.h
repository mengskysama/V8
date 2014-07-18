#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\usermanager\useritem.h"

// CPaodaoBtnDlg 对话框
class CRoomParentDlg ;
class CPaodaoDlg ;
class CPaodaoStaitc ;

class CPaodaoBtnDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaodaoBtnDlg)

public:
	CPaodaoBtnDlg( CRoomParentDlg* pParentDlg , CPaodaoStaitc* pPaodaoStaitc , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaodaoBtnDlg();

	// 对话框数据
	enum { IDD = IDD_PAODAO_BTN_DLG };

public:
	void OnClickNameLink( CString& strName , uint32 unUUID ) ;
	void OnDeleteUser(IUserItem * pUserItem);
	void OnRoomRightSendToChange( CString& strSelText ) ;
	void UpdateGiftList();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	HRESULT OnMouseLeave( WPARAM wParam , LPARAM lParam ) ;
	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	CPaodaoStaitc* m_pPaodaoStatic ;
	CPaodaoDlg* m_pPaodaoDlg ;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
