#include "stdafx.h"
#include "ui/FlashFrameWnd.h"

namespace common 
{
	namespace ui
	{
		FlashFrameWnd::FlashFrameWnd(void)
		{
			m_imgFrame = NULL;
			m_rcWnd.SetRectEmpty();
			m_frameBorderWidth = 3;
			m_flashTimes = 7;
			m_bFlashing = false;
		}

		FlashFrameWnd::~FlashFrameWnd(void)
		{
			if (m_imgFrame != NULL)
			{
				delete m_imgFrame;
				m_imgFrame = NULL;
			}
		}

		BEGIN_MESSAGE_MAP(FlashFrameWnd, CWnd)
			ON_WM_CLOSE()
			ON_WM_TIMER()
		END_MESSAGE_MAP()

		BOOL FlashFrameWnd::CreateEx(CWnd* pBottomWnd, CRect& rcWnd)
		{
			if ( CWnd::CreateEx(WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),_T("FlashFrameWnd"),WS_POPUP,rcWnd,pBottomWnd,0))
			{
				m_rcWnd = rcWnd;

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

		BOOL FlashFrameWnd::PreTranslateMessage(MSG* pMsg)
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

		void FlashFrameWnd::SetFlashFrameImg(CString & strFrameImg)
		{
			if (strFrameImg.IsEmpty())
			{
				return;
			}
			if (m_imgFrame != NULL)
			{
				delete m_imgFrame;
				m_imgFrame = NULL;
			}
			m_imgFrame = new common::ui::ImageEx(strFrameImg.GetBuffer());
		}

		void FlashFrameWnd::SetFrameBorderWidth(int nBorderWidth)
		{
			if (nBorderWidth <= 0)
			{
				return;
			}
			m_frameBorderWidth = nBorderWidth;
		}

		void FlashFrameWnd::DrawFlashFrame()
		{
			if ( m_rcWnd.IsRectEmpty() || m_imgFrame == NULL || m_flashTimes < 0)
			{
				return;
			}
			if (m_rcWnd.Width() < m_frameBorderWidth || m_rcWnd.Height() < m_frameBorderWidth)
			{
				return;
			}

			CDC *pCDC = GetDC();
			HDC hdcTemp = pCDC->GetSafeHdc();
			HDC hdcMemory = CreateCompatibleDC( hdcTemp );
			HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , m_rcWnd.Width(), m_rcWnd.Height() );
			HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap );
			Graphics graph( hdcMemory );
			if (m_flashTimes % 2 == 0 || m_flashTimes == 0)
			{
				SolidBrush brush(RGB(0,0,0));
				Rect rcTemp(0, 0, m_rcWnd.Width(), m_rcWnd.Height());
				graph.FillRectangle(&brush, rcTemp);
			}else
			{
				ImageAttributes imgAtt;
				imgAtt.SetWrapMode(WrapModeTileFlipXY);//GDI+在位伸图片时会自动加上渐变效果，加此取消
				Rect rcTop(0, 0, m_rcWnd.Width(), m_frameBorderWidth);
				Rect rcRight(m_rcWnd.Width() - m_frameBorderWidth, 0, m_frameBorderWidth, m_rcWnd.Height());
				Rect rcLeft(0, 0, m_frameBorderWidth, m_rcWnd.Height());
				Rect rcBottom(0, m_rcWnd.Height() - m_frameBorderWidth, m_rcWnd.Width(), m_frameBorderWidth);

				graph.DrawImage(m_imgFrame, rcTop, 0, 0, m_rcWnd.Width(), m_frameBorderWidth, UnitPixel,&imgAtt);//上边
				graph.DrawImage(m_imgFrame, rcRight, m_rcWnd.Width() - m_frameBorderWidth, 0, m_frameBorderWidth, m_rcWnd.Height(), UnitPixel,&imgAtt);//右边
				graph.DrawImage(m_imgFrame, rcLeft, 0, 0, m_frameBorderWidth, m_rcWnd.Height(), UnitPixel,&imgAtt);//左边
				graph.DrawImage(m_imgFrame, rcBottom, 0, m_rcWnd.Height() - m_frameBorderWidth, m_rcWnd.Width(), m_frameBorderWidth, UnitPixel,&imgAtt);//下边
			}
			
			BOOL bResult = BitBlt( hdcTemp , m_rcWnd.left , m_rcWnd.top , m_rcWnd.Width() , m_rcWnd.Height() , hdcMemory , 0 , 0 , SRCCOPY );

			POINT ptWinPos = { m_rcWnd.left,m_rcWnd.top };
			POINT ptSrc= {0,0};
			SIZE sizeWindow = {m_rcWnd.Width(), m_rcWnd.Height() };
			::UpdateLayeredWindow(m_hWnd, hdcTemp, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &m_blend, ULW_ALPHA);
			graph.ReleaseHDC(hdcMemory);
			SelectObject(hdcMemory, hOld);
			DeleteObject(hBitMap);
			DeleteDC(hdcMemory);
			ReleaseDC(pCDC);
		}

		void FlashFrameWnd::OnClose()  //屏蔽组合键关闭
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			return;
			CWnd::OnClose();
		}
		
		void FlashFrameWnd::ShowFlashFrame(CRect& rc)
		{
			if (rc.IsRectEmpty())
			{
				return;
			}
			MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
			m_rcWnd = rc;
			ShowWindow(SW_SHOWNOACTIVATE);
		}

		void FlashFrameWnd::FlashTheFrame()
		{
			if (m_bFlashing)
			{
				return;
			}
			KillTimer(TIMER_DRAW_FLASH_FRAME);
			m_flashTimes = 7;
			DrawFlashFrame();
			m_bFlashing = true;
			SetTimer(TIMER_DRAW_FLASH_FRAME, 300, NULL);
		}

		void FlashFrameWnd::OnTimer(UINT_PTR nIDEvent)
		{
			// TODO: 在此添加消息处理程序代码和/或调用默认值
			if (nIDEvent == TIMER_DRAW_FLASH_FRAME)
			{
				--m_flashTimes;
				if (m_flashTimes < 0)
				{
					KillTimer(TIMER_DRAW_FLASH_FRAME);
					m_bFlashing = false;
				}else
				{
					DrawFlashFrame();	
				}		
			}

			CWnd::OnTimer(nIDEvent);
		}
	}
}