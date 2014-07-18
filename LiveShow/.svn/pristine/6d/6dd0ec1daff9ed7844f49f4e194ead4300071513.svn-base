
#include "stdafx.h"
#include "VividTree.h"
#include "RoomDefine.h"
#include "utility\SystemHelper.h"
#include "coredefine.h"
#include "../HallModule.h"
#include "EnterRoomRequest.h"

#define WM_NOTIFY_HALL_TREE_CLICK WM_USER+5682   //

#define ICON_WIDTH 16
#define ICON_HEIGHT 16
#define PEOPLE_NUM_WIDTH 60
#define PEOPLE_NUM_MARGIN 2    //人数显示位置右边距
#define CHANNEL_NAME_MARGIN 38  //频道名字左边距
#define TEXT_CENTER_MARGIN  1   //GDI+的居中显示文字有偏差

#define COLOR_ITEM_TEXT_NO_SELECTED       RGB(76, 87, 119)
#define COLOR_ITEM_TEXT_SELECTED          RGB(255, 255, 255)
#define COLOR_ITEM_TEXT_NUM_PARENTNODE    RGB(76, 87, 119)
#define COLOR_ITEM_TEXT_NUM_CHILDNODE     RGB(76, 87, 119)

// CTreeCtrlEx

IMPLEMENT_DYNAMIC(VividTree, CTreeCtrl)

VividTree::VividTree()
{
    m_selectedItem = NULL;
	m_hoverItem = NULL;
	m_fontParentName = NULL;
	m_fontChildName = NULL;
	m_fontParentNum = NULL;
	m_fontChildNum = NULL;

	SetTheFont();
	CString strTemp = common::utility::systemhelper::Get179AppPath().c_str();
	m_strParentNodeHoverItemBk.Format(L"%sresource\\halltree\\Hall_imgParentNodeHover.png", strTemp);
	m_strChildNodeHoverItemBk.Format(L"%sresource\\halltree\\Hall_imgChildNodeHoverSel.png", strTemp);
	m_strParentNodeSelItemBk.Format(L"%sresource\\halltree\\Hall_imgParentNodeSelected.png", strTemp);
	m_strChildNodeSelItemBk.Format(L"%sresource\\halltree\\Hall_imgChildNodeHoverSel.png", strTemp);
	m_strParentNodeNormalItemBk.Format(L"%sresource\\halltree\\Hall_imgParentNodeNormal.png", strTemp);
	m_strExpand.Format(L"%sresource\\halltree\\Hall_imgTreeIconExpand.png", strTemp);
	m_strContract.Format(L"%sresource\\halltree\\Hall_imgTreeIconContract.png", strTemp);
	m_bInClearStack = false;
}

VividTree::~VividTree()
{
  if (m_fontParentName)
  {
	  delete m_fontParentName;
	  m_fontParentName = NULL;
  }
  if (m_fontChildName)
  {
	  delete m_fontChildName;
	  m_fontChildName = NULL;
  }
  if (m_fontParentNum)
  {
	  delete m_fontParentNum;
	  m_fontParentNum = NULL;
  }
  if (m_fontChildNum)
  {
	  delete m_fontChildNum;
	  m_fontChildNum = NULL;
  }
}


BEGIN_MESSAGE_MAP(VividTree, CTreeCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL VividTree::OnEraseBkgnd(CDC* pDC)
{
    
	return TRUE;
}


void VividTree::OnPaint()
{
	if(m_bInClearStack)
	{
		return;//临时解决调用Clear()时会调用OnPaint导致崩溃
	}

	CPaintDC dc(this);	
	GetClientRect(&m_rect);

	CDC MemDC ;	
	if(!MemDC.CreateCompatibleDC(&dc))
	{
		assert(0);
		return;
	}
	CBitmap MemBitmap ;
	if(!MemBitmap.CreateCompatibleBitmap(&dc ,m_rect.Width(), m_rect.Height()))
	{
		assert(0);
		MemDC.DeleteDC();
		return;
	}
	MemDC.SetBkMode(TRANSPARENT);
	CBitmap* pOldBitmap = MemDC.SelectObject( &MemBitmap ) ;
	DrawBackGround(&MemDC);
	DrawItems(&MemDC);
	dc.BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject( pOldBitmap ) ;
	MemBitmap.DeleteObject() ;
	MemDC.DeleteDC();
}


void VividTree::DrawBackGround(CDC* pDC)
{
	ImageAttributes imgAtt;
	imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果。但此处不需要，所以得加上此属性
	Graphics graph(*pDC);
	Image imgBk(m_strImgBk);
	Rect rcTemp(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());
	graph.DrawImage(&imgBk, rcTemp, 0, 0, m_rect.Width(), m_rect.Height(),UnitPixel,&imgAtt);
}

void VividTree::DrawItemBk(CDC *pDC, CRect& rcBk, CString& imgPath)
{
	ImageAttributes imgAtt;
	imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果。但此处不需要，所以得加上此属性
	Graphics graph(*pDC);
	Image imgBk(imgPath);
	//CRect tempClient = m_rect;
	//CRect tempNode = rcBk;
	//ClientToScreen(&tempClient);
	//ClientToScreen(&tempNode);
	//Rect rcTemp(rcBk.left, tempNode.top - tempClient.top, rcBk.Width(), rcBk.Height());
	//graph.DrawImage(&imgBk, rcTemp, 0, 0, rcBk.Width(), rcBk.Height(), UnitPixel, &imgAtt);

	ASSERT(( rcBk.Width() >= imgBk.GetWidth() ) && ( rcBk.Height() >= imgBk.GetHeight() ) );

	int imgLeft = rcBk.left + (rcBk.Width() - imgBk.GetWidth())/2;
	int imgTop = rcBk.top + (rcBk.Height() - imgBk.GetHeight())/2;
	Rect rcTemp(0,0,0,0);
	rcTemp.X = imgLeft;
	rcTemp.Y = imgTop;
	rcTemp.Width = imgBk.GetWidth();
	rcTemp.Height = imgBk.GetHeight();
	graph.DrawImage(&imgBk, rcTemp, 0, 0, imgBk.GetWidth(), imgBk.GetHeight(), UnitPixel, &imgAtt );
}

void VividTree::DrawIcon(CDC* pDC, CRect& rcItem, CString strIcon)
{
	ImageAttributes imgAtt;
	imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果。但此处不需要，所以得加上此属性
	Graphics graph(*pDC);
	Image imgIcon(strIcon);
	CRect tempClient = m_rect;
	CRect tempItem = rcItem;
	ClientToScreen(&tempClient);
	ClientToScreen(&tempItem);
	CRect rcIcon = rcItem;
	rcIcon.top = (tempItem.top - tempClient.top) + (rcItem.Height() - imgIcon.GetHeight())/2;
	Rect rcTemp(rcIcon.left, rcIcon.top, imgIcon.GetWidth(), imgIcon.GetHeight());
	graph.DrawImage(&imgIcon, rcTemp, 0, 0, imgIcon.GetWidth(), imgIcon.GetHeight(), UnitPixel, &imgAtt);

}

void VividTree::DrawName(CDC* pDC, CRect& rcItem, CString strName, BOOL bSel, BOOL bHasParent)
{
	Graphics graph(*pDC);
	StringFormat stringFormat(0, LANG_NEUTRAL);
	stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);//只显示成单行
	stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
	//stringFormat.SetLineAlignment(StringAlignmentCenter);//用此方法不会完全居中，而是会偏上一点，改用自己手动控制位置吧。。。
	Color brushColor;
	if (bSel)
	{
		COLORREF clr = bHasParent ? COLOR_ITEM_TEXT_SELECTED : COLOR_ITEM_TEXT_NO_SELECTED;
		brushColor.SetFromCOLORREF(clr);
	}else
	{
		brushColor.SetFromCOLORREF(COLOR_ITEM_TEXT_NO_SELECTED);
	}
	SolidBrush solidBrush(brushColor);

	CRect nameRc(0,0,0,0), treeRc(0,0,0,0);
	GetWindowRect(&treeRc);
	ScreenToClient(&treeRc);
	nameRc = rcItem;
	nameRc.left += CHANNEL_NAME_MARGIN;
	nameRc.right = treeRc.right - PEOPLE_NUM_WIDTH - PEOPLE_NUM_MARGIN;
	RectF drawRc(nameRc.left, nameRc.top, nameRc.Width(), nameRc.Height());
	RectF boundRc;
	if (!bHasParent)
	{
		graph.MeasureString(strName, strName.GetLength(), m_fontParentName, PointF(nameRc.left, nameRc.top), &boundRc);
		RectF rcf = drawRc;
		rcf.Y = drawRc.Y + (drawRc.Height - boundRc.Height)/2 + TEXT_CENTER_MARGIN;
		graph.DrawString(strName, strName.GetLength(), m_fontParentName, rcf, &stringFormat, &solidBrush);
	}else
	{
		graph.MeasureString(strName, strName.GetLength(), m_fontChildName, PointF(nameRc.left, nameRc.top), &boundRc);
		RectF rcf = drawRc;
		rcf.Y = drawRc.Y + (drawRc.Height - boundRc.Height)/2 + TEXT_CENTER_MARGIN;
		graph.DrawString(strName, strName.GetLength(), m_fontChildName, rcf, &stringFormat, &solidBrush);
	}
}

void VividTree::DrawNumber(CDC* pDC, CRect& rcItem, CString strNum, BOOL bSel, BOOL bHasParent)
{
	Graphics graph(*pDC);
	StringFormat stringFormat(0, LANG_NEUTRAL);
	stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);//只显示成单行
	stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
	Color brushColor;
    if (bSel)
    {
		COLORREF clr = bHasParent ? COLOR_ITEM_TEXT_SELECTED : COLOR_ITEM_TEXT_NO_SELECTED;
		brushColor.SetFromCOLORREF(clr);
	}else
	{
		//brushColor.SetFromCOLORREF(COLOR_ITEM_TEXT_NO_SELECTED);
		if (!bHasParent)
		{
			brushColor.SetFromCOLORREF(COLOR_ITEM_TEXT_NUM_PARENTNODE);
		}else
		{
			brushColor.SetFromCOLORREF(COLOR_ITEM_TEXT_NUM_CHILDNODE);
		}
	}
	SolidBrush solidBrush(brushColor);
	
	CRect numRect(0,0,0,0), treeRc(0,0,0,0);
	GetWindowRect(&treeRc);
	ScreenToClient(&treeRc);
	numRect.top = rcItem.top;
	numRect.bottom = rcItem.bottom;
	numRect.right = treeRc.right - PEOPLE_NUM_MARGIN;
	numRect.left = numRect.right - PEOPLE_NUM_WIDTH;

	RectF drawRc(numRect.left, numRect.top, numRect.Width(), numRect.Height());
	RectF boundRc;
	if (!bHasParent)
	{
		graph.MeasureString(strNum, strNum.GetLength(), m_fontParentNum, PointF(numRect.left, numRect.top), &boundRc);
		RectF rcf = drawRc;
		rcf.Y = drawRc.Y + (drawRc.Height - boundRc.Height)/2 + TEXT_CENTER_MARGIN;
		graph.DrawString(strNum, strNum.GetLength(), m_fontParentNum, rcf, &stringFormat, &solidBrush);
	}else
	{
		graph.MeasureString(strNum, strNum.GetLength(), m_fontChildNum, PointF(numRect.left, numRect.top), &boundRc);
		RectF rcf = drawRc;
		rcf.Y = drawRc.Y + (drawRc.Height - boundRc.Height)/2 + TEXT_CENTER_MARGIN;
		graph.DrawString(strNum, strNum.GetLength(), m_fontChildNum, rcf, &stringFormat, &solidBrush);
	}
}


void VividTree::DrawItems( CDC *pDC )
{
	HTREEITEM show_item;
	CRect rc_item;
	CString name, strNum;
	int count = 0;
	bool selected, has_children, has_parent;

	int countTree = GetCount();
	if(countTree <= 0 )
	{
		return;
	}

	show_item = GetFirstVisibleItem();
	if ( show_item == NULL )
		return;
/*
	CRect rcTB(0,0,0,0),rcDraw(0,0,0,0),rcLR(0,0,0,0);
	GetWindowRect(&rcLR);
	ScreenToClient(&rcLR);
    //HOVER的ITEM画背景
	if (GetItemRect( m_hoverItem, rcTB, TRUE ))
	{
		rcDraw.top = rcTB.top;
		rcDraw.bottom = rcTB.bottom;
		rcDraw.right = rcLR.right;
		rcDraw.left = rcLR.left;
		DrawItemBk(pDC, rcDraw, m_strHoverItemBk);	
	}
    //Selected的ITEM画背景
	rcTB.SetRectEmpty();
	rcDraw.SetRectEmpty();
	if (GetItemRect( m_selectedItem, rcTB, TRUE ))
	{
		rcDraw.top = rcTB.top;
		rcDraw.bottom = rcTB.bottom;
		rcDraw.right = rcLR.right;
		rcDraw.left = rcLR.left;
		DrawItemBk(pDC, rcDraw, m_strSelItemBk);
	}
*/	
    //画所有ITEM
	do
	{
		has_children = ItemHasChildren( show_item );
        if (GetParentItem(show_item))
        {
			has_parent = TRUE;
        }else
		{
			has_parent = FALSE;
		}		

		hallNode* pNode = (hallNode*)GetItemData(show_item);
		CRect rcTB(0,0,0,0),rcDraw(0,0,0,0),rcLR(0,0,0,0);
		GetWindowRect(&rcLR);
		ScreenToClient(&rcLR);
		if (m_hoverItem == show_item)//悬停态
		{  
			if (GetItemRect( m_hoverItem, rcTB, TRUE ))
			{
				//Hover的Item画背景
				rcDraw.top = rcTB.top;
				rcDraw.bottom = rcTB.bottom;
				rcDraw.right = rcLR.right;
				rcDraw.left = rcLR.left;
				CString strDraw = has_parent ? m_strChildNodeHoverItemBk : m_strParentNodeHoverItemBk;
				DrawItemBk(pDC, rcDraw, strDraw);	
			}
		}else if (TRUE == pNode->m_CurSelect)//选中态
		{
			//Selected的ITEM画背景
			SetSelItem(show_item);
			rcTB.SetRectEmpty();
			rcDraw.SetRectEmpty();
			GetItemRect( m_selectedItem, rcTB, TRUE );
			rcDraw.top = rcTB.top;
			rcDraw.bottom = rcTB.bottom;
			rcDraw.right = rcLR.right;
			rcDraw.left = rcLR.left;
			CString strDraw = has_parent ? m_strChildNodeSelItemBk : m_strParentNodeSelItemBk;
			DrawItemBk(pDC, rcDraw, strDraw);
		}else //正常态
		{		
			if (!has_parent)//根结点
			{
				GetItemRect( show_item, rcTB, TRUE );
				rcDraw.top = rcTB.top;
				rcDraw.bottom = rcTB.bottom;
				rcDraw.right = rcLR.right;
				rcDraw.left = rcLR.left;
				DrawItemBk(pDC, rcDraw, m_strParentNodeNormalItemBk);
			}
		}

		if (show_item == m_selectedItem)
		{
			selected = TRUE;
		}
		else
		{
			selected = FALSE;
		}

		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{
			CString strBmp = L"";
			CString strExpCon = L"";
			if ( has_children )
			{
				strExpCon = GetExpConPath(show_item);
			}
			strBmp = GetItemBmp(show_item);
			
            if (strBmp != L"")//不管是不是默认的都加载成功的话
            {
				if (has_children && strExpCon != L"")
				{
					CRect rcTemp1 = rc_item;
					rcTemp1.left = rc_item.left - ICON_WIDTH - 2 + 18;
					DrawIcon(pDC, rcTemp1, strExpCon);
				}
				CRect rcTemp2 = rc_item;
				rcTemp2.left = rc_item.left - ICON_WIDTH + 20 + 14;
				DrawIcon(pDC, rcTemp2, strBmp);
            }
		
			name = GetItemText( show_item );
			strNum = pNode->m_PeopleNum;
			rc_item.DeflateRect( 0,1,0,1 );
			DrawNumber(pDC, rc_item, strNum, selected, has_parent);
			DrawName(pDC, rc_item, name, selected, has_parent);
		}
		
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );

    //让选中的ITEM文字变白色
	/*if (m_selectedItem)
	{ 
		CRect tempSelectRc;
		GetItemRect( m_selectedItem, tempSelectRc, TRUE );
		tempSelectRc.DeflateRect(0,1,0,1);
		BOOL selItem_has_parent;
		if (GetParentItem(m_selectedItem))
		{
			selItem_has_parent = TRUE;
		}else
		{
			selItem_has_parent = FALSE;
		}
		hallNode* pNode = (hallNode*)GetItemData(m_selectedItem);		
		strNum = pNode->m_PeopleNum;
		name = pNode->m_Name;

		DrawNumber(pDC, tempSelectRc, strNum, TRUE, selItem_has_parent);
		DrawName(pDC, tempSelectRc, name, TRUE, selItem_has_parent);
	}*/
}

void VividTree::SetBkImgPath(CString imgPath)
{
	m_strImgBk = imgPath;
}

bool VividTree::ItemIsVisible( HTREEITEM item )
{
	HTREEITEM scan_item;
	scan_item = GetFirstVisibleItem();
	while ( scan_item != NULL )
	{
		if ( item == scan_item )
			return true;
		scan_item = GetNextVisibleItem( scan_item );
	}
	return false;
}

void VividTree::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (ItemIsVisible( pNMTreeView->itemNew.hItem ) )
		Invalidate();
	*pResult = 0;
}

void VividTree::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM show_item;
	CRect rc_item,rect;
	BOOL selected;
	DWORD tree_style;
	int state;

	show_item = GetFirstVisibleItem();
	if ( show_item == NULL )
		return;

    tree_style = ::GetWindowLong( m_hWnd, GWL_STYLE ); 

	do
	{
		state = GetItemState( show_item, TVIF_STATE );
		selected = (state & TVIS_SELECTED) && ((this == GetFocus()) || 
			(tree_style & TVS_SHOWSELALWAYS));
		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{   
			rect.top = rc_item.top;
			rect.bottom = rc_item.bottom;
			CRect rc;
			GetWindowRect(&rc);
			ScreenToClient(&rc);
			rect.right = rc.right;
			rect.left = rc.left;

			if(PtInRect(&rect,point))
			{	
				m_hoverItem = show_item;
				Invalidate();
				break;
			}
		}
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void VividTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	HTREEITEM show_item;
	CRect rc_item,rect;
	CPoint curPoint;
	int state;
    
	GetCursorPos(&curPoint);
	ScreenToClient(&curPoint);
	show_item = GetFirstVisibleItem();

	if ( show_item == NULL )
		return;

	do
	{
		state = GetItemState( show_item, TVIF_STATE );
		
		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{   
			rect.top = rc_item.top;
			rect.bottom = rc_item.bottom;
			CRect rc;
			GetWindowRect(&rc);
			ScreenToClient(&rc);
			rect.right = rc.right;
			rect.left = rc.left;			

			if(PtInRect(&rect,curPoint))
			{
				m_selectedItem = show_item;//保存选中的ITEM
				SetSelItem(m_selectedItem);
                OnClickItem(m_selectedItem);//响应点击项
				if(state & TVIS_EXPANDED)
				{
					BOOL mb = Expand(show_item,TVE_COLLAPSE);   
				}
				else
				{
					BOOL mb = Expand(show_item,TVE_EXPAND);
				}		
				Invalidate();
				return;
			}else
			{
				Invalidate();
			}
		}
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void VividTree::SetParentHWMD(HWND parentHWND)
{
	m_parentHWMD = parentHWND;
}

void VividTree::OnClickItem(HTREEITEM hItem)
{
	::SendMessage(m_parentHWMD,WM_NOTIFY_HALL_TREE_CLICK,(WPARAM)hItem,0);
}

void VividTree::SetRoomView(HTREEITEM &hparent,list<hallNode*> &RoomNode)
{
	list<hallNode*> ::iterator iter;

	iter = RoomNode.begin();
	TV_INSERTSTRUCT TreeItem;
	HTREEITEM parentItem = hparent;

	list<hallNode*> ::iterator iterParent = iter ;

	while(iter != RoomNode.end())
	{	
		hallNode* curRoom = *iter;
		TreeItem.hParent = parentItem; 
		TreeItem.hInsertAfter = TVI_LAST ; // TVI_FIRST  TVI_LAST
		TreeItem.itemex.mask = TVIF_TEXT | TVIF_CHILDREN | TVIF_STATE; 
		TreeItem.itemex.state = TVIS_EXPANDED;
		TreeItem.itemex.stateMask = TVIS_EXPANDED;
		TreeItem.itemex.pszText = curRoom->m_Name.GetBuffer();
		TreeItem.itemex.cChildren = curRoom->m_Child.size();
		TreeItem.hParent = InsertItem(&TreeItem);

		SetItemData(TreeItem.hParent, (DWORD_PTR)curRoom);

		curRoom->m_Name.ReleaseBuffer();

		if(curRoom->m_Child.size()>0)
		{
			parentItem = TreeItem.hParent;
			SetRoomView(parentItem,curRoom->m_Child);
			parentItem = hparent;
		}
		iter++;
	}
}

void VividTree::Clear()
{
	m_bInClearStack = true;
	DeleteAllItems();
	m_bInClearStack = false;
	m_selectedItem = NULL;
	m_hoverItem = NULL;
}

CString VividTree::GetDefaultButton()
{
	std::wstring strPath = common::utility::systemhelper::Get179AppPath();
	std::wstring strAdd = _T("resource\\halltree\\default.png");
	strPath += strAdd;

	return strPath.c_str();
}

CString VividTree::GetExpConPath(HTREEITEM hItem)
{
	if (hItem != NULL)
	{
		int state;
		state = GetItemState( hItem, TVIF_STATE );
		if ( state & TVIS_EXPANDED )
		{					
			return m_strExpand;
		}
		else
		{					
			return m_strContract;
		}
	}

	return L"";
}

CString VividTree::GetItemBmp(HTREEITEM hItem)
{
	if (hItem != NULL)
	{
		hallNode* pNode = (hallNode*)GetItemData(hItem);
		CString strBmp = L"";
		if (hItem == m_selectedItem)
		{
			strBmp = pNode->m_pngSelectContract;
		}else
		{
			strBmp = pNode->m_pngNormalExpand;
		}

		if (strBmp == L"")//不成功则加载默认图标
		{
			strBmp = GetDefaultButton();
		}

		return strBmp;
	}

	return L"";
}

void VividTree::SetSelItem(HTREEITEM selItem)
{
	m_selectedItem = selItem;

	HTREEITEM show_item;
    show_item = GetFirstVisibleItem();
    if ( show_item != NULL )
    {
		do 
		{
			hallNode* pNode = (hallNode*)GetItemData(show_item);
			if (m_selectedItem == show_item)
			{
				pNode->m_CurSelect = TRUE;
			}else
			{
				pNode->m_CurSelect = FALSE;
			}

		} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
    }

}

void VividTree::SetAllToUnSelected()
{
	HTREEITEM show_item;
	show_item = GetFirstVisibleItem();
	if ( show_item != NULL )
	{
		do 
		{
			hallNode* pNode = (hallNode*)GetItemData(show_item);

			pNode->m_CurSelect = FALSE;

		} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
	}
}

void VividTree::SetTheFont()//默认微软雅黑，备选宋体
{
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);

	int nFontStyle = 0;
	m_fontParentName = new Font(strFont.c_str(), 13, nFontStyle, UnitPixel);
	m_fontChildName = new Font(strFont.c_str(), 12, nFontStyle, UnitPixel);
	strFont = L"宋体";
	m_fontChildNum = new Font(strFont.c_str(), 12, nFontStyle, UnitPixel);
	nFontStyle |= FontStyleBold;
	m_fontParentNum = new Font(strFont.c_str(), 12, nFontStyle, UnitPixel);

}