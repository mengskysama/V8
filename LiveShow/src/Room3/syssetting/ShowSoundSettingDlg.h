#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "AudioVideoSetting.h"
#include "SysSettingDlg.h"
// CShowHarassmentDlg 对话框

class CShowAudioTestDlg;
class CShowSoundSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowSoundSettingDlg)

public:
	CShowSoundSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowSoundSettingDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_SOUND_SETTING_DLG };  //音频设置对话框

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	virtual BOOL OnInitDialog();
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheck();

	void Init();
	void OnClickBtnAudioTest();
	void OnClickBtnMoreAudioSetting();  //更多音频设置按扭响应函数
	void OnClickBtnGotoFreshRoom();  //
	void Confirm();  //确认保存
	

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CShowAudioTestDlg *m_pAudioTest;
	EnMixerControlMode m_enmMode;
	
};
