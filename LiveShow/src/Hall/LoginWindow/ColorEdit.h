#pragma once

#include "afx.h"

// CColorEdit

class  CColorEdit : 
	public CEdit
{
	DECLARE_DYNAMIC(CColorEdit)

public:
	CColorEdit();
	virtual ~CColorEdit();


	/**
	@fn SetTextColor
	@brief 设置编辑框中文本的颜色
	@param	[in] rgb	设置的文本颜色
	**/
	void SetTextColor(COLORREF rgb);

	/**
	@fn GetTextColor
	@brief 获取编辑框中文本的颜色
	@return	
	**/
	const COLORREF GetTextColor();

	/**
	@fn SetBackColor
	@brief 设置编辑框的背景色
	@param	[in] rgb	设置的背景颜色
	**/
	void SetBackColor(COLORREF rgb);

	/**
	@fn GetBackColor
	@brief 获取编辑框的背景色
	@return	
	**/
	const COLORREF GetBackColor();

	/**
	@fn SetDisableBackColor( COLORREF rgb)
	@brief 当编辑框被禁用的时候的背景色
	@param	[in] rgb	设置的禁用背景颜色
	**/
	void SetDisableBackColor( COLORREF rgb);

	/**
	@fn SetRimColor( COLORREF rgb)
	@brief 编辑框的背景色
	@param	[in] rgb	设置的边框色
	**/
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
};


