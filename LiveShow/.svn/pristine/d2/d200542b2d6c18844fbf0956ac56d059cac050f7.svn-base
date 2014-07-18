#pragma once
#include "..\resource.h"
#include "Skin_i.h"
#include "RoomDefine.h"
// CAudioDlg 对话框

#define DEVICE_NAME_LEN 128

class CVideoSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoSettingDlg)

public:

	CVideoSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoSettingDlg();

	// 对话框数据
	enum { IDD = IDD_ATROOM_AUDIO_DLG };
	
	void StartVideo(int iDevice);  //开启指定视频
	void CloseVideo();  //关闭视频
	void ShowVideoAdjust();  //显示已开启视频的属性对话框
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);	

	void OnVedioChanged();
	void OnDestroy();
	BOOL PreTranslateMessage(MSG* pMsg);	
	void SendSetVideo();   //发送设置视频请求
	void SendStartVideo(); //发送开启视频请求
	void SendCloseVideo(); //发送关闭视频请求

	DECLARE_MESSAGE_MAP()

private:

	CComPtr<ISkinEngine> m_pSkinEngine;
	int m_iDevice; //摄像头设备序列 视频设备	
};