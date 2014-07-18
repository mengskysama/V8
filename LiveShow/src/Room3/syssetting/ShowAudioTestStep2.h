#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "..\personalsetting/VideoManager.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"
// CAudioTestStep2 对话框

class CShowAudioTestStep2 : public CDialog
{
	DECLARE_DYNAMIC(CShowAudioTestStep2)

public:
	CShowAudioTestStep2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowAudioTestStep2();

	void StopTest();

// 对话框数据
	enum { IDD = IDD_ATROOM_AUDIOTEST_STEP2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void LoadAudioInputDeviceList();
	void DrawLevelProgess(int nTotal,int nFinish);

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine>  m_pSkinEngine;
	RoomAVLogic* m_pRoomAvLogic;
	bool m_bWavInEnable;
	//CVDCComboBox m_comboMic;
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
