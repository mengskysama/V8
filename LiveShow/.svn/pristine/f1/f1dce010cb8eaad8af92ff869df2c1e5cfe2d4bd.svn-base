// CombineGiftDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "CombineGiftDlg.h"
#include "..\..\CommonLibrary/include\utility\FileHelper.h"

#define ROOM_DRAW_COMBINEGIFT_TIMER_ID		30050
#define ROOM_DRAW_COMBINEGIFT_FREEZTIME_ID	30051

// CCombineGiftDlg 对话框

IMPLEMENT_DYNAMIC(CCombineGiftDlg, CDialog)

CCombineGiftDlg::CCombineGiftDlg(combinegift_info * pInfo, IGiftItem * pGiftItem, std::wstring strSrcName, std::wstring strDstName, CWnd* pParent /*=NULL*/)
	: CDialog(CCombineGiftDlg::IDD, pParent),
	m_pInfo(pInfo),
	m_pGiftItem(pGiftItem),
	m_strSrcName(strSrcName),
	m_strDstName(strDstName)
{
	//m_hInst = NULL;
	m_nFrames = 0;
	m_nType = 0;
	m_bFirst = true;
	m_nCount = 0;
	m_nPointPerCircle = 0;
	m_bFinish = false;

	m_pBackImage = NULL;
	m_pCardImage = NULL;
	m_pUnitImage = NULL;
	m_pFrontImage = NULL;
}

CCombineGiftDlg::~CCombineGiftDlg()
{
	//if (m_hInst)
	//{
	//	FreeLibrary(m_hInst);
	//}
	if (m_pBackImage != NULL)
	{
		delete m_pBackImage;
		m_pBackImage = NULL;
	}
	if (m_pCardImage != NULL)
	{
		delete m_pCardImage;
		m_pCardImage = NULL;
	}
	if (m_pUnitImage != NULL)
	{
		delete m_pUnitImage;
		m_pUnitImage = NULL;
	}
	if (m_pFrontImage != NULL)
	{
		delete m_pFrontImage;
		m_pFrontImage = NULL;
	}
}

void CCombineGiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCombineGiftDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCombineGiftDlg 消息处理程序

int CCombineGiftDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	LONG dwstyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	dwstyle |= (/*WS_CHILD | */WS_CLIPCHILDREN | WS_POPUP);
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwstyle);

	dwstyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	dwstyle |= (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
	SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle);
	//HINSTANCE m_hInst = LoadLibrary(_T("User32.DLL") );

	m_blend.BlendOp = 0 ;
	m_blend.BlendFlags = 0 ;
	m_blend.AlphaFormat = 1 ;
	m_blend.SourceConstantAlpha = 255 ;
	//UpdateLayeredWindow = (FUpdateLayeredWindow)GetProcAddress( m_hInst ,("UpdateLayeredWindow") ) ;

	return 0;
}

BOOL CCombineGiftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LoadRes();

	//if (m_pInfo->nWidth != 0 && m_pInfo->nHeight != 0)
	//{
	//	//int cx = m_pBackImage->GetWidth();
	//	//int cy = m_pBackImage->GetHeight();
	//	int cx = m_pInfo->nWidth;
	//	int cy = m_pInfo->nHeight;

	//	MoveWindow(0,0,cx,cy);
	//	//CenterWindow(CWnd::GetDesktopWindow());
	//	//CenterWindow(GetParent());
	//}
	//else
	//{
	//	int cx = GetSystemMetrics(SM_CXSCREEN);
	//	int cy = GetSystemMetrics(SM_CYSCREEN);
	//	MoveWindow(0,0,cx,cy);
	//}

	SetTimer(ROOM_DRAW_COMBINEGIFT_TIMER_ID, m_pInfo->nInterval, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCombineGiftDlg::LoadRes()
{
	ASSERT(m_pInfo != NULL);
	if (m_pInfo->strBackImage != L"" && common::utility::filehelper::IsExist(m_pInfo->strBackImage.c_str()))
	{
		m_pBackImage = new Image(m_pInfo->strBackImage.c_str());
	}
	if (m_pInfo->strCardImage != L"" && common::utility::filehelper::IsExist(m_pInfo->strCardImage.c_str()))
	{
		m_pCardImage = new Image(m_pInfo->strCardImage.c_str());
	}
	if (m_pInfo->strFrontImage != L"" && common::utility::filehelper::IsExist(m_pInfo->strFrontImage.c_str()))
	{
		m_pFrontImage = new Image(m_pInfo->strFrontImage.c_str());
	}
	
	//ASSERT(m_pGiftItem != NULL);
	if (m_pGiftItem != NULL)
	{
		m_pUnitImage = new Image(m_pGiftItem->GetSmallImagePath().c_str());
	}
	m_nFrames = m_pInfo->nFrames;
	m_nPointPerCircle = m_pInfo->vecPos.size() / 15;


	//SetTimer(ROOM_DRAW_COMBINEGIFT_TIMER_ID, m_pInfo->nInterval, NULL);
}

void CCombineGiftDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ROOM_DRAW_COMBINEGIFT_TIMER_ID)
	{
		Draw();
	}

	if (nIDEvent == ROOM_DRAW_COMBINEGIFT_FREEZTIME_ID)
	{
		//PostMessage(WM_CLOSE, NULL, NULL);
		m_bFinish = true;
	}

	CDialog::OnTimer(nIDEvent);
}

void CCombineGiftDlg::Draw()
{
	CRect rect ;
	GetWindowRect( &rect ) ;
	CDC *pCDC = GetDC() ;
	HDC hdcTemp = pCDC->GetSafeHdc();
	HDC hdcMemory = CreateCompatibleDC( hdcTemp ) ;
	HBITMAP hBitMap = CreateCompatibleBitmap( hdcTemp , rect.Width(), rect.Height() ) ;
	HBITMAP hOld = (HBITMAP)SelectObject( hdcMemory , hBitMap ) ;
	Graphics graph( hdcMemory ) ;
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);

	//if (m_pBackImage != NULL)
	//{
	//	graph.DrawImage(m_pBackImage, m_pInfo->backPos.x, m_pInfo->backPos.y, m_pBackImage->GetWidth(), m_pBackImage->GetHeight());
	//}
	
	if (m_nType == 1)
	{
		if (m_nCount > m_pInfo->vecPos.size())
		{
			m_nCount = 0;
		}
		for (int i = 0; i < m_nCount; i++)
		{
			int nPosX = rand() % rect.Width();
			int nPoxY = rand() % rect.Height();
			if (m_pUnitImage != NULL)
			{
				graph.DrawImage(m_pUnitImage, m_pInfo->vecPos[i].x, m_pInfo->vecPos[i].y, m_pUnitImage->GetWidth(), m_pUnitImage->GetHeight()) ;
			}
		}
		m_nCount++;
	}
	else
	{
		if (m_bFirst)
		{
			//m_bFirst = false;
			//m_nCount = 0;
			for (int i = 0; i < m_nPointPerCircle; i++)
			{
				int nPosX = rand() % rect.Width();
				int nPoxY = rand() % rect.Height();
				CPoint point;
				point.x = nPosX;
				point.y = nPoxY;
				m_vecBeginPos.push_back(point);
				m_nCount++;
				if (m_nCount > m_pInfo->vecPos.size())
				{
					m_nCount = 0;
					m_bFirst = false;
					break;
				}
			}

			for (int i = 0; i < m_vecBeginPos.size(); i++)
			{
				if (m_pUnitImage != NULL)
				{
					graph.DrawImage(m_pUnitImage, m_vecBeginPos[i].x, m_vecBeginPos[i].y, m_pUnitImage->GetWidth(), m_pUnitImage->GetHeight()) ;
				}
			}
		}
		else
		{
			if (m_pBackImage != NULL && m_nCount == m_nFrames)
			{
				graph.DrawImage(m_pBackImage, m_pInfo->backPos.x, m_pInfo->backPos.y, m_pBackImage->GetWidth(), m_pBackImage->GetHeight());
			}
			m_nCount++;
			if (m_nCount > m_nFrames)
			{
				m_nCount = m_nFrames;
				KillTimer(ROOM_DRAW_COMBINEGIFT_TIMER_ID);
				SetTimer(ROOM_DRAW_COMBINEGIFT_FREEZTIME_ID, m_pInfo->nFreezeTime * 1000, NULL);
			}
			for (int i = 0; i < m_pInfo->vecPos.size(); i++)
			{
				int nPosX = (m_pInfo->vecPos[i].x - m_vecBeginPos[i].x)*m_nCount/m_nFrames + m_vecBeginPos[i].x;
				int nPosY = (m_pInfo->vecPos[i].y - m_vecBeginPos[i].y)*m_nCount/m_nFrames + m_vecBeginPos[i].y;
				if (m_pUnitImage != NULL)
				{
					graph.DrawImage(m_pUnitImage, nPosX, nPosY, m_pUnitImage->GetWidth(), m_pUnitImage->GetHeight()) ;
				}	
			}
		}
	}

	if (m_pCardImage != NULL)
	{
		graph.DrawImage(m_pCardImage, m_pInfo->cardPos.x, m_pInfo->cardPos.y, m_pCardImage->GetWidth(), m_pCardImage->GetHeight());

		FontFamily fontFamily(L"黑体");
		Font font(&fontFamily, 18, FontStyleBold, UnitPixel);
		SolidBrush solidBrush(Color(44, 171, 255));
		graph.DrawString(m_strSrcName.c_str(), m_strSrcName.length(), &font, PointF(m_pInfo->cardPos.x + m_pInfo->srcNamePos.x, m_pInfo->cardPos.y + m_pInfo->srcNamePos.y), &solidBrush);
		graph.DrawString(m_strDstName.c_str(), m_strDstName.length(), &font, PointF(m_pInfo->cardPos.x + m_pInfo->dstNamePos.x, m_pInfo->cardPos.y + m_pInfo->dstNamePos.y), &solidBrush);


		//std::wstring strText = L"送给";
		//graph.DrawString(strText.c_str(), strText.length(), &font, PointF(m_pInfo->dstNamePos.x - 50, m_pInfo->dstNamePos.y), &solidBrush2);
		if (m_pGiftItem != NULL)
		{
			FontFamily fontFamily2(L"黑体");
			Font font2(&fontFamily2, 14, FontStyleBold, UnitPixel);
			SolidBrush solidBrush2(Color(149,18,16));
			//std::wstring strText3 = m_pGiftItem->GetUnitName() + L" " + m_pGiftItem->GetName();
			graph.DrawString(m_pGiftItem->GetUnitName().c_str(), m_pGiftItem->GetUnitName().length(), &font2, PointF(m_pInfo->cardPos.x + m_pInfo->unitPos.x, m_pInfo->cardPos.y + m_pInfo->unitPos.y), &solidBrush2);
			graph.DrawString(m_pGiftItem->GetName().c_str(), m_pGiftItem->GetName().length(), &font2, PointF(m_pInfo->cardPos.x + m_pInfo->giftNamePos.x, m_pInfo->cardPos.y + m_pInfo->giftNamePos.y), &solidBrush2);
		}
	}

	if (!m_bFirst && m_nCount == m_nFrames)
	{
		if (m_pFrontImage != NULL)
		{
			graph.DrawImage(m_pFrontImage, m_pInfo->frontPos.x, m_pInfo->frontPos.y, m_pFrontImage->GetWidth(), m_pFrontImage->GetHeight());
		}
	}

	//BOOL bResult = BitBlt( hdcTemp , 0 , 0 , rect.Width() , rect.Height() , hdcMemory , 0 , 0 , SRCCOPY ) ;

	POINT ptWinPos = { rect.left,rect.top } ;
	POINT ptSrc= {0,0} ;
	SIZE sizeWindow = {rect.Width() , rect.Height() } ;
	::UpdateLayeredWindow( m_hWnd , hdcTemp , &ptWinPos ,&sizeWindow , hdcMemory ,&ptSrc , 0 ,&m_blend , ULW_ALPHA ) ;
	graph.ReleaseHDC( hdcMemory ) ;
	SelectObject( hdcMemory, hOld) ;
	DeleteObject( hBitMap) ;
	DeleteDC( hdcMemory ) ;
	ReleaseDC(pCDC);
}