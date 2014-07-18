// RoomMidDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Room.h"
#include "RoomMidDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "RoomParentDlg.h"
#include "RoomModule.h"
#include "UserMessageDefine.h"
#include "shortcut\SelfDefSentenceDlg.h"
#include "userlist\UserListMenu.h"
#include "roomsetting\RoomOption.h"
#include "SendEventManager.h"
#include "GlobalRoomDefine.h"
#include "gift\ChangeVoiceItem.h"
#include "gift\GiftManager.h"
#include "autoreply\AutoReplyConfig.h"
#include "screeneffect\ScreenEffectManager.h"
//#include "SysSetting/SysSettingDlg.h"

#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\ui\UIHelper.h"
#include "..\CommonLibrary\include\ui\C179MsgBox.h"
#include "..\CommonLibrary/include\utility\ConfigManager.h"
#include "autoreply\AutoReplySettingDlg.h"

#include "personalsetting/PersonalConfig.h"
#include "utility\TheMoreDlg.h"

#define SKIN_ON_SPLITTER_MOVE            1
#define SKIN_ON_CLICK_BTN_EMOTION        2
#define SKIN_ON_CLICK_BTN_COLORWORD      3
#define SKIN_ON_CLICK_BTN_CLEANSCREEN    4
#define SKIN_ON_CLICK_BTN_STAMP          5
#define SKIN_ON_CLICK_BTN_SHORTCUT       6
#define SKIN_ON_CLICK_BTN_COMPLAINT      7
#define SKIN_ON_CLICK_JIACU              9
#define SKIN_ON_CLICK_XIETI              10
#define SKIN_ON_CLICK_UNDERLINE          11
#define SKIN_ON_CLICK_FONT_COLOR         12
#define SKIN_ON_CLICK_BTN_SENDMSG        13
#define SKIN_ON_CLICK_BTN_AUTOREPLY      14
#define SKIN_ON_CLICK_BTN_COPYURL        15
#define SKIN_ON_CLICK_BTN_FLYTEXT		 16
#define SKIN_ON_CLICK_BTN_SENDFLYTEXT	 17
#define SKIN_ON_CLICK_BTN_FLYTEXTEFFECT	 18

#define SKIN_ON_CLICK_BTN_FONT              21
#define SKIN_NOTIFY_SHOW_FONT               22
#define SKIN_ON_CLICK_BTN_MORE              23

#define SPLIT_1	L"&spor;"
#define SPLIT_2	L"&spco;"

#define DEFAULT_ROOMMID_INPUT_TEXT           L"勾选【私聊】就能和对方说悄悄话哦！"
// CRoomMidDlg 对话框

IMPLEMENT_DYNAMIC(CRoomMidDlg, CDialog)

CRoomMidDlg::CRoomMidDlg(CRoomParentDlg* pParentDlg ,CWnd* pParent /*=NULL*/)
	: CDialog(CRoomMidDlg::IDD, pParent),
	m_pParentDlg( pParentDlg ) ,
	m_InputLogic(pParentDlg , HTML_TYPE_INPUT ),
	m_PublicLogic(pParentDlg , HTML_TYPE_PUBLIC ),
	m_PrivateLogic(pParentDlg , HTML_TYPE_PRIVATE ),
	m_SendMsgTime(0)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_bShortCutMsgMenu = m_bCaiZiMenu = TRUE ;
	m_nLastCaiZiTime = m_nRepeatCount = 0 ;
	//m_bAutoReplyMenu = TRUE;
	//DragonZ:跑道移到RoomParentDlg
	//m_pPaodaoStatic = NULL ;

	m_nFlyTextType = 0;
	m_pTheMoreDlg = NULL;
}

CRoomMidDlg::~CRoomMidDlg()
{
	//DragonZ:跑道移到RoomParentDlg
	//SAFE_DELETE_DIALOG(m_pPaodaoStatic) ;

	SAFE_DELETE_DIALOG(m_pTheMoreDlg);
}

void CRoomMidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROOM_TALKTO, m_ComboTalkTo);
}


BEGIN_MESSAGE_MAP(CRoomMidDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_USER_SPLITTER,OnDragSplit)
	ON_MESSAGE(WM_CLICK_EMOTION , OnClickEmotionList)
	ON_MESSAGE(WM_CLICK_CAIZI , OnClickCaiZi)
	ON_MESSAGE(WM_CLICK_SHORTCUT, OnClickShortCutMsg)
	//ON_CBN_SELCHANGE(IDC_COMBO_ROOM_FONTSIZE, &CRoomMidDlg::OnCbnSelchangeComboFontsize)
	//ON_CBN_SELCHANGE(IDC_COMBO_ROOM_FONT, &CRoomMidDlg::OnCbnSelchangeComboFont)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_ROOM_PILLOWCHAT, &CRoomMidDlg::OnBnClickedCheckRoomPillowchat)
	ON_BN_CLICKED(IDC_CHECK_ROOM_FLYTEXT, &CRoomMidDlg::OnBnClickedCheckRoomFlytext)
	ON_MESSAGE(WM_OUTPUTHTMLEDITIMPLM_FOCUS_IN, &CRoomMidDlg::OnInputMsgFocusIn)
	ON_MESSAGE(WM_OUTPUTHTMLEDITIMPLM_FOCUS_OUT, &CRoomMidDlg::OnInputMsgFocusOut)
	ON_MESSAGE(COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER, &CRoomMidDlg::OnClickMsgBoxGoToRegister)
END_MESSAGE_MAP()


// CRoomMidDlg 消息处理程序

int CRoomMidDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RoomMidDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CRoomMidDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//创建跑道、公屏、私屏的IE控件

	//跑道
	//DragonZ:跑道移到RoomParentDlg
	/*m_pPaodaoStatic = new CPaodaoStaitc( m_pParentDlg ) ;
	m_pPaodaoStatic->Create( NULL , WS_CHILD|WS_VISIBLE , CRect(0,0,0,0) , this ,IDC_HTML_ROOM_PAODAO ) ;
	CComVariant var = (LONG)m_pPaodaoStatic->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() ,L"itemHtmlPaoDao" , L"hwnd" , var ) ;*/

	//公聊  
	m_pPublicHtml = new COutputHtmlEditImpl(ROOM_OUTPUTCHAT_PUBLIC_MAX_HOLD_LINE , HTML_TYPE_PUBLIC ) ;
	m_pPublicHtml->SetHtmlEditLogic( &m_PublicLogic ) ;
	m_pPublicHtml->Create( this , IDC_HTML_ROOM_PUBLICCHAT , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE|WS_HSCROLL ) ;
	CComVariant var = (LONG)m_pPublicHtml->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd() , L"itemHtmlPublicChat",L"hwnd",var);

	//私聊
	m_pPrivateHtml = new COutputHtmlEditImpl(ROOM_OUTPUTCHAT_PRIVATE_MAX_HOLD_LINE , HTML_TYPE_PRIVATE ) ;
	m_pPrivateHtml->SetHtmlEditLogic( &m_PrivateLogic ) ;
	m_pPrivateHtml->Create( this ,IDC_HTML_ROOM_PRIVATECHAT , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE|WS_HSCROLL ) ;
	var = (long)m_pPrivateHtml->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlPrivateChat",L"hwnd",var);
	//m_InputLogic.SetFontSize( 14 ) ;

	m_pParentDlg->SetPublicOutputGotoEndAble( CPersonalConfig::GetInstance()->GetPublicAutoScroll() ) ;
	m_pParentDlg->SetPrivateOutputGotoEndAble( CPersonalConfig::GetInstance()->GetPrivateAutoScroll() ) ;

	//输入
	m_pInputHtml = new COutputHtmlEditImpl(ROOM_OUTPUTCHAT_PUBLIC_MAX_HOLD_LINE , HTML_TYPE_INPUT ) ;
	m_pInputHtml->SetHtmlEditLogic( &m_InputLogic ) ;
	m_pInputHtml->Create( this ,IDC_HTML_ROOM_MSGINPUT , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE) ;
	var = (long)m_pInputHtml->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlInputMsg",L"hwnd",var);

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->AddString( L"所有人" ) ;
	if ( nIndex != CB_ERR )
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->SetCurSel( nIndex ) ;
		((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->SetItemData( nIndex , 0 ) ;
	}
	InitFontPanel() ;

	//创建分割条
	//最后一个参数为TRUE时则是上下方向的分割栏
	if(!m_Splitter.Create(WS_CHILD | WS_VISIBLE, _T(""), CRect(0, 0, 0, 0), this, 30000, TRUE))
		return -1;
	m_Splitter.SetLimit(250,GetSystemMetrics(SM_CYSCREEN)-400);

	CreateAutoWelcomeTimer() ;

	if (GetCurInfo().bFirstLogin)
	{
		CreateFirstLoginAutoWelcomeTimer();
	}else
	{
		CreateAutoWelcomeTimer() ;
	}
	
	if ( GetCurInfo().enmVIPLevel == core::VIPLEVEL_NONE )
	{
		SetTimer( ROOM_VISITOR_REGISTER_TIMER , ROOM_VISITOR_REGISTER_FIRST_INTERVAL , NULL ) ;
	}

	if ( GetCurInfo().enmVIPLevel == core::VIPLEVEL_REGISTER)
	{
		m_SendMsgTime.SetUpdatePeriod(ROOM_SENDMSG_FIRST_INTERVAL);
		m_SendMsgTime.SetNextUpdateTime(GetTickCount());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CRoomMidDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CRoomMidDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_SPLITTER_MOVE:
		{
            ModifySplitter();
		}
		break;
	case SKIN_ON_CLICK_BTN_EMOTION:
		{
			OnClickBtnEmotion();
		}
		break;
	case SKIN_ON_CLICK_BTN_COLORWORD:
		{
			OnClickBtnColorWord();
		}
		break;
	case SKIN_ON_CLICK_BTN_CLEANSCREEN:
		{
			OnClickBtnCleanScreen();
		}
		break;
	case SKIN_ON_CLICK_BTN_STAMP:
		{
			OnClickBtnStamp();
		}
		break;
	case SKIN_ON_CLICK_BTN_SHORTCUT:
		{
			OnClickBtnShortCut();
		}
		break;
	case SKIN_ON_CLICK_BTN_COMPLAINT:
		{
			OnClickBtnComplaint();
		}
		break;
	case SKIN_ON_CLICK_JIACU:
		{
			OnClickBtnJiaCu((int)lParam );
		}
		break;
	case SKIN_ON_CLICK_XIETI:
		{
			OnClickBtnXieTi((int)lParam );
		}
		break;
	case SKIN_ON_CLICK_UNDERLINE:
		{
			OnClickBtnUnderline((int)lParam );
		}
		break;
	case SKIN_ON_CLICK_FONT_COLOR:
		{
			OnClickBtnFontColor();
		}
		break;
	case SKIN_ON_CLICK_BTN_SENDMSG:
		{
			OnClickBtnSendMsg();
		}
		break;
	case SKIN_ON_CLICK_BTN_AUTOREPLY:
		{
		//	OnClickBtnAutoReply();
		}
		break;
	case SKIN_ON_CLICK_BTN_COPYURL:
		{
			OnClickBtnCopyUrl();
		}
		break;
	case SKIN_ON_CLICK_BTN_FLYTEXT:
		{
			OnClickBtnFlyText();
		}
		break;
	case SKIN_ON_CLICK_BTN_SENDFLYTEXT:
		{
			OnClickBtnSendFlyText();
		}
		break;
	case SKIN_ON_CLICK_BTN_FLYTEXTEFFECT:
		{
			OnClickBtnFlyTextEffect();
		}
		break;
	case SKIN_ON_CLICK_BTN_FONT:
		{
			OnClickBtnFont();
		}
		break;
	case SKIN_ON_CLICK_BTN_MORE:
		{
			OnClickBtnMore();
		}
		break;
	default:break;
	}

	return TRUE;
}

LRESULT CRoomMidDlg::OnDragSplit(WPARAM wParam, LPARAM lParam)
{

	int splitterTop = (int)wParam;
	CRect publicChatRc(0,0,0,0),privateChatRc(0,0,0,0),splitterRc(0,0,0,0),paodaoRc(0,0,0,0),sofaRc(0,0,0,0);//公屏、私屏、滑动条区域

	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"itemHtmlPublicChat",&(publicChatRc.left),&(publicChatRc.top),
		                         &(publicChatRc.right),&(publicChatRc.bottom));
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"itemSofaDlg",&(sofaRc.left),&(sofaRc.top),
		                         &(sofaRc.right),&(sofaRc.bottom));
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"itemHtmlPrivateChat",&(privateChatRc.left),&(privateChatRc.top),
		                         &(privateChatRc.right),&(privateChatRc.bottom));
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"Splitter",&(splitterRc.left),&(splitterRc.top),
								 &(splitterRc.right),&(splitterRc.bottom));

	CComVariant var;
	//设置沙发，top变
	var = splitterTop - sofaRc.Height();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemSofaDlg", L"top", var);
	var = sofaRc.Height();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemSofaDlg", L"height", var);
	//设置公屏,height变
	var = splitterTop -  sofaRc.Height() - publicChatRc.top;//高度
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemHtmlPublicChat", L"height", var);
	
    //设置滑动条，top变，height也重设一下保险点。PS:滑动条背景图也要设置,不然你懂的,会闪烁。。
	var = splitterTop;
    m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"Splitter", L"top", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgSplitter", L"top", var);
	//var = splitterTop + 2;
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgSplitterEffect", L"top", var);
	var = splitterRc.Height();
    m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"Splitter", L"height", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgSplitter", L"height", var);
    
	//设置私屏，top和height都变
	var = splitterTop + splitterRc.Height();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemHtmlPrivateChat", L"top", var);
	var = privateChatRc.bottom - ( splitterTop + splitterRc.Height() );
    m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemHtmlPrivateChat", L"height", var);

	//ShowSofaChat();
	
	return 0;
}

void CRoomMidDlg::ModifySplitter()
{
	CRect rcPublicChat(0,0,0,0),rcInputMessage(0,0,0,0);//跑道、输入信息区域，用于做限制用
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"itemHtmlPublicChat",&(rcPublicChat.left),&(rcPublicChat.top),
		&(rcPublicChat.right),&(rcPublicChat.bottom));
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"itemHtmlInputMsg",&(rcInputMessage.left),&(rcInputMessage.top),
		&(rcInputMessage.right),&(rcInputMessage.bottom));
	m_Splitter.SetLimit(rcPublicChat.top + 80,rcInputMessage.top - 80);//设置移动范围为跑道区以下的200开始到信息输入区之上的200结束
}

void CRoomMidDlg::OnClickBtnEmotion()
{
//#if 0
//	static uint32 s_unID = 10001;
//	core::EnterRoomNotifyEvent * pEvent = new core::EnterRoomNotifyEvent();
//	pEvent->unRoomID = m_pParentDlg->GetRoomID();
//	pEvent->stUserInfo.unUIN = s_unID++;
//	pEvent->stUserInfo.strName = common::utility::stringhelper::IntToString(pEvent->stUserInfo.unUIN);
//	pEvent->stUserInfo.unVipLevel = 0x40 >> (rand()%7);
//	pEvent->stUserInfo.unTitleLevel = 0x10 >> (rand()%6);
//	pEvent->stUserInfo.unBusiness = 0x0000;
//	pEvent->stUserInfo.unRichLevel = rand()%24 + 1;
//	if (pEvent->stUserInfo.unTitleLevel == core::TITLELEVEL_SUPER_MANAGER)
//	{
//		pEvent->stUserInfo.unBusiness = core::BUSINESSTYPE_PATROLMAN;
//	}
//	pEvent->stUserInfo.unUsrLevel = rand()%100;
//	pEvent->stUserInfo.unStatus = 0;
//	pEvent->stUserInfo.un179id = pEvent->stUserInfo.unUIN;
//	s_vecID.push_back(pEvent->stUserInfo.unUIN);
//
//	g_RoomModule->m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_ENTER_ROOM_NOTIFY, MODULE_ID_ROOM, 0, 0, 0, pEvent) );
//	return;
//#endif

	m_pParentDlg->OnClickEmotion( this ) ;
}

void CRoomMidDlg::OnClickBtnColorWord()
{
//#if 0
//	if (s_vecID.size() > 0)
//	{
//		uint32 unUIN = s_vecID.front();
//		s_vecID.pop_front();
//		g_RoomModule->m_pModuleManager->PushEvent( MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_LEAVE_ROOM_NOTIFY, MODULE_ID_ROOM, unUIN, m_pParentDlg->GetRoomID(), 0, NULL));
//	}
//	return;
//#endif
	if ( m_bCaiZiMenu )
	{
		CaiZiMenu caizimenu(this) ;
		caizimenu.PopupMenu() ;
	}
	m_bCaiZiMenu = !m_bCaiZiMenu ;
}

//清屏
void CRoomMidDlg::OnClickBtnCleanScreen()
{
	m_PrivateLogic.ClearOutput() ;
	m_PublicLogic.ClearOutput() ;
}

void CRoomMidDlg::OnClickBtnStamp()
{
	m_pParentDlg->OnClickStamp() ; 
}

void CRoomMidDlg::OnClickBtnShortCut()
{
	if ( m_bShortCutMsgMenu )
	{
		ShortCutMsgMenu ShortCutMenu( this ) ;
		ShortCutMenu.PopupMenu() ;
	}
	m_bShortCutMsgMenu = !m_bShortCutMsgMenu ;
}

void CRoomMidDlg::OnClickBtnComplaint()
{
	common::utility::systemhelper::OpenUrl( m_pParentDlg->GetRoomBusiness()->GetFeedBackRoomUrl().GetBuffer() ) ;
}

//加粗
void CRoomMidDlg::OnClickBtnJiaCu(int iBold)
{
	SetFontBold( iBold ) ;
	CPersonalConfig::GetInstance()->SetChatMessageFont(m_InputLogic.GetFontInfo());
}

//斜体
void CRoomMidDlg::OnClickBtnXieTi(int iItalic)
{
	SetFontItalic( iItalic ) ;
	CPersonalConfig::GetInstance()->SetChatMessageFont(m_InputLogic.GetFontInfo());
}

//下划线
void CRoomMidDlg::OnClickBtnUnderline(int iUnderline)
{
	SetFontUnderLine( iUnderline ) ;
	CPersonalConfig::GetInstance()->SetChatMessageFont(m_InputLogic.GetFontInfo());
}

//字体颜色
void CRoomMidDlg::OnClickBtnFontColor()
{
	CColorDialog ColorDlg( GetFontInfo().font_color ) ;
	if ( ColorDlg.DoModal() == IDOK )
	{
		SetFontColor( ColorDlg.GetColor() ) ;
		CPersonalConfig::GetInstance()->SetChatMessageFont(m_InputLogic.GetFontInfo());
	}
}
//点击字体
void CRoomMidDlg::OnClickBtnFont()
{
	core::CurInfo myCurInfo = GetCurInfo();
	if (myCurInfo.enmVIPLevel >= core::VIPLEVEL_HONOR)//荣耀VIP及以上
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_SHOW_FONT, (LONG)0);
	}else
	{
		m_pParentDlg->GetHtmlOutPutManager()->OnModifyFont();
	}
}

//发送聊天消息
void CRoomMidDlg::OnClickBtnSendMsg()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_ROOM_FLYTEXT))->GetCheck())//发的是飞屏
	{
		OnClickBtnSendFlyText();
	}else
	{
		OnSendMsg();
	}
	m_pInputHtml->_SetFocus();
}

//DragonZ:跑道移到RoomParentDlg
//void CRoomMidDlg::SetSendObj( CString& strName , uint32 unUIN ) 
//{
//	if ( m_pPaodaoStatic != NULL )
//	{
//		m_pPaodaoStatic->OnClickNameLink( strName , unUIN ) ;
//	}	
//}

//选中聊天对象
void CRoomMidDlg::SetChatObj( CString strName , uint32 unUIN ) 
{
	int nIndex = m_ComboTalkTo.FindString( 1 , strName ) ;
	if ( nIndex != CB_ERR )
	{
		m_ComboTalkTo.DeleteString( nIndex ) ;
	}
	nIndex = m_ComboTalkTo.InsertString( 1 , strName ) ;
	if ( nIndex >= 0 )
	{
		m_ComboTalkTo.SetItemData( nIndex , unUIN ) ;
		m_ComboTalkTo.SetCurSel( nIndex ) ;
		if ( m_ComboTalkTo.GetCount() > TALKTO_COMBOX_MAX+1 )
		{
			m_ComboTalkTo.DeleteString( m_ComboTalkTo.GetCount() -1 ) ;
		}
	}
}

//是否允许再输入字符
bool CRoomMidDlg::OnInput( int nContenLen ) 
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
	if (pSelfItem == NULL)
	{
		return false;
	}
	if ( nContenLen > 20 && myCurInfo.enmVIPLevel < core::VIPLEVEL_VIP && pSelfItem->GetUserTitleLevel() == core::TITLELEVEL_NONE && !pSelfItem->IsOfficial() && !pSelfItem->IsActor())
	{
		CString strTemp ;
		strTemp.Format( L"发送内容不能超过20个字符哦，VIP以上用户自由发言无限制， <a href=\"%s\" target=\"_blank\">马上升级VIP</a>" , m_pParentDlg->GetRoomBusiness()->GetUpdateVipUrl() ) ;
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips(  _T("【提示】： ") , strTemp , COLOR_OPERATE ) ;
		return false ;
	}
	else if ( nContenLen>150/* && myCurInfo.enmVIPLevel >= core::VIPLEVEL_VIP */)
	{
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( _T("【提示】： ") ,_T("一次发表内容不能多于150个字符") , COLOR_OPERATE ) ;
		return false ;
	}
	return true ;
}

bool CRoomMidDlg::CanSendMsg( UINT32& uDestUin , CString&strText,bool bAutoMsg)
{
	core::CurInfo myCurInfo = GetCurInfo() ;	
	IUserItem* pSrcUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;	
	if ( pSrcUserItem == NULL )
	{
		return FALSE;
	}
	RoomOption* pRoomOption = m_pParentDlg->GetRoomBusiness()->GetRoomOption() ;
	if ( pRoomOption == NULL )
	{
		return FALSE ;
	}

	CString strTalkTo ;
	((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetWindowText( strTalkTo ) ;
	if ( strTalkTo.IsEmpty() )
	{
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( _T("【提示】： ") ,_T("请选择聊天对象") , COLOR_OPERATE ) ;
		return FALSE ;
	}
	else if ( strTalkTo == _T("所有人") || !((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->GetCheck() )
	{
		if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE && pSrcUserItem->GetUserTitleLevel() == core::TITLELEVEL_NONE)
		{
			C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能在房间发言，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
				m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 	
			return FALSE;
		}

		ENM_RoomPublicChat enmType = pRoomOption->GetPublicChatType() ;
		if ( enmType == PUBLICCHAT_ABOVE_VIP )
		{
			if ( (myCurInfo.enmVIPLevel < core::VIPLEVEL_VIP) && pSrcUserItem->GetUserTitleLevel() == core::TITLELEVEL_NONE && !pSrcUserItem->IsOfficial())
			{	
				C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK, L"只有VIP以上用户才能公聊", 
					L"", L"", L"立即升级", m_pParentDlg->GetRoomBusiness()->GetUpdateVipUrl().GetBuffer(0));
				return FALSE;
			}
		}

		if (!m_SendMsgTime.isReady(GetTickCount()))
		{
			uint32 nLeftTime = m_SendMsgTime.GetRestPeriod(GetTickCount()) / 1000 + 1;
			m_pParentDlg->GetHtmlOutPutManager()->OnSendMsgLimitedTime(nLeftTime);
			return FALSE;
		}

		if ( strTalkTo != L"所有人" )
		{
			int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetCurSel() ;
			IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( 
				((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetItemData( nCurSel ) ) ;
			if ( pDstUserItem == NULL )
			{
				m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( _T("【提示】： ") ,_T("抱歉，对方已离开房间") , COLOR_OPERATE ) ;
				((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->DeleteString(nCurSel);
				return FALSE;
			}
			uDestUin = pDstUserItem->GetUserUIN() ;
		}
	}
	else
	{
		if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE && pSrcUserItem->GetUserTitleLevel() == core::TITLELEVEL_NONE)
		{
			C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能在房间发言，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
				m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 	
			return FALSE;
		}
		int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetCurSel() ;
		IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( 
			((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetItemData( nCurSel ) ) ;
		if ( pDstUserItem == NULL )
		{
			m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( _T("【提示】： ") ,_T("抱歉，对方已离开房间") , COLOR_OPERATE ) ;
			((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->DeleteString(nCurSel);
			return FALSE;
		}

		IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
		if (pSelfItem == NULL)
		{
			return FALSE;
		}

		if (!m_SendMsgTime.isReady(GetTickCount()))
		{
			uint32 nLeftTime = m_SendMsgTime.GetRestPeriod(GetTickCount()) / 1000 + 1;
			m_pParentDlg->GetHtmlOutPutManager()->OnSendMsgLimitedTime(nLeftTime);
			return FALSE;
		}
		//if (myCurInfo.enmVIPLevel == core::VIPLEVEL_REGISTER && !pSelfItem->IsOfficial() && !pSelfItem->IsActor() && pSelfItem->GetUserTitleLevel() == core::TITLELEVEL_NONE)
		//{
		//	((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->SetCheck(FALSE);
		//	C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK, L"只有VIP以上用户才能私聊", 
		//		L"", L"", L"马上升级VIP", m_pParentDlg->GetRoomBusiness()->GetUpdateVipUrl().GetBuffer(0));

		//	return FALSE;
		//}
		uDestUin = pDstUserItem->GetUserUIN() ;
	}

	if ( strText != m_strLastMessage )
	{
		m_strLastMessage = strText ;
		m_nRepeatCount = 0 ;
	}
	else
	{
		if ( ++m_nRepeatCount >= 3 && bAutoMsg==FALSE)
		{
			m_pParentDlg->GetHtmlOutPutManager()->OnNotRepeat() ;		//禁止3次重复发言
			return FALSE;
		}
	}
	return TRUE ;
}

bool CRoomMidDlg::CanSendAutoMsg()
{
	core::CurInfo myCurInfo = GetCurInfo() ;	
	IUserItem* pSrcUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( myCurInfo.unUIN ) ;	
	if ( pSrcUserItem == NULL )//用户不存在
	{
		return FALSE;
	}
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE && pSrcUserItem->GetUserTitleLevel() == core::TITLELEVEL_NONE)//是游客
	{
		return FALSE;
	}
	if ( pSrcUserItem->GetUserStatus() & core::USERSTATUSTYPE_FORBIDDEN )//被禁言
	{		
		return FALSE;
	}

	return TRUE ;
}

void CRoomMidDlg::OnSendMsg() //发送聊天信息
{
	UINT32 udestuin = 0 ;
	CString strText, strTextCache;
	m_InputLogic.GetOutputString( strText , strTextCache ) ;
	strTextCache.Replace(L"<IMG", L"*&^IMG%&^");
	m_PublicLogic.TranslateMsgForInput(strTextCache);
	strTextCache.Replace(L"*&^IMG%&^", L"<IMG");

	IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
	if (pSelfItem != NULL && pSelfItem->IsChangeVoice())
	{
		CChangeVoiceItem * pItem = dynamic_cast<CChangeVoiceItem*>(GiftManager::GetInstance()->SearchGiftByID(pSelfItem->GetUserChangeVoiceID()));
		if (pItem != NULL)
		{
			strTextCache = pItem->GetVoiceWords().c_str();
		}
	}
	if ( strTextCache.IsEmpty() )
	{
		m_pParentDlg->GetHtmlOutPutManager()->OnMsgEmpty() ;
		return ;
	}
	if ( OnInput(m_InputLogic.GetPictureNum()+ m_InputLogic.GetContentLen()) && CanSendMsg( udestuin , strTextCache) )
	{
		SendMsg( udestuin , strTextCache ,FALSE) ;

		if (pSelfItem->GetUserVipLevel() == core::VIPLEVEL_REGISTER && pSelfItem->GetUserTitleLevel() == core::TITLELEVEL_NONE && !pSelfItem->IsOfficial() && !pSelfItem->IsActor())
		{
			m_SendMsgTime.SetUpdatePeriod(ROOM_SENDMSG_SECOND_INTERVAL);
			m_SendMsgTime.SetNextUpdateTime(GetTickCount());
		}
	}
}

void CRoomMidDlg::OnSendAutoMsg(uint32 uDestUin)//发送自动回复
{
	CString strText = AutoReplyMgr->GetCheckedAutoReply().c_str();
	if ( !strText.IsEmpty() && CanSendAutoMsg() )
	{
		SendMsg( uDestUin , strText , TRUE) ;
	}
}

//DragonZ:跑道移到RoomParentDlg
//void CRoomMidDlg::InsertPaoDaoText(CString& strSrcName , CString& strDstName ,CString& strGiftWord , uint32 unSrc179ID , 
//					  uint32 unDst179ID ,  uint16 unGiftID ,uint16 unCount , CString& strTime ) 
//{
//	m_pPaodaoStatic->InsertPaoDaoText( strSrcName , strDstName ,strGiftWord , unSrc179ID , unDst179ID , unGiftID , unCount , strTime ) ;
//}

void CRoomMidDlg::InsertPrivateText(CString& strText ) 
{
	m_pPrivateHtml->InsertText( strText ) ;
}

void CRoomMidDlg::InsertPublicText(CString& strText ) 
{
	m_pPublicHtml->InsertText( strText ) ;
}

void CRoomMidDlg::SetPublicOutputGotoEndAble( BOOL bAble ) 
{
	if ( bAble )
	{
		m_pPublicHtml->SetGotoEndAble() ;
	}	
	else
	{
		m_pPublicHtml->SetUnGotoEnd() ;
	}
}


void CRoomMidDlg::SetPrivateOutputGotoEndAble( BOOL bAble ) 
{
	if ( bAble )
	{
		m_pPrivateHtml->SetGotoEndAble() ;
	}	
	else
	{
		m_pPrivateHtml->SetUnGotoEnd() ;
	}
}

//DragonZ:跑道移到RoomParentDlg
//void CRoomMidDlg::OnRoomRightSendToChange( CString& strSelText ) 
//{
//	m_pPaodaoStatic->OnRoomRightSendToChange( strSelText ) ;
//}

void CRoomMidDlg::OnSendSheChiGiftNotify()
{
}

void CRoomMidDlg::OnClickNameLink( CString& strName , uint32 unUUID )
{
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( unUUID == myCurInfo.unUIN )
	{
		return ;
	}
	SetChatObj( strName , unUUID ) ;
	//DragonZ:跑道移到RoomParentDlg
	//m_pPaodaoStatic->OnClickNameLink( strName , unUUID ) ;
	m_pParentDlg->OnClickNameLink(strName, unUUID);
}

void CRoomMidDlg::OnClickNoRemind(uint32 uuUUID)
{
	AutoReplyMgr->SetRejectAutoMsg(uuUUID,TRUE);
}

void CRoomMidDlg::SendMsg( UINT32 uDestUin , CString& strText , BOOL bAutoMsg)
{
	bool bPill = false ;
	bool bForbidden = m_pParentDlg->GetHtmlOutPutManager()->ProcessSendMsgText( strText ) ;

	if(!bForbidden)
	{
		bForbidden = GetCurInfo().bInBlacklist;
	}
	 
	if ( ( uDestUin != 0 && ((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->GetCheck()) || (bAutoMsg == TRUE))
	{
		bPill = true ;
	}
	else
	{
		bPill = false ;
	}

	BOOL bRejectAutoMsg = AutoReplyMgr->GetRejectAutoMsg(uDestUin);//我对此人是否设置不再提醒自动回复

	if(bForbidden || CSendEventManager::SendMessageOutEvent( m_pParentDlg->GetRoomID(), uDestUin , bPill , std::wstring(strText) , GetFontInfo(), bAutoMsg, bRejectAutoMsg) )
	{
		if ( !bAutoMsg )
		{
			m_InputLogic.ClearInput() ;
		}		
		if ( bPill )
		{
			BackMessage( uDestUin , GetFontInfo() , strText.GetBuffer() , core::SENDMESSAGETYPE_PRIVATE , bAutoMsg) ;//私聊
		}
		else
		{
			BackMessage( uDestUin , GetFontInfo() , strText.GetBuffer() , core::SENDMESSAGETYPE_PUBLIC ) ;//公聊
		}

		//如果被禁用，直接取出来显示在公屏
		if(bForbidden)
		{
			SendMessageEvent message = m_dMessageBack.back();
			m_dMessageBack.pop_back();
			m_pParentDlg->GetHtmlOutPutManager()->OnSendMsgSuccess( message ) ;
		}
	}
}

void CRoomMidDlg::BackMessage( UINT32 dstuin , MessageFont& stFont , std::wstring strText , ENM_SendMessageType enmType , bool bAutoMsg)//备份
{
	SendMessageEvent message ;
	message.dstuin = dstuin ;
	message.stFont = stFont ;
	message.message =  strText ;
	message.enmType = enmType ;
	message.bIsAutoMsg = bAutoMsg;
	m_dMessageBack.push_back( message ) ;
}

//从备份中取一条
bool CRoomMidDlg::GetAMessageBack( SendMessageEvent& message ) 
{
	if ( m_dMessageBack.empty() )
	{
		return false ;
	}
	else
	{
		message = m_dMessageBack.front() ;
		m_dMessageBack.pop_front() ;
		return true ;
	}
}

void CRoomMidDlg::InitFontPanel() 
{	
	core::MessageFont FontInfo;
	if (GetCurInfo().enmVIPLevel >= core::VIPLEVEL_HONOR)//荣耀VIP以上才可修改字体，防止从配置文件修改
	{
		FontInfo = CPersonalConfig::GetInstance()->GetChatMessageFont();
	}else
	{
		//这里的初始化与ConfigManager里的聊天字体信息初始化一致
		FontInfo.font_size = 14;
		FontInfo.font_color = 0;
		FontInfo.font_style = 0;
		FontInfo.font_type = L"宋体";
	}

	SetFontSize(FontInfo.font_size);
	
	wstring wsCurrentFont = FontInfo.font_type;
	vector<wstring> vFontList ;
	common::utility::systemhelper::GetChineseFontNameList( vFontList ) ;
	vector<wstring>::iterator iter ;
	for ( iter = vFontList.begin() ; iter != vFontList.end() ; ++iter )
	{
		//int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_FONT))->AddString( (*iter).c_str() ) ;
		if ( wsCurrentFont == (*iter) )
		{
			//((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_FONT))->SetCurSel( nIndex ) ;
			CString strTempFontname(wsCurrentFont.c_str());
			SetFontName(strTempFontname);
		}
	}
	
	m_InputLogic.SetFontColor(FontInfo.font_color);
	bool bBold = FontInfo.font_style & core::FONTSTYLETYPE_BOLD;
	bool bItalic = FontInfo.font_style & core::FONTSTYLETYPE_ITALICS;
	bool bUnderLine = FontInfo.font_style & core::FONTSTYLETYPE_UNDERLINE;
	SetButtonStatus( bBold , bItalic , bUnderLine ) ;
}

LRESULT CRoomMidDlg::OnClickEmotionList( WPARAM wParam ,LPARAM lParam ) 
{
	CString strOutPutStr;
	m_InputLogic.GetOutputStringWithoutPic(strOutPutStr);
	if (strOutPutStr.Compare(DEFAULT_ROOMMID_INPUT_TEXT) == 0)
	{
		m_pInputHtml->ClearMsg();
	}

	CString strFilePath = (LPCTSTR)lParam ;
	m_InputLogic.OnCustomEmotion( m_pInputHtml , strFilePath.GetBuffer() ) ;
	m_pInputHtml->_SetFocus();
	return S_OK ;
}

LRESULT CRoomMidDlg::OnClickCaiZi( WPARAM wParam ,LPARAM lParam ) 
{
	int nCaiZiType = lParam ;
	CRect rcTemp;
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"btnColorWord",&(rcTemp.left),&(rcTemp.top),
		&(rcTemp.right),&(rcTemp.bottom));
	ClientToScreen( &rcTemp ) ;
	common::ui::CCursorPos pt;
	if ( !rcTemp.PtInRect( pt ) && nCaiZiType == 0 )
	{
		m_bCaiZiMenu = FALSE ;
	}	
	if ( nCaiZiType == 0 )
	{		
		return S_OK;
	}
	m_bCaiZiMenu = FALSE ;
	core::CurInfo myCurInfo = GetCurInfo() ;
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
	{	
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能在房间发言，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 
		return S_OK;
	}
	IUserItem* pSelfUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() ;
	if ( pSelfUserItem != NULL && pSelfUserItem->GetUserTitleLevel() == core::TITLELEVEL_NONE && !m_pParentDlg->GetRoomBusiness()->GetRoomOption()->CanColorText() )
	{
		m_pParentDlg->GetHtmlOutPutManager()->OnSendMsg_NoCaiZi() ;
		return S_OK;
	}
	CString strTalkTo ;
	((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetWindowText( strTalkTo ) ;
	UINT udestuin = 0 ;
	if ( strTalkTo != _T("所有人"))
	{
		int nindex = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetCurSel() ;
		if ( nindex == CB_ERR )
		{
			return S_OK;
		}
		IUserItem* pDstUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN(
			((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetItemData( nindex ) ) ;
		if ( pDstUserItem == NULL )
		{
			return S_OK;
		}
		udestuin = pDstUserItem->GetUserUIN() ;
	}
	uint32 nCaiZiIntervalTime = m_pParentDlg->GetRoomBusiness()->GetCaiZiIntervalTime( myCurInfo.enmVIPLevel ) ;
	if ( nCaiZiIntervalTime != 0 && m_nLastCaiZiTime != 0 && (myCurInfo.unSystemTime - m_nLastCaiZiTime) < nCaiZiIntervalTime )
	{
		m_pParentDlg->GetHtmlOutPutManager()->OnNotCaiZiTime( myCurInfo.enmVIPLevel ,nCaiZiIntervalTime-(myCurInfo.unSystemTime-m_nLastCaiZiTime) ) ;
	}
	else
	{
		m_nLastCaiZiTime = myCurInfo.unSystemTime ;
		MessageFont FontInfo = GetFontInfo() ;
		FontInfo.font_style = FONTSTYLETYPE_COLORTEXT ;
		CString strTextCache ;
		strTextCache.Format(_T("%d") ,nCaiZiType ) ;
		CSendEventManager::SendMessageOutEvent( m_pParentDlg->GetRoomID(), udestuin , false , std::wstring(strTextCache) , FontInfo ) ;
		BackMessage( udestuin , FontInfo , strTextCache.GetBuffer() , core::SENDMESSAGETYPE_PUBLIC ) ;
	}

	return S_OK ;
}

LRESULT CRoomMidDlg::OnClickShortCutMsg( WPARAM wParam ,LPARAM lParam )
{
	int nIndex = lParam ;
	if ( nIndex == 0 )
	{
		CRect rcTemp;
		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"btnShortcut",&(rcTemp.left),&(rcTemp.top),
			&(rcTemp.right),&(rcTemp.bottom));
		ClientToScreen( &rcTemp ) ;
		common::ui::CCursorPos pt;
		if ( !rcTemp.PtInRect( pt ) )
		{
			m_bShortCutMsgMenu = FALSE ;
		}
		return S_OK ;
	}
	else if ( nIndex == SELF_DEFINE_SHORCUT )
	{
		SwitchResourceToModule(_T("Room"));
		CSelfDefSentenceDlg pSelfDefSentenceDlg(this) ;
		pSelfDefSentenceDlg.DoModal();
	}
	else
	{
		wstring wsTemp = ShortCutMsgMgr->GetShortCutMsgByIndex( nIndex-1 ) ;
		if ( !wsTemp.empty() )
		{
			CString strShortCutMsg = wsTemp.c_str();
			UINT32 udestuin = 0 ;
			if ( CanSendMsg( udestuin , strShortCutMsg ) )
			{
				SendMsg( udestuin , strShortCutMsg );
			}
		}
	}
	m_bShortCutMsgMenu = FALSE ;
	return S_OK;
}

//void CRoomMidDlg::OnCbnSelchangeComboFontsize()
//{
//	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_FONTSIZE))->GetCurSel() ;
//	if ( nIndex != CB_ERR )
//	{
//		CString strTmep ;
//		((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_FONTSIZE))->GetWindowText( strTmep ) ;
//		int nFontSize = _wtoi( strTmep) ;
//		if ( nFontSize != GetFontInfo().font_size )
//		{
//			SetFontSize( nFontSize) ;
//			CPersonalConfig::GetInstance()->SetChatMessageFont(m_InputLogic.GetFontInfo());
//		}
//	}
//}
//
//void CRoomMidDlg::OnCbnSelchangeComboFont()
//{
//	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_FONT))->GetCurSel() ;
//	if ( nIndex != CB_ERR )
//	{
//		CString strTmep ;
//		((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_FONT))->GetWindowText( strTmep ) ;
//		CString strFontName( GetFontInfo().font_type.c_str() ) ;
//		if ( strTmep != strFontName )
//		{
//			SetFontName( strTmep) ;
//			CPersonalConfig::GetInstance()->SetChatMessageFont(m_InputLogic.GetFontInfo());
//		}
//	}
//}

void CRoomMidDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch ( nIDEvent )
	{
	case ROOM_VISITOR_REGISTER_TIMER:
		{
			KillTimer( ROOM_VISITOR_REGISTER_TIMER ) ;
			SetTimer( ROOM_VISITOR_REGISTER_TIMER , ROOM_VISITOR_REGISTER_SECOND_INTERVAL , NULL ) ;
			IUserItem* pUserItem = GetARandomActorUserItem() ;
			vector<IUserItem*> vecMicList ;
			vector<IUserItem*>::iterator iter ;
			if ( pUserItem == NULL )
			{
				for ( int i = 1 ; i<= 3 ; ++i )
				{
					uint32 unUIN = m_pParentDlg->GetPulicMicUINByIndex( i ) ;
					if (  unUIN != 0  )
					{
						IUserItem* pUserItemTemp = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN( unUIN ) ;
						if ( pUserItemTemp != NULL )
						{
							vecMicList.push_back( pUserItemTemp ) ;
						}						
					}
				}
			}
			if ( !vecMicList.empty() )
			{
				//非艺人在麦上
				iter = vecMicList.begin() ;
				advance( iter , rand()%vecMicList.size() ) ;
				pUserItem = *iter ;
			}
			if ( pUserItem != NULL )
			{
				m_pParentDlg->GetHtmlOutPutManager()->VisitorRegister( pUserItem ) ;
			}
		}
		break;
	//case ROOM_SENDMSG_TIMER:
	//	{
	//		//m_SendMsgTime
	//	}
	//	break;
	case ROOM_AUTO_WELECOME_TIMER:
		{
			//自动欢迎词
			CreateAutoWelcomeTimer() ;
			IUserItem* pUserItem = GetARandomActorUserItem() ;
			if ( pUserItem != NULL )
			{
				m_pParentDlg->GetHtmlOutPutManager()->AutoWelcome( pUserItem ) ;
			}
		}
		break;
	case ROOM_FIRSTLOGIN_AUTO_WELECOME_TIMER:
		{
			//第一次登录的自动欢迎词
			CreateFirstLoginAutoWelcomeTimer();
			IUserItem* pUserItem = GetARandomActorUserItem();
			if (pUserItem != NULL)
			{
				m_pParentDlg->GetHtmlOutPutManager()->AutoWelcome( pUserItem ) ;
			}
		}
		break;
	default:break;
		}

	CDialog::OnTimer(nIDEvent);
}

void CRoomMidDlg::SetFontSize(UINT fontsize ) 
{
	m_InputLogic.SetFontSize( fontsize ) ;
	m_pInputHtml->UpdateMy3() ;
}


void CRoomMidDlg::SetFontBold( int checked ) 
{
	m_InputLogic.SetFontBold( checked ) ;
	m_pInputHtml->UpdateMy3() ;
}

void CRoomMidDlg::SetFontItalic( int checked ) 
{
	m_InputLogic.SetFontItalic( checked ) ;
	m_pInputHtml->UpdateMy3() ;
}

void CRoomMidDlg::SetFontUnderLine( int checked ) 
{
	m_InputLogic.SetFontUnderLine( checked ) ;
	m_pInputHtml->UpdateMy3() ;
}

void CRoomMidDlg::SetFontName(CString& strFontName ) 
{
	m_InputLogic.SetFontName( strFontName ) ;
	m_pInputHtml->UpdateMy3() ;
}

void CRoomMidDlg::SetFontColor(COLORREF cf ) 
{
	m_InputLogic.SetFontColor( cf ) ;
	m_pInputHtml->UpdateMy3() ;
}

void CRoomMidDlg::SetButtonStatus(bool bBold ,bool bItalic ,bool bUnderLine )
{
	if (bBold)
	{
		//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnJiaCu", L"checked", (CComVariant)TRUE);
		m_InputLogic.SetFontBold(1);
	}
	else
	{
		//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnJiaCu", L"checked", (CComVariant)FALSE);
		m_InputLogic.SetFontBold(0);
	}

	if (bItalic)
	{
		//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnXieTi", L"checked", (CComVariant)TRUE);
		m_InputLogic.SetFontItalic(1);
	}
	else
	{
		//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnXieTi", L"checked", (CComVariant)FALSE);
		m_InputLogic.SetFontItalic(0);
	}

	if (bUnderLine)
	{
		//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnUnderLine", L"checked", (CComVariant)TRUE);
		m_InputLogic.SetFontUnderLine(1);
	}
	else
	{
		//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnUnderLine", L"checked", (CComVariant)FALSE);
		m_InputLogic.SetFontUnderLine(0);
	}
}

MessageFont CRoomMidDlg::GetFontInfo() 
{
	return m_InputLogic.GetFontInfo() ;
}

void CRoomMidDlg::CreateAutoWelcomeTimer() 
{
	UINT nTime = (rand()%7200+5)*1000 ;
	SetTimer( ROOM_AUTO_WELECOME_TIMER , nTime , NULL ) ;
}

void CRoomMidDlg::CreateFirstLoginAutoWelcomeTimer()
{
	UINT nTime = 5 * 60 * 1000;//每5分钟一次
	SetTimer(ROOM_FIRSTLOGIN_AUTO_WELECOME_TIMER, nTime, NULL);
}

IUserItem* CRoomMidDlg::GetARandomActorUserItem() 
{
	vector<IUserItem*> vecActorList ;
	vector<IUserItem*> vecMicList ;
	vector<IUserItem*>::iterator iter ;
	IUserItem* pUserItemResult = NULL ;
	m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetAllActorExceptSelf( vecActorList ) ;
	for ( iter = vecActorList.begin() ; iter != vecActorList.end() ; ++iter )
	{
		IUserItem* pUserItem = *iter ;
		if ( pUserItem->IsPublicOne() || pUserItem->IsPublicTwo() || pUserItem->IsPublicThree() )
		{
			vecMicList.push_back( pUserItem ) ;
		}
	}
	if ( !vecMicList.empty() )
	{
		//艺人在麦上
		iter = vecMicList.begin() ;
		advance( iter , rand()%vecMicList.size() ) ;
		pUserItemResult = *iter ;
	}
	else if ( !vecActorList.empty() )
	{
		//在线艺人
		iter = vecActorList.begin() ;
		advance( iter , rand()%vecActorList.size() ) ;
		pUserItemResult = *iter ;
	}
	return pUserItemResult ;
}

void CRoomMidDlg::OnAddUser( IUserItem * pUserItem )
{
	if (pUserItem->GetUserUIN() == GetCurInfo().unUIN)
	{
		//普通有管理的用户
		if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_REGISTER && pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE || pUserItem->IsOfficial() || pUserItem->IsActor())
		{
			m_SendMsgTime.SetUpdatePeriod(0);
			m_SendMsgTime.SetNextUpdateTime(GetTickCount());
		}
	}

	m_pParentDlg->GetRoomBusiness()->GetSofaManager()->UpdateSofaUserInfo(pUserItem);
}

void CRoomMidDlg::OnDeleteUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	//用户离开的时候不删除，发送聊天消息的时候再判断
	//CString strTalkTo ;
	//((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetWindowText( strTalkTo ) ;
	//int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->FindString( 0, pUserItem->GetUserName().c_str()); 
	//if ( nIndex != CB_ERR )
	//{
	//	((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->DeleteString(nIndex);
	//	int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->GetCount() ;
	//	if ( strTalkTo == CString(pUserItem->GetUserName().c_str()) || nCount == 1 )
	//	{
	//		((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->SetCurSel( 0 ) ;	
	//	}
	//	else
	//	{
	//		((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->SetCurSel( nIndex-1 ) ;
	//	}

	//	((CComboBox*)GetDlgItem(IDC_COMBO_ROOM_TALKTO))->Invalidate(TRUE);
	//}

	//DragonZ:跑道移到RoomParentDlg
	//m_pPaodaoStatic->OnDeleteUser( pUserItem ) ;
}

//void CRoomMidDlg::OnClickBtnAutoReply()//自动回复
//{
//	if (m_bAutoReplyMenu)
//	{
//		AutoReplyMenu autoReplyMenu(this) ;
//		autoReplyMenu.PopupMenu() ;
//	}
//	m_bAutoReplyMenu = !m_bAutoReplyMenu ;
//}

void CRoomMidDlg::OnClickBtnCopyUrl()//复制房间地址
{
	m_pParentDlg->GetHtmlOutPutManager()->OnCopyRoomUrl();	
}

//HRESULT CRoomMidDlg::OnClickBtnAutoReplyMsg(WPARAM wParam, LPARAM lParam)
//{
//	int nIndex = lParam ;
//	if (nIndex == 0)
//	{
//		CRect rcTemp;
//		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"btnAutoReply",&(rcTemp.left),&(rcTemp.top),
//			&(rcTemp.right),&(rcTemp.bottom));
//		ClientToScreen(&rcTemp) ;
//		common::ui::CCursorPos pt;
//		if ( !rcTemp.PtInRect(pt) )
//		{
//			m_bAutoReplyMenu = FALSE ;
//		}
//		return S_OK ;
//	}
//	else if (nIndex == SET_AUTOREPLY_MENU)//点击设置自动回复
//	{	
//		SwitchResourceToModule(_T("Room"));
//		CAutoReplySettingDlg pDlg;
//		if (IDOK == pDlg.DoModal())
//		{
//			AutoReplyMgr->SetAutoReply(pDlg.GetAutoReplyState());
//			AutoReplyMgr->UpdateAutoReplyList(pDlg.GetNewAutoReply());
//		}
//		
//	}
//	else if (nIndex == CANCEL_AUTOREPLY_MENU)//点击取消自动回复
//	{
//		AutoReplyMgr->SetAutoReply(FALSE);
//	}
//	else //选择了自动回复的语句
//	{
//		stAutoReplyMenuItem stItem;
//		AutoReplyMgr->SetCheckedByIndex(nIndex - 1);
//	}
//	m_bAutoReplyMenu = FALSE ;
//	return S_OK;
//}

void CRoomMidDlg::OnClickBtnFlyText()
{
	core::CurInfo myCurInfo = GetCurInfo();
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE ,L"游客不能发送飞屏，" ,L"想要畅玩白兔KTV？" ,L"" ,L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() , L"提 示") ;
		return ;
	}
	//if ( myCurInfo.enmVIPLevel < core::VIPLEVEL_VIP)
	//{
	//	CString strUrl = m_pParentDlg->GetRoomBusiness()->GetUpdateVipUrl() ;

	//	C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK ,L"VIP以上用户才能发送飞屏" , L"" , L"" , L"立即升级" , strUrl) ;
	//	return;
	//}

	CWnd * pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		pParentWnd->SendMessage(WM_CLICK_FLYINGTEXT, 0, m_nFlyTextType);
	}
}

void CRoomMidDlg::OnClickBtnSendFlyText()
{
	core::CurInfo myCurInfo = GetCurInfo();
	if ( myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE )
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能发送飞屏，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" ,  
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() , L"提 示" ) ;
		return;
	}
	//if ( myCurInfo.enmVIPLevel < core::VIPLEVEL_VIP)
	//{
	//	CString strUrl = m_pParentDlg->GetRoomBusiness()->GetUpdateVipUrl() ;
	//	C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK, L"VIP以上用户才能发送飞屏", L"", L"", L"立即升级", strUrl );
	//	return;
	//}

	CString strText, strTextCache;
	//m_InputLogic.GetOutputString(strText, strTextCache);
	m_InputLogic.GetOutputStringWithoutPic(strText);
	CString strTemp = strText;
	if (strTemp.Trim().IsEmpty())
	{
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( L"【提示】： " ,L"请输入飞屏内容"  , COLOR_OPERATE ) ;
		return;
	}

	if (strText.IsEmpty())
	{
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( L"【提示】： " ,L"请输入飞屏内容"  , COLOR_OPERATE ) ;
		return;
	}

	if (m_InputLogic.GetContentLen() == 0)
	{
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( L"【提示】： " ,L"请输入飞屏内容" , COLOR_OPERATE ) ;
		return;
	}

	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	if (nLen > 50)
	{
		m_pParentDlg->GetHtmlOutPutManager()->InsertPrivateTips( L"【提示】： " ,L"飞屏内容不能多于50个字符" , COLOR_OPERATE ) ;
		return;
	}

	CString strPrompt;
	CConfigManager::Instance()->GetUserInfo_Setting(GetCurInfo().unUIN,USERINFO_FLYTEXT_PROMPT,strPrompt);
	bool bPrompt = true;
	if (strPrompt == L"" || strPrompt == L"true")
	{
		bPrompt = true;
	}
	else
	{
		bPrompt = false;
	}

	int nRet;
	if (bPrompt)
	{
		if (m_InputLogic.GetPictureNum() != 0)
		{			
			nRet = C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_CANCEL_THREE_STRING_WITH_CHECKBOX ,L"飞屏内容暂不支持表情图片，", L"图片部分将不予显示。" ,L"每条飞屏将消耗10000币，确定发送？");
		}
		else
		{
			nRet = C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_CANCEL_ONE_STRING_WITH_CHECKBOX , L"每条飞屏将消耗10000币，确定发送？" );
		}

		if (C179MsgBox::IsChecked())
		{
			CConfigManager::Instance()->SetUserInfo_Setting(GetCurInfo().unUIN,USERINFO_FLYTEXT_PROMPT,L"false");
		}

		if (nRet != RET_OK)
		{
			return;
		}
	}


	std::wstring strContent = strText.GetBuffer(0);
	common::utility::systemhelper::FilterWords(strContent);
	int nType = m_nFlyTextType;
	if (m_nFlyTextType == 0)
	{
		nType = rand() % 4 + 1;
	}
	ScreenEffectManager::EncodeFlyingText(strContent, nType);

	IGiftItem * pGiftItem = GiftManager::GetInstance()->GetFlyingWordsItem();
	if (pGiftItem != NULL)
	{
		CSendEventManager::SendFlyingWords(m_pParentDlg->GetRoomID(), pGiftItem->GetID(), strContent);
	}

	m_InputLogic.ClearInput();
}

void CRoomMidDlg::OnClickBtnFlyTextEffect()
{
	//int nType = m_pParentDlg->GetFlyTextEffect();
	//if (nType != -1)
	//{
	//	m_nFlyTextType = nType;
	//}

	FlyTextMenu menu(this, m_nFlyTextType);
	int nRet = menu.PopupMenu();
	if (nRet != 0)
	{
		if (nRet == FLYINT_TEXT_EFFECT_RANDOM)
		{
			m_nFlyTextType = 0;
		}
		else
		{
			m_nFlyTextType = nRet;
		}
	}
}

void CRoomMidDlg::SetFlyTextType( int nType )
{
	m_nFlyTextType = nType;
}

void CRoomMidDlg::OnBnClickedCheckRoomPillowchat()
{
	// TODO: 在此添加控件通知处理程序代码
	core::CurInfo myCurInfo = GetCurInfo();
	if (myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->SetCheck(FALSE);
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能在房间发言，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 

		return;
	}

	IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
	if (pSelfItem == NULL)
	{
		return;
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_CHECK_ROOM_FLYTEXT))->SetCheck(FALSE);
	}
}

void CRoomMidDlg::OnUpdateUser( IUserItem * pUserItem )
{
	ASSERT(pUserItem != NULL);
	if (pUserItem->GetUserUIN() == GetCurInfo().unUIN)
	{
		if (pUserItem->GetUserVipLevel() == core::VIPLEVEL_REGISTER)
		{
			if (pUserItem->GetUserTitleLevel() > core::TITLELEVEL_NONE || pUserItem->IsOfficial() || pUserItem->IsActor())
			{
				m_SendMsgTime.SetUpdatePeriod(0);
				m_SendMsgTime.SetNextUpdateTime(GetTickCount());
			}
			else
			{
				//原来是管理，公聊没限制
				if (m_SendMsgTime.GetUpdatePeriod() == 0)
				{
					m_SendMsgTime.SetUpdatePeriod(ROOM_SENDMSG_SECOND_INTERVAL);
					m_SendMsgTime.SetNextUpdateTime(GetTickCount());
				}
			}
		}
		else if (pUserItem->GetUserVipLevel() > core::VIPLEVEL_REGISTER)
		{
			m_SendMsgTime.SetUpdatePeriod(0);
			m_SendMsgTime.SetNextUpdateTime(GetTickCount());
		}
	}
}

void CRoomMidDlg::GetCaiTiaoAndPublicScreenRect(CRect &rcCaiTiao, CRect &rcPublicScreen)
{
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"btnEmotionAndFont", &(rcCaiTiao.left), &(rcCaiTiao.top), &(rcCaiTiao.right), &(rcCaiTiao.bottom));
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"itemHtmlPublicChat", &(rcPublicScreen.left), &(rcPublicScreen.top), &(rcPublicScreen.right), &(rcPublicScreen.bottom));
	ClientToScreen(&rcCaiTiao);
	ClientToScreen(&rcPublicScreen);
}

//DragonZ:跑道移到RoomParentDlg
//void CRoomMidDlg::ReloadGiftData()
//{
//	if (m_pPaodaoStatic != NULL)
//	{
//		m_pPaodaoStatic->UpdateGiftList();
//	}
//}

void CRoomMidDlg::GetSendMsgRc(CRect& rcSendMsg)
{
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"itemHtmlInputMsg", &(rcSendMsg.left), &(rcSendMsg.top), &(rcSendMsg.right), &(rcSendMsg.bottom));
	CRect rcSendBtnRc(0,0,0,0);
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"imgSendMsgBk", &(rcSendBtnRc.left), &(rcSendBtnRc.top), &(rcSendBtnRc.right), &(rcSendBtnRc.bottom));
	rcSendMsg.right += rcSendBtnRc.Width();
	ClientToScreen(&rcSendMsg);
}

void CRoomMidDlg::SetPillowChatCheckBox(BOOL bCheck)
{
	((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->SetCheck(bCheck);
}

void CRoomMidDlg::OnBnClickedCheckRoomFlytext()
{
	// TODO: 在此添加控件通知处理程序代码
	core::CurInfo myCurInfo = GetCurInfo();
	if (myCurInfo.enmVIPLevel == core::VIPLEVEL_NONE)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ROOM_FLYTEXT))->SetCheck(FALSE);
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能在房间发飞屏，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pParentDlg->GetRoomBusiness()->GetRegisteUrl() ); 

		return;
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_ROOM_FLYTEXT))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_CHECK_ROOM_PILLOWCHAT))->SetCheck(FALSE);
	}
}

void CRoomMidDlg::OnClickBtnMore()
{
	if (m_pTheMoreDlg == NULL)
	{
		SwitchResourceToModule(_T("Room"));
		m_pTheMoreDlg = new CTheMoreDlg(m_pParentDlg);
		m_pTheMoreDlg->Create(CTheMoreDlg::IDD, this);
		m_pTheMoreDlg->ShowWindow(SW_HIDE);
	}

	if (m_pTheMoreDlg->IsWindowVisible())
	{
		m_pTheMoreDlg->ShowWindow(SW_HIDE);
	}else
	{
		CRect rcBtnMore(0, 0, 0, 0), rcMoreDlg(0,0,0,0);
		m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(), L"btnMore", &(rcBtnMore.left), &(rcBtnMore.top), &(rcBtnMore.right), &(rcBtnMore.bottom));
		ClientToScreen(&rcBtnMore);
		m_pTheMoreDlg->GetClientRect(&rcMoreDlg);
		m_pTheMoreDlg->MoveWindow(rcBtnMore.right - rcMoreDlg.Width(), rcBtnMore.top - rcMoreDlg.Height(), rcMoreDlg.Width(), rcMoreDlg.Height());
		m_pTheMoreDlg->ShowWindow(SW_SHOWNOACTIVATE);
	}
}

void CRoomMidDlg::OnSize( UINT nType, int cx, int cy )
{
	if (m_pTheMoreDlg != NULL && m_pTheMoreDlg->GetSafeHwnd() != NULL)
	{
		if (m_pTheMoreDlg->IsWindowVisible())
		{
			m_pTheMoreDlg->ShowWindow(SW_HIDE);
		}
	}
}

LRESULT CRoomMidDlg::OnInputMsgFocusIn( WPARAM wParam ,LPARAM lParam )
{
	if ((COutputHtmlEditImpl*)(wParam) != m_pInputHtml)//聊天输入框
		return FALSE;
	
	OutputDebugString(L"\n##################  FocusIn  ###################\n");
	CString strOutPutStr;
	m_InputLogic.GetOutputStringWithoutPic(strOutPutStr);
	if (strOutPutStr.Compare(DEFAULT_ROOMMID_INPUT_TEXT) == 0)
	{
		m_pInputHtml->ClearMsg();
	}
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSendMsg", L"disabled", (CComVariant)FALSE);
	return TRUE;
}

LRESULT CRoomMidDlg::OnInputMsgFocusOut( WPARAM wParam ,LPARAM lParam )
{
	if ((COutputHtmlEditImpl*)(wParam)  != m_pInputHtml)//聊天输入框
		return FALSE;
	
	CString strOutPutStr, strOutPUtPic;
	m_InputLogic.GetOutputString(strOutPutStr, strOutPUtPic);
	
	if (strOutPutStr.Compare(L"") != 0 || strOutPUtPic.Compare(L"") != 0)
		return FALSE;
	
	OutputDebugString(L"\n##################  FocusOut  ###################\n");

	CString strTemp = DEFAULT_ROOMMID_INPUT_TEXT;
	m_pParentDlg->GetHtmlOutPutManager()->InsertRoomMidInputDefaultText(strTemp);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSendMsg", L"disabled", (CComVariant)TRUE);
	return TRUE;
}

void CRoomMidDlg::InsertInputMsgDefaultText( CString& strText )
{
	m_pInputHtml->InsertAtSelectionArea(strText);
}

void CRoomMidDlg::CleanPublicOutput()
{
	m_pPublicHtml->ClearMsg();
}

void CRoomMidDlg::CleanPrivateOutput()
{
	m_pPrivateHtml->ClearMsg();
}

LRESULT CRoomMidDlg::OnClickMsgBoxGoToRegister( WPARAM wParam, LPARAM lParam )
{
	if ((CRoomMidDlg*)wParam == this)
	{
		m_pParentDlg->OnClickGotoRegister();
	}

	return TRUE;
}

void CRoomMidDlg::OnModifyFont()
{
	m_pParentDlg->GetHtmlOutPutManager()->OnModifyFont();
}