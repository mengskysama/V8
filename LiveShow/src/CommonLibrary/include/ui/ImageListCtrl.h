/*
@file:		ImageListCtrl.h
@purpose:	显示图片缩略图的ListCtrl，基于MFC的CListCtrl
@version:	1.0 2010.01.15 create zhangjb
*/

#pragma once
#include "../Common.h"
#include "ImageWrapper.h"
// CImageListCtrl

namespace common
{
	namespace ui
	{
		class COMMON_INTERFACE CImageListCtrl : public CListCtrl
		{
			DECLARE_DYNAMIC(CImageListCtrl)

		public:

			struct  ImageCache
			{
				ImageCache()
				{
					dwData=0;
					strPicPath=L"";
				}
				DWORD_PTR dwData;
				CImageWrapper imageWrapper;
				CString strPicPath;
			};

			/*
			/*function 构造函数
			/*@param 
			/*return
			*/
			CImageListCtrl();
			
			/*
			/*function 析构函数
			/*@param 
			/*return
			*/
			virtual ~CImageListCtrl();

			/*
			/*function 初始化控件的各个属性
			/*@param [in] sizeItem 每个item的size ，包括了item旁边的空白部分
			/*@param [in] sizeImage 每个显示的Image的size。
			/*@param [in] iImageToBorderSpacing Image到边框的距离。
			/*@param [in] rgbNomalBorder 边框未选中时，边框的颜色。
			/*@param [in] rgbSelectBorder 选中边框时，边框的颜色。
			/*@param [in] iLineNormalWidth 边框未选中时，边框的宽度。
			/*@param [in] iLineSelectWidth 边框选中时，边框的宽度。
			/*return 无
			*/
			void Initialize(const CSize& sizeItem,const CSize& sizeImage,int iImageToBorderSpacing,
							COLORREF rgbNomalBorder=RGB(191,224,239),COLORREF rgbSelectBorder=RGB(10,36,106),
							int iLineNormalWidth=1,int iLineSelectWidth=2);

			/*
			/*function 添加一个图片
			/*@param [in] iIndex          图片的序号（主要用于标识固定头像）
			/*@param [in] strFilename     图片文件的路径
			/*@param [in] dwData          附加的信息，主要是用来存放指针
			/*@param [in] bIsReample      图像大小是否缩放
			/*@param [in] iReampleWidth   缩放缩放的宽度
			/*@param [in] iReampleHeight  缩放缩放的高度
			/*return 成功返回item序号,失败返回 -1
			*/
			int AddImage(LPCTSTR strFilename,DWORD_PTR dwData,
				         BOOL bIsResample=FALSE,int iResampleWidth=0,
						 int iResampleHeight=0);

			/*function 添加一个图片
			/*@param [in] iIndex 图片的序号（主要用于标识固定头像）
			/*@param [in] imageWrapper 图像的封装对象
			/*@param [in] dwData      附加的信息，主要是用来存放指针
			/*return 成功返回item序号,失败返回 -1
			*/
			int AddImage(const CImageWrapper& imageWrapper,DWORD_PTR dwData);

			/*
			/*function 清除所有的图片item
			/*@param 无
			/*return
			*/
			void ClearAllImage();

			/*
			/*function 获取Item的相关信息
			/*@param [in] iItem 控件中Item的序号
			/*@param [out] strFileName 图片的路径
			/*@param [out] dwData 图片的序号
			/*return 成功返回TRUE，失败返回FALSE
			*/
			BOOL GetItemInfo(int iItem,CString& strFileName,DWORD_PTR& dwData);

		protected:

			CImageList m_imageList;
			COLORREF m_rgbItemBorderNormal;
			COLORREF m_rgbItemBorderSelect;

			CSize m_sizeItem,m_sizeImage;
			int m_iImageToBorderSpacing;
			int m_iLineNormalWidth;
			int m_iLineSelectWidth;


			BOOL PreCreateWindow(CREATESTRUCT& cs);
			afx_msg void OnCustomDraw( NMHDR*   pNMHDR,   LRESULT*   pResult   );
			DECLARE_MESSAGE_MAP()
			void DrawBorder(CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth);

			afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
			afx_msg void OnSize(UINT nType,int cx,int cy );
			afx_msg void OnDestroy();
		};
	}
}


