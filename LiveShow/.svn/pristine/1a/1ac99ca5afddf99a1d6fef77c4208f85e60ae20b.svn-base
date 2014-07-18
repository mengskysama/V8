#include "stdafx.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "ui/TickerStatic.h"

namespace common 
{
	namespace ui
	{
		IMPLEMENT_DYNAMIC(CTickerStatic, CStatic)

		#define TICKER_TIMER_IDEVENT   1

		CTickerStatic::CTickerStatic()
		:m_colorDefault(Gdiplus::Color(160,215,235))
		,m_colorSelect(Gdiplus::Color(255,255,255))
		,m_colorDown(Gdiplus::Color(1,55,96))
		{
			m_intervalTime = 0;
			m_intervallen = 10;
			m_curIndex = -1;	
			m_isPause = true;
			m_isClick = true;
			m_bDown = false;
			m_bNeedUpdate = false;

			m_fontDefault = new Gdiplus::Font(L"宋体", 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			m_fontSelect = new Gdiplus::Font(L"宋体", 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			m_cursour =  LoadCursor(NULL,IDC_HAND);
		}

		CTickerStatic::~CTickerStatic()
		{
			if(m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
			{
				m_dcBk.SelectObject(m_pbmpOldBk);
			}

			m_bmpBk.DeleteObject();

			delete m_fontDefault;
			delete m_fontSelect;
		}

		BEGIN_MESSAGE_MAP(CTickerStatic, CStatic)
			ON_WM_PAINT()
			ON_WM_TIMER()
			ON_WM_SIZE()
			ON_WM_MOUSEMOVE()
			ON_WM_SETCURSOR()			
			ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
			ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
			ON_WM_LBUTTONDOWN()
			ON_WM_LBUTTONUP()
		END_MESSAGE_MAP()

		//运行跑马灯		
		void CTickerStatic::RunTicker(vector<tickerInfo> &tickerVec,uint32 intervalTime)
		{
			m_tickerVec.clear();
			for(int i=0;i<tickerVec.size();i++)
			{
				tickerInfoRect ti;
				ti.m_tickerInfo = tickerVec[i];
				m_tickerVec.push_back(ti);
			}
			m_intervalTime = intervalTime;
			m_bOver = false;

			SetTimer( TICKER_TIMER_IDEVENT, m_intervalTime , NULL );
			CRect rectTicker ;
			GetClientRect(&rectTicker);
			m_nScrollPos = rectTicker.Width() ;
		}

		void CTickerStatic::SetTickerFont(Gdiplus::Font &fontDefault,Gdiplus::Font &fontSelect)
		{	
			Gdiplus::FontFamily *family;
			if(m_fontDefault != NULL)
			{
				delete m_fontDefault;
			}
			fontDefault.GetFamily(family);
			m_fontDefault = new Gdiplus::Font(family,fontDefault.GetSize(),fontDefault.GetStyle(),fontDefault.GetUnit());

			if(m_fontSelect != NULL)
			{
				delete m_fontSelect;
			}
			fontSelect.GetFamily(family);
			m_fontSelect = new Gdiplus::Font(family,fontSelect.GetSize(),fontSelect.GetStyle(),fontSelect.GetUnit());
		}

		void CTickerStatic::SetTickerColor(COLORREF colorDefault,COLORREF colorSelect)
		{
			Gdiplus::Color textColor1,textColor2;	
			textColor1.SetFromCOLORREF(colorDefault);
			textColor2.SetFromCOLORREF(colorSelect);	
			m_colorDefault.SetColor(textColor1);
			m_colorSelect.SetColor(textColor2);
		}

		//设置鼠标悬停时是否暂停滚动   请在RunTicker之前调用
		void CTickerStatic::SetPause(bool isPause) 
		{
			m_isPause = isPause;
		}

		void CTickerStatic::PreSubclassWindow()
		{
			// TODO: 在此添加专用代码和/或调用基类
			ModifyStyle( 0 , SS_NOTIFY ); //就设置之后才会有ON_WM_MOUSEMOVE 消息						
			CStatic::PreSubclassWindow();
		}		

		//设置是否有鼠标点击后打开对应url事件  请在RunTicker之前调用
		//若可点击，则有鼠标悬停时文字变色和手型光标
		void CTickerStatic::SetClickTitle(bool isClick)
		{
			m_isClick = isClick;
		}
		void CTickerStatic::OnTimer(UINT_PTR nIDEvent)
		{
			if ( nIDEvent == TICKER_TIMER_IDEVENT )
			{		
				// move text up one pixel
				if(m_tickerVec.size()>0)
				{
					m_nScrollPos = m_nScrollPos - 3 ;
				}
				InvalidateRect(NULL);
			}
			
			CStatic::OnTimer(nIDEvent);
		}

		//获取鼠标当前point所在的下标
		int CTickerStatic::GetCursorPointIndex()
		{
			int nIndex = -1;
			CPoint point ;
			GetCursorPos( &point ) ;  //鼠标当前point

 			for(int i=0;i<m_tickerVec.size(); i++)
			{
				tickerInfoRect &tInfo = m_tickerVec[i];
				if ( tInfo.m_Rect.PtInRect(point) )
				{
					nIndex = i;
					break;
				}
			}

			return nIndex;
		}

		void CTickerStatic::OnSize(UINT nType, int cx, int cy)
		{
			m_bNeedUpdate = true;

			CRect m_rectTicker ;
			GetClientRect(&m_rectTicker);
			ClientToScreen(&m_rectTicker);
			m_nScrollPos = m_rectTicker.Width();   //首次时，文本开始从控件最右侧显示
		}

		void CTickerStatic::OnPaint()
		{
			CPaintDC dc(this); // device context for painting
		
			CRect m_rectTicker ;
			GetClientRect(&m_rectTicker);
			ClientToScreen(&m_rectTicker);	

			CDC memDC;
			CBitmap memBitmap;
			memDC.CreateCompatibleDC(&dc);
			memBitmap.CreateCompatibleBitmap(&dc, m_rectTicker.Width(), m_rectTicker.Height());
			memDC.SelectObject(&memBitmap);
			memDC.SetBkMode(TRANSPARENT);			
			memDC.BitBlt(0, 0, m_rectTicker.Width(), m_rectTicker.Height(), &m_dcBk, 0, 0, SRCCOPY );			
			
			Gdiplus::Graphics MemGraph(memDC.GetSafeHdc());

			float fTextPos = m_nScrollPos;	
			Gdiplus::RectF boundRect;
			int ileft,itop,iwidth,iheight;				
			for(int i=0;i<m_tickerVec.size(); i++)
			{
				tickerInfoRect & infoTemp = m_tickerVec[i];		
				Gdiplus::PointF pointF(fTextPos,m_rectTicker.Height()/3.0 );
				if(i == m_curIndex)
				{
					if(m_bDown) //鼠标点击
					{
						MemGraph.MeasureString( infoTemp.m_tickerInfo.m_Message , infoTemp.m_tickerInfo.m_Message.GetLength() ,m_fontSelect ,pointF , &boundRect ) ;				
						MemGraph.DrawString( infoTemp.m_tickerInfo.m_Message ,infoTemp.m_tickerInfo.m_Message.GetLength(), m_fontSelect, pointF, &m_colorDown );
					}
					else //鼠标悬停
					{
						MemGraph.MeasureString( infoTemp.m_tickerInfo.m_Message , infoTemp.m_tickerInfo.m_Message.GetLength() ,m_fontSelect ,pointF , &boundRect ) ;				
						MemGraph.DrawString( infoTemp.m_tickerInfo.m_Message ,infoTemp.m_tickerInfo.m_Message.GetLength(), m_fontSelect, pointF, &m_colorSelect );
					}					
				}
				else 
				{
					MemGraph.MeasureString( infoTemp.m_tickerInfo.m_Message , infoTemp.m_tickerInfo.m_Message.GetLength() ,m_fontDefault ,pointF , &boundRect ) ;				
					MemGraph.DrawString( infoTemp.m_tickerInfo.m_Message ,infoTemp.m_tickerInfo.m_Message.GetLength(), m_fontDefault, pointF, &m_colorDefault );
				}
				
				ileft = boundRect.X+m_rectTicker.left;
				itop = boundRect.Y+m_rectTicker.top;
				iwidth = ileft + boundRect.Width - 3; //减3是为了调整一下宽度，boundRect.Width与显示的不是太准，不知道是否也将\n宽度也计算进去了的原因
				iheight = itop + boundRect.Height;
				infoTemp.m_Rect.SetRect(ileft,itop,iwidth,iheight);	
			
				fTextPos += boundRect.Width;
				fTextPos += m_intervallen;
			}	

			if(fTextPos <= 0)
			{
				m_nScrollPos = m_rectTicker.Width(); //所有文本显示完毕后，文本再次从控件最右侧开始显示
			}
			dc.BitBlt(0, 0, m_rectTicker.Width(), m_rectTicker.Height(), &memDC, 0, 0, SRCCOPY );		
			memDC.DeleteDC();
			memBitmap.DeleteObject();
		}

		void CTickerStatic::OnMouseMove(UINT nFlags, CPoint point)
		{
			
			if (!m_bOver)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = m_hWnd;
				tme.dwFlags = TME_LEAVE | TME_HOVER;
				tme.dwHoverTime = 1;
				m_bOver = _TrackMouseEvent(&tme);				
			}

			if(m_isClick) //鼠标可点击标题时，可设置鼠标光标
			{
				int nIndex = GetCursorPointIndex();
				if(nIndex != m_curIndex)
				{
					m_curIndex = nIndex;

					if(m_curIndex != -1)
						SetCursourHead(true);  //设置鼠标光标为手型
					else
						SetCursourHead(false);  //设置鼠标光标为标准箭头

					InvalidateRect(NULL);
				}
			}			

			CStatic::OnMouseMove(nFlags, point);
		}

		HRESULT CTickerStatic::OnMouseLeave( WPARAM wParam , LPARAM lParam )
		{				
			if(m_isPause)
			{
				SetTimer( TICKER_TIMER_IDEVENT , m_intervalTime , NULL ) ;
			}
			
			m_bOver = false;
			m_bDown = false;

			if(m_isClick) //鼠标可点击标题时，可设置鼠标光标
			{
				SetCursourHead(false);  //设置鼠标光标为标准箭头	
			}				
			
			m_curIndex = -1;

			return S_OK;
		}

		LRESULT CTickerStatic::OnMouseHover(WPARAM wParam, LPARAM lParam)
		{
			m_bOver = true;
			if(m_isPause) //暂停滚动
			{
				KillTimer(TICKER_TIMER_IDEVENT);
			}
			return 0;
		}

		//鼠标点击
		void CTickerStatic::OnLButtonUp(UINT nFlags, CPoint point)
		{
			if(m_isClick)
			{
				if(m_curIndex != -1 && m_curIndex<m_tickerVec.size())
				{
					tickerInfoRect & infoTemp = m_tickerVec[m_curIndex];		
					if(!infoTemp.m_tickerInfo.m_Url.IsEmpty())
					{
						common::utility::systemhelper::OpenUrl(wstring(infoTemp.m_tickerInfo.m_Url));  //使用浏览器打开对应url
					}
					
				}

				m_bDown = false;
				InvalidateRect(NULL);		
			}
				
		}

		void CTickerStatic::OnLButtonDown(UINT nFlags, CPoint point)
		{		
			if(m_isClick)
			{
				m_bDown = true;
				InvalidateRect(NULL);
			}
			
		}

		BOOL CTickerStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
		{
			return TRUE;
		}

		//设置鼠标光标类型 true为手型，false为标准箭头
		//m_isHead为true时才有用
		void CTickerStatic::SetCursourHead(bool head)
		{
			
			if(head)
				m_cursour = LoadCursor(NULL,IDC_HAND);
			else
				m_cursour = LoadCursor(NULL,IDC_ARROW);
			::SetCursor(m_cursour);		
			
		}		

		void CTickerStatic::OnPostParentPaint(HDC hParentHDC)
		{
			if(!m_bNeedUpdate)
				return;

			m_bNeedUpdate = false;

			if(m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
			{
				m_dcBk.SelectObject(m_pbmpOldBk);
			}

			if (m_dcBk.m_hDC != NULL)
			{
				m_dcBk.DeleteDC();
				m_bmpBk.DeleteObject();
			}

			CRect rect;
			GetClientRect(rect);

			CRect rect1;
			GetWindowRect(rect1);
			GetParent()->ScreenToClient(rect1);

			m_dcBk.CreateCompatibleDC(CDC::FromHandle(hParentHDC));
			m_bmpBk.CreateCompatibleBitmap(CDC::FromHandle(hParentHDC), rect.Width(), rect.Height());
			m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
			m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), CDC::FromHandle(hParentHDC), rect1.left, rect1.top, SRCCOPY);
		}
	}
}