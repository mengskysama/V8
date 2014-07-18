#include "StdAfx.h"
#include "PropsListCtrl.h"
#include "GiftItem.h"

CPropsListCtrl::CPropsListCtrl(std::wstring strTabName)
:	CGiftListCtrl(strTabName)
{
}

CPropsListCtrl::~CPropsListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CPropsListCtrl, CGiftListCtrl)
	//ON_NOTIFY_REFLECT(NM_CLICK, &CPropsListCtrl::OnNMClick)
END_MESSAGE_MAP()

bool CPropsListCtrl::InitGiftListCtrl()
{
	m_imageGiftList.Create(32,32,ILC_COLOR24 | ILC_MASK,1,0);
	LONG lStyle; 
	lStyle = GetWindowLong(m_hWnd, GWL_STYLE);//获取当前窗口style  
	lStyle = lStyle | LVS_ICON; //设置style 
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = GetExtendedStyle(); 
	dwStyle = dwStyle | LVS_EX_ONECLICKACTIVATE | LVS_EX_TRACKSELECT; /*LVS_EX_INFOTIP*/ //LVS_EX_TRACKSELECT这个是用于鼠标HOVER时
	dwStyle &= ~LVS_SHOWSELALWAYS;

	SetExtendedStyle(dwStyle); //设置扩展风格
	SetIconSpacing(54,60);//设置礼物的左右上下间距
	SetBkColor(RGB(255,255,255));
	SetTextBkColor(RGB(255,255,255));
	SetImageList(&m_imageGiftList,LVSIL_NORMAL);

//	GetToolTips()->SetDelayTime(TTDT_INITIAL,100);//设置多久显示TIP
//	GetToolTips()->SetMaxTipWidth(200);
	SetListData(GiftManager::GetInstance()->GetPropsList());

	//AddStampItem();
	//AddSpeakerItem();
	//AddWishSong();

	return true;
}

void CPropsListCtrl::AddStampItem()
{
	GiftItem * pGiftItem = new GiftItem();
	pGiftItem->m_GiftInfo.unID = 0;
	pGiftItem->m_GiftInfo.strName = L"盖章";
	pGiftItem->m_GiftInfo.strDesc = L"点击选择印章种类";
	pGiftItem->m_GiftInfo.unPrice = 0;
	pGiftItem->m_GiftInfo.strSmallImagePath = L"";
	pGiftItem->m_GiftInfo.strLargeImagePath = L"";
	pGiftItem->m_GiftInfo.unGiftType = room::SENDTYPE_NONE;
	AddImage(pGiftItem->GetSmallImagePath());
	InsertItem(LVIF_TEXT | LVIF_PARAM, 0, pGiftItem->GetName().c_str(), 0, 0, 0, (LPARAM)pGiftItem);
}

void CPropsListCtrl::AddSpeakerItem()
{
	GiftItem * pGiftItem = new GiftItem();
	pGiftItem->m_GiftInfo.unID = 0;
	pGiftItem->m_GiftInfo.strName = L"喇叭";
	pGiftItem->m_GiftInfo.strDesc = L"喇叭";
	pGiftItem->m_GiftInfo.unPrice = 0;
	pGiftItem->m_GiftInfo.strSmallImagePath = L"";
	pGiftItem->m_GiftInfo.strLargeImagePath = L"";
	pGiftItem->m_GiftInfo.unGiftType = room::SENDTYPE_NONE;
	AddImage(pGiftItem->GetSmallImagePath());
	InsertItem(LVIF_TEXT | LVIF_PARAM, 1, pGiftItem->GetName().c_str(), 0, 0, 1, (LPARAM)pGiftItem);
}

void CPropsListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项

	if (nItem != -1)
	{
		IGiftItem * pGiftItem = (IGiftItem*)GetItemData(nItem);
		if (pGiftItem != NULL)
		{
			//CRoomParentDlg * pRoomParentDlg = (CRoomParentDlg*)GetParent();
			//if (pRoomParentDlg != NULL)
			//{
			//	if (pGiftItem->GetName() == L"盖章")
			//	{
			//		pRoomParentDlg->OnClickStamp();
			//	}
			//	else if (pGiftItem->GetName() == L"喇叭")
			//	{
			//		pRoomParentDlg->OnClickSpeaker();
			//	}
			//	else if (pGiftItem->GetName() == L"浪漫点歌")
			//	{
			//		pRoomParentDlg->OnClickWishSong();
			//	}
			//	else
			//	{
			//		pRoomParentDlg->OnClickGiftList(pGiftItem);
			//	}
			//}
		}
	}

	*pResult = 0;
}

void CPropsListCtrl::AddWishSong()
{
	GiftItem * pGiftItem = new GiftItem();
	pGiftItem->m_GiftInfo.unID = 0;
	pGiftItem->m_GiftInfo.strName = L"浪漫点歌";
	pGiftItem->m_GiftInfo.strDesc = L"浪漫点歌";
	pGiftItem->m_GiftInfo.unPrice = 0;
	pGiftItem->m_GiftInfo.strSmallImagePath = L"";
	pGiftItem->m_GiftInfo.strLargeImagePath = L"";
	pGiftItem->m_GiftInfo.unGiftType = room::SENDTYPE_NONE;
	AddImage(pGiftItem->GetSmallImagePath());
	InsertItem(LVIF_TEXT | LVIF_PARAM, 2, pGiftItem->GetName().c_str(), 0, 0, 2, (LPARAM)pGiftItem);
}