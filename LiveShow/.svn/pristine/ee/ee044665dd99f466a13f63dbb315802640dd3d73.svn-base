#include "stdafx.h"
#include "ui/PngButton.h"

namespace common 
{
	namespace ui
	{
		PngButton::PngButton(void)
		{
			m_pParentWnd = NULL;
			m_bHandCursor = FALSE;
			m_bNormal = TRUE;
			m_bHover = FALSE;
			m_bDown = FALSE;
			m_bOver = FALSE;
			m_bDisabled = FALSE;
			m_bChecked = FALSE;

			m_imgNoamrl = NULL;
			m_imgHover = NULL;
			m_imgDown = NULL;
			m_imgDisabled = NULL;

			m_strContent = L"";
			m_strConFont = L"宋体";
			m_colCon = RGB(0, 0, 0);
			m_fontSize = 12;

			m_rcWnd.SetRectEmpty();
		}

		PngButton::~PngButton(void)
		{
			if (m_imgNoamrl != NULL)
			{
				delete m_imgNoamrl;
				m_imgNoamrl = NULL;
			}
			if (m_imgHover != NULL)
			{
				delete m_imgHover;
				m_imgHover = NULL;
			}
			if (m_imgDown != NULL)
			{
				delete m_imgDown;
				m_imgDown = NULL;
			}
			if (m_imgDisabled != NULL)
			{
				delete m_imgDisabled;
				m_imgDisabled = NULL;
			}
		}

		BEGIN_MESSAGE_MAP(PngButton, CWnd)
			ON_WM_MOUSEMOVE()
			ON_WM_LBUTTONUP()
			ON_WM_LBUTTONDOWN()
			ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
			ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
			ON_WM_CLOSE()
		END_MESSAGE_MAP()

		BOOL PngButton::CreateEx(CWnd* pBottomWnd, CWnd* pParentWnd)
		{
			if ( CWnd::CreateEx(WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),_T("PngButton"),WS_POPUP,m_rcWnd,pBottomWnd,0))
			{
				m_pParentWnd = pParentWnd;

				//窗口透明
				LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
				dwstyle |= WS_EX_LAYERED ;

				SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;

				m_blend.BlendOp = 0 ;
				m_blend.BlendFlags = 0 ;
				m_blend.AlphaFormat = 1 ;
				m_blend.SourceConstantAlpha = 255 ;
				

				return TRUE;
			}
			return FALSE;
		}

		void PngButton::SetBtnHandCursor(BOOL bHand)
		{
			m_bHandCursor = bHand;
		}
		
		void PngButton::SetBtnDisabled(BOOL bDis)
		{
			m_bDisabled = bDis;
			if (!m_bDisabled)
			{
				m_bNormal = TRUE;
				m_bHover = FALSE;
				m_bDown = FALSE;
			}
			DrawBtn();
		}

		void PngButton::SetBtnChecked(BOOL bChecked)
		{
			m_bChecked = bChecked;
			if (!m_bDisabled)
			{
				if (m_bChecked)
				{
					m_bNormal = FALSE;
					m_bHover = FALSE;
					m_bDown = FALSE;
				}else
				{
					m_bNormal = TRUE;
					m_bHover = FALSE;
					m_bDown = FALSE;
				}
			}
			DrawBtn();
		}

		void PngButton::SetNormalImg(CString & strNormalImg)
		{
			if (m_imgNoamrl != NULL)
			{
				delete m_imgNoamrl;
				m_imgNoamrl = NULL;
			}
			m_imgNoamrl = new common::ui::ImageEx(strNormalImg.GetBuffer());
			MoveWindow(0, 0, m_imgNoamrl->GetWidth(), m_imgNoamrl->GetHeight());
			m_rcWnd.SetRectEmpty();
			m_rcWnd.right = m_imgNoamrl->GetWidth();
			m_rcWnd.bottom = m_imgNoamrl->GetHeight();
		}

		void PngButton::SetHoverImg(CString & strHoverImg)
		{
			if (m_imgHover != NULL)
			{
				delete m_imgHover;
				m_imgHover = NULL;
			}
			m_imgHover = new common::ui::ImageEx(strHoverImg.GetBuffer());
			MoveWindow(0, 0, m_imgHover->GetWidth(), m_imgHover->GetHeight());
			m_rcWnd.SetRectEmpty();
			m_rcWnd.right = m_imgHover->GetWidth();
			m_rcWnd.bottom = m_imgHover->GetHeight();
		}

		void PngButton::SetDownImg(CString & strDownImg)
		{
			if (m_imgDown != NULL)
			{
				delete m_imgDown;
				m_imgDown = NULL;
			}
			m_imgDown = new common::ui::ImageEx(strDownImg.GetBuffer());
			MoveWindow(0, 0, m_imgDown->GetWidth(), m_imgDown->GetHeight());
			m_rcWnd.SetRectEmpty();
			m_rcWnd.right = m_imgDown->GetWidth();
			m_rcWnd.bottom = m_imgDown->GetHeight();
		}

		void PngButton::SetDisabledImg(CString & strDisabledImg)
		{
			if (m_imgDisabled != NULL)
			{
				delete m_imgDisabled;
				m_imgDisabled = NULL;
			}
			m_imgDisabled = new common::ui::ImageEx(strDisabledImg.GetBuffer());
			MoveWindow(0, 0, m_imgDisabled->GetWidth(), m_imgDisabled->GetHeight());
			m_rcWnd.SetRectEmpty();
			m_rcWnd.right = m_imgDisabled->GetWidth();
			m_rcWnd.bottom = m_imgDisabled->GetHeight();
		}

		void PngButton::SetContent(CString strContent, CString strFont, COLORREF color, int fontSize)
		{
			if (strContent != L"")
			{
				m_strContent = strContent;
			}
			if (strFont != L"")
			{
				m_strConFont = strFont;
			}
			m_colCon = color;
			if (fontSize > 0)
			{
				m_fontSize = fontSize;
			}

			DrawBtn();
		}

		BOOL PngButton::PreTranslateMessage(MSG* pMsg)
		{
			// TODO: 在此添加专用代码和/或调用基类
			if ( pMsg->message == WM_KEYDOWN)
			{
				switch(pMsg->wParam)
				{
				case VK_ESCAPE:
				case VK_RETURN:

					return TRUE ;
				default: break;
				}	
			}
			return CWnd::PreTranslateMessage(pMsg);
		}

		void PngButton::DrawBtn()
		{
			common::ui::ImageEx* drawImage = GetDrawImage();
			if (drawImage == NULL || m_rcWnd.IsRectEmpty())
			{
				return;
			}

			CRect rect(&m_rcWnd);
			ClientToScreen(&rect);
			//GetWindowRect(&rect);
			CDC *pCDC = GetDC();
			HDC hdcTemp = pCDC->GetSafeHdc();
			HDC hdcMemory = CreateCompatibleDC( hdcTemp );
			HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() );
			HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap );
			Graphics graph( hdcMemory );
			graph.DrawImage(drawImage, 0, 0, rect.Width(), rect.Height());

			if (m_strContent != L"")//有文字内容
			{
				Color conColor;
				conColor.SetFromCOLORREF(m_colCon);
				SolidBrush  brush(conColor);
				FontFamily  fontFamily(m_strConFont.GetBuffer(0));
				Font        font(&fontFamily, m_fontSize, FontStyleBold, UnitPixel);
				PointF      pointF( 0.0f , 0.0f );
				RectF boundRect;
				graph.MeasureString( m_strContent , m_strContent.GetLength() ,&font ,pointF , &boundRect );
				pointF.X = m_rcWnd.left + (m_rcWnd.Width() - boundRect.Width)/2;
				pointF.Y = m_rcWnd.top + (m_rcWnd.Height() - boundRect.Height)/2;
				graph.DrawString( m_strContent , m_strContent.GetLength(), &font, pointF, &brush );
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

		common::ui::ImageEx* PngButton::GetDrawImage()
		{
			if (m_bNormal && m_imgNoamrl != NULL)
			{
				return m_imgNoamrl;
			}else if (m_bHover && m_imgHover != NULL)
			{
				return m_imgHover;
			}else if (m_bDown && m_imgDown != NULL)
			{
				return m_imgDown;
			}else if (m_bDisabled && m_imgDisabled != NULL)
			{
				return m_imgDisabled;
			}else if (m_bChecked && m_imgDown != NULL)//要选中状态，则要设置DownImg;
			{
				return m_imgDown;
			}

			return NULL;
		}

		void PngButton::OnMouseMove(UINT nFlags, CPoint point)
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			if(!m_bOver)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = m_hWnd;
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.dwHoverTime = 1;

				_TrackMouseEvent(&tme);	
			}
			

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

		LRESULT PngButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
		{
			//m_OwnToolTipCtrl.UpdateTipText(m_strTipText.GetBuffer(), this);
			m_bOver = true;
			if (!m_bDisabled)
			{
				if (m_bChecked)
				{
					m_bNormal = FALSE;
					m_bDown = FALSE;
					m_bHover = FALSE;
				}else
				{
					m_bNormal = FALSE;
					m_bDown = FALSE;
					m_bHover = TRUE;
				}
			}else
			{	
				m_bNormal = FALSE;
				m_bDown = FALSE;
				m_bHover = FALSE;
				m_bChecked = FALSE;
			}
			DrawBtn();
			if (m_pParentWnd != NULL && m_pParentWnd->GetSafeHwnd() && !m_bDisabled)
			{
				::PostMessage(m_pParentWnd->GetSafeHwnd(), WM_ONMOUSERHOVER_PNGBUTTON, (WPARAM)this, 0);
			}
			return S_OK;
		}
		
		LRESULT PngButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
		{
			m_bOver = false;
			if (!m_bDisabled)
			{
				if (m_bChecked)
				{
					m_bNormal = FALSE;
					m_bDown = FALSE;
					m_bHover = FALSE;
				}else
				{
					m_bNormal = TRUE;
					m_bDown = FALSE;
					m_bHover = FALSE;
				}
				
			}else
			{
				m_bNormal = FALSE;
				m_bDown = FALSE;
				m_bHover = FALSE;
			}
			DrawBtn();
			if (m_pParentWnd != NULL && m_pParentWnd->GetSafeHwnd() && !m_bDisabled)
			{
				::PostMessage(m_pParentWnd->GetSafeHwnd(), WM_ONMOUSERLEAVE_PNGBUTTON, (WPARAM)this, 0);
			}
			return S_OK;
		}

		void PngButton::OnLButtonUp(UINT nFlags, CPoint point)
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			if (!m_bDisabled)
			{
				if (!m_bChecked)
				{
					m_bNormal = FALSE;
					m_bDown = FALSE;
					m_bHover = FALSE;
				}else
				{
					m_bNormal = TRUE;
					m_bDown = FALSE;
					m_bHover = FALSE;
				}
			}else
			{
				m_bNormal = FALSE;
				m_bDown = FALSE;
				m_bHover = FALSE;
			}
			DrawBtn();

			if (m_pParentWnd != NULL && m_pParentWnd->GetSafeHwnd() && !m_bDisabled)
			{
				::PostMessage(m_pParentWnd->GetSafeHwnd(), WM_ONCLICK_PNGBUTTON, (WPARAM)this, m_bChecked);
			}
			
			CWnd::OnLButtonUp(nFlags, point);
		}

		void PngButton::OnLButtonDown(UINT nFlags, CPoint point)
		{
			if (!m_bDisabled)
			{
				m_bNormal = FALSE;
				m_bDown = TRUE;
				m_bHover = FALSE;
			}else
			{
				m_bNormal = FALSE;
				m_bDown = FALSE;
				m_bHover = FALSE;
			}
			DrawBtn();
			CWnd::OnLButtonDown(nFlags, point);
		}

		void PngButton::OnClose()  //屏蔽组合键关闭
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			return;
			CWnd::OnClose();
		}
		
		void PngButton::ShowBtnTask(CRect& rc)
		{
			CRect rcTemp(&m_rcWnd);
			//GetWindowRect(&rcTemp);
			MoveWindow(rc.left, rc.top, rcTemp.Width(), rcTemp.Height());
			DrawBtn();
			ShowWindow(SW_SHOWNOACTIVATE);
		}
	}
}