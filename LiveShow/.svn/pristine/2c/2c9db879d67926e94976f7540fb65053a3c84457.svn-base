#pragma once

#include "Skin_i.h"
#include "../../Resource1.h"
#include "C179MsgBox.h"



class CMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CMsgBox)

public:
	CMsgBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgBox();

// 对话框数据
	enum { IDD = IDD_MSGBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	void OnClickBtnOK();
	void OnClickBtnCancel();
	void SetContent();

	
	DECLARE_MESSAGE_MAP()

public:	
	
	BOOL GetIsChecked();
	static BOOL GetMsgBoxVisible(){ return m_bMsgBoxVisible; }

	//新提示框接口
	int Show(MSGBOX_STYLE nStyle, LPCTSTR pzPrompt1, LPCTSTR pzPrompt2, LPCTSTR pzPrompt3,  LPCTSTR linkLabel, LPCTSTR linkAddress, LPCTSTR pzTitle, LPCTSTR pzCheckBox);
	int Show(MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1, LPCTSTR pzPrompt2, LPCTSTR pzTitle);
	int ShowNonDomodal(MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1, LPCTSTR pzTitle);
    //点歌系统提示框
	int ShowForOrderSong(MSGBOX_STYLE nStyle, LPCTSTR pzString1, LPCTSTR pzString2, LPCTSTR colorString1, LPCTSTR colorString2, LPCTSTR btnOKLabel, LPCTSTR btnCancelLabel, LPCTSTR pzTitle);
private:
	CComPtr<ISkinEngine>  m_pSkinEngine;
	CString m_strTitle;
	CString m_strPrompt;
	CString m_strLinkLabel;
	CString m_strLinkAddress;
	//带2个链接的,上面三个默认为1
	CString m_strPrompt2;
	CString m_strPrompt3;
	CString m_strLinkLabel2;
	CString m_strLinkAddress2;
	CString m_iconPath;
	CString m_strCheckBox;
	BOOL m_bChecked;

	//用于点歌系统
	CString m_strOKLabel;
	CString m_strCancelLabel;
	CString m_str1Color;
	CString m_str2Color;

	MSGBOX_STYLE m_Style;
	MSGBOX_ICONTYPE m_iconType;
	int m_iRet;	

    static BOOL m_bMsgBoxVisible;

	CWnd* m_pParent;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckNoPrompt();
};
