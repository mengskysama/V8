// MsgBox.cpp : 实现文件
//

#include "../../stdafx.h"
#include "../../include/ui/MsgBox.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "../../include/utility/SystemHelper.h"

#define SKIN_ON_CLICK_CLOSE                    1
#define SKIN_ON_CLICK_OK                       2
#define SKIN_ON_CLICK_CANCEL                   7
#define NOTIFY_SKIN_REFRESH                    8
//#define MSGBOX_STYLE_OK                        _T("3")
//#define MSGBOX_STYLE_OK_CANCEL                 _T("4")
//#define MSGBOX_STYLE_OK_WITH_LINK              _T("5")
//#define MSGBOX_STYLE_OK_CANCEL_WITH_LINK       _T("6")
//#define MSGBOX_STYLE_OK_CANCEL_WITH_2_LINK     _T("8")
//#define MSGBOX_STYLE_OK_WITH_TAIL_LINK         _T("12")
//#define MSGBOX_STYLE_OK_WITH_CHECKBOX          _T("13")
//#define MSGBOX_STYLE_OK_CANCEL_WITH_CHECKBOX   _T("14")
//
//#define SKIN_ON_CLICK_BTN_LINK                 9
//#define SKIN_ON_CLICK_BTN_LINK_MID             10
//#define SKIN_ON_CLICK_BTN_LINK_TAIL            11
//////////////////
#define SKIN_ON_CLICK_BTN_NEW_LINK             100


BOOL CMsgBox::m_bMsgBoxVisible = FALSE;
// CMsgBox 对话框

IMPLEMENT_DYNAMIC(CMsgBox, CDialog)

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent),
	m_pParent(pParent)
{
	m_pSkinEngine = NULL;
	m_strTitle = _T("提 示");
	m_iRet = RET_CANCEL;
	m_bChecked = FALSE;
	::GetSkinEngine(&m_pSkinEngine);

}

CMsgBox::~CMsgBox()
{
	CMsgBox::m_bMsgBoxVisible = FALSE;
}

void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMsgBox, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_BN_CLICKED(IDC_CHECK_NO_PROMPT, &CMsgBox::OnBnClickedCheckNoPrompt)
END_MESSAGE_MAP()

// CMsgBox 消息处理程序

int CMsgBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ModifyStyleEx( WS_EX_APPWINDOW , WS_EX_TOOLWINDOW) ;
	//ModifyStyleEx( WS_EX_TOOLWINDOW , 0 ) ;

	ASSERT(m_pSkinEngine != NULL);
	if(NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"MsgBox");
		ASSERT(hr==S_OK && __LINE__);
	}
    
	//SetContent();

	return 0;
}

LRESULT CMsgBox::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	   case SKIN_ON_CLICK_CLOSE:
		   {
			  if (m_Style == NewMsgBox::MSGBOX_ICO_OK_ONE_STRING_NONDOMODAL)//非模态销毁
			  {
				  //DestroyWindow();
				  ::PostMessage(GetParent()->GetSafeHwnd(), COMMONLIB_UM_NONDOMODAL_MSGBOX_CLOSE,(WPARAM) this, 0);
			  }else//模态销毁
			  {
                  PostMessage(WM_CLOSE,NULL,NULL);
			  }
		   }
		   break;
	   case SKIN_ON_CLICK_CANCEL:
		   {
              OnClickBtnCancel();
		   }
		   break;
	   case SKIN_ON_CLICK_OK:
		   {
			  OnClickBtnOK();
		   }
		   break;
	 
	   case SKIN_ON_CLICK_BTN_NEW_LINK:
		   {
			   if (m_Style == NewMsgBox::MSGBOX_GUEST_OPERATE)//游客类的“快去注册吧”不直接打开浏览器
		      {
				  ::PostMessage(m_pParent->GetSafeHwnd(), COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER,(WPARAM)m_pParent, 0);
				  PostMessage(WM_CLOSE,NULL,NULL);
		      }else
			  {
				  common::utility::systemhelper::OpenUrl(std::wstring(m_strLinkAddress));	
			  }
			  
		   }
		   break;
	   default:break;
	}
	return TRUE;
}


void CMsgBox::OnClickBtnOK()
{
	m_iRet = RET_OK;
	PostMessage(WM_CLOSE,NULL,NULL);
}

void CMsgBox::OnClickBtnCancel()
{
	m_iRet = RET_CANCEL;
	PostMessage(WM_CLOSE,NULL,NULL);
}  

void CMsgBox::SetContent()
{
    ///////////////////   新提示框  ///////////////////


	HWND dlgHwnd = this->GetSafeHwnd();
	CComVariant varTitle = m_strTitle.GetBuffer();
	m_pSkinEngine->SetSubSkinProp(dlgHwnd,L"textTheTitle",L"text",varTitle);
	CComVariant varStyle = m_Style;
	m_pSkinEngine->SetSubSkinProp(dlgHwnd,L"textMsgBoxStyle",L"text",varStyle);

	switch (m_Style)
	{
	case NewMsgBox::MSGBOX_OK_ONE_STRING:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);

		}
		break;
	case NewMsgBox::MSGBOX_OK_ONE_STRING_WITH_LINK:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strLinkLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnNewLink", L"label", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_TWO_STRING:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_TWO_STRING_WITH_LINK:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_strLinkLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnNewLink", L"label", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ICO_OK_ONE_STRING:
		{
			CComVariant varTemp = m_iconType;
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textIconType", L"text", varTemp);
			varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ICO_OK_TWO_STRING:
		{
			CComVariant varTemp = m_iconType;
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textIconType", L"text", varTemp);
			varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ICO_OK_CANCEL_ONE_STRING:
		{
			CComVariant varTemp = m_iconType;
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textIconType", L"text", varTemp);
			varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ICO_OK_CANCEL_TWO_STRING:
		{
			CComVariant varTemp = m_iconType;
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textIconType", L"text", varTemp);
			varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_GUEST_OPERATE:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_strLinkLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnNewLink", L"label", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_THREE_STRING:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_strPrompt3.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt3", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_THREE_STRING_WITH_LINK:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_strPrompt3.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt3", L"text", varTemp);
			varTemp = m_strLinkLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnNewLink", L"label", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_CANCEL_ONE_STRING_WITH_CHECKBOX:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_CANCEL_TWO_STRING_WITH_CHECKBOX:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_OK_CANCEL_THREE_STRING_WITH_CHECKBOX:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_strPrompt3.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt3", L"text", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ORDERSONG_OK_TWO_STRING:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_str1Color.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"color", varTemp);
			varTemp = m_str2Color.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"color", varTemp);
			varTemp = m_strOKLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnOK", L"label", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ORDERSONG_OK_CANCEL_TWO_STRING:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_strPrompt2.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"text", varTemp);
			varTemp = m_str1Color.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"color", varTemp);
			varTemp = m_str2Color.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt2", L"color", varTemp);
			varTemp = m_strOKLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnOK", L"label", varTemp);
			varTemp = m_strCancelLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnCancel", L"label", varTemp);
		}
		break;
	case NewMsgBox::MSGBOX_ORDERSONG_OK_CANCEL_ONE_STRING:
		{
			CComVariant varTemp = m_strPrompt.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"text", varTemp);
			varTemp = m_str1Color.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"textNewPrompt1", L"color", varTemp);
			varTemp = m_strOKLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnOK", L"label", varTemp);
			varTemp = m_strCancelLabel.GetBuffer();
			m_pSkinEngine->SetSubSkinProp(dlgHwnd, L"btnCancel", L"label", varTemp);
		}
		break;
	default:break;
	}

}

BOOL CMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_Style == NewMsgBox::MSGBOX_OK_CANCEL_ONE_STRING_WITH_CHECKBOX || m_Style == NewMsgBox::MSGBOX_OK_CANCEL_TWO_STRING_WITH_CHECKBOX || NewMsgBox::MSGBOX_OK_CANCEL_THREE_STRING_WITH_CHECKBOX)
	{
		((CButton*)GetDlgItem(IDC_CHECK_NO_PROMPT))->SetCheck(FALSE);
		GetDlgItem(IDC_CHECK_NO_PROMPT)->SetWindowText(m_strCheckBox);
	}
	SetContent();
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CMsgBox::GetIsChecked()
{
	return m_bChecked;
}

void CMsgBox::OnBnClickedCheckNoPrompt()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_NO_PROMPT))->GetCheck())
	{
		m_bChecked = TRUE;
	}
	else
	{
		m_bChecked = FALSE;
	}
}


////////////////////////////  新提示框接口  //////////////////////////////////
int CMsgBox::Show(MSGBOX_STYLE nStyle, LPCTSTR pzPrompt1, LPCTSTR pzPrompt2, LPCTSTR pzPrompt3, LPCTSTR linkLabel, LPCTSTR linkAddress, LPCTSTR pzTitle, LPCTSTR pzCheckBox)
{
    m_Style = nStyle;
	m_strPrompt = pzPrompt1;
	m_strPrompt2 = pzPrompt2;
	m_strPrompt3 = pzPrompt3;
	m_strLinkLabel = linkLabel;
	m_strLinkAddress = linkAddress;
	m_strTitle = pzTitle;
	m_strCheckBox = pzCheckBox;
	CMsgBox::m_bMsgBoxVisible = TRUE;
	if (-1 == DoModal())
	{
		CMsgBox::m_bMsgBoxVisible = FALSE;
	}

	return m_iRet;
}

int CMsgBox::Show(MSGBOX_STYLE nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1, LPCTSTR pzPrompt2, LPCTSTR pzTitle)
{
	m_Style = nStyle;
	m_iconType = nIconType;
	m_strPrompt = pzPrompt1;
	m_strPrompt2 = pzPrompt2;
	m_strTitle = pzTitle;

	CMsgBox::m_bMsgBoxVisible = TRUE;
	if (-1 == DoModal())
	{
		CMsgBox::m_bMsgBoxVisible = FALSE;
	}

	return m_iRet;
}

int CMsgBox::ShowForOrderSong(MSGBOX_STYLE nStyle, LPCTSTR pzString1, LPCTSTR pzString2, LPCTSTR colorString1, LPCTSTR colorString2, LPCTSTR btnOKLabel, LPCTSTR btnCancelLabel, LPCTSTR pzTitle )
{
	m_Style = nStyle;
	m_strPrompt = pzString1;
	m_strPrompt2 = pzString2;
	m_str1Color = colorString1;
	m_str2Color = colorString2;
	m_strOKLabel = btnOKLabel;
	m_strCancelLabel = btnCancelLabel;
	m_strTitle = pzTitle;

	CMsgBox::m_bMsgBoxVisible = TRUE;
	if (-1 == DoModal())
	{
		CMsgBox::m_bMsgBoxVisible = FALSE;
	}

	return m_iRet;
}

int CMsgBox::ShowNonDomodal(INT nStyle, MSGBOX_ICONTYPE nIconType, LPCTSTR pzPrompt1, LPCTSTR pzTitle)
{
	m_Style = nStyle;
	m_iconType = nIconType;
	m_strPrompt = pzPrompt1;
	m_strTitle = pzTitle;
	CMsgBox::m_bMsgBoxVisible = TRUE;
	SetWindowPos(NULL, 0, 0, 250, 150, SWP_SHOWWINDOW);
	CenterWindow(GetParent());
	CComVariant varTitle = m_strTitle.GetBuffer();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTheTitle",L"text",varTitle);
	CComVariant varStyle = m_Style;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textMsgBoxStyle",L"text",varStyle);
	CComVariant varTemp = m_iconType;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textIconType", L"text", varTemp);
	varTemp = m_strPrompt.GetBuffer();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textNewPrompt1", L"text", varTemp);
	NOTIFY_SKIN_HELPER(NOTIFY_SKIN_REFRESH, LONG(0));
	return m_iRet;
}