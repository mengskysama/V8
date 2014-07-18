#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"

// CKickDlg 对话框
class CRoomParentDlg ;
class CKickDlg : public CDialog
{
	DECLARE_DYNAMIC(CKickDlg)

public:
	CKickDlg( CRoomParentDlg* pParentDlg , uint32 unDstUIN, uint8 unType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKickDlg();

	void SetKickType(uint8 unType);
	void SetKickDstUIN(uint32 unDstUIN);

// 对话框数据
	enum { IDD = IDD_KICK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCbnSelchangeComboKickReason();

	void OnClickSaveReason();
	void OnClickDeleteReason();
	void OnClickConfirm();

	bool LoadCustomizeReason();
	bool SaveCustomizeReason();

	uint16 GetKickTime(CString strTime);

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	CComboBox m_comboReason;
	CComboBox m_comboTime;

	std::vector<CString> m_vecCustomizeReason;
	std::set<CString> m_setDefaultReason;

	CRoomParentDlg* m_pParentDlg ;
	uint32 m_unDstUIN;
	uint8 m_unType; // 0:踢人 1:踢人并封IP

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
