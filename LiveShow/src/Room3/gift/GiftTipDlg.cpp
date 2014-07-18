// GiftTipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "GiftTipDlg.h"
#include "SkinUtil.h"
#include "..\UserMessageDefine.h"
#include "HallDefine.h"
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define SKIN_ON_CLICK_SEND 1
#define SKIN_ON_CLICK_RECHARGE 2
#define SKIN_NOTIFY_CHANGE_GIFT	3

// CGiftTipDlg 对话框

IMPLEMENT_DYNAMIC(CGiftTipDlg, CDialog)

CGiftTipDlg::CGiftTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGiftTipDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	m_pGiftItem = NULL;
	m_pParentWnd = pParent;
	::GetSkinEngine(&m_pSkinEngine);
}

CGiftTipDlg::~CGiftTipDlg()
{
}

void CGiftTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGiftTipDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CGiftTipDlg 消息处理程序

int CGiftTipDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if(NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"GiftTipDlg");
		ASSERT(hr==S_OK && __LINE__);
	}

	return 0;
}

void CGiftTipDlg::Show( IGiftItem * pGiftItem, int nCount/* = 0*/ )
{
	ASSERT(pGiftItem != NULL);
	m_pGiftItem = pGiftItem;
	//ShowWindow(SW_HIDE);
	std::wstring strImagePath = pGiftItem->GetLargeImagePath();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textName", L"text", (CComVariant)pGiftItem->GetRichName().c_str());
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSend", L"label", (CComVariant)pGiftItem->GetStrSendBtn().c_str());
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"absoluteImage", (CComVariant)strImagePath.c_str());
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrice1", L"text", (CComVariant)pGiftItem->GetPrice());
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrice2", L"text", (CComVariant)pGiftItem->GetDstEarn());
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textDesc", L"text", (CComVariant)pGiftItem->GetDesc().c_str());

	//if (pGiftItem->GetCatalogName() == L"奢侈")
	//{
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"width", (CComVariant)190);
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"height", (CComVariant)60);
	//}
	//else if (pGiftItem->GetCatalogName() == L"道具")
	//{
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"width", (CComVariant)64);
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"height", (CComVariant)64);
	//}
	//else
	//{
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"width", (CComVariant)103);
	//	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"height", (CComVariant)103);
	//}
	if (pGiftItem->GetCatalogName() == L"道具")
	{
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"left", (CComVariant)5);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"top", (CComVariant)5);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"width", (CComVariant)64);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"height", (CComVariant)64);

		if (pGiftItem->GetName() == L"喇叭")
		{
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrice1", L"text", (CComVariant)L"10000-60000");
		}

		if (pGiftItem->GetName() == L"印章")
		{
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrice1", L"text", (CComVariant)L"300-6000");
		}
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);

		CComVariant var1, var2;
		m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"originWidth", &var1);
		m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"originHeight", &var2);

		int nPosX = 5 + (rc.Width() - 134 - var1.intVal)/2;
		int nPosY = 5 + (rc.Height() - 30 - var2.intVal)/2;

		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"left", (CComVariant)nPosX);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"top", (CComVariant)nPosY);

		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"width", var1);
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgGiftItem", L"height", var2);
	}

	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_CHANGE_GIFT, (LONG)0);

	if (pGiftItem->GetCatalogName() == L"道具")
	{
		HideDstErn();
		//HideBtnRecharge();
	}
	else
	{
		ShowDstErn();
		//ShowBtnRecharge();
	}

	if (pGiftItem->GetStrPrescription() != L"")
	{
		ShowPrescription(pGiftItem->GetStrPrescription());
	}
	else
	{
		HidePrescription();
	}

	if (nCount != 0)
	{
		//HideSendAndRecharge();
		ShowCount(nCount);
	}
	else
	{
		//ShowSendAndRecharge();
		HideCount();
	}

	ShowWindow(SW_SHOWNOACTIVATE);
}

void CGiftTipDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CGiftTipDlg::OnMouseLeave( WPARAM wParam,LPARAM lParam )
{
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CGiftTipDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_SEND:
		{
			OnClickSend();
		}
		break;
	case SKIN_ON_CLICK_RECHARGE:
		{
			OnClickRecharge();
		}
		break;
	default:break;
	}


	return TRUE;
}

void CGiftTipDlg::OnClickSend()
{
	if (m_pParentWnd != NULL && m_pGiftItem != NULL)
	{
		m_pParentWnd->SendMessage(WM_CLICK_GIFTLIST, 0, (LPARAM)m_pGiftItem);
	}
}

void CGiftTipDlg::OnClickRecharge()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_PAY;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	common::utility::systemhelper::OpenUrl(weburl.m_WebInterfaceUrl) ;
} 

void CGiftTipDlg::HideDstErn()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPriceTitle2", L"visible", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrice2", L"visible", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPriceUnit2", L"visible", (CComVariant)false);
}

void CGiftTipDlg::HideBtnRecharge()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRecharge", L"visible", (CComVariant)false);
}

void CGiftTipDlg::ShowDstErn()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPriceTitle2", L"visible", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrice2", L"visible", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPriceUnit2", L"visible", (CComVariant)true);
}

void CGiftTipDlg::ShowBtnRecharge()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRecharge", L"visible", (CComVariant)true);
}

void CGiftTipDlg::ShowPrescription(std::wstring strPrescription)
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrescription", L"text", (CComVariant)strPrescription.c_str());
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrescriptionTitle", L"visible", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrescription", L"visible", (CComVariant)true);
}

void CGiftTipDlg::HidePrescription()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrescriptionTitle", L"visible", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textPrescription", L"visible", (CComVariant)false);
}

void CGiftTipDlg::ShowCount( int nCount )
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCount", L"text", (CComVariant)nCount);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountTitle", L"visible", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCount", L"visible", (CComVariant)true);
}

void CGiftTipDlg::HideCount()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountTitle", L"visible", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCount", L"visible", (CComVariant)false);
}

void CGiftTipDlg::ShowSendAndRecharge()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSend", L"visible", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRecharge", L"visible", (CComVariant)true);
}

void CGiftTipDlg::HideSendAndRecharge()
{
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSend", L"visible", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnRecharge", L"visible", (CComVariant)false);
}