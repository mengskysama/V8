// ImageListCtrl.cpp : 实现文件
//
#include "stdafx.h"
#include "../../../../include/Skin_i.h"
#include "ui/ImageListCtrl.h"
#include "ui/ImageImpl.h"

#define	THUMBNAIL_WIDTH		40
#define	THUMBNAIL_HEIGHT	40

#define ITEM_WIDTH          60    
#define ITEM_HEIGHT         60

// CImageListCtrl

namespace common
{
	namespace ui
	{
		IMPLEMENT_DYNAMIC(CImageListCtrl, CListCtrl)

		CImageListCtrl::CImageListCtrl()
		{
			m_rgbItemBorderNormal=RGB(191,224,239);
			m_rgbItemBorderSelect=RGB(10,36,106);
			m_iLineNormalWidth=1;
			m_iLineSelectWidth=2;

			m_sizeItem.cx=ITEM_WIDTH;
			m_sizeItem.cy=ITEM_HEIGHT;

			m_sizeImage.cx=THUMBNAIL_WIDTH;
			m_sizeImage.cy=THUMBNAIL_HEIGHT;

			m_iImageToBorderSpacing=4;
		}

		CImageListCtrl::~CImageListCtrl()
		{
		}


		BEGIN_MESSAGE_MAP(CImageListCtrl, CListCtrl)
			ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,OnCustomDraw)
			ON_WM_SIZE()
			ON_WM_VSCROLL()
			ON_WM_DESTROY()
		END_MESSAGE_MAP()

		// CImageListCtrl 消息处理程序

		BOOL CImageListCtrl::PreCreateWindow(CREATESTRUCT& cs)
		{
			cs.style &=~WS_VSCROLL; 
			return CListCtrl::PreCreateWindow(cs);
		}

		void CImageListCtrl::Initialize(const CSize& sizeItem,const CSize& sizeImage,int iImageToBorderSapcing,
												COLORREF rgbNomalBorder,COLORREF rgbSelectBorder,
												int iLineNormalWidth,int iLineSelectWidth)
		{
			m_sizeItem=sizeItem;
			m_sizeImage=sizeImage;
			m_iImageToBorderSpacing=iImageToBorderSapcing;
			m_rgbItemBorderSelect=rgbSelectBorder;
			m_rgbItemBorderNormal=rgbNomalBorder;
			m_iLineNormalWidth=iLineNormalWidth;
			m_iLineSelectWidth=iLineSelectWidth;

			SetIconSpacing(sizeItem.cx,sizeItem.cy); 
			m_imageList.Create(sizeItem.cx-2,sizeItem.cy-21, ILC_COLOR24, 0, 1);

			SetImageList(&m_imageList, LVSIL_NORMAL);
			
			SetBkColor(RGB(255,255,255));//humy
		}

		int CImageListCtrl::AddImage(LPCTSTR strFilename,DWORD_PTR dwData,BOOL bIsResample,int iResampleWidth,int iResampleHeight)
		{
			ImageCache *pCache=new ImageCache;
			pCache->dwData=dwData;
			pCache->strPicPath=strFilename;
			
			CImageImpl* pImage=new CImageImpl(NULL);
			pCache->imageWrapper.Attach(pImage);
			if (!pImage->Load(strFilename))
				return -1;

			if(bIsResample && iResampleWidth>0 && iResampleHeight>0)
				pImage->Resample(iResampleWidth,iResampleHeight,3);
			
			int i=InsertItem(GetItemCount(),L"",0);
			SetItemData(i,(DWORD_PTR)pCache);
			return i;
		}

		int CImageListCtrl::AddImage(const CImageWrapper& imageWrapper,DWORD_PTR dwData)
		{
			ImageCache *pCache=new ImageCache;
			pCache->dwData=dwData;
			pCache->strPicPath=L"";
			pCache->imageWrapper=imageWrapper;
	
			int i=InsertItem(GetItemCount(),L"",0);
			SetItemData(i,(DWORD_PTR)pCache);
			return i;
		}

		void CImageListCtrl::ClearAllImage()
		{
			int iCount=GetItemCount();
			for (int i=0;i<iCount;i++)
			{
				ImageCache *pCache=(ImageCache*)GetItemData(i);
				if (pCache)
				{
					//清除CImageWrapper数据，让引用计数减一
					pCache->imageWrapper=NULL;
					delete pCache;
				}
			}
			DeleteAllItems();
		}

		void CImageListCtrl::OnCustomDraw( NMHDR*   pNMHDR,   LRESULT*   pResult   )
		{
			NMLVCUSTOMDRAW* pLVCD = reinterpret_cast <NMLVCUSTOMDRAW*> (   pNMHDR   ); 

			*pResult= 0; 


			//   Request   item-specific   notifications   if   this   is   the 
			//   beginning   of   the   paint   cycle. 
			if   (CDDS_PREPAINT== pLVCD-> nmcd.dwDrawStage) 
			{ 
				*pResult = CDRF_NOTIFYITEMDRAW; 
			} 
			else if(CDDS_ITEMPREPAINT == pLVCD-> nmcd.dwDrawStage) 
			{ 
				//   This   is   the   beginning   of   an   item 's   paint   cycle. 
				LVITEM       rItem; 
				int          nItem   =   static_cast <int> (   pLVCD-> nmcd.dwItemSpec   ); 
				CDC*         pDC       =   CDC::FromHandle   (   pLVCD-> nmcd.hdc   ); 
				BOOL         bListHasFocus; 
				CRect        rcItem; 
				CRect        rcText; 
				CString      sText; 
				UINT         uFormat; 

				bListHasFocus = (GetSafeHwnd()==::GetFocus()); 

				//   Get   the   image   index   and   selected/focused   state   of   the 
				//   item   being   drawn. 
				ZeroMemory(&rItem,sizeof(LVITEM)); 
				rItem.mask = LVIF_IMAGE|LVIF_STATE; 
				rItem.iItem = nItem; 
				rItem.stateMask= LVIS_SELECTED|LVIS_FOCUSED; 
				GetItem   (&rItem); 

				//   Get   the   rect   that   holds   the   item 's   icon. 
				GetItemRect(nItem,&rcItem, LVIR_BOUNDS);  
				//   Draw   the   icon. 
				uFormat=ILD_TRANSPARENT;


				//背景设为白色
				pDC->FillSolidRect(rcItem,RGB(255,255,255));

				int iPicOffsetX= (rcItem.Width() - m_sizeImage.cx)/2;
				int iPicOffsetY= (rcItem.Height() - m_sizeImage.cy)/2;

				CPoint ptPic=rcItem.TopLeft();
				ptPic.Offset(iPicOffsetX,iPicOffsetY);
				ImageCache *pCache=(ImageCache*)GetItemData(rItem.iItem);
				CRect rcPic(ptPic,m_sizeImage);


				CRect rcBorder=rcPic;
				rcBorder.InflateRect(m_iImageToBorderSpacing,m_iImageToBorderSpacing);
				
				if (rItem.state & LVIS_SELECTED) 
				{ 
					int iOffset= m_iLineSelectWidth%2==0 ? 1:0;
					rcBorder.InflateRect(0,0,iOffset,iOffset);
					DrawBorder(pDC,rcBorder,m_rgbItemBorderSelect,m_iLineSelectWidth);
				} 
				else 
				{ 
					int iOffset= m_iLineNormalWidth%2==0 ? 1:0;
					rcBorder.InflateRect(0,0,iOffset,iOffset);
					DrawBorder(pDC,rcBorder,m_rgbItemBorderNormal,m_iLineNormalWidth); 
				} 

				pCache->imageWrapper.Render(pDC->m_hDC,rcPic);

				*pResult   =   CDRF_SKIPDEFAULT;         //   We 've   painted   everything. 
			} 
		}

		void CImageListCtrl::DrawBorder(CDC* pDC,const CRect& rcBorder, COLORREF color,int iWidth)
		{
			CPen newPen(PS_SOLID,iWidth,color);
			CPen *pOldPen=pDC->SelectObject(&newPen);
			pDC->Rectangle(rcBorder);
			pDC->SelectObject(pOldPen);
		}

		void CImageListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
		{
			// TODO: Add your message handler code here and/or call default

			CListCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
		}

		void CImageListCtrl::OnSize(UINT nType,int cx,int cy )
		{
			CListCtrl::OnSize(nType, cx, cy);
		}

		BOOL CImageListCtrl::GetItemInfo(int iItem,CString& strFileName,DWORD_PTR& dwData)
		{
			ImageCache *pCache= (ImageCache*)GetItemData(iItem);
			if(pCache==NULL)
				return FALSE;
			dwData=pCache->dwData;
			strFileName=pCache->strPicPath;
			return TRUE;
		}

		void CImageListCtrl::OnDestroy()
		{
			ClearAllImage();
			CListCtrl::OnDestroy();
		}

	}
}