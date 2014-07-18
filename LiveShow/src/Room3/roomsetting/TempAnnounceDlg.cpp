// TempAnnounceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "TempAnnounceDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"
#include "..\RoomParentDlg.h"
#include "..\personalsetting/PersonalConfig.h"

#define  SKIN_ON_CLICK_JIACU        1
#define  SKIN_ON_CLICK_XIETI        2
#define  SKIN_ON_CLICK_UNDERLINE    3
#define  SKIN_ON_CLICK_FONT_COLOR   4
#define  SKIN_ON_CLICK_BTN_PUBLISH  5
#define  SKIN_ON_CLICK_BTN_ADD      6
#define  SKIN_ON_CLICK_BTN_SAVE_MODIFY  7
#define  SKIN_ON_CLICK_BTN_DELETE       8
// CTempAnnounceDlg 对话框

IMPLEMENT_DYNAMIC(CTempAnnounceDlg, CDialog)

CTempAnnounceDlg::CTempAnnounceDlg( CRoomParentDlg* pParentDlg , CWnd* pParent /*=NULL*/)
	: CDialog(CTempAnnounceDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
    m_pSkinEngine = NULL;
	//m_pInputHtml = NULL;
	m_nCurSelItem = -1;
	::GetSkinEngine(&m_pSkinEngine);
}

CTempAnnounceDlg::~CTempAnnounceDlg()
{
	//if (m_pInputHtml)
	//{
	//	delete m_pInputHtml;
	//	m_pInputHtml = NULL;
	//}

	for (VecNoticeInfo::iterator it = m_vecNoticeList.begin(); it != m_vecNoticeList.end(); ++it)
	{
		if (*it != NULL)
		{
			delete *it;
		}
	}
	m_vecNoticeList.clear();
}

void CTempAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SAVED_ANNOUNCED, m_savedAnnounceList);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_FontSizeCombo);
	DDX_Control(pDX, IDC_COMBO_FONT, m_FontCombo);
}


BEGIN_MESSAGE_MAP(CTempAnnounceDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_CBN_SELCHANGE(IDC_COMBO_FONTSIZE, &CTempAnnounceDlg::OnCbnSelchangeComboFontsize)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT, &CTempAnnounceDlg::OnCbnSelchangeComboFont)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SAVED_ANNOUNCED, &CTempAnnounceDlg::OnNMClickListSavedAnnounced)
	ON_EN_CHANGE(IDC_HTML_CONTENT, OnInputChange)
END_MESSAGE_MAP()


// CTempAnnounceDlg 消息处理程序

int CTempAnnounceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
    ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"TempAnnounceDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}


LRESULT CTempAnnounceDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
    switch(wParam)
	{
	case SKIN_ON_CLICK_JIACU:
		{
			if (1 == lParam)//选择加粗
			{
				m_InputCtrl.SetFontBold(true);				
			}
			else//取消加粗
			{
				m_InputCtrl.SetFontBold(false);
			}
			CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
		}
        break;
	case SKIN_ON_CLICK_XIETI:
		{
			if (1 == lParam)//选择斜体
			{
				m_InputCtrl.SetFontItalic(true);
			}
			else//取消斜体
			{
				m_InputCtrl.SetFontItalic(false);
			}
			CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
		}
		break;
	case SKIN_ON_CLICK_UNDERLINE:
		{ 
			if (1 == lParam)//选择下划线
			{
				m_InputCtrl.SetFontUnderLine(true);
			}
			else//取消下划线
			{
				m_InputCtrl.SetFontUnderLine(false);
			}
			CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
		}
		break;
	case SKIN_ON_CLICK_FONT_COLOR:
		{
			CColorDialog ColorDlg(GetFontInfo().font_color );
			if ( ColorDlg.DoModal() == IDOK )
			{
				m_InputCtrl.SetFontColor(ColorDlg.GetColor());
			}
			CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
		}
		break;
	case SKIN_ON_CLICK_BTN_PUBLISH:
		{
			PublishTempAnnounce();
		}
		break;
	case SKIN_ON_CLICK_BTN_ADD:
		{
			OnClickAdd();
		}
		break;
	case SKIN_ON_CLICK_BTN_SAVE_MODIFY:
		{
			OnClickSave();
		}
		break;
	case SKIN_ON_CLICK_BTN_DELETE:
		{
			OnClickDelete();
		}
		break;
	default: break;
	}

    return TRUE;
}

BOOL CTempAnnounceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_InputCtrl.Create( WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL , CRect(0,0,0,0), this, IDC_HTML_CONTENT);
	m_InputCtrl.SetFontInfo( CPersonalConfig::GetInstance()->GetTempMessageFont() ) ;
	m_InputCtrl.SendMessage(EM_SETLANGOPTIONS, 0, 0);
	CComVariant var = (long)m_InputCtrl.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemHtmlContent", L"hwnd", var);

	m_InputCtrl.SetEventMask(ENM_CHANGE);

	InitFontPanel();
	InitSavedAnnounceList();

	if (m_nCurSelItem == -1)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTempAnnounceDlg::InitSavedAnnounceList()
{
	DWORD dwStyle = 0;
	LONG lStyle; 
	lStyle = GetWindowLong(m_savedAnnounceList.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle = lStyle | LVS_REPORT /*| LVS_SHOWSELALWAYS*/; //设置style 
	SetWindowLong(m_savedAnnounceList.m_hWnd, GWL_STYLE, lStyle);
	dwStyle = m_savedAnnounceList.GetExtendedStyle();
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	m_savedAnnounceList.SetExtendedStyle(dwStyle); //设置扩展风格 
	m_savedAnnounceList.SetBkColor(RGB(255,255,255));
	m_savedAnnounceList.SetTextBkColor(RGB(255,255,255));

	m_savedAnnounceList.InsertColumn(0,L"临时公告内容(点击显示在上方)",LVCFMT_CENTER,369);

	CString strPath;
	strPath.Format(L"%s\\userdata\\%d\\%d.xml", common::utility::systemhelper::Get179AppPath().c_str(), 
		GetCurInfo().unUIN, m_pParentDlg->GetRoomBusiness()->GetRoomID());

	if (CTempNoticeManager::LoadXML(strPath.GetBuffer(0), m_vecNoticeList))
	{
		for (int i = 0; i < m_vecNoticeList.size(); i++)
		{
			m_savedAnnounceList.InsertItem(i,m_vecNoticeList[i]->strContent.c_str());
			m_savedAnnounceList.SetItemData(i,(DWORD_PTR)(m_vecNoticeList[i]));
		}
	}
}


void CTempAnnounceDlg::InitFontPanel()
{
	core::MessageFont FontInfo = CPersonalConfig::GetInstance()->GetTempMessageFont() ;
	for ( int i = 9 ; i<=20 ; ++i )
	{
		CString strTemp;
		strTemp.Format(_T("%d") ,i ) ;
		int nindex = m_FontSizeCombo.AddString( strTemp ) ;
		if ( i == FontInfo.font_size )
		{
			m_FontSizeCombo.SetCurSel( nindex ) ;
		}
	}
	wstring wsCurrentFont = FontInfo.font_type ;
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
	bool bBold = FontInfo.font_style & core::FONTSTYLETYPE_BOLD;
	bool bItalic = FontInfo.font_style & core::FONTSTYLETYPE_ITALICS;
	bool bUnderLine = FontInfo.font_style & core::FONTSTYLETYPE_UNDERLINE;
	SetButtonStatus( bBold , bItalic , bUnderLine ) ;
}

MessageFont CTempAnnounceDlg::GetFontInfo()
{
	return m_InputCtrl.GetFontInfo();
}

void CTempAnnounceDlg::OnCbnSelchangeComboFontsize()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_FontSizeCombo.GetCurSel() ;
	if ( nIndex != CB_ERR )
	{
		CString strTemp ;
		m_FontSizeCombo.GetLBText(nIndex, strTemp);
		int nFontSize = _wtoi( strTemp) ;
		if ( nFontSize != GetFontInfo().font_size )
		{
			m_InputCtrl.SetFontSize(nFontSize);
			CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
		}
	}
}

void CTempAnnounceDlg::OnCbnSelchangeComboFont()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_FontCombo.GetCurSel() ;
	if ( nIndex != CB_ERR )
	{
		CString strTemp;
		m_FontCombo.GetLBText(nIndex, strTemp);
		CString strFontName( GetFontInfo().font_type.c_str() ) ;
		if ( strTemp != strFontName )
		{
			m_InputCtrl.SetFontName(strTemp.GetBuffer(0));
			CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
		}
	}
}

void CTempAnnounceDlg::PublishTempAnnounce()
{
	CString strMessage;
	m_InputCtrl.GetWindowText(strMessage);
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strMessage.GetBuffer(0));
	if (nLen == 0 || nLen > 500)
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"内容不能为空，且不能超过500个字符" );
		return;
	}
	m_InputCtrl.SetWindowText(L"") ;
	CPersonalConfig::GetInstance()->SetTempMessageFont( m_InputCtrl.GetFontInfo() ) ;
	std::wstring strText = strMessage.GetBuffer(0);
	common::utility::systemhelper::FilterWords(strText);

	core::SendMessageEvent * pEvent = new core::SendMessageEvent();
	pEvent->roomid = m_pParentDlg->GetRoomBusiness()->GetRoomID();
	pEvent->message = strText;
	pEvent->enmType = core::SENDMESSAGETYPE_TEMP_NOTICE;
	pEvent->stFont = m_InputCtrl.GetFontInfo() ;

	g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_SEND_MESSAGE, MODULE_ID_CORE, 0, 0, 0, pEvent));
	m_pParentDlg->BackMessage( 0 , m_InputCtrl.GetFontInfo() , strText , core::SENDMESSAGETYPE_TEMP_NOTICE ) ;
}

void CTempAnnounceDlg::OnClickAdd()
{

	if (m_vecNoticeList.size() >= 15)
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"最多只能保存15条临时公告" );
		return;
	}

	CString strMessage;
	m_InputCtrl.GetWindowText(strMessage);
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strMessage.GetBuffer(0));
	if (nLen == 0 || nLen > 500)
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"内容不能为空，且不能超过500个字符" );
		return;
	}

	int nItem = m_savedAnnounceList.GetItemCount();

	NoticeInfo * pNoticeInfo = new NoticeInfo();
	pNoticeInfo->strContent = strMessage.GetBuffer(0);
	pNoticeInfo->stFont = GetFontInfo();
	//m_vecNoticeList[nItem] = stNoticeInfo;
	m_vecNoticeList.push_back(pNoticeInfo);

	SaveXML();

	m_savedAnnounceList.InsertItem(nItem,strMessage);
	m_savedAnnounceList.SetItemData(nItem,(DWORD_PTR)m_vecNoticeList[nItem]);
	m_InputCtrl.ClearInput();
	m_nCurSelItem = -1;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);

#if 0
	std::wstring strPath = common::utility::systemhelper::Get179AppPath()();
	strPath += L"\\123456";

	VecNoticeInfo noticelist;
	for (int i = 0; i < 10; i++)
	{
		NoticeInfo stNoticeInfo;
		stNoticeInfo.strTitle = common::utility::stringhelper::IntToString(i);
		stNoticeInfo.strContent = L"test";
		stNoticeInfo.stFont.font_color = GetFontInfo().crColor;
		stNoticeInfo.stFont.font_size = GetFontInfo().unFontSize;
		stNoticeInfo.stFont.font_style = GetFontInfo().unFontType;
		stNoticeInfo.stFont.font_type = GetFontInfo().strFontName;
		noticelist.push_back(stNoticeInfo);
	}


	CTempNoticeManager::SaveXML(strPath, L"test.xml", noticelist);
	strPath += L"\\test.xml";
	CTempNoticeManager::LoadXML(strPath, noticelist);
#endif
}

void CTempAnnounceDlg::OnNMClickListSavedAnnounced(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项

	if (nItem == -1)
	{
		return;
	}

	m_nCurSelItem = nItem;
	if (m_nCurSelItem >= 0)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)false);
	}
	NoticeInfo * pNoticeInfo = (NoticeInfo *)m_savedAnnounceList.GetItemData(nItem);
	if (pNoticeInfo != NULL)
	{
		//m_InputCtrl.Clear();

		bool bBold = pNoticeInfo->stFont.font_style & core::FONTSTYLETYPE_BOLD;
		bool bItalic = pNoticeInfo->stFont.font_style & core::FONTSTYLETYPE_ITALICS;
		bool bUnderLine = pNoticeInfo->stFont.font_style & core::FONTSTYLETYPE_UNDERLINE;

		m_InputCtrl.SetFontColor(pNoticeInfo->stFont.font_color);
		m_InputCtrl.SetFontName(pNoticeInfo->stFont.font_type);
		m_InputCtrl.SetFontSize(pNoticeInfo->stFont.font_size);
		m_InputCtrl.SetFontBold(bBold);
		m_InputCtrl.SetFontItalic(bItalic);
		m_InputCtrl.SetFontUnderLine(bUnderLine);

		m_InputCtrl.SetWindowText(pNoticeInfo->strContent.c_str());

		int nIndex = m_FontCombo.FindString(0, pNoticeInfo->stFont.font_type.c_str());
		if (nIndex != CB_ERR)
		{
			m_FontCombo.SetCurSel(nIndex);
		}

		nIndex = m_FontSizeCombo.FindString(0, common::utility::stringhelper::IntToString(pNoticeInfo->stFont.font_size).c_str());
		if (nIndex != CB_ERR)
		{
			m_FontSizeCombo.SetCurSel(nIndex);
		}

		SetButtonStatus( bBold , bItalic , bUnderLine ) ;


		//m_savedAnnounceList.SetFocus();
	}

	*pResult = 0;
}

void CTempAnnounceDlg::OnClickSave()
{
	if (m_nCurSelItem == -1)
	{
		return;
	}

	CString strMessage;
	m_InputCtrl.GetWindowText(strMessage);
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strMessage.GetBuffer(0));
	if (nLen == 0 || nLen > 500)
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING ,L"内容不能为空，且不能超过500个字符" );
		return;
	}

	NoticeInfo * pNoticeInfo = (NoticeInfo *)m_savedAnnounceList.GetItemData(m_nCurSelItem);
	if (pNoticeInfo != NULL)
	{
		pNoticeInfo->strContent = strMessage.GetBuffer(0);
		pNoticeInfo->stFont = GetFontInfo();
	}
	m_savedAnnounceList.SetItemText(m_nCurSelItem, 0, strMessage);

	m_InputCtrl.ClearInput();
	m_nCurSelItem = -1;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);

	SaveXML();
}

void CTempAnnounceDlg::OnClickDelete()
{
	if (m_nCurSelItem == -1)
	{
		return;
	}

	if (C179MsgBox::Show( this, NewMsgBox::MSGBOX_ICO_OK_CANCEL_ONE_STRING, NewMsgBox::ICONTYPE_WARNING, L"您确定删除本条公告吗？" ) != RET_OK)
	{
		return;
	}

	NoticeInfo * pNoticeInfo = (NoticeInfo *)m_savedAnnounceList.GetItemData(m_nCurSelItem);
	if (pNoticeInfo != NULL)
	{
		for (VecNoticeInfo::iterator it = m_vecNoticeList.begin(); it != m_vecNoticeList.end();)
		{
			if ((*it)->strContent == pNoticeInfo->strContent)
			{
				delete *it;
				it = m_vecNoticeList.erase(it);
				break;
			}
			else
			{
				it++;
			}
		}

		m_savedAnnounceList.DeleteItem(m_nCurSelItem);
	}

	m_nCurSelItem = -1;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);
	
	SaveXML();
}

void CTempAnnounceDlg::SaveXML()
{
	CString strPath;
	strPath.Format(L"%s\\userdata\\%d", common::utility::systemhelper::Get179AppPath().c_str(), GetCurInfo().unUIN);

	CString strFile;
	strFile.Format(L"%d.xml", m_pParentDlg->GetRoomBusiness()->GetRoomID());

	CTempNoticeManager::SaveXML(strPath.GetBuffer(0), strFile.GetBuffer(0), m_vecNoticeList);
}

//清空输入框中的内容
void CTempAnnounceDlg::ClearInputCtrlValue() 
{
	m_InputCtrl.SetWindowText(L"") ;
}

BOOL CTempAnnounceDlg::PreTranslateMessage(MSG* pMsg)
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

void CTempAnnounceDlg::SetInputAnnounce( CString& strText ) 
{
	m_InputCtrl.SetWindowText(strText);
}

void CTempAnnounceDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}

void CTempAnnounceDlg::OnInputChange()
{
	CString strMessage;
	m_InputCtrl.GetWindowText(strMessage);

	if (m_nCurSelItem != -1)
	{
		NoticeInfo * pNoticeInfo = (NoticeInfo *)m_savedAnnounceList.GetItemData(m_nCurSelItem);
		if (strMessage.Compare(pNoticeInfo->strContent.c_str()) == 0 )
		{
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);
		}
		else
		{
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)false);
		}
	}
}

void CTempAnnounceDlg::OnEnChangeEditTitle()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strMessage;
	m_InputCtrl.GetWindowText(strMessage);

	if (m_nCurSelItem != -1)
	{
		NoticeInfo * pNoticeInfo = (NoticeInfo *)m_savedAnnounceList.GetItemData(m_nCurSelItem);
		if (strMessage.Compare(pNoticeInfo->strContent.c_str()) == 0 )
		{
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);
		}
		else
		{
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)false);
		}
	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);
	}
}

void CTempAnnounceDlg::SetButtonStatus( bool bBold , bool bItalic , bool bUnderLine )
{
	if (bBold)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnJiaCu", L"checked", (CComVariant)TRUE);
	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnJiaCu", L"checked", (CComVariant)FALSE);
	}

	if (bItalic)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnXieTi", L"checked", (CComVariant)TRUE);
	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnXieTi", L"checked", (CComVariant)FALSE);
	}

	if (bUnderLine)
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnUnderLine", L"checked", (CComVariant)TRUE);
	}
	else
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnUnderLine", L"checked", (CComVariant)FALSE);
	}
}
