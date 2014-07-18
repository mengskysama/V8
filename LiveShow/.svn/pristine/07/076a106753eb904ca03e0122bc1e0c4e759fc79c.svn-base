// ScrollBtnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PaodaoDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\RoomParentDlg.h"
#include "..\gift/GiftManager.h"
#include "..\gift/GiftListCtrl.h"
#include "..\SendEventManager.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"

#define  SKIN_ON_CLICK_CLOSE_DLG        1
// CPaodaoDlg 对话框

IMPLEMENT_DYNAMIC(CPaodaoDlg, CDialog)

CPaodaoDlg::CPaodaoDlg( CRoomParentDlg* pParentDlg  , CWnd* pParent /*=NULL*/)
: CDialog(CPaodaoDlg::IDD, pParent),
m_pParentDlg( pParentDlg )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_pListCtrl = NULL ;
}

CPaodaoDlg::~CPaodaoDlg()
{
	if ( m_pListCtrl != NULL )
	{
		delete m_pListCtrl ;
		m_pListCtrl = NULL ;
	}
}

void CPaodaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AMMOUNT, m_GiftCountCombo);
	DDX_Control(pDX, IDC_COMBO_GIVETO, m_GiftSendToCombo);
}


BEGIN_MESSAGE_MAP(CPaodaoDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_CLICK_GIFTLIST, OnClickGiftList)
	ON_CBN_SELCHANGE(IDC_COMBO_GIVETO, &CPaodaoDlg::OnCbnSelchangeComboGiveto)
	ON_MESSAGE(COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER, &CPaodaoDlg::OnClickMsgBoxGoToRegister)
END_MESSAGE_MAP()


// CScrollBtnDlg 消息处理程序

BOOL CPaodaoDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CPaodaoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitLuxuryList() ;
	InitSendToCombo() ;
	//礼物数量框初始化
	m_GiftCountCombo.InitGiftCombo();
	return TRUE ;
}


int CPaodaoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"PaodaoDlg");
		ASSERT(hr == S_OK && __LINE__);
	}
	ModifyStyleEx( WS_EX_APPWINDOW , 0 ) ;
	ModifyStyleEx( WS_EX_TOOLWINDOW , 0 ) ;
	return 0;
}

LRESULT CPaodaoDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE_DLG:
		{
			ShowWindow( SW_HIDE ) ;
		}
		break;
	default:break;
	}

	return TRUE;
}

void CPaodaoDlg::InitLuxuryList()
{
	for (std::vector<std::wstring>::iterator it = GiftManager::GetInstance()->GetGiftCatalogList().begin(); it != GiftManager::GetInstance()->GetGiftCatalogList().end(); ++it)
	{
		if ( *it == L"奢侈" )
		{
			CComVariant vars[1];
			vars[0] = -1;
			DISPPARAMS dispParams = {vars, 0, 1, 0};
			m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(),L"tabGift",L"AddTabItem",NULL,&dispParams);

			CComVariant vars2[2];
			vars2[0] = CString((*it).c_str());
			vars2[1] = 0;
			DISPPARAMS dispParams2 = {vars2, 0, 2, 0};
			m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(),L"tabGift",L"SetTabItemText",NULL,&dispParams2);
			m_pListCtrl = new CGiftListCtrl(*it);
			m_pListCtrl->SubclassDlgItem( IDC_LIST_LUXURY , this);
			m_pListCtrl->InitGiftListCtrl();
			break;
		}
	}
}

void CPaodaoDlg::InitSendToCombo() 
{
	vector<SenToComboInfo> vSenToComboInfoList ;
	int nIndex  = m_pParentDlg->GetSenToComboInfo( vSenToComboInfoList ) ;
	vector<SenToComboInfo>::iterator iter ;
	for ( iter = vSenToComboInfoList.begin() ; iter != vSenToComboInfoList.end() ; ++iter )
	{
		int nIndex = m_GiftSendToCombo.AddString( iter->strName ) ;
		if ( nIndex != CB_ERR )
		{
			m_GiftSendToCombo.SetItemData( nIndex ,iter->unUIN ) ;
		}
	}
	if ( nIndex != CB_ERR )
	{
		m_GiftSendToCombo.SetCurSel( nIndex ) ;
	}
}

void CPaodaoDlg::OnClickNameLink( CString& strName ,uint32 unUUID )
{
	SetSendObj( strName , unUUID ) ;
}

void CPaodaoDlg::SetSendObj( CString strName , uint32 unUIN ) 
{
	int nIndex = m_GiftSendToCombo.FindString( 0 , strName ) ;
	if ( nIndex != CB_ERR )
	{
		m_GiftSendToCombo.DeleteString( nIndex ) ;
	}
	nIndex = m_GiftSendToCombo.InsertString( 0 , strName );
	if (nIndex >= 0)
	{
		m_GiftSendToCombo.SetItemData(nIndex, unUIN );
		m_GiftSendToCombo.SetCurSel(nIndex);
		if ( m_GiftSendToCombo.GetCount() > SENDTO_COMBOX_MAX )
		{
			m_GiftSendToCombo.DeleteString( m_GiftSendToCombo.GetCount() -1 ) ;
		}
	}
}

void CPaodaoDlg::OnDeleteUser(IUserItem * pUserItem)
{
	int nIndex = m_GiftSendToCombo.FindString( 0, pUserItem->GetUserName().c_str());
	if (nIndex != CB_ERR)
	{
		m_GiftSendToCombo.DeleteString(nIndex);
		m_GiftSendToCombo.Invalidate(TRUE);
	}
}

void CPaodaoDlg::OnRoomRightSendToChange( CString& strSelText )
{
	int nIndex = m_GiftSendToCombo.FindString( 0 , strSelText ) ;
	if ( nIndex != CB_ERR )
	{
		m_GiftSendToCombo.SetCurSel( nIndex ) ;
	}
}

LRESULT CPaodaoDlg::OnClickGiftList( WPARAM wParam, LPARAM lParam )
{
	UpdateData( TRUE ) ;
	IGiftItem * pGiftItem = (IGiftItem*)lParam;
	ASSERT(pGiftItem != NULL);

	if (GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE)
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能赠送礼物，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return S_OK;
	}
	ASSERT(pGiftItem != NULL);

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
			m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】： " ,L"请重新输入，只能输入数字" , COLOR_OPERATE);
			return S_OK;
		}
		uint32 unUIN = m_GiftSendToCombo.GetItemData(nSel);

		IUserItem * pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unUIN );
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
					m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(L"【提示】：" ,L"不能赠送道具给游客" , COLOR_OPERATE );
				}				
				return S_OK;
			}
			bool bLuxury = (pGiftItem->GetGiftType() == core::GIFTTYPE_SHECHI);
			if ( bLuxury && pGiftItem->IsNeedGiftWords() )
			{
				m_pParentDlg->OnClickNeedGiftWord( pGiftItem ,pUserItem , nCount ) ;
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
		CSendEventManager::SendNormalGiftEvent(m_pParentDlg->GetRoomID(), GetCurInfo().unUIN, pGiftItem->GetID(), 1, pGiftItem->GetDstType() , L"" );
	}
	return S_OK;
}
void CPaodaoDlg::OnCbnSelchangeComboGiveto()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSelText ;
	m_GiftSendToCombo.GetWindowText( strSelText ) ;
	m_pParentDlg->OnPaodaoSendToChange( strSelText ) ;
}

void CPaodaoDlg::UpdateGiftList()
{
	m_pListCtrl->ReloadListData();
}

LRESULT CPaodaoDlg::OnClickMsgBoxGoToRegister( WPARAM wParam, LPARAM lParam )
{
	if((CPaodaoDlg*)wParam == this)
	{
		m_pParentDlg->OnClickGotoRegister();
	}

	return TRUE;
}