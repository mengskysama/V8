// ScrollBtnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PaodaoBtnDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\RoomParentDlg.h"
#include "..\paodao\PaodaoDlg.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "PaodaoStaitc.h"
#define  SKIN_ON_CLICK_BTN_PAODAO        1
// CScrollBtnDlg 对话框

IMPLEMENT_DYNAMIC(CPaodaoBtnDlg, CDialog)

CPaodaoBtnDlg::CPaodaoBtnDlg( CRoomParentDlg* pParentDlg  ,CPaodaoStaitc* pPaodaoStaitc , CWnd* pParent /*=NULL*/)
: CDialog(CPaodaoBtnDlg::IDD, pParent),
m_pParentDlg( pParentDlg ),
m_pPaodaoStatic( pPaodaoStaitc )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pPaodaoDlg = NULL ;
}

CPaodaoBtnDlg::~CPaodaoBtnDlg()
{
	SAFE_DELETE_DIALOG(m_pPaodaoDlg) ;
}

void CPaodaoBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaodaoBtnDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()


// CPaodaoBtnDlg 消息处理程序

BOOL CPaodaoBtnDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
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

	return CDialog::PreTranslateMessage(pMsg);
}

int CPaodaoBtnDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"PaodaoBtnDlg");
		ASSERT(hr == S_OK && __LINE__);
	}
	ModifyStyleEx( WS_EX_APPWINDOW , 0 ) ;
	ModifyStyleEx( WS_EX_TOOLWINDOW , 0 ) ;
	return 0;
}

LRESULT CPaodaoBtnDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_PAODAO:
		{
			ShowWindow( SW_HIDE ) ;
			common::ui::CCursorPos pt;
			SwitchResourceToModule(_T("Room"));
			if ( m_pPaodaoDlg == NULL )
			{
				m_pPaodaoDlg = new CPaodaoDlg(m_pParentDlg) ;
				m_pPaodaoDlg->Create(CPaodaoDlg::IDD,this);				

			}
			if ( !m_pPaodaoDlg->IsWindowVisible() )
			{
				m_pPaodaoDlg->SetWindowPos(NULL,pt.x-260,pt.y+25,260,260,SWP_SHOWWINDOW|SWP_NOZORDER);
			}
			else
			{
				m_pPaodaoDlg->ShowWindow( SW_HIDE ) ;
			}
		}
		break;
	default:break;
	}

	return TRUE;
}


void CPaodaoBtnDlg::OnClickNameLink( CString& strName , uint32 unUUID )
{
	if ( m_pPaodaoDlg != NULL )
	{
		m_pPaodaoDlg->OnClickNameLink( strName , unUUID ) ;
	}
}

void CPaodaoBtnDlg::OnDeleteUser(IUserItem * pUserItem)
{
	if ( m_pPaodaoDlg != NULL )
	{
		m_pPaodaoDlg->OnDeleteUser( pUserItem ) ;
	}
}
void CPaodaoBtnDlg::OnRoomRightSendToChange( CString& strSelText )
{
	if ( m_pPaodaoDlg != NULL )
	{
		m_pPaodaoDlg->OnRoomRightSendToChange( strSelText ) ;
	}
}

void CPaodaoBtnDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme ;
	tme.cbSize = sizeof( tme ) ;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1 ;
	tme.hwndTrack = m_hWnd ;
	::TrackMouseEvent(&tme) ;
	CDialog::OnMouseMove(nFlags, point);
}


HRESULT CPaodaoBtnDlg::OnMouseLeave( WPARAM wParam , LPARAM lParam )
{
	common::ui::CCursorPos pt;
	CRect rect ;
	m_pPaodaoStatic->GetClientRect( &rect ) ;
	m_pPaodaoStatic->ClientToScreen( &rect ) ;
	if ( !rect.PtInRect(pt) && IsWindowVisible() )
	{
		ShowWindow( SW_HIDE ) ;
		m_pPaodaoStatic->StartMove() ;
	}
	return S_OK ;
}

void CPaodaoBtnDlg::UpdateGiftList()
{
	if (m_pPaodaoDlg != NULL)
	{
		m_pPaodaoDlg->UpdateGiftList();
	}
}