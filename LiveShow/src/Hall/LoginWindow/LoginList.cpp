#include "StdAfx.h"
#include "LoginList.h"
#include "utility/FileHelper.h"
//#include "MainFrameDataCenter.h"
#include "GGHelper.h"
#include "../CommonLibrary/include/ui/UIHelper.h"
#include "../CommonLibrary/include/utility/systemhelper.h"

/**********************************************************
*														  *
* 功能说明：描述登陆账号下拉框中的每一个Item项			  *
*														  *
**********************************************************/

IMPLEMENT_DYNAMIC(CLoginListCtrlItem, CWnd)
CLoginListCtrlItem::CLoginListCtrlItem()
{
	m_eState		=	ES_NORMAL;
	m_eImageState	=	ES_NORMAL;
	m_bTrackMouse	=	FALSE;
	m_rectCloseBtn	=	CRect(0,0,0,0);	
	
	m_strShowAccount	=	L"";
	m_strNickName	=	L"";

    m_pHoverImage = new common::ui::CImageImpl();
    m_pCloseImage = new common::ui::CImageImpl();
}

CLoginListCtrlItem::~CLoginListCtrlItem()
{
	if (m_pLargeImage != NULL) {
		m_pLargeImage->Destroy();
		m_pLargeImage = NULL;
	}

	if (m_pHoverImage != NULL) {
		m_pHoverImage->Destroy();
		m_pHoverImage = NULL;
	}

    if (m_pCloseImage != NULL) {
        m_pCloseImage->Destroy();
        m_pCloseImage = NULL;
    }
}

BEGIN_MESSAGE_MAP(CLoginListCtrlItem, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL	
CLoginListCtrlItem::Create(const CRect& rc, CWnd* pParentWnd, UINT uId)
{
	BOOL bRet = __super::Create(AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_IBEAM)), 
		_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rc, pParentWnd, uId);

	// Item被选中的时候的背景色
	CString strImagePath = common::utility::systemhelper::Get179AppPath().c_str();
	strImagePath += L"resource\\other\\Login_AccountList_itemBk_H.png";
	bRet = m_pHoverImage->Load(strImagePath);
	ASSERT( bRet == TRUE);
	if( bRet == FALSE)
	{
		AfxMessageBox(L"加载图片失败");
	}

	return bRet;
}


void 
CLoginListCtrlItem::OnPaint()
{
	CPaintDC dc(this);
	common::ui::CClientRect rcClient(this);

	// 背景色
	{
		CBrush backgroundBrush( RGB(255,255,255));
		FillRect( dc.GetSafeHdc(),&rcClient, (HBRUSH)backgroundBrush.GetSafeHandle());
	}

	{
		CBrush borderBrush( RGB(255,255,255));
		FrameRect( dc.GetSafeHdc(),&rcClient, (HBRUSH)borderBrush.GetSafeHandle());
	}

	// 鼠标移动到Item上的时候
	if( m_eState == ES_HOVER)
	{
		// 绘制背景
		m_pHoverImage->Render( dc.GetSafeHdc(), rcClient, NULL, NULL);

		// 绘制大头像
		if( m_pLargeImage != NULL )
		{
			CRect dstRect;

			dstRect.left	= CLoginListCtrlItem::IMAGE_HOVER_LEFT_OFFSET - 1;
			dstRect.top		= CLoginListCtrlItem::IMAGE_HOVER_TOP_OFFSET - 1;
			dstRect.right	=	dstRect.left	+ LARGE_IMAGE_WIDTH + 2;
			dstRect.bottom	=	dstRect.top		+ LARGE_IMAGE_WIDTH + 2;

			CBrush whiteBrush( RGB(255,255,255));
			FillRect(dc.GetSafeHdc(), &dstRect, (HBRUSH)whiteBrush.GetSafeHandle());

			dstRect.left	= CLoginListCtrlItem::IMAGE_HOVER_LEFT_OFFSET;
			dstRect.top		= CLoginListCtrlItem::IMAGE_HOVER_TOP_OFFSET;
			dstRect.right	=	dstRect.left	+ LARGE_IMAGE_WIDTH;
			dstRect.bottom	=	dstRect.top		+ LARGE_IMAGE_WIDTH;
			m_pLargeImage->Render(dc.GetSafeHdc(), dstRect);
		}

		// 绘制文字和账号
		LOGFONT lf;
		memset(&lf, 0x0,sizeof LOGFONT);
		lf.lfHeight = 12;
		lf.lfWeight = 400;
		wcscpy(lf.lfFaceName ,L"宋体");

		CFont cFont;
		cFont.CreateFontIndirect(&lf);
		CFont* pOldFont = dc.SelectObject( &cFont);

		if( m_strNickName != L"")
		{
			dc.SetBkMode( TRANSPARENT);

			CString strOutputText =	m_strNickName;			
			
            dc.SetTextColor(RGB(0,56,85));
			dc.TextOut( TEXT_HOVER_LEFT_OFFSET, 5, strOutputText);
		}
		
		SetTextColor( dc.GetSafeHdc(), RGB(255,255,255));
		dc.TextOut( TEXT_HOVER_LEFT_OFFSET, 26, m_strShowAccount);
		dc.SelectObject( pOldFont);

		// 绘制右侧的关闭按钮
		m_rectCloseBtn.left		=	rcClient.right - 22;
		m_rectCloseBtn.right	=	m_rectCloseBtn.left	+ 12;
		m_rectCloseBtn.top		=	rcClient.Height()/2 - 7;
		m_rectCloseBtn.bottom	=	m_rectCloseBtn.top + 11;

		CString strImagePath;		
		strImagePath.Format(L"%sresource\\other\\Login_AccountList_btnClose.png", common::utility::systemhelper::Get179AppPath().c_str());

		BOOL bRet = m_pCloseImage->Load(strImagePath);
		ASSERT( bRet == TRUE);
		m_pCloseImage->Render(dc.GetSafeHdc(), m_rectCloseBtn, NULL, NULL);

	}
	else
	{
		// 绘制小头像
		if( m_pLargeImage != NULL )
		{
			CRect dstRect;
			dstRect.left	= CLoginListCtrlItem::IMAGE_NORMAL_LEFT_OFFSET;
			dstRect.top		= CLoginListCtrlItem::IMAGE_NORMAL_TOP_OFFSET;
			dstRect.right	=	dstRect.left	+ THUMBNAIL_IMAGE_WIDTH;
			dstRect.bottom	=	dstRect.top		+ THUMBNAIL_IMAGE_WIDTH;
			m_pLargeImage->Render(dc.GetSafeHdc(), dstRect);
		}

		LOGFONT lf;
		memset(&lf, 0x0,sizeof LOGFONT);
		lf.lfHeight = 12;
		lf.lfWeight = 400;
		wcscpy(lf.lfFaceName ,L"宋体");

		CFont cFont;
		cFont.CreateFontIndirect(&lf);
		CFont* pOldFont = dc.SelectObject( &cFont);		


		dc.SetBkMode( TRANSPARENT);
		dc.SetTextColor( RGB(0,90,128));
		dc.TextOutW( TEXT_NORMAL_LEFT_OFFSET, 5, m_strShowAccount);
		dc.SelectObject( pOldFont);
	}
}

LRESULT 
CLoginListCtrlItem::OnMouseLeave(WPARAM w, LPARAM l)
{
	TRACE(L"Mouse Leave Index: %d\n", m_nIndex);
	//m_eState	=	ES_NORMAL;
	m_bTrackMouse	=	FALSE;

	::ReleaseCapture();

	CWnd* pParent = GetParent();
	if( pParent)
	{
		pParent->SendMessage(WM_ITEM_UNSELECT, 0 ,0);
	}

	return 0;
}

void 
CLoginListCtrlItem::OnMouseMove(UINT nFlags, CPoint point)
{
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	if( m_bTrackMouse == FALSE)	
	{
		TRACE(L"Mouse Enter Index: %d\n", m_nIndex);

		TRACKMOUSEEVENT tme;
		tme.cbSize		= sizeof(tme);
		tme.hwndTrack	= GetSafeHwnd();
		tme.dwFlags		= TME_LEAVE;
		tme.dwHoverTime = 1;

		m_bTrackMouse	= _TrackMouseEvent(&tme);	// post WM_MOUSELEAVE when mouse leave the wnd		
		m_eState		= ES_HOVER;

		CWnd* pParent	= GetParent();
		if( pParent)
		{
			pParent->SendMessage( WM_ITEM_SELECT, m_nIndex,0 );
		}

		m_eState	=	ES_HOVER;
	}

	common::ui::CClientRect rcClient(this);
	if(rcClient.PtInRect( point) == FALSE)
	{
		OnMouseLeave(0,0);
	}

	if( m_rectCloseBtn.PtInRect(point))
	{
		if( m_eImageState != ES_HOVER)
		{
			ClientToScreen(&point);
			point.y += 20;

			m_ctrlTips.Show( L"删除账号记录", this, &point);
			m_eImageState	=	ES_HOVER;
			InvalidateRect(&m_rectCloseBtn);
		}
	}
	else
	{
		if( m_eImageState != ES_NORMAL)
		{
			m_ctrlTips.Hide();
			m_eImageState	=	ES_NORMAL;
			InvalidateRect(&m_rectCloseBtn);
		}
	}
}

void 
CLoginListCtrlItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = ::ReleaseCapture();
	ASSERT( TRUE == bRet);
	
	CWnd* pParent = GetParent();
	if( m_rectCloseBtn.PtInRect(point))
	{
		if( pParent)
			pParent->SendMessage( WM_ITEM_DELETE, m_nUin, 0);
	}
	else
	{
		if( pParent)
			pParent->SendMessage( WM_ITEM_CLICK, m_nIndex, 0);
	}
}

IMPLEMENT_DYNAMIC(CLoginListCtrl, CWnd)
CLoginListCtrl::CLoginListCtrl(void)
{
	for( size_t i=0; i<m_vpLoginListItem.size();i++)
	{
		if( m_vpLoginListItem[i])
		{
			delete m_vpLoginListItem[i];
			m_vpLoginListItem[i] = NULL;
		}
	}

	m_vpLoginListItem.clear();

	m_pAssoicatedWnd	=	NULL;
	m_nSelIndex		=	-1;
	m_pSkinEngine	=	NULL;
	m_bHide			=	TRUE;
}



/**********************************************************
*														  *
* 功能说明：描述登陆账号下拉框							  *
*														  *
**********************************************************/
CLoginListCtrl::~CLoginListCtrl(void)
{
	m_nSelIndex	=	-1;
	DeleteAllItem();
}

BEGIN_MESSAGE_MAP(CLoginListCtrl, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE( WM_ITEM_SELECT,		OnItemSelected)
	ON_MESSAGE( WM_ITEM_UNSELECT,	OnItemUnSelected)
	ON_MESSAGE( WM_ITEM_CLICK,		OnItemClick)
	ON_MESSAGE( WM_ITEM_DELETE,		OnItemDelete)
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL	
CLoginListCtrl::Create(const CRect& rc, CWnd* pParentWnd, UINT uId)
{
	BOOL bRet = CreateEx(
		WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 
		AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_IBEAM)),		
		_T(""), 
		WS_POPUP, 
		rc, 
		NULL, 
		NULL,
		NULL);

	ASSERT( bRet == TRUE);

	if( bRet == TRUE)
	{
		bRet = ModifyStyle( 0, WS_CLIPCHILDREN, 0);
		ASSERT( bRet == TRUE);
	}
	m_pParentWnd = pParentWnd;

	return bRet;
}

BOOL 
CLoginListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int) pMsg->wParam;
		if( nVirtKey == VK_DOWN || nVirtKey == VK_RIGHT)
		{
			ChangeNextSelect();
		}
		else if( nVirtKey == VK_UP || nVirtKey == VK_LEFT)
		{
			ChangePrevSelect();
		}
		else if( nVirtKey == VK_RETURN)
		{
			if( m_nSelIndex != -1)
			{
				CLoginListCtrlItem* pItem = m_vpLoginListItem[m_nSelIndex];
				if( pItem)
					pItem->SendMessage( WM_LBUTTONDOWN, 0, 0);
			}
		}

		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void
CLoginListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	if( m_bTrackMouse == FALSE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize		= sizeof(tme);
		tme.hwndTrack	= GetSafeHwnd();
		tme.dwFlags		= TME_LEAVE;
		tme.dwHoverTime = 1;

		m_bTrackMouse	= _TrackMouseEvent(&tme);	// post WM_MOUSELEAVE when mouse leave the wnd		
	}

	for( int i=0;i<m_vpLoginListItem.size();i++)
	{
		CLoginListCtrlItem* pItem = m_vpLoginListItem[i];
		if( pItem)
		{
			common::ui::CClientRect rcClient( pItem, this);
			if( rcClient.PtInRect( point))
			{
				::SetCapture( pItem->GetSafeHwnd());
				break;
			}
		}
	}

	common::ui::CClientRect rcClient(this);
	if(rcClient.PtInRect( point) == FALSE)
	{
		OnMouseLeave(0,0);
	}
}

void	
CLoginListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	common::ui::CClientRect rc(this);
	if( rc.PtInRect( point) == FALSE)
	{
		::ReleaseCapture();
		Hide();		
		::SendMessage(m_pParentWnd->GetSafeHwnd(),WM_LISTCTRL_SHOWHIDE,0,0);	
		
	}
}

LRESULT 
CLoginListCtrl::OnMouseLeave(WPARAM w, LPARAM l)
{
	
	return 0;
}

int		
CLoginListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL)
	{
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"LoginList");
		ASSERT(result == S_OK && __LINE__);
	}

	return 0;
}

VOID	
CLoginListCtrl::SetAssoicatedWnd( CWnd* pWnd)
{
	m_pAssoicatedWnd	=	pWnd;
}

VOID	
CLoginListCtrl::Show( CPoint& point)
{
	common::ui::CClientRect rcClient(this);

	CRect	rcNewClient;
	rcNewClient.left	= point.x;
	rcNewClient.top		= point.y;
	rcNewClient.right	= point.x + rcClient.Width();	
	if(rcClient.Height() ==0)
	{
		rcNewClient.bottom  = point.y + 50;   //空用户列表时
	}
	else
	{
		rcNewClient.bottom	= point.y + rcClient.Height();
	}
	  

	::MoveWindow( GetSafeHwnd(), rcNewClient.left, rcNewClient.top,rcNewClient.Width(),rcNewClient.Height(), TRUE);
	ShowWindow( SW_SHOW);

	::SetCapture(GetSafeHwnd());
}

VOID
CLoginListCtrl::Hide()
{
	ShowWindow( SW_HIDE);
}

BOOL	
CLoginListCtrl::AddItem( UINT32	nUin,  CString strShowAccount,  CString strNickName,
						common::ui::CImageImpl* imageWrapper)
{
	if( nUin == 0 || strShowAccount == _T(""))
		return FALSE;

	// 检查uin是否已经存在
	for( int i=0; i<m_vpLoginListItem.size();i++)
	{
		CLoginListCtrlItem* pItem = m_vpLoginListItem[i];
		if( pItem)
		{
			if( pItem->m_nUin == nUin )
				return FALSE;
		}
	}

	CLoginListCtrlItem* pItem = new CLoginListCtrlItem;
	pItem->m_pLargeImage	=	imageWrapper;
	pItem->m_nUin = nUin;
	pItem->m_strShowAccount	=	strShowAccount;	
	pItem->m_strNickName	=	strNickName;
	pItem->m_nIndex			=	m_vpLoginListItem.size();
	
	common::ui::CClientRect rcListClient(this);
	CRect rcItem;
	rcItem.left		= rcListClient.left + CLoginListCtrlItem::CONTENT_LEFT_GAP;
	rcItem.right	= rcListClient.right - CLoginListCtrlItem::CONTENT_RIGHT_GAP;
	rcItem.top		= m_vpLoginListItem.size()*( CLoginListCtrlItem::LI_NORMAL_HEIGHT) + CLoginListCtrlItem::CONTENT_TOP_GAP;
	rcItem.bottom	= rcItem.top + CLoginListCtrlItem::LI_NORMAL_HEIGHT;
	pItem->Create( rcItem, this, 1000 + m_vpLoginListItem.size());
	pItem->ShowWindow( SW_HIDE);

	m_vpLoginListItem.push_back( pItem);

	if( m_vpLoginListItem.size() > 0)
	{
		// 调整最外面窗口的大小
		common::ui::CClientRect rcBottom( m_vpLoginListItem[ m_vpLoginListItem.size()-1], this);
		common::ui::CClientRect rcTop( m_vpLoginListItem[0], this);

		//common::ui::CClientRect rcList( this, GetParent());
		common::ui::CWindowRect rcList( this);
		common::ui::SetRectHeight( &rcList, rcBottom.bottom + CLoginListCtrlItem::CONTENT_BOTTOM_GAP);
		MoveWindow( &rcList, TRUE);
	}

	return TRUE;
}

BOOL	
CLoginListCtrl::InsertItem( int nIndex, UINT32	nUin,CString strShowAccount,  CString strNickName,
				   common::ui::CImageImpl* imageWrapper)
{
	if( nUin == 0 || strShowAccount == L"" || nIndex < 0 )
		return FALSE;

	if( ( m_vpLoginListItem.size() == 0) && nIndex > 0)
		return	FALSE;

	if( nIndex > m_vpLoginListItem.size() - 1)
		return  FALSE;

	// 检查uin是否已经存在
	for( int i=0; i<m_vpLoginListItem.size();i++)
	{
		CLoginListCtrlItem* pItem = m_vpLoginListItem[i];
		if( pItem)
		{
			if( pItem->m_nUin == nUin || pItem->m_strShowAccount == strShowAccount)
				return FALSE;
		}
	}

	CLoginListCtrlItem* pItem = new CLoginListCtrlItem;
	pItem->m_pLargeImage	=	imageWrapper;
	pItem->m_nUin = nUin;
	pItem->m_strShowAccount	=	strShowAccount;	
	pItem->m_strNickName	=	strNickName;
	pItem->m_nIndex = m_vpLoginListItem.size();

	common::ui::CClientRect rcListClient(this);
	CRect rcItem;
	rcItem.left		= rcListClient.left + CLoginListCtrlItem::CONTENT_LEFT_GAP;
	rcItem.right	= rcListClient.right - CLoginListCtrlItem::CONTENT_RIGHT_GAP;
	rcItem.top		= m_vpLoginListItem.size()*( CLoginListCtrlItem::LI_NORMAL_HEIGHT) + CLoginListCtrlItem::CONTENT_TOP_GAP;
	rcItem.bottom	= rcItem.top + CLoginListCtrlItem::LI_NORMAL_HEIGHT;
	pItem->Create( rcItem, this, 1000 + m_vpLoginListItem.size());
	pItem->ShowWindow( SW_HIDE);


	if( m_vpLoginListItem.size() == 0 )
		m_vpLoginListItem.push_back( pItem);
	else
		m_vpLoginListItem.insert( m_vpLoginListItem.begin() + nIndex, pItem);

	if( m_vpLoginListItem.size() > 0)
	{
		// 调整最外面窗口的大小
		common::ui::CClientRect rcBottom( m_vpLoginListItem[ m_vpLoginListItem.size()-1], this);
		common::ui::CClientRect rcTop( m_vpLoginListItem[0], this);

		//common::ui::CClientRect rcList( this, GetParent());
		common::ui::CWindowRect rcList( this);
		common::ui::SetRectHeight( &rcList, rcBottom.bottom + CLoginListCtrlItem::CONTENT_BOTTOM_GAP);
		MoveWindow( &rcList, TRUE);
	}

	return TRUE;
}

void
CLoginListCtrl::DeleteAllItem()
{
	std::vector< CLoginListCtrlItem*>::iterator itr = m_vpLoginListItem.begin();
	for( ; itr != m_vpLoginListItem.end(); itr++)
	{
		if(*itr)
		{
			CLoginListCtrlItem* pItem = (*itr);
			delete pItem; 
			pItem = NULL;
		}
	}

	m_vpLoginListItem.clear();
}

void	
CLoginListCtrl::DeleteItem( UINT32	nUin)
{
	std::vector< CLoginListCtrlItem*>::iterator itr = m_vpLoginListItem.begin();
	for( ; itr != m_vpLoginListItem.end(); itr++)
	{
		if( (*itr) && (*itr)->m_nUin == nUin)
		{
			CLoginListCtrlItem* pItem = (*itr);
			pItem->ShowWindow( SW_HIDE);
			delete pItem; pItem = NULL;

			m_vpLoginListItem.erase( itr);
			return;
		}
	}
}

void	
CLoginListCtrl::DeleteItem( CString	strShowAccount)
{
	INT nIndex = -1;
	std::vector< CLoginListCtrlItem*>::iterator itr = m_vpLoginListItem.begin();
	for( ; itr != m_vpLoginListItem.end(); itr++)
	{
		if( (*itr) && (*itr)->m_strShowAccount == strShowAccount)
		{
			nIndex = itr - m_vpLoginListItem.begin();

			CLoginListCtrlItem* pItem = (*itr);
			pItem->ShowWindow( SW_HIDE);
			delete pItem; pItem = NULL;

			m_vpLoginListItem.erase( itr);

			break;
		}
	}

	for( ; nIndex < m_vpLoginListItem.size() ;nIndex++)
	{
		CLoginListCtrlItem* pItem = m_vpLoginListItem[nIndex];
		if( pItem)
		{
			pItem->m_nIndex	=	nIndex;
			common::ui::CClientRect rcListClient(this);
			CRect rcItem;
			rcItem.left		= rcListClient.left + CLoginListCtrlItem::CONTENT_LEFT_GAP;
			rcItem.right	= rcListClient.right - CLoginListCtrlItem::CONTENT_RIGHT_GAP;
			rcItem.top		= nIndex*( CLoginListCtrlItem::LI_NORMAL_HEIGHT) + CLoginListCtrlItem::CONTENT_TOP_GAP;
			rcItem.bottom	= rcItem.top + CLoginListCtrlItem::LI_NORMAL_HEIGHT;
			pItem->MoveWindow(&rcItem);
		}
	}

	if( m_vpLoginListItem.size() > 0)
	{
		// 调整最外面窗口的大小
		common::ui::CClientRect rcBottom( m_vpLoginListItem[ m_vpLoginListItem.size()-1], this);
		common::ui::CClientRect rcTop( m_vpLoginListItem[0], this);

		//common::ui::CClientRect rcList( this, GetParent());
		common::ui::CWindowRect rcList( this);
		common::ui::SetRectHeight( &rcList, rcBottom.bottom + CLoginListCtrlItem::CONTENT_BOTTOM_GAP);
		MoveWindow( &rcList, TRUE);
	}	
}

uin		
CLoginListCtrl::GetUinAtIndex( UINT nIndex)
{
	if( m_vpLoginListItem.size() == 0)
		return 0;

	if( nIndex > m_vpLoginListItem.size() - 1)
		return 0;

	return m_vpLoginListItem[nIndex]->m_nUin;
}

wstring	
CLoginListCtrl::GetAccountAtIndex( UINT nIndex)
{
	if( m_vpLoginListItem.size() == 0)
		return L"";

	if( nIndex > m_vpLoginListItem.size() - 1 )
		return L"";
	
	if(  m_vpLoginListItem[nIndex]->m_strShowAccount != L"")
		return m_vpLoginListItem[nIndex]->m_strShowAccount;
}

wstring 
CLoginListCtrl::GetShowUinAtIndex( UINT nIndex)
{
	if( m_vpLoginListItem.size() == 0)
		return L"";

	if( nIndex > m_vpLoginListItem.size() - 1 )
		return L"";

	return m_vpLoginListItem[nIndex]->m_strShowAccount;
}


int
CLoginListCtrl::SetCurSel(int nSelect)
{
	if( nSelect == -1 || nSelect >= m_vpLoginListItem.size())
	{
		return -1;
	}

	CLoginListCtrlItem* pNextItem = m_vpLoginListItem[nSelect];
	if( pNextItem)
	{
		CWnd* pParent	= pNextItem->GetParent();
		if( pParent)
		{
			pParent->SendMessage( WM_ITEM_SELECT, nSelect,0 );
		}

		pNextItem->m_eState	=	ES_HOVER;
		pNextItem->Invalidate(TRUE);

		TRACE(L"Select Item Index: %d\n", nSelect);
	}

	m_nSelIndex = nSelect;

}

UINT 
CLoginListCtrl::OnGetDlgCode()
{
	return __super::OnGetDlgCode() | DLGC_WANTARROWS;
}


void 
CLoginListCtrl::OnPaint()
{
	CPaintDC dc(this);

	// 边框
	common::ui::CClientRect rcClient(this);
	CBrush borderBrush(RGB(255,0,0));
	FrameRect(dc.GetSafeHdc(), &rcClient, (HBRUSH)borderBrush.GetSafeHandle());

	rcClient.DeflateRect( 2,2);

	CBrush backBrush(RGB(255,0,255));
	FillRect(dc.GetSafeHdc(), &rcClient, (HBRUSH)backBrush.GetSafeHandle());

	// 显示所有的
	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
		{
			common::ui::CClientRect rc(m_vpLoginListItem[i]);
			m_vpLoginListItem[i]->ShowWindow( SW_SHOW);
		}
	}
}

void	
CLoginListCtrl::DrawItem()
{
	common::ui::CClientRect rcList(this);

	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( i==0)
		{
			CRect rcItem = rcList;
			if( m_vpLoginListItem[i]->m_eState == ES_NORMAL)
			{
				common::ui::SetRectHeight( &rcItem,CLoginListCtrlItem::LI_NORMAL_HEIGHT);
			}
			else
			{
				common::ui::SetRectHeight( &rcItem, CLoginListCtrlItem::LI_HOVER_HEIGHT);
			}

			rcItem.left		= rcList.left + CLoginListCtrlItem::CONTENT_RIGHT_GAP;
			rcItem.right	= rcList.right - CLoginListCtrlItem::CONTENT_RIGHT_GAP;
			rcItem.top += CLoginListCtrlItem::CONTENT_TOP_GAP;
			rcItem.bottom += CLoginListCtrlItem::CONTENT_TOP_GAP;

			m_vpLoginListItem[i]->MoveWindow(&rcItem, TRUE);
			m_vpLoginListItem[i]->Invalidate( TRUE);
		}
		else
		{
			common::ui::CClientRect rcPrev( m_vpLoginListItem[i-1], this);
			rcPrev.top = rcPrev.bottom;

			if( m_vpLoginListItem[i]->m_eState == ES_NORMAL)
			{
				common::ui::SetRectHeight( &rcPrev, CLoginListCtrlItem::LI_NORMAL_HEIGHT);
			}
			else
			{
				common::ui::SetRectHeight( &rcPrev, CLoginListCtrlItem::LI_HOVER_HEIGHT);
			}

			m_vpLoginListItem[i]->MoveWindow(&rcPrev, TRUE);
			m_vpLoginListItem[i]->Invalidate( TRUE);
		}
	}

	if( m_vpLoginListItem.size() > 0)
	{
		// 调整最外面窗口的大小
		common::ui::CClientRect rcBottom( m_vpLoginListItem[ m_vpLoginListItem.size()-1], this);
		common::ui::CClientRect rcTop( m_vpLoginListItem[0], this);

		//common::ui::CClientRect rcList( this, GetParent());
		common::ui::CWindowRect rcList( this);
		common::ui::SetRectHeight( &rcList, rcBottom.bottom + CLoginListCtrlItem::CONTENT_BOTTOM_GAP);
		MoveWindow( &rcList, TRUE);
	}
}

LRESULT
CLoginListCtrl::OnItemSelected( WPARAM w, LPARAM l)
{
	m_nSelIndex = (UINT)w;
	
	if( m_nSelIndex > m_vpLoginListItem.size() - 1)
	{
		ASSERT(0);
	}

	// 通知nSelectIndex之前的Item全部回复正常状态
	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
			m_vpLoginListItem[i]->m_eState = ES_NORMAL;
	}
	m_vpLoginListItem[m_nSelIndex]->m_eState = ES_HOVER;

	DrawItem();

	return 0;
}

LRESULT	
CLoginListCtrl::OnItemUnSelected( WPARAM w, LPARAM l)
{
	DrawItem();

	if( m_bHide == false)
	{
		::SetCapture( GetSafeHwnd());
	}

	return 0;
}

LRESULT 
CLoginListCtrl::OnItemClick( WPARAM w, LPARAM l)
{
	m_nSelIndex	=	w;

	if( m_pAssoicatedWnd)
		m_pAssoicatedWnd->SendMessage( WM_ITEM_CLICK, w,l);

	ShowWindow( SW_HIDE);

	::ReleaseCapture();

	return 0;
}

LRESULT 
CLoginListCtrl::OnItemDelete( WPARAM w, LPARAM l)
{
	if( m_pAssoicatedWnd )
		m_pAssoicatedWnd->SendMessage( WM_ITEM_DELETE, w,l);

	return 0;
}


int		
CLoginListCtrl::GetCount() const
{
	return m_vpLoginListItem.size();
}

int		
CLoginListCtrl::GetCurSel() const
{
	return m_nSelIndex;
}

/**
* @function		查找给定的UIN的索引
* @param		nUin	需要查找的UIN
* @return		返回给定UIN的索引，如果没有找到，则返回-1
*/
int		
CLoginListCtrl::Find( uin nUin)
{
	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
		{
			if( m_vpLoginListItem[i]->m_nUin == nUin)
				return i;
		}
	}

	return -1;
}

/**
* @function		查找给定的账号的索引
* @param		strShowAccount	需要查找的账号
* @return		返回给定账号的索引，如果没有找到，则返回-1
*/
int		
CLoginListCtrl::Find( CString strShowAccount)
{
	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
		{
			if( m_vpLoginListItem[i]->m_strShowAccount == strShowAccount)
				return i;
		}
	}

	return -1;
}

int		
CLoginListCtrl::SelectAccount( int nStartAfter, LPCTSTR	lpszString)
{
	if( lpszString == NULL || nStartAfter < 0)
		return -1;

	if( m_vpLoginListItem.size() == 0)
		return -1;

	//if( common::utility::systemhelper::IsDigit(lpszString))
	{
		for( int i=nStartAfter; i<m_vpLoginListItem.size(); i++)
		{
			if( m_vpLoginListItem[i])
			{
				CString strPrefix = m_vpLoginListItem[i]->m_strShowAccount.Left( wcslen( lpszString));
				if( strPrefix.CompareNoCase( lpszString) == 0)
					return i;
			}
		}	
	}
		

	return -1;
}

int		
CLoginListCtrl::SelectUin( int nStartAfter, uin nUin)
{
	if( nUin == 0 || nStartAfter < 0)
		return -1;

	if( m_vpLoginListItem.size() == 0)
		return -1;

	for( int i=nStartAfter; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
		{
			if( m_vpLoginListItem[i]->m_nUin == nUin)
				return i;
		}
	}

	return -1;

}

void	
CLoginListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	m_bHide	= TRUE;
	Hide();
}

void	
CLoginListCtrl::ChangeNextSelect()
{
	INT nPrevIndex = m_nSelIndex;
	INT nToBeSelectedIndex = 0;

	if( m_nSelIndex == -1)
	{
		nToBeSelectedIndex = 0;
	}
	else  if( m_nSelIndex + 1 >= m_vpLoginListItem.size())
	{
		nToBeSelectedIndex = 0;
	}
	else
	{
		nToBeSelectedIndex = m_nSelIndex + 1;
	}

	// 通知nSelectIndex之前的Item全部回复正常状态
	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
			m_vpLoginListItem[i]->m_eState = ES_NORMAL;
	}
	m_vpLoginListItem[nToBeSelectedIndex]->m_eState = ES_HOVER;
	m_nSelIndex = nToBeSelectedIndex;

	CLoginListCtrlItem* pNextItem = m_vpLoginListItem[nToBeSelectedIndex];
	if( pNextItem)
	{
		CWnd* pParent	= pNextItem->GetParent();
		if( pParent)
		{
			pParent->SendMessage( WM_ITEM_SELECT, nToBeSelectedIndex,0 );
		}

		pNextItem->m_eState	=	ES_HOVER;
		pNextItem->Invalidate(TRUE);

		TRACE(L"Select Item Index: %d\n", nToBeSelectedIndex);
	}

	if( nPrevIndex != -1)
	{
		CLoginListCtrlItem* pPrevItem = m_vpLoginListItem[nPrevIndex];
		if( pPrevItem)
		{
			pPrevItem->m_eState	=	ES_NORMAL;
			pPrevItem->m_bTrackMouse	=	FALSE;

			::ReleaseCapture();

			CWnd* pParent = pPrevItem->GetParent();
			if( pParent)
			{
				pParent->SendMessage(WM_ITEM_UNSELECT, 0 ,0);
			}

			pPrevItem->Invalidate( TRUE);
			TRACE(L"UnSelect Item Index: %d\n", nPrevIndex);
		}
	}

	DrawItem();
}

void	
CLoginListCtrl::ChangePrevSelect()
{
	INT nPrevIndex = m_nSelIndex;
	INT nToBeSelectedIndex = 0;

	if( m_nSelIndex == -1)
	{
		return;
	}
	else  if( m_nSelIndex == 0)
	{
		nToBeSelectedIndex = m_vpLoginListItem.size() - 1;
	}
	else
	{
		nToBeSelectedIndex = m_nSelIndex - 1;
	}

	// 通知nSelectIndex之前的Item全部回复正常状态
	for( int i=0; i<m_vpLoginListItem.size(); i++)
	{
		if( m_vpLoginListItem[i])
			m_vpLoginListItem[i]->m_eState = ES_NORMAL;
	}
	m_vpLoginListItem[nToBeSelectedIndex]->m_eState = ES_HOVER;
	m_nSelIndex = nToBeSelectedIndex;

	CLoginListCtrlItem* pNextItem = m_vpLoginListItem[nToBeSelectedIndex];
	if( pNextItem)
	{
		CWnd* pParent	= pNextItem->GetParent();
		if( pParent)
		{
			pParent->SendMessage( WM_ITEM_SELECT, nToBeSelectedIndex,0 );
		}

		pNextItem->m_eState	=	ES_HOVER;
		pNextItem->Invalidate(TRUE);

		TRACE(L"Select Item Index: %d\n", nToBeSelectedIndex);
	}

	if( nPrevIndex != -1)
	{
		CLoginListCtrlItem* pPrevItem = m_vpLoginListItem[nPrevIndex];
		if( pPrevItem)
		{
			pPrevItem->m_eState	=	ES_NORMAL;
			pPrevItem->m_bTrackMouse	=	FALSE;

			::ReleaseCapture();

			CWnd* pParent = pPrevItem->GetParent();
			if( pParent)
			{
				pParent->SendMessage(WM_ITEM_UNSELECT, 0 ,0);
			}

			pPrevItem->Invalidate( TRUE);
			TRACE(L"UnSelect Item Index: %d\n", nPrevIndex);
		}
	}

	DrawItem();
}
