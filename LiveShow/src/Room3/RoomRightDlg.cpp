// RoomRightDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Room.h"
#include "RoomParentDlg.h"
#include "RoomRightDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "userlist/SearchDlg.h"
#include "userlist/IconIntroductionDlg.h"
#include "gift/PresentDetailDlg.h"
#include "usermanager/UserManager.h"
#include "UserMessageDefine.h"
#include "userlist/UserListMenu.h"
#include "SendEventManager.h"
#include "roomsetting/RoomOption.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"
#include "..\CommonLibrary\include\ui\ImageEx.h"
#include "..\ErrorReport\include\crashrpt.h"
#include "personalsetting/PersonalConfig.h"

#define ICON_INTRO_WIDTH	477
#define ICON_INTRO_HEIGHT	760

#define  SKIN_NOTIFY_SEARCH_DLG_HWND         1
#define  SKIN_ON_CLICK_TABBTN_MGR            2
#define  SKIN_ON_CLICK_TABBTN_USER			 3
#define  SKIN_ON_CLICK_TABBTN_MIC			 4
#define  SKIN_ON_CLICK_BTN_SEARCH			 5
#define  SKIN_ON_CLICK_ICON_INSTRUCTION		 6
#define  SKIN_ON_CLICK_BTN_WHEREIAM          7
#define  SKIN_ON_CLICK_BTN_GIFTLIST_DOWN     8
#define  SKIN_NOTIFY_GIFTLIST_DOWN           9
#define  SKIN_ON_CLICK_BTN_GIFTLIST_UP       10
#define  SKIN_ON_CLICK_TAB_GIFT              11
#define  SKIN_ON_CLICK_PRESENT_DETAIL        12
#define  SKIN_ON_CLICK_SPEAKER               13
#define  SKIN_NOTIFY_SEARCH_LIST			 14
#define  SKIN_ON_CLICK_BTN_MY_GOODS			 15
#define  SKIN_NOTIFY_SHOW_USERITEM			 16
#define  SKIN_ON_CLICK_BTN_PAY_VIP           17
#define  SKIN_NOTIFY_MARK_DLG				 18
#define  SKIN_ON_CLICK_BTN_GIFT				 19
#define  SKIN_ON_CLICK_BTN_GAME				 20
#define  SKIN_ON_CLICK_BTN_BAITU_SERVICE	 21
#define  SKIN_ON_CLICK_TEXT_PAY				 22


#define  BTN_PAYMONEY_IMAGE_COUNT            8    //充值按钮帧数

// CRoomRightDlg 对话框

IMPLEMENT_DYNAMIC(CRoomRightDlg, CDialog)

CRoomRightDlg::CRoomRightDlg(CRoomParentDlg* pParentDlg, CWnd* pParent /*=NULL*/)
: CDialog(CRoomRightDlg::IDD, pParent),
m_pParentDlg(pParentDlg),
m_SpeakerLogic(pParentDlg, true)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pPresentDetailDlg = NULL;
	m_pRoomBusiness = NULL;
	m_pPresentDetailDlg = NULL;
	m_pGiftTipDlg = NULL;

	m_pCoinFlashWnd = NULL;
	m_nGiftTypeCurSel = 0;
	m_nPayImageIndex = 1;
	m_unLastMoney = 0 ;
	//m_pGifBtnPayMoney = NULL;
}

CRoomRightDlg::~CRoomRightDlg()
{
	SAFE_DELETE_DIALOG(m_pPresentDetailDlg) ;	
	for (std::map<std::wstring, CGiftListCtrl*>::iterator it = m_mapGiftListCtrl.begin(); it != m_mapGiftListCtrl.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
	}
	if (m_pCoinFlashWnd != NULL)
	{
		m_pCoinFlashWnd->Finish();  
		SAFE_DELETE_DIALOG(m_pCoinFlashWnd);
	}

}

void CRoomRightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROOM_GIVETO, m_GiftSendToCombo);
	DDX_Control(pDX, IDC_COMBO_ROOM_AMMOUNT, m_GiftCountCombo);
	DDX_Control(pDX, IDC_ROOM_MYGOODS, m_MyGoodsListCtrl);
}


BEGIN_MESSAGE_MAP(CRoomRightDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_CLICK_USERLIST, OnClickUserList)
	ON_MESSAGE(WM_CLICK_GIFTLIST, OnClickGiftList)
	ON_MESSAGE(WM_SHOW_GIFT_TIP, OnShowGiftTip)
	ON_CBN_SELCHANGE(IDC_COMBO_ROOM_GIVETO, &CRoomRightDlg::OnCbnSelchangeComboRoomGiveto)
	//ON_MESSAGE(WM_ONCLICK_GIFBUTTON , &CRoomRightDlg::OnClickGifButton )
	ON_MESSAGE(COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER, &CRoomRightDlg::OnClickMsgBoxGoToRegister)
END_MESSAGE_MAP()


// CRoomRightDlg 消息处理程序

int CRoomRightDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RoomRightDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CRoomRightDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//创建喇叭IE控件
	m_pSpeakerHtml = new COutputHtmlEditImpl(ROOM_OUTPUTSPEAKER_MAX_HOLD_LINE , HTML_TYPE_SPEAK ) ;
	m_pSpeakerHtml->SetHtmlEditLogic( &m_SpeakerLogic ) ;
	m_pSpeakerHtml->Create( this ,IDC_HTML_ROOM_SPEAKER , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE) ;
	CComVariant var = (long)m_pSpeakerHtml->GetSafeHwnd() ;

	m_pParentDlg->SetSpeakerOutputGotoEndAble( CPersonalConfig::GetInstance()->GetIsAutoScrollSpeaker() ) ;

	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlSpeaker",L"hwnd",var);

	std::wstring strRoomName = L"";
	bool bShowRoomName = false;
	CRoomParentDlg * pParentDlg = (CRoomParentDlg *)GetParent();
	if (pParentDlg != NULL)
	{
		RoomOption * pRoomOption = pParentDlg->GetRoomOption();
		if (pRoomOption != NULL)
		{
			strRoomName = pRoomOption->GetRoomName();
		}
	}

	//礼物列表初始化
	InitGiftListCtrl();

	//包裹初始化
	m_MyGoodsListCtrl.InitGiftListCtrl();

	//礼物数量框初始化
	m_GiftCountCombo.InitGiftCombo();

	////////关于礼物列表上升下降初始化
	//SetAboutGiftVisible(FALSE);

	CComVariant arrow_var = TRUE;
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListDown",L"visible",arrow_var);//将向下的箭头图标显示	
	arrow_var = FALSE;
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListUp",L"visible",arrow_var);//将向上的箭头图标隐藏

	//加币效果
	InitCoinFlashWnd();

	//DrawGifButton();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CRoomRightDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CRoomRightDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_GIFTLIST_DOWN:
		{
			//OnClickBtnGiftListDown();
		}
		break;
	case SKIN_ON_CLICK_BTN_GIFTLIST_UP:
		{
			//OnClickBtnGiftListUp();
		}
		break;
	case SKIN_ON_CLICK_TAB_GIFT:
		{
			m_nGiftTypeCurSel = (int)lParam;
			OnClickTabGift((int)lParam);
		}
		break;
	case SKIN_ON_CLICK_PRESENT_DETAIL:
		{
			OnClickPresentDetail();
		}
		break;
	case SKIN_ON_CLICK_SPEAKER:
		{
			//OnClickSpeaker();
		}
		break;
	case SKIN_ON_CLICK_BTN_MY_GOODS:
		{
			OnClickBtnMyGoods();
		}
		break;
	case SKIN_ON_CLICK_BTN_PAY_VIP:
		{
			OnClickBtnPayVIP();
		}
		break;
	case SKIN_ON_CLICK_BTN_GIFT:
		{
			OnClickBtnGift();
		}
		break;
	case SKIN_ON_CLICK_BTN_GAME:
		{
			OnClickBtnGame();
		}
		break;
	case SKIN_ON_CLICK_BTN_BAITU_SERVICE:
		{
			OnClickBtnBaiTuService();
		}
		break;
	case SKIN_ON_CLICK_TEXT_PAY:
		{
			OnClickBtnPayVIP();
		}
		break;
	default:break;
	}

	return TRUE;
}

//void CRoomRightDlg::OnClickBtnGiftListDown()
//{
//	if(m_pSkinEngine != NULL)
//	{
//		CComVariant var = FALSE;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListDown",L"visible",var);//将向下的箭头图标隐藏
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListUp",L"visible",var);//将向上的箭头图标隐藏
//		SetAboutGiftVisible(TRUE);
//		var = TRUE;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListUp",L"visible",var);//将向上的箭头图标显示
//		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_GIFTLIST_DOWN,(LONG)0);
//	}	
//}
//
//void CRoomRightDlg::SetAboutGiftVisible(BOOL visible)
//{
//	if(m_pSkinEngine == NULL)
//	{
//		return;
//	}
//
//	CComVariant var = visible;
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgGiftAndUp",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textGiftAndUp",L"visible",var);
//
//	var = !visible;
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgExpand",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textSendTo",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemComboSendTo",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textAmount",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemComboAmount",L"visible",var);
//	if (CFlashWnd::IsSupportFlash() && m_pCoinFlashWnd != NULL)
//	{
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgCoin",L"visible", (CComVariant)FALSE);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textCoin",L"visible", (CComVariant)FALSE);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textHowMuch",L"visible", (CComVariant)FALSE);
//
//		visible ? (m_pCoinFlashWnd->ShowWindow(SW_HIDE)) : (m_pCoinFlashWnd->ShowWindow(SW_SHOWNOACTIVATE));
//
//	}else
//	{
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgCoin",L"visible",var);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textCoin",L"visible",var);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textHowMuch",L"visible",var);
//	}
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPresentDetail",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnPayVIP", L"visible", var);
//	MoveGifButton(var.boolVal);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgBtnGiftBk",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgMallTitle",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftHot",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftLove",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftFriend",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftLuxury",L"visible",var);
//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftProps",L"visible",var);
//
//	if (!visible)
//	{   
//		//CComVariant tempVar;
//		int nCurSel = m_nGiftTypeCurSel;
//
//		if (0 == nCurSel)
//		{
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"HotGiftList",L"visible",var);
//		}
//		else if (1 == nCurSel)
//		{
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"LoveGiftList",L"visible",var);
//		}
//		else if (2 == nCurSel)
//		{
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"FriendGiftList",L"visible",var);
//		}
//		else if (3 == nCurSel)
//		{
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"LuxuryGiftList",L"visible",var);
//		}
//		else if (4 == nCurSel)
//		{
//			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"PropsList",L"visible",var);
//		}
//	}
//	else
//	{
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"HotGiftList",L"visible",var);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"LoveGiftList",L"visible",var);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"FriendGiftList",L"visible",var);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"LuxuryGiftList",L"visible",var);
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"PropsList",L"visible",var);
//	}
//}

//void CRoomRightDlg::OnClickBtnGiftListUp()
//{
//	if(m_pSkinEngine != NULL)
//	{
//		CComVariant var = FALSE;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListDown",L"visible",var);//将向下的箭头图标隐藏
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListUp",L"visible",var);//将向上的箭头图标隐藏
//		SetAboutGiftVisible(FALSE);
//		var = TRUE;
//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnGiftListDown",L"visible",var);//将向下的箭头图标显示
//		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_GIFTLIST_DOWN,(LONG)0);
//	}
//}

void CRoomRightDlg::OnClickTabGift(int nTab)
{
	ShowGiftListCtrlByTab(nTab);
}

void CRoomRightDlg::OnAddUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	int nIndex = pUserItem->GetPublicMicIndex() ;
	if ( nIndex != 0 )
	{
		int iComboIndex = m_GiftSendToCombo.InsertString(0 ,pUserItem->GetUserName().c_str());
		m_GiftSendToCombo.SetItemData( iComboIndex ,pUserItem->GetUserUIN());
		int nCurSel = m_GiftSendToCombo.GetCurSel() ;
		if ( nCurSel != CB_ERR )
		{
			//默认选中低序公麦
			uint32 unUIN = m_GiftSendToCombo.GetItemData( nCurSel ) ;
			IUserItem* pCurSelItem = GetUserManager()->SearchUserByUIN( unUIN ) ;
			if ( pCurSelItem != NULL )
			{
				int nCurSelIndex = pCurSelItem->GetPublicMicIndex() ;
				if ( nCurSelIndex != 0 && nCurSelIndex < nIndex )
				{
					iComboIndex = nCurSel ;
				}
			}
		}
		m_GiftSendToCombo.SetCurSel( iComboIndex );		
	}

}

void CRoomRightDlg::OnDeleteUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	int nIndex = m_GiftSendToCombo.FindString( 0, pUserItem->GetUserName().c_str());
	if (nIndex != CB_ERR)
	{
		m_GiftSendToCombo.DeleteString(nIndex);
		m_GiftSendToCombo.Invalidate(TRUE);
	}
}

void CRoomRightDlg::SetRoomBusiness( RoomBusiness * pRoomBusiness )
{
	ASSERT(pRoomBusiness != NULL);
	m_pRoomBusiness = pRoomBusiness;
}

void CRoomRightDlg::OnUpdateMoney( uint32 lMoney , BOOL bFirstInit ) 			//更新钱币
{	
	if (CFlashWnd::IsSupportFlash() && m_pCoinFlashWnd != NULL)
	{		
		if ( lMoney <= MONEY_MAX )
		{
			uint32 addMoney = 0 ;
			if ( !bFirstInit && lMoney>m_unLastMoney )
			{
				addMoney = lMoney - m_unLastMoney ;
			}				
			m_unLastMoney = lMoney ;
			PlayCoinEffect( lMoney, addMoney );
		}
		else
		{
			PlayCoinEffect(0, 0 );// 钱超过40亿的为负数，就显示0
			m_unLastMoney = 0 ;
		}		
	}
	else
	{
		CString strMoney = L"0";
		if ( lMoney <= MONEY_MAX )  // 钱超过40亿的为负数，就显示0
		{
			strMoney.Format( L"%u" ,lMoney ) ;
		}
		CComVariant var = strMoney ;
		if(m_pSkinEngine != NULL)
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textHowMuch",L"text",var);
	}
}

void CRoomRightDlg::SetSendObj( CString strName , uint32 unUIN ) 
{
	int nIndex = m_GiftSendToCombo.FindString(0, strName);
	if (nIndex != CB_ERR)
	{
		m_GiftSendToCombo.DeleteString( nIndex ) ;
	}

	nIndex = m_GiftSendToCombo.InsertString( 0 , strName);
	if (nIndex >= 0)
	{
		m_GiftSendToCombo.SetItemData(nIndex, unUIN);
		m_GiftSendToCombo.SetCurSel(nIndex);
		if ( m_GiftSendToCombo.GetCount() > SENDTO_COMBOX_MAX )
		{
			m_GiftSendToCombo.DeleteString( m_GiftSendToCombo.GetCount() -1 ) ;
		}
	}
}

void CRoomRightDlg::SetSpeakerOutputGotoEndAble( BOOL bAble ) 
{
	if ( bAble )
	{
		m_pSpeakerHtml->SetGotoEndAble() ;
	}
	else
	{
		m_pSpeakerHtml->SetUnGotoEnd() ;
	}
}

void CRoomRightDlg::InsertSpeakerText(CString& strText )
{
	m_pSpeakerHtml->InsertText( strText ) ;
}

void CRoomRightDlg::OnClickNameLink( CString& strName ,uint32 unUUID )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( unUUID == myCurInfo.unUIN )
	{
		return ;
	}
	SetSendObj( strName , unUUID ) ;
}

void CRoomRightDlg::OnPaodaoSendToChange( CString& strSelText )
{
	int nIndex = m_GiftSendToCombo.FindString( 0 , strSelText ) ;
	if ( nIndex != CB_ERR )
	{
		m_GiftSendToCombo.SetCurSel( nIndex ) ;
	}
}

int CRoomRightDlg::GetSenToComboInfo(vector<SenToComboInfo>& vSenToComboInfoList )
{
	for (int i = 0; i < m_GiftSendToCombo.GetCount(); ++i)
	{
		SenToComboInfo sendtocomboinfo ;
		m_GiftSendToCombo.GetLBText( i , sendtocomboinfo.strName ) ;
		sendtocomboinfo.unUIN = m_GiftSendToCombo.GetItemData( i ) ;
		vSenToComboInfoList.push_back( sendtocomboinfo ) ;
	}
	return m_GiftSendToCombo.GetCurSel() ;
}

UserManager * CRoomRightDlg::GetUserManager()
{
	if (m_pRoomBusiness != NULL)
	{
		return m_pRoomBusiness->GetUserManager();
	}

	return NULL;
}

IUserItem * CRoomRightDlg::GetSelfUserItem()
{
	if (GetUserManager() != NULL)
	{
		return GetUserManager()->GetSelfUserItem();
	}

	return NULL;
}

int ROOM_GIFTLIST_ID[5] = {IDC_ROOM_GIFTLIST1,IDC_ROOM_GIFTLIST2,IDC_ROOM_GIFTLIST3,IDC_ROOM_GIFTLIST4,IDC_ROOM_GIFTLIST5};

void CRoomRightDlg::InitGiftListCtrl()
{
	int nCount = 0;
	for (std::vector<std::wstring>::iterator it = GiftManager::GetInstance()->GetGiftCatalogList().begin(); it != GiftManager::GetInstance()->GetGiftCatalogList().end(); ++it)
	{

		CGiftListCtrl * pListCtrl = new CGiftListCtrl(*it);

		m_mapGiftListCtrl[*it] = pListCtrl;
		pListCtrl->SubclassDlgItem(ROOM_GIFTLIST_ID[nCount++], this);
		pListCtrl->InitGiftListCtrl();
	}

	ShowGiftListCtrlByTab(0);

	/*CComVariant vars = 0;
	if(m_pSkinEngine != NULL)
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabStamp",L"tabItemSelected",vars); */
}

void CRoomRightDlg::ShowGiftListCtrlByTab( int nTab )
{
	if (nTab < 0)
	{
		return;
	}
	ASSERT(nTab < GiftManager::GetInstance()->GetGiftCatalogList().size());
	//if (nTab >= 0)
	//{
	//	int nCurSel = 0;
	//	CComVariant var;
	//	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(),L"tabGift",L"tabItemSelected",&var);
	//	nCurSel = var.intVal;
	//	if (nTab != nCurSel)
	//	{
	//		CComVariant vars = nTab;
	//		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabGift",L"tabItemSelected",vars);
	//	}
	//}
	if ( GiftManager::GetInstance()->GetGiftCatalogList().size() <= 0  )
	{
		return ;
	}
	std::wstring strTabName = GiftManager::GetInstance()->GetGiftCatalogList()[nTab];
	if (strTabName != L"")
	{
		for (std::map<std::wstring, CGiftListCtrl*>::iterator it = m_mapGiftListCtrl.begin(); it != m_mapGiftListCtrl.end(); ++it)
		{
			if (strTabName == it->first)
			{
				it->second->ShowWindow(SW_SHOW);
			}
			else
			{
				it->second->ShowWindow(SW_HIDE);
			}
		}
	}
}

LRESULT CRoomRightDlg::OnClickUserList( WPARAM wParam, LPARAM lParam )
{
	IUserItem * pUserItem = (IUserItem*)lParam;
	ASSERT(pUserItem != NULL);
	m_pParentDlg->SendMessage(WM_CLICK_USERLIST, wParam , lParam);

	core::CurInfo myCurInfo = GetCurInfo() ;
	if (pUserItem->GetUserUIN() == myCurInfo.unUIN)
	{
		return S_OK;
	}
	SetSendObj( pUserItem->GetUserName().c_str() , pUserItem->GetUserUIN() ) ;
	return S_OK;
}

LRESULT CRoomRightDlg::OnClickGiftList( WPARAM wParam, LPARAM lParam )
{
	IGiftItem * pGiftItem = (IGiftItem*)lParam;
	ASSERT(pGiftItem != NULL);

	if (GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE)
	{
		if (pGiftItem->GetSendType() == room::SENDTYPE_NONE || pGiftItem->GetSendType() == room::SENDTYPE_SELF)
		{
			C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能使用道具，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
				m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		}
		else
		{
			C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能赠送礼物，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
				m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		}

		return S_OK;
	}
	ASSERT(pGiftItem != NULL);

	if (pGiftItem->GetName() == L"印章")
	{
		OnClickStamp();
		return S_OK;
	}
	else if (pGiftItem->GetName() == L"喇叭")
	{
		OnClickSpeaker();
		return S_OK;
	}
	else if (pGiftItem->GetName() == L"浪漫点歌")
	{
		OnClickWishSong();
		return S_OK;
	}
	else if (pGiftItem->GetName() == L"变声卡")
	{
		OnClickChangeVoice();
		return S_OK;
	}
	else if (pGiftItem->GetName() == L"飞屏")
	{
		OnClickFlyingText();
		return S_OK;
	}

	int nCount = 1;
	if (pGiftItem->GetSendType() == room::SENDTYPE_OTHERSINGLE)
	{
		CString sCount;
		m_GiftCountCombo.GetEditText(sCount);
		//int nSel = m_GiftCountCombo.GetCurSel();
		//if (nSel < 0)
		//{
		//	m_GiftCountCombo.GetEditText(sCount);
		//}
		//else
		//{
		//	m_GiftCountCombo.GetLBText(nSel, sCount);
		//}

		int nSel = m_GiftSendToCombo.GetCurSel();
		if (nSel < 0)
		{
			//提示选择赠送对象
			m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】： " ,L"请选择赠送对象" , COLOR_OPERATE );
			return S_OK;
		}
		std::wstring strCount = sCount.GetBuffer(0);
		if (!common::utility::stringhelper::StringToInt(strCount, &nCount))
		{
			//提示只能输入数字
			m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】： " ,L"请重新输入，只能输入数字" , COLOR_OPERATE );
			return S_OK;
		}
		if (nCount <= 0)
		{
			//提示只能输入数字
			m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】： " ,L"请重新输入，只能输入数字" , COLOR_OPERATE );
			return S_OK;
		}
		uint32 unUIN = m_GiftSendToCombo.GetItemData(nSel);

		IUserItem * pUserItem = NULL;
		if (GetUserManager() != NULL)
		{
			pUserItem = GetUserManager()->SearchUserByUIN(unUIN);
		}
		if (pUserItem != NULL)
		{
			//m_GiftCountCombo.SetWindowText(L"1");
			if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_NONE)
			{
				if ( pGiftItem->GetGiftType() == core::GIFTTYPE_NORMAL || pGiftItem->GetGiftType() == core::GIFTTYPE_SHECHI )
				{
					m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】： " ,L"不能赠送礼物给游客" , COLOR_OPERATE );
				}
				else
				{
					m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】： " ,L"不能赠送道具给游客" , COLOR_OPERATE );
				}
				return S_OK;
			}
			bool bLuxury = (pGiftItem->GetGiftType() == core::GIFTTYPE_SHECHI);
			if ( bLuxury && pGiftItem->IsNeedGiftWords() )
			{
				m_pParentDlg->OnClickNeedGiftWord( pGiftItem , pUserItem , nCount ) ;
			}
			else
			{
				CSendEventManager::SendNormalGiftEvent(m_pParentDlg->GetRoomID(), pUserItem->GetUserUIN(), pGiftItem->GetID(), nCount, pGiftItem->GetDstType(),L"", bLuxury);
			}			


			return S_OK;
		}
	}

	if (pGiftItem->GetSendType() == room::SENDTYPE_OTHERALL)
	{
		//m_GiftCountCombo.SetWindowText(L"1");
		bool bLuxury = (pGiftItem->GetGiftType() == core::GIFTTYPE_SHECHI);
		if ( bLuxury && pGiftItem->IsNeedGiftWords() )
		{
			m_pParentDlg->OnClickNeedGiftWord( pGiftItem , NULL , nCount ) ;
		}
		else
		{
			CSendEventManager::SendNormalGiftEvent(m_pParentDlg->GetRoomID(), 0, pGiftItem->GetID(), 1, pGiftItem->GetDstType(),L"", bLuxury);
		}		
		return S_OK;
	}

	if (pGiftItem->GetSendType() == room::SENDTYPE_SELF)
	{
		//m_GiftCountCombo.SetWindowText(L"1");
		CSendEventManager::SendNormalGiftEvent(m_pParentDlg->GetRoomID(), GetCurInfo().unUIN, pGiftItem->GetID(), 1 ,pGiftItem->GetDstType(), L"");

	}
	return S_OK;
}

void CRoomRightDlg::OnClickStamp()
{
	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_STAMP, 0, 0);
	}
}

void CRoomRightDlg::OnClickPresentDetail()
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_GUEST_OPERATE, L"游客不能查看赠收详情，", L"想要畅玩白兔KTV？", L"", L"快去注册吧", m_pRoomBusiness->GetRegisteUrl().GetBuffer());
		return ;
	}

	SwitchResourceToModule(_T("Room"));
	if (m_pPresentDetailDlg == NULL)
	{
		m_pPresentDetailDlg = new CPresentDetailDlg();
		if (m_pPresentDetailDlg)
		{
			m_pPresentDetailDlg->Create(CPresentDetailDlg::IDD,NULL);
			m_pPresentDetailDlg->SetWindowPos(NULL,0,0,425,329,SWP_SHOWWINDOW);
			m_pPresentDetailDlg->CenterWindow();
			m_pPresentDetailDlg->ShowWindow(SW_SHOW);
			return ;
		}
	}

	if (m_pPresentDetailDlg != NULL && m_pPresentDetailDlg->GetSafeHwnd())
	{
		m_pPresentDetailDlg->ShowWindow(SW_SHOW);
		m_pPresentDetailDlg->SetForegroundWindow();
	}
}
//#include "FlyingTextInputDlg.h"
void CRoomRightDlg::OnClickSpeaker()
{
	//SwitchResourceToModule(_T("Room"));
	//CFlyingTextInputDlg * pDlg = new CFlyingTextInputDlg;
	//pDlg->Create(CFlyingTextInputDlg::IDD, NULL);
	//pDlg->ShowWindow(SW_SHOW);
	//return;

	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_SPEAKER, 0, 0);
	}
}

void CRoomRightDlg::OnClickWishSong()
{
	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_WISHSONG, 0, 0);
	}
}

bool CRoomRightDlg::GetSendToUIN( uint32 & unUIN )
{
	int nSel = m_GiftSendToCombo.GetCurSel();
	if (nSel == CB_ERR)
	{
		return false;
	}

	CString sDstName;
	m_GiftSendToCombo.GetLBText(nSel,sDstName);
	if (sDstName.IsEmpty())
	{
		return false;
	}

	unUIN = m_GiftSendToCombo.GetItemData(nSel);
	return true;
}

LRESULT CRoomRightDlg::OnShowGiftTip( WPARAM wParam, LPARAM lParam )
{
	//IGiftItem * pGiftItem = (IGiftItem*)lParam;
	//ASSERT(pGiftItem != NULL);

	//SwitchResourceToModule(_T("Room"));
	//if (m_pGiftTipDlg == NULL)
	//{
	//	m_pGiftTipDlg = new CGiftTipDlg;
	//	m_pGiftTipDlg->Create(CGiftTipDlg::IDD, this);
	//}

	//m_pGiftTipDlg->Show(pGiftItem);

	return S_OK;
}

void CRoomRightDlg::OnClickChangeVoice()
{
	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_CHANGEVOICE, 0, 0);
	}
}

void CRoomRightDlg::OnClickFlyingText()
{
	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_FLYINGTEXT, 0, 0);
	}
}

void CRoomRightDlg::OnClickBtnPayVIP()
{
	//CString strUrl;
	//strUrl.Format(L"%s&room=%d", m_pParentDlg->GetRoomBusiness()->GetRoomPayUrl(), m_pParentDlg->GetRoomID());
	common::utility::systemhelper::OpenUrl( m_pParentDlg->GetRoomBusiness()->GetRoomPayUrl().GetBuffer() ) ;
}

void CRoomRightDlg::OnCbnSelchangeComboRoomGiveto()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSelText ;
	m_GiftSendToCombo.GetWindowText( strSelText ) ;
	m_pParentDlg->OnRoomRightSendToChange( strSelText ) ;
}

void CRoomRightDlg::ShowCoinFlashWnd()
{
	if (CFlashWnd::IsSupportFlash())
	{
		if (m_pCoinFlashWnd != NULL)
		{
			CRect rcTemp(0,0,0,0), rcCoinFlash(0,0,0,0);
			m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgMoneyBk", &(rcTemp.left), &(rcTemp.top), &(rcTemp.right), &(rcTemp.bottom));
			ClientToScreen(&rcTemp);
			flash_info* pInfo = ScreenEffectManager::GetFlashInfo(EFFECT_TYPE_ADD_MONEY);
			rcCoinFlash.left = rcTemp.left + 20;
			rcCoinFlash.bottom = rcTemp.bottom;
			rcCoinFlash.right = rcCoinFlash.left + pInfo->nWidth;
			rcCoinFlash.top = rcCoinFlash.bottom - pInfo->nHeight;
			m_pCoinFlashWnd->MoveWindow(&rcCoinFlash, TRUE);
		}
	}
}


void CRoomRightDlg::PlayCoinEffect(uint32 totalMoney, uint32 addMoney)
{
	//flash提供两个接口: changeCoin(_totalCoin:String)用于显示总钱数；addCoin(_addCoin:String,_totalCoin:String)用于显示加币数量和总钱数
	if (CFlashWnd::IsSupportFlash())
	{
		if (m_pCoinFlashWnd != NULL)
		{
			if (addMoney > 0)//说明有加币效果
			{
				CString strTemp;
				std::vector<CString> vFuncParams;
				strTemp.Format(L"%u", addMoney);
				vFuncParams.push_back(strTemp);
				strTemp.Format(L"%u", totalMoney);
				vFuncParams.push_back(strTemp);
				m_pCoinFlashWnd->CallFunction(L"addCoin", vFuncParams);
			}else
			{
				CString strTemp;
				strTemp.Format(L"%u", totalMoney);
				std::vector<CString> vFuncParams;
				vFuncParams.push_back(strTemp);
				m_pCoinFlashWnd->CallFunction(L"changeCoin", vFuncParams);
			}
		}
	}
}

void CRoomRightDlg::GetGiftItemRect(CRect &rcGiftItem)
{
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"HotGiftList", &(rcGiftItem.left), &(rcGiftItem.top), &(rcGiftItem.right), &(rcGiftItem.bottom));
	rcGiftItem.left += 55;
	//rcGiftItem.top += 0;
	rcGiftItem.right = rcGiftItem.left + 62;
	rcGiftItem.bottom = rcGiftItem.top + 55;
	ClientToScreen(&rcGiftItem);
}

//
//void CRoomRightDlg::DrawGifButton()
//{
//	SwitchResourceToModule(_T("Room"));
//	m_pGifBtnPayMoney = new common::ui::GifButton();
//	std::wstring strAppPath = common::utility::systemhelper::Get179AppPath();
//	CString strTemp = L"", strTips = L"";
//	if (GetCurInfo().enmVIPLevel == VIPLEVEL_NONE || GetCurInfo().enmVIPLevel == VIPLEVEL_REGISTER)//游客、注册用户
//	{
//		strTemp.Format(L"%sresource\\other\\Room_btnPayMoneyForVIP.gif", strAppPath.c_str());
//		strTips = L"充值、购买VIP";
//	}else if (GetCurInfo().enmVIPLevel == VIPLEVEL_TOP)//至尊
//	{
//		strTemp.Format(L"%sresource\\other\\Room_btnPayMoneyForAntiRelegation.gif", strAppPath.c_str());
//		strTips = L"充值、保级";
//	}else
//	{
//		strTemp.Format(L"%sresource\\other\\Room_btnPayMoneyForLevelUp.gif", strAppPath.c_str());
//		strTips = L"充值、升级皇冠";
//	}
//
//	if (m_pGifBtnPayMoney->CreateEx(this, strTemp))
//	{
//		//m_pGifBtnPayMoney->ShowGifBtn(rcClient);
//		MoveGifButton(TRUE);
//		m_pGifBtnPayMoney->SetBtnHandCursor(TRUE);
//		m_pGifBtnPayMoney->SetGifBtnTipText(strTips);
//	}
//}
//
//void CRoomRightDlg::MoveGifButton(BOOL bShow)
//{
//	if (m_pGifBtnPayMoney != NULL && m_pGifBtnPayMoney->GetSafeHwnd() != NULL)
//	{
//		if (!bShow)
//		{
//			m_pGifBtnPayMoney->ShowWindow(SW_HIDE);
//		}else
//		{
//			CRect rc;
//			m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"btnPayVIP", &(rc.left), &(rc.top), &(rc.right), &(rc.bottom));
//			ClientToScreen(&rc);
//			m_pGifBtnPayMoney->ShowGifBtn(rc);
//		}
//	}
//}
//
//void CRoomRightDlg::MoveThePayMoneyBtn(BOOL bShow)
//{
//	CComVariant var;
//	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"btnPayVIP", L"visible", &var);
//	if (!bShow)
//	{
//		var = FALSE;
//	}
//	MoveGifButton(var.boolVal);
//}
//

//LRESULT CRoomRightDlg::OnClickGifButton(WPARAM wParam, LPARAM lParam)
//{
//	if ((CWnd*)wParam == m_pGifBtnPayMoney)
//	{
//		if (m_pGifBtnPayMoney->GetSafeHwnd() != NULL)
//		{
//			OnClickBtnPayVIP();
//		}
//	}
//
//	return S_OK;
//}

void CRoomRightDlg::InitCoinFlashWnd()
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pCoinFlashWnd == NULL)
	{
		m_pCoinFlashWnd = new CFlashWnd;
		flash_info* pInfo = ScreenEffectManager::GetFlashInfo(EFFECT_TYPE_ADD_MONEY);
		if (!m_pCoinFlashWnd->Play(pInfo->nWidth, pInfo->nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), this))
		{
			SetFlashVersion(m_pCoinFlashWnd->GetVersion());
			SAFE_DELETE_DIALOG(m_pCoinFlashWnd);
			return;
		}
		SetFlashVersion(m_pCoinFlashWnd->GetVersion());
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"imgCoin",L"visible", (CComVariant)FALSE);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textCoin",L"visible", (CComVariant)FALSE);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textHowMuch",L"visible", (CComVariant)FALSE);
		m_pCoinFlashWnd->ShowWindow(SW_SHOWNOACTIVATE);
		ShowCoinFlashWnd();
		PlayCoinEffect(0, 0);//初始化为0
	}
}

void CRoomRightDlg::ReloadGiftData()
{
	for (std::map<std::wstring, CGiftListCtrl*>::iterator it = m_mapGiftListCtrl.begin(); it != m_mapGiftListCtrl.end(); ++it)
	{
		CGiftListCtrl * pListCtrl = it->second;
		if (pListCtrl != NULL)
		{
			pListCtrl->ReloadListData();
		}
	}
}

void CRoomRightDlg::OnClickBtnGift()
{
	m_MyGoodsListCtrl.ShowWindow(SW_HIDE);
	ShowGiftListCtrlByTab(0);
}

void CRoomRightDlg::OnClickBtnGame()
{
	
}

void CRoomRightDlg::OnClickBtnMyGoods()
{
	for (std::map<std::wstring, CGiftListCtrl*>::iterator it = m_mapGiftListCtrl.begin(); it != m_mapGiftListCtrl.end(); ++it)
	{
		it->second->ShowWindow(SW_HIDE);
	}

	m_MyGoodsListCtrl.ShowWindow(SW_SHOW);
}

LRESULT CRoomRightDlg::OnClickMyGoodsList( WPARAM wParam, LPARAM lParam )
{
	CMyGoodsItem * pMyGoodsItem = (CMyGoodsItem*)lParam;
	if (pMyGoodsItem != NULL)
	{
		IGiftItem * pGiftItem = pMyGoodsItem->GetGiftItem();
		//OnClickGiftList(0, (LPARAM)pGiftItem);
	}

	return S_OK;
}

void CRoomRightDlg::OnClickBtnBaiTuService()
{
	common::utility::systemhelper::OpenUrl( m_pParentDlg->GetRoomBusiness()->GetServiceUrl().GetBuffer()) ;
}

LRESULT CRoomRightDlg::OnClickMsgBoxGoToRegister( WPARAM wParam, LPARAM lParam )
{
	if ((CRoomRightDlg*)wParam == this)
	{
		m_pParentDlg->OnClickGotoRegister();
	}

	return TRUE;
}

void CRoomRightDlg::CleanSpeakerOutput()
{
	m_pSpeakerHtml->ClearMsg();
}