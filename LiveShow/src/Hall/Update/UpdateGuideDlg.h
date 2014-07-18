#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "../../include/ui/mlhtmlview.h"

#define TIMER_UPDATE_TRAY 2      //定时器 更新时的托盘图标动态显示
#define TIMER_UPDATE_PROGESS_IDEVENT 1  //定时器 获取当前下载进度
#define TIMER_UPDATE_PROGESS_INTERVALTIME 200 //定时器间隔 获取当前下载进度的间隔时间（单位：豪秒，即0.5秒）

// CUpdateGuideDlg 对话框
class CAutoUpdate;
class CUpdateGuideDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateGuideDlg)

public:
	CUpdateGuideDlg(CAutoUpdate *autoUpdate,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateGuideDlg();

// 对话框数据
	enum { IDD = IDD_UPDATEGUIDE_DLG };

	void SetUpdateVersion(CString sCurVer,CString sNewVer);  //设置更新版本,
	void DownProgess(uint32 &nTotalTime,uint32 &nFinishTime,uint32 &nProcess); //下载进度条

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnTimer(UINT_PTR nIDEvent);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDownloadProgessFull(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateText(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();	

	void DrawUpdateProgess(uint32 nTotalTime,uint32 nFinishTime);
	void CloseEnabled(bool isEnabled); //关闭按扭是否可用

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlUpdateGuide;
	CAutoUpdate *m_autoUpdate;
};
