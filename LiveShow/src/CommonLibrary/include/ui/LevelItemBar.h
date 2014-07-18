#pragma once
#include "afxwin.h"
#include <string>
#include "ItemBar.h"

///////////////////////////////////////////////////////////
//	类名: CToolLevelItem
//  类功能描述：
//  	Bar中的Item用以描述51的等级数目

class CToolLevelItem	:
	public CToolItem
{
public:
	CToolLevelItem(void);
	CToolLevelItem( CToolLevelItem& barItem);
	CToolLevelItem( CToolLevelItem* pbarItem);
	virtual ~CToolLevelItem(void);


	/**
	@fn Draw( CDC* pDC)
	@brief Item的绘制接口
	*/	
	virtual void	Draw( CDC* pDC) {} ;

	/**
	@fn 设置当前Item的等级数目
	@brief 等级设置接口
	@param [in] nLevel 等级
	*/
	virtual	void	SetLevel( UINT nLevel) {};


private:

	std::wstring				m_strIconPath;		//	图标的路径，目前都是以PNG格式
};
