// StampDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "StampDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\UserMessageDefine.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE                     1
#define  SKIN_ON_CLICK_TAB_STAMP                 2
// CStampDlg 对话框

IMPLEMENT_DYNAMIC(CStampDlg, CDialog)

CStampDlg::CStampDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStampDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_bShowStamp = TRUE;
}

CStampDlg::~CStampDlg()
{
}

void CStampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STAMP, m_stampListCtrl);
}


BEGIN_MESSAGE_MAP(CStampDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CStampDlg 消息处理程序

BOOL CStampDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CStampDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_stampListCtrl.Initialize();
	DWORD dwstyle = m_stampListCtrl.GetExtendedStyle();
	dwstyle = 0;
	m_stampListCtrl.SetExtendedStyle(dwstyle);
	m_stampListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	//	m_stampListCtrl.SetItemCount(20);
	m_stampListCtrl.SetListData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CStampDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"StampDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CStampDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			ShowWindow( SW_HIDE ) ;
		}
		break;
	case SKIN_ON_CLICK_TAB_STAMP:
		{
			OnClickStampTab((int)lParam);
		}
		break;
	default:break;
	}


	return TRUE;
}

void CStampDlg::ShowStamp()
{
	map< int , wstring >::iterator iter ;
	map< int , wstring > mTypeList = GiftManager::GetInstance()->GetStampTypeList() ;
	int i = 0 ;
	for ( iter = mTypeList.begin() ; iter != mTypeList.end() ; ++iter , ++i )
	{
		CComVariant vars[1];
		vars[0] = -1;
		DISPPARAMS dispParams = {vars, 0, 1, 0};
		m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(),L"tabStamp",L"AddTabItem",NULL,&dispParams);

		CComVariant vars2[2];
		vars2[0] =  CString( iter->second.c_str() );
		vars2[1] = i;
		DISPPARAMS dispParams2 = {vars2, 0, 2, 0};
		m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(),L"tabStamp",L"SetTabItemText",NULL,&dispParams2);

	}

	CComVariant vars = 0;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabStamp",L"tabItemSelected",vars);   
}

void CStampDlg::OnClickStampTab(int iTab)
{
	if (iTab >= 0)
	{
		int nCurSel = 0;
		CComVariant var;
		m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(),L"tabStamp",L"tabItemSelected",&var);
		nCurSel = var.intVal;
		if (iTab != nCurSel)
		{
			CComVariant vars = iTab;
			m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"tabStamp",L"tabItemSelected",vars);
			m_stampListCtrl.SetListData(iTab);
		}
	}
}

void CStampDlg::OnSelStamp(int iStampGroup, int iStampIndex)
{
	map< int , wstring >::iterator miter ;
	map< int , wstring > mTypeList = GiftManager::GetInstance()->GetStampTypeList() ;
	int i = 0 ;
	for ( miter = mTypeList.begin() ; miter !=  mTypeList.end() ; ++miter , ++i )
	{
		if ( iStampGroup == i )
		{
			VecGiftItem vStampList;
			GiftManager::GetInstance()->GetStampListByCatalog( miter->second, vStampList );
			if ( iStampIndex < vStampList.size() )
			{
				IGiftItem* pGiftItem = vStampList[iStampIndex] ;
				CWnd * pParentWnd = GetParent();
				if (pParentWnd != NULL)
				{
					pParentWnd->SendMessage( WM_CLICK_STAMPLIST , 0 , pGiftItem->GetID() ) ;
				}
			}
			break;
		}
	}
}

void CStampDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (IsWindowVisible())
	{
		if (m_bShowStamp)
		{
			ShowStamp();
			m_bShowStamp = FALSE;
		}
	}else
	{
		m_bShowStamp = TRUE;
	}
}

void CStampDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	// TODO: 在此处添加消息处理程序代码
}
