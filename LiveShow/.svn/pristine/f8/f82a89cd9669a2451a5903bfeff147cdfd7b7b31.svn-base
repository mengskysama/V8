#include "stdafx.h"
#include "ui/GifButton.h"

namespace common 
{
	namespace ui
	{
		#define  TIMER_DRAW_GIFT_BUTTON    1

		GifButton::GifButton(void)
		{
			m_strGifIcon = L"";
			m_strTipText = L"";
			m_bHandCursor = FALSE;
			m_drawTime = 100;
			m_pGifIcon = NULL;
			m_pParentWnd = NULL;
			m_rcWnd.SetRectEmpty();
		}

		GifButton::~GifButton(void)
		{
			if (m_pGifIcon != NULL)
			{
				delete m_pGifIcon;
				m_pGifIcon = NULL;
			}
		}

		BEGIN_MESSAGE_MAP(GifButton, CWnd)
			ON_WM_TIMER()
			ON_WM_MOUSEMOVE()
			ON_WM_LBUTTONUP()
			ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
			ON_WM_SHOWWINDOW()
		END_MESSAGE_MAP()

		BOOL GifButton::CreateEx(CWnd* pwndParent, CString& strIconPath)
		{
			if (!strIconPath.IsEmpty())
			{
				m_strGifIcon = strIconPath;
				m_pGifIcon = new common::ui::ImageEx(m_strGifIcon.GetBuffer());
			}
			
			CRect rcWnd(0,0,0,0);
			if (m_pGifIcon != NULL)
			{
				rcWnd.right = m_pGifIcon->GetWidth();
				rcWnd.bottom = m_pGifIcon->GetHeight();
			}
			
			if ( CWnd::CreateEx(WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),_T("GifButton"),WS_POPUP,rcWnd,pwndParent,0))
			{
				m_rcWnd = rcWnd;

				m_OwnToolTipCtrl.Create(this);
				m_OwnToolTipCtrl.Activate(TRUE);
				m_OwnToolTipCtrl.AddTool(this, L"");

				m_pParentWnd = pwndParent;

				//窗口透明
				LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
				dwstyle |= WS_EX_LAYERED ;

				SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;

				m_blend.BlendOp = 0 ;
				m_blend.BlendFlags = 0 ;
				m_blend.AlphaFormat = 1 ;
				m_blend.SourceConstantAlpha = 255 ;
				
				SetTimer(TIMER_DRAW_GIFT_BUTTON, m_drawTime, NULL);

				return TRUE;
			}
			return FALSE;
		}

		void GifButton::SetBtnHandCursor(BOOL bHand)
		{
			m_bHandCursor = bHand;
		}

		void GifButton::SetDrawTime(LONG drawTime)
		{
			m_drawTime = drawTime;
		}

		void GifButton::SetGifBtnTipText(CString & strTip)
		{
			m_strTipText = strTip;
		}

		void GifButton::SetGifBtnGifIcon(CString& strPath)
		{
			if (strPath.IsEmpty())
			{
				return;
			}

			KillTimer(TIMER_DRAW_GIFT_BUTTON);
			if (m_pGifIcon != NULL)
			{
				delete m_pGifIcon;
				m_pGifIcon = NULL;
			}
			m_strGifIcon = strPath;
			m_pGifIcon = new common::ui::ImageEx(m_strGifIcon.GetBuffer());
			if (m_pGifIcon == NULL)
			{
				return;
			}

			MoveWindow(0, 0, m_pGifIcon->GetWidth(), m_pGifIcon->GetHeight());
			m_rcWnd.SetRectEmpty();
			m_rcWnd.right = m_pGifIcon->GetWidth();
			m_rcWnd.bottom = m_pGifIcon->GetHeight();

			SetTimer(TIMER_DRAW_GIFT_BUTTON, m_drawTime, NULL);
		}

		BOOL GifButton::PreTranslateMessage(MSG* pMsg)
		{
			// TODO: 在此添加专用代码和/或调用基类
			if (m_OwnToolTipCtrl.m_hWnd)
			{
				m_OwnToolTipCtrl.RelayEvent(pMsg);
			}
			return CWnd::PreTranslateMessage(pMsg);
		}

		void GifButton::OnTimer(UINT_PTR nIDEvent)
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			if (nIDEvent == TIMER_DRAW_GIFT_BUTTON)
			{
				if (m_pGifIcon != NULL && m_pGifIcon->IsAnimatedGIF())
				{
					m_pGifIcon->DrawFrameGIF();
				}
				DrawGif();
			}

			CWnd::OnTimer(nIDEvent);
		}

		void GifButton::DrawGif()
		{
			if (m_pGifIcon == NULL || m_rcWnd.IsRectEmpty())
			{
				return;
			}
			CRect rect(&m_rcWnd);
			ClientToScreen(&rect);
			//GetWindowRect( &rect );//万恶啊，全屏啊！！！！

			CDC *pCDC = GetDC();
			HDC hdcTemp = pCDC->GetSafeHdc();
			HDC hdcMemory = CreateCompatibleDC( hdcTemp );
			HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() );
			HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap );
			Graphics graph( hdcMemory );
			ImageAttributes imgAtt;
			imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果
			Rect rcTemp(0, 0, rect.Width(), rect.Height());
			if (m_pGifIcon != NULL)
			{
				graph.DrawImage(m_pGifIcon->GetCurImage(), rcTemp, 0, 0, rect.Width(), rect.Height(),UnitPixel,&imgAtt);
			}

			BOOL bResult = BitBlt( hdcTemp , rect.left , rect.top , rect.Width() , rect.Height() , hdcMemory , 0 , 0 , SRCCOPY );

			POINT ptWinPos = { rect.left,rect.top };
			POINT ptSrc= {0,0};
			SIZE sizeWindow = {rect.Width(), rect.Height() };
			::UpdateLayeredWindow(m_hWnd, hdcTemp, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &m_blend, ULW_ALPHA);
			graph.ReleaseHDC(hdcMemory);
			SelectObject(hdcMemory, hOld);
			DeleteObject(hBitMap);
			DeleteDC(hdcMemory);
			ReleaseDC(pCDC);
		}

		void GifButton::ShowGifBtn(CRect& rc)
		{
			CRect rcTemp(&m_rcWnd);
			//GetWindowRect(&rcTemp);
			MoveWindow(rc.left, rc.top, rcTemp.Width(), rcTemp.Height());
			ShowWindow(SW_SHOWNOACTIVATE);
		}

		void GifButton::OnMouseMove(UINT nFlags, CPoint point)
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_HOVER;
			tme.dwHoverTime = 1;
			_TrackMouseEvent(&tme);	

			if (m_bHandCursor)
			{
				HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
				::SetCursor(hCur);
			}else
			{
				HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);
				::SetCursor(hCur);
			}

			CWnd::OnMouseMove(nFlags, point);
		}

		LRESULT GifButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
		{
			m_OwnToolTipCtrl.UpdateTipText(m_strTipText.GetBuffer(), this);
			return S_OK;
		}

		void GifButton::OnLButtonUp(UINT nFlags, CPoint point)
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			if (m_pParentWnd != NULL && m_pParentWnd->GetSafeHwnd())
			{
				::PostMessage(m_pParentWnd->GetSafeHwnd(), WM_ONCLICK_GIFBUTTON, (WPARAM)this, 0);
			}
			
			CWnd::OnLButtonUp(nFlags, point);
		}
		
		void GifButton::OnShowWindow(BOOL bShow, UINT nStatus)
		{
			if (nStatus != 0)//说明不是ShowWindow触发，是父窗口显示或隐藏时触发
			{
				return;
			}

			CWnd::OnShowWindow(bShow, nStatus);
			// TODO: 在此处添加消息处理程序代码
		}
	
		///////////////////////////////////////////////////////////////////////////////
		
	}
}