// UserMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "UserMgrDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "utility/StringHelper.h"
#include "utility/expat++/expat_document.h"
#include "..\..\CommonLibrary\include\utility\FileHelper.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"
#include "..\emotion\EmotionDlg.h"
#include "..\RoomParentDlg.h"
#include "RoomOption.h"
#include "..\UserMessageDefine.h"

#define SKIN_ON_CLICK_BTN_CONFIRM_SAVE		  1
#define SKIN_ON_CLICK_JIACU                   2
#define SKIN_ON_CLICK_XIETI                   3
#define SKIN_ON_CLICK_UNDERLINE               4
#define SKIN_ON_CLICK_FONT_COLOR              5
#define SKIN_ON_CLICK_BTN_EMOTION             6

// CUserMgrDlg 对话框

IMPLEMENT_DYNAMIC(CUserMgrDlg, CDialog)

CUserMgrDlg::CUserMgrDlg( CRoomParentDlg* pParentDlg , CWnd* pParent /*=NULL*/)
	: CDialog(CUserMgrDlg::IDD, pParent),
	m_pParentDlg(pParentDlg),
	m_InputLogic(static_cast<IHtmlSink*>(this))
{
	m_pSkinEngine = NULL;
	m_pInputHtml = NULL;
	m_pEmotionDlg = NULL ;
	m_bInitDataed = FALSE ;
	::GetSkinEngine(&m_pSkinEngine);
}

CUserMgrDlg::~CUserMgrDlg()
{
}

void CUserMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WELCOME_FONTSIZE, m_FontSizeCombo);
	DDX_Control(pDX, IDC_COMBO_WELCOME_FONT, m_FontCombo);
}


BEGIN_MESSAGE_MAP(CUserMgrDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_CBN_SELCHANGE(IDC_COMBO_WELCOME_FONTSIZE, &CUserMgrDlg::OnCbnSelchangeComboWelcomeFontsize)
	ON_CBN_SELCHANGE(IDC_COMBO_WELCOME_FONT, &CUserMgrDlg::OnCbnSelchangeComboWelcomeFont)
	ON_BN_CLICKED(IDC_RADIO_USER_SYS_WELCOME_WORD, &CUserMgrDlg::OnBnClickedRadioUserSysWelcomeWord)
	ON_BN_CLICKED(IDC_RADIO_USE_DEFINE_WELCOME_WORD, &CUserMgrDlg::OnBnClickedRadioUseDefineWelcomeWord)
	ON_MESSAGE(WM_CLICK_EMOTION , OnClickEmotionList)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CUserMgrDlg 消息处理程序

int CUserMgrDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"UserMgrDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

LRESULT CUserMgrDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_CONFIRM_SAVE:
		{
			OnClickSave();
		}
		break;
	case SKIN_ON_CLICK_JIACU:
		{
			if (1 == lParam)//选择加粗
			{
				m_InputLogic.SetFontBold(1);
			}
			else//取消加粗
			{
				m_InputLogic.SetFontBold(0);
			}

			m_pInputHtml->UpdateMy3();
			NotifyParentModify();
		}
		break;
	case SKIN_ON_CLICK_XIETI:
		{
			if (1 == lParam)//选择斜体
			{
				m_InputLogic.SetFontItalic(1);
			}
			else//取消斜体
			{
				m_InputLogic.SetFontItalic(0);
			}

			m_pInputHtml->UpdateMy3();
			NotifyParentModify();
		}
		break;
	case SKIN_ON_CLICK_UNDERLINE:
		{
			if (1 == lParam)//选择下划线
			{
				m_InputLogic.SetFontUnderLine(1);
			}
			else//取消下划线
			{
				m_InputLogic.SetFontUnderLine(0);
			}

			m_pInputHtml->UpdateMy3();
			NotifyParentModify();
		}
		break;
	case SKIN_ON_CLICK_FONT_COLOR:
		{
			CColorDialog ColorDlg(GetFontInfo().font_color );
			if ( ColorDlg.DoModal() == IDOK )
			{
				m_InputLogic.SetFontColor(ColorDlg.GetColor());
				m_pInputHtml->UpdateMy3();
			}
			NotifyParentModify();
		}
		break;
	case SKIN_ON_CLICK_BTN_EMOTION:
		{
			OnClickEmotion();
		}
		break;
	default:break;
	}

	return TRUE;
}


BOOL CUserMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pInputHtml = new COutInputHtmlEditImpl;
	m_pInputHtml->SetHtmlEditLogic(&m_InputLogic);
	m_pInputHtml->Create(this, IDC_HTML_WELCOME_INPUT, CRect(0,0,0,0), WS_CHILD|WS_VISIBLE);
	CComVariant var = (long)m_pInputHtml->GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemHtmlContent", L"hwnd", var);
	InitFontPanel();	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUserMgrDlg::InitFontPanel()
{
	for ( int i = FONT_SIZE_MIN ; i<= FONT_SIZE_MAX ; ++i )
	{
		CString strTemp;
		strTemp.Format(_T("%d") ,i ) ;
		int nindex = m_FontSizeCombo.AddString( strTemp ) ;
		if ( i == GetFontInfo().font_size )
		{
			m_FontSizeCombo.SetCurSel( nindex ) ;
		}
	}
	wstring wsCurrentFont = GetFontInfo().font_type ;
	vector<wstring> vFontList ;
	common::utility::systemhelper::GetChineseFontNameList( vFontList ) ;
	vector<wstring>::iterator iter ;
	for ( iter = vFontList.begin() ; iter != vFontList.end() ; ++iter )
	{
		int nIndex = m_FontCombo.AddString( (*iter).c_str() ) ;
		if ( wsCurrentFont == (*iter) )
		{
			m_FontCombo.SetCurSel( nIndex ) ;
		}
	}
}


core::MessageFont CUserMgrDlg::GetFontInfo()
{
	return m_InputLogic.GetFontInfo();
}

void CUserMgrDlg::OnCbnSelchangeComboWelcomeFontsize()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_FontSizeCombo.GetCurSel() ;
	if ( nIndex != CB_ERR )
	{
		CString strTmep ;
		//m_FontSizeCombo.GetWindowText( strTmep ) ;
		m_FontSizeCombo.GetLBText(nIndex, strTmep);
		int nFontSize = _wtoi( strTmep) ;
		if ( nFontSize != GetFontInfo().font_size )
		{
			m_InputLogic.SetFontSize(nFontSize) ;
			m_pInputHtml->UpdateMy3() ;
		}
	}

	NotifyParentModify();
}

void CUserMgrDlg::OnCbnSelchangeComboWelcomeFont()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_FontCombo.GetCurSel() ;
	if ( nIndex != CB_ERR )
	{
		CString strTmep;
		m_FontCombo.GetWindowText( strTmep );
		CString strFontName( GetFontInfo().font_type.c_str() ) ;
		if ( strTmep != strFontName )
		{
			m_InputLogic.SetFontName(strTmep);
			m_pInputHtml->UpdateMy3();
		}
	}

	NotifyParentModify();
}


BOOL CUserMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return TRUE;
			}
			break;
		case VK_RETURN:
			{
				OnClickSave();
				return TRUE;
			}
			break;
		default: break;
		}	
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CUserMgrDlg::OnBnClickedRadioUserSysWelcomeWord()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableOwnDefine(FALSE);
	NotifyParentModify();
}

void CUserMgrDlg::OnBnClickedRadioUseDefineWelcomeWord()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableOwnDefine(TRUE);
	NotifyParentModify();
}

LRESULT CUserMgrDlg::OnClickEmotionList( WPARAM wParam ,LPARAM lParam ) 
{
	CString strFilePath = (LPCTSTR)lParam ;
	m_InputLogic.OnCustomEmotion( m_pInputHtml , strFilePath.GetBuffer() ) ;
	return S_OK ;
}

void CUserMgrDlg::InitDlgData()
{
	RoomOption* pRoomOption = m_pParentDlg->GetRoomBusiness()->GetRoomOption() ;
	if ( pRoomOption == NULL)
	{
		return;
	}
	
	IUserItem* pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() ;
	if ( pSelfItem == NULL)
	{
		return;
	}
	
	m_bInitDataed = FALSE;
	//bool bAutoWelcome = m_pRoomDlg->GetRoomOption()->IsAutoWelcome();
	//if (bAutoWelcome)
	//{
	//	((CButton*)GetDlgItem(IDC_CHECK_AUTO_SEND))->SetCheck(TRUE);
	//}
	//else
	//{
	//	((CButton*)GetDlgItem(IDC_CHECK_AUTO_SEND))->SetCheck(FALSE);
	//}

	std::wstring strWelcomeWords = pRoomOption->GetWelcomeWords();

	if (strWelcomeWords != L"" && strWelcomeWords != DEFAULT_WELCOME_WORDS)
	{
		((CButton*)GetDlgItem(IDC_RADIO_USE_DEFINE_WELCOME_WORD))->SetCheck(TRUE);
		m_InputLogic.ClearInput();
		m_pInputHtml->InsertText(strWelcomeWords.c_str());
		SetFontInfo( pRoomOption->GetWelcomeFontInfo());
		EnableOwnDefine(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_USER_SYS_WELCOME_WORD))->SetCheck(TRUE);
		m_InputLogic.ClearInput();
		std::wstring strSelfWords;
		core::MessageFont stFont;
		if (LoadWelcomeWords(strSelfWords, stFont))
		{
			m_pInputHtml->InsertText(strSelfWords.c_str(), false);
			SetFontInfo(stFont);
		}
		else
		{
			m_pInputHtml->InsertText(DEFAULT_WELCOME_WORDS);
		}
		EnableOwnDefine(FALSE);
	}

	if ( pSelfItem->GetUserTitleLevel() < core::TITLELEVEL_OWNER)
	{
		((CButton*)GetDlgItem(IDC_RADIO_USER_SYS_WELCOME_WORD))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_USE_DEFINE_WELCOME_WORD))->EnableWindow(FALSE);
		EnableOwnDefine(FALSE);
	}

	m_bInitDataed = TRUE;
}

void CUserMgrDlg::EnableOwnDefine( BOOL bOwn )
{
	m_pInputHtml->EnableWindow(bOwn);
	if (bOwn)
	{
		m_pInputHtml->SetEditAble();
	}
	else
	{
		m_pInputHtml->SetUnEditAble();
	}
	GetDlgItem(IDC_COMBO_WELCOME_FONTSIZE)->EnableWindow(bOwn);
	GetDlgItem(IDC_COMBO_WELCOME_FONT)->EnableWindow(bOwn);
	CComVariant var = !bOwn;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnJiaCu", L"disabled", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnXieTi", L"disabled", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnUnderLine", L"disabled", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnColor", L"disabled", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnEmotion", L"disabled", var);
}

void CUserMgrDlg::OnClickSave()
{
	RoomOption* pRoomOption = m_pParentDlg->GetRoomBusiness()->GetRoomOption() ;
	if ( pRoomOption == NULL)
	{
		return;
	}

	//bool bAutoWelcome = (((CButton*)GetDlgItem(IDC_CHECK_AUTO_SEND))->GetCheck() == TRUE);

	CString strWelcomeWords;
	bool bDefault = true;
	if (((CButton*)GetDlgItem(IDC_RADIO_USER_SYS_WELCOME_WORD))->GetCheck())
	{
		strWelcomeWords = DEFAULT_WELCOME_WORDS;
	}
	else
	{
		strWelcomeWords = m_pInputHtml->GetContentText();
		//m_InputLogic.GetOutputString( strText , strWelcomeWords ) ;
		bDefault = false;

		int nLen = common::utility::stringhelper::CalcStringAsciiLength(strWelcomeWords);
		if (nLen == 0 || nLen > 100)
		{
			C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"迎宾词不能为空，且不超过100个字符" );
			return;
		}
		//m_pRoomDlg->GetRoomOption()->EncodeWelcomeWords(strWelcomeWords, GetFontInfo());
	}
	m_pParentDlg->GetHtmlOutPutManager()->ProcessSendMsgText( strWelcomeWords , false ) ;
	pRoomOption->OnConfirmUserManager(/*bAutoWelcome, */strWelcomeWords.GetBuffer(), GetFontInfo(), bDefault);

	CString strSelfWords = m_pInputHtml->GetContentText();
	SaveWelcomeWords(strSelfWords.GetBuffer(0), GetFontInfo());
}

void CUserMgrDlg::SetFontInfo( core::MessageFont stFont )
{
	m_InputLogic.SetFontColor(stFont.font_color);
	CString strFontName = stFont.font_type.c_str();
	m_InputLogic.SetFontName(strFontName);
	m_InputLogic.SetFontSize(stFont.font_size);
	m_InputLogic.SetFontBold(stFont.font_style & core::FONTSTYLETYPE_BOLD ? 1 : 0);
	m_InputLogic.SetFontItalic(stFont.font_style & core::FONTSTYLETYPE_ITALICS ? 1 : 0);
	m_InputLogic.SetFontUnderLine(stFont.font_style & core::FONTSTYLETYPE_UNDERLINE ? 1 : 0);
	m_pInputHtml->UpdateMy3();
}

void CUserMgrDlg::OnClickEmotion()
{
	common::ui::CCursorPos pt;
	SwitchResourceToModule(_T("Room"));
	if ( m_pEmotionDlg == NULL )
	{
		m_pEmotionDlg = new CEmotionDlg(this) ;
		m_pEmotionDlg->Create(CEmotionDlg::IDD,this);
		m_pEmotionDlg->SetWindowPos(NULL,pt.x,pt.y-262,282,262,SWP_SHOWWINDOW);

	}
	else
	{
		m_pEmotionDlg->SetWindowPos(NULL,pt.x,pt.y-262,282,262,SWP_SHOWWINDOW);
	}
}
void CUserMgrDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if ( bShow/* && !m_bInitDataed*/ )
	{
		InitDlgData() ;
		//m_bInitDataed = TRUE;
	}
	// TODO: 在此处添加消息处理程序代码
}

void CUserMgrDlg::Refresh()
{
	//InitDlgData();
}

bool CUserMgrDlg::SaveWelcomeWords( std::wstring strWelcomeWords, core::MessageFont stFont )
{
	CString strPath;
	strPath.Format(L"%s\\userdata\\%d", common::utility::systemhelper::Get179AppPath().c_str(), GetCurInfo().unUIN);

	if (!PathFileExists(strPath))
	{
		common::utility::filehelper::CreateAllDirectory(strPath.GetBuffer(0));
	}

	std::wstring strFullFilePath = strPath.GetBuffer(0);
	strFullFilePath += L"\\welcome.xml";

	CFile xmlFile;
	if (!xmlFile.Open(strFullFilePath.c_str(), CFile::modeCreate|CFile::modeWrite))
	{
		return false;
	}

	char chHead[] = {0xEF, 0xBB, 0xBF};
	xmlFile.Write(chHead, 3);

	std::wstring strTempWelcome = strWelcomeWords;
	m_pParentDlg->GetRoomBusiness()->GetRoomOption()->EncodeWelcomeWords(strTempWelcome, stFont);

	std::string strTemp = common::utility::stringhelper::UnicodeToUTF8(strTempWelcome);
	xmlFile.Write(strTemp.c_str(), strTemp.length());

	xmlFile.Close();

	return true;
}

bool CUserMgrDlg::LoadWelcomeWords( std::wstring & strWelcomeWords, core::MessageFont & stFont )
{
	CString strPath;
	strPath.Format(L"%s\\userdata\\%d\\welcome.xml", common::utility::systemhelper::Get179AppPath().c_str(), GetCurInfo().unUIN);

	common::utility::expat_document xmlDoc;
	if (!xmlDoc.load_from_file(strPath.GetBuffer(0)))
	{
		return false;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	if (!pElement)
	{
		return false;
	}

	strWelcomeWords = pElement->get_element_contents();
	stFont.font_type = pElement->get_attribute_by_name(L"FontType")->get_attribute_value();
	stFont.font_color = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"FontColor")->get_attribute_value());
	stFont.font_size = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"FontSize")->get_attribute_value());
	stFont.font_style = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"FontStyle")->get_attribute_value());

	return true;
}

void CUserMgrDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}

void CUserMgrDlg::NotifyParentModify()
{
	if (!m_bInitDataed)
	{
		return;
	}
	GetParent()->PostMessage(WM_NOTIFY_ROOM_SYSSET_MODIFY, 0, 0);
}

bool CUserMgrDlg::OnInput( int nContenLen )
{
	NotifyParentModify();
	return true;
}