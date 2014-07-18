#pragma once
#include "Skin_i.h"
#include "..\Resource.h"
#include "GiftManager.h"
#include "MyGoodsItem.h";

// CGiftTipDlg 对话框

class CGiftTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CGiftTipDlg)

public:
	CGiftTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGiftTipDlg();

	void Show(IGiftItem * pGiftItem, int nCount = 0);

// 对话框数据
	enum { IDD = IDD_GIFTTIPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

protected:
	void OnClickSend();
	void OnClickRecharge();
	void ShowDstErn();
	void HideDstErn();
	void ShowBtnRecharge();
	void HideBtnRecharge();
	void ShowPrescription(std::wstring strPrescription);
	void HidePrescription();
	void ShowSendAndRecharge();
	void HideSendAndRecharge();
	void ShowCount(int nCount);
	void HideCount();

private:
	CComPtr<ISkinEngine>  m_pSkinEngine;
	IGiftItem * m_pGiftItem;
	CWnd * m_pParentWnd;
};
