#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "../../../include/ui/mlhtmlview.h"

#define SYS_HTML_NOTIFY_WIDTH 250   
#define SYS_HTML_NOTIFY_HIGHT 170   
// CSysHtmlNotifyDlg 对话框

class CSysHtmlNotifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysHtmlNotifyDlg)

public:
	CSysHtmlNotifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysHtmlNotifyDlg();

// 对话框数据
	enum { IDD = IDD_HALL_SYSNOTIFY_HTML_DLG };

	BOOL IfMouseLeave();
	void SetSystemMessage(SystemMessage* sysMes);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnClick(WPARAM wParam,LPARAM lParam);   //载取点击m_htmlSysNotify系统消息页面时的响应函数

	void InitTheHtml();

	void PutElementHtmlMsg();  // 修改网页中消息元素的内容

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	CMLHtmlView m_htmlSysNotify;
	BOOL m_bMouseLeave;
	SystemMessage* m_CurSysMessage;  //当前系统消息
};
