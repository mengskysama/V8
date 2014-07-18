// UserInfoCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SkinUtil.h"
#include "UserInfoCardDlg.h"
#include "..\userlist\ListIconManager.h"
#include "..\RoomParentDlg.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"
#include "..\..\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary/include/utility/URLEncode.h"
#include "..\gift\GiftManager.h"
#include "..\GlobalRoomDefine.h"
#include "..\richlevel\RichLevelManager.h"

#define  SKIN_ON_CLICK_DETAIL          1
#define  SKIN_ON_CLICK_PHOTO           2
#define  SKIN_ON_CLICK_SELFVERIFY      3
// CUserInfoCardDlg 对话框

IMPLEMENT_DYNAMIC(CUserInfoCardDlg, CDialog)

CUserInfoCardDlg::CUserInfoCardDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent /*=NULL*/)
	: CDialog(CUserInfoCardDlg::IDD, pParent),
	m_pParentDlg( pParentDlg )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_unUserUIN = 0 ;


	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();
	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);
	m_ResListiconPath.Format( L"%s%s" , strModulePath.c_str() , LISTICON_DIR ) ;
}

CUserInfoCardDlg::~CUserInfoCardDlg()
{
}

void CUserInfoCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserInfoCardDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER, &CUserInfoCardDlg::OnClickMsgBoxGoToRegister)
END_MESSAGE_MAP()


// CUserInfoCardDlg 消息处理程序


int CUserInfoCardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"UserInfoCardDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CUserInfoCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//根据冠获取头像部分VIP图标的路径
void CUserInfoCardDlg::GetVIPPath(uint8 vipLevel,CString &imgVip)
{		
	switch(vipLevel)
	{
	case core::VIPLEVEL_NONE:  //未注册
		{
			imgVip.Format(_T("%sLevelVipNone.png"),m_ResListiconPath);
		}
		break;
	case core::VIPLEVEL_REGISTER:  //已注册
		{
			imgVip.Format(_T("%sLevelVipNone.png"),m_ResListiconPath);		
		}
		break;
	case core::VIPLEVEL_VIP:   //vip
		{
			imgVip.Format(_T("%sLevelVip.png"),m_ResListiconPath);			
		}
		break;
	case core::VIPLEVEL_HONOR:  //荣耀vip
		{
			imgVip.Format(_T("%sLevelVipHonor.png"),m_ResListiconPath);		
		}
		break;
	case core::VIPLEVEL_SILVER:  //银冠
		{
			imgVip.Format(_T("%sLevelSilver.png"),m_ResListiconPath);
		}
		break;
	case core::VIPLEVEL_GOLDEN:  //金冠
		{
			imgVip.Format(_T("%sLevelGolden.png"),m_ResListiconPath);
		}
		break;
	case core::VIPLEVEL_CRYSTAL:  //水晶皇冠
		{
			imgVip.Format(_T("%sLevelCrystal.png"),m_ResListiconPath);		
		}
		break;
	case core::VIPLEVEL_DIAMOND:  //钻石皇冠
		{
			imgVip.Format(_T("%slevelDiamond.png"),m_ResListiconPath);		
		}
		break;
	case core::VIPLEVEL_BLACKDIAMOND:  //黑钻皇冠
		{
			imgVip.Format(_T("%sLevelBlackDiamond.png"),m_ResListiconPath);		
		}
		break;
	case core::VIPLEVEL_TOP:  //至尊皇冠
		{
			imgVip.Format(_T("%sLevelTop.png"),m_ResListiconPath);				
		}
		break;

	default: 
		imgVip.Format(_T("%sLevelVipNone.png"),m_ResListiconPath);	
		break;
	}	
}

void CUserInfoCardDlg::Show( IUserItem * pUserItem )
{
	m_unUserUIN = pUserItem->GetUserUIN() ;
	CString strTemp ;
	strTemp.Format( L"%s(%u)" , pUserItem->GetUserName().c_str() , pUserItem->GetUser179ID() ) ;

	CComVariant var = strTemp ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textNameAndCount", L"text", var );

	CString strVIPIconPath ;
	GetVIPPath( pUserItem->GetUserVipLevel() , strVIPIconPath ) ;
	std::wstring strVIPIcon = ListIconManager::GetVIPIcon(pUserItem ) ;

	var = strVIPIconPath ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgVip" ,L"absoluteImage" , var ) ;

	if ( pUserItem->IsActor() )
	{
		var = TRUE ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPhoto" , L"visible" , var ) ;
	}
	else
	{
		var = FALSE ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPhoto" , L"visible" , var ) ;
	}
	Hall::HeadImage headImage ;
	headImage.m_bMan = pUserItem->IsMale() ;
	headImage.m_unUIN = m_unUserUIN ;
	headImage.m_isGuest = (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE ? true : false);
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_UPDATE_HEAD_IMAGE_PATH, (param)&headImage);
	var = CString(headImage.m_HeadImagePath.c_str()) ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgUserHead", L"absoluteImage", var );

	uint16 unGiftID = 0 ;
	uint32 unCount = 0 ;
	vector<core::GiftStarInfo> vGiftStarInfoList = pUserItem->GetGiftStarInfo() ;
	vector<core::GiftStarInfo>::iterator iter ;
	for ( iter = vGiftStarInfoList.begin() ; iter != vGiftStarInfoList.end() ; ++iter )
	{
		if ( iter->unCount > unCount )
		{
			unGiftID = iter->unGiftID ;
		}
	}

	IGiftItem* pGiftItem = NULL ;
	if ( unGiftID != 0 )
	{
		pGiftItem =  GiftManager::GetInstance()->SearchGiftByID( unGiftID ) ;
	}
	if ( pGiftItem != NULL )
	{
		var = pGiftItem->GetSmallImagePath().c_str() ;		
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGift", L"absoluteImage", var );

		var = L"第一名" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGiftRank", L"text", var );
	}
	else
	{
		var = L"" ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGift", L"absoluteImage", var );
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGiftRank", L"text", var );
	}
	SetRichLevel(pUserItem);

	//设置销售认证
	var = pUserItem->IsAgent();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgBaiTuVerify", L"visible", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSelfVerify", L"visible", var);

	this->ShowWindow(SW_HIDE);
	this->ShowWindow(SW_SHOW);
}

void CUserInfoCardDlg::Hide()
{
	this->ShowWindow(SW_HIDE);
}

void CUserInfoCardDlg::OnMouseMove(UINT nFlags, CPoint point)
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

LRESULT CUserInfoCardDlg::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	this->ShowWindow(SW_HIDE);

	return TRUE;
}

LRESULT CUserInfoCardDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_DETAIL:
		{
			OnClickDetail();
		}
		break;
	case SKIN_ON_CLICK_PHOTO:
		{
			OnClickPhoto() ;
		}
		break;
	case SKIN_ON_CLICK_SELFVERIFY:
		{
			OnClickSelfVerify();
		}
		break;
	default:break;
	}


	return TRUE;
}

void CUserInfoCardDlg::OnClickDetail() 
{
	this->ShowWindow( SW_HIDE ) ;
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel ==  core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能查看详细信息，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return ;
	}

	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
	m_pParentDlg->OnUserInfo( pUserItem ) ;
}

void CUserInfoCardDlg::OnClickPhoto() 
{
	this->ShowWindow( SW_HIDE ) ;
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel ==  core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能查看详细信息，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return ;
	}

	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
	m_pParentDlg->OnUserInfo( pUserItem ) ;
}

void CUserInfoCardDlg::OnHallHeadImageUpdate() 
{
	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
	if ( pUserItem != NULL )
	{
		Hall::HeadImage headImage ;
		headImage.m_bMan = pUserItem->IsMale() ;
		headImage.m_unUIN = m_unUserUIN ;
		headImage.m_isGuest = (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE ? true : false);
		g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_HEAD_IMAGE_PATH, (param)&headImage);
		CComVariant var = (CString)headImage.m_HeadImagePath.c_str() ;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgUserHead", L"absoluteImage", var );
	}
}

void CUserInfoCardDlg::SetRichLevel(IUserItem * pUserItem)
{
	//设置富豪图标
	if (pUserItem->GetUserVipLevel() > core::VIPLEVEL_NONE && pUserItem->GetUserRichLevel() > 0)
	{
		std::wstring strRichLevelIco = RichLevelManager::GetRichLevelImgPath(pUserItem);
		if (!strRichLevelIco.empty())
		{
			CComVariant var = strRichLevelIco.c_str();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"visible", (CComVariant)true);
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"absoluteImage", var);
			m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"originWidth", &var);  
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"width", var);
			m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"originHeight", &var);
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"height", var);
		}
	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgRichLevel", L"visible", (CComVariant)false);
	}
}

void CUserInfoCardDlg::OnClickSelfVerify()
{
	this->ShowWindow(SW_HIDE);
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel ==  core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能查看销售账户，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return ;
	}

	IUserItem* pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( m_unUserUIN ) ;
	m_pParentDlg->OnAgentInfo( pUserItem ) ;
}

LRESULT CUserInfoCardDlg::OnClickMsgBoxGoToRegister( WPARAM wParam, LPARAM lParam )
{
	if ((CUserInfoCardDlg*)wParam == this)
	{
		m_pParentDlg->OnClickGotoRegister();
	}

	return TRUE;
}