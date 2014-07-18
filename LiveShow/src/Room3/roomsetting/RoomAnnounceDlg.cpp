// RoomAnnounceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "RoomAnnounceDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"

#include "..\RoomParentDlg.h"
#include "RoomOption.h"
#include "PreviewDlg.h"

#define  SKIN_ON_CLICK_BTN_PREVIEW      1
#define  SKIN_ON_CLICK_BTN_ADD          2
#define  SKIN_ON_CLICK_BTN_SAVE_MODIFY  3
#define  SKIN_ON_CLICK_BTN_DELETE       4
#define  SKIN_ON_CLICK_BTN_TEACHYOU     5

#define PREVIEW_WIDTH	620
#define PREVIEW_HEIGHT	270
// CRoomAnnounceDlg 对话框

IMPLEMENT_DYNAMIC(CRoomAnnounceDlg, CDialog)

CRoomAnnounceDlg::CRoomAnnounceDlg( CRoomParentDlg* pParentDlg , CWnd* pParent /*=NULL*/)
	: CDialog(CRoomAnnounceDlg::IDD, pParent),
	m_pParentDlg(pParentDlg)
{
	m_pSkinEngine = NULL;
	//m_pInputHtml = NULL;
	m_pPreviewDlg = NULL;
	m_nCurSelItem = -1;
	::GetSkinEngine(&m_pSkinEngine);
}

CRoomAnnounceDlg::~CRoomAnnounceDlg()
{
	//if (m_pInputHtml)
	//{
	//	delete m_pInputHtml;
	//	m_pInputHtml = NULL;
	//}
}

void CRoomAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ROOM_ANNOUNCE, m_listRoomAnnounce);
}


BEGIN_MESSAGE_MAP(CRoomAnnounceDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_NOTIFY(NM_CLICK, IDC_LIST_ROOM_ANNOUNCE, &CRoomAnnounceDlg::OnNMClickListRoomAnnounce)
	ON_NOTIFY(NM_HOVER, IDC_LIST_ROOM_ANNOUNCE, &CRoomAnnounceDlg::OnNMHoverListRoomAnnounce)
	ON_EN_CHANGE(IDC_HTML_ROOM_ANNOUNCE_CONTENT, OnInputChange)
END_MESSAGE_MAP()


// CRoomAnnounceDlg 消息处理程序

int CRoomAnnounceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"RoomAnnounceDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

BOOL CRoomAnnounceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_pInputHtml = new COutInputHtmlEditImpl;
	//m_pInputHtml->SetHtmlEditLogic(&m_InputLogic);
	//m_pInputHtml->Create(this, IDC_HTML_ROOM_ANNOUNCE_CONTENT, CRect(0,0,0,0), WS_CHILD|WS_VISIBLE);
	m_InputCtrl.Create( WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL , CRect(0,0,0,0), this, IDC_HTML_ROOM_ANNOUNCE_CONTENT);
	m_InputCtrl.SetDefaultFont(12);
	m_InputCtrl.SendMessage(EM_SETLANGOPTIONS, 0, 0);

	m_InputCtrl.SetEventMask(ENM_CHANGE);
	//CComVariant var = (long)m_pInputHtml->GetSafeHwnd();
	CComVariant var = (long)m_InputCtrl.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"itemHtmlRoomAnnounce", L"hwnd", var);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);

	SetTeachYouAddr();
	InitRoomAnnounceList();
	InitDlgData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CRoomAnnounceDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_PREVIEW:
		{
			OnClickPreview();
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
	case SKIN_ON_CLICK_BTN_TEACHYOU:
		{
			OnClickTeachYou();
		}
		break;
	default: break;
	}

	return TRUE;
}

//清空输入框中的内容
void CRoomAnnounceDlg::ClearInputCtrlValue() 
{
	m_InputCtrl.SetWindowText(L"") ;
}

void CRoomAnnounceDlg::InitRoomAnnounceList()
{
	DWORD dwStyle = 0;
	LONG lStyle; 
	lStyle = GetWindowLong(m_listRoomAnnounce.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle = lStyle | LVS_REPORT; //设置style 
	SetWindowLong(m_listRoomAnnounce.m_hWnd, GWL_STYLE, lStyle);
	dwStyle = m_listRoomAnnounce.GetExtendedStyle();
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	m_listRoomAnnounce.SetExtendedStyle(dwStyle); //设置扩展风格 
	m_listRoomAnnounce.SetBkColor(RGB(255,255,255));
	m_listRoomAnnounce.SetTextBkColor(RGB(255,255,255));

	m_listRoomAnnounce.InsertColumn(0,L"房间公告内容(点击显示在上方)",LVCFMT_CENTER,370);

}

void CRoomAnnounceDlg::SetTeachYouAddr()
{
	CComVariant var = L"www.51.com/company";
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnTeachYou",L"hyperLinkAddress",var);
}

void CRoomAnnounceDlg::OnClickAdd()
{
	if (m_pParentDlg == NULL)
	{
		return;
	}
	if (m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() == NULL)
	{
		return;
	}
	if (m_pParentDlg != NULL &&
		 (m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem()->GetUserTitleLevel() < core::TITLELEVEL_SECOND_OWNER ) )
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING, L"抱歉，您的权限不够");
		return;
	}

	//CString strContent = m_pInputHtml->GetContentText();
	CString strContent;
	m_InputCtrl.GetWindowText(strContent);
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strContent.GetBuffer(0));
	//if (nLen == 0)
	//{
	//	MessageBox(L"内容不能为空！", L"提示", MB_OK);
	//	return;
	//}

	if (nLen > 300)
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING, L"房间公告不能超过300个字符");
		return;
	}

	if (m_listRoomAnnounce.GetItemCount() >= 3)
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING, L"最多只能保存3条房间公告");
		return;
	}

	int nItem = m_listRoomAnnounce.GetItemCount();
	m_listRoomAnnounce.InsertItem(nItem, strContent);

	//m_nCurSelItem = nItem;
	m_InputCtrl.SetWindowText(L"");
	m_nCurSelItem = -1;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);

	SaveNotice();

	//m_InputLogic.ClearInput();
}

void CRoomAnnounceDlg::OnClickSave()
{
	if (m_pParentDlg == NULL)
	{
		return;
	}
	if (m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() == NULL)
	{
		return;
	}
	if (m_nCurSelItem == -1)
	{
		return;
	}

	if (m_pParentDlg != NULL && 
		 (m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem()->GetUserTitleLevel() < core::TITLELEVEL_SECOND_OWNER) )
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING, L"抱歉，您的权限不够");
		return;
	}

	//CString strContent = m_pInputHtml->GetContentText();
	CString strContent;
	m_InputCtrl.GetWindowText(strContent);
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strContent.GetBuffer(0));
	//if (nLen == 0)
	//{
	//	MessageBox(L"内容不能为空！", L"提示", MB_OK);
	//	return;
	//}

	if (nLen > 300)
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING, L"房间公告不能超过300个字符");
		return;
	}

	m_listRoomAnnounce.SetItemText(m_nCurSelItem, 0, strContent);
	m_InputCtrl.ClearInput();
	m_nCurSelItem = -1;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);

	SaveNotice();

	UpdateData(TRUE);
}

void CRoomAnnounceDlg::OnClickDelete()
{
	if (m_pParentDlg == NULL)
	{
		return;
	}
	if (m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem() == NULL)
	{
		return;
	}
	if (m_nCurSelItem == -1)
	{
		return;
	}

	if (m_pParentDlg != NULL &&
		m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem()->GetUserTitleLevel() < core::TITLELEVEL_SECOND_OWNER)
	{
		C179MsgBox::Show(this, NewMsgBox::MSGBOX_OK_ONE_STRING, L"抱歉，您的权限不够");
		return;
	}

	if (C179MsgBox::Show(this, NewMsgBox::MSGBOX_ICO_OK_CANCEL_ONE_STRING, NewMsgBox::ICONTYPE_WARNING, L"您确定删除本条公告吗？") == RET_OK)
	{
		m_listRoomAnnounce.DeleteItem(m_nCurSelItem);
		SaveNotice();
		m_nCurSelItem = -1;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"btnSaveModify", L"disabled", (CComVariant)true);
	}
	else
	{
		return;
	}
}

void CRoomAnnounceDlg::OnClickTeachYou()
{
	CString strTemp = m_pParentDlg->GetRoomBusiness()->GetTeachAnnounceUrl();
	common::utility::systemhelper::OpenUrl(strTemp.GetBuffer());
}

void CRoomAnnounceDlg::OnNMClickListRoomAnnounce(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*) pNMHDR;
	int nItem = pMListView->iItem;//获取选中项

	if (nItem == -1)
	{
		m_nCurSelItem = -1;
		return;
	}

	m_nCurSelItem = nItem;
	CString strContent = m_listRoomAnnounce.GetItemText(nItem,0);

	if (strContent != L"")
	{
		//m_InputLogic.ClearInput();
		//m_pInputHtml->InsertText(strContent, true);
		m_InputCtrl.SetWindowText(strContent);
	}
	m_listRoomAnnounce.SetFocus();

	*pResult = 0;
}

void CRoomAnnounceDlg::SaveNotice()
{
	std::vector<std::wstring> vecNoticeList;
	for (int i = 0; i < m_listRoomAnnounce.GetItemCount(); i++)
	{
		CString strContent = m_listRoomAnnounce.GetItemText(i,0);
		vecNoticeList.push_back(strContent.GetBuffer(0));
	}
	m_pParentDlg->GetRoomBusiness()->GetRoomOption()->OnSaveRoomNotice( vecNoticeList ) ;
}
BOOL CRoomAnnounceDlg::PreTranslateMessage(MSG* pMsg)
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

void CRoomAnnounceDlg::OnClickPreview()
{
	//CString strContent = m_pInputHtml->GetContentText();
	CString strContent;
	m_InputCtrl.GetWindowText(strContent);
	if (NULL == m_pPreviewDlg)
	{
		SwitchResourceToModule(_T("Room"));
		m_pPreviewDlg = new CPreviewDlg();
		m_pPreviewDlg->Create(CPreviewDlg::IDD,this);
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		m_pPreviewDlg->SetWindowPos(NULL,cx/2 - PREVIEW_WIDTH/2,cy/2 - PREVIEW_HEIGHT/2,PREVIEW_WIDTH,PREVIEW_HEIGHT,SWP_SHOWWINDOW);
		m_pPreviewDlg->CenterWindow();

		m_pPreviewDlg->SetInitContent(strContent.GetBuffer(0));
		return;
	}

	if (NULL != m_pPreviewDlg && m_pPreviewDlg->GetSafeHwnd())
	{
		m_pPreviewDlg->ShowWindow(SW_SHOW);
		m_pPreviewDlg->CenterWindow();
		m_pPreviewDlg->SetContent(strContent.GetBuffer(0));
	}
}

void CRoomAnnounceDlg::InitDlgData()
{
	if ( m_pParentDlg->GetRoomBusiness()->GetRoomOption() == NULL )
	{
		return ;
	}

	m_listRoomAnnounce.DeleteAllItems();
	int nItem = 0;
	std::vector<std::wstring> vecNotice = m_pParentDlg->GetRoomBusiness()->GetRoomOption()->GetRoomNotice();
	for (std::vector<std::wstring>::iterator it = vecNotice.begin(); it != vecNotice.end(); ++it)
	{
		m_listRoomAnnounce.InsertItem(nItem++, (*it).c_str());
	}
}

void CRoomAnnounceDlg::Refresh()
{
	InitDlgData();
}

void CRoomAnnounceDlg::OnNMHoverListRoomAnnounce(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}

void CRoomAnnounceDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}

void CRoomAnnounceDlg::OnInputChange()
{
	CString strContent;
	m_InputCtrl.GetWindowText(strContent);

	if (m_nCurSelItem != -1)
	{
		CString strText = m_listRoomAnnounce.GetItemText(m_nCurSelItem,0);
		if (strContent == strText)
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