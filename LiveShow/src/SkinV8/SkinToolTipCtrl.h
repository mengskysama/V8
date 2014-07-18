#pragma once

#include "SkinTextStyle.h"

// CSkinToolTipCtrl

class CImageBase;

class CSkinToolTipCtrl : public CWnd
{
	DECLARE_DYNCREATE(CSkinToolTipCtrl)

public:
	CSkinToolTipCtrl();
	virtual ~CSkinToolTipCtrl();

    BOOL Create(CWnd* pParent);

public:
    CImageBase* GetBackImage();
    void SetBackImage(CImageBase* pImage);
    CImageBase* GetIconImage();
    void SetIconImage(CImageBase* pImage);

    CString GetCaption();
    void SetCaption(LPCTSTR lpzCaption);
    CString GetText();
    void SetText(LPCTSTR lpzText);
    CString GetButtonText();
    void SetButtonText(LPCTSTR lpzText);

    LONG GetMaxWidth();
    void SetMaxWidth(LONG lMaxWidth);
    LONG GetMaxHeight();
    void SetMaxHeight(LONG lMaxHeight);

    POINT GetPos();
    void SetPos(POINT pt);

    BOOL GetLeftPreferred();
    void SetLeftPreferred(BOOL bLeft);
    BOOL GetTopPreferred();
    void SetTopPreferred(BOOL bTop);

protected:
    SIZE Layout();

protected:
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

protected:
    //客户区域大小
    CSize m_szClient;

    //背景
    CImageBase* m_pBackImage;

    //图标
    CImageBase* m_pIconImage;
    CRect m_rcIcon;

    //标题
    CString m_strCaption;
    CSkinTextStyle m_styleCaption;
    CRect m_rcCaption;

    //文字
    CString m_strText;
    CSkinTextStyle m_styleText;
    CRect m_rcText;

    //按钮
    CString m_strButton;
    CSkinTextStyle m_styleButton;
    CRect m_rcButton;

    //最大宽度和高度，-1为不限
    LONG m_nMaxWidth;
    LONG m_nMaxHeight;

    //指示点
    CPoint m_ptPos;

    //弹出位置偏好
    BOOL m_bLeftPreferred;  //左偏好
    BOOL m_bTopPreferred;   //上偏好

    //间距
    LONG m_lHorizMargin;
    LONG m_lVertiMargin;
};

