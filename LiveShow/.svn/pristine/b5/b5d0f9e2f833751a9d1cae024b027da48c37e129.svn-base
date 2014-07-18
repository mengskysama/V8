#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"
// CAudioTestStep1 对话框

class CShowAudioTestStep1 : public CDialog
{
	DECLARE_DYNAMIC(CShowAudioTestStep1)

public:
	CShowAudioTestStep1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowAudioTestStep1();

	void StopTest();

// 对话框数据
	enum { IDD = IDD_ATROOM_AUDIOTEST_STEP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void OnClickTest();
	void OnClickStop();

	void LoadAudioOutputDeviceList();

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine>  m_pSkinEngine;
//	CVDCComboBox m_comboLoudSpeaker;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
