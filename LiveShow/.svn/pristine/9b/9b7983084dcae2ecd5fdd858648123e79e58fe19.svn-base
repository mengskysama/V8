#pragma once

#include "../Common.h"
#include "afx.h"
#include "afxtempl.h"

namespace common { namespace ui
{

class COMMON_INTERFACE CSimpleTipCtrl 
	: public CWnd
{
public:

	typedef struct
	{
		CString	szText;						//	Tooltip text

	} TipToolInfo;

	enum
	{
		timerShow			= 100,
		timerHide			= 101
	};

public:
	CSimpleTipCtrl();
	virtual ~CSimpleTipCtrl();

	/**
	@fn Show(CString szText, CWnd* pParent, CPoint *pt)
	@brief 在指定的位置点显示Tips
	@param	[in] szText	Tips的内容
	@param	[in] pt	弹出tips的位置，如果为NULL，弹出点为鼠标当前位置
	@param	[in] nWidth	该Tips的宽度
	@return 
	**/
	virtual void	Show(CString szText, CWnd* pParent, CPoint *pt = NULL, UINT nWidth = 60);

	/**
	@fn BeHide
	@brief 指定当前tips是否隐藏
	@return 
	**/
	BOOL	BeHide() ;

	/**
	@fn Hide
	@brief 隐藏
	@return 
	**/
	virtual void	Hide();

	void	TerminateTimer( UINT nTimerId) { KillTimer( nTimerId);}

	/**
	@fn SetShowDelay(int nDelay)
	@brief 当鼠标移动到给定窗口上后延迟显示Tips的时间
	@param	[in] nDelay		延迟时间
	**/
	void	SetShowDelay(int nDelay) { m_nShowDelay = nDelay; };

	/**
	@fn GetShowDelay
	@brief 获取鼠标移动到给定窗口上后延迟显示Tips的时间
	@return	延迟时间
	**/
	const int	GetShowDelay() { return m_nShowDelay;}

	/**
	@fn SetHideDelay
	@brief 当Tips弹出后自动消失的延迟时间
	@param	[in] nDelay		延迟时间, RB_INFINITE 为不自动消失
	@return	延迟时间
	**/
	void	SetHideDelay(int nDelay) { m_nHideDelay = nDelay; };

	/**
	@fn GetHideDelay
	@brief 获取消失的延迟时间
	@return	消失延迟时间
	**/
	const int	GetHideDelay() { return m_nHideDelay;}

	/**
	@fn SetBkgColor
	@brief 获取Tip的背景颜色
	@param	[in] clrBkg		设置的背景色
	**/
	void	SetBkgColor( COLORREF clrBkg);

	/**
	@fn GetBkgColor
	@brief 获取Tip的背景颜色
	**/	
	const COLORREF	GetBkgColor();

	/**
	@fn SetBorderColor
	@brief 设置和获取Tip的边框颜色
	@param	[in] clrBorder	设置的边界颜色
	**/
	void	SetBorderColor( COLORREF clrBorder);

	/**
	@fn GetBorderColor
	@brief 获取Tip的边界颜色
	**/	
	const COLORREF	GetBorderColor();

	/**
	@fn SetTextColor
	@brief 获取Tip的文字颜色
	@param	[in] clrText	设置的字体颜色
	**/
	void	SetTextColor( COLORREF clrText);

	/**
	@fn GetTextColor
	@brief 获取Tip的边界颜色
	**/	
	const COLORREF	GetTextColor();

	/**
	@fn SetTipsText
	@brief 设置Tip的提示文字
	@param	[in] strText	提示文字
	**/
	void	SetTipsText( CString strText);

	/**
	@fn GetTipsText
	@brief 获取Tip的提示文字
	@return	Tips的提示文字
	**/
	const CString	GetTipsText();

	/**
	@fn SetFont
	@brief 设置Tip上的文字字体
	@param	[in] hFont	字体
	**/
	void	SetFont( HFONT hFont);

	/**
	@fn GetFont
	@brief 获取Tip上的文字字体
	**/
	const LOGFONT	GetFont();

	/**
	@fn SetFont
	@brief 设置Tip上的文字字体
	@param	[in] hFont	字体
	**/
	void	SetFitted( BOOL bFitted){m_bFitted = bFitted;}

	/**
	@fn GetFont
	@brief 获取Tip上的文字字体
	**/
	const BOOL 	GetFitted() { return m_bFitted;}
protected:
	BOOL			GetWindowRegion(CDC *pDC, HRGN* hRegion, CSize* Size = NULL);
	BOOL			Create(CWnd *parent);

	afx_msg void	OnPaint();
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg void	OnDestroy();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	UINT		m_nTimer;					//	定时器

	int			m_nShowDelay;				//	延迟显示时间
	int			m_nHideDelay;				//	延迟消失的时间，如果为-1，则不自动消失
	CString		m_strText;					//	提示文字
	CRect       m_rcButtonRect;
	CPoint		m_ptStartPos;				//	弹出坐标

	/**相关颜色属性*/
	COLORREF	m_clrBkg;					//	背景颜色
	COLORREF	m_clrBorder;				//	边框颜色
	COLORREF	m_clrText;					//	显示的字体的颜色
	BOOL		m_bHide;					//	是否处于隐藏状态
	LOGFONT		m_stLogFont;				//	字体的描述信息

	UINT		m_nWidth;					//	Tips的宽度
	BOOL		m_bFitted;					//	宽度是否需要限定在给定的宽度m_nWidth之内，默认是需要限定
};

}}