#include "stdafx.h"
#include "ximage.h"
#include "../../include/ui/IconItemBar.h"
#include "../../include/utility/SystemHelper.h"
#include "../../include/ui/UIHelper.h"


namespace common { namespace ui
{

///////////////////////////////////////////////////////////
//	类名: CToolIconItem
//  类功能描述：
//  	Bar中的每一个Item是一个图标

CToolIconItem::CToolIconItem()
{

}

void	
CToolIconItem::Draw( CDC* pDC)
{
	__super::Draw(pDC);

	if (m_strIconPath != L"")
	{
		CRect rcIcon( m_Rect);
		rcIcon.DeflateRect(1,1);

		std::wstring strPath = common::utility::systemhelper::GetModulePath() + L"skin\\internal\\";
		strPath += m_strIconPath.c_str();

		CxImage image;
		image.LoadW( strPath.c_str(), CXIMAGE_FORMAT_BMP);
		image.SetTransColor(CxImage::RGBtoRGBQUAD(RGB(255, 0, 255)));
		image.SetTransIndex(0);
		image.Tile( pDC->GetSafeHdc(), rcIcon);
		image.SetTransIndex(-1);
	}
}

void	
CToolIconItem::SetIcon( std::wstring strIconPath)
{
	m_strIconPath	=	strIconPath;
}

}}