#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"

// CMicSettingDlg 对话框
class CRoomParentDlg ;
class CMicSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CMicSettingDlg)

public:
	CMicSettingDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMicSettingDlg();

	void Refresh();
	void OnConfirm();

// 对话框数据
	enum { IDD = IDD_MIC_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void InitDlgData();
	void NotifyParentModify();

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	bool m_bInited;
	//CVDCComboBox m_comboMicTime;
	
public:
	afx_msg void OnBnClickedRadioFreeMicClose();
	afx_msg void OnBnClickedRadioFreeMicOpen();
	afx_msg void OnCbnSelchangeComboMicTime();
	afx_msg void OnBnClickedCheckLimitMgrTime();
	afx_msg void OnBnClickedCheckModifyMgrTime();
	afx_msg void OnBnClickedRadioRoomPrivatemicClose();
	afx_msg void OnBnClickedRadioRoomPrivatemicOpen();
};
