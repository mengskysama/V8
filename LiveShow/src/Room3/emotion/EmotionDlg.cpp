// EmotionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "EmotionDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\GlobalManager.h"
#include "..\UserMessageDefine.h"
#include "..\personalsetting\PersonalConfig.h"
#include "..\RoomMidDlg.h"
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE                     1
#define  SKIN_ON_CLICK_TAB_EMOTION               2
#define  SKIN_ON_CLICK_BTN_PREPAGE               3
#define  SKIN_ON_CLICK_BTN_NEXTPAGE              4
#define  SKIN_ON_CLICK_JIACU					 5
#define  SKIN_ON_CLICK_XIETI					 6
#define  SKIN_ON_CLICK_UNDERLINE				 7
#define  SKIN_ON_CLICK_FONT_COLOR				 8

#define  SKIN_NOTIFY_IF_SHOW_FONTPANNEL          20
// CEmotionDlg 对话框

IMPLEMENT_DYNAMIC(CEmotionDlg, CDialog)

CEmotionDlg::CEmotionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmotionDlg::IDD, pParent),
	m_pParent( pParent )
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	
	m_pRoomMidDlg = NULL;
	m_bInitFontPannel = false;
}

CEmotionDlg::~CEmotionDlg()
{
}

void CEmotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMOTION, m_emotionListCtrl);
}


BEGIN_MESSAGE_MAP(CEmotionDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_ACTIVATE()
	//ON_WM_KILLFOCUS()
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_IN_EMOTION_DLG, &CEmotionDlg::OnCbnSelchangeComboFontInEmotionDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTSIZE_IN_EMOTION_DLG, &CEmotionDlg::OnCbnSelchangeComboFontsizeInEmotionDlg)
END_MESSAGE_MAP()


// CEmotionDlg 消息处理程序

int CEmotionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"EmotionDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

BOOL CEmotionDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CEmotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
   // ShowEmotion();  // InitEmotion放这里无效果

	m_emotionListCtrl.Initialize();
	DWORD dwstyle = m_emotionListCtrl.GetExtendedStyle();
	dwstyle = 0;
	m_emotionListCtrl.SetExtendedStyle(dwstyle);
	m_emotionListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	m_emotionListCtrl.SetListData( 0, 0);

	int iTotalPageInCurGroup = m_emotionListCtrl.GetTotalPageInCurGroup();
	CString textPage;
	textPage.Format(_T("1/%d"),iTotalPageInCurGroup);
	CComVariant var = textPage;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
	ShowEmotion() ;
	//return TRUE;  // return TRUE unless you set the focus to a control
	return TRUE;
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CEmotionDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			ShowWindow( SW_HIDE ) ;
		}
		break;
	case SKIN_ON_CLICK_TAB_EMOTION:
		{
            OnClickEmotionTab((int)lParam);
		}
	    break;
	case SKIN_ON_CLICK_BTN_PREPAGE:
		{
            OnClickPrePage();
		}
		break;
	case SKIN_ON_CLICK_BTN_NEXTPAGE:
		{
            OnClickNextPage();
		}
		break;
	case SKIN_ON_CLICK_JIACU:
		{
			OnClickBtnBold((int)lParam );
		}
		break;
	case SKIN_ON_CLICK_UNDERLINE:
		{
			OnClickBtnUnderline((int)lParam );
		}
		break;
	case SKIN_ON_CLICK_XIETI:
		{
			OnClickBtnXieTi((int)lParam );
		}
		break;
	case SKIN_ON_CLICK_FONT_COLOR:
		{
			OnClickBtnColor();
		}
		break;
	default:break;
	}


	return TRUE;
}


void CEmotionDlg::ShowEmotion() //对话框显示着的时候才有效果
{
	map<int , wstring >::iterator iter ;
	int i = 0 ;
	map<int , wstring > mTypeList = EmotionMgr->GetTypeList() ;
	for ( iter = mTypeList.begin() ; iter != mTypeList.end() ; ++iter , ++i )
	{
		CComVariant vars[1];
		vars[0] = -1;
		DISPPARAMS dispParams = {vars, 0, 1, 0};
		m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(),L"tabEmotion",L"AddTabItem",NULL,&dispParams);

		CComVariant vars2[2];
		vars2[0] = CString( iter->second.c_str() ) ; 
		vars2[1] = i;
		DISPPARAMS dispParams2 = {vars2, 0, 2, 0};
		m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(),L"tabEmotion",L"SetTabItemText",NULL,&dispParams2);
	}

	CComVariant vars = 0;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabEmotion",L"tabItemSelected",vars);
	m_nCurSel = 0 ;
	
}


void CEmotionDlg::OnClickEmotionTab(int iTab)
{
	if (iTab >= 0)
	{
		if (iTab != m_nCurSel)
		{
			CComVariant vars = iTab;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabEmotion",L"tabItemSelected",vars);
			m_emotionListCtrl.SetListData(iTab);

			int iTotalPageInCurGroup = m_emotionListCtrl.GetTotalPageInCurGroup();
			CString textPage;
			textPage.Format(_T("1/%d"),iTotalPageInCurGroup);
			CComVariant var = textPage;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
			m_nCurSel = iTab ;

		}
	}
	
}


void CEmotionDlg::OnSelEmotion(int iEmoGroup,int iEmoIndex)
{
	map< int , wstring >::iterator miter ;
	int i = 0 ;
	map< int , wstring > mTypeList = EmotionMgr->GetTypeList() ;

	map< int , wstring >::iterator tempIter = mTypeList.begin();
	advance(tempIter, iEmoGroup);
	bool bCaiTiao = (tempIter->second == L"彩条");//是否彩条
	for ( miter = mTypeList.begin() ; miter != mTypeList.end() ; ++miter ,++i )
	{
		if ( iEmoGroup == i )
		{
			vector<EmotionItem*> vEmotionList = EmotionMgr->GetEmotionListByType( miter->first ) ;
			if ( iEmoIndex < vEmotionList.size() )
			{
				if ( m_pParent != NULL )
				{
					EmotionItem* pEmotionItem = vEmotionList[iEmoIndex] ;
					CString strTemp = pEmotionItem->GetBigEmotionPath().c_str() ;
					if (bCaiTiao)
					{
						m_pParent->SendMessage( WM_CLICK_CAIZI , 0 ,(LPARAM) (iEmoIndex + 1) ) ;
					}else
					{
						m_pParent->SendMessage( WM_CLICK_EMOTION , 0 ,(LPARAM)(LPCTSTR)strTemp ) ;
					}
					
				}
			}
			break;
		}
	}
	
}

void CEmotionDlg::OnClickPrePage()
{
	int iCurGroup = m_emotionListCtrl.GetCurGroup();
	int iCurPage = m_emotionListCtrl.GetCurPage();
	int iTotalPageInCurGroup = m_emotionListCtrl.GetTotalPageInCurGroup();
    
	if ((iCurPage - 1) >= 0)//存在上一页
	{
		m_emotionListCtrl.SetListData(iCurGroup,--iCurPage);
		CString textPage;
		textPage.Format(_T("%d/%d"),iCurPage + 1,iTotalPageInCurGroup);
		CComVariant var = textPage;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
	}

}

void CEmotionDlg::OnClickNextPage()
{
    int iCurGroup = m_emotionListCtrl.GetCurGroup();//当前组
	int iCurPage = m_emotionListCtrl.GetCurPage();//该组下的当前页
	int iTotalPageInCurGroup = m_emotionListCtrl.GetTotalPageInCurGroup();//该组下的总页数

	if ((iCurPage+1) < iTotalPageInCurGroup)//存在下一页
	{
       m_emotionListCtrl.SetListData(iCurGroup,++iCurPage);
	   CString textPage;
	   textPage.Format(_T("%d/%d"),iCurPage + 1,iTotalPageInCurGroup);
	   CComVariant var = textPage;
	   m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textPage",L"text",var);
	}

}


void CEmotionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( nState == WA_INACTIVE )
	{
		if(pWndOther == NULL || !pWndOther->IsKindOf(RUNTIME_CLASS(CColorDialog)))
		{
			ShowWindow( SW_HIDE );
		}
	}

	// TODO: 在此处添加消息处理程序代码
}

void CEmotionDlg::SetParentWnd( CWnd * pWnd )
{
	ASSERT(pWnd != NULL);
	m_pParent = pWnd;
}

void CEmotionDlg::InitFontPanel() 
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

	for ( int i = FONT_SIZE_MIN ; i<= FONT_SIZE_MAX ; ++i )
	{
		CString strTemp;
		strTemp.Format(_T("%d") ,i ) ;
		int nindex = ((CComboBox*)GetDlgItem(IDC_COMBO_FONTSIZE_IN_EMOTION_DLG))->AddString( strTemp ) ;
		if ( i == FontInfo.font_size )
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_FONTSIZE_IN_EMOTION_DLG))->SetCurSel( nindex ) ;
		}
	}	

	wstring wsCurrentFont = FontInfo.font_type;
	vector<wstring> vFontList ;
	common::utility::systemhelper::GetChineseFontNameList( vFontList ) ;
	vector<wstring>::iterator iter ;
	for ( iter = vFontList.begin() ; iter != vFontList.end() ; ++iter )
	{
		int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_FONT_IN_EMOTION_DLG))->AddString( (*iter).c_str() ) ;
		if ( wsCurrentFont == (*iter) )
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_FONT_IN_EMOTION_DLG))->SetCurSel( nIndex ) ;
		}
	}
	bool bBold = FontInfo.font_style & core::FONTSTYLETYPE_BOLD;
	bool bItalic = FontInfo.font_style & core::FONTSTYLETYPE_ITALICS;
	bool bUnderLine = FontInfo.font_style & core::FONTSTYLETYPE_UNDERLINE;
	SetButtonStatus( bBold , bItalic , bUnderLine ) ;
}

void CEmotionDlg::SetRoomMidDlg( CRoomMidDlg* pRoomMidDlg )
{
	if (pRoomMidDlg != NULL)
	{
		m_pRoomMidDlg = pRoomMidDlg;
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_IF_SHOW_FONTPANNEL, (LONG)true);
		if (!m_bInitFontPannel)
		{
			InitFontPanel();
			m_bInitFontPannel = true;
		}
	}else
	{
		NOTIFY_SKIN_HELPER(SKIN_NOTIFY_IF_SHOW_FONTPANNEL, (LONG)false);//不显示字体设置的地方也不显示彩条，js代码里控制，默认到第一组表情
	}
}

void CEmotionDlg::SetButtonStatus( bool bBold ,bool bItalic ,bool bUnderLine )
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnJiaCu", L"checked", (CComVariant)(bBold));
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnXieTi", L"checked", (CComVariant)(bItalic));
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnUnderLine", L"checked", (CComVariant)(bUnderLine));
}

void CEmotionDlg::OnCbnSelchangeComboFontInEmotionDlg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pRoomMidDlg == NULL)
	{
		return;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_HONOR)//荣耀VIP以下
	{
		m_pRoomMidDlg->OnModifyFont();
		ShowWindow(SW_HIDE);
		return;
	}
		
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_FONT_IN_EMOTION_DLG))->GetCurSel() ;
	if ( nIndex != CB_ERR )
	{
		CString strTmep ;
		((CComboBox*)GetDlgItem(IDC_COMBO_FONT_IN_EMOTION_DLG))->GetWindowText( strTmep ) ;
		CString strFontName( m_pRoomMidDlg->GetFontInfo().font_type.c_str() ) ;
		if ( strTmep != strFontName )
		{
			m_pRoomMidDlg->SetFontName( strTmep) ;
			CPersonalConfig::GetInstance()->SetChatMessageFont(m_pRoomMidDlg->GetFontInfo());
		}
	}

}

void CEmotionDlg::OnCbnSelchangeComboFontsizeInEmotionDlg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pRoomMidDlg == NULL)
	{
		return;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_HONOR)//荣耀VIP以下
	{
		m_pRoomMidDlg->OnModifyFont();
		ShowWindow(SW_HIDE);
		return;
	}
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_FONTSIZE_IN_EMOTION_DLG))->GetCurSel() ;
	if ( nIndex != CB_ERR )
	{
		CString strTmep ;
		((CComboBox*)GetDlgItem(IDC_COMBO_FONTSIZE_IN_EMOTION_DLG))->GetWindowText( strTmep ) ;
		int nFontSize = _wtoi( strTmep) ;
		if ( nFontSize != m_pRoomMidDlg->GetFontInfo().font_size )
		{
			m_pRoomMidDlg->SetFontSize( nFontSize) ;
			CPersonalConfig::GetInstance()->SetChatMessageFont(m_pRoomMidDlg->GetFontInfo());
		}
	}
}

void CEmotionDlg::OnClickBtnBold(int iBold)
{
	if (m_pRoomMidDlg == NULL)
	{
		return;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_HONOR)//荣耀VIP以下
	{
		m_pRoomMidDlg->OnModifyFont();
		ShowWindow(SW_HIDE);
		return;
	}
	m_pRoomMidDlg->OnClickBtnJiaCu(iBold);
	//SetFocus();
}

void CEmotionDlg::OnClickBtnUnderline(int iUnderLine)
{
	if (m_pRoomMidDlg == NULL)
	{
		return;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_HONOR)//荣耀VIP以下
	{
		m_pRoomMidDlg->OnModifyFont();
		ShowWindow(SW_HIDE);
		return;
	}
	m_pRoomMidDlg->OnClickBtnUnderline(iUnderLine);
}

void CEmotionDlg::OnClickBtnXieTi(int iXieTi)
{
	if (m_pRoomMidDlg == NULL)
	{
		return;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_HONOR)//荣耀VIP以下
	{
		m_pRoomMidDlg->OnModifyFont();
		ShowWindow(SW_HIDE);
		return;
	}
	m_pRoomMidDlg->OnClickBtnXieTi(iXieTi);
}

void CEmotionDlg::OnClickBtnColor()
{
	if (m_pRoomMidDlg == NULL)
	{
		return;
	}
	if (GetCurInfo().enmVIPLevel < core::VIPLEVEL_HONOR)//荣耀VIP以下
	{
		m_pRoomMidDlg->OnModifyFont();
		ShowWindow(SW_HIDE);
		return;
	}
	m_pRoomMidDlg->OnClickBtnFontColor();
}