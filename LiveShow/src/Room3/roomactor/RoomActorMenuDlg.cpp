// UserInfoCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinUtil.h"
#include "RoomActorMenuDlg.h"
#include "..\RoomParentDlg.h"
#include "..\..\include\utility\SystemHelper.h"
#include "..\RoomMidDlg.h"
#include "..\RoomRightDlg.h"
#include "RoomActor.h"


#define SKIN_ON_CLICK_ACTORINFO			 1
#define SKIN_ON_CLICK_SELTALK			 2
#define SKIN_ON_CLICK_SELSENDGIFTS		 3
#define SKIN_ON_CLICK_SEEPRIVATEVIDEO	 4
#define SKIN_ON_CLICK_ONETOONEVIDEO		 5
// CRoomActorMenuDlg 对话框

IMPLEMENT_DYNAMIC(CRoomActorMenuDlg, CDialog)

CRoomActorMenuDlg::CRoomActorMenuDlg( CRoomParentDlg* pParentDlg ,CRoomActor* pRoomActor,CWnd* pParent /*=NULL*/)
	: CDialog(CRoomActorMenuDlg::IDD, pParent),
	m_pParentDlg( pParentDlg ),
	m_RoomActor(pRoomActor)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_pActor = NULL;

	
}

CRoomActorMenuDlg::~CRoomActorMenuDlg()
{
}

void CRoomActorMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomActorMenuDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


// CRoomActorMenuDlg 消息处理程序


int CRoomActorMenuDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RoomActorMenuDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CRoomActorMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRoomActorMenuDlg::OnTimer(UINT_PTR nIDEvent)
{	
	CDialog::OnTimer(nIDEvent);

	if(nIDEvent == ROOMACTOR_MOUSE_TIMER_IDEVENT )
	{		
		if(m_RoomActor != NULL)
		{
			KillTimer(ROOMACTOR_MOUSE_TIMER_IDEVENT);
			m_RoomActor->ShowActorMenuDlg(); 
		}
	}	
	else if(nIDEvent == ROOMACTOR_GIDUE_TIMER_IDEVENT)
	{
		if(m_RoomActor != NULL)
		{
			KillTimer(ROOMACTOR_GIDUE_TIMER_IDEVENT);
			m_RoomActor->ShowGuideEnd();
		}
	}


}

void CRoomActorMenuDlg::SetActorInfo(ActorInfo *pActor)
{
	ASSERT(pActor != NULL);
	m_pActor = pActor;
}
void CRoomActorMenuDlg::Show( ActorInfo *pActor )
{
	ASSERT(pActor != NULL);
	
	m_pActor = pActor;
	CString strTemp ;
	strTemp.Format( L"%s" , pActor->m_ActorRoleName.c_str()) ;

	CComVariant var = strTemp ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textRoomActorName", L"text", var );	


	if(pActor->m_IsLine) //在线
	{
		CComVariant varVisible = TRUE;		
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSelTalk", L"visible", varVisible); 
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSelSendGifts", L"visible", varVisible); 
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSeePrivateVideo", L"visible", varVisible); 
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnOneToOneVideo", L"visible", varVisible); 


	}
	else  //离线
	{
		CComVariant varVisible = FALSE;		
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSelTalk", L"visible", varVisible); 
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSelSendGifts", L"visible", varVisible); 
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSeePrivateVideo", L"visible", varVisible); 
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnOneToOneVideo", L"visible", varVisible); 

	}

	ShowWindow(SW_SHOWNOACTIVATE);  
}

void CRoomActorMenuDlg::Hide()
{
	this->ShowWindow(SW_HIDE);
}

void CRoomActorMenuDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CRoomActorMenuDlg::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	this->ShowWindow(SW_HIDE);
	OutputDebugString(_T("CRoomActorMenuDlg::OnMouseLeave"));

	return TRUE;
}

LRESULT CRoomActorMenuDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_ACTORINFO:
		{
			OnClickbtnActorInfo();
		}
		break;
	case SKIN_ON_CLICK_SELTALK:
		{
			OnClickbtnSelTalk() ;
		}
		break;
	case SKIN_ON_CLICK_SELSENDGIFTS:
		{
			OnClickbtnSelSendGifts();
		}
		break;
	case SKIN_ON_CLICK_SEEPRIVATEVIDEO:
		{
			OnClickbtnSeePrivateVideo();
		}
		break;
	case SKIN_ON_CLICK_ONETOONEVIDEO:
		{
			OnClickbtnOneToOneVideo();
		}
		break;
	default:break;
	}


	return TRUE;
}

void CRoomActorMenuDlg::OnClickbtnActorInfo() 
{
	this->ShowWindow( SW_HIDE ) ;		
	
	if(m_pActor != NULL)
	{
		m_pParentDlg->OnOtherUserInfo(m_pActor->m_ActorUin,m_pActor->m_IsMan,true);
	}
	
}

void CRoomActorMenuDlg::OnClickbtnSelTalk() 
{
	this->ShowWindow( SW_HIDE ) ;
	if(m_pActor == NULL)
	{
		return;
	}	

	CRoomMidDlg* pRoomMidDlg = m_pParentDlg->GetRoomMidDlg();
	if(pRoomMidDlg!= NULL)
	{		
		CString actorName;
		actorName.Format(_T("%s"),m_pActor->m_ActorRoleName.c_str());
		pRoomMidDlg->OnClickNameLink(actorName,m_pActor->m_ActorUin);
	}
}


void CRoomActorMenuDlg::OnClickbtnSelSendGifts()
{
	this->ShowWindow(SW_HIDE);
	if(m_pActor == NULL)
	{
		return;
	}	

	CRoomRightDlg* pRoomRightDlg = m_pParentDlg->GetRoomRightDlg();
	if(pRoomRightDlg!= NULL)
	{	
		pRoomRightDlg->SetSendObj(m_pActor->m_ActorRoleName.c_str(),m_pActor->m_ActorUin);
	}
}

void CRoomActorMenuDlg::OnClickbtnSeePrivateVideo()
{
	this->ShowWindow(SW_HIDE);	
	if(m_pActor == NULL)
	{
		return;
	}
	m_pParentDlg->SendOpenPrivate(m_pActor->m_ActorUin);
}

void CRoomActorMenuDlg::OnClickbtnOneToOneVideo()
{
	this->ShowWindow(SW_HIDE);
	if(m_pActor == NULL)
	{
		return;
	}
	m_pParentDlg->SendRequestP2PMic(m_pActor->m_ActorUin);
}