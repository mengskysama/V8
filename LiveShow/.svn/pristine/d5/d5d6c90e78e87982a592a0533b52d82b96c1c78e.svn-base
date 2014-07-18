#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"

#define WM_ONVSCROLL_VERTI_SLIDER    WM_USER + 880

// CVeritySliderDlg 对话框

class CVeritySliderDlg : public CDialog
{
	DECLARE_DYNAMIC(CVeritySliderDlg)

public:
	CVeritySliderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVeritySliderDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_VERITY_SLIDER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HRESULT OnMouseLeave( WPARAM wParam , LPARAM lParam ) ;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
public:
	void SetRange(int iMin, int iMax, BOOL bRedraw = TRUE);
	void SetPageSize(int iSize);
	void SetPos(int nPos);
	int  GetPos();
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CSliderCtrl m_VeritySlider;
};
