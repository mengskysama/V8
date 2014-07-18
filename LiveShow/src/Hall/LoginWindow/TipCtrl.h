#pragma once

#include "afx.h"
#include "afxtempl.h"
#include "../CommonLibrary/include/ui/SimpleTipCtrl.h"

using namespace common::ui;

#define RB_INFINITE	-1	

class  CTipCtrl : 
	public CSimpleTipCtrl
{
public:
	CTipCtrl();
	virtual ~CTipCtrl();

	/**
	@fn SetIcon
	@brief 设置和获取图
	@param	[in] hIcon	设置的图标
	@param	[in] iconSize	图标的大小，默认为16,16
	**/
	void	SetIcon( HICON hIcon, CSize iconSiz = (16,16));

	/**
	@fn SetShowIcon
	@brief 是否显示图标
	@param	[in] bShowIcon	true，需要显示图标，false，不需要显示图标
	**/
	void	SetShowIcon( BOOL bShowIcon) { m_bShowIcon = bShowIcon;}

	/**
	@fn SetLeft
	@brief 设置提示的底端箭头偏左还是偏右
	@param	[in] bLeft		为TRUE为偏左显示
	**/
	void	SetLeft(BOOL bLeft);

	/**
	@fn SetButtonText
	@brief 设置按钮的文本
	@param	[in] strText		按钮的文本，为空则没有提示按钮
	**/
	void	SetButtonText(CString strText);

	/**
	@fn GetButtonText
	@brief 获取按钮的文本
	**/
	const CString   GetButtonText();

	/**
	@fn SetClickClose
	@brief 点击的时候是否关闭
	@param	[in] strText		按钮的文本，为空则没有提示按钮
	**/
	void	SetClickClose(BOOL bClose);
    const BOOL   GetClickClose();

	/**
	@fn SetAssociateWnd
	@brief 设置与该tip关联的窗口
	@param	[in] strText		按钮的文本，为空则没有提示按钮
	*/
	void	SetAssociateWnd( CWnd* pAssociateWnd);

	/**
	@fn GetAssociateWnd
	@brief 获取与该tip关联的窗口
	@param	[in] strText		按钮的文本，为空则没有提示按钮
	*/
	const CWnd*   GetAssociateWnd();

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
	typedef struct
	{
		CString	szText;						//	Tooltip text
		HICON	hIcon;						//	Tooltip icon

	} TipToolInfo;

	CString     m_strButtonText;			//	如果Tips上显示button，则为button的文字
    BOOL        m_bClickClose;              //  鼠标点击时候是否消失
	HICON		m_hIcon;					//	Tips上的图标
	CSize		m_IconSize;					//	图标的大小
	BOOL		m_bShowIcon;				//	是否显示图标
	BOOL		m_bLeft;					//	箭头是否在左侧

	CFont*		m_pFont;
	CWnd*		m_pAssociateWnd;			//	触发该控件的窗口
};