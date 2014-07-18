#pragma once


// CColorEdit

class AFX_EXT_CLASS CColorEdit : public CEdit
{
	DECLARE_DYNAMIC(CColorEdit)

public:
	CColorEdit();
	virtual ~CColorEdit();

	//设置文本颜色
	void SetTextColor(COLORREF rgb);

	//设置背景颜色
	void SetBackColor(COLORREF rgb);
	void SetDisableBackColor( COLORREF rgb);

	//设置边框颜色
	void SetRimColor(COLORREF rgb)
	{
		m_crBorder = rgb;
	}

protected:

	COLORREF            m_crText;               //文本颜色
	CBrush              m_brBackGnd;            //背景刷子
	COLORREF            m_crBackGnd;            //背景颜色
	COLORREF			m_crBorder;
	COLORREF			m_crDisableBackGnd;		//禁用时候的背景色

	//{{AFX_MSG(CColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void   OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


