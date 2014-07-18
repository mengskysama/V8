// OrderSongDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "OrderSongDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\SendEventManager.h"
#include "utility\SystemHelper.h"
#include "utility\StringHelper.h"
#include "utility\json\json.h"
#include "SongWishDlg.h"
#include "SpotSongDlg.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"

#define  SKIN_ON_CLICK_BTN_CLOSE                      1
//#define  SKIN_ON_CLICK_BTN_SONGWISH                   2
#define  SKIN_ON_CLICK_TABBTN_ORDERED_SONG            3
#define  SKIN_ON_CLICK_TABBTN_SONGLIST                4
#define  SKIN_ON_CLICK_TABBTN_MARKLIST				  5

#define SONG_LIST_HTML			L"resource\\html\\vod\\songlist.html"
#define ORDER_LIST_HTML			L"resource\\html\\vod\\orderlist.html"
#define MARK_LIST_HTML			L"resource\\html\\vod\\evaluatelist.html"

#define UPDATE_ORDER_LIST_TIMER	300099

// COrderSongDlg 对话框

IMPLEMENT_DYNAMIC(COrderSongDlg, CDialog)

COrderSongDlg::COrderSongDlg(CRoomParentDlg* pParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(COrderSongDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pSongWishDlg = NULL;
	m_pSpotSongDlg = NULL;
	m_bSongListChanged = false;
	m_pParentDlg = pParentDlg;
	m_enmListType = LISTTYPE_SONG;

	m_htmlOrderList.SetType(LISTTYPE_ORDER);
	m_htmlSongList.SetType(LISTTYPE_SONG);
	m_htmlMarkList.SetType(LISTTYPE_MARK);
	m_bSongListLoadCompleted = false;
	m_bOrderListLoadCompleted = false;

	m_nCurSongerID = 0;
	m_strCurSong = L"";
}

COrderSongDlg::~COrderSongDlg()
{
	if (m_pSongWishDlg)
	{
		m_pSongWishDlg->DestroyWindow();
		delete m_pSongWishDlg;
		m_pSongWishDlg = NULL;
	}
}

void COrderSongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COrderSongDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_MISSION_COMPLETED, OnLoadHTMLComplete)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COrderSongDlg 消息处理程序

int COrderSongDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"OrderSongDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL COrderSongDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_htmlOrderList.CreateFromStatic(IDC_HTML_ORDER_LIST, this);

	CComVariant var = (LONG)m_htmlOrderList.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemOrderList",L"hwnd",var);

	m_htmlSongList.CreateFromStatic(IDC_HTML_SONG_LIST,this);

	var = (LONG)m_htmlSongList.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemSongList",L"hwnd",var);

	m_htmlMarkList.CreateFromStatic(IDC_HTML_MARK_LIST,this);

	var = (LONG)m_htmlMarkList.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemMarkList",L"hwnd",var);

	SetTimer(UPDATE_ORDER_LIST_TIMER, 20000, NULL);

	InitBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COrderSongDlg::InitBrowser()
{
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + ORDER_LIST_HTML;
	m_htmlOrderList.SetUrl(strUrl);
	m_htmlOrderList.UpdateURL();

	strUrl = common::utility::systemhelper::Get179AppPath() + SONG_LIST_HTML;
	m_htmlSongList.SetUrl(strUrl);
	m_htmlSongList.UpdateURL();

	strUrl = common::utility::systemhelper::Get179AppPath() + MARK_LIST_HTML;
	m_htmlMarkList.SetUrl(strUrl);
	m_htmlMarkList.UpdateURL();

	m_htmlOrderList.ShowWindow(SW_HIDE);
}

BOOL COrderSongDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;

		default: 
			break;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}


LRESULT COrderSongDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_CLOSE:
		{
			//PostMessage(WM_CLOSE, 0, 0);
			if (!CheckSongListChange())
			{
				//PostMessage(WM_CLOSE, NULL, NULL);
				ShowWindow(SW_HIDE);
			}
			else
			{
				int nRet = C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_CANCEL_TWO_STRING, L"您已编辑过歌单，", L"为免操作丢失请先进行保存", L"#0000ff", L"#0000ff", L"确定", L"直接关闭", L"提示");
				if (nRet == RET_CANCEL)
				{
					//PostMessage(WM_CLOSE, NULL, NULL);
					ShowWindow(SW_HIDE);
				}
			}
		}
		break;
	/*case SKIN_ON_CLICK_BTN_SONGWISH:
		{
			ShowSongWishDlg();
		}	
		break;*/
	case SKIN_ON_CLICK_TABBTN_ORDERED_SONG:
		{
			m_enmListType = LISTTYPE_ORDER;
			m_htmlSongList.ShowWindow(SW_HIDE);
			m_htmlMarkList.ShowWindow(SW_HIDE);
			m_htmlOrderList.ShowWindow(SW_SHOW);
		}
		break;
	case SKIN_ON_CLICK_TABBTN_SONGLIST:
		{
			m_enmListType = LISTTYPE_SONG;
			m_htmlOrderList.ShowWindow(SW_HIDE);
			m_htmlMarkList.ShowWindow(SW_HIDE);
			m_htmlSongList.ShowWindow(SW_SHOW);
		}
		break;
	case SKIN_ON_CLICK_TABBTN_MARKLIST:
		{
			m_enmListType = LISTTYPE_MARK;
			m_htmlOrderList.ShowWindow(SW_HIDE);
			m_htmlSongList.ShowWindow(SW_HIDE);
			m_htmlMarkList.ShowWindow(SW_SHOW);
		}
		break;
	default:break;
	}
	return TRUE;
}

void COrderSongDlg::ShowSongWishDlg()
{
	if (m_pSongWishDlg == NULL)
	{
		SwitchResourceToModule(_T("Room"));
		m_pSongWishDlg = new CSongWishDlg;
		m_pSongWishDlg->Create(CSongWishDlg::IDD, this);
	}
	m_pSongWishDlg->ShowWindow(SW_SHOW);
	m_pSongWishDlg->CenterWindow(this);
}

BOOL COrderSongDlg::OnSaveSongList( int nSongerID, LPCTSTR lpszParam )
{	
	std::wstring strParam = lpszParam;
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(common::utility::stringhelper::UnicodeToUTF8(strParam), root))
	{
		return FALSE;
	}

	std::vector<core::Song> vecSongList;
	for (int i = 0; i < root.size(); i++)
	{
		core::Song song;
		song.songName = common::utility::stringhelper::UTF8ToUnicode(root[i]["song"].asString());
		song.songType = root[i]["hot"].asUInt();
		vecSongList.push_back(song);
	}

	return CSendEventManager::SendUpdateSongListEvent(m_pParentDlg->GetRoomID(), vecSongList);
}

BOOL COrderSongDlg::OnAddSong( int nSongerID, LPCTSTR lpszSong )
{
	if (CheckSongListChange())
	{
		int nRet = C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_CANCEL_TWO_STRING, L"您已编辑过歌单，", L"为免操作丢失请先进行保存", L"#0000ff", L"#0000ff", L"确定", L"直接添加", L"提示");
		if (nRet == RET_OK)
		{
			return TRUE;
		}
	}

	COrderSongManager * pOrderMgr = m_pParentDlg->GetRoomBusiness()->GetOrderSongManager();
	std::vector<core::Song> vecSongList;
	pOrderMgr->GetSongListByUin(nSongerID, vecSongList);

	CString strSongName = lpszSong;
	for (std::vector<core::Song>::iterator it = vecSongList.begin(); it != vecSongList.end(); ++it)
	{
		core::Song songinfo = *it;
		if (strSongName.CompareNoCase(songinfo.songName.c_str()) == 0)
		{
			C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_TWO_STRING, L"您输入的歌曲已存在", L"请重新输入", L"#0054ff", L"#0054ff", L"确定", L"", L"提示");
			return TRUE;
		}
	}

	core::Song song;
	song.songName = lpszSong;
	song.songType = 0;
	vecSongList.push_back(song);

	return CSendEventManager::SendUpdateSongListEvent(m_pParentDlg->GetRoomID(), vecSongList);
}

BOOL COrderSongDlg::OnClearSongList( int nSongerID )
{
	int nRet = C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_CANCEL_ONE_STRING, L"确定清空您的点歌单？", L"", L"#0054ff", L"", L"确定", L"取消", L"清空");
	if (nRet == RET_OK)
	{
		std::vector<core::Song> vecSongList;
		return CSendEventManager::SendUpdateSongListEvent(m_pParentDlg->GetRoomID(), vecSongList);
	}

	return TRUE;
}

BOOL COrderSongDlg::OnOrderSong( int nSongerID, LPCTSTR lpszSong )
{
	IUserItem * pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN(nSongerID);
	if (pUserItem == NULL)
	{
		C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_TWO_STRING, L"当前主播已不在房间", L"请选择麦上主播点歌", L"#0054ff", L"#0054ff", L"确定", L"", L"提示");
		return TRUE;
	}
	if (pUserItem != NULL && pUserItem->GetPublicMicIndex() == 0)
	{
		C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_TWO_STRING, L"当前主播已不在麦上", L"请选择麦上主播点歌", L"#0054ff", L"#0054ff", L"确定", L"", L"提示");
		return TRUE;
	}
	ShowSongWishDlg();

	m_pSongWishDlg->SetSongInfo(m_pParentDlg->GetRoomID(), nSongerID, lpszSong);
	//return CSendEventManager::SendOrderSongEvent(m_pParentDlg->GetRoomID(), nSongerID, lpszSong, strWish.GetBuffer(0));
	return TRUE;
}

BOOL COrderSongDlg::OnSongListChange( int nChange )
{
	m_bSongListChanged = (nChange == 1 ? true : false);
	if (!m_bSongListChanged)
	{
		//PostMessage(WM_CLOSE, NULL, NULL);
		ShowWindow(SW_HIDE);
	}
	else
	{
		int nRet = C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_CANCEL_TWO_STRING, L"您已编辑过歌单，", L"为免操作丢失请先进行保存", L"#0000ff", L"#0000ff", L"确定", L"直接关闭", L"提示");
		if (nRet == RET_CANCEL)
		{
			//PostMessage(WM_CLOSE, NULL, NULL);
			ShowWindow(SW_HIDE);
		}
	}
	return TRUE;
}

BOOL COrderSongDlg::OnDecideSong( int nOrderID, int nStat )
{
	bool bAccept = (nStat == 1 ? true : false);
	return CSendEventManager::SendProcessSongEvent(m_pParentDlg->GetRoomID(), nOrderID, bAccept);

	////处理完订单重新获取已点歌曲列表
	//CSendEventManager::SendGetOrderListEvent(m_pParentDlg->GetRoomID());
}

void COrderSongDlg::InitHeadList( uint32 unSelfUin, uint32 unIndex, bool bUpdate/* = false*/)
{
	COrderSongManager * pOrderMgr = m_pParentDlg->GetRoomBusiness()->GetOrderSongManager();
	std::wstring strHeadList = pOrderMgr->GetHeadList();

	_variant_t pResult;
	CStringArray paramArray;
	paramArray.Add(common::utility::stringhelper::IntToString(unSelfUin).c_str());
	paramArray.Add(common::utility::stringhelper::IntToString(pOrderMgr->GetChangedIndex(unIndex)).c_str());
	paramArray.Add(strHeadList.c_str());
	CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString NewVerFile,strPrice;
	NewVerFile.Format(_T("%s%s\\%s"),strModulePath,L"VersionDown",L"NewVersion.ini");//NewVersion.ini最新版本配置文件路径
	GetPrivateProfileString(_T("OtherSet"), _T("SongPrice"), _T("15000"), strPrice.GetBuffer(MAX_PATH), MAX_PATH, NewVerFile);
	paramArray.Add(strPrice);
	strPrice.ReleaseBuffer();
	if (bUpdate)
	{
		paramArray.Add(L"1");
	}
	else
	{
		paramArray.Add(L"0");
	}
	m_htmlSongList.CallJScript(L"vodArtist", paramArray, &pResult);
}

void COrderSongDlg::InitSongList( uint32 unIndex, uint32 unSongerUin, bool bRefresh/* = false*/, bool bSort/* = true*/ )
{
	COrderSongManager * pOrderMgr = m_pParentDlg->GetRoomBusiness()->GetOrderSongManager();
	std::wstring strSongList = pOrderMgr->GetSongListByUin(unSongerUin);

	_variant_t pResult;
	CStringArray paramArray;
	paramArray.Add(common::utility::stringhelper::IntToString(pOrderMgr->GetChangedIndex(unIndex)).c_str());
	paramArray.Add(common::utility::stringhelper::IntToString(unSongerUin).c_str());
	paramArray.Add(strSongList.c_str());
	if (m_unCurSelIndex == unIndex)
	{
		paramArray.Add(L"1");
	}
	else
	{
		if (bRefresh)
		{
			paramArray.Add(L"1");
		}
		else
		{
			paramArray.Add(L"0");
		}
	}
	if (bSort)
	{
		paramArray.Add(L"1");
	}
	else
	{
		paramArray.Add(L"0");
	}
	m_htmlSongList.CallJScript(L"vodSongList", paramArray, &pResult);
}

void COrderSongDlg::InitSongList()
{
	//发送获取主播歌曲列表请求
	if (m_pParentDlg != NULL)
	{
		m_pParentDlg->SendGetSongList();
	}

	IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
	if (pSelfItem != NULL)
	{
		InitHeadList(pSelfItem->GetUserUIN(), m_unCurSelIndex);
	}
}
void COrderSongDlg::InitOrderList( uint32 unSelfUin, bool bActor )
{
	COrderSongManager * pOrderMgr = m_pParentDlg->GetRoomBusiness()->GetOrderSongManager();
	std::wstring strOrderList = pOrderMgr->GetOrderList();
	uint32 unCount = pOrderMgr->GetOrderListCount();
	uint32 unActor = (bActor ? 1 : 0);

	_variant_t pResult;
	CStringArray paramArray;
	paramArray.Add(common::utility::stringhelper::IntToString(unSelfUin).c_str());
	paramArray.Add(common::utility::stringhelper::IntToString(unActor).c_str());
	paramArray.Add(common::utility::stringhelper::IntToString(unCount).c_str());
	paramArray.Add(strOrderList.c_str());
	m_htmlOrderList.CallJScript(L"vodOrderedList", paramArray, &pResult);
}

void COrderSongDlg::InitOrderList()
{
	CSendEventManager::SendGetOrderListEvent(m_pParentDlg->GetRoomID());
}

bool COrderSongDlg::CheckSongListChange()
{
	_variant_t pResult;
	m_htmlSongList.CallJScript(L"vodSongListChange", &pResult);

	return (pResult.intVal == 1);
}

LRESULT COrderSongDlg::OnLoadHTMLComplete( WPARAM wParam,LPARAM lParam )
{
	ENM_ListType enmType = (ENM_ListType)lParam;
	switch (enmType)
	{
	case LISTTYPE_SONG:
		{
			if (!m_bSongListLoadCompleted)
			{
				InitSongList();
				m_bSongListLoadCompleted = true;
			}
		}
		break;
	case LISTTYPE_ORDER:
		{
			if (!m_bOrderListLoadCompleted)
			{
				InitOrderList();
				m_bOrderListLoadCompleted = true;
			}
		}
		break;
	case LISTTYPE_MARK:
		{
			if (!m_bMarkListLoadCompleted)
			{
				InitMarkList();
				m_bMarkListLoadCompleted = true;
			}
		}
		break;
	default:
		break;
	}

	return S_OK;
}

void COrderSongDlg::ClearSongListStat()
{
	_variant_t pResult;
	m_htmlSongList.CallJScript(L"vodSongListStatClear", &pResult);
}

void COrderSongDlg::SetCurSelIndex( uint32 unIndex )
{
	m_unCurSelIndex = unIndex;
}

void COrderSongDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == UPDATE_ORDER_LIST_TIMER)
	{
		CSendEventManager::SendGetOrderListEvent(m_pParentDlg->GetRoomID());
	}

	CDialog::OnTimer(nIDEvent);
}

void COrderSongDlg::ShowSongList()
{
	m_enmListType = LISTTYPE_SONG;
	m_htmlOrderList.ShowWindow(SW_HIDE);
	m_htmlMarkList.ShowWindow(SW_HIDE);
	m_htmlSongList.ShowWindow(SW_SHOW);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnSongList", L"checked", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnOrderedSong", L"checked", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnMarkList", L"checked", (CComVariant)false);
}

void COrderSongDlg::ShowOrderList()
{
	m_enmListType = LISTTYPE_ORDER;
	m_htmlSongList.ShowWindow(SW_HIDE);
	m_htmlMarkList.ShowWindow(SW_HIDE);
	m_htmlOrderList.ShowWindow(SW_SHOW);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnSongList", L"checked", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnOrderedSong", L"checked", (CComVariant)true);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnMarkList", L"checked", (CComVariant)false);
}

void COrderSongDlg::ShowMarkList()
{
	m_enmListType = LISTTYPE_MARK;
	m_htmlSongList.ShowWindow(SW_HIDE);
	m_htmlOrderList.ShowWindow(SW_HIDE);
	m_htmlMarkList.ShowWindow(SW_SHOW);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnSongList", L"checked", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnOrderedSong", L"checked", (CComVariant)false);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"tabBtnMarkList", L"checked", (CComVariant)true);
}

void COrderSongDlg::Init()
{
	ShowSongList();

	if (m_bSongListLoadCompleted)
	{
		InitSongList();
	}

	if (m_bOrderListLoadCompleted)
	{
		InitOrderList();
	}

	if (m_bMarkListLoadCompleted)
	{
		InitMarkList();
	}
}

BOOL COrderSongDlg::OnOrderPriority( int nSongerID, LPCTSTR lpszSong )
{
	IUserItem * pUserItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->SearchUserByUIN(nSongerID);
	if (pUserItem == NULL)
	{
		C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_TWO_STRING, L"当前主播已不在房间", L"请选择麦上主播点歌", L"#0054ff", L"#0054ff", L"确定", L"", L"提示");
		return TRUE;
	}
	if (pUserItem != NULL && pUserItem->GetPublicMicIndex() == 0)
	{
		C179MsgBox::ShowForOrderSong(this, NewMsgBox::MSGBOX_ORDERSONG_OK_TWO_STRING, L"当前主播已不在麦上", L"请选择麦上主播点歌", L"#0054ff", L"#0054ff", L"确定", L"", L"提示");
		return TRUE;
	}

	m_nCurSongerID = nSongerID;
	m_strCurSong = lpszSong;
	ShowSpotSongDlg();
	return TRUE;
}

void COrderSongDlg::ShowSpotSongDlg()
{
	if (m_pSpotSongDlg == NULL)
	{
		SwitchResourceToModule(_T("Room"));
		m_pSpotSongDlg = new CSpotSongDlg;
		m_pSpotSongDlg->Create(CSpotSongDlg::IDD, this);

		CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();
		CString NewVerFile,strPrice;
		NewVerFile.Format(_T("%s%s\\%s"),strModulePath,L"VersionDown",L"NewVersion.ini");//NewVersion.ini最新版本配置文件路径
		GetPrivateProfileString(_T("OtherSet"), _T("SongPrice"), _T("15000"), strPrice.GetBuffer(MAX_PATH), MAX_PATH, NewVerFile);
		strPrice.ReleaseBuffer();
		m_pSpotSongDlg->SetTheSongPrice(strPrice);
	}

	m_pSpotSongDlg->ShowWindow(SW_SHOW);
	m_pSpotSongDlg->CenterWindow(this);
}

void COrderSongDlg::OnPriority( int nCount )
{
	if (m_nCurSongerID != 0 && m_strCurSong != L"")
	{
		ShowSongWishDlg();

		m_pSongWishDlg->SetSongInfo(m_pParentDlg->GetRoomID(), m_nCurSongerID, m_strCurSong, nCount);
	}
}

void COrderSongDlg::InitMarkList()
{
	IUserItem * pSelfItem = m_pParentDlg->GetRoomBusiness()->GetUserManager()->GetSelfUserItem();
	if (pSelfItem != NULL)
	{
		InitMarkList(pSelfItem->GetUserUIN());
	}
}

void COrderSongDlg::InitMarkList( uint32 unSelfUin )
{
	COrderSongManager * pOrderMgr = m_pParentDlg->GetRoomBusiness()->GetOrderSongManager();
	std::wstring strMarkList = pOrderMgr->GetMarkList();
	uint32 unCount = pOrderMgr->GetMarkListCount();

	_variant_t pResult;
	CStringArray paramArray;
	paramArray.Add(common::utility::stringhelper::IntToString(unCount).c_str());
	paramArray.Add(strMarkList.c_str());
	m_htmlMarkList.CallJScript(L"vodEvaluateList", paramArray, &pResult);
}

BOOL COrderSongDlg::OnDecideEvaluate( int nOrderID, int nStat )
{
	uint8 unMark;
	if (nStat == 3)
	{
		unMark = 1;
	}
	else if (nStat == 2 )
	{
		unMark = 2;
	}
	else if (nStat == 1)
	{
		unMark = 0;
	}

	return CSendEventManager::SendMarkSongEvent(m_pParentDlg->GetRoomID(), nOrderID, unMark);
}

BOOL COrderSongDlg::OnOpenOrderSong()
{
	ShowSongList();
	return TRUE;
}