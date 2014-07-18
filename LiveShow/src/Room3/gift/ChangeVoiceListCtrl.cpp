// ChangeVoiceListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ChangeVoiceListCtrl.h"
#include "ChangeVoiceDlg.h"
#include "GiftManager.h"
#include "utility/SystemHelper.h"

#define	THUMBNAIL_WIDTH		40
#define	THUMBNAIL_HEIGHT	40


// CChangeVoiceListCtrl

IMPLEMENT_DYNAMIC(CChangeVoiceListCtrl, CListCtrl)

CChangeVoiceListCtrl::CChangeVoiceListCtrl()
{
	m_lastToolTip = -1;
	m_nHoverItem = -1;
	m_pHoverImage = NULL;
	m_font.CreatePointFont(100,L"宋体",NULL);
}

CChangeVoiceListCtrl::~CChangeVoiceListCtrl()
{
	std::vector<CxImage*>::iterator iter ;
	for ( iter = m_pVectImgs.begin() ; iter != m_pVectImgs.end() ; ++iter )
	{
		CxImage* pImage = *iter ;
		delete pImage ;
	}
	m_pVectImgs.clear();

	if (m_pHoverImage != NULL)
	{
		delete m_pHoverImage;
		m_pHoverImage = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChangeVoiceListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CChangeVoiceListCtrl::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CChangeVoiceListCtrl::OnNMClick)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChangeVoiceListCtrl 消息处理程序



void CChangeVoiceListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//   Request   item-specific   notifications   if   this   is   the 
	//   beginning   of   the   paint   cycle. 
	if (CDDS_PREPAINT == pNMCD->dwDrawStage) 
	{ 
		*pResult = CDRF_NOTIFYITEMDRAW; 
	} 
	else if(CDDS_ITEMPREPAINT == pNMCD->dwDrawStage) 
	{ 
		//   This   is   the   beginning   of   an   item 's   paint   cycle. 
		LVITEM       rItem; 
		int          nItem   =   static_cast <int> (pNMCD->dwItemSpec);
		CDC*         pDC     =   CDC::FromHandle   (pNMCD->hdc);
		COLORREF     crBkgnd; 
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
		GetItemRect(nItem,&rcItem, LVIR_ICON); 
		//rcItem.OffsetRect(2,5);
		//   Draw   the   icon. 
		uFormat=ILD_TRANSPARENT; 

		pDC->SelectObject(m_font);
		pDC->FillSolidRect(rcItem,RGB(255,255,255));

		if (nItem == m_nHoverItem && m_pHoverImage != NULL)
		{
			CRect rcHover;
			rcHover.top = rcItem.top;
			rcHover.left = rcItem.left;
			rcHover.right = rcHover.left + m_pHoverImage->GetWidth();
			rcHover.bottom = rcHover.top + m_pHoverImage->GetHeight();

			m_pHoverImage->Draw(pDC->m_hDC,rcHover,NULL,true);
		}

		int iPicOffsetX= (rcItem.Width() - THUMBNAIL_WIDTH)/2;
		int iPicOffsetY= (rcItem.Height() - THUMBNAIL_HEIGHT)/2;

		CPoint ptPic=rcItem.TopLeft();
		ptPic.Offset(iPicOffsetX,iPicOffsetY);

		if (rItem.iItem>=m_pVectImgs.size())
		{
			return;
		}
		CxImage * pItemImg = m_pVectImgs[rItem.iItem];
		if (pItemImg == NULL)
		{
			return;
		}
		CRect rcPic(ptPic,CSize(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT));

		CRect rcBk = rcPic;
		rcBk.InflateRect(2,2);
		//pDC->FillSolidRect(rcBk,RGB(255,255,255));
		CRect rcReal;
		rcReal.top = rcItem.top;
		rcReal.left = rcItem.left;
		rcReal.right = rcReal.left + pItemImg->GetWidth();
		rcReal.bottom = rcReal.top + pItemImg->GetHeight();
		//Resize(pItemImg->GetWidth(),pItemImg->GetHeight(),rcPic,rcReal);

		pItemImg->Draw(pDC->m_hDC,rcReal,NULL,true);

		//rcText = rcItem;
		//rcText.top = rcItem.bottom;
		//rcText.bottom = rcText.top + 18;

		//IGiftItem * pGiftItem = (IGiftItem*)GetItemData(nItem);
		//sText = pGiftItem->GetName().c_str();

		//pDC->DrawText(sText,sText.GetLength(), rcText, DT_CENTER|DT_VCENTER|DT_SINGLELINE); 

		*pResult   =   CDRF_SKIPDEFAULT;         //   We 've   painted   everything. 
	} 
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
}

void CChangeVoiceListCtrl::Initialize()
{
	SetIconSpacing(60,70); 
	m_imageList.Create(60, 59, ILC_COLOR24, 0, 1);
	SetImageList(&m_imageList, LVSIL_NORMAL);

	SetBkColor(RGB(255,255,255));

	SetListData();
}

void CChangeVoiceListCtrl::Resize( long cx, long cy, RECT rcOuter, RECT& rcRet )
{
	long nwidth,nheight;
	nwidth = rcOuter.right - rcOuter.left;
	nheight = rcOuter.bottom - rcOuter.top;
	if (cx>cy&&cx>=(rcOuter.right-rcOuter.left))
	{
		rcRet.left = rcOuter.left+1;
		rcRet.right = rcOuter.right-1;	  
		rcRet.top = rcOuter.top+(nheight-2)/2-(nwidth-2)*cy/(2*cx);
		rcRet.bottom = rcOuter.bottom-(nheight-2)/2+(nwidth-2)*cy/(2*cx);
	}
	else if	(cx<cy&&cy>=(rcOuter.bottom-rcOuter.top))
	{
		rcRet.top=rcOuter.top+1;
		rcRet.bottom = rcOuter.bottom-1;
		rcRet.left = rcOuter.left+(nwidth-2)/2-(nheight-2)*cx/(2*cy);
		rcRet.right = rcOuter.right-(nwidth-2)/2+(nheight-2)*cx/(2*cy);
	}
	else if	(cx==cy&&cy>=(rcOuter.bottom-rcOuter.top))
	{
		rcRet.top=rcOuter.top+1;
		rcRet.bottom = rcOuter.bottom-1;
		rcRet.left = rcOuter.left+1;
		rcRet.right = rcOuter.right-1;
	}
	else
	{
		if ((cx<= (rcOuter.right-rcOuter.left)) && (cy <= (rcOuter.bottom- rcOuter.top)))
		{
			//居中调整
			rcRet.left = (rcOuter.right+rcOuter.left)/2 - cx/2;
			rcRet.right = rcRet.left + cx;
			rcRet.top = (rcOuter.top+ rcOuter.bottom)/2 - cy/2;
			rcRet.bottom = rcRet.top+ cy;
			return;
		}
		double per = double(cx)/double(cy);
		if ( per == 1)
		{
			rcRet = rcOuter;
			return;
		}
		if ( per > 1)
		{
			rcRet.left = rcOuter.left;
			rcRet.right = rcOuter.right;
			long newHeight =static_cast<long>((rcOuter.right- rcOuter.left ) / per) ;
			rcRet.top = (rcOuter.top + rcOuter.bottom)/2 - newHeight/2;
			rcRet.bottom = rcRet.top + newHeight;
		}
		else
		{
			rcRet.top = rcOuter.top;
			rcRet.bottom = rcOuter.bottom;
			long newWidth = static_cast<long>((rcOuter.bottom- rcOuter.top ) * per);
			rcRet.left = (rcOuter.left + rcOuter.right)/2 - newWidth/2;
			rcRet.right = rcRet.left + newWidth;        
		}
	}
}

void CChangeVoiceListCtrl::DrawHoverBorder( CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth )
{
	COLORREF bkColor = GetBkColor();//先用底色擦除上一次画的线框
	CPen newPen(PS_SOLID,iWidth,bkColor);
	CPen *pOldPen=pDC->SelectObject(&newPen);
	//pDC->Rectangle(m_oldHoveRect);
	pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.top);
	pDC->LineTo(m_oldHoveRect.right,m_oldHoveRect.top);
	pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.bottom);
	pDC->LineTo(m_oldHoveRect.right,m_oldHoveRect.bottom);
	pDC->MoveTo(m_oldHoveRect.right,m_oldHoveRect.top);
	pDC->LineTo(m_oldHoveRect.right,m_oldHoveRect.bottom);
	pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.bottom);
	pDC->LineTo(m_oldHoveRect.left,m_oldHoveRect.top);
	pDC->SelectObject(pOldPen);

	CPen newPen2(PS_SOLID,iWidth,color);//画新的线框
	CPen *pOldPen2=pDC->SelectObject(&newPen2);
	pDC->MoveTo(rcBorder.left,rcBorder.top);
	pDC->LineTo(rcBorder.right,rcBorder.top);
	pDC->MoveTo(rcBorder.left,rcBorder.bottom);
	pDC->LineTo(rcBorder.right,rcBorder.bottom);
	pDC->MoveTo(rcBorder.right,rcBorder.top);
	pDC->LineTo(rcBorder.right,rcBorder.bottom);
	pDC->MoveTo(rcBorder.left,rcBorder.bottom);
	pDC->LineTo(rcBorder.left,rcBorder.top);
	pDC->SelectObject(pOldPen2);
	m_oldHoveRect = rcBorder;
}
BOOL CChangeVoiceListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_OwnToolTipCtrl.m_hWnd)
	{
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

void CChangeVoiceListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//Disalbe the CToolTipCtrl of CListCtrl so it won't disturb our own tooltip-ctrl
	GetToolTips()->Activate(FALSE);

	//Enable our own tooltip-ctrl and make it show tooltip even if not haveing focus
	VERIFY(m_OwnToolTipCtrl.Create(this,TTS_ALWAYSTIP));
	m_OwnToolTipCtrl.SetMaxTipWidth(300);
	m_OwnToolTipCtrl.SetDelayTime(TTDT_INITIAL,100);

	CListCtrl::PreSubclassWindow();
}

void CChangeVoiceListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CPoint pt(GetMessagePos());
	//ScreenToClient(&pt);

	//Find the item
	LVHITTESTINFO hitinfo={0};
	hitinfo.flags = nFlags;
	hitinfo.pt = point;
	HitTest(&hitinfo);

	if (hitinfo.iItem >= 0)
	{
		HCURSOR hCur = LoadCursor(NULL,IDC_HAND);
		::SetCursor(hCur);
		if (m_nHoverItem != hitinfo.iItem)
		{
			CRect rcOld, rcNew;
			if (m_nHoverItem >= 0)
			{
				GetItemRect(m_nHoverItem, &rcOld, LVIR_ICON);
				InvalidateRect(rcOld);
			}

			if (hitinfo.iItem >= 0)
			{
				GetItemRect(hitinfo.iItem, &rcNew, LVIR_ICON);
				InvalidateRect(rcNew);
			}
			m_nHoverItem = hitinfo.iItem;
			//Invalidate();
		}
	}
	if (m_lastToolTip != hitinfo.iItem)
	{
		//Mouse moved ove a new item
		m_lastToolTip = hitinfo.iItem;

		//Remove thie old tooltip (if available)
		if (m_OwnToolTipCtrl.GetToolCount() > 0)
		{
			m_OwnToolTipCtrl.DelTool(this);
			m_OwnToolTipCtrl.Activate(FALSE);
		}

		//Add the new tooltip(if available)
		if (m_lastToolTip != -1)
		{
			//Not using CToolTipCtrl::AddTool(),
			//because it redirects the messages to CListCtrl parent
			TOOLINFO ti = {0};
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND;    // Indicate that uId is handle to a control
			ti.uId = (UINT_PTR)m_hWnd;   //Handle to the control
			ti.hwnd = m_hWnd;            //Handle to window to receive the tooltip-message
			ti.hinst = AfxGetInstanceHandle();
			ti.lpszText = LPSTR_TEXTCALLBACK;
			m_OwnToolTipCtrl.SendMessage(TTM_ADDTOOL,0,(LPARAM) ( LPTOOLINFO) &ti);
			m_OwnToolTipCtrl.Activate(TRUE);
		}

	}

	CListCtrl::OnMouseMove(nFlags, point);
}

void CChangeVoiceListCtrl::SetListData()
{
	VecGiftItem vChangeVoiceList;
	GiftManager::GetInstance()->GetChangeVoiceList(vChangeVoiceList);
	for ( int i = 0 ; i < vChangeVoiceList.size() ; ++i )
	{
		IGiftItem* pGiftItem = vChangeVoiceList[i];
		CxImage* pImage = new CxImage();
		if (pImage->LoadW(pGiftItem->GetSmallImagePath().c_str()))
		{
			m_pVectImgs.push_back(pImage);
		}
		else
		{
			delete pImage ;
		}

		InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, i, pGiftItem->GetName().c_str(), 0, 0, i, (LPARAM)pGiftItem);
		//SetItemData(i, (DWORD_PTR)pGiftItem);
	}

	if (m_pHoverImage == NULL)
	{
		m_pHoverImage = new CxImage();
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		std::wstring strPath = strModulePath + L"resource\\other\\Room_ChangeVoiceHover.png";
		m_pHoverImage->LoadW(strPath.c_str());
	}
}

void CChangeVoiceListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项
	if (nItem == -1)
	{
		return;
	}
	VecGiftItem vChangeVoiceList;
	GiftManager::GetInstance()->GetChangeVoiceList(vChangeVoiceList);
	IGiftItem * pGiftItem = vChangeVoiceList[nItem];
	//IGiftItem * pGiftItem = (IGiftItem *)GetItemData(nItem);
	if (pGiftItem != NULL)
	{
		CChangeVoiceDlg * pDlg = (CChangeVoiceDlg*)GetParent();
		if (pDlg != NULL)
		{
			pDlg->OnSelChangeVoice(pGiftItem);
		}
	}
	*pResult = 0;
}

BOOL CChangeVoiceListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return CListCtrl::OnEraseBkgnd(pDC);
}
