#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "..\..\include/ui/mlhtmlview.h"
#include "..\fans\fansValue.h"

// CShowFansDlg 对话框

class CShowFansDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowFansDlg)

public:
	CShowFansDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowFansDlg();

// 对话框数据
	enum { IDD = IDD_ATROOM_SHOW_FANS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	virtual void OnShowWindow(BOOL bShow,UINT nStatus);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void Init();	

	DECLARE_MESSAGE_MAP()


public:
	void SetApplyEffect(BOOL bEffect);//设置点击应用的效果
	void OnGetFansRsp(Event const&evt);  //获取粉丝数据返回响应函数	

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlFans;
	CFansValue m_fansValue;
};
