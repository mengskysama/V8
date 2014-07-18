#pragma once

#include "Skin_i.h"
#include "../Resource1.h"
#include "C179MsgBox.h"

class CModelessMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CModelessMsgBox)

public:
	CModelessMsgBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModelessMsgBox();

// 对话框数据
	enum { IDD = IDD_MSGBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

public:	
	void Show(CWnd* pParent, LPCTSTR pzPrompt,MSGBOX_STYLE style = MsgBox::STYLE_OK,LPCTSTR pzTitle = _T("提示"));//无链接时用的Show,默认是只有一个确定键
	void Show(CWnd* pParent, LPCTSTR pzPrompt,LPCTSTR linkLabel,LPCTSTR linkAddress,MSGBOX_STYLE style = MsgBox::STYLE_OK_WITH_LINK,LPCTSTR pzTitle = _T("提示"));//有链接时用的Show,默认是只有一个确定键
	void Show(CWnd* pParent, LPCTSTR pzPrompt1,LPCTSTR linkLabel1,LPCTSTR linkAddress1,
		LPCTSTR pzPrompt2,LPCTSTR linkLabel2,LPCTSTR linkAddress2,
		MSGBOX_STYLE style = MsgBox::STYLE_OK_CANCEL_WITH_2_LINK,LPCTSTR pzTitle = _T("提示"));//两个链接的用这个接口，唉。杂！！

private:

	void CreateAndShow(CWnd* pParent);

	CComPtr<ISkinEngine>  m_pSkinEngine;
	CString m_strTitle;
	CString m_strPrompt;
	CString m_strLinkLabel;
	CString m_strLinkAddress;
	//带2个链接的,上面三个默认为1
	CString m_strPrompt2;
	CString m_strLinkLabel2;
	CString m_strLinkAddress2;

	MSGBOX_STYLE m_Style;
	BOOL m_bDrawed;
};
