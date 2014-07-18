#include "StdAfx.h"
#include "StampListCtrl.h"
#include "StampDlg.h"

#define	THUMBNAIL_WIDTH		65
#define	THUMBNAIL_HEIGHT	32

#define ITEM_WIDTH          75    
#define ITEM_HEIGHT         42


CStampListCtrl::CStampListCtrl(void)
{
	m_lastToolTip = -1;
	m_curGroup = 0;
}

CStampListCtrl::~CStampListCtrl(void)
{
	DeleteAll() ;
}

BEGIN_MESSAGE_MAP(CStampListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CStampListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CStampListCtrl::OnNMClick)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolNeedText)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



void CStampListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast <NMLVCUSTOMDRAW*> (   pNMHDR   ); 
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

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
		rcItem.OffsetRect(2,5);
		//   Draw   the   icon. 
		uFormat=ILD_TRANSPARENT; 


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
		pDC-> FillSolidRect(rcBk,RGB(255,255,255));
		CRect rcReal;
		Resize(pItemImg->GetWidth(),pItemImg->GetHeight(),rcPic,rcReal);


		pItemImg->Draw(pDC->m_hDC,rcReal,NULL,true);

		*pResult   =   CDRF_SKIPDEFAULT;         //   We 've   painted   everything. 
	} 
}


void CStampListCtrl::Initialize()
{
	SetIconSpacing(ITEM_WIDTH,ITEM_HEIGHT); 
	m_imageList.Create(32, 32, ILC_COLOR24, 0, 1);
	SetImageList(&m_imageList, LVSIL_NORMAL);

	SetBkColor(RGB(255,255,255));
}

void CStampListCtrl::DrawBorder(CDC* pDC,const CRect& rcBorder, COLORREF color,int iWidth)
{
	CPen newPen(PS_SOLID,iWidth,color);
	CPen *pOldPen=pDC->SelectObject(&newPen);
	pDC->Rectangle(rcBorder);
	pDC->SelectObject(pOldPen);
}

void CStampListCtrl::Resize(long cx, long cy, RECT rcOuter, RECT& rcRet)
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


int CStampListCtrl::AddImage(int iIndex,CxImage * pImage)
{
	m_pVectImgs.push_back( pImage ) ;
	int i=InsertItem(GetItemCount(),L"",0);

	return 0;
}

void CStampListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项
    CStampDlg *pStampDlg = NULL;
	pStampDlg = (CStampDlg*)this->GetParent();
	pStampDlg->OnSelStamp(m_curGroup,nItem);
	pStampDlg->ShowWindow( SW_HIDE ) ;
	*pResult = 0;
}

void CStampListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	//Find the item
	LVHITTESTINFO hitinfo={0};
	hitinfo.flags = nFlags;
	hitinfo.pt = pt;
	HitTest(&hitinfo);
    
	if (hitinfo.iItem >= 0)
	{
		HCURSOR hCur = LoadCursor(NULL,IDC_HAND);
		::SetCursor(hCur);
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

BOOL CStampListCtrl::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int iItem;
	iItem = HitTest(pt);
	IGiftItem* pGiftItem = NULL ;
	std::map< int , IGiftItem* >::iterator iter = m_mapEmotion.find( iItem ) ;
	if ( iter != m_mapEmotion.end() )
	{
		pGiftItem = iter->second ;
	}
	if ( pGiftItem != NULL )
	{
		TOOLTIPTEXTW* pTT   = (TOOLTIPTEXT*) pNMHDR;
		wcscpy(pTT->szText,pGiftItem->GetDesc().c_str() );
	}
 
	return TRUE;
}

void CStampListCtrl::PreSubclassWindow()
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

BOOL CStampListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_OwnToolTipCtrl.m_hWnd)
	{
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

//清空信息
void CStampListCtrl::DeleteAll() 
{
	std::vector<CxImage*>::iterator iter ;
	for ( iter = m_pVectImgs.begin() ; iter != m_pVectImgs.end() ; ++iter )
	{
		CxImage* pImage = *iter ;
		delete pImage ;
	}
	m_pVectImgs.clear();	
	m_mapEmotion.clear();
}

BOOL CStampListCtrl::SetListData( int iGroup)//目前只支持一组表情，后期会更改;
{
	std::map<int ,std::wstring >::iterator miter ;
	std::map<int ,std::wstring > mStampTypeList = GiftManager::GetInstance()->GetStampTypeList() ;
	int nIndex = 0 ;
	for ( miter = mStampTypeList.begin() ; miter != mStampTypeList.end() ; ++miter ,++nIndex )
	{
		if ( iGroup == nIndex )
		{
			this->DeleteAllItems() ;  //清空上一次显示的盖章
			DeleteAll() ;
			m_curGroup = iGroup ;
			VecGiftItem vStampList;
			GiftManager::GetInstance()->GetStampListByCatalog( miter->second, vStampList );
			for ( int i = 0 ; i < vStampList.size() ; ++i )
			{
				IGiftItem* pGiftItem = vStampList[i] ;
				CxImage* pImage = new CxImage() ;
				if ( pImage->LoadW( pGiftItem->GetLargeImagePath().c_str() ) )
				{
					AddImage( i , pImage ) ;
					m_mapEmotion.insert( std::make_pair(i,pGiftItem) ) ;
				}
				else
				{
					delete pImage ;
				}
			}
			break;
		}
	}
	
	return true;
}

void CStampListCtrl::DrawHoverBorder(CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth)
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
