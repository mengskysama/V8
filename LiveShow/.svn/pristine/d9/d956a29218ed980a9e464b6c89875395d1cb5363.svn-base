#include "StdAfx.h"
#include "GiftListCtrl.h"
#include "..\UserMessageDefine.h"
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define LUXURY_TIP_WIDTH	325
#define LUXURY_TIP_HEIGHT	94
#define PROP_TIP_WIDTH		224
#define PROP_TIP_HEIGHT		100
#define NORMAL_TIP_WIDTH	230
#define NORMAL_TIP_HEIGHT	95

CGiftTipDlg * CGiftListCtrl::s_pGiftTipDlg = NULL;
CGiftListCtrl::CGiftListCtrl(void)
{
	m_lastToolTip = -1;
	//m_font.CreatePointFont(120,L"宋体",NULL);
	m_font.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("宋体"));
	m_pGiftWordsImg = NULL;
	m_pNormalBorder = NULL;
	m_pHoverBorder = NULL;
	m_nHoverItem = -1;
}

CGiftListCtrl::CGiftListCtrl(std::wstring strTabName)
: m_strTabName(strTabName)
{
	m_lastToolTip = -1;
	//font.CreatePointFont(120,L"宋体",NULL);
	m_font.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("宋体"));
	m_pGiftWordsImg = NULL;
	m_pNormalBorder = NULL;
	m_pHoverBorder = NULL;
	m_nHoverItem = -1;
}

CGiftListCtrl::~CGiftListCtrl(void)
{
	std::vector<CxImage*>::iterator iter ;
	for ( iter = m_pVectImgs.begin() ; iter != m_pVectImgs.end() ; ++iter )
	{
		CxImage* pImage = *iter;
		delete pImage;
	}
	m_pVectImgs.clear();

	if (s_pGiftTipDlg != NULL)
	{
		delete s_pGiftTipDlg;
		s_pGiftTipDlg = NULL;
	}

	if (m_pGiftWordsImg != NULL)
	{
		delete m_pGiftWordsImg;
		m_pGiftWordsImg = NULL;
	}

	if (m_pNormalBorder != NULL)
	{
		delete m_pNormalBorder;
		m_pNormalBorder = NULL;
	}

	if (m_pHoverBorder != NULL)
	{
		delete m_pHoverBorder;
		m_pHoverBorder = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftListCtrl, CListCtrl)
//	ON_NOTIFY_REFLECT(LVN_GETINFOTIP, &CGiftListCtrl::OnLvnGetInfoTip)
	ON_NOTIFY_REFLECT(NM_CLICK, &CGiftListCtrl::OnNMClick)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolNeedText)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CGiftListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

bool CGiftListCtrl::InitGiftListCtrl()
{
	m_imageGiftList.Create(32,32,ILC_COLOR24 | ILC_MASK,50,50);
	LONG lStyle; 
	lStyle = GetWindowLong(m_hWnd,  GWL_STYLE);//获取当前窗口style  
	//GetWindowLong()
	lStyle = lStyle | LVS_ICON; //设置style 
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = GetExtendedStyle(); 
	dwStyle = dwStyle | LVS_EX_ONECLICKACTIVATE | LVS_EX_TRACKSELECT; /* LVS_EX_INFOTIP*/  //LVS_EX_TRACKSELECT这个是用于鼠标HOVER时
	dwStyle &= ~LVS_SHOWSELALWAYS;

	SetExtendedStyle(dwStyle); //设置扩展风格
	SetIconSpacing(54,53);//设置礼物的左右上下间距
	SetBkColor(RGB(227,233,241));
	SetTextBkColor(RGB(255,255,255));
	//SetImageList(&m_imageGiftList,LVSIL_NORMAL);
	//DragonZ:
	//SetTextBkColor(CLR_NONE);
	//SetBkImage(L"D:\\bkImage.bmp");
	/////////
	InitListData();

	//SetImageList(&m_imageGiftList,LVSIL_NORMAL);

	return true;
}

bool CGiftListCtrl::SetListData( VecGiftItem & vecGiftList )
{
	for (int i = 0; i < vecGiftList.size(); i++)
	{
		IGiftItem * pGiftItem = vecGiftList[i];
		ASSERT(pGiftItem != NULL);
		AddImage(pGiftItem->GetSmallImagePath());
		//InsertItem(LVIF_TEXT | LVIF_PARAM, GetItemCount(), pGiftItem->GetName().c_str(), 0, 0, i, (LPARAM)pGiftItem);
	}
	SetImageList(&m_imageGiftList,LVSIL_NORMAL);

	for (int i = 0; i < vecGiftList.size(); i++)
	{
		IGiftItem * pGiftItem = vecGiftList[i];
		ASSERT(pGiftItem != NULL);
		//AddImage(pGiftItem->GetSmallImagePath());
		InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, i, pGiftItem->GetName().c_str(), 0, 0, i, (LPARAM)pGiftItem);
		//InsertItem(i,L"test",i);
	}

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	if (m_pGiftWordsImg == NULL)
	{
		m_pGiftWordsImg = new CxImage();
		std::wstring strPath = strModulePath + IMG_GIFTWORDS_PATH;
		m_pGiftWordsImg->LoadW(strPath.c_str());
	}

	if (m_pNormalBorder == NULL)
	{
		m_pNormalBorder = new CxImage();
		std::wstring strPath = strModulePath + IMG_GIFT_NORMALBORDER_PATH;
		m_pNormalBorder->LoadW(strPath.c_str());
	}

	if (m_pHoverBorder == NULL)
	{
		m_pHoverBorder = new CxImage();
		std::wstring strPath = strModulePath + IMG_GIFT_HOVERBORDER_PATH;
		m_pHoverBorder->LoadW(strPath.c_str());
	}

	return true;
}

void CGiftListCtrl::AddImage( std::wstring strImagePath )
{
	//HBITMAP hbmp = (HBITMAP)LoadImage(NULL,
	//	strImagePath.c_str(),
	//	IMAGE_BITMAP,
	//	0,
	//	0,
	//	LR_LOADFROMFILE);
	//CBitmap bm;
	////VERIFY(bm.Attach(hbmp));
	//if (!bm.Attach(hbmp))
	//{
	//	HBITMAP htmpbmp = (HBITMAP)LoadImage(NULL,
	//		L"resource\\listicon\\tortoise.bmp",
	//		IMAGE_BITMAP,
	//		0,
	//		0,
	//		LR_LOADFROMFILE);
	//	VERIFY(bm.Attach(htmpbmp));
	//}
	//int n = m_imageGiftList.Add(&bm,RGB(0,0,0));
	//bm.Detach();

	CxImage* pImage = new CxImage();
	if (pImage->LoadW(strImagePath.c_str()))
	{
		m_pVectImgs.push_back(pImage);
	}
	else
	{
		delete pImage ;
	}
}

/*
void CGiftListCtrl::OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//CRect rc;
	//GetWindowRect(&rc);
	//CPoint pot;
	//GetCursorPos(&pot);
	//pot.x = pot.x - rc.left;
	//pot.y = pot.y - rc.top;

	//LVHITTESTINFO LVHTestInfo;
	//LVHTestInfo.pt = pot;
	//HitTest(&LVHTestInfo);
	//int ihoverItem = LVHTestInfo.iItem;//获得第几项

	IGiftItem * pGiftItem = (IGiftItem*)GetItemData(pGetInfoTip->iItem);
	if (pGiftItem != NULL)
	{
		wcscpy(pGetInfoTip->pszText,pGiftItem->GetDesc().c_str());
	}

	*pResult = 0;
}
*/

void CGiftListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项

	if (nItem != -1)
	{
		IGiftItem * pGiftItem = (IGiftItem*)GetItemData(nItem);
		if (pGiftItem != NULL)
		{
			CWnd * pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				pParentWnd->SendMessage(WM_CLICK_GIFTLIST, 0, (LPARAM)pGiftItem);
			}
			//CRoomDlg * pRoomDlg = (CRoomDlg*)GetParent();
			//if (pRoomDlg != NULL)
			//{
			//	pRoomDlg->OnClickGiftList(pGiftItem);
			//}
		}
	}

	*pResult = 0;
}

void CGiftListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CListCtrl::PreSubclassWindow();

	//Disalbe the CToolTipCtrl of CListCtrl so it won't disturb our own tooltip-ctrl
	GetToolTips()->Activate(FALSE);

	//Enable our own tooltip-ctrl and make it show tooltip even if not haveing focus
	VERIFY(m_OwnToolTipCtrl.Create(this,TTS_ALWAYSTIP));
	m_OwnToolTipCtrl.SetMaxTipWidth(300);
	m_OwnToolTipCtrl.SetDelayTime(TTDT_INITIAL,500);
}



BOOL CGiftListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_OwnToolTipCtrl.m_hWnd)
	{
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

void CGiftListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	//Find thie item
	LVHITTESTINFO hitinfo={0};
	hitinfo.flags = nFlags;
	hitinfo.pt = pt;
	HitTest(&hitinfo);

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

	if (hitinfo.iItem == -1)
	{
		if (s_pGiftTipDlg != NULL && s_pGiftTipDlg->GetSafeHwnd() && s_pGiftTipDlg->IsWindowVisible())
		{
			s_pGiftTipDlg->ShowWindow(SW_HIDE);
		}
		m_lastToolTip = -1;
		return;
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

	CRect rc, rcOld, rcNew;
	GetClientRect(&rc);
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

	CListCtrl::OnMouseMove(nFlags, point);
}


BOOL CGiftListCtrl::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int iItem;
	iItem = HitTest(pt);

	if (iItem == -1)
	{
		return FALSE;
	}

	IGiftItem * pGiftItem = (IGiftItem*)GetItemData(iItem);

	POINT itempt;
	//GetItemPosition(iItem, &itempt);

	CRect rc;
	GetItemRect(iItem, &rc, LVIR_ICON);
	itempt.x = rc.left;
	itempt.y = rc.top;

	ClientToScreen(&itempt);
	
	if (pGiftItem != NULL)
	{
		//if (pGiftItem->GetDesc().empty())
		//{
		//	return FALSE;
		//}

        //TOOLTIPTEXTW* pTT   = (TOOLTIPTEXT*) pNMHDR;

		//wcscpy(pTT->szText,pGiftItem->GetDesc().c_str());
		//CWnd * pParentWnd = GetParent();
		//if (pParentWnd != NULL)
		//{
		//	pParentWnd->SendMessage(WM_SHOW_GIFT_TIP, 0, (LPARAM)pGiftItem);
		//}
		ShowTip(itempt, pGiftItem);
	}

    return TRUE;
}

void CGiftListCtrl::ShowTip( CPoint itempt, IGiftItem* pGiftItem, int nCount/* = 0*/ )
{
	ASSERT(pGiftItem != NULL);

	SwitchResourceToModule(_T("Room"));
	if (s_pGiftTipDlg == NULL)
	{
		CWnd * pParentWnd = GetParent();
		s_pGiftTipDlg = new CGiftTipDlg(pParentWnd);
		s_pGiftTipDlg->Create(CGiftTipDlg::IDD, pParentWnd);
	}	

	int nWidth;
	int nHeight;
	if (pGiftItem->GetCatalogName() == L"奢侈")
	{
		nWidth = LUXURY_TIP_WIDTH;
		nHeight = LUXURY_TIP_HEIGHT;
	}
	else if (pGiftItem->GetCatalogName() == L"道具")
	{
		nWidth = PROP_TIP_WIDTH;
		nHeight = PROP_TIP_HEIGHT;
	}
	else
	{
		nWidth = NORMAL_TIP_WIDTH;
		nHeight = NORMAL_TIP_HEIGHT;
	}

	if (s_pGiftTipDlg != NULL && s_pGiftTipDlg->GetSafeHwnd())
	{
		//s_pGiftTipDlg->Show(pGiftItem);
		s_pGiftTipDlg->MoveWindow(itempt.x - nWidth + 15, itempt.y - nHeight + 7, nWidth, nHeight);
		s_pGiftTipDlg->Show(pGiftItem, nCount);
	}
	SetFocus() ;
}

LRESULT CGiftListCtrl::OnMouseLeave( WPARAM wParam,LPARAM lParam )
{
	CPoint curPos;
	GetCursorPos(&curPos);
	if (s_pGiftTipDlg != NULL && s_pGiftTipDlg->GetSafeHwnd() && s_pGiftTipDlg->IsWindowVisible())
	{
		CRect rect;
		s_pGiftTipDlg->GetWindowRect(&rect);

		if (s_pGiftTipDlg->IsWindowVisible() && !rect.PtInRect(curPos))
		{
			s_pGiftTipDlg->ShowWindow(SW_HIDE);
		}
	}
	m_lastToolTip = -1;

	CRect rc;
	if (m_nHoverItem >= 0)
	{
		GetItemRect(m_nHoverItem, &rc, LVIR_BOUNDS);
		InvalidateRect(rc);

		m_nHoverItem = -1;
	}

	return 0;
}

void CGiftListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
		//GetItemRect(nItem,&rcItem, LVIR_ICON); 
		GetItemRect(nItem,&rcItem, LVIR_BOUNDS); 
		//rcItem.OffsetRect(2,5);
		//   Draw   the   icon. 
		uFormat=ILD_TRANSPARENT; 

		pDC->SelectObject(m_font);

		int iPicOffsetX= (rcItem.Width() - 32)/2;
		int iPicOffsetY= (rcItem.Height() - 32)/2;

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
		CRect rcPic(ptPic,CSize(32,32));


		//CRect rcBk = rcPic;
		//rcBk.InflateRect(2,2);
		//pDC->FillSolidRect(rcBk,RGB(255,0,0));
		//CRect rcReal;
		//Resize(pItemImg->GetWidth(),pItemImg->GetHeight(),rcPic,rcReal);

		CRect rcBorder(rcPic);
		if (nItem == m_nHoverItem)
		{
			rcBorder.InflateRect(11,12, 12, 12);
			m_pHoverBorder->Draw(pDC->m_hDC, rcBorder, NULL, true);
		}
		else
		{
			rcBorder.InflateRect(9, 9, 9, 10);
			m_pNormalBorder->Draw(pDC->m_hDC, rcBorder, NULL, true);
		}
		
		pItemImg->Draw(pDC->m_hDC,rcPic,NULL,true);

		rcText = rcItem;
		rcText.top = rcText.bottom + 2;
		rcText.bottom = rcText.top + 25;

		IGiftItem * pGiftItem = (IGiftItem*)GetItemData(nItem);
		sText = pGiftItem->GetName().c_str();

		//pDC->DrawText(sText,sText.GetLength(), rcText, DT_CENTER|DT_VCENTER|DT_WORDBREAK/*|DT_SINGLELINE*/); 

		if (pGiftItem->IsNeedGiftWords())
		{
			m_pGiftWordsImg->Draw(pDC->m_hDC, rcPic.left + 15, rcPic.top - 5, m_pGiftWordsImg->GetWidth(), m_pGiftWordsImg->GetHeight(), NULL, true);
		}

		*pResult   =   CDRF_SKIPDEFAULT;         //   We 've   painted   everything. 
	} 

	//*pResult = 0;
}

void CGiftListCtrl::ReleaseAllItems()
{
	DeleteAllItems();
	std::vector<CxImage*>::iterator iter ;
	for ( iter = m_pVectImgs.begin() ; iter != m_pVectImgs.end() ; ++iter )
	{
		CxImage* pImage = *iter;
		delete pImage;
	}
	m_pVectImgs.clear();
}

void CGiftListCtrl::InitListData()
{
	VecGiftItem vecGiftList;
	GiftManager::GetInstance()->GetGiftListByCatalog(m_strTabName, vecGiftList);
	SetListData(vecGiftList);
}

void CGiftListCtrl::ReloadListData()
{
	ReleaseAllItems();

	InitListData();
}