#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "FlyingInputRichEdit.h"

class CRoomParentDlg;
// CFlyingTextInputDlg 对话框

class CFlyingTextInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CFlyingTextInputDlg)

public:
	CFlyingTextInputDlg(CRoomParentDlg * pParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlyingTextInputDlg();

	void OnClickSend();
	int GetType();
	void SetType(int nType);

// 对话框数据
	enum { IDD = IDD_FLYINGTEXT_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CFlyingInputRichEdit m_InputCtrl;
	int m_nType;
	CRoomParentDlg * m_pParentDlg;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnInputChange();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
