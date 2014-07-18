#pragma once
#include <vector>
#include "afxcmn.h"
#include "..\Resource.h"
#include "Skin_i.h"
#include "AutoReplyConfig.h"
#include "afxwin.h"

// CAutoReplySettingDlg 对话框

class CAutoReplySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoReplySettingDlg)

public:
	CAutoReplySettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoReplySettingDlg();

// 对话框数据
	enum { IDD = IDD_AUTOREPLY_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMHoverListAutoreply(NMHDR *pNMHDR, LRESULT *pResult);

	void InitListCtrl();
    void OnClickBtnAdd();
	void OnClickBtnDelete();
	void OnClickBtnModify();

	void OnClickBtnConfirm();
	void OnClickBtnClose();

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	std::vector<stAutoReplyMenuItem> m_vAutoReplyList ;
	CListCtrl m_autoReplyListCtrl;
	CButton m_OpenAutoReply;
	BOOL m_bCheck;
	int m_nMySelelected;//选中的自动回复消息
	BOOL m_bChange;//有进行增删改
public:
	BOOL GetAutoReplyState();
	int GetSelectedItem(){ return m_nMySelelected; }
	BOOL GetAutoReplyChange(){return m_bChange;}
	std::vector<stAutoReplyMenuItem>& GetNewAutoReply(){return m_vAutoReplyList;}	
};
