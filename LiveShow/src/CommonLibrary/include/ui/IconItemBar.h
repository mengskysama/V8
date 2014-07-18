#pragma once
#include "afxwin.h"
#include <vector>
#include <string>
#include "../Common.h"
#include "../../include/ui/ItemBar.h"

namespace common { namespace ui
{

///////////////////////////////////////////////////////////
//	类名: CToolIconItem
//  类功能描述：
//  	Bar中的每一个Item是一个图标

class COMMON_INTERFACE CToolIconItem	:
	public CToolItem
{
public:
	CToolIconItem(void);
	virtual ~CToolIconItem(void){};

	/**
	@fn Draw( CDC* pDC)
	@brief Item的绘制接口
	*/	
	virtual void	Draw( CDC* pDC);

	/**
	@fn 设置当前Item的Icon
	@brief Item的绘制接口
	@param [in] strIconPath Icon图标文件的路径
	*/
	virtual	void	SetIcon( std::wstring strIconPath) ;

	/**
	@fn 获取当前Item对应的图标路径
	@brief Item的绘制接口
	@return Icon图标文件的路径
	*/
	std::wstring	GetIconPath(){return m_strIconPath;}


protected:

	std::wstring				m_strIconPath;		//	图标的路径，目前都是以PNG格式
};

}}