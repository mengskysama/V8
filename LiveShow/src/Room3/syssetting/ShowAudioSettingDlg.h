/*
注：这个类是------------- 视频测试 -----------对话框，不是音频测试
*/
#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "VideoSettingDlg.h"
#include "RoomDefine.h"
#include "SysSettingDlg.h"
#include "AudioVideoSetting.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"

#define DEVICE_NAME_LEN 128
#define MAX_VIDEO_DEVICE_NUM      10

// CShowAudioSettingDlg 对话框
class CAudioDlg;
class CShowAudioSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowAudioSettingDlg)

public:
	CShowAudioSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowAudioSettingDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_AUDIO_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCbnSelchangeComboCamera(); //选择摄像头响应函数

	void NotifyCreateSkinItem();
	void CloseVideoSet();  //关闭摄像头
    
	void OnClickBtnRefresh();  //“刷新”按扭响应函数
	void OnClickBtnPicQuality(); //“视频设置”按扭响应函数

	void Confirm();
	void Init();

	DECLARE_MESSAGE_MAP()
public:
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CVideoSettingDlg * m_pAudioDlg;
	
	CComboBox m_VideoList;
	
};
