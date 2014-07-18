// WishSpeakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "WishSpeakerDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\emotion\EmotionDlg.h"
#include "SpeakerMsg.h"
#include "..\userlist\UserListMenu.h"
#include "..\shortcut\SelfDefSentenceDlg.h"
#include "utility\StringHelper.h"
#include "..\gift\GiftManager.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\usermanager\UserManager.h"
#include "..\UserMessageDefine.h"
#include "..\SendEventManager.h"
#include "..\roomsetting\RoomOption.h"

#define  SKIN_ON_CLICK_CLOSE          1
#define  SKIN_ON_CLICK_SEND           2
#define  SKIN_ON_CLICK_COLOR          3
#define  SKIN_ON_CLICK_WISH           4
#define  SKIN_ON_CLICK_EMOTION        5

#define DEFAULT_WISH_WORDS	L"我在%d房间中点播歌曲一首赠予（请填写对方昵称或账号），愿幸福常伴左右。"
#define MAX_WISH_INPUT_LEN	100
#define CHECK_INPUT_LIMIT	500
// CWishSpeakerDlg 对话框

IMPLEMENT_DYNAMIC(CWishSpeakerDlg, CDialog)

CWishSpeakerDlg::CWishSpeakerDlg(CRoomParentDlg* pParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CWishSpeakerDlg::IDD, pParent),
	m_pParentDlg(pParentDlg),
	m_SpeakerInputLogic(static_cast<IHtmlSink*>(this))
{
	m_pSkinEngine = NULL;
	m_pEmotionDlg = NULL;
	m_nLeft = 100;
	m_strInitContent = L"";
    ::GetSkinEngine(&m_pSkinEngine);
}

CWishSpeakerDlg::~CWishSpeakerDlg()
{
}

void CWishSpeakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWishSpeakerDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_CLICK_EMOTION , OnClickEmotionList)
	ON_MESSAGE(WM_CLICK_SHORTCUT, OnClickShortCutMsg)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWishSpeakerDlg 消息处理程序

BOOL CWishSpeakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	m_pSpeakerInputHtml = new COutInputHtmlEditImpl ;
	m_pSpeakerInputHtml->SetHtmlEditLogic( &m_SpeakerInputLogic ) ;
	m_pSpeakerInputHtml->Create( this ,IDC_HTML_WISHSPEAKER_INPUT , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE) ;
	CComVariant var = (long)m_pSpeakerInputHtml->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlWishSpeaker",L"hwnd",var);

	if (m_strInitContent == L"")
	{
		CString strContent;
		strContent.Format(DEFAULT_WISH_WORDS, m_pParentDlg->GetRoomID());
		SetInitContent(strContent);
	}

	SetTimer(CHECK_INPUT_LIMIT, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CWishSpeakerDlg::PreTranslateMessage(MSG* pMsg)
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

int CWishSpeakerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"WishSpeakerDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CWishSpeakerDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
	case SKIN_ON_CLICK_CLOSE:
		{
           OnClickClose();
		}
		break;
	case SKIN_ON_CLICK_COLOR:
		{
           OnClickColor();
		}
		break;
	case SKIN_ON_CLICK_EMOTION:
		{
           OnClickEmotion();
		}
		break;
	case SKIN_ON_CLICK_SEND:
		{
           OnClickSend();
		}
		break;
	case SKIN_ON_CLICK_WISH:
		{
           OnClickWish();
		}
		break;
	default:break;
    }

	return TRUE;
}

void CWishSpeakerDlg::OnClickClose()
{
	//DestroyWindow();//可改。这里要销毁还是隐藏取决于使用
	m_SpeakerInputLogic.ClearInput();
	ShowWindow(SW_HIDE);
	KillTimer(CHECK_INPUT_LIMIT);
}

void CWishSpeakerDlg::OnClickColor()
{
	CColorDialog ColorDlg(m_SpeakerInputLogic.GetFontInfo().font_color );
	if ( ColorDlg.DoModal() == IDOK )
	{
		m_SpeakerInputLogic.SetFontColor(ColorDlg.GetColor());
		m_pSpeakerInputHtml->UpdateMy3();
	}
}

void CWishSpeakerDlg::OnClickEmotion()
{
	m_pParentDlg->OnClickEmotion( this ) ;
}

void CWishSpeakerDlg::OnClickSend()
{
	ASSERT(m_pParentDlg != NULL);
	ASSERT(m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() != NULL);
	if ( m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() == NULL )
	{
		return ;
	}
	CString strText;
	m_pSpeakerInputHtml->GetContent(strText);
	m_pParentDlg->GetHtmlOutPutManager()->ProcessSendMsgText( strText ) ;
	std::wstring strContent = strText.GetBuffer();
	CSpeakerMsg::EncodeMessage(strContent, m_SpeakerInputLogic.GetFontInfo(), m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem(),
		m_pParentDlg->GetRoomOption()->GetRoomName() ,m_pParentDlg->GetRoomID() ,true );

	IGiftItem * pGiftItem = GiftManager::GetInstance()->GetWishSongItem();
	ASSERT(pGiftItem != NULL);
	CSendEventManager::SendSpeakerEvent(m_pParentDlg->GetRoomID(), pGiftItem->GetID(), core::LOUDSPEAKERTYPE_SMALL, strContent);
	m_SpeakerInputLogic.ClearInput();

	ShowWindow(SW_HIDE);
}

void CWishSpeakerDlg::OnClickWish()
{
	SwitchResourceToModule(_T("Room"));
	ShortCutMsgMenu ShortCutMenu(this) ;
	ShortCutMenu.PopupMenu() ;
}

bool CWishSpeakerDlg::OnInput( int nLen )
{
	int nCount = m_SpeakerInputLogic.GetPictureNum();
	if (nLen + nCount > MAX_WISH_INPUT_LEN)
	{
		return false;
	}

	//CComVariant var = MAX_WISH_INPUT_LEN - nLen;
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPrompt2",L"text",var);

	return true;
}

LRESULT CWishSpeakerDlg::OnClickEmotionList( WPARAM wParam ,LPARAM lParam )
{
	CString strFilePath = (LPCTSTR)lParam ;
	int nCount = m_SpeakerInputLogic.GetPictureNum();
	if (nCount >= 3)
	{
		CComVariant var = TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textEmotionPrompt", L"visible", var);
		return S_OK;
	}
	m_SpeakerInputLogic.OnCustomEmotion( m_pSpeakerInputHtml , strFilePath.GetBuffer() ) ;
	return S_OK ;
}


LRESULT CWishSpeakerDlg::OnClickShortCutMsg( WPARAM wParam ,LPARAM lParam )
{
	int nIndex = lParam ;
	if ( nIndex == 0 )
	{
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
			m_pSpeakerInputHtml->InsertText(strShortCutMsg);
		}
	}	
	return S_OK;
}

void CWishSpeakerDlg::SetInitContent( CString strContent )
{
	//m_SpeakerInputLogic.ClearInput();
	m_strInitContent = strContent;
	m_SpeakerInputLogic.SetInitText(strContent);
}

void CWishSpeakerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		CString strContent;
		strContent.Format(DEFAULT_WISH_WORDS, m_pParentDlg->GetRoomID());
		m_SpeakerInputLogic.ClearInput();
		m_pSpeakerInputHtml->InsertText(strContent);
		SetTimer(CHECK_INPUT_LIMIT, 500, NULL);
	}
}

void CWishSpeakerDlg::CheckInputLimit()
{
	CString strText = m_pSpeakerInputHtml->GetContentText();
	strText.Trim();
	int nCount = m_SpeakerInputLogic.GetPictureNum();
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	int nLeft = MAX_WISH_INPUT_LEN - nLen - nCount;


	if (nLeft < 0)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)TRUE);
	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)FALSE);
	}

	if (nLeft != m_nLeft)
	{
		if (nLeft >= 0)
		{
			CString strPrompt;
			strPrompt.Format(L"您还可以输入%d个字符", nLeft);
			CComVariant var = strPrompt;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrompt", L"text", var);

			if (nCount < 3)
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textEmotionPrompt", L"visible", var);
			}

			m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", &var);
			if (var.boolVal)
			{
				var = FALSE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", var);
			}
		}
		else
		{
			CComVariant var = L"字数超出范围了";
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrompt", L"text", var);

			m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", &var);
			if (!var.boolVal)
			{
				var = TRUE;
				m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", var);
			}
		}
		m_nLeft = nLeft;
	}
}
void CWishSpeakerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (CHECK_INPUT_LIMIT == nIDEvent)
	{
		CheckInputLimit();
	}

	__super::OnTimer(nIDEvent);
}

void CWishSpeakerDlg::SetContent( CString strContent )
{
	m_SpeakerInputLogic.ClearInput();
	m_pSpeakerInputHtml->InsertText(strContent);
}