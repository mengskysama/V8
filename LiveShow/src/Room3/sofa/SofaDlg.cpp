// SofaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SofaDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\screeneffect\ScreenEffectManager.h"
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "utility/expat++/expat_document.h"
#include "..\UserMessageDefine.h"

#define SKIN_ON_CLICK_BTN_GETFLASH	1

// CSofaDlg 对话框
#define FLASH_VERSION	0x90000

IMPLEMENT_DYNAMIC(CSofaDlg, CDialog)

CSofaDlg::CSofaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSofaDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_bVersionLow = false;
	m_bInited = false;
}

CSofaDlg::~CSofaDlg()
{
}

void CSofaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FLASH, m_flash);
}


BEGIN_MESSAGE_MAP(CSofaDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_SKIN_MESSAGE
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CLICK_FLASH, OnClickFlash)
	ON_MESSAGE(WM_FLASH_KILLFOCUS, OnFlashKillFocus)
END_MESSAGE_MAP()


// CSofaDlg 消息处理程序
BEGIN_EVENTSINK_MAP(CSofaDlg, CDialog)
	ON_EVENT(CSofaDlg, IDC_FLASH, 197, CSofaDlg::FlashCallFlash, VTS_BSTR)
	ON_EVENT(CSofaDlg, IDC_FLASH, 150, CSofaDlg::FSCommandFlash, VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

void CSofaDlg::FlashCallFlash(LPCTSTR request)
{
	// TODO: 在此处添加消息处理程序代码
	std::vector<CString> vecParams;
	CString strFunc;
	ParseParams(request, strFunc, vecParams);

	if (strFunc == L"sendGold")
	{
		if (vecParams.size() == 1)
		{
			int n179ID, nIndex, nPrice;
			n179ID = nIndex = nPrice = 0;
			swscanf(vecParams[0], L"%d,%d,%d", &n179ID, &nIndex, &nPrice);

			CWnd * pWnd = GetParent();
			pWnd->SendMessage(WM_CLICK_BUYSOFA, nIndex, nPrice);
		}
	}
	else if(strFunc == L"clickName")
	{
		if (vecParams.size() == 1)
		{
			uint32 un179ID = common::utility::stringhelper::StringToInt(vecParams[0].GetBuffer(0));
			CWnd * pWnd = GetParent();
			pWnd->SendMessage(WM_CLICK_SOFA_NAME, (WPARAM)un179ID, 0);
		}
	}
}

void CSofaDlg::FSCommandFlash(LPCTSTR command, LPCTSTR args)
{
	// TODO: 在此处添加消息处理程序代码
}

BOOL CSofaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	long lVersion = 0;

	try
	{
		lVersion = m_flash.FlashVersion();
	}	
	catch(...)
	{
		lVersion = 0;
	}
	
	if (lVersion < FLASH_VERSION)
	{
		m_bVersionLow = true;
		m_flash.MoveWindow(0,0,0,0);

		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnGetFlash", L"visible", (CComVariant)TRUE);
		return TRUE;
	}

	CRect rc;
	GetClientRect(&rc);

	//m_flash.MoveWindow(rc);

	flash_info* pInfo = ScreenEffectManager::GetFlashInfo(EFFECT_TYPE_SOFA);
	m_flash.LoadMovie(0, pInfo->strFlashPath.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CSofaDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SofaDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

void CSofaDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_flash.GetSafeHwnd() && !m_bVersionLow)
	{
		m_flash.MoveWindow(0, 0, cx, cy);
		UpdateSize(cx);
	}
}

void CSofaDlg::InitSofaList( uint32 unSelf179ID, bool bGuest, CString strInitInfo )
{
	if (m_bVersionLow)
	{
		return;
	}

	if (m_bInited)
	{
		return;
	}

	std::vector<CString> vecParams;
	vecParams.push_back(strInitInfo);
	vecParams.push_back(common::utility::stringhelper::IntToString(unSelf179ID).c_str());
	if (bGuest)
	{
		vecParams.push_back(L"1");
	}
	else
	{
		vecParams.push_back(L"0");
	}	
	CallFunction(L"initData", vecParams);

	m_bInited = true;
}

void CSofaDlg::UpdateSofa( uint8 unIndex, CString strSofaInfo )
{
	if (m_bVersionLow)
	{
		return;
	}

	if (unIndex < 1 || unIndex > 5)
	{
		return;
	}

	if (!m_bInited)
	{
		return;
	}

	std::vector<CString> vecParams;
	vecParams.push_back(strSofaInfo);
	CallFunction(L"updateSeat", vecParams);
}

LRESULT CSofaDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_GETFLASH:
		{
			common::utility::systemhelper::OpenUrl(URL_GETFLASH);
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void CSofaDlg::CallFunction( CString func, std::vector<CString> vecParams )
{
	if (m_bVersionLow)
	{
		return;
	}

	CString strTemp;
	strTemp.Format(L"<invoke name=\"%s\" returntype=\"xml\">", func.GetBuffer(0));
	for (std::vector<CString>::iterator it = vecParams.begin(); it != vecParams.end(); it++)
	{
		CString strParam;
		CString strContent = *it;
		strContent.Replace(_T("<"), _T("&lt;"));
		strContent.Replace(_T(">"), _T("&gt;"));
		//strParam.Format(L"<arguments><string>%s</string>", strContent.GetBuffer(0));
		strParam.Format(L"<arguments><string>%s</string></arguments>", strContent.GetBuffer(0));
		strTemp += strParam;
	}
	strTemp += L"</invoke>";

	m_flash.CallFunction(strTemp.GetBuffer(0));
}

void CSofaDlg::UpdateSofaInfo( CString strSofaInfo )
{
	if (m_bVersionLow)
	{
		return;
	}
}

void CSofaDlg::ParseParams( CString strParam, CString& strFunc, std::vector<CString>& vecParam )
{
	CString strTemp = L"<?xml version=\"1.0\" encoding=\"utf-8\" ?><call>" + strParam + L"</call>";
	common::utility::expat_document xmlDoc;

	std::string strUtf8 = common::utility::stringhelper::UnicodeToUTF8( strTemp.GetBuffer() ) ;
	if (!xmlDoc.load_from_buffer(strUtf8.c_str(), strUtf8.size()))
	{
		return;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	if (!pElement)
	{
		return;
	}
	strFunc = pElement->get_attribute_value(L"name").c_str();

	pElement = pElement->get_first_child_element()->get_first_child_element();
	while (pElement)
	{
		vecParam.push_back(pElement->get_element_contents().c_str());
		pElement = pElement->get_sibling_element();
	}
}

void CSofaDlg::UpdateSize( int cx )
{
	if (m_bVersionLow)
	{
		return;
	}

	if (!m_bInited)
	{
		return;
	}

	std::vector<CString> vecParams;
	vecParams.push_back(common::utility::stringhelper::IntToString(cx).c_str());
	CallFunction(L"moveXY", vecParams);
}

bool CSofaDlg::IsInited()
{
	return m_bInited;
}

LRESULT CSofaDlg::OnClickFlash( WPARAM wParam, LPARAM lParam )
{
	CWnd * pWnd = GetParent();
	pWnd->SendMessage(WM_CLICK_FLASH, 0, 0);
	return S_OK;
}

void CSofaDlg::ClearSofaList()
{
	if (m_bVersionLow)
	{
		return;
	}

	if (!m_bInited)
	{
		return;
	}

	std::vector<CString> vecParams;
	vecParams.push_back(L"0");
	CallFunction(L"clearSeats", vecParams);
}

void CSofaDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;

	CDialog::OnOK();
}

void CSofaDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;

	CDialog::OnCancel();
}

BOOL CSofaDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::PreTranslateMessage(pMsg);
}

void CSofaDlg::HideSendPanel()
{
	if (m_bVersionLow)
	{
		return;
	}

	if (!m_bInited)
	{
		return;
	}

	std::vector<CString> vecParams;
	CallFunction(L"hideSendPanel", vecParams);
}

LRESULT CSofaDlg::OnFlashKillFocus( WPARAM wParam, LPARAM lParam )
{
	HideSendPanel();

	return S_OK;
}