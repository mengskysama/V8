#include "StdAfx.h"
#include "179TipCtrl.h"
#include "LoginDlg.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/ui/UIHelper.h"
#include "..\HallWindow\HallDlg.h"

#define  TIP_ARROW_HEIGHT    10
#define  UP_CORNER_RADIUS    2
#define  DOWN_CORNER_RADIUS  2

C179TipCtrl::C179TipCtrl(eArrowPos arrowpos)
:m_arrowPos( arrowpos )
{
	m_pParentWnd = NULL;
	m_colorBorder = RGB(227, 123, 0);
	m_strTipText = L"";
	m_colorTipText = RGB(156,86,0);
	m_strBtnText = L"";
	m_strImgPath = L"";
	m_colorBtnText = RGB(15,164,231);
	m_bkColor = RGB(243,244,238);
	m_bkBeginColor = RGB(255,230,214);
	m_bkEndColor = RGB(255,255,255);
	m_bHided = m_bClose = FALSE;

	CRect rc(0,0,0,0);
	m_rcBtnText = m_rcIconClose = m_rcTipText = rc;
	m_showRgn = NULL;
	m_showRgn = ::CreateRectRgn(0,0,0,0);
}

C179TipCtrl::~C179TipCtrl(void)
{
	if (m_font.GetSafeHandle())
	{
		DeleteObject(m_font);
	}
	if (m_showRgn != NULL)
	{
		DeleteObject(m_showRgn);
	}
}

BEGIN_MESSAGE_MAP(C179TipCtrl, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL C179TipCtrl::CreateEx(CWnd *pwndParent, UINT uId)
{
	CRect rc(0,0,0,0) ;
	if ( CWnd::CreateEx(0,AfxRegisterWndClass(0),_T(""),WS_POPUP|WS_CLIPCHILDREN|WS_VISIBLE,rc,pwndParent,uId))
	{
		m_pParentWnd = pwndParent ;

		std::wstring strFont;
		common::utility::systemhelper::GetDefaultFontName(strFont);

		CPaintDC dc(this);
		m_font.CreatePointFont( 90 ,L"宋体",&dc);		
	//	m_font.CreateFont(11,7,0,0,400,false,false,false,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,L"宋体");
	//	::SetWindowPos(NULL, HWND_TOP,0,0,0,0,SWP_HIDEWINDOW);
		return TRUE;
	}

	return FALSE;
}

void C179TipCtrl::SetArrowPos(int iPos)
{
	m_arrowPos = iPos;
}

void C179TipCtrl::SetBorderColor(COLORREF borderColor)
{
	m_colorBorder = borderColor;
}

void C179TipCtrl::SetTipText(CString strText)
{
	m_strTipText = strText;
}

void C179TipCtrl::SetTipTextColor(COLORREF textColor)
{
	m_colorTipText = textColor;
}

void C179TipCtrl::SetButtonText(CString strBtnText)
{
	m_strBtnText = strBtnText;
}

void C179TipCtrl::SetButtonTextColor(COLORREF btnColor)
{
	m_colorBtnText = btnColor;
}

void C179TipCtrl::SetClose(BOOL bClose)
{
	m_bClose = bClose;
}

void C179TipCtrl::SetTipBkColor(COLORREF bkColor)
{
	m_bkColor = bkColor;
}

void C179TipCtrl::ShowTip(int nArrowLeft , int& nWidth , int& nHeight )
{
	//根据内容制定大小

	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);
	CRect rcClient(0,0,0,0);
	CSize szTipText(0,0), szBtnText(0,0), szIcon(0,0);
	if (m_strTipText != L"")
	{
		szTipText = common::ui::GetTextExtent(dc.GetSafeHdc(), m_strTipText, (HFONT)m_font.GetSafeHandle());
		m_rcTipText.left = 0;
		m_rcTipText.top = 0;
		m_rcTipText.right = m_rcTipText.left + szTipText.cx;
		m_rcTipText.bottom = m_rcTipText.top + szTipText.cy;
		rcClient.right = m_rcTipText.right;
	}
	if (m_strBtnText != L"")
	{
		szBtnText = common::ui::GetTextExtent(dc.GetSafeHdc(), m_strBtnText, (HFONT)m_font.GetSafeHandle());
		m_rcBtnText.left = rcClient.right + 5;
		m_rcBtnText.top = 0;
		m_rcBtnText.right = m_rcBtnText.left + szBtnText.cx;
		//szBtnText.cy += 2;//2用于画下划线
		m_rcBtnText.bottom = m_rcBtnText.top + szBtnText.cy;

		rcClient.right = m_rcBtnText.right;
	}
	if (m_bClose)
	{
		CString strTemp = common::utility::systemhelper::Get179AppPath().c_str();
		m_strImgPath.Format(L"%sresource\\other\\Login_Tip_iconClose.png",strTemp);

		Image img(m_strImgPath);
		szIcon.cx = img.GetWidth();
		szIcon.cy = img.GetHeight();
		
		m_rcIconClose.left = rcClient.right + 5;
		m_rcIconClose.right = m_rcIconClose.left + szIcon.cx;
		m_rcIconClose.top = 0;
		m_rcIconClose.bottom = m_rcIconClose.top + szIcon.cy;

		rcClient.right = m_rcIconClose.right;
	}
	
	rcClient.right += 10;//左右端各留5个像素
	int temp = 0;
	int max = (temp = (szTipText.cy > szBtnText.cy) ? szTipText.cy : szBtnText.cy) > szIcon.cy ? temp : szIcon.cy;
	max += 4;//max高度用于显示内容
	rcClient.bottom = rcClient.top + max + TIP_ARROW_HEIGHT*2;//上下端各留10个像素用于显示箭头
	//统一一下位置
	temp = m_rcTipText.Width();
	m_rcTipText.left = rcClient.left + 5;
	m_rcTipText.right = m_rcTipText.left + temp;
	m_rcTipText.top = rcClient.top + TIP_ARROW_HEIGHT + 4;
    m_rcTipText.bottom = rcClient.top + TIP_ARROW_HEIGHT + max;

	temp = m_rcBtnText.Width();
	m_rcBtnText.left = m_rcTipText.right + 5;
	m_rcBtnText.right = m_rcBtnText.left + temp;
	m_rcBtnText.top = rcClient.top + TIP_ARROW_HEIGHT + 3;
	m_rcBtnText.bottom = rcClient.top + TIP_ARROW_HEIGHT + max - 1;

	temp = m_rcIconClose.Width();
	if (m_strBtnText != L"")
	{
		m_rcIconClose.left = m_rcBtnText.right + 5;
	}else
	{
		m_rcIconClose.left = m_rcBtnText.right;
	}
	m_rcIconClose.right = m_rcIconClose.left + temp;
	m_rcIconClose.top = rcClient.top + TIP_ARROW_HEIGHT + 3;
	m_rcIconClose.bottom = rcClient.top + TIP_ARROW_HEIGHT + max - 1;
   
	CRgn rgnText,rgnArrow;
	rgnText.CreateRoundRectRgn(rcClient.left, rcClient.top + TIP_ARROW_HEIGHT,rcClient.Width(),rcClient.top + TIP_ARROW_HEIGHT + max+4,UP_CORNER_RADIUS,DOWN_CORNER_RADIUS);//内容区，有圆角
	POINT	ptLeader[3] = {0} ;
	switch ( m_arrowPos )
	{
	//PS：三角箭头最好是等腰直角三角形，避免锯齿出现
	case ARROW_TOP_LEFT:
		{
			ptLeader[0].x = nArrowLeft;
			ptLeader[0].y = rcClient.top + TIP_ARROW_HEIGHT;
			ptLeader[1].x = ptLeader[0].x + 14;
			ptLeader[1].y = ptLeader[0].y;
			ptLeader[2].x = ptLeader[0].x + 7;
			ptLeader[2].y = ptLeader[0].y - 7;
		}
		break;
	case ARROW_BOTTOM_MIDDLE:
		{
			ptLeader[0].x = nArrowLeft - 7 ;
			ptLeader[0].y = rcClient.top + TIP_ARROW_HEIGHT + max + 2;
			ptLeader[1].x = nArrowLeft + 7;
			ptLeader[1].y = ptLeader[0].y;
			ptLeader[2].x = nArrowLeft;
			ptLeader[2].y = ptLeader[0].y + 7;
		}
		break;
	case ARROW_BOTTOM_LEFT:
		{
			ptLeader[0].x = nArrowLeft;
			ptLeader[0].y = rcClient.top + TIP_ARROW_HEIGHT + max + 2;
			ptLeader[1].x = ptLeader[0].x + 14;
			ptLeader[1].y = ptLeader[0].y;
			ptLeader[2].x = ptLeader[0].x + 7;
			ptLeader[2].y = ptLeader[0].y + 7;
		}
		break;
		default: break;
	}
	rgnArrow.CreatePolygonRgn(ptLeader, 3, ALTERNATE);

	HRGN showRgn;
	showRgn = ::CreateRectRgn(0,0,rcClient.Width(),rcClient.Height());
	CombineRgn(showRgn, rgnText.operator HRGN(), rgnArrow.operator HRGN(), RGN_OR);
	if(m_showRgn != NULL)
	{
		DeleteObject(m_showRgn);
	}
	m_showRgn = showRgn;

	rgnText.CombineRgn(&rgnText,&rgnArrow,RGN_OR);
	SetWindowRgn((HRGN)rgnText.GetSafeHandle(),TRUE);

	nWidth = rcClient.Width() ;
	nHeight = rcClient.Height();

	rgnText.DeleteObject();
	rgnArrow.DeleteObject();

	Invalidate();
	m_bHided = FALSE ;
}

void C179TipCtrl::OnPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CPaintDC dc( this ); 
	CRect	rectTipWindow;
	GetClientRect(&rectTipWindow);

	CDC MemDC ;
	CBitmap MemBitmap ;
	MemDC.CreateCompatibleDC( &dc ) ;
	MemBitmap.CreateCompatibleBitmap( &dc ,rectTipWindow.Width() , rectTipWindow.Height() ) ;
	MemDC.SetBkMode(TRANSPARENT);
	CBitmap* pOldBitmap = MemDC.SelectObject( &MemBitmap ) ;
	Graphics graph(MemDC);
	//背景色
	//CBrush bkgBrush;
	//bkgBrush.CreateSolidBrush(m_bkColor);
    //MemDC.FillRgn(CRgn::FromHandle(m_showRgn),&bkgBrush);
	Color begincolor, endcolor;
	begincolor.SetFromCOLORREF(m_bkBeginColor);
	endcolor.SetFromCOLORREF(m_bkEndColor);
	RectF fRect(rectTipWindow.left,rectTipWindow.top,rectTipWindow.Width(),rectTipWindow.Height());
	LinearGradientBrush bkgBrush(fRect, begincolor, endcolor, LinearGradientModeVertical);//从上往下渐变色
	Region rgnTemp(m_showRgn);
	graph.FillRegion(&bkgBrush,&rgnTemp);//MemDC.FillRect()
	CBrush borderBrush(m_colorBorder);
	MemDC.FrameRgn(CRgn::FromHandle(m_showRgn),&borderBrush,1,1);
	CFont* pOldFont = (CFont*)MemDC.SelectObject(&m_font) ;
	if ( m_strTipText != L"" )
	{
		MemDC.SetTextColor( m_colorTipText ) ;
		MemDC.DrawText(m_strTipText, &m_rcTipText, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
	}
	if ( m_strBtnText != L"" )
	{
		MemDC.SetTextColor(m_colorBtnText);
		MemDC.DrawText(m_strBtnText, m_rcBtnText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX);

		//绘制下划线
		CPen linePen(PS_SOLID, 1, m_colorBtnText);
		CPen* pOldPen = MemDC.SelectObject(&linePen);
		MemDC.MoveTo(CPoint(m_rcBtnText.left, m_rcBtnText.bottom));
		MemDC.LineTo(CPoint(m_rcBtnText.right, m_rcBtnText.bottom));
		MemDC.SelectObject(pOldPen);
	}

	if(m_bClose)
	{
		if (m_strImgPath != L"")
		{
			Image image(m_strImgPath);
			graph.DrawImage( &image , m_rcIconClose.left, m_rcIconClose.top+(m_rcIconClose.Height() - image.GetHeight())/2,image.GetWidth() , image.GetHeight() );
		}
	}
	dc.BitBlt( 0 , 0 , rectTipWindow.Width() , rectTipWindow.Height() , &MemDC , 0 , 0 ,SRCCOPY ) ;

	//::DeleteObject(m_showRgn);
	//m_showRgn = NULL;
	MemDC.SelectObject(pOldFont);
	MemDC.SelectObject( pOldBitmap ) ;
	MemBitmap.DeleteObject() ;
	MemDC.DeleteDC() ;
}

BOOL C179TipCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE ;
}

void C179TipCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (PtInRect(&m_rcBtnText,point))
	{
		OnClickBtn();
	}

	if (m_bClose)
	{
		if (PtInRect(&m_rcIconClose,point))
		{
			OnClickClose();
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void C179TipCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsWindowVisible())
	{
		if (PtInRect(&m_rcBtnText,point))
		{
			::SetCursor(::LoadCursor(NULL,IDC_HAND));
		}
		else
		{
			::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		}
		if (m_bClose)
		{
			if (PtInRect(&m_rcIconClose,point))
			{
				::SetCursor(::LoadCursor(NULL,IDC_HAND));
			}
		}
		else
		{
			::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		}

	}

	CWnd::OnMouseMove(nFlags, point);
}

void C179TipCtrl::OnClickBtn()
{
	if ( m_pParentWnd != NULL )
	{
		m_pParentWnd->SendMessage( WM_TIP_CLICK_BTN , (WPARAM)this , 0 ) ;
	}
}

void C179TipCtrl::OnClickClose()
{
	m_bHided = TRUE ;
	ShowWindow( SW_HIDE ) ;
	if ( m_pParentWnd != NULL )
	{
		m_pParentWnd->SendMessage( WM_TIP_CLICK_CLOSE , (WPARAM)this , 0 ) ;
	}
	
}