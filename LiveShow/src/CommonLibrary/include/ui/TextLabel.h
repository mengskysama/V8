#pragma once

#include "stdafx.h"
#include "Common.h"
#include "afx.h"
#include <string>

using namespace std;

///////////////////////////////////////////////////////////
//	类名: CTextLabel
//  类功能描述：
//  	文本标签，可以设置该标签的颜色，背景。
//		给定一个矩形区域和文本，该区域会自动进行省略处理
//		该类目前在用户头像tips中广泛使用


class COMMON_INTERFACE CTextLabel
{
public:
	CTextLabel();
	virtual ~CTextLabel();

	wstring GetSplitterString( wstring wstrText);

	/**
	@fn SetDC( CDC* pDC)
	@brief 设置标签所在的DC
	@param	[in] pDC	DC环境
	**/
	void	SetDC( CDC* pDC) { m_pDC = pDC;}

	/**
	@fn SetFont( CFont* pFont)
	@brief 设置标签中文本的颜色
	@param	[in] pDC	DC环境
	**/
	void	SetFont( CFont* pFont) { m_pFont = pFont;}

	/**
	@fn SetText( wstring wstrText)
	@brief 设置标签中文本内容
	@param	[in] wstrText	文本内容
	**/
	void	SetText( wstring wstrText) { m_strText = wstrText;}

	/**
	@fn SetColor( COLORREF clrText)
	@brief 设置标签中文本的颜色
	@param	[in] wstrText	文本内容
	**/
	void	SetColor( COLORREF clrText) { m_clrText = clrText;}

	/**
	@fn SetUnderLine( BOOL bUnderline)
	@brief 设置标签中文本是否需要添加下划线
	@param	[in] bUnderline	TRUE表示需要下划线，FALSE，不需要
	**/
	void	SetUnderLine( BOOL bUnderline = TRUE) { m_bUnderline = bUnderline;}

	/**
	@fn SetLineSpace( UINT nLineSpace)
	@brief 设置行与行之间的间隔距离，默认为0
	@param	[in] nLineSpace	行距
	**/
	void	SetLineSpace( UINT nLineSpace) { m_nLineSpace = nLineSpace;}

	/**
	@fn GetLineSpace()
	@brief 获取行与行之间的距离
	@param	[in] nLineSpace	行距
	**/
	UINT	GetLineSpace() { return m_nLineSpace;}

	/**
	@fn SetRect( CRect rect)
	@brief 设置该标签所占据的矩形区域
	@param	[in] rect	行距
	**/
	void	SetRect( CRect rect) { m_rcText = rect;}

	/**
	@fn Draw()
	@brief 绘制
	@param	[in] rect	行距
	**/	
	void	Draw();

private:

	void	DrawLine( BOOL bDraw = TRUE);
	UINT	GetCharWidthInfo( wstring strText );
	UINT	GetCharHeightInfo( wstring strText);
	wstring	GetFittableStringFromFront( wstring wstrText, UINT nWidth, int nBeginIndex, int& nEndIndex);

	wstring		m_strText;			//	Label中的文字
	CRect		m_rcText;			//	Label的矩形区域
	CDC*		m_pDC;				//	所在的设备环境
	CFont*		m_pFont;			//	字体设置
	COLORREF	m_clrText;			//	文字的颜色
	BOOL		m_bUnderline;		//	是否要绘制下划线
	UINT		m_nLineSpace;		//	行与行之间的间距，默认为0
};