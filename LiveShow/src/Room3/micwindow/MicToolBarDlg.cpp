// PublicMicToolBarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "MicToolBarDlg.h"
#include "VideoDlg.h"
#include "MicPopupDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "P2PChatDlg.h"
#include "coredefine.h"

#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"


#define SKIN_ON_CLICK_BTN_POPUP        1
#define SKIN_ON_CLICK_BTN_VIDEO        2
#define SKIN_ON_CLICK_BTN_AUDIO        3
#define SKIN_ON_CLICK_BTN_TAKEPHOTO    4
#define SKIN_ON_MOUSE_LEAVE_BKG        5
// CPublicMicToolBarDlg 对话框

IMPLEMENT_DYNAMIC(CMicToolBarDlg, CDialog)

CMicToolBarDlg::CMicToolBarDlg( CVideoDlg* pVidepDlg ,CWnd* pParent /*=NULL*/)
	: CDialog(CMicToolBarDlg::IDD, pParent),
	m_pVideoDlg( pVidepDlg )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_hInst = NULL;
	m_btnPhoto = m_btnAudio = m_btnVideo = m_btnPopup = BTN_STATUS_OPEN;
	m_unUserUIN = 0 ;
}

CMicToolBarDlg::~CMicToolBarDlg()
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}

void CMicToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMicToolBarDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CPublicMicToolBarDlg 消息处理程序

int CMicToolBarDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"MicToolBarDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	LONG dwstyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) ;
	dwstyle |= WS_EX_LAYERED ;
	SetWindowLong( GetSafeHwnd() , GWL_EXSTYLE , dwstyle) ;

	typedef BOOL (WINAPI* FSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD) ;
	FSetLayeredWindowAttributes SetLayeredWindowAttributes ;
	m_hInst = LoadLibrary(_T("User32.DLL") ) ;
	SetLayeredWindowAttributes = (FSetLayeredWindowAttributes)GetProcAddress(m_hInst ,("SetLayeredWindowAttributes") ) ;
	if ( SetLayeredWindowAttributes)
	{
		SetLayeredWindowAttributes( m_hWnd , RGB(0,0,0) ,180 ,LWA_ALPHA) ;
		::RedrawWindow( m_hWnd , NULL ,NULL , RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN) ;
	}
	return 0;
}


LRESULT CMicToolBarDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SKIN_ON_CLICK_BTN_POPUP:
		{
		   OnClickPopup();
		}
		break;
	case SKIN_ON_CLICK_BTN_VIDEO:
		{
           OnClickVideo();
		}
		break;
	case SKIN_ON_CLICK_BTN_AUDIO:
		{
           OnClickAudio();
		}
		break;
	case SKIN_ON_CLICK_BTN_TAKEPHOTO:
		{
           OnClickTakePhoto();
		}
		break;
	case SKIN_ON_MOUSE_LEAVE_BKG:
		{
			common::ui::CCursorPos pt;
			CRect rect ;
			m_pVideoDlg->GetWindowRect( &rect ) ;
			CWnd* pWnd = WindowFromPoint(pt);
			if (pWnd != m_pVideoDlg && pWnd != this)
			{
				ShowWindow(SW_HIDE);
				break;
			}
			if ( !rect.PtInRect(pt) && IsWindowVisible() )
			{
				ShowWindow( SW_HIDE ) ;
			}
		} 
		break;
	default:break;
	}

	return TRUE;
}

void CMicToolBarDlg::OnClickPopup()
{	
	if ( m_btnPopup != BTN_STATUS_CLICKDISABLE )
	{	
		m_pVideoDlg->ChangeSize( m_btnPopup ) ;
		if ( m_btnPopup == BTN_STATUS_OPEN )
		{
			SetBtnPopupStatus( BTN_STATUS_CLOSE ) ;
		}
		else if ( m_btnPopup == BTN_STATUS_CLOSE )
		{
			SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
		}		
	}
	
}

void CMicToolBarDlg::OnClickVideo()
{    
	if ( m_btnVideo != BTN_STATUS_CLICKDISABLE )
	{
		m_pVideoDlg->ChangeVideo( m_btnVideo ) ;		
	}

}

void CMicToolBarDlg::OnClickAudio()
{   
	if ( m_btnAudio != BTN_STATUS_CLICKDISABLE )
	{
		m_pVideoDlg->ChangeAudio( m_btnAudio ) ;		 
	}

}

void CMicToolBarDlg::OnClickTakePhoto()
{
    m_pVideoDlg->Photo() ;
}

void CMicToolBarDlg::InitBtn( uint32 unUIN ,uint16 uUserStatus ) 
{
	m_unUserUIN = unUIN ;
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.unUIN == unUIN )
	{
		//视频默认打开
		SetBtnVideoStatus( BTN_STATUS_OPEN ) ;
		SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
		SetBtnPhotoStatus( BTN_STATUS_OPEN ) ;
		//默认音频是关闭的
		SetBtnAudioStatus( BTN_STATUS_CLOSE ) ;	
	}
	else
	{
		if ( uUserStatus & core::USERSTATUSTYPE_VIDEO  )
		{
			SetBtnVideoStatus( BTN_STATUS_OPEN ) ;
			SetBtnPhotoStatus( BTN_STATUS_OPEN ) ;
			SetBtnPopupStatus( BTN_STATUS_OPEN ) ;
		}
		else
		{
			//视频未打开
			SetBtnPopupStatus( BTN_STATUS_CLOSE , FALSE ) ;
			SetBtnPopupStatus( BTN_STATUS_CLICKDISABLE ) ;
			SetBtnVideoStatus( BTN_STATUS_CLOSE , FALSE ) ;
			SetBtnVideoStatus( BTN_STATUS_CLICKDISABLE ) ;
			SetBtnPhotoStatus( BTN_STATUS_CLOSE , FALSE ) ;
			SetBtnPhotoStatus( BTN_STATUS_CLICKDISABLE ) ;
		}
		if ( uUserStatus & core::USERSTATUSTYPE_AUDIO )
		{
			SetBtnAudioStatus( BTN_STATUS_OPEN ) ;
		}
		else
		{			
			//音频未打开
			SetBtnAudioStatus( BTN_STATUS_CLOSE , FALSE ) ;
			SetBtnAudioStatus( BTN_STATUS_CLICKDISABLE ) ;
		}
	}
}

void CMicToolBarDlg::SetBtnPopupStatus(eBtnStatus btnStatus , BOOL bHoveImage )
{
    m_btnPopup = btnStatus;
	CComVariant var;
	if (m_btnPopup == BTN_STATUS_CLICKDISABLE)
	{ 
		var = L"Room_forbid.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPopup",L"iconImage",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPopup",L"clickable",var);
		var = L"" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnPopup" ,L"hoverImage" ,var ) ;
		var = L"未开启视频，无法弹出" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnPopup" ,L"tip" ,var ) ;
		return;
	}else if (m_btnPopup == BTN_STATUS_OPEN)
	{   
		var = L"Room_imgBtnPopup.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPopup",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPopup",L"clickable",var);
		var = L"Room_btnMicToolBar_H.png" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnPopup" ,L"hoverImage" ,var ) ;
		var = L"点击弹出视频框" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnPopup" ,L"tip" ,var ) ;
		return;
	}else if (m_btnPopup == BTN_STATUS_CLOSE)
	{   
		var = L"Room_imgBtnNoPopup.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPopup",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPopup",L"clickable",var);
		if ( bHoveImage )
		{
			var = L"Room_btnMicToolBar_H.png" ;			
		}
		else
		{
			var = L"";
		}
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnPopup" ,L"hoverImage" ,var ) ;
		var = L"点击缩回视频框" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnPopup" ,L"tip" ,var ) ;

		return;
	}else
	{
		return;
	}
}

void CMicToolBarDlg::SetBtnVideoStatus(eBtnStatus btnStatus , BOOL bHoveImage )
{
	m_btnVideo = btnStatus;
	CComVariant var;
	if (m_btnVideo == BTN_STATUS_CLICKDISABLE)
	{ 
		var = L"Room_imgBtnVideoForbid.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnVideo",L"iconImage",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnVideo",L"clickable",var);
		var = L"" ;	
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnVideo" ,L"hoverImage" ,var ) ;
		var = L"对方未开启视频" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnVideo" ,L"tip" ,var ) ;
		return;
	}else if (m_btnVideo == BTN_STATUS_OPEN)
	{   
		var = L"Room_imgBtnVideoOn.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnVideo",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnVideo",L"clickable",var);
		var = L"Room_btnMicToolBar_H.png" ;	
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnVideo" ,L"hoverImage" ,var ) ;
		var = L"点击关闭视频" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnVideo" ,L"tip" ,var ) ;
		return;
	}else if (m_btnVideo == BTN_STATUS_CLOSE)
	{   
		var = L"Room_imgBtnVideoOff.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnVideo",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnVideo",L"clickable",var);
		if ( bHoveImage )
		{
			var = L"Room_btnMicToolBar_H.png" ;			
		}
		else
		{
			var = L"";
		}
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnVideo" ,L"hoverImage" ,var ) ;
		var = L"点击打开视频" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnVideo" ,L"tip" ,var ) ;
		return;
	}else
	{
		return;
	}
}

void CMicToolBarDlg::SetBtnAudioStatus(eBtnStatus btnStatus , BOOL bHoveImage )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	m_btnAudio = btnStatus;
	CComVariant var;
	if (m_btnAudio == BTN_STATUS_CLICKDISABLE)
	{ 
		var = L"Room_imgBtnAudioForbid.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnAudio",L"iconImage",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnAudio",L"clickable",var);
		var = L"对方未开启音频" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnAudio" ,L"tip" ,var ) ;
		return;
	}else if (m_btnAudio == BTN_STATUS_OPEN)
	{   
		if ( myCurInfo.unUIN == m_unUserUIN )
		{
			var = L"Room_imgBtnAudioOn2.png";
		}
		else
		{
			var = L"Room_imgBtnAudioOn.png";
		}
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnAudio",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnAudio",L"clickable",var);
		var = L"点击关闭音频" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnAudio" ,L"tip" ,var ) ;
		return;
	}else if (m_btnAudio == BTN_STATUS_CLOSE)
	{   
		if ( myCurInfo.unUIN == m_unUserUIN )
		{
			var = L"Room_imgBtnAudioOff2.png";
		}
		else
		{
			var = L"Room_imgBtnAudioOff.png";
		}		
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnAudio",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnAudio",L"clickable",var);
		if ( bHoveImage )
		{
			var = L"Room_btnMicToolBar_H.png" ;			
		}
		else
		{
			var = L"";
		}
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnAudio" ,L"hoverImage" ,var ) ;
		var = L"点击打开音频" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnAudio" ,L"tip" ,var ) ;
		return;
	}else
	{
		return;
	}
}

void CMicToolBarDlg::SetBtnPhotoStatus(eBtnStatus btnStatus , BOOL bHoveImage )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	m_btnPhoto = btnStatus;
	CComVariant var;
	if (m_btnPhoto == BTN_STATUS_CLICKDISABLE)
	{ 
		var = L"Room_imgBtnPhotoOff.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTakePhoto",L"iconImage",var);
		var = FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTakePhoto",L"clickable",var);
		var = L"" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnTakePhoto" ,L"hoverImage" ,var ) ;
		if ( myCurInfo.unUIN == m_unUserUIN )
		{
			var = L"未开启视频，无法拍照" ;
		}
		else
		{
			var = L"对方未开启视频，无法拍照" ;
		}
		
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnTakePhoto" ,L"tip" ,var ) ;
		return;
	}else if (m_btnPhoto == BTN_STATUS_OPEN)
	{   
		var = L"Room_imgBtnTakePhotoOn.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTakePhoto",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTakePhoto",L"clickable",var);
		var = L"Room_btnMicToolBar_H.png" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnTakePhoto" ,L"hoverImage" ,var ) ;
		var = L"点击拍照" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnTakePhoto" ,L"tip" ,var ) ;
		return;
	}else if (m_btnPhoto == BTN_STATUS_CLOSE)
	{   
		var = L"Room_imgBtnPhotoOff.png";
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTakePhoto",L"iconImage",var);
		var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTakePhoto",L"clickable",var);
		if ( bHoveImage )
		{
			var = L"Room_btnMicToolBar_H.png" ;			
		}
		else
		{
			var = L"";
		}
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnTakePhoto" ,L"hoverImage" ,var ) ;
		var = L"未开启视频，无法拍照" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"btnTakePhoto" ,L"tip" ,var ) ;
		return;
	}else
	{
		return;
	}
}

BOOL CMicToolBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CMicToolBarDlg::PreTranslateMessage(MSG* pMsg)
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
	else if ( pMsg->message == WM_ERASEBKGND )
	{
		return TRUE ;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
