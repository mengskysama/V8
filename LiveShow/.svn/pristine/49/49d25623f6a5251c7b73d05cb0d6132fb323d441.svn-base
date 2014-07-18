#include "StdAfx.h"
#include "MyGoodsListCtrl.h"
#include "..\Room.h"
#include "..\GlobalRoomDefine.h"
#include "..\UserMessageDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

CMyGoodsListCtrl::CMyGoodsListCtrl(void)
{
}

CMyGoodsListCtrl::~CMyGoodsListCtrl(void)
{
	for (std::vector<CMyGoodsItem*>::iterator it = m_vecMyGoodsList.begin(); it != m_vecMyGoodsList.end(); ++it)
	{
		if (*it != NULL)
		{
			delete *it;
			*it = NULL;
		}
	}
	m_vecMyGoodsList.clear();
}

BEGIN_MESSAGE_MAP(CMyGoodsListCtrl, CGiftListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CMyGoodsListCtrl::OnNMClick)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolNeedText)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyGoodsListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

bool CMyGoodsListCtrl::InitGiftListCtrl()
{
	m_imageGiftList.Create(32,32,ILC_COLOR24 | ILC_MASK,50,50);
	LONG lStyle; 
	lStyle = GetWindowLong(m_hWnd,  GWL_STYLE);//获取当前窗口style  
	lStyle = lStyle | LVS_ICON; //设置style 
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = GetExtendedStyle(); 
	dwStyle = dwStyle | LVS_EX_ONECLICKACTIVATE | LVS_EX_TRACKSELECT; /* LVS_EX_INFOTIP*/  //LVS_EX_TRACKSELECT这个是用于鼠标HOVER时
	dwStyle &= ~LVS_SHOWSELALWAYS;

	SetExtendedStyle(dwStyle); //设置扩展风格
	SetIconSpacing(70,60);//设置礼物的左右上下间距
	SetBkColor(RGB(255,255,255));
	SetTextBkColor(RGB(255,255,255));

	InitListData();

	return true;
}

bool CMyGoodsListCtrl::SetListData( std::vector<CMyGoodsItem*> & vecMyGoodsList )
{
	for (int i = 0; i < vecMyGoodsList.size(); i++)
	{
		CMyGoodsItem * pMyGoodsItem = vecMyGoodsList[i];
		ASSERT(pMyGoodsItem != NULL);
		AddImage(pMyGoodsItem->GetGiftItem()->GetSmallImagePath());
	}
	SetImageList(&m_imageGiftList,LVSIL_NORMAL);

	for (int i = 0; i < vecMyGoodsList.size(); i++)
	{
		CMyGoodsItem * pMyGoodsItem = vecMyGoodsList[i];
		ASSERT(pMyGoodsItem != NULL);
		//AddImage(pGiftItem->GetSmallImagePath());
		InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, i, pMyGoodsItem->GetGiftItem()->GetName().c_str(), 0, 0, i, (LPARAM)pMyGoodsItem);
		//InsertItem(i,L"test",i);
	}

	if (m_pGiftWordsImg == NULL)
	{
		m_pGiftWordsImg = new CxImage();
		std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
		std::wstring strPath = strModulePath + IMG_GIFTWORDS_PATH;
		m_pGiftWordsImg->LoadW(strPath.c_str());
	}

	return true;
}

void CMyGoodsListCtrl::OnNMClick( NMHDR *pNMHDR, LRESULT *pResult )
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项

	if (nItem != -1)
	{
		CMyGoodsItem * pMyGoodsItem = (CMyGoodsItem*)GetItemData(nItem);
		if (pMyGoodsItem != NULL)
		{
			CWnd * pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				pParentWnd->SendMessage(WM_CLICK_MYGOODSLIST, 0, (LPARAM)pMyGoodsItem);
			}
		}
	}

	*pResult = 0;
}

void CMyGoodsListCtrl::OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult )
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
		GetItemRect(nItem,&rcItem, LVIR_ICON); 
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

		pItemImg->Draw(pDC->m_hDC,rcPic,NULL,true);

		rcText = rcItem;
		rcText.top = rcText.bottom + 2;
		rcText.bottom = rcText.top + 25;

		CMyGoodsItem * pMyGiftItem = (CMyGoodsItem*)GetItemData(nItem);
		sText = pMyGiftItem->GetGiftItem()->GetName().c_str();

		pDC->DrawText(sText,sText.GetLength(), rcText, DT_CENTER|DT_VCENTER|DT_WORDBREAK/*|DT_SINGLELINE*/); 

		if (pMyGiftItem->GetGiftItem()->IsNeedGiftWords())
		{
			m_pGiftWordsImg->Draw(pDC->m_hDC, rcPic.left + 15, rcPic.top - 5, m_pGiftWordsImg->GetWidth(), m_pGiftWordsImg->GetHeight(), NULL, true);
		}

		CRect rcCount(rcPic);
		rcCount.right += 5;
		rcCount.left = rcCount.right - 20;
		rcCount.top = rcCount.bottom - 10;

		CString strCount;
		strCount.Format(L"%d", pMyGiftItem->GetCount());
		pDC->DrawText(strCount, strCount.GetLength(), rcCount, DT_CENTER|DT_VCENTER|DT_WORDBREAK);

		*pResult   =   CDRF_SKIPDEFAULT;         //   We 've   painted   everything. 
	} 

	//*pResult = 0;
}

BOOL CMyGoodsListCtrl::OnToolNeedText( UINT id, NMHDR* pNMHDR, LRESULT* pResult )
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int iItem;
	iItem = HitTest(pt);

	if (iItem == -1)
	{
		return FALSE;
	}

	CMyGoodsItem * pMyGoodsItem = (CMyGoodsItem*)GetItemData(iItem);

	POINT itempt;
	//GetItemPosition(iItem, &itempt);

	CRect rc;
	GetItemRect(iItem, &rc, LVIR_ICON);
	itempt.x = rc.left;
	itempt.y = rc.top;

	ClientToScreen(&itempt);

	if (pMyGoodsItem != NULL)
	{
		ShowTip(itempt, pMyGoodsItem->GetGiftItem(), pMyGoodsItem->GetCount());
	}

	return TRUE;
}

void CMyGoodsListCtrl::InitListData()
{
	core::GetMyGoods my_goods_info;
	//memset(&my_goods_info, 0x0, sizeof(core::GetMyGoods));
	IModuleManager * pModuleManager = g_RoomModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		//g_RoomModule->m_pModuleManager->CallService(core::SERVICE_VALUE_CORE_GET_MY_GOODS, (param)&my_goods_info);
	}

#if 1
	my_goods_info.mapItem2Count[12] = 100;
	my_goods_info.mapItem2Count[1] = 99;
	my_goods_info.mapItem2Count[2] = 88;
	my_goods_info.mapItem2Count[4150] = 23;
#endif

	//VecGiftItem vecGiftList;
	if (m_vecMyGoodsList.size() != 0)
	{
		for (std::vector<CMyGoodsItem*>::iterator it = m_vecMyGoodsList.begin(); it != m_vecMyGoodsList.end(); ++it)
		{
			if (*it != NULL)
			{
				delete *it;
				*it = NULL;
			}
		}
		m_vecMyGoodsList.clear();
	}

	for (std::map<uint16, uint16>::iterator it = my_goods_info.mapItem2Count.begin(); it != my_goods_info.mapItem2Count.end(); ++it)
	{
		IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(it->first);
		CMyGoodsItem * pMyGoodsItem = new CMyGoodsItem();
		pMyGoodsItem->SetGiftItem(pGiftItem);
		pMyGoodsItem->SetCount(it->second);
		if (pGiftItem != NULL)
		{
			m_vecMyGoodsList.push_back(pMyGoodsItem);
		}
	}
	SetListData(m_vecMyGoodsList);
}