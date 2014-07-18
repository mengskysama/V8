#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

// CAudioTestDlg 对话框

class CShowAudioTestStep1;
class CShowAudioTestStep2;

class CShowAudioTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowAudioTestDlg)

public:
	CShowAudioTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowAudioTestDlg();

	void StopTest();

// 对话框数据
	enum { IDD = IDD_ATROOM_AUDIOTEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void NotifyCreateSkinItem();
    void OnClickCancel();
	void OnClickPreStep();
	void OnClickNextStep();
	void OnClickDone();

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine>  m_pSkinEngine;

	CShowAudioTestStep1 * m_pAudioTestStep1;
	CShowAudioTestStep2 * m_pAudioTestStep2;
};
