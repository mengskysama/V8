// MicPopupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "MicPopupDlg.h"
#include "..\UserMessageDefine.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "VideoDlg.h"
#include "..\GlobalRoomDefine.h"

#define SKIN_ON_CLICK_BTN_CLOSE          1
#define SKIN_ON_CLICK_BTN_ZOOM_IN        2
#define SKIN_ON_CLICK_BTN_ZOOM_OUT       3


// CMicPopupDlg 对话框

IMPLEMENT_DYNAMIC(CMicPopupDlg, CDialog)

CMicPopupDlg::CMicPopupDlg( HWND hParent ,RoomAVLogic *pRoomAVLogic  ,CWnd* pParent /*=NULL*/)
	: CDialog(CMicPopupDlg::IDD, pParent),
	m_hParent( hParent ),
	m_pRoomAVLogic( pRoomAVLogic) ,
	m_unUserUIN( 0 )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_pVideoDlg = NULL;

	curSizeLevel = Size_Level_4;
}

CMicPopupDlg::~CMicPopupDlg()
{
	m_mapVideoSize.erase(m_mapVideoSize.begin(),m_mapVideoSize.end());
}

void CMicPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMicPopupDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_CLICK_VIDEO_DLG ,ClickVideoDlg)
END_MESSAGE_MAP()


// CMicPopupDlg 消息处理程序

int CMicPopupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"MicPopupDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	return 0;
}

LRESULT CMicPopupDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SKIN_ON_CLICK_BTN_CLOSE:
		{
			ShowWindow( SW_HIDE ) ;
			::SendMessage( m_hParent , WM_MICPOPUPDLG_CLOSE , m_unUserUIN , 0 ) ;
		}
		break;
	case SKIN_ON_CLICK_BTN_ZOOM_IN://放大
		{
            OnClickZoomIn();
		}
		break;
	case SKIN_ON_CLICK_BTN_ZOOM_OUT://缩小
		{
            OnClickZoomOut();
		}
		break;
	default:break;
	}

	return TRUE;
}

BOOL CMicPopupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    this->SetWindowPos(NULL,700,300,412,356,SWP_SHOWWINDOW);//初始为Size_Level_4时的主窗口大小
															//其中 412 = 408 +  THE_WIDTH_DIF;
	                                                        //356 = 306*(1+HEIGHT_ADD_PERSENT) + THE_HEIGHT_DIF

	InitVideoSize();

	//创建视频显示窗口
	m_pVideoDlg = new CVideoDlg( FALSE , this->m_hWnd , m_pRoomAVLogic );
	if (m_pVideoDlg)
	{
		m_pVideoDlg->Create(CVideoDlg::IDD,this);
		/*strVideoSize *pVideoSize = NULL;
		map<int,strVideoSize*>::iterator iter = m_mapVideoSize.begin();
		if ((iter=m_mapVideoSize.find(Size_Level_4)) != m_mapVideoSize.end())
		{
			pVideoSize = iter->second;
		}*/
		m_pVideoDlg->SetWindowPos(NULL,1,18,216,165,SWP_HIDEWINDOW);
		ShowSizeLevel(Size_Level_4);//初始为Size_Level_4大小
		m_pVideoDlg->ShowWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMicPopupDlg::OnClickZoomIn()
{
    if (curSizeLevel >= Size_Level_1 && curSizeLevel <= Size_Level_7 )
    {
		curSizeLevel++;

		if (curSizeLevel > Size_Level_7)
		{
			curSizeLevel = Size_Level_7;
		}

		ShowSizeLevel(curSizeLevel);	
    }
}

void CMicPopupDlg::OnClickZoomOut()
{
	if (curSizeLevel <= Size_Level_7 && curSizeLevel >= Size_Level_1 )
	{
		curSizeLevel--;

		if (curSizeLevel < Size_Level_1)
		{
			curSizeLevel = Size_Level_1;
		}

		ShowSizeLevel(curSizeLevel);
	}
}


void CMicPopupDlg::ShowSizeLevel(int sizeLevel)
{
	CRect curRc(0,0,0,0);
	GetWindowRect(&curRc);
	LONG videoWidth = 0 ;
	LONG videoHeight = 0 ; 
	strVideoSize *pVideoSize = NULL;

	if (sizeLevel >= Size_Level_1 && sizeLevel <= Size_Level_7)
	{
		map<int,strVideoSize*>::iterator iter = m_mapVideoSize.begin();
		if ((iter=m_mapVideoSize.find(sizeLevel)) != m_mapVideoSize.end())
		{
			pVideoSize = iter->second;
		}
		videoWidth = pVideoSize->lWidth;
		videoHeight = pVideoSize->lHeight;
	}
	
	CRect rc(curRc.left,curRc.top,curRc.right,curRc.bottom);
	rc.right = rc.left + (videoWidth+THE_WIDTH_DIF);
	rc.bottom = rc.top + (videoHeight+THE_HEIGHT_DIF);
	this->MoveWindow(rc);
	CRect videoRc(2,22,0,0);
	videoRc.right = videoRc.left + videoWidth;
	videoRc.bottom = videoRc.top + videoHeight;
	m_pVideoDlg->MoveWindow(videoRc);

}

LRESULT CMicPopupDlg::ClickVideoDlg( WPARAM wParam ,LPARAM lParam ) 
{
	::SendMessage( m_hParent , WM_CLICK_VIDEO_DLG , wParam , lParam ) ;
	return S_OK ;
}

HWND CMicPopupDlg::GetMicHandle()
{
	return m_pVideoDlg->GetSafeHwnd();
}

void CMicPopupDlg::SetDefault()
{
	m_pVideoDlg->SetToDefault();
}
BOOL CMicPopupDlg::PreTranslateMessage(MSG* pMsg)
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

void CMicPopupDlg::Init( CString& strName , uint32 unUIN , uint32 un179ID ) 
{
	m_unUserUIN = unUIN ;
	CString strTemp ;
	strTemp.Format( L"%s(%u)" ,strName , un179ID ) ;
	CComVariant var = strTemp ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textUser",L"text",var);		
	m_pVideoDlg->Init( m_unUserUIN ) ;
}

void CMicPopupDlg::InitVideoSize()
{
	strVideoSize * pVideoSize = NULL;
	//Size_Level_1
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 216;
	pVideoSize->lHeight = 162 + 3;
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_1, pVideoSize));
	
    //Size_Level_2
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 280;
	pVideoSize->lHeight = (LONG)(210*(1+HEIGHT_ADD_PERSENT));
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_2, pVideoSize));

	//Size_Level_3
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 344;
	pVideoSize->lHeight =(LONG)(258*(1+HEIGHT_ADD_PERSENT));
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_3, pVideoSize));

    //Size_Level_4
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 408;
	pVideoSize->lHeight =(LONG)(306*(1+HEIGHT_ADD_PERSENT));
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_4, pVideoSize));

	//Size_Level_5
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 472;
	pVideoSize->lHeight =(LONG)(354*(1+HEIGHT_ADD_PERSENT));
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_5, pVideoSize));

	//Size_Level_6
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 536;
	pVideoSize->lHeight =(LONG)(402*(1+HEIGHT_ADD_PERSENT));
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_6, pVideoSize));

	//Size_Level_7
	pVideoSize = new strVideoSize;
	pVideoSize->lWidth = 620;
	pVideoSize->lHeight =(LONG)(465*(1+HEIGHT_ADD_PERSENT));
	m_mapVideoSize.insert(std::pair<int,strVideoSize*>(Size_Level_7, pVideoSize));
}
