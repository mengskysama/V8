// SpeakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SpeakerDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "SpeakerMsg.h"
#include "utility\StringHelper.h"
#include "..\..\HTMLEdit\FilterHtmlEdit.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\emotion\EmotionDlg.h"
#include "..\UserMessageDefine.h"
#include "..\userlist\UserListMenu.h"
#include "..\shortcut\SelfDefSentenceDlg.h"
#include "..\gift\GiftManager.h"
#include "..\RoomParentDlg.h"
#include "..\roomsetting\RoomOption.h"
#include "..\SendEventManager.h"

#define SKIN_ON_CLICK_CLOSE 1 
#define SKIN_ON_CLICK_SEND  2
#define SKIN_ON_CLICK_COLOR 3
#define SKIN_ON_CLICK_INSERTROOM 4
#define SKIN_ON_CLICK_EMOTION 5
#define SKIN_ON_CLICK_WISH	6

#define MAX_SPEAKER_INPUT_LEN	100
#define CHECK_INPUT_LIMIT		500
// CSpeakerDlg 对话框

IMPLEMENT_DYNAMIC(CSpeakerDlg, CDialog)

CSpeakerDlg::CSpeakerDlg(CRoomParentDlg* pParentDlg , core::ENM_LoudSpeakerType enmType, CWnd* pParent /*=NULL*/)
	: CDialog(CSpeakerDlg::IDD, pParent),
	m_pParentDlg(pParentDlg),
	m_enmSpeakerType(enmType),
	m_SpeakerInputLogic(static_cast<IHtmlSink*>(this))
{
     m_pSkinEngine = NULL;
	 m_pSpeakerInputHtml = NULL;
	 m_enmSpeakerType = core::LOUDSPEAKERTYPE_SMALL;
	 m_bCanInput = true;
	 m_unSpeakerID = 0;
	 m_nLeft = 100;
	 ::GetSkinEngine(&m_pSkinEngine);
}

CSpeakerDlg::~CSpeakerDlg()
{
}

void CSpeakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_SMALL_SPEAKER, m_radioSmallSpeaker);
}


BEGIN_MESSAGE_MAP(CSpeakerDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_BN_CLICKED(IDC_RADIO_SMALL_SPEAKER, &CSpeakerDlg::OnBnClickedRadioSmallSpeaker)
	ON_BN_CLICKED(IDC_RADIO_BIG_SPEAKER, &CSpeakerDlg::OnBnClickedRadioBigSpeaker)
	ON_BN_CLICKED(IDC_RADIO_SUPER_SPEAKER, &CSpeakerDlg::OnBnClickedRadioSuperSpeaker)
	ON_MESSAGE(WM_CLICK_EMOTION , OnClickEmotionList)
	ON_MESSAGE(WM_CLICK_SHORTCUT, OnClickShortCutMsg)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSpeakerDlg 消息处理程序

int CSpeakerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SpeakerDlg");
		ASSERT(hr== S_OK && __LINE__);
	}
   
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CSpeakerDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			m_SpeakerInputLogic.ClearInput();
			ShowWindow(SW_HIDE);
			//KillTimer(CHECK_INPUT_LIMIT);
		}
		break;
	case SKIN_ON_CLICK_SEND:
		{
			OnClickSend();
		}
		break;
	case SKIN_ON_CLICK_COLOR:
		{
			OnClickColor();
		}
		break;
	case SKIN_ON_CLICK_INSERTROOM:
		{
			OnClickInsertRoom();
		}
		break;
	case SKIN_ON_CLICK_EMOTION:
		{
			OnClickEmotion();
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

BOOL CSpeakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_TOOLWINDOW);
	//m_radioSmallSpeaker.SetCheck(TRUE);
	CheckRadioButton(IDC_RADIO_SMALL_SPEAKER, IDC_RADIO_SUPER_SPEAKER, IDC_RADIO_SMALL_SPEAKER);

	//喇叭输入区
	m_pSpeakerInputHtml = new COutInputHtmlEditImpl ;
	m_pSpeakerInputHtml->SetHtmlEditLogic( &m_SpeakerInputLogic ) ;
	m_pSpeakerInputHtml->Create( this ,IDC_HTML_SPEAKER_INPUT , CRect(0,0,0,0) , WS_CHILD|WS_VISIBLE) ;
	CComVariant var = (long)m_pSpeakerInputHtml->GetSafeHwnd() ;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlSpeakerInput",L"hwnd",var);

	SetType(m_enmSpeakerType);
	//var = vecSpeakerList[0]->GetDesc().c_str();
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
	//m_enmSpeakerType = core::LOUDSPEAKERTYPE_SMALL;
	//m_unSpeakerID = vecSpeakerList[0]->GetID();
	//var = TRUE;
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);


	SetTimer(CHECK_INPUT_LIMIT, 500, NULL);

	//InitBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSpeakerDlg::OnBnClickedRadioSmallSpeaker()
{
	// TODO: 在此添加控件通知处理程序代码
	//CComVariant var = L"10000币/条，所有分区房间能收到！";
	if (!((CButton*)GetDlgItem(IDC_RADIO_SMALL_SPEAKER))->GetCheck())
	{
		return;
	}
	VecGiftItem vecSpeakerList = GiftManager::GetInstance()->GetSpeakerList();
	ASSERT(vecSpeakerList.size() >= 3);
	CComVariant var = vecSpeakerList[0]->GetDesc().c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
	m_enmSpeakerType = core::LOUDSPEAKERTYPE_SMALL;
	m_unSpeakerID = vecSpeakerList[0]->GetID();
	var = TRUE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);
	var = FALSE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTipMid", L"visible", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTipRight", L"visible", var);
	var = TRUE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTip", L"visible", var);
	

	CString strContent;
	m_pSpeakerInputHtml->GetContent(strContent);
	m_pParentDlg->GetHtmlOutPutManager()->ProcessSendMsgText(strContent, false);
	m_SpeakerInputLogic.ClearInput();
	m_pSpeakerInputHtml->InsertText(strContent);
	strContent = m_pSpeakerInputHtml->GetContentText();
	m_pParentDlg->GetHtmlOutPutManager()->ProcessRecvMsgText(strContent, false);
	m_SpeakerInputLogic.ClearInput();
	m_pSpeakerInputHtml->InsertText(strContent);

	//CSpeakerMsg::DisableRoomShortcut(strContent);

	//m_SpeakerInputLogic.ClearInput();
	//m_pSpeakerInputHtml->InsertText(strContent);
}

void CSpeakerDlg::OnBnClickedRadioBigSpeaker()
{
	// TODO: 在此添加控件通知处理程序代码
	//ComVariant var = L"40000币/条，所有房间能收到！";
	if (!((CButton*)GetDlgItem(IDC_RADIO_BIG_SPEAKER))->GetCheck())
	{
		return;
	}
	VecGiftItem vecSpeakerList = GiftManager::GetInstance()->GetSpeakerList();
	ASSERT(vecSpeakerList.size() >= 3);
	CComVariant var = vecSpeakerList[1]->GetDesc().c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
	m_enmSpeakerType = core::LOUDSPEAKERTYPE_BIG;
	m_unSpeakerID = vecSpeakerList[1]->GetID();
	var = TRUE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);
	var = FALSE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTip", L"visible", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTipRight", L"visible", var);
	var = TRUE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTipMid", L"visible", var);
	

	CString strContent;
	m_pSpeakerInputHtml->GetContent(strContent);
	m_pParentDlg->GetHtmlOutPutManager()->ProcessSendMsgText(strContent, false);
	m_SpeakerInputLogic.ClearInput();
	m_pSpeakerInputHtml->InsertText(strContent);
	strContent = m_pSpeakerInputHtml->GetContentText();
	m_pParentDlg->GetHtmlOutPutManager()->ProcessRecvMsgText(strContent, false);
	m_SpeakerInputLogic.ClearInput();
	m_pSpeakerInputHtml->InsertText(strContent);

	//CSpeakerMsg::DisableRoomShortcut(strContent);

	//m_SpeakerInputLogic.ClearInput();
	//m_pSpeakerInputHtml->InsertText(strContent);
}

void CSpeakerDlg::OnBnClickedRadioSuperSpeaker()
{
	// TODO: 在此添加控件通知处理程序代码
	//CComVariant var = L"60000币/条，所有房间能收到！";
	if (!((CButton*)GetDlgItem(IDC_RADIO_SUPER_SPEAKER))->GetCheck())
	{
		return;
	}
	VecGiftItem vecSpeakerList = GiftManager::GetInstance()->GetSpeakerList();
	ASSERT(vecSpeakerList.size() >= 3);
	CComVariant var = vecSpeakerList[2]->GetDesc().c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
	m_enmSpeakerType = core::LOUDSPEAKERTYPE_SUPER;
	m_unSpeakerID = vecSpeakerList[2]->GetID();
	var = FALSE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTip", L"visible", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTipMid", L"visible", var);
	var = TRUE;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgTipRight", L"visible", var);
}


LRESULT CSpeakerDlg::OnClickEmotionList( WPARAM wParam ,LPARAM lParam ) 
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

bool CSpeakerDlg::OnInput( int nContenLen )
{
	int nCount = m_SpeakerInputLogic.GetPictureNum();
	if (nContenLen + nCount >= MAX_SPEAKER_INPUT_LEN)
	{
		return false;
	}
	
	//int nleft = MAX_SPEAKER_INPUT_LEN - nContenLen - nCount;
	//CString strPrompt;
	//strPrompt.Format(L"你还可以输入%d个字符", nleft);
	//CComVariant var = strPrompt;
	//m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrompt", L"text", var);

	return true;
}

BOOL CSpeakerDlg::PreTranslateMessage(MSG* pMsg)
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

void CSpeakerDlg::OnClickSend()
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
	CSpeakerMsg::EncodeMessage(strContent, m_SpeakerInputLogic.GetFontInfo(), m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() ,
		m_pParentDlg->GetRoomOption()->GetRoomName() , m_pParentDlg->GetRoomID() );

	CSendEventManager::SendSpeakerEvent(m_pParentDlg->GetRoomID(), m_unSpeakerID, m_enmSpeakerType, strContent);
	m_SpeakerInputLogic.ClearInput();

	ShowWindow(SW_HIDE);
}

void CSpeakerDlg::OnClickColor()
{
	CColorDialog ColorDlg(m_SpeakerInputLogic.GetFontInfo().font_color );
	if ( ColorDlg.DoModal() == IDOK )
	{
		m_SpeakerInputLogic.SetFontColor(ColorDlg.GetColor());
		m_pSpeakerInputHtml->UpdateMy3();
	}
}

void CSpeakerDlg::OnClickInsertRoom()
{
	CString strTemp = CSpeakerMsg::GetRoomShortCut(m_pParentDlg->GetRoomID(), m_pParentDlg->GetRoomBusiness()->GetRoomOption()->GetRoomName());
	m_pSpeakerInputHtml->InsertText(strTemp);
}

void CSpeakerDlg::SetInitContent( CString strContent )
{
	//m_SpeakerInputLogic.ClearInput();
	m_SpeakerInputLogic.SetInitText(strContent);
}

void CSpeakerDlg::CheckInputLimit()
{
	CString strText = m_pSpeakerInputHtml->GetContentText();
	//strText.Trim();
	CHtmlFilter::DelLink(strText);
	int nCount = m_SpeakerInputLogic.GetPictureNum();
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	int nLeft = MAX_SPEAKER_INPUT_LEN - nLen - nCount;

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

	CString strTemp = strText;
	strTemp.Trim();
	if ( nCount == 0 && common::utility::stringhelper::CalcStringAsciiLength(strTemp.GetBuffer(0)) == 0)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"op_send", L"disabled", (CComVariant)TRUE);
	}
}

void CSpeakerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (CHECK_INPUT_LIMIT == nIDEvent)
	{
		CheckInputLimit();
	}

	CDialog::OnTimer(nIDEvent);
}

void CSpeakerDlg::OnClickEmotion()
{
	m_pParentDlg->OnClickEmotion( this ) ;
}

void CSpeakerDlg::OnClickWish()
{
	SwitchResourceToModule(_T("Room"));
	ShortCutMsgMenu ShortCutMenu(this) ;
	ShortCutMenu.PopupMenu() ;
}

LRESULT CSpeakerDlg::OnClickShortCutMsg( WPARAM wParam ,LPARAM lParam )
{
	int nIndex = lParam ;
	if ( nIndex == 0 )
	{
		return S_OK ;
	}
	else if ( nIndex == SELF_DEFINE_SHORCUT )
	{
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

void CSpeakerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetTimer(CHECK_INPUT_LIMIT, 500, NULL);
		CheckRadioButton(IDC_RADIO_SMALL_SPEAKER, IDC_RADIO_SUPER_SPEAKER, IDC_RADIO_SMALL_SPEAKER);
		OnBnClickedRadioSmallSpeaker();
	}
	else
	{
		KillTimer(CHECK_INPUT_LIMIT);
	}
}

void CSpeakerDlg::SetContent( CString strContent )
{
	m_SpeakerInputLogic.ClearInput();
	m_pSpeakerInputHtml->InsertText(strContent);
}

void CSpeakerDlg::SetType( core::ENM_LoudSpeakerType enmType )
{
	m_enmSpeakerType = enmType;
	VecGiftItem vecSpeakerList = GiftManager::GetInstance()->GetSpeakerList();
	ASSERT(vecSpeakerList.size() >= 3);
	CComVariant var;
	switch (m_enmSpeakerType)
	{
	case core::LOUDSPEAKERTYPE_SMALL:
		{
			CheckRadioButton(IDC_RADIO_SMALL_SPEAKER, IDC_RADIO_SUPER_SPEAKER, IDC_RADIO_SMALL_SPEAKER);
			var = vecSpeakerList[0]->GetDesc().c_str();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
			m_unSpeakerID = vecSpeakerList[0]->GetID();
			var = TRUE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);
		}
		break;
	case core::LOUDSPEAKERTYPE_BIG:
		{
			CheckRadioButton(IDC_RADIO_SMALL_SPEAKER, IDC_RADIO_SUPER_SPEAKER, IDC_RADIO_BIG_SPEAKER);
			var = vecSpeakerList[1]->GetDesc().c_str();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
			m_unSpeakerID = vecSpeakerList[1]->GetID();
			var = TRUE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);
		}
		break;
	case core::LOUDSPEAKERTYPE_SUPER:
		{
			CheckRadioButton(IDC_RADIO_SMALL_SPEAKER, IDC_RADIO_SUPER_SPEAKER, IDC_RADIO_SUPER_SPEAKER);
			var = vecSpeakerList[2]->GetDesc().c_str();
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTip1",L"text",var);
			m_unSpeakerID = vecSpeakerList[2]->GetID();
			var = FALSE;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnInsertRoom", L"disabled", var);
		}
		break;
	default:
		break;
	}

}