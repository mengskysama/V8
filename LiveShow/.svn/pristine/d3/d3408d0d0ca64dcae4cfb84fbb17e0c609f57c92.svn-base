#include "StdAfx.h"
#include "EmotionListCtrl.h"
#include "EmotionDlg.h"

#define	THUMBNAIL_WIDTH		32
#define	THUMBNAIL_HEIGHT	32

#define CAITIAO_WIDTH       268
#define CAITIAO_HEIGHT      30

#define ITEM_WIDTH          38    
#define ITEM_HEIGHT         38

#define EMOTION_PER_PAGE    35   //每页的表情数

CEmotionListCtrl::CEmotionListCtrl(void)
{
	m_lastToolTip = -1;
	m_curGroup = 0;//第一组
	m_curPage = 0;//第一页
	m_totalPageInCurGroup = 0;//当前组的总页数
	m_bOldHoveChangeed = false ;
	m_bCurShowCaiTiao = false;
}

CEmotionListCtrl::~CEmotionListCtrl(void)
{
	DeleteAll() ;
}
BEGIN_MESSAGE_MAP(CEmotionListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEmotionListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CEmotionListCtrl::OnNMClick)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolNeedText)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



void CEmotionListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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

		int iPicOffsetX= m_bCurShowCaiTiao ? (rcItem.Width() - CAITIAO_WIDTH)/2 : (rcItem.Width() - THUMBNAIL_WIDTH)/2;
		int iPicOffsetY= m_bCurShowCaiTiao ? (rcItem.Height() - CAITIAO_HEIGHT)/2 : (rcItem.Height() - THUMBNAIL_HEIGHT)/2;

		CPoint ptPic=rcItem.TopLeft();
		ptPic.Offset(iPicOffsetX,iPicOffsetY);
	
		CxImage * pItemImg = NULL;
		CRect rcPic(ptPic,CSize(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT));
		if (!m_bCurShowCaiTiao)
		{
			if (rItem.iItem>=m_pVectImgs.size())
			{
				return;
			}
			pItemImg = m_pVectImgs[rItem.iItem];
		}else
		{
			if (rItem.iItem>=m_pVectCaiTiaoImgs.size())
			{
				return;
			}
			pItemImg = m_pVectCaiTiaoImgs[rItem.iItem];
			rcPic.right = rcPic.left + CAITIAO_WIDTH;
			rcPic.bottom = rcPic.top + CAITIAO_HEIGHT;
		}
		
		if (pItemImg == NULL)
		{
			return;
		}
		
		CRect rcBk = rcPic;
		rcBk.InflateRect(4,4);
		pDC-> FillSolidRect(rcBk,RGB(255,255,255));
		CRect rcReal;
		Resize(pItemImg->GetWidth(),pItemImg->GetHeight(),rcPic,rcReal);

		CRect rcBorder=rcPic;
		rcBorder.InflateRect(2,2,2,2);
		DrawBorder(pDC,rcBorder,RGB(230,233,238),1);

		pItemImg->Draw(pDC->m_hDC,rcReal,NULL,true);

		*pResult   =   CDRF_SKIPDEFAULT;         //   We 've   painted   everything. 
	} 
}


void CEmotionListCtrl::Initialize()
{
	SetIconSpacing(ITEM_WIDTH,ITEM_HEIGHT); 
	m_imageList.Create(32, 32, ILC_COLOR24, 0, 1);
	SetImageList(&m_imageList, LVSIL_NORMAL);
	m_imageCaiTiaoList.Create(32*7 - 20, 32, ILC_COLOR24, 0, 1);
	//SetImageList(&m_imageCaiTiaoList, LVSIL_NORMAL);
	SetBkColor(RGB(255,255,255));
}

void CEmotionListCtrl::DrawBorder(CDC* pDC,const CRect& rcBorder, COLORREF color,int iWidth)
{
	CPen newPen(PS_SOLID,iWidth,color);
	CPen *pOldPen=pDC->SelectObject(&newPen);
	pDC->Rectangle(rcBorder);
	pDC->SelectObject(pOldPen);
}

void CEmotionListCtrl::Resize(long cx, long cy, RECT rcOuter, RECT& rcRet)
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
			//long newHeight = CAITIAO_HEIGHT; //高度固定，再算出宽度,给图的宽度不能超过表情框所能放下的宽度
			//long newWidth = CAITIAO_HEIGHT * per;
			//rcRet.top = rcOuter.top + ( nheight - newHeight )/2;
			//rcRet.bottom = rcOuter.top + newHeight;
			//rcRet.left = rcOuter.left + (nwidth - newWidth)/2;
			//rcRet.right = rcOuter.left + newWidth;

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

int CEmotionListCtrl::AddImage(int iIndex,CxImage * pImage, bool bCaiTiao)
{
	if (bCaiTiao)
	{
		m_pVectCaiTiaoImgs.push_back(pImage);
	}else
	{
		m_pVectImgs.push_back( pImage ) ;
	}
	
	int i=InsertItem(GetItemCount(),L"",0);

	return 0;
}

void CEmotionListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项
	if(nItem < 0)
		return;
    CEmotionDlg *pEmoDlg = NULL;
	pEmoDlg = (CEmotionDlg*)this->GetParent();
	int iEmoPerPage = EMOTION_PER_PAGE;
	if (m_bCurShowCaiTiao)
	{
		iEmoPerPage = 5;
	}
	pEmoDlg->OnSelEmotion(m_curGroup,GetCurPage()*iEmoPerPage+nItem);
	pEmoDlg->ShowWindow( SW_HIDE ) ;
	*pResult = 0;
}

void CEmotionListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	//Find the item
	LVHITTESTINFO hitinfo={0};
	hitinfo.flags = nFlags;
	hitinfo.pt = pt;
	HitTest(&hitinfo);
    
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

		CPoint pt(GetMessagePos());
		ScreenToClient(&pt);

		//Find the item
		LVHITTESTINFO hitinfo={0};
		//hitinfo.flags = nFlags;
		hitinfo.pt = pt;
		HitTest(&hitinfo);
		map< int , EmotionItem* >::iterator iter = m_mapEmotion.find(hitinfo.iItem);
		if ( iter != m_mapEmotion.end() )
		{
			CRect rcItem;
			GetItemRect(hitinfo.iItem,&rcItem, LVIR_ICON); 
			rcItem.OffsetRect(2,5);

			int iPicOffsetX= m_bCurShowCaiTiao ? (rcItem.Width() - CAITIAO_WIDTH)/2 : (rcItem.Width() - THUMBNAIL_WIDTH)/2;
			int iPicOffsetY= m_bCurShowCaiTiao ? (rcItem.Height() - CAITIAO_HEIGHT)/2 : (rcItem.Height() - THUMBNAIL_HEIGHT)/2;

			CPoint ptPic=rcItem.TopLeft();
			ptPic.Offset(iPicOffsetX,iPicOffsetY);

			CxImage * pItemImg = NULL;
			CRect rcPic(ptPic,CSize(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT));
			if (!m_bCurShowCaiTiao)
			{
				if (hitinfo.iItem>=m_pVectImgs.size())
				{
					return;
				}
				pItemImg = m_pVectImgs[hitinfo.iItem];
			}else
			{
				if (hitinfo.iItem>=m_pVectCaiTiaoImgs.size())
				{
					return;
				}
				pItemImg = m_pVectCaiTiaoImgs[hitinfo.iItem];
				rcPic.right = rcPic.left + CAITIAO_WIDTH;
				rcPic.bottom = rcPic.top + CAITIAO_HEIGHT;
			}

			if (pItemImg == NULL)
			{
				return;
			}


			CRect rcBk = rcPic;
			rcBk.InflateRect(4,4);

			CDC *pDC = GetDC();
			pDC-> FillSolidRect(rcBk,RGB(255,255,255));
			CRect rcReal;
			Resize(pItemImg->GetWidth(),pItemImg->GetHeight(),rcPic,rcReal);

			CRect rcBorderBk=rcPic;
			rcBorderBk.InflateRect(2,2,2,2);
			DrawBorder(pDC,rcBorderBk,RGB(230,233,238),1);

			CRect rcBorder=rcPic;
			CRect rc = rcBorder;
			rc.InflateRect(-2,-2,-2,-2);
			rc.left--;
			rc.top--;
			DrawHoverBorder(pDC,&rc,RGB(128,189,254),1);
			pItemImg->Draw(pDC->m_hDC,rcReal,NULL,true);
			ReleaseDC( pDC ) ;
		}
		else
		{
			if ( m_bOldHoveChangeed )
			{
				m_bOldHoveChangeed = false ;
				CRect rcItem;
				GetItemRect(hitinfo.iItem,&rcItem, LVIR_ICON); 
				rcItem.OffsetRect(2,5);
				int iPicOffsetX= m_bCurShowCaiTiao ? (rcItem.Width() - CAITIAO_WIDTH)/2 : (rcItem.Width() - THUMBNAIL_WIDTH)/2;
				int iPicOffsetY= m_bCurShowCaiTiao ? (rcItem.Height() - CAITIAO_HEIGHT)/2 : (rcItem.Height() - THUMBNAIL_HEIGHT)/2;

				CPoint ptPic=rcItem.TopLeft();
				ptPic.Offset(iPicOffsetX,iPicOffsetY);
				CRect rcPic(ptPic,CSize(THUMBNAIL_WIDTH,THUMBNAIL_WIDTH));
				if (m_bCurShowCaiTiao)
				{
					rcPic.right = rcPic.left + CAITIAO_WIDTH;
					rcPic.bottom = rcPic.top + CAITIAO_HEIGHT;
				}

				CDC *pDC = GetDC();
				COLORREF bkColor = GetBkColor();//用底色擦除上一次画的线框
				CPen newPen(PS_SOLID,1,bkColor);
				CPen *pOldPen=pDC->SelectObject(&newPen);
				pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.top);
				pDC->LineTo(m_oldHoveRect.right,m_oldHoveRect.top);
				pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.bottom);
				pDC->LineTo(m_oldHoveRect.right+1,m_oldHoveRect.bottom);
				pDC->MoveTo(m_oldHoveRect.right,m_oldHoveRect.top);
				pDC->LineTo(m_oldHoveRect.right,m_oldHoveRect.bottom);
				pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.bottom);
				pDC->LineTo(m_oldHoveRect.left,m_oldHoveRect.top);
				pDC->SelectObject(pOldPen);
				ReleaseDC( pDC ) ;
			}
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}

BOOL CEmotionListCtrl::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int iItem;
	iItem = HitTest(pt);
	map<int,EmotionItem*>::iterator iter = m_mapEmotion.find(iItem);
	EmotionItem* pEmotionItem = NULL ;
	if (iter != m_mapEmotion.end())
	{
		pEmotionItem = iter->second;
	}
	//pEmotionItem = (stEmotion*)GetItemData(iItem);  //取不到，原因是InsertItem时GetItemCount会按2递增。这里暂时采用Map实现

	if (pEmotionItem != NULL)
	{	
	    TOOLTIPTEXTW* pTT   = (TOOLTIPTEXT*) pNMHDR;
		wcscpy(pTT->szText , pEmotionItem->m_strEmotionName.c_str() ) ;
	} 

	return TRUE;
}

void CEmotionListCtrl::PreSubclassWindow()
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

BOOL CEmotionListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_OwnToolTipCtrl.m_hWnd)
	{
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

//清空信息
void CEmotionListCtrl::DeleteAll() 
{
	std::vector<CxImage*>::iterator iter ;
	for ( iter = m_pVectImgs.begin() ; iter != m_pVectImgs.end() ; ++iter )
	{
		CxImage* pImage = *iter ;
		delete pImage ;
	}
	m_pVectImgs.clear();	
	for ( iter = m_pVectCaiTiaoImgs.begin() ; iter != m_pVectCaiTiaoImgs.end() ; ++iter )
	{
		CxImage* pImage = *iter ;
		delete pImage ;
	}
	m_pVectCaiTiaoImgs.clear();	

	m_mapEmotion.clear();//清空上一次显示的表情
	m_bOldHoveChangeed = false ;
}

BOOL CEmotionListCtrl::SetListData( int iGroup, int iPage)
{
	m_lastToolTip = -1 ;
	map< int , wstring > mTypeList = EmotionMgr->GetTypeList() ;
	int i = 0 ;
	map< int , wstring >::iterator mIter ;

	map< int , wstring >::iterator tempIter = mTypeList.begin();
	advance(tempIter, iGroup);
	bool bCaiTiao = (tempIter->second == L"彩条");//是否彩条
	m_bCurShowCaiTiao = bCaiTiao;//当前显示是否是彩条
	if (m_bCurShowCaiTiao)
	{
		SetImageList(&m_imageCaiTiaoList, LVSIL_NORMAL);
		SetIconSpacing(CAITIAO_WIDTH + 3,CAITIAO_HEIGHT + 3); 
	}else
	{
		SetImageList(&m_imageList, LVSIL_NORMAL);
		SetIconSpacing(ITEM_WIDTH,ITEM_HEIGHT); 
	}
	
	for ( mIter = mTypeList.begin() ; mIter != mTypeList.end() ; ++mIter ,++i )
	{
		if ( iGroup == i )
		{
			vector<EmotionItem*> vEmotionList = EmotionMgr->GetEmotionListByType( mIter->first ) ;
			this->DeleteAllItems();
			DeleteAll() ; //清空信息

			m_curGroup = iGroup;//当前组
			m_curPage = iPage;//该组下的当前页

			int iEmoPerPage = EMOTION_PER_PAGE;
			if (bCaiTiao)
			{
				iEmoPerPage = 5;
			}

			int iTotalEmotion = vEmotionList.size();//当前组的所有表情
			int iFrom = iPage * iEmoPerPage;//当前页第一个表情的索引
			int iEmotionInPage = 0;//该页下的表情数
			int iTotalPage = 0;//该组表情的总页数
			
			if (0 != iTotalEmotion % iEmoPerPage)
			{
				iTotalPage = iTotalEmotion / iEmoPerPage + 1;
			}else
			{
				iTotalPage = iTotalEmotion / iEmoPerPage;
			}
			m_totalPageInCurGroup = iTotalPage;

			if ((iPage+1) == iTotalPage)//iPage是从0计数的,最后一页的情况
			{
				iEmotionInPage = iTotalEmotion - iFrom;
			}else if((iPage+1) < iTotalPage)//非最后一页的情况
			{
				iEmotionInPage = iEmoPerPage;
			}else//非法情况
			{
				return FALSE;
			}

			int j = 0;//j作为存放到m_mapEmotion的索引
			for (int i = iFrom; i < (iFrom + iEmotionInPage) && i<vEmotionList.size(); ++i)
			{
				EmotionItem *pEmotionItem = vEmotionList[i] ;
				CxImage *pImage = new CxImage() ;
				if ( pImage->LoadW( pEmotionItem->GetBigEmotionPath().c_str() ) )
				{
					AddImage( i, pImage, bCaiTiao);
					m_mapEmotion.insert( make_pair( j++ , pEmotionItem ) ) ;
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

void CEmotionListCtrl::DrawHoverBorder(CDC* pDC,const CRect& rcBorder,COLORREF color,int iWidth)
{
	COLORREF bkColor = GetBkColor();//先用底色擦除上一次画的线框
	CPen newPen(PS_SOLID,iWidth,bkColor);
	CPen *pOldPen=pDC->SelectObject(&newPen);
	//pDC->Rectangle(m_oldHoveRect);
	pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.top);
	pDC->LineTo(m_oldHoveRect.right,m_oldHoveRect.top);
	pDC->MoveTo(m_oldHoveRect.left,m_oldHoveRect.bottom);
	pDC->LineTo(m_oldHoveRect.right+1,m_oldHoveRect.bottom);
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
	pDC->LineTo(rcBorder.right+1,rcBorder.bottom);
	pDC->MoveTo(rcBorder.right,rcBorder.top);
	pDC->LineTo(rcBorder.right,rcBorder.bottom);
	pDC->MoveTo(rcBorder.left,rcBorder.bottom);
	pDC->LineTo(rcBorder.left,rcBorder.top);
	pDC->SelectObject(pOldPen2);
	if ( m_oldHoveRect != rcBorder )
	{
		m_bOldHoveChangeed = true ;
	}
	m_oldHoveRect = rcBorder;
	
}
