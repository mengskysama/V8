//////////////////////////////////////////////////////////////////////////////////////////////
#pragma once



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	class CAppBar
//		give derived window the ability to dock on any side of the screen
//		gain behavior similar with Windows Task bar
//
//	Author:		Yao Zhifeng
//	Contact:	yaozhifeng@hotmail.com
//
//	Usage:
//		1.use this class as a base class
//		2.use CHAIN_MSG_MAP to chain message to this class
//		3.call InitAppBar in the OnCreate or OnInitDialog function
//		4.(optional) call DockAppBar to dock the window to any side of the screen
//		5.(optional) call SetAutoHide to change the autohide behavior
//		6.(optional) call SetKeepSize to enable keeping original window size when docking on the screen
//		7.(optional) if you want to do any specific operation when docking side changed, 
//		  override OnDockingChanged function in ATL way
//
//MFC  implemetion
//  rewrite by hu mingyu(longing);
//  corp 51.com 
//  mail:humy@mail.51.com
//  work on MFC environment,it make the dialogue derived 
//  from the class action as slider bar on either side of the screen;
//  date 10/3/2008
#define APPBAR_CALLBACK     WM_USER + 1010
#define WM_SLIDEOUT_END     WM_USER + 500
#define WM_SLIDEIN_END      WM_SLIDEOUT_END+1
#define TIMER_BASE 1200
#define SLIDEHIDETIME   200
#define INFLATESIZE   10
#define DETASTEP 10
#define MAGINLEFT 3

namespace common { namespace ui
{

class CAppBar
{
public: //constants
	enum DockingSide
	{
		APPBAR_DOCKING_NONE		= 0x00,
		APPBAR_DOCKING_LEFT		= 0x01,
		APPBAR_DOCKING_RIGHT	= 0x02,
		APPBAR_DOCKING_TOP		= 0x04,
		APPBAR_DOCKING_BOTTOM	= 0x08,
		APPBAR_DOCKING_ALL		= 0x0f

	};
	UINT	m_uCurrentDockingSide;	
private:
	CRect	m_rcOriginal;	//window rect while not docking on any side
	CRect	m_rcStartDraging;
	CRect	m_rcLastDraging;
	CPoint	m_ptStartDraging;
	bool	m_bDraging;
	HWND    m_hwnd;

	UINT	m_uFlags;	//docking flags
	bool	m_bRegistered; //if registered
	bool	m_bAutoHide;
	bool	m_bHiding;
	bool	m_bKeepSize;
	bool    m_DragMutx;
	bool    m_firsttrigger;

	bool    m_justclick;

	UINT_PTR	m_uHidingTimer;
	UINT_PTR	m_uShowingTimer;
	UINT_PTR    m_uRelaseDraging;
	UINT_PTR    m_uForbidDbclick;
    bool    m_bIslidering;
public:
	CAppBar()
	{
		m_uFlags = APPBAR_DOCKING_NONE;
		m_uCurrentDockingSide = APPBAR_DOCKING_NONE;
		m_bRegistered = false;
		m_bDraging = false;
		m_bAutoHide = false;
		m_bHiding = false;
		m_uHidingTimer = 1+TIMER_BASE;
		m_uShowingTimer = 2+TIMER_BASE;
		m_uRelaseDraging = 3+TIMER_BASE;
		m_uForbidDbclick = 4+TIMER_BASE;
		m_bKeepSize = false;
		m_DragMutx = true;
		m_firsttrigger = true;
		m_hwnd = NULL;
		m_justclick = false;
        m_bIslidering = false;
	
	}
	~CAppBar()
	{
	}
	void SetHwnd(HWND _hwnd)
	{
       ASSERT(_hwnd);
		   m_hwnd = _hwnd;
	}
		 
public:
   ////////////////////////////////////////////////////////////////////////////
	// is the bar onhide
	//this must be true then fade out
	BOOL GetDragContent()
	{
		BOOL bContent = FALSE;
		SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,TRUE,&bContent,0) ;
		return bContent;
	}
	bool IsBarOnHide()
	{
		int iSX=GetSystemMetrics(SM_CXSCREEN);
		int iSY=GetSystemMetrics(SM_CYSCREEN);
		CRect rcOld;
		GetWindowRect(m_hwnd,&rcOld);
		int cx = ( rcOld.left+rcOld.right )/2;
		int cy = ( rcOld.top+rcOld.bottom )/2;

		if (cx<0||cx>iSX)
		return true;
		if (cy<0||cy>iSY)
			return true;

		return FALSE;
	}
////////////////////////////////////////////////////////////////////////////
    BOOL IsAppWndHanging()
    {
       return	!m_bIslidering;//m_uCurrentDockingSide == APPBAR_DOCKING_NONE;
    }

	//////////////////////////////////////////////////////////////////////////
	// Initialize
	// must be called right after window be created
	void InitAppBar(UINT uFlags=APPBAR_DOCKING_ALL, bool bAutoHide=false, bool bKeepSize=false)
	{
		m_uFlags = uFlags;
		m_bAutoHide = bAutoHide;
		m_bKeepSize = bKeepSize;
		m_uCurrentDockingSide = APPBAR_DOCKING_NONE;

		GetWindowRect(m_hwnd,&m_rcOriginal);

	}

	///////////////////////////////////////////////////////////
	//change the autohide behavior
	void SetAutoHide(bool bAutoHide)
	{
		if (m_bAutoHide != bAutoHide)
		{
			UINT uCurrentSide = m_uCurrentDockingSide;
			RegisterAppBar(false);
			m_bAutoHide = bAutoHide;
			DockAppBar(uCurrentSide);
		}
	}

	///////////////////////////////////////////////////////////
	//change the keepsize behavior
	void SetKeepSize(bool bKeepSize)
	{
		if (m_bKeepSize != bKeepSize)
		{
			UINT uCurrentSide = m_uCurrentDockingSide;
			RegisterAppBar(false);
			m_bKeepSize = bKeepSize;
			DockAppBar(uCurrentSide);
		}
	}

	//////////////////////////////////////////////////////////
	// dock window to one side of the screen
	void DockAppBar(UINT uFlag)
	{
		if (uFlag == APPBAR_DOCKING_NONE)
		{
			//Undock the window
			RegisterAppBar(false);
			m_uCurrentDockingSide = uFlag;
		//	CWnd::FromHandle(m_hwnd)->MoveWindow(&m_rcOriginal);
		}
		else
		{
			//docking flag must match the initial flags
			ASSERT((uFlag & m_uFlags)!=0);

			if (!m_bRegistered)
				RegisterAppBar(true);

			//record the original position before becoming an appbar
			if (m_uCurrentDockingSide == APPBAR_DOCKING_NONE)
			GetWindowRect(m_hwnd,&m_rcOriginal);

			//autohide processing
			APPBARDATA	abd;
			abd.cbSize	= sizeof(abd);
			abd.hWnd	= m_hwnd;
			abd.uEdge	= GetEdge(uFlag);
			abd.lParam	= m_bAutoHide ? TRUE:FALSE;

			::SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd);

			//set area and move window
			RECT	rc = SetAppBarPos(uFlag);
			m_uCurrentDockingSide = uFlag;

			//for autohide&keepsize setting
			//reajust the window position
			if (m_bAutoHide && m_bKeepSize)
				rc = QueryDockingRect(uFlag);
//			CWnd::FromHandle(m_hwnd)->MoveWindow(&rc);

			OnDockingChanged(uFlag);

			//start the hiding timer to hide the autohide appbar
			if (m_bAutoHide)
			{
				m_uRelaseDraging = SetTimer(m_hwnd,(UINT_PTR)m_uRelaseDraging, SLIDEHIDETIME,NULL);
			}
		}
	}

	//////////////Message Handlers/////////////////////////////////////////////////////
   void OnWindowPosChanged(WINDOWPOS* lpwndpos)
	{


		WINDOWPOS *pWindowPos = (WINDOWPOS*)lpwndpos;
		if (m_uCurrentDockingSide==APPBAR_DOCKING_NONE)
		{
			//update original rect
			GetWindowRect(m_hwnd,&m_rcOriginal);
		}
		else
		{
			if (!m_bHiding)
			{
				//if window being sized when docked, update appbar area
				if (m_uCurrentDockingSide==APPBAR_DOCKING_LEFT || m_uCurrentDockingSide==APPBAR_DOCKING_RIGHT)
				{
					m_rcOriginal.right = m_rcOriginal.left + pWindowPos->cx;
					SetAppBarPos(m_uCurrentDockingSide);
				}
				else if (m_uCurrentDockingSide==APPBAR_DOCKING_TOP || m_uCurrentDockingSide==APPBAR_DOCKING_BOTTOM)
				{
					m_rcOriginal.bottom = m_rcOriginal.top + pWindowPos->cy;
					SetAppBarPos(m_uCurrentDockingSide);
				}
			}
		}

		if (m_bRegistered && m_uCurrentDockingSide!=APPBAR_DOCKING_NONE)
		{
			APPBARDATA	abd;
			memset(&abd, 0, sizeof(APPBARDATA));
			abd.cbSize = sizeof(abd);
			abd.hWnd =m_hwnd; 

			::SHAppBarMessage(ABM_WINDOWPOSCHANGED, &abd);
		}

		return ;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// revoke appbar when destroy window
	void OnDestroy()
	{

		RegisterAppBar(false);
		return ;
	}

	//record start position for draging operation
	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

         m_ptStartDraging = point;
		//first rect
		if (m_uCurrentDockingSide==APPBAR_DOCKING_NONE)
		{
//			pT->GetWindowRect(&m_rcStartDraging);
			GetWindowRect(m_hwnd,&m_rcStartDraging);
		}
		else
		{
			m_rcStartDraging.SetRect(m_ptStartDraging, m_ptStartDraging+m_rcOriginal.Size());
			m_rcStartDraging.OffsetRect(-m_rcOriginal.Width()/2, -m_rcOriginal.Height()/2);
		}

	   return;
	}

	//determin if draging, draw a draging rectangle to show target rect
	void OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

		CPoint	ptCurrent;//=point;
		CPoint ptOffset = ptCurrent - m_ptStartDraging;

		enum
		{
			DRAG_DISTANCE = 2
		};

		//only if the mouse has been dragged for a small distance,
		//we can determine this is a draging operation
		
			if (abs(ptOffset.x)>DRAG_DISTANCE || abs(ptOffset.y)>DRAG_DISTANCE)
			{
				m_bDraging = true;
				//draw the first frame recorded in LButtonDown handler
				DrawFrame(m_rcStartDraging);
				m_rcLastDraging = m_rcStartDraging;
			}
		
		{
			//cancel last frame rect
			DrawFrame(m_rcLastDraging);

			//calculate new frame rect
			m_rcLastDraging = m_rcStartDraging;
			m_rcLastDraging.OffsetRect(ptOffset);

			UINT uFlag = GetProposedDockingFlag(ptCurrent);
			if (uFlag != APPBAR_DOCKING_NONE)
			{
				//m_rcLastDraging = GetProposedDockingRect(uFlag);
				m_rcLastDraging = QueryDockingRect(uFlag);
			}

			//draw new frame rect
			DrawFrame(m_rcLastDraging);
		}

	
		return ;
	}

	void SliderWindow(UINT uFlag)
	{
	   m_uFlags = uFlag;
	   m_DragMutx = true;
       m_bIslidering = false;
	   KillTimer(m_hwnd, m_uHidingTimer);
	   KillTimer(m_hwnd, m_uShowingTimer);
	   KillTimer(m_hwnd, m_uRelaseDraging);


	  
		   m_DragMutx = false;	    	
		   m_rcLastDraging = QueryDockingRect(m_uFlags);
		   DockAppBar(m_uFlags);
		   if(m_hwnd)
		   CWnd::FromHandle(m_hwnd)->MoveWindow(m_rcLastDraging);
	
	   

	}
	//if draging, move window to new position or dock to correct side
	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

//		if (m_bDraging)
		{
			m_bDraging = false;

			DrawFrame(m_rcLastDraging);

			CPoint	ptCurrent;
			::GetCursorPos(&ptCurrent);
			UINT uFlag = GetProposedDockingFlag(ptCurrent);
			DockAppBar(uFlag);
			if (uFlag == APPBAR_DOCKING_NONE)
			{
//				pT->MoveWindow(m_rcLastDraging);
				CWnd::FromHandle(m_hwnd)->MoveWindow(m_rcLastDraging);
			}
			
		}

	
		return ;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// take over default hit test behavior
	// prevent unnormal moving and sizing operation
	LRESULT OnNcHitTest(LRESULT hit)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

		//take care of the auto unhide stuff
		if (m_bAutoHide && m_bHiding && m_firsttrigger)
		{	
			m_uShowingTimer = SetTimer(m_hwnd,(UINT_PTR)m_uShowingTimer, SLIDEHIDETIME,NULL);//pT->SetTimer(m_uShowingTimer, 50);
		//	OnTimer(m_uShowingTimer);
			
		    m_firsttrigger = false;
		}
        
		switch (hit)
		{
		case HTCAPTION:
			hit = HTCLIENT;
			break;
		case HTLEFT:
			if (m_uCurrentDockingSide==APPBAR_DOCKING_LEFT ||
				m_uCurrentDockingSide==APPBAR_DOCKING_TOP ||
				m_uCurrentDockingSide==APPBAR_DOCKING_BOTTOM)
				hit = HTCLIENT;
			break;
		case HTRIGHT:
			if (m_uCurrentDockingSide==APPBAR_DOCKING_RIGHT ||
				m_uCurrentDockingSide==APPBAR_DOCKING_TOP ||
				m_uCurrentDockingSide==APPBAR_DOCKING_BOTTOM)
				hit = HTCLIENT;
			break;
		case HTTOP:
			if (m_uCurrentDockingSide==APPBAR_DOCKING_LEFT ||
				m_uCurrentDockingSide==APPBAR_DOCKING_TOP ||
				m_uCurrentDockingSide==APPBAR_DOCKING_RIGHT)
				hit = HTCLIENT;
			break;
		case HTBOTTOM:
			if (m_uCurrentDockingSide==APPBAR_DOCKING_LEFT ||
				m_uCurrentDockingSide==APPBAR_DOCKING_BOTTOM ||
				m_uCurrentDockingSide==APPBAR_DOCKING_RIGHT)
				hit = HTCLIENT;
			break;
		case HTTOPLEFT:
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
			if (m_uCurrentDockingSide!=APPBAR_DOCKING_NONE)
				hit = HTCLIENT;
			break;
		}

	
		return hit;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// take over default hit test behavior
	// prevent unnormal moving and sizing operation
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{


		// TODO: 在此处添加消息处理程序代码
		APPBARDATA abd;
		
		// Always send the activate message to the system
		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd =m_hwnd;// pT->m_hWnd;
		abd.lParam = 0;                
		SHAppBarMessage(ABM_ACTIVATE, &abd);

		// Now determine if we're getting or losing activation
		switch (nState)
		{		
		    case WA_INACTIVE:
              if(m_justclick)
			  {//SetActiveWindow(m_hwnd);
				 SetForegroundWindow(m_hwnd);
			  }
			
		      break;
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_justclick =true;
				// If we're gaining activation, make sure we're visible
				if (m_uCurrentDockingSide!=APPBAR_DOCKING_NONE && m_bHiding && m_bAutoHide)
				{					
					if(IsBarOnHide())
					DoShowAppBar();
					else
					{
						m_bHiding = false;
						m_firsttrigger = true;
					}
					//start the hiding timer after showing
					//pT->SetTimer(m_uHidingTimer, 500);
					KillTimer(m_hwnd,m_uForbidDbclick);
			        m_uHidingTimer =  SetTimer(m_hwnd,(UINT_PTR)m_uHidingTimer, SLIDEHIDETIME,NULL);
					
				}
				m_uForbidDbclick = SetTimer(m_hwnd,(UINT_PTR)m_uForbidDbclick,500,NULL);
				//CWnd* p;
				//p = CWnd::FromHandle(m_hwnd)->FindWindow(L"Shell_TrayWnd",NULL);
				//if (p)
				//{
				//	p->EnableWindow(FALSE);
				//}
				break;


	
		
	}
		return ;
	}
	void OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

		if (nIDEvent==m_uHidingTimer)
		{
			if (m_uCurrentDockingSide==APPBAR_DOCKING_NONE || !m_bAutoHide)
			{
			//	pT->KillTimer(wParam);
				KillTimer(m_hwnd,nIDEvent);
			}
			else
			{
				// Get the mouse position, the window position, and active 
				// window
				CPoint	pt;
				CRect	rcWnd;
				::GetCursorPos(&pt);
//			
				GetWindowRect(m_hwnd,&rcWnd);
				rcWnd.InflateRect(INFLATESIZE,INFLATESIZE);
				HWND  hwndActive = ::GetForegroundWindow();

				// If the mouse is outside of our window, or we are not active,
				// or at least one window is active, or we are not the parent
				// of an active window, the hide the appbar window.
				if ((!rcWnd.PtInRect(pt)) && (hwndActive != m_hwnd) && 
					(hwndActive != NULL) &&CWnd::FromHandle(hwndActive)->GetParent()!= CWnd::FromHandle(m_hwnd)&&m_justclick == false)
				{
					KillTimer(m_hwnd,nIDEvent);
					DoHideAppBar();
				}
			}
		}
		else if (nIDEvent==m_uShowingTimer)
		{
				KillTimer(m_hwnd,nIDEvent);
			if (m_bAutoHide && m_bHiding && m_uCurrentDockingSide!=APPBAR_DOCKING_NONE)
			{
				CPoint	pt;
				CRect	rcWnd;
				::GetCursorPos(&pt);
				GetWindowRect(m_hwnd,&rcWnd);
				rcWnd.InflateRect(INFLATESIZE,2*INFLATESIZE);

				//check if the cursor is still in the window rect
				if (rcWnd.PtInRect(pt))
				{
					if(IsBarOnHide())
					DoShowAppBar();
					else
					{
						m_bHiding = false;
						m_firsttrigger = true;
					}
					//start the hiding timer after showing
				   
					m_uRelaseDraging = SetTimer(m_hwnd,(UINT_PTR)m_uRelaseDraging, SLIDEHIDETIME,NULL);// pT->SetTimer(m_uHidingTimer, 500);
				}
				else
				   m_firsttrigger = true;
			}
		}
		else if (nIDEvent == m_uRelaseDraging)
		{

			if (m_uCurrentDockingSide==APPBAR_DOCKING_NONE || !m_bAutoHide)
			{
				//	pT->KillTimer(wParam);
				KillTimer(m_hwnd,nIDEvent);
			}
			else
			{
				// Get the mouse position, the window position, and active 
				// window
				CPoint	pt;
				CRect	rcWnd;
				::GetCursorPos(&pt);
				//			
				GetWindowRect(m_hwnd,&rcWnd);
				rcWnd.InflateRect(INFLATESIZE,INFLATESIZE);

				if ((!rcWnd.PtInRect(pt))/*pT->m_hWnd*/)
				{
					KillTimer(m_hwnd,nIDEvent);
					DoHideAppBar();
				//	HWND hwndTaskbar = GetTopWindow(NULL);
				//	SetForegroundWindow(hwndTaskbar);
				}

		}


		}
		else if (m_uForbidDbclick == nIDEvent)
		{
			KillTimer(m_hwnd,m_uForbidDbclick);
			m_justclick = false;
			//CWnd* p;
			//p =CWnd::FromHandle(m_hwnd)->FindWindow(L"Shell_TrayWnd",NULL);
			//if (p)
			//{
			//	p->EnableWindow(TRUE);
			//}

		}
		return ;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//handle system callback
	LRESULT OnAppBarNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{

		APPBARDATA abd; 
		UINT uState; 

		abd.cbSize = sizeof(abd); 
		abd.hWnd = m_hwnd;//pT->m_hWnd; 

		switch (wParam)
		{
		case ABN_FULLSCREENAPP:
			//to be implemented
			//...
			break;
		case ABN_POSCHANGED:
			//adjust window position and docking area
			if (m_uCurrentDockingSide!=APPBAR_DOCKING_NONE)
			{
				CRect	rc = SetAppBarPos(m_uCurrentDockingSide);
//				pT->MoveWindow(&rc);
				CWnd::FromHandle(m_hwnd)->MoveWindow(&rc);
			}
			break;
		case ABN_STATECHANGE:
            // Check to see if the taskbar's always-on-top state has 
            // changed and, if it has, change the appbar's state 
            // accordingly. 
            uState = SHAppBarMessage(ABM_GETSTATE, &abd); 

/*            pT->SetWindowPos( 
                (ABS_ALWAYSONTOP & uState) ? HWND_TOPMOST : HWND_NOTOPMOST, 
                0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); */
			SetWindowPos(m_hwnd,(ABS_ALWAYSONTOP & uState) ? HWND_TOPMOST : HWND_NOTOPMOST, 
				0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			break;
		case ABN_WINDOWARRANGE:
			//exclude the appbar from window arrangement
			if (lParam)//begin
			{
				ShowWindow(m_hwnd,SW_HIDE);
			}
			else //completed
			{
				ShowWindow(m_hwnd,SW_SHOW);
			}
			break;
		}
		return 0;
	}
	////////End Message Handlers//////////////////////////////////////////////////////


	////////////For override in derived class///////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////
	// Be Called after docking side changed
	void OnDockingChanged(UINT uDockingSide)
	{
	}

	/////////////////////////////////////////////////////////////////
	// get width and height while docking window
	SIZE GetDockSize()
	{
		SIZE	szDock;
		szDock.cx = m_rcOriginal.Width();
		szDock.cy = m_rcOriginal.Height();

		return szDock;
	}

	////////////End for override in derived class///////////////////////////


protected:
	////////////////////////////////////////////////////////
	//add or remove appbar from system
	void RegisterAppBar(bool bRegister)
	{

		APPBARDATA	abd;
		memset(&abd, 0, sizeof(APPBARDATA));
		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd	= m_hwnd;//pT->m_hWnd;

		if (!m_bRegistered && bRegister)
		{
			//register appbar to system
			abd.uCallbackMessage	= APPBAR_CALLBACK;

			if (!m_bAutoHide)
			{
				::SHAppBarMessage(ABM_NEW, &abd);
			}
			m_bRegistered = true;

		}
		else if (m_bRegistered && !bRegister)
		{
			//remove appbar from system
			::SHAppBarMessage(ABM_REMOVE, &abd);
			m_bRegistered = false;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// set or adjust the appbar
	// return the target rect for appbar
	RECT SetAppBarPos(UINT uFlag)
	{

		APPBARDATA	abd;
		memset(&abd, 0, sizeof(APPBARDATA));
		abd.cbSize	= sizeof(APPBARDATA);
		abd.hWnd	= m_hwnd;// pT->m_hWnd;

		abd.uEdge	= GetEdge(uFlag);
		abd.rc		= GetProposedDockingRect(uFlag);

		::SHAppBarMessage(ABM_QUERYPOS, &abd);

		//the proposed rect might be changed, we need to ajust the width or height
		AdjustDockingRect(abd.rc, uFlag);

		//set docking pos
		::SHAppBarMessage(ABM_SETPOS, &abd);

		return abd.rc;
	}

	///////////////////////////////////////////////////////////////////////
	// calculat proposed docking rect
	RECT GetProposedDockingRect(UINT uFlag)
	{

		//get screen rect
		RECT	rc;
		::GetWindowRect(::GetDesktopWindow(), &rc);

		//adjust the rect according to docking flag
		SIZE	sz =GetDockSize();// pT->GetDockSize();
		switch (uFlag)
		{
		case APPBAR_DOCKING_LEFT:
			rc.right = rc.left + sz.cx;
			break;
		case APPBAR_DOCKING_RIGHT:
			rc.left = rc.right - sz.cx;
			break;
		case APPBAR_DOCKING_TOP:
			rc.bottom = rc.top + sz.cy;
			break;
		case APPBAR_DOCKING_BOTTOM:
			rc.top = rc.bottom - sz.cy;
			break;
		default:
	//		ATLASSERT("Invalid argument");
			break;
		}

		return rc;
	}

	/////////////////////////////////////////////////////////////////////
	// query docking rect
	// for visual effect while moving window with mouse
	RECT QueryDockingRect(UINT uFlag)
	{

		APPBARDATA	abd;
		memset(&abd, 0, sizeof(APPBARDATA));
		abd.cbSize	= sizeof(APPBARDATA);
		abd.hWnd	= m_hwnd;// pT->m_hWnd;

		abd.uEdge	= GetEdge(uFlag);
		abd.rc		= GetProposedDockingRect(uFlag);

		::SHAppBarMessage(ABM_QUERYPOS, &abd);

		//the proposed rect might be changed, we need to ajust the width or height
		AdjustDockingRect(abd.rc, uFlag);

		//ajust rect for autosize&keepsize settings
		if (m_bAutoHide && m_bKeepSize)
		{
			CPoint	pt;
			::GetCursorPos(&pt);
			//SIZE	sz = GetDockSize();//pT->GetDockSize();
			switch (uFlag)
			{
			case APPBAR_DOCKING_LEFT:
			case APPBAR_DOCKING_RIGHT:
/*				abd.rc.top = pt.y - sz.cy/2;
				abd.rc.bottom = abd.rc.top + sz.cy;*/
				break;
			case APPBAR_DOCKING_TOP:
			case APPBAR_DOCKING_BOTTOM:
/*				abd.rc.left = pt.x - sz.cx/2;
				abd.rc.right = abd.rc.left + sz.cx;*/
				break;
			}
			GetWindowRect(m_hwnd,&abd.rc);
			
		}

		return abd.rc;
	}

	/////////////////////////////////////////////////////////////
	// after calling ABM_QUERYPOS, the acquired rect need to be adjusted
	void AdjustDockingRect(RECT &rc, UINT uFlag)
	{
//		T* pT = static_cast<T*>(this);
		SIZE	sz = GetDockSize();//pT->GetDockSize();
		switch (uFlag)
		{
		case APPBAR_DOCKING_LEFT:
			rc.right = rc.left + sz.cx;
			break;
		case APPBAR_DOCKING_RIGHT:
			rc.left = rc.right - sz.cx;
			break;
		case APPBAR_DOCKING_TOP:
			rc.bottom = rc.top + sz.cy;
			break;
		case APPBAR_DOCKING_BOTTOM:
			rc.top = rc.bottom - sz.cy;
			break;
		default:
//			ATLTRACE("Invalid argument");
//			ATLASSERT(FALSE);
			break;
		}
	}

	////////////////////////////////////////////////////////////////
	// get proposed target docking side while moving window
	UINT GetProposedDockingFlag(POINT ptCursor)
	{
		UINT	uFlag = APPBAR_DOCKING_NONE;
		RECT	rcScreen;
		::GetWindowRect(::GetDesktopWindow(), &rcScreen);
		if (ptCursor.x - rcScreen.left<10)
			uFlag = APPBAR_DOCKING_LEFT;
		else if (ptCursor.y - rcScreen.top<10)
			uFlag = APPBAR_DOCKING_TOP;
		else if (rcScreen.right - ptCursor.x<10)
			uFlag = APPBAR_DOCKING_RIGHT;
		else if (rcScreen.bottom - ptCursor.y<10)
			uFlag = APPBAR_DOCKING_BOTTOM;

		//Check if the edge is allowed by initial flags
		if ((uFlag & m_uFlags) > 0)
			return uFlag;
		else
			return APPBAR_DOCKING_NONE;
	}

	//////////////////////////////////////////////////////////////
	// get uEdge value given docking flag
	UINT GetEdge(UINT uFlag)
	{
		UINT uEdge ;
		switch (uFlag)
		{
		case APPBAR_DOCKING_LEFT:
			uEdge = ABE_LEFT;
			break;
		case APPBAR_DOCKING_RIGHT:
			uEdge = ABE_RIGHT;
			break;
		case APPBAR_DOCKING_TOP:
			uEdge = ABE_TOP;
			break;
		case APPBAR_DOCKING_BOTTOM:
			uEdge = ABE_BOTTOM;
			break;
		default:
			uEdge = ABE_BOTTOM;
//			ATLTRACE("Invalid argument");
//			ATLASSERT(FALSE);
			break;
		}

		return uEdge;
	}

	//////////////////////////////////////////////////////////////////////
	// draw temp frame for target rect
	void DrawFrame(CRect rc)
	{
		enum
		{
			FRAME_BORDER_SIZE = 3
		};

		CWindowDC	dc(CWnd::FromHandle(::GetDesktopWindow()));
		CBrush *brush =CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)); //GetStockBrush;//CDCHandle::GetHalftoneBrush();
		if(brush->m_hObject != NULL)
		{
			CBrush * hBrushOld = dc.SelectObject(brush);
			dc.PatBlt(rc.left, rc.top, rc.Width(), FRAME_BORDER_SIZE, PATINVERT);
			dc.PatBlt(rc.left, rc.bottom-FRAME_BORDER_SIZE, rc.Width(), FRAME_BORDER_SIZE, PATINVERT);
			dc.PatBlt(rc.left, rc.top+FRAME_BORDER_SIZE, FRAME_BORDER_SIZE, rc.Height()-FRAME_BORDER_SIZE*2, PATINVERT);
			dc.PatBlt(rc.right-FRAME_BORDER_SIZE, rc.top+FRAME_BORDER_SIZE, FRAME_BORDER_SIZE, rc.Height()-FRAME_BORDER_SIZE*2, PATINVERT);

			dc.SelectObject(hBrushOld);
		}
	}
    void DoHideAppBarEx()
	{
		if(	m_uCurrentDockingSide != APPBAR_DOCKING_NONE)
		{
			int iSX=GetSystemMetrics(SM_CXSCREEN);
			int iSY=GetSystemMetrics(SM_CYSCREEN);
			CRect rcOld;
			GetWindowRect(m_hwnd,&rcOld);
			int nHeight = rcOld.Height();
			int nWidth = rcOld.Width();
			switch (m_uCurrentDockingSide)
			{
			case APPBAR_DOCKING_LEFT:
				rcOld.right = MAGINLEFT;
				rcOld.left = rcOld.right - nWidth;
				break;
			case APPBAR_DOCKING_TOP:
				rcOld.bottom = MAGINLEFT;
				rcOld.top = rcOld.bottom - nHeight;
				break;
			case APPBAR_DOCKING_BOTTOM:
				rcOld.top = iSY - MAGINLEFT;
				rcOld.bottom = rcOld.top + MAGINLEFT;
				break;
			case APPBAR_DOCKING_RIGHT:
				rcOld.left = iSX - MAGINLEFT;
				rcOld.right = rcOld.left+MAGINLEFT;
				break;

			}
			SetWindowPos(m_hwnd,NULL, rcOld.left, rcOld.top,
					nWidth, nHeight,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_NOSIZE);
		}
			::PostMessage(m_hwnd, WM_SLIDEIN_END, 0, 0);
	}
	void DoHideAppBar()
	{
		if (!m_bHiding)
		{
			m_bHiding = true;
            m_bIslidering = true;
			SlideWindow(false);
			DoHideAppBarEx();
            m_bIslidering = false;
		}
	}
	
	void IsHanging()//检测窗口是否悬挂
	{
		int iSX=GetSystemMetrics(SM_CXFULLSCREEN);
		int iSY=GetSystemMetrics(SM_CYFULLSCREEN);
		CRect rcWin;
		GetWindowRect(m_hwnd,&rcWin);
	    switch(m_uFlags)
		{
		case APPBAR_DOCKING_BOTTOM:
			if (rcWin.bottom<iSY-DETASTEP)
			{
				m_uCurrentDockingSide = APPBAR_DOCKING_NONE;
				KillTimer(m_hwnd,m_uRelaseDraging);
			}	
			break;
		case APPBAR_DOCKING_TOP:
			if (rcWin.top>DETASTEP)
			{
				m_uCurrentDockingSide = APPBAR_DOCKING_NONE;
				KillTimer(m_hwnd,m_uRelaseDraging);
			}			
			break;
		case APPBAR_DOCKING_LEFT:
			if (rcWin.left>DETASTEP)
			{
				m_uCurrentDockingSide = APPBAR_DOCKING_NONE;
				KillTimer(m_hwnd,m_uRelaseDraging);
			}			
			break;
		case APPBAR_DOCKING_RIGHT:
			if (rcWin.right<iSX-DETASTEP)
			{
				m_uCurrentDockingSide = APPBAR_DOCKING_NONE;
				KillTimer(m_hwnd,m_uRelaseDraging);
			}			
			break;
		default: 
             m_uCurrentDockingSide = APPBAR_DOCKING_NONE;
		

		}
		if (m_uCurrentDockingSide==APPBAR_DOCKING_NONE)
		m_uFlags = m_uCurrentDockingSide;
		return;

	}
    void DoShowAppBarEx()
	{
		if(	m_uCurrentDockingSide != APPBAR_DOCKING_NONE)
		{
			int iSX=GetSystemMetrics(SM_CXSCREEN);
			int iSY=GetSystemMetrics(SM_CYSCREEN);
			CRect rcOld;
			GetWindowRect(m_hwnd,&rcOld);
			int nHeight = rcOld.Height();
			int nWidth = rcOld.Width();
			switch (m_uCurrentDockingSide)
			{
			case APPBAR_DOCKING_LEFT:
				rcOld.left = 0;
				rcOld.right = rcOld.left + nWidth;
				break;
			case APPBAR_DOCKING_TOP:
				rcOld.top = 0;
				rcOld.bottom = rcOld.top + nHeight;
				break;
			case APPBAR_DOCKING_BOTTOM:
				rcOld.bottom = iSY;
				rcOld.top = rcOld.bottom - nHeight;
				break;
			case APPBAR_DOCKING_RIGHT:
				rcOld.right = iSX ;
				rcOld.left = rcOld.right - nWidth;
				break;

			}
			SetWindowPos(m_hwnd,NULL, rcOld.left, rcOld.top,
				nWidth, nHeight,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_NOSIZE);

			::PostMessage(m_hwnd, WM_SLIDEOUT_END, 0, 0);
		}
	}
	void DoShowAppBar()
	{
		if (m_bHiding)
		{
            m_bIslidering = true;
			SlideWindow(true);
            IsHanging();
			DoShowAppBarEx();
            m_bIslidering = false;
			m_bHiding = false;
		    m_firsttrigger = true;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	//	Slides the AppBar off or from the edge of the screen when the AppBar
	//	has the AutoHide state set.
	void SlideWindow(bool bShow)
	{
		enum SlideTick
		{
			HIDE_TICKS = 200,//260,
			SHOW_TICKS = 120,//200,
			HIDING_BAR_SIZE = 2*3,
		};

		CRect	rcWnd;
		GetWindowRect(m_hwnd,&rcWnd);
		int	dx = 0;
		int dy = 0;
		switch (GetEdge(m_uCurrentDockingSide))
		{
			case ABE_TOP:
				dy = HIDING_BAR_SIZE - rcWnd.Height();
				if (bShow)
					dy = - dy;
				break;
			case ABE_BOTTOM:
				dy = rcWnd.Height()-HIDING_BAR_SIZE;
				if (bShow)
					dy = -dy;
				break;
			case ABE_LEFT:
				dx = HIDING_BAR_SIZE-rcWnd.Width();
				if (bShow)
					dx = -dx;
				break;
			case ABE_RIGHT:
				dx = rcWnd.Width()-HIDING_BAR_SIZE;
				if (bShow)
					dx = -dx;
				break;
			default:
				return;
		}

		//Do move
		HANDLE hThreadMe = ::GetCurrentThread();
		int priority = ::GetThreadPriority(hThreadMe);
		::SetThreadPriority(hThreadMe, THREAD_PRIORITY_HIGHEST);

		int x, y, dt, t, t0;
		dt = bShow? SHOW_TICKS : HIDE_TICKS;

		t0 = GetTickCount();
		while ((t = GetTickCount()) < t0 + dt)
		{
			x = rcWnd.left + dx * (t - t0) / dt;
			y = rcWnd.top + dy * (t - t0) / dt;

			SetWindowPos(m_hwnd, NULL, x, y, 0, 0,
									SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			if (bShow)
				UpdateWindow(m_hwnd);
			else
				::UpdateWindow(GetDesktopWindow());
		}

		SetThreadPriority(hThreadMe, priority);

		//Ajust position to the right place
		SetWindowPos(m_hwnd,NULL, rcWnd.left+dx, rcWnd.top+dy,
			0, 0,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_NOSIZE);

	}


};

} } //namespace common::ui

