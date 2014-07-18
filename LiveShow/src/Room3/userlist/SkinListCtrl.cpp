// SkinListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinListCtrl.h"
#include "..\utility\MemDC.h"
//#include "ImageEx.h"
//#include "TipDlg.h"
#include "utility/StringHelper.h"
#include "utility/SystemHelper.h"
#include "ListIconManager.h"
#include "..\UserMessageDefine.h"
#include "..\..\CommonLibrary\include\ui\ImageEx.h"
#include "..\GlobalManager.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVEN_ITEM_COLOR     RGB(197,211,224)
#define ITEM_HEIGHT	29

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl

CSkinListCtrl::CSkinListCtrl()
{
    m_iSelectedItem = -1;//选中项
	std::wstring strFont;
	m_fontItem.CreatePointFont(90,L"宋体",NULL);
	m_bWaitingList = false;
	m_bShowGuest = true;
	std::wstring strPath = common::utility::systemhelper::Get179AppPath() + L"resource\\other\\Room_UserListHover.png";
	m_pHoverImage = new common::ui::ImageEx(strPath.c_str());
	strPath = common::utility::systemhelper::Get179AppPath() + L"resource\\other\\Room_UserListSelect.png";
	m_pSelectImage = new common::ui::ImageEx(strPath.c_str());
	strPath = common::utility::systemhelper::Get179AppPath() + L"resource\\other\\Room_UserListNormal.png";
	m_pNormalImage = new common::ui::ImageEx(strPath.c_str());
	m_nHoverItem = -1;
	memset(m_narrColumnWidth, 0, sizeof(m_narrColumnWidth));
}

CSkinListCtrl::~CSkinListCtrl()
{
	if (m_pHoverImage != NULL)
	{
		delete m_pHoverImage;
		m_pHoverImage = NULL;
	}

	if (m_pSelectImage != NULL)
	{
		delete m_pSelectImage;
		m_pSelectImage = NULL;
	}

	if (m_pNormalImage != NULL)
	{
		delete m_pNormalImage;
		m_pNormalImage = NULL;
	}
	
	if (m_fontItem.GetSafeHandle())
	{
		m_fontItem.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT() 
	ON_NOTIFY_REFLECT(NM_CLICK, &CSkinListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CSkinListCtrl::OnNMRclick)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_HOVER, &CSkinListCtrl::OnNMHover)
	ON_NOTIFY_REFLECT(LVN_ENDSCROLL, &CSkinListCtrl::OnLvnEndScroll)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolNeedText)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl message handlers

void CSkinListCtrl::PreSubclassWindow() 
{
	ModifyStyle(0,LVS_OWNERDRAWFIXED,0);

	CListCtrl::PreSubclassWindow();
}

void CSkinListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{ 
	lpMeasureItemStruct->itemHeight = ITEM_HEIGHT;//设置每一项高度
} 

void CSkinListCtrl::DrawAnimation()
{
	CClientDC dc(this);

	if(dc.GetSafeHdc() == NULL)
	{
		assert(0);
		return;
	}

	CRect rect;
	GetClientRect(rect);

	Gdiplus::Bitmap bmp(GetColumnWidth(0) + GetColumnWidth(1), rect.Height());
	Gdiplus::Graphics graphics(&bmp);

	int top = GetTopIndex();

	int bottom = top + GetCountPerPage();

	if(bottom >= GetItemCount())
	{
		bottom = GetItemCount()-1;
	}

	CRect rcItem;
	GetItemRect(0, rcItem, LVIR_BOUNDS);

	for(int item=top; item<=bottom; item++)
	{
		IUserItem * pUserItem = (IUserItem*)GetItemData(item);	
		if(pUserItem == NULL)
		{
			continue;
		}

		common::ui::ImageEx* pImage = ImageManager::getInstance()->GetIcon(pUserItem->GetListIconIndex(0));
		if(pImage != NULL && pImage->IsAnimatedGIF())
		{
			int nLeft = rcItem.left + 5;
			int nTop = ITEM_HEIGHT*item + (ITEM_HEIGHT - pImage->GetHeight())/2 + rcItem.top;
			graphics.DrawImage(pImage->GetCurImage(), nLeft, nTop, pImage->GetWidth(), pImage->GetHeight());
		}

		if (pUserItem->GetUserRichLevel() > 0)
		{
			pImage = ImageManager::getInstance()->GetRichImg(pUserItem->GetUserRichLevel());
			if (pImage != NULL && pImage->IsAnimatedGIF())
			{
				int nLeft = rcItem.left + GetColumnWidth(0) + 2;
				int nTop = ITEM_HEIGHT*item + (ITEM_HEIGHT - pImage->GetHeight())/2 + rcItem.top;
				graphics.DrawImage(pImage->GetCurImage(), nLeft, nTop, pImage->GetWidth(), pImage->GetHeight());
			}
		}
	}

	Gdiplus::Graphics graph(dc.GetSafeHdc());
	graph.DrawImage(&bmp, 0, 0, bmp.GetWidth(), bmp.GetHeight());
}

void CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	int nItem = lpDIS->itemID;

	if(nItem == -1)
		return ;

	CRect itemRect = lpDIS->rcItem;

	ImageAttributes imAtt;
	imAtt.SetColorKey(Color(255,0,255), Color(255,0,255));
	imAtt.SetWrapMode(WrapModeTileFlipXY);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	pDC->SetBkMode(TRANSPARENT);

	Graphics graphics(lpDIS->hDC);

	graphics.DrawImage(m_pNormalImage, RectF(itemRect.left, itemRect.top, itemRect.Width(), itemRect.Height()), 0, 0, m_pNormalImage->GetWidth(), m_pNormalImage->GetHeight(), UnitPixel, &imAtt);

	if ((lpDIS->itemAction | ODA_SELECT) && (lpDIS->itemState & ODS_SELECTED))
	{
		graphics.DrawImage(m_pSelectImage, RectF(itemRect.left, itemRect.top, itemRect.Width(), itemRect.Height()), 0, 0, m_pHoverImage->GetWidth(), m_pHoverImage->GetHeight(), UnitPixel, &imAtt);
	}
	else
	{
		if (nItem == m_nHoverItem)
		{
			graphics.DrawImage(m_pHoverImage, RectF(itemRect.left, itemRect.top, itemRect.Width(), itemRect.Height()), 0, 0, m_pHoverImage->GetWidth(), m_pHoverImage->GetHeight(), UnitPixel, &imAtt);
		}
	}

	int columnCount = GetHeaderCtrl()->GetItemCount();//获得总列数
    
	int preRight = itemRect.left;
	for (int i = 0; i < columnCount; ++i)//前面4列才有图标
	{   
		CRect rc(0,0,0,0);
		rc.left = preRight;
		rc.right = rc.left + m_narrColumnWidth[i];
		rc.top = itemRect.top;
		rc.bottom = itemRect.bottom;

		CString sText = GetItemText(nItem, i);

		IUserItem * pUserItem = (IUserItem*)GetItemData(nItem);
		CRect textRc = rc;
		CRect imgRc = rc;
		common::ui::ImageEx* pImage = NULL;

		if (pUserItem != NULL)
		{
			if (i == 2 && m_bWaitingList)
			{
				pImage = ImageManager::getInstance()->GetIcon(USERLIST_ICON_WAIT_MIC);
			}
			else if (i == 1)
			{
				if (pUserItem->GetUserRichLevel() > 0)
				{
					pImage = ImageManager::getInstance()->GetRichImg(pUserItem->GetUserRichLevel());
				}
			}
			else
			{
				pImage = ImageManager::getInstance()->GetIcon(pUserItem->GetListIconIndex(i));
			}		
		}
		else	//游客(10)
		{
			if (i == 2)
			{
				if (m_bShowGuest)
					pImage = ImageManager::getInstance()->GetIcon(USERLIST_ICON_GUEST_HIDE);
				else
					pImage = ImageManager::getInstance()->GetIcon(USERLIST_ICON_GUEST_SHOW);
			}
			else if (i == 3)
			{
				pImage = ImageManager::getInstance()->GetIcon(USERLIST_ICON_GUEST);
			}
		}

		if (pImage != NULL && i < 4)//取得VIP、等级、麦、相册图标的动态图标，盖章不在这里画,i = 4时是取得盖章的图片
		{
			if (i == 0)
			{
				imgRc.left += 5;
			}
			else
			{
				imgRc.left += 2;
			}
			
			imgRc.top += (rc.Height() - pImage->GetHeight())/2; //注意，要从0开始加，而不是rc.top.下同
			
			graphics.DrawImage(pImage, imgRc.left, imgRc.top, pImage->GetWidth(), pImage->GetHeight());
		}

		if (i < 4 && pImage != NULL)//昵称与盖章同一列
		{
			textRc.left = textRc.left + pImage->GetWidth();
		}

		if (i == 4)
		{
			textRc.left += 4;
		}
		
		pDC->SelectObject(m_fontItem);//设置字体

		if (pUserItem != NULL && pUserItem->GetUserVipLevel() > core::VIPLEVEL_HONOR)
		{
			pDC->SetTextColor(RGB(255,0,0));
		}
		else
		{
			pDC->SetTextColor(RGB(68,68,68));
		}

		if (3 == i)
		{
			pDC->SetTextColor(RGB(99, 140, 12));
		}
		
		pDC->DrawText(sText,sText.GetLength(),&textRc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		if(4 == i)//这里画盖章
		{
			if(pUserItem != NULL)
			{
				pImage = ImageManager::getInstance()->GetStamp(pUserItem->GetUserStampID());
			}
			else
			{
				pImage = NULL;
			}
			if (pImage != NULL)
			{
				int h = pImage->GetHeight();
				CRect stampRc = rc;
				stampRc.left = stampRc.right - pImage->GetWidth() - 2;
				stampRc.top += (rc.Height() - pImage->GetHeight())/2;
				graphics.DrawImage(pImage,stampRc.left,stampRc.top,pImage->GetWidth(),pImage->GetHeight());
			}
		}

		if (5==i)
		{
			if (pUserItem != NULL && pUserItem->GetWealthyIndex() != 0)
			{
				pImage = ImageManager::getInstance()->GetWealthyImg(pUserItem->GetWealthyIndex());
				if (pImage != NULL)
				{
					graphics.DrawImage(pImage, rc.left-4, rc.top, pImage->GetWidth(), pImage->GetHeight());
				}
			}
		}

		preRight = rc.right;
	}
}

BOOL CSkinListCtrl::DeleteItem(int nItem)
{
	IUserItem * pUserItem = (IUserItem*)GetItemData(nItem);

	Invalidate(TRUE);

	return __super::DeleteItem(nItem);
}

void CSkinListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	m_iSelectedItem = pMListView->iItem;//获取选中项

    if (m_iSelectedItem >= 0 && m_iSelectedItem < (this->GetItemCount()))
    {
		IUserItem *pUserItem = (IUserItem*)GetItemData( m_iSelectedItem ) ;
		if ( pUserItem != NULL )
		{
			CWnd * pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				pParentWnd->SendMessage(WM_CLICK_SHORTCUT, 0, (LPARAM)pUserItem);
			}
		}
    }
}


BOOL CSkinListCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	m_iSelectedItem = pMListView->iItem;//获取选中项

	return FALSE;
}


BOOL CSkinListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CSkinListCtrl::OnNMHover(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;//*pResult若为0，则鼠标移至ITEM时会选中，非0则不会

	return;
}

void CSkinListCtrl::OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 5.5 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0560。
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	Invalidate(FALSE);
}

BOOL CSkinListCtrl::OnToolNeedText( UINT id, NMHDR* pNMHDR, LRESULT* pResult )
{
	CRect rc;
	GetWindowRect(&rc);
	CPoint pot;
	GetCursorPos(&pot);
	pot.x = pot.x - rc.left;
	pot.y = pot.y - rc.top;

	LVHITTESTINFO LVHTestInfo;
	LVHTestInfo.pt = pot;
	HitTest(&LVHTestInfo);
	int iItem = LVHTestInfo.iItem;//获得行
	SubItemHitTest(&LVHTestInfo);
	int iSubItem = LVHTestInfo.iSubItem;//获得列	

	if (iItem >= 0 && iItem < GetItemCount())
	{
		IUserItem * pUserItem = (IUserItem*)GetItemData(iItem);//获取该选中项信息

		if (pUserItem != NULL && pUserItem->GetUserUIN() != 0)
		{
			int columnCount = GetHeaderCtrl()->GetItemCount();//获得总列数
			if (iSubItem >= 0 && iSubItem < columnCount)
			{	
				if (iSubItem < 4)
				{
					std::wstring strTip = ListIconManager::GetIconTipByIndex(pUserItem->GetListIconIndex(iSubItem));
					if (strTip != L"")
					{
						TOOLTIPTEXTW* pTT = (TOOLTIPTEXT*) pNMHDR;
						wcscpy(pTT->szText,strTip.c_str());
					}
				}
				else if (4 == iSubItem || 5 == iSubItem)//昵称
				{//显示相关信息
					std::wstring strName = pUserItem->GetUserName() + L"(" + common::utility::stringhelper::IntToString(pUserItem->GetUser179ID()) + L")";
					TOOLTIPTEXTW* pTT = (TOOLTIPTEXT*) pNMHDR;
					wcscpy(pTT->szText,strName.c_str());
				}
			}
		}
	}

	*pResult = 1;

	return TRUE;
}

void CSkinListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitinfo={0};
	hitinfo.flags = nFlags;
	hitinfo.pt = point;
	HitTest(&hitinfo);

	CRect rc, rcOld, rcNew;
	GetClientRect(&rc);

	//if (hitinfo.iItem >= 0)
	//{
		if (m_nHoverItem != hitinfo.iItem)
		{
			if (m_nHoverItem >= 0)
			{
				GetItemRect(m_nHoverItem, &rcOld, LVIR_BOUNDS);
				InvalidateRect(rcOld);
			}
			if (hitinfo.iItem >= 0)
			{
				GetItemRect(hitinfo.iItem, &rcNew, LVIR_BOUNDS);
				InvalidateRect(rcNew);
			}

			m_nHoverItem = hitinfo.iItem;
		}
	//}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags  = TME_LEAVE ;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	::_TrackMouseEvent(&tme);

	CListCtrl::OnMouseMove(nFlags, point);
}

LRESULT CSkinListCtrl::OnMouseLeave( WPARAM wParam,LPARAM lParam )
{
	CRect rc;
	if (m_nHoverItem >= 0)
	{
		GetItemRect(m_nHoverItem, &rc, LVIR_BOUNDS);
		InvalidateRect(rc);

		m_nHoverItem = -1;
	}
	return 0;
}

void CSkinListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient, rcClip, rcHeader;
	GetClientRect(&rcClient);

	GetHeaderCtrl()->GetWindowRect(&rcHeader);
	dc.GetClipBox(&rcClip);

	CDC MemDC;
	CBitmap MemBitmp, *pOldBitmap;

	MemDC.CreateCompatibleDC(&dc);
	int nMode = MemDC.SetMapMode(dc.GetMapMode());

	MemBitmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	pOldBitmap = MemDC.SelectObject(&MemBitmp);
	MemDC.FillSolidRect(&rcClient, EVEN_ITEM_COLOR);

	CRgn rgn;
	rgn.CreateRectRgn(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
	MemDC.SelectClipRgn(&rgn);

	DefWindowProc(WM_PAINT, (WPARAM)MemDC.m_hDC, (LPARAM)0);
	MemDC.SelectClipRgn(NULL);

	if (rcClip.top < rcHeader.Height())
	{
		rcClip.top = rcHeader.Height();
	}

	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &MemDC, rcClip.left, rcClip.top, SRCCOPY);

	MemDC.SetMapMode(nMode);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	MemBitmp.DeleteObject();
	rgn.DeleteObject();
}