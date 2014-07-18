#include "StdAfx.h"
#include "ScreenEffectManager.h"
#include "CombineGiftDlg.h"
#include "FlyingTextDlg.h"
#include "FlyLuxuryWnd.h"
#include "utility\expat++\expat_document.h"
#include "utility\SystemHelper.h"
#include "utility\StringHelper.h"
#include "utility\URLEncode.h"
#include "..\GlobalRoomDefine.h"
#include "..\RoomParentDlg.h"
#include "..\personalsetting\PersonalConfig.h"

//ScreenEffectManager * ScreenEffectManager::s_pScreenEffectManager = NULL;
std::map<int, combinegift_info*> ScreenEffectManager::s_mapGiftConfig;
std::map<int, flying_text_info*> ScreenEffectManager::s_mapFlyingConfig;
std::map<int, flash_info*> ScreenEffectManager::s_mapFlashConfig;

#define FLYING_TEXT_TOP_OFFSET	138
#define INPUT_HEIGHT	144
#define BORDER_WIDTH	2
#define MAX_FLY_LUXURY_NUM	3
#define CHRISTMAS_COUNT	1225
#define CHRISTMAS_GIFT_ID	0x0013

ScreenEffectManager::ScreenEffectManager(CRoomParentDlg * pRoomParentDlg)
{
	m_pCurCombineGiftDlg = NULL;
	m_pFireworkWnd = NULL;
	m_pSaluteWnd = NULL;
	m_pPlutusWnd = NULL;
	m_pLuckyWnd = NULL;
	m_pEnterEffectWnd = NULL;
	m_pFlyLuxuryWnd = NULL;
	m_pChristmasWnd = NULL;
	m_pOrderSongWnd = NULL;

	m_bCombinGift = false;
	m_bFirework = false;
	m_bSalute = false;
	m_bPlutus = false;
	m_bFlyingText = false;
	m_bLuckyEffect = false;
	m_bEnterEffect = false;
	m_bChristmas = false;
	m_bOrderSong = false;
	m_nFlyLuxuryCount = 0;
	m_pRoomParentDlg = pRoomParentDlg;
}

ScreenEffectManager::~ScreenEffectManager(void)
{
	if (m_pCurCombineGiftDlg != NULL)
	{
		delete m_pCurCombineGiftDlg;
		m_pCurCombineGiftDlg = NULL;
	}

	for (int i = 0; i < 6; i++)
	{
		if(m_FlyTextWindowInfo[i].pFlyingTextDlg != NULL)
		{
			delete m_FlyTextWindowInfo[i].pFlyingTextDlg;
			m_FlyTextWindowInfo[i].pFlyingTextDlg = NULL;
		}
	}

	if (m_pFireworkWnd != NULL)
	{
		if(!m_pFireworkWnd->IsFinish())
		{
			m_pFireworkWnd->Finish();
		}
		m_pFireworkWnd->DestroyWindow();
		delete m_pFireworkWnd;
		m_pFireworkWnd = NULL;
	}

	if (m_pPlutusWnd != NULL)
	{
		if(!m_pPlutusWnd->IsFinish())
		{
			m_pPlutusWnd->Finish();
		}
		m_pPlutusWnd->DestroyWindow();
		delete m_pPlutusWnd;
		m_pPlutusWnd = NULL;
	}

	if (m_pSaluteWnd != NULL)
	{
		if(!m_pSaluteWnd->IsFinish())
		{
			m_pSaluteWnd->Finish();
		}
		m_pSaluteWnd->DestroyWindow();
		delete m_pSaluteWnd;
		m_pSaluteWnd = NULL;
	}

	if (m_pLuckyWnd != NULL)
	{
		if(!m_pLuckyWnd->IsFinish())
		{
			m_pLuckyWnd->Finish();
		}
		m_pLuckyWnd->DestroyWindow();
		delete m_pLuckyWnd;
		m_pLuckyWnd = NULL;
	}

	if (m_pEnterEffectWnd != NULL)
	{
		if(!m_pEnterEffectWnd->IsFinish())
		{
			m_pEnterEffectWnd->Finish();
		}
		m_pEnterEffectWnd->DestroyWindow();
		delete m_pEnterEffectWnd;
		m_pEnterEffectWnd = NULL;
	}

	if (m_pChristmasWnd != NULL)
	{
		if(!m_pChristmasWnd->IsFinish())
		{
			m_pChristmasWnd->Finish();
		}
		m_pChristmasWnd->DestroyWindow();
		delete m_pChristmasWnd;
		m_pChristmasWnd = NULL;
	}

	if (m_pOrderSongWnd != NULL)
	{
		if(!m_pOrderSongWnd->IsFinish())
		{
			m_pOrderSongWnd->Finish();
		}
		m_pOrderSongWnd->DestroyWindow();
		delete m_pOrderSongWnd;
		m_pOrderSongWnd = NULL;
	}

	std::list<Effect_Info*>::iterator it = m_listCombineGift.begin();

	while(it != m_listCombineGift.end())
	{
		delete *it;
		it = m_listCombineGift.erase(it);
	}

	//CFlashWnd::StopAllFlash();
}

bool ScreenEffectManager::LoadCombineGiftConfig()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + COMBINE_GIFT_CONFIG;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();

	while (pElement)
	{
		combinegift_info * pInfo = new combinegift_info;
		pInfo->nGiftType = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"id"));
		if (pElement->get_child_element_contents_by_name(L"width") != L"")
		{
			pInfo->nWidth = common::utility::stringhelper::StringToInt(pElement->get_child_element_contents_by_name(L"width"));
		}
		if (pElement->get_child_element_contents_by_name(L"height") != L"")
		{
			pInfo->nHeight = common::utility::stringhelper::StringToInt(pElement->get_child_element_contents_by_name(L"height"));
		}
		pInfo->nFrames = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"frames"));
		pInfo->nInterval = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"interval"));
		pInfo->nFreezeTime = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"freezetime"));
		if (pElement->get_child_element_contents_by_name(L"bgimage") != L"")
		{
			pInfo->strBackImage = strModulePath + COMBINE_GIFT_DIR + pElement->get_child_element_contents_by_name(L"bgimage");
		}
		if (pElement->get_child_element_contents_by_name(L"frontimage") != L"")
		{
			pInfo->strFrontImage = strModulePath + COMBINE_GIFT_DIR + pElement->get_child_element_contents_by_name(L"frontimage");
		}
		if (pElement->get_child_element_contents_by_name(L"cardimage") != L"")
		{
			pInfo->strCardImage = strModulePath + COMBINE_GIFT_DIR + pElement->get_child_element_contents_by_name(L"cardimage");
		}
		std::wstring strTemp;
		strTemp = pElement->get_child_element_contents_by_name(L"bgpos");
		ParseSinglePos(strTemp.c_str(), pInfo->backPos);
		strTemp = pElement->get_child_element_contents_by_name(L"frontpos");
		ParseSinglePos(strTemp.c_str(), pInfo->frontPos);
		strTemp = pElement->get_child_element_contents_by_name(L"cardpos");
		ParseSinglePos(strTemp.c_str(), pInfo->cardPos);
		strTemp = pElement->get_child_element_contents_by_name(L"point");
		ParsePos(strTemp.c_str(), pInfo->vecPos);
		strTemp = pElement->get_child_element_contents_by_name(L"srcnamepos");
		ParseSinglePos(strTemp.c_str(), pInfo->srcNamePos);
		strTemp = pElement->get_child_element_contents_by_name(L"dstnamepos");
		ParseSinglePos(strTemp.c_str(), pInfo->dstNamePos);
		strTemp = pElement->get_child_element_contents_by_name(L"giftnamepos");
		ParseSinglePos(strTemp.c_str(), pInfo->giftNamePos);
		strTemp = pElement->get_child_element_contents_by_name(L"unitpos");
		ParseSinglePos(strTemp.c_str(), pInfo->unitPos);

		s_mapGiftConfig[pInfo->nGiftType] = pInfo;

		pElement = pElement->get_sibling_element();
	}

	return true;
}

//ScreenEffectManager * ScreenEffectManager::GetInstance()
//{
//	if (s_pScreenEffectManager == NULL)
//	{
//		s_pScreenEffectManager = new ScreenEffectManager();
//	}
//
//	return s_pScreenEffectManager;
//}
//
//void ScreenEffectManager::DeleteInstance()
//{
//	if (s_pScreenEffectManager)
//	{
//		delete s_pScreenEffectManager;
//		s_pScreenEffectManager = NULL;
//	}
//}

void ScreenEffectManager::ParsePos( CString strPos, std::vector<CPoint>& vecPos )
{
	CString strTemp = strPos;
	CString strSinglePos;
	CPoint point;
	while (true)
	{
		if (strTemp.Find(L")") == -1)
		{
			break;
		}
		strSinglePos = strTemp.Left(strTemp.Find(L")") + 1);
		ParseSinglePos(strSinglePos, point);
		vecPos.push_back(point);
		strTemp = strTemp.Right(strTemp.GetLength() - strTemp.Find(L")") - 1);
	}
}

void ScreenEffectManager::ParseSinglePos( CString strPos, CPoint & point )
{
	CString strX, strY;
	strX = strPos.Mid(strPos.Find(L"(") + 1, strPos.Find(L",") - strPos.Find(L"(") - 1);
	strY = strPos.Mid(strPos.Find(L",") + 1, strPos.Find(L")") - strPos.Find(L",") - 1);

	point.x = common::utility::stringhelper::StringToInt(strX.GetBuffer(0));
	point.y = common::utility::stringhelper::StringToInt(strY.GetBuffer(0));
}

void ScreenEffectManager::ShowCombineGift( int nType, IGiftItem * pGiftItem, std::wstring strSrcName, std::wstring strDstName )
{
	//ASSERT(pGiftItem != NULL);
	if (s_mapGiftConfig.find(nType) == s_mapGiftConfig.end())
	{
		return;
	}

	SwitchResourceToModule(_T("Room"));
	combinegift_info * pInfo = s_mapGiftConfig[nType];
	HWND hWnd = GetForegroundWindow();
	m_pCurCombineGiftDlg = new CCombineGiftDlg(pInfo, pGiftItem, strSrcName, strDstName, m_pRoomParentDlg);
	m_pCurCombineGiftDlg->Create(CCombineGiftDlg::IDD, m_pRoomParentDlg);
	//m_pCurCombineGiftDlg->ShowWindow(SW_SHOW);
	SetForegroundWindow(hWnd);
	m_pCurCombineGiftDlg->ShowWindow(SW_SHOWNOACTIVATE);
	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);
	if (pInfo->nWidth != 0 && pInfo->nHeight != 0)
	{
		POINT pt;
		pt.x = rc.left + (rc.Width() - pInfo->nWidth) / 2;
		pt.y = rc.top + (rc.Height() - pInfo->nHeight) / 2;
		m_pCurCombineGiftDlg->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
	}
	else
	{
		m_pCurCombineGiftDlg->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
	}

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pCurCombineGiftDlg->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ProcessSendGift( int nCount, uint16 unGiftID, std::wstring strSrcName, uint32 unSrc179ID, std::wstring strDstName, uint32 unDst179ID )
{
	if(g_RoomModule->IsSpeedUpMode())
	{
		core::CurInfo info = GetCurInfo();
		if(unSrc179ID != info.un179id && unDst179ID != info.un179id)
		{
			return;
		}
	}

	if (nCount == CHRISTMAS_COUNT && unGiftID == CHRISTMAS_GIFT_ID)
	{
		if (m_bChristmas || m_bEnterEffect)
		{
			//加入队列
			Effect_Info * pEffectInfo = new Effect_Info;
			pEffectInfo->enmType = EFFECT_TYPE_CHRISTMAS;
			pEffectInfo->strSrcName = strSrcName;
			pEffectInfo->unSrc179ID = unSrc179ID;
			pEffectInfo->strDstName = strDstName;
			pEffectInfo->unDst179ID = unDst179ID;
			m_listChristmas.push_back(pEffectInfo);
		}
		else
		{
			IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(unGiftID);
			if (pGiftItem != NULL)
			{
				ShowChristmas(nCount, pGiftItem, strSrcName,unSrc179ID, strDstName, unDst179ID);
			}
		}
		return;
	}

	if (s_mapGiftConfig.find(nCount) != s_mapGiftConfig.end())
	{
		m_pRoomParentDlg->FlashRoomWindow() ;
		ProcessCombineGift(nCount, unGiftID, strSrcName, strDstName);
	}
	else
	{
		if (GiftManager::GetInstance()->IsFirework(unGiftID))
		{
			ProcessSendFireWork();
		}
		else if (GiftManager::GetInstance()->IsSalute(unGiftID))
		{
			ProcessSendSalute();
		}
	}
}

void ScreenEffectManager::OnCycleTrigger()
{
	if (m_bCombinGift)
	{
		if (m_pCurCombineGiftDlg != NULL && m_pCurCombineGiftDlg->IsFinish())
		{
			m_pCurCombineGiftDlg->DestroyWindow();
			delete m_pCurCombineGiftDlg;
			m_pCurCombineGiftDlg = NULL;

			m_bCombinGift = false;
		}
	}
	else
	{
		if (m_listCombineGift.size() != 0)
		{
			Effect_Info * pEffectInfo = m_listCombineGift.front();
			m_listCombineGift.pop_front();

			IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(pEffectInfo->unGiftID);
			//ASSERT(pGiftItem != NULL);
			if (pGiftItem != NULL)
			{
				ShowCombineGift(pEffectInfo->nGiftCount, pGiftItem, pEffectInfo->strSrcName, pEffectInfo->strDstName);
				m_bCombinGift = true;
			}

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bFirework)
	{
		if (m_pFireworkWnd != NULL && m_pFireworkWnd->IsFinish())
		{
			m_pFireworkWnd->DestroyWindow();
			delete m_pFireworkWnd;
			m_pFireworkWnd = NULL;
			m_bFirework = false;
		}
	}
	else
	{
		if (m_listFirework.size() != 0)
		{
			Effect_Info * pEffectInfo = m_listFirework.front();
			m_listFirework.pop_front();

			ShowFirework();

			m_bFirework = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bSalute)
	{
		if (m_pSaluteWnd != NULL && m_pSaluteWnd->IsFinish())
		{
			m_pSaluteWnd->DestroyWindow();
			delete m_pSaluteWnd;
			m_pSaluteWnd = NULL;
			m_bSalute = false;
		}
	}
	else
	{
		if (m_listSalute.size() != 0)
		{
			Effect_Info * pEffectInfo = m_listSalute.front();
			m_listSalute.pop_front();

			ShowSalute();

			m_bSalute = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bPlutus)
	{
		if (m_pPlutusWnd != NULL && m_pPlutusWnd->IsFinish())
		{
			m_pPlutusWnd->DestroyWindow();
			delete m_pPlutusWnd;
			m_pPlutusWnd = NULL;
			m_bPlutus = false;
		}
	}
	else
	{
		if (m_listPlutus.size() != 0)
		{
			Effect_Info * pEffectInfo = m_listPlutus.front();
			m_listPlutus.pop_front();

			ShowPlutus();

			m_bPlutus = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bLuckyEffect)
	{
		if (m_pLuckyWnd != NULL && m_pLuckyWnd->IsFinish())
		{
			m_pLuckyWnd->DestroyWindow();
			delete m_pLuckyWnd;
			m_pLuckyWnd = NULL;
			m_bLuckyEffect = false;
		}
	}
	else
	{
		if (m_listLucky.size() != 0)
		{
			Effect_Info * pEffectInfo = m_listLucky.front();
			m_listLucky.pop_front();

			ShowLuckyEffect(pEffectInfo->enmType, pEffectInfo->strSrcName, pEffectInfo->strDstName);

			m_bLuckyEffect = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bEnterEffect)
	{	
		if (m_pEnterEffectWnd != NULL && m_pEnterEffectWnd->IsFinish())
		{
			m_pEnterEffectWnd->DestroyWindow();
			delete m_pEnterEffectWnd;
			m_pEnterEffectWnd = NULL;
			m_bEnterEffect = false;
		}
	}
	else
	{
		if (m_listEnterEffect.size() != 0)
		{
			Effect_Info * pEffectInfo = m_listEnterEffect.front();
			m_listEnterEffect.pop_front();

			ShowEnterEffect(pEffectInfo->enmType, pEffectInfo->strSrcName, pEffectInfo->strDstName);

			m_bEnterEffect = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bChristmas)
	{
		if (m_pChristmasWnd != NULL && m_pChristmasWnd->IsFinish())
		{
			m_pChristmasWnd->DestroyWindow();
			delete m_pChristmasWnd;
			m_pChristmasWnd = NULL;
			m_bChristmas = false;
		}
	}
	else
	{
		if (m_listChristmas.size() != 0 && !m_bEnterEffect)
		{
			Effect_Info * pEffectInfo = m_listChristmas.front();
			m_listChristmas.pop_front();

			ShowChristmas(CHRISTMAS_COUNT, NULL, pEffectInfo->strSrcName, pEffectInfo->unSrc179ID, pEffectInfo->strDstName, pEffectInfo->unDst179ID);

			m_bChristmas = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}

	if (m_bOrderSong)
	{
		if (m_pOrderSongWnd != NULL && m_pOrderSongWnd->IsFinish())
		{
			m_pOrderSongWnd->DestroyWindow();
			delete m_pOrderSongWnd;
			m_pOrderSongWnd = NULL;
			m_bOrderSong = false;
		}
	}else
	{
		if (m_listOrderSong.size() != 0 && !m_bEnterEffect)
		{
			OrderSong_Info * pSongInfo = m_listOrderSong.front();
			m_listOrderSong.pop_front();

			//ShowChristmas(CHRISTMAS_COUNT, NULL, pEffectInfo->strSrcName, pEffectInfo->unSrc179ID, pEffectInfo->strDstName, pEffectInfo->unDst179ID);
			ShowOrderSong(pSongInfo->strSrcName, pSongInfo->strDstName, pSongInfo->unSrc179ID, pSongInfo->unDst179ID, pSongInfo->strFlyingText, pSongInfo->strSongWish);
			m_bOrderSong = true;

			delete pSongInfo;
			pSongInfo = NULL;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (m_FlyTextWindowInfo[i].bInUse)
		{
			if (m_FlyTextWindowInfo[i].bFlyText)
			{
				if (m_FlyTextWindowInfo[i].pFlyingTextDlg != NULL && m_FlyTextWindowInfo[i].pFlyingTextDlg->GetSafeHwnd() && m_FlyTextWindowInfo[i].pFlyingTextDlg->IsFinish())
				{
					m_FlyTextWindowInfo[i].pFlyingTextDlg->DestroyWindow();
					delete m_FlyTextWindowInfo[i].pFlyingTextDlg;
					m_FlyTextWindowInfo[i].pFlyingTextDlg = NULL;
					m_FlyTextWindowInfo[i].bInUse = false;
				}
			}
			else
			{
				if (m_FlyTextWindowInfo[i].pFlyLuxuryWnd != NULL && m_FlyTextWindowInfo[i].pFlyLuxuryWnd->GetSafeHwnd() && m_FlyTextWindowInfo[i].pFlyLuxuryWnd->IsFinish())
				{
					m_FlyTextWindowInfo[i].pFlyLuxuryWnd->DestroyWindow();
					delete m_FlyTextWindowInfo[i].pFlyLuxuryWnd;
					m_FlyTextWindowInfo[i].pFlyLuxuryWnd = NULL;
					m_FlyTextWindowInfo[i].bInUse = false;
					m_nFlyLuxuryCount--;
				}
			}
		}
		//if (m_FlyTextWindowInfo[i].bInUse && m_FlyTextWindowInfo[i].pFlyingTextDlg != NULL && m_FlyTextWindowInfo[i].pFlyingTextDlg->GetSafeHwnd() && m_FlyTextWindowInfo[i].pFlyingTextDlg->IsFinish())
		//{
		//	m_FlyTextWindowInfo[i].pFlyingTextDlg->DestroyWindow();
		//	delete m_FlyTextWindowInfo[i].pFlyingTextDlg;
		//	m_FlyTextWindowInfo[i].pFlyingTextDlg = NULL;
		//	m_FlyTextWindowInfo[i].bInUse = false;
		//}
	}

	if (m_listFlyLuxury.size() != 0 && m_nFlyLuxuryCount < MAX_FLY_LUXURY_NUM)
	{
		int nIndex = FindFreeFlyingTextWindowIndex();
		if (nIndex != -1)
		{
			Luxury_Info * pLuxuryInfo = m_listFlyLuxury.front();
			m_listFlyLuxury.pop_front();

			ShowFlyLuxury(pLuxuryInfo->strSrcName.c_str(), pLuxuryInfo->strDstName.c_str(), pLuxuryInfo->unSrc179ID, pLuxuryInfo->unDst179ID, pLuxuryInfo->unGiftID, pLuxuryInfo->unCount, nIndex);

			//m_bFlyingText = true;

			delete pLuxuryInfo;
			pLuxuryInfo = NULL;
		}
	}

	if (m_listFlyingText.size() != 0)
	{
		int nIndex = FindFreeFlyingTextWindowIndex();
		if (nIndex != -1)
		{
			Effect_Info * pEffectInfo = m_listFlyingText.front();
			m_listFlyingText.pop_front();

			ShowFlyingText(pEffectInfo->strSrcName, pEffectInfo->strFlyingText, (ENM_FlyingText_Type)pEffectInfo->nFlyingTextType, nIndex);

			//m_bFlyingText = true;

			delete pEffectInfo;
			pEffectInfo = NULL;
		}
	}
}

void ScreenEffectManager::ShowFlyingText( std::wstring strNickName, std::wstring strText, ENM_FlyingText_Type enmType, int nIndex )
{
	//CreateFlyingTextWindowInfo();
	SwitchResourceToModule(_T("Room"));
	if (m_FlyTextWindowInfo[nIndex].pFlyingTextDlg == NULL)
	{
		//对话框创建的时候会SetForegroundWindow，所以创建之前先保存当前的FroegroundWindow，创建完再设置回去。这样 ShowWindow(SW_SHOWNOACTIVATE) 才会有效
		HWND hWnd = GetForegroundWindow();
		m_FlyTextWindowInfo[nIndex].pFlyingTextDlg = new CFlyingTextDlg(m_pRoomParentDlg);
		m_FlyTextWindowInfo[nIndex].pFlyingTextDlg->Create(CFlyingTextDlg::IDD, m_pRoomParentDlg);
		SetForegroundWindow(hWnd);
		m_FlyTextWindowInfo[nIndex].bFlyText = true;
	}

	//int cx = GetSystemMetrics(SM_CXSCREEN);
	//int cy = GetSystemMetrics(SM_CYSCREEN);
	CRect rcParent;
	m_pRoomParentDlg->GetWindowRect(&rcParent);
	//m_pFlyingTextDlg->MoveWindow(rcParent.left, rcParent.top + FLYING_TEXT_TOP_OFFSET, rcParent.Width(), rcParent.Height() - FLYING_TEXT_TOP_OFFSET - INPUT_HEIGHT);
	m_FlyTextWindowInfo[nIndex].pFlyingTextDlg->MoveWindow(m_FlyTextWindowInfo[nIndex].nPosX, m_FlyTextWindowInfo[nIndex].nPosY, m_FlyTextWindowInfo[nIndex].nWidth, m_FlyTextWindowInfo[nIndex].nHeight);
	m_FlyTextWindowInfo[nIndex].pFlyingTextDlg->ShowWindow(SW_SHOWNOACTIVATE);
	m_FlyTextWindowInfo[nIndex].pFlyingTextDlg->Show(strNickName, strText, enmType);

	CString strTemp;
	strTemp.Format(L"ScreenEffectManager::ShowFlyingText left = %d, top = %d, width = %d, height = %d type = %d", rcParent.left, rcParent.top + FLYING_TEXT_TOP_OFFSET, rcParent.Width(), rcParent.Height() - FLYING_TEXT_TOP_OFFSET - INPUT_HEIGHT, enmType);
	OutputDebugString(strTemp);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_FlyTextWindowInfo[nIndex].pFlyingTextDlg->ShowWindow(SW_HIDE);
	}
}

DWORD ScreenEffectManager::ParseStringColor( std::wstring strColor )
{
	CString strVar = strColor.c_str();
	const int len = strVar.GetLength();
	if (len <= 0)
		return FALSE;
	if (strVar.GetAt(0) == _T('#')) {
		DWORD value = 0;
		int base = 1;
		for (int i=len-1; i>=1; i--){
			if ( strVar.GetAt(i) >= _T('0') && strVar.GetAt(i) <= _T('9') ){
				value += (strVar.GetAt(i) - _T('0')) * base;
				base *= 16;
			}else if ( strVar.GetAt(i) >= _T('a') && strVar.GetAt(i) <= _T('f') ){
				value += (strVar.GetAt(i) - _T('a') + 10) * base;
				base *= 16;
			}else if ( strVar.GetAt(i) >= _T('A') && strVar.GetAt(i) <= _T('F') ){
				value += (strVar.GetAt(i) - _T('A') + 10) * base;
				base *= 16;
			}else{
				return 0;
			}
		}
		return value;
	} else if (len > 1 && 
		strVar.GetAt(0) == _T('0') &&
		strVar.GetAt(1) == _T('x')) {
			DWORD value = 0;
			int base = 1;
			for (int i=len-1; i>=2; i--){
				if ( strVar.GetAt(i) >= _T('0') && strVar.GetAt(i) <= _T('9') ){
					value += (strVar.GetAt(i) - _T('0')) * base;
					base *= 16;
				}else if ( strVar.GetAt(i) >= _T('a') && strVar.GetAt(i) <= _T('f') ){
					value += (strVar.GetAt(i) - _T('a') + 10) * base;
					base *= 16;
				}else if ( strVar.GetAt(i) >= _T('A') && strVar.GetAt(i) <= _T('F') ){
					value += (strVar.GetAt(i) - _T('A') + 10) * base;
					base *= 16;
				}else{
					return 0;
				}
			}
			return value;
	}

	return 0;
}

bool ScreenEffectManager::LoadFlyingTextConfig()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + FLYING_TEXT_CONFIG;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while (pElement)
	{
		flying_text_info * pInfo = new flying_text_info;
		pInfo->nFlyingType = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"id"));

		common::utility::expat_element * pImageElement = pElement->get_element_by_name(L"Images");
		if (pImageElement)
		{
			common::utility::expat_element * pImageChildElement = pImageElement->get_first_child_element();
			while (pImageChildElement)
			{
				std::wstring strPath = pImageChildElement->get_attribute_value(L"path");
				if (strPath != L"")
				{
					strPath = strModulePath + COMBINE_GIFT_DIR + strPath;
					pInfo->vecStrImagePath.push_back(strPath);
				}

				pImageChildElement = pImageChildElement->get_sibling_element();
			}
		}

		common::utility::expat_element * pChildElement = pElement->get_element_by_name(L"NickName");
		pInfo->stNickName.strFontFamily = pChildElement->get_attribute_value(L"fontfamily");
		pInfo->stNickName.nFontSize = common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"fontsize"));
		pInfo->stNickName.dwBeginColor = ParseStringColor(pChildElement->get_attribute_value(L"begincolor"));
		pInfo->stNickName.dwEndColor = ParseStringColor(pChildElement->get_attribute_value(L"endcolor"));
		if (pChildElement->get_attribute_value(L"strokecolor") != L"")
		{
			pInfo->stNickName.dwStrokeColor = ParseStringColor(pChildElement->get_attribute_value(L"strokecolor"));
			pInfo->stNickName.bStroke = true;
		}
		else
		{
			pInfo->stNickName.bStroke = false;
		}
		pInfo->stNickName.bBold = (pChildElement->get_attribute_value(L"bold") == L"true" ? true : false);
		pInfo->stNickName.bItalic = (pChildElement->get_attribute_value(L"italic") == L"true" ? true : false);
		pInfo->stNickName.bUnderline = (pChildElement->get_attribute_value(L"underline") == L"true" ? true : false);
		pInfo->stNickName.nGradientMode = common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"gradientmode"));

		pChildElement = pElement->get_element_by_name(L"Say");
		pInfo->stSay.strFontFamily = pChildElement->get_attribute_value(L"fontfamily");
		pInfo->stSay.nFontSize = common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"fontsize"));
		pInfo->stSay.dwBeginColor = ParseStringColor(pChildElement->get_attribute_value(L"begincolor"));
		pInfo->stSay.dwEndColor = ParseStringColor(pChildElement->get_attribute_value(L"endcolor"));
		if (pChildElement->get_attribute_value(L"strokecolor") != L"")
		{
			pInfo->stSay.dwStrokeColor = ParseStringColor(pChildElement->get_attribute_value(L"strokecolor"));
			pInfo->stSay.bStroke = true;
		}
		else
		{
			pInfo->stSay.bStroke = false;
		}
		pInfo->stSay.bBold = (pChildElement->get_attribute_value(L"bold") == L"true" ? true : false);
		pInfo->stSay.bItalic = (pChildElement->get_attribute_value(L"italic") == L"true" ? true : false);
		pInfo->stSay.bUnderline = (pChildElement->get_attribute_value(L"underline") == L"true" ? true : false);
		pInfo->stSay.nGradientMode = common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"gradientmode"));

		pChildElement = pElement->get_element_by_name(L"Content");
		pInfo->stContent.strFontFamily = pChildElement->get_attribute_value(L"fontfamily");
		pInfo->stContent.nFontSize = common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"fontsize"));
		pInfo->stContent.dwBeginColor = ParseStringColor(pChildElement->get_attribute_value(L"begincolor"));
		pInfo->stContent.dwEndColor = ParseStringColor(pChildElement->get_attribute_value(L"endcolor"));
		if (pChildElement->get_attribute_value(L"strokecolor") != L"")
		{
			pInfo->stContent.dwStrokeColor = ParseStringColor(pChildElement->get_attribute_value(L"strokecolor"));
			pInfo->stContent.bStroke = true;
		}
		else
		{
			pInfo->stContent.bStroke = false;
		}
		if (pChildElement->get_attribute_value(L"shadowcolor") != L"")
		{
			pInfo->stContent.dwShadowColor = ParseStringColor(pChildElement->get_attribute_value(L"shadowcolor"));
			pInfo->stContent.bShadow = true;
		}
		else
		{
			pInfo->stContent.bShadow = false;
		}
		pInfo->stContent.bBold = (pChildElement->get_attribute_value(L"bold") == L"true" ? true : false);
		pInfo->stContent.bItalic = (pChildElement->get_attribute_value(L"italic") == L"true" ? true : false);
		pInfo->stContent.bUnderline = (pChildElement->get_attribute_value(L"underline") == L"true" ? true : false);
		pInfo->stContent.nGradientMode = common::utility::stringhelper::StringToInt(pChildElement->get_attribute_value(L"gradientmode"));

		s_mapFlyingConfig[pInfo->nFlyingType] = pInfo;

		pElement = pElement->get_sibling_element();
	}

	return true;
}

flying_text_info * ScreenEffectManager::GetFlyingInfo( ENM_FlyingText_Type enmType )
{
	if (s_mapFlyingConfig.find((int)enmType) != s_mapFlyingConfig.end())
	{
		return s_mapFlyingConfig[(int)enmType];
	}

	return NULL;
}

void ScreenEffectManager::Init()
{
	LoadCombineGiftConfig();
	LoadFlyingTextConfig();
	LoadFlashRes();
}

void ScreenEffectManager::ProcessFlyingText( std::wstring strSrcName, std::wstring strText, int nType )
{
	m_pRoomParentDlg->FlashRoomWindow() ;
	ASSERT(nType != 0);
	int nIndex = FindFreeFlyingTextWindowIndex();
	if (nIndex == -1)
	{
		//加入队列
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_FLYINGTEXT;
		pEffectInfo->strSrcName = strSrcName;
		pEffectInfo->strFlyingText = strText;
		pEffectInfo->nFlyingTextType = nType;
		m_listFlyingText.push_back(pEffectInfo);

		OutputDebugString(L"ScreenEffectManager::ProcessFlyingText TrackFull");

		return;
	}
	else
	{
		ShowFlyingText(strSrcName, strText, (ENM_FlyingText_Type)nType, nIndex);
	}

}

void ScreenEffectManager::ProcessSendFireWork()
{
	if (m_bFirework)
	{
		//加入队列
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_FIREWORK;
		m_listFirework.push_back(pEffectInfo);
		return;
	}
	else
	{
		//播放
		ShowFirework();
		m_bFirework = true;
	}
}

void ScreenEffectManager::ProcessSendSalute()
{
	if (m_bSalute)
	{
		//加入队列
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_SALUTE;
		m_listSalute.push_back(pEffectInfo);
		return;
	}
	else
	{
		//播放
		ShowSalute();
		m_bSalute = true;
	}
}

void ScreenEffectManager::ProcessCombineGift( int nCount, uint16 unGiftID, std::wstring strSrcName, std::wstring strDstName )
{
	if (m_bCombinGift)
	{
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_COMBINEGIFT;
		pEffectInfo->unGiftID = unGiftID;
		pEffectInfo->nGiftCount = nCount;
		pEffectInfo->strSrcName = strSrcName;
		pEffectInfo->strDstName = strDstName;

		m_listCombineGift.push_back(pEffectInfo);
		return;
	}
	else
	{
		IGiftItem * pGiftItem = GiftManager::GetInstance()->SearchGiftByID(unGiftID);
		//ASSERT(pGiftItem != NULL);
		if (pGiftItem != NULL)
		{
			ShowCombineGift(nCount, pGiftItem, strSrcName, strDstName);
			m_bCombinGift = true;
		}
	}
}

void ScreenEffectManager::ProcessSendPlutus()
{
	if (m_bPlutus)
	{
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_PLUTUS;
		m_listPlutus.push_back(pEffectInfo);

		return;
	}
	else
	{
		ShowPlutus();
		m_bPlutus = true;
	}
}

void ScreenEffectManager::ShowFirework()
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pFireworkWnd == NULL)
	{
		m_pFireworkWnd = new CFlashWnd;
	}

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}

	flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_FIREWORK];

	if (!m_pFireworkWnd->Play(pInfo->nWidth, pInfo->nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pFireworkWnd->DestroyWindow();
		delete m_pFireworkWnd;
		m_pFireworkWnd = NULL;
		m_bFirework = false;
		return;
	}

	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);
	POINT pt;
	pt.x = rc.left + (rc.Width() - pInfo->nWidth) / 2;
	pt.y = rc.top + rc.Height() - pInfo->nHeight - INPUT_HEIGHT;
	::SetWindowPos(m_pFireworkWnd->m_hWnd, hWnd, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pFireworkWnd->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ShowSalute()
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pSaluteWnd == NULL)
	{
		m_pSaluteWnd = new CFlashWnd;
	}

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}

	flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_SALUTE];

	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);

	int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rc.Width();
	int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rc.Height();

	if (!m_pSaluteWnd->Play(nWidth, nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pSaluteWnd->DestroyWindow();
		delete m_pSaluteWnd;
		m_pSaluteWnd = NULL;
		m_bSalute = false;
		return;
	}

	//CRect rc;
	//m_pRoomParentDlg->GetWindowRect(&rc);
	POINT pt;
	//pt.x = rc.left + (rc.Width() - nWidth) / 2;
	//pt.y = rc.top + rc.Height() - nHeight - INPUT_HEIGHT;
	pt.x = rc.left + (rc.Width() - nWidth) / 2;
	pt.y = rc.top + (rc.Height() - nHeight) / 2;
	::SetWindowPos(m_pSaluteWnd->m_hWnd, hWnd, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);
	//m_pSaluteWnd->SetParent(m_pRoomParentDlg);	//导致部分Window7和Vista系统无法显示Flash

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pSaluteWnd->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ShowPlutus()
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pPlutusWnd == NULL)
	{
		m_pPlutusWnd = new CFlashWnd;
	}

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}

	flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_PLUTUS];
	if (!m_pPlutusWnd->Play(pInfo->nWidth, pInfo->nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pPlutusWnd->DestroyWindow();
		delete m_pPlutusWnd;
		m_pPlutusWnd = NULL;
		m_bPlutus = false;
		return;
	}

	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);
	POINT pt;
	pt.x = rc.left + (rc.Width() - pInfo->nWidth) / 2;
	pt.y = rc.top + rc.Height() - pInfo->nHeight - INPUT_HEIGHT;
	::SetWindowPos(m_pPlutusWnd->m_hWnd, hWnd, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pPlutusWnd->ShowWindow(SW_HIDE);
	}
}

bool ScreenEffectManager::LoadFlashRes()
{
	common::utility::expat_document xmlDoc;

	std::wstring strModulePath = common::utility::systemhelper::Get179AppPath();

	common::CURLEncode urlEncode;
	strModulePath = urlEncode.URLEncodeLocalPath(strModulePath);

	std::wstring strXmlPath = strModulePath + FLASH_CONFIG;

	if (!xmlDoc.load_from_file(strXmlPath.c_str()))
	{
		return false;
	}

	common::utility::expat_element * pRoot = xmlDoc.get_root();
	if (!pRoot)
	{
		return false;
	}

	common::utility::expat_element * pElement = pRoot->get_first_child_element();
	while (pElement)
	{
		flash_info * pInfo = new flash_info;
		pInfo->nType = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"type"));
		pInfo->nWidth = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"width"));
		pInfo->nHeight = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"height"));
		pInfo->nInterval = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"circle"));
		pInfo->nPlayTime = common::utility::stringhelper::StringToInt(pElement->get_attribute_value(L"time"));
		pInfo->strFlashPath = strModulePath + FLASH_PATH + pElement->get_attribute_value(L"file");

		pElement = pElement->get_sibling_element();
		s_mapFlashConfig[pInfo->nType] = pInfo;
	}

	return true;
}

void ScreenEffectManager::Release()
{
	for (std::map<int, combinegift_info*>::iterator it = s_mapGiftConfig.begin(); it != s_mapGiftConfig.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	s_mapGiftConfig.clear();

	for (std::map<int, flying_text_info*>::iterator it = s_mapFlyingConfig.begin(); it != s_mapFlyingConfig.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	s_mapFlyingConfig.clear();

	for (std::map<int, flash_info*>::iterator it = s_mapFlashConfig.begin(); it != s_mapFlashConfig.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	s_mapFlashConfig.clear();
}

void ScreenEffectManager::OnMoving( LPRECT pRect )
{
	if (m_pCurCombineGiftDlg != NULL && m_pCurCombineGiftDlg->GetSafeHwnd() != NULL && !m_pCurCombineGiftDlg->IsFinish())
	{
		CRect rc;
		CRect rcParent(pRect);
		m_pCurCombineGiftDlg->GetWindowRect(&rc);
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - rc.Width()) / 2;
		pt.y = rcParent.top + (rcParent.Height() - rc.Height()) / 2;
		m_pCurCombineGiftDlg->MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());
		//m_pCurCombineGiftDlg->SetWindowPos(NULL, pt.x, pt.y, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}

	CreateFlyingTextWindowInfo();
	for (int i = 0; i < 6; i++)
	{
		if (m_FlyTextWindowInfo[i].bFlyText)
		{
			if (m_FlyTextWindowInfo[i].pFlyingTextDlg != NULL && m_FlyTextWindowInfo[i].pFlyingTextDlg->GetSafeHwnd() && !m_FlyTextWindowInfo[i].pFlyingTextDlg->IsFinish())
			{
				m_FlyTextWindowInfo[i].pFlyingTextDlg->MoveWindow(m_FlyTextWindowInfo[i].nPosX, m_FlyTextWindowInfo[i].nPosY, m_FlyTextWindowInfo[i].nWidth, m_FlyTextWindowInfo[i].nHeight);
				if (!m_FlyTextWindowInfo[i].pFlyingTextDlg->IsWindowVisible())
				{
					m_FlyTextWindowInfo[i].pFlyingTextDlg->ShowWindow(SW_SHOW);
				}
			}
		}
		else
		{
			if (m_FlyTextWindowInfo[i].pFlyLuxuryWnd != NULL && m_FlyTextWindowInfo[i].pFlyLuxuryWnd->GetSafeHwnd() && !m_FlyTextWindowInfo[i].pFlyLuxuryWnd->IsFinish())
			{
				m_FlyTextWindowInfo[i].pFlyLuxuryWnd->MoveWindow(m_FlyTextWindowInfo[i].nPosX, m_FlyTextWindowInfo[i].nPosY, m_FlyTextWindowInfo[i].nWidth, m_FlyTextWindowInfo[i].nHeight);
				if (!m_FlyTextWindowInfo[i].pFlyLuxuryWnd->IsWindowVisible())
				{
					m_FlyTextWindowInfo[i].pFlyLuxuryWnd->ShowWindow(SW_SHOW);
				}
			}
		}
	}

	CRect rcParent;
	m_pRoomParentDlg->GetWindowRect(&rcParent);
	if (m_pFireworkWnd != NULL && m_pFireworkWnd->GetSafeHwnd() != NULL && !m_pFireworkWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_FIREWORK];
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - pInfo->nWidth) / 2;
		pt.y = rcParent.top + rcParent.Height() - pInfo->nHeight - INPUT_HEIGHT;
		//m_pFireworkWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
		m_pFireworkWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pFireworkWnd->IsWindowVisible())
		{
			m_pFireworkWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pSaluteWnd != NULL && m_pSaluteWnd->GetSafeHwnd() != NULL && !m_pSaluteWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_SALUTE];
		int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rcParent.Width();
		int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rcParent.Height();
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - nWidth) / 2;
		pt.y = rcParent.top + (rcParent.Height() - nHeight) / 2;
		//m_pSaluteWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
		m_pSaluteWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pSaluteWnd->IsWindowVisible())
		{
			m_pSaluteWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pPlutusWnd != NULL && m_pPlutusWnd->GetSafeHwnd() != NULL && !m_pPlutusWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_SALUTE];
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - pInfo->nWidth) / 2;
		pt.y = rcParent.top + rcParent.Height() - pInfo->nHeight - INPUT_HEIGHT;
		//m_pPlutusWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
		m_pPlutusWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pPlutusWnd->IsWindowVisible())
		{
			m_pPlutusWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pLuckyWnd != NULL && m_pLuckyWnd->GetSafeHwnd() != NULL && !m_pLuckyWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[m_enmCurLuckyEffect];
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - pInfo->nWidth) / 2;
		pt.y = rcParent.top + (rcParent.Height() - pInfo->nHeight) / 2;
		//m_pLuckyWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
		m_pLuckyWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pLuckyWnd->IsWindowVisible())
		{
			m_pLuckyWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pEnterEffectWnd != NULL && m_pEnterEffectWnd->GetSafeHwnd() != NULL && !m_pEnterEffectWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[m_enmCurEnterEffect];
		int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rcParent.Width();
		int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rcParent.Height();
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - nWidth) / 2;
		pt.y = rcParent.top + (rcParent.Height() - nHeight) / 2;
		m_pEnterEffectWnd->SetWindowPos(NULL, pt.x, pt.y, nWidth, nWidth, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pEnterEffectWnd->IsWindowVisible())
		{
			m_pEnterEffectWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pChristmasWnd != NULL && m_pChristmasWnd->GetSafeHwnd() != NULL && !m_pChristmasWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_CHRISTMAS];
		int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rcParent.Width();
		int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rcParent.Height();
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - nWidth) / 2;
		pt.y = rcParent.top + (rcParent.Height() - nHeight) / 2;
		//m_pPlutusWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
		m_pChristmasWnd->SetWindowPos(NULL, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pChristmasWnd->IsWindowVisible())
		{
			m_pChristmasWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pOrderSongWnd != NULL && m_pOrderSongWnd->GetSafeHwnd() != NULL && !m_pOrderSongWnd->IsFinish())
	{
		flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_ORDERSONG];
		int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rcParent.Width();
		int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rcParent.Height();
		POINT pt;
		pt.x = rcParent.left + (rcParent.Width() - nWidth) / 2;
		pt.y = rcParent.top + (rcParent.Height() - nHeight) / 2;
		m_pOrderSongWnd->SetWindowPos(NULL, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		if (!m_pOrderSongWnd->IsWindowVisible())
		{
			m_pOrderSongWnd->ShowWindow(SW_SHOW);
		}
	}
}

void ScreenEffectManager::OnSize(UINT nType, int cx, int cy )
{
	CRect rcParent;
	m_pRoomParentDlg->GetWindowRect(&rcParent);
	if (m_pFireworkWnd != NULL && m_pFireworkWnd->GetSafeHwnd() != NULL && !m_pFireworkWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_FIREWORK];
			POINT pt;
			pt.x = rcParent.left + (cx - pInfo->nWidth) / 2;
			pt.y = rcParent.top + cy - pInfo->nHeight - INPUT_HEIGHT;
			//m_pFireworkWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
			m_pFireworkWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			//if (!m_pFireworkWnd->IsWindowVisible())
			//{
			//	m_pFireworkWnd->ShowWindow(SW_SHOW);
			//}
		}
	}

	if (m_pSaluteWnd != NULL && m_pSaluteWnd->GetSafeHwnd() != NULL && !m_pSaluteWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_SALUTE];
			POINT pt;
			pt.x = rcParent.left + (cx - pInfo->nWidth) / 2;
			pt.y = rcParent.top + cy - pInfo->nHeight - INPUT_HEIGHT;
			//m_pSaluteWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
			//m_pSaluteWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			//if (!m_pSaluteWnd->IsWindowVisible())
			//{
			//	m_pSaluteWnd->ShowWindow(SW_SHOW);
			//}
		}
	}

	if (m_pPlutusWnd != NULL && m_pPlutusWnd->GetSafeHwnd() != NULL && !m_pPlutusWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_SALUTE];
			POINT pt;
			pt.x = rcParent.left + (cx - pInfo->nWidth) / 2;
			pt.y = rcParent.top + cy - pInfo->nHeight - INPUT_HEIGHT;
			//m_pPlutusWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
			m_pPlutusWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			//if (!m_pPlutusWnd->IsWindowVisible())
			//{
			//	m_pPlutusWnd->ShowWindow(SW_SHOW);
			//}
		} 
	}

	if (m_pLuckyWnd != NULL && m_pLuckyWnd->GetSafeHwnd() != NULL && !m_pLuckyWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[m_enmCurLuckyEffect];
			POINT pt;
			pt.x = rcParent.left + (cx - pInfo->nWidth) / 2;
			pt.y = rcParent.top + (cy - pInfo->nHeight) / 2;
			//m_pLuckyWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
			m_pLuckyWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			//if (!m_pLuckyWnd->IsWindowVisible())
			//{
			//	m_pLuckyWnd->ShowWindow(SW_SHOW);
			//}
		}
	}

	if (m_pEnterEffectWnd != NULL && m_pEnterEffectWnd->GetSafeHwnd() != NULL && !m_pEnterEffectWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[m_enmCurEnterEffect];
			int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : cx;
			int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : cy;
			POINT pt;
			pt.x = rcParent.left + (cx - nWidth) / 2;
			pt.y = rcParent.top + (cy - nHeight) / 2;
			//m_pEnterEffectWnd->SetWindowPos(NULL, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			//if (!m_pLuckyStarWnd->IsWindowVisible())
			//{
			//	m_pLuckyStarWnd->ShowWindow(SW_SHOW);
			//}
		}
	}

	if (m_pCurCombineGiftDlg != NULL && m_pCurCombineGiftDlg->GetSafeHwnd() != NULL && !m_pCurCombineGiftDlg->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			CRect rc;
			m_pCurCombineGiftDlg->GetWindowRect(&rc);
			POINT pt;
			pt.x = rcParent.left + (rcParent.Width() - rc.Width()) / 2;
			pt.y = rcParent.top + (rcParent.Height() - rc.Height()) / 2;
			m_pCurCombineGiftDlg->MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());
			if (!m_pCurCombineGiftDlg->IsWindowVisible())
			{
				m_pCurCombineGiftDlg->ShowWindow(SW_SHOW);
			}
			//m_pCurCombineGiftDlg->SetWindowPos(NULL, pt.x, pt.y, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
		}
	}

	if (m_pChristmasWnd != NULL && m_pChristmasWnd->GetSafeHwnd() != NULL && !m_pChristmasWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_CHRISTMAS];
			int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : cx;
			int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : cy;
			POINT pt;
			pt.x = rcParent.left + (cx - nWidth) / 2;
			pt.y = rcParent.top + (cy - nHeight) / 2;
			//m_pChristmasWnd->SetWindowPos(NULL, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			//if (!m_pLuckyStarWnd->IsWindowVisible())
			//{
			//	m_pLuckyStarWnd->ShowWindow(SW_SHOW);
			//}
		}
	}

	if (m_pOrderSongWnd != NULL && m_pOrderSongWnd->GetSafeHwnd() != NULL && !m_pOrderSongWnd->IsFinish())
	{
		if (nType != SIZE_MINIMIZED)
		{
			flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_ORDERSONG];
			int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : cx;
			int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : cy;
			POINT pt;
			pt.x = rcParent.left + (cx - nWidth) / 2;
			pt.y = rcParent.top + (cy - nHeight) / 2;
		}
	}

	if (nType != SIZE_MINIMIZED)
	{
		CreateFlyingTextWindowInfo();
		for (int i = 0; i < 6; i++)
		{
			if (m_FlyTextWindowInfo[i].bFlyText)
			{
				if (m_FlyTextWindowInfo[i].pFlyingTextDlg != NULL && m_FlyTextWindowInfo[i].pFlyingTextDlg->GetSafeHwnd() && !m_FlyTextWindowInfo[i].pFlyingTextDlg->IsFinish())
				{
					m_FlyTextWindowInfo[i].pFlyingTextDlg->MoveWindow(m_FlyTextWindowInfo[i].nPosX, m_FlyTextWindowInfo[i].nPosY, m_FlyTextWindowInfo[i].nWidth, m_FlyTextWindowInfo[i].nHeight);
					if (!m_FlyTextWindowInfo[i].pFlyingTextDlg->IsWindowVisible())
					{
						m_FlyTextWindowInfo[i].pFlyingTextDlg->ShowWindow(SW_SHOW);
					}
				}
			} 
			else
			{
				if (m_FlyTextWindowInfo[i].pFlyLuxuryWnd != NULL && m_FlyTextWindowInfo[i].pFlyLuxuryWnd->GetSafeHwnd() && !m_FlyTextWindowInfo[i].pFlyLuxuryWnd->IsFinish())
				{
					m_FlyTextWindowInfo[i].pFlyLuxuryWnd->MoveWindow(m_FlyTextWindowInfo[i].nPosX, m_FlyTextWindowInfo[i].nPosY, m_FlyTextWindowInfo[i].nWidth, m_FlyTextWindowInfo[i].nHeight);
					if (!m_FlyTextWindowInfo[i].pFlyLuxuryWnd->IsWindowVisible())
					{
						m_FlyTextWindowInfo[i].pFlyLuxuryWnd->ShowWindow(SW_SHOW);
					}
				}
			}
		}
	}
}

bool ScreenEffectManager::EncodeFlyingText( std::wstring & strText, int nType)
{
	strText = common::utility::stringhelper::XMLEncode(strText);

	CString strContent;
	strContent.Format(L"<?xml version=\"1.0\" encoding=\"utf-8\" ?><FLYINTTEXT><Content Type=\"%d\" >%s</Content></FLYINTTEXT>", nType, strText.c_str());

	strText = strContent.GetBuffer(0);

	return true;
}

bool ScreenEffectManager::DecodeFlyingText( std::wstring & strText, int & nType )
{
	common::utility::expat_document xmlDoc;

	std::string strUtf8 = common::utility::stringhelper::UnicodeToUTF8(strText);
	if (!xmlDoc.load_from_buffer(strUtf8.c_str(), strUtf8.size()))
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

	strText = pElement->get_element_contents();
	nType = common::utility::stringhelper::StringToInt(pElement->get_attribute_by_name(L"Type")->get_attribute_value());

	return true;
}

void ScreenEffectManager::CreateFlyingTextWindowInfo()
{
	CRect rcParent;
	m_pRoomParentDlg->GetWindowRect(&rcParent);
	int nHeight = rcParent.Height() -  FLYING_TEXT_TOP_OFFSET - INPUT_HEIGHT;

	for (int i = 0; i < 6; i++)
	{
		m_FlyTextWindowInfo[i].nPosX = rcParent.left + BORDER_WIDTH;
		m_FlyTextWindowInfo[i].nPosY = nHeight * i / 6 + rcParent.top + FLYING_TEXT_TOP_OFFSET;
		m_FlyTextWindowInfo[i].nWidth = rcParent.Width() - BORDER_WIDTH * 2;
		m_FlyTextWindowInfo[i].nHeight = nHeight / 6;
	}
}

int ScreenEffectManager::FindFreeFlyingTextWindowIndex()
{
	std::vector<int> vecTrackID;
	for (int i = 0; i < 6; i++)
	{
		if (!m_FlyTextWindowInfo[i].bInUse)
		{
			vecTrackID.push_back(i);
		}
	}

	int nSize = vecTrackID.size();
	if (nSize == 0)
	{
		return -1;
	}

	int nIndex = rand() % nSize;
	int nRet = vecTrackID[nIndex];
	vecTrackID.clear();
	m_FlyTextWindowInfo[nRet].bInUse = true;

	return nRet;
}

void ScreenEffectManager::ProcessLucky(uint32 unSrc179ID, uint16 unLuckyTimes, std::wstring strUserName2, std::wstring strRoomName, bool bInRoom )
{
	if(g_RoomModule->IsSpeedUpMode())
	{
		core::CurInfo info = GetCurInfo();
		if(unSrc179ID != info.un179id)
		{
			return;
		}
	}

	CString strTemp;
	strTemp.Format(L"%s(%d)", strUserName2.c_str(), unSrc179ID);

	std::wstring strUserName = strTemp.GetBuffer();

	ENM_Effect_Type enmType;
	switch (unLuckyTimes)
	{
	case 500:
		{
			if (bInRoom)
			{
				enmType = EFFECT_TYPE_500_IN_ROOM;
			}
			else
			{
				enmType = EFFECT_TYPE_500_OTHER_ROOM;
			}
		}
		break;
	case 1000:
		{
			if (bInRoom)
			{
				enmType = EFFECT_TYPE_1000_IN_ROOM;
			}
			else
			{
				enmType = EFFECT_TYPE_1000_OTHER_ROOM;
			}
		}
		break;
	case 5000:
		{
			if (bInRoom)
			{
				enmType = EFFECT_TYPE_5000_IN_ROOM;
			}
			else
			{
				enmType = EFFECT_TYPE_5000_OTHER_ROOM;
			}
		}
		break;
	default:
		//if (bInRoom)
		//{
		//	enmType = EFFECT_TYPE_5000_IN_ROOM;
		//}
		//else
		//{
		//	enmType = EFFECT_TYPE_5000_OTHER_ROOM;
		//}
		return;
		break;
	}

	if (m_bLuckyEffect)
	{
		//加入队列
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = enmType;
		pEffectInfo->strSrcName = strUserName;
		pEffectInfo->strDstName = strRoomName;
		m_listLucky.push_back(pEffectInfo);
		return;
	}
	else
	{
		//播放
		ShowLuckyEffect(enmType, strUserName, strRoomName);
		m_bLuckyEffect = true;
	}
}

void ScreenEffectManager::ShowLuckyEffect( ENM_Effect_Type enmType, std::wstring strUserName, std::wstring strRoomName )
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pLuckyWnd == NULL)
	{
		m_pLuckyWnd = new CFlashWnd;
	}

	m_enmCurLuckyEffect = enmType;

	flash_info * pInfo = s_mapFlashConfig[enmType];

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}
	if (!m_pLuckyWnd->Play(pInfo->nWidth, pInfo->nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pLuckyWnd->DestroyWindow();
		delete m_pLuckyWnd;
		m_pLuckyWnd = NULL;
		m_bLuckyEffect = false;
		return;
	}


	std::vector<CString> vecParams;
	if (strRoomName != L"")
	{
		vecParams.push_back(strRoomName.c_str());
	}
	vecParams.push_back(strUserName.c_str());
	if (EFFECT_TYPE_1000_IN_ROOM <= enmType && enmType <= EFFECT_TYPE_5000_OTHER_ROOM)
	{
		if (CPersonalConfig::GetInstance()->GetIsBlockLuckySound())
		{
			vecParams.push_back(L"0");
		}
		else
		{
			vecParams.push_back(L"1");
		}
	}

	m_pLuckyWnd->CallFunction(L"setParams", vecParams);
	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);
	POINT pt;
	pt.x = rc.left + (rc.Width() - pInfo->nWidth) / 2;
	pt.y = rc.top + (rc.Height() - pInfo->nHeight) / 2;
	::SetWindowPos(m_pLuckyWnd->m_hWnd, hWnd, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pLuckyWnd->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ProcessLuckyStar( std::wstring strUserName, uint32 unSrc179ID )
{
	if(g_RoomModule->IsSpeedUpMode())
	{
		core::CurInfo info = GetCurInfo();
		if(unSrc179ID != info.un179id)
		{
			return;
		}
	}

	std::wstring str179ID = common::utility::stringhelper::IntToString(unSrc179ID);
	str179ID = L"(" + str179ID + L")";

	if (m_bEnterEffect)
	{
		//加入队列
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_LUCKY_STAR;
		pEffectInfo->strSrcName = strUserName;
		pEffectInfo->strDstName = str179ID;
		m_listEnterEffect.push_back(pEffectInfo);
		return;
	}
	else
	{
		//播放
		//ShowLuckyStar(strUserName, str179ID);
		ShowEnterEffect(EFFECT_TYPE_LUCKY_STAR, strUserName, str179ID);
		m_bEnterEffect = true;
	}
}

//void ScreenEffectManager::ShowLuckyStar( std::wstring strUserName, std::wstring str179ID )
//{
//	if (!CFlashWnd::IsSupportFlash())
//	{
//		return;
//	}
//
//	if (m_pEnterEffectWnd == NULL)
//	{
//		m_pEnterEffectWnd = new CFlashWnd;
//	}
//
//	flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_LUCKY_STAR];
//
//	if (!m_pEnterEffectWnd->Play(pInfo->nWidth, pInfo->nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
//	{
//		m_pEnterEffectWnd->DestroyWindow();
//		delete m_pEnterEffectWnd;
//		m_pEnterEffectWnd = NULL;
//		m_bEnterEffect = false;
//		return;
//	}
//	
//
//	std::vector<CString> vecParams;
//	vecParams.push_back(strUserName.c_str());
//	vecParams.push_back(str179ID.c_str());
//	m_pEnterEffectWnd->CallFunction(L"setParams", vecParams);
//	CRect rc;
//	m_pRoomParentDlg->GetWindowRect(&rc);
//	POINT pt;
//	pt.x = rc.left + (rc.Width() - pInfo->nWidth) / 2;
//	pt.y = rc.top + (rc.Height() - pInfo->nHeight) / 2;
//	//m_pLuckyStarWnd->MoveWindow(pt.x, pt.y, pInfo->nWidth, pInfo->nHeight);
//	m_pEnterEffectWnd->SetWindowPos(NULL, pt.x, pt.y, pInfo->nWidth, pInfo->nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
//
//	if (m_pRoomParentDlg->IsIconic())
//	{
//		m_pEnterEffectWnd->ShowWindow(SW_HIDE);
//	}
//}

flash_info * ScreenEffectManager::GetFlashInfo( ENM_Effect_Type enmType )
{
	if (s_mapFlashConfig.find((int)enmType) != s_mapFlashConfig.end())
	{
		return s_mapFlashConfig[(int)enmType];
	}

	return NULL;
}

void ScreenEffectManager::StopAllFlash()
{
	m_bLuckyEffect = false;
	m_bEnterEffect = false;
	m_bFirework = false;
	m_bPlutus = false;
	m_bSalute = false;
}

void ScreenEffectManager::ProcessFlyLuxury( CString strSrcName , CString strDstName , uint32 unSrc179ID , uint32 unDst179ID , uint16 unGiftID ,uint16 unCount )
{
	if(g_RoomModule->IsSpeedUpMode())
	{
		core::CurInfo info = GetCurInfo();
		if(unSrc179ID != info.un179id && unDst179ID != info.un179id)
		{
			return;
		}
	}

	m_pRoomParentDlg->FlashRoomWindow() ;
	//ASSERT(nType != 0);
	if (m_nFlyLuxuryCount < MAX_FLY_LUXURY_NUM)
	{
		int nIndex = FindFreeFlyingTextWindowIndex();
		if (nIndex == -1)
		{
			//加入队列
			Luxury_Info * pLuxuryInfo = new Luxury_Info;
			pLuxuryInfo->strSrcName = strSrcName.GetBuffer(0);
			pLuxuryInfo->strDstName = strDstName.GetBuffer(0);
			pLuxuryInfo->unSrc179ID = unSrc179ID;
			pLuxuryInfo->unDst179ID = unDst179ID;
			pLuxuryInfo->unGiftID = unGiftID;
			pLuxuryInfo->unCount = unCount;
			//pLuxuryInfo->strTime = strTime.GetBuffer(0);
			m_listFlyLuxury.push_back(pLuxuryInfo);

			return;
		}
		else
		{
			ShowFlyLuxury(strSrcName, strDstName, unSrc179ID, unDst179ID, unGiftID, unCount, nIndex);
		}
	}
	else
	{
		//加入队列
		Luxury_Info * pLuxuryInfo = new Luxury_Info;
		pLuxuryInfo->strSrcName = strSrcName.GetBuffer(0);
		pLuxuryInfo->strDstName = strDstName.GetBuffer(0);
		pLuxuryInfo->unSrc179ID = unSrc179ID;
		pLuxuryInfo->unDst179ID = unDst179ID;
		pLuxuryInfo->unGiftID = unGiftID;
		pLuxuryInfo->unCount = unCount;
		//pLuxuryInfo->strTime = strTime.GetBuffer(0);
		m_listFlyLuxury.push_back(pLuxuryInfo);

		return;
	}
}

void ScreenEffectManager::ShowFlyLuxury( CString strSrcName , CString strDstName , uint32 unSrc179ID , uint32 unDst179ID , uint16 unGiftID ,uint16 unCount , int nIndex )
{
	SwitchResourceToModule(_T("Room"));
	if (m_FlyTextWindowInfo[nIndex].pFlyLuxuryWnd == NULL)
	{
		m_FlyTextWindowInfo[nIndex].pFlyLuxuryWnd = new CFlyLuxuryWnd(m_pRoomParentDlg);
		m_FlyTextWindowInfo[nIndex].pFlyLuxuryWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, AfxRegisterWndClass(0), _T("FlyLuxury"), WS_POPUP, CRect(0,0,0,0), m_pRoomParentDlg, 0);
		m_FlyTextWindowInfo[nIndex].bFlyText = false;
		m_nFlyLuxuryCount++;
	}

	CRect rcParent;
	m_pRoomParentDlg->GetWindowRect(&rcParent);

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}

	::SetWindowPos(m_FlyTextWindowInfo[nIndex].pFlyLuxuryWnd->m_hWnd, hWnd, m_FlyTextWindowInfo[nIndex].nPosX, m_FlyTextWindowInfo[nIndex].nPosY, m_FlyTextWindowInfo[nIndex].nWidth, m_FlyTextWindowInfo[nIndex].nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);
	m_FlyTextWindowInfo[nIndex].pFlyLuxuryWnd->InsertPaoDaoText(strSrcName, strDstName, unSrc179ID, unDst179ID, unGiftID, unCount);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_FlyTextWindowInfo[nIndex].pFlyLuxuryWnd->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ProcessVipTop( std::wstring strUserName, uint32 unSrc179ID )
{
	if(g_RoomModule->IsSpeedUpMode())
	{
		core::CurInfo info = GetCurInfo();
		if(unSrc179ID != info.un179id)
		{
			return;
		}
	}

	std::wstring str179ID = common::utility::stringhelper::IntToString(unSrc179ID);
	str179ID = L"(" + str179ID + L")";

	if (m_bEnterEffect)
	{
		//加入队列
		Effect_Info * pEffectInfo = new Effect_Info;
		pEffectInfo->enmType = EFFECT_TYPE_VIP_TOP;
		pEffectInfo->strSrcName = strUserName;
		pEffectInfo->strDstName = str179ID;
		m_listEnterEffect.push_back(pEffectInfo);
		return;
	}
	else
	{
		//播放
		//ShowLuckyStar(strUserName, str179ID);
		ShowEnterEffect(EFFECT_TYPE_VIP_TOP, strUserName, str179ID);
		m_bEnterEffect = true;
	}
}

void ScreenEffectManager::ShowEnterEffect( ENM_Effect_Type enmType, std::wstring strUserName, std::wstring str179ID )
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pEnterEffectWnd == NULL)
	{
		m_pEnterEffectWnd = new CFlashWnd;
	}

	m_enmCurEnterEffect = enmType;

	flash_info * pInfo = s_mapFlashConfig[enmType];

	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);

	int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rc.Width();
	int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rc.Height();

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}
	if (!m_pEnterEffectWnd->Play(nWidth, nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pEnterEffectWnd->DestroyWindow();
		delete m_pEnterEffectWnd;
		m_pEnterEffectWnd = NULL;
		m_bEnterEffect = false;
		return;
	}

	std::vector<CString> vecParams;
	vecParams.push_back(strUserName.c_str());
	vecParams.push_back(str179ID.c_str());
	m_pEnterEffectWnd->CallFunction(L"setParams", vecParams);

	POINT pt;
	pt.x = rc.left + (rc.Width() - nWidth) / 2;
	pt.y = rc.top + (rc.Height() - nHeight) / 2;
	::SetWindowPos(m_pEnterEffectWnd->m_hWnd, hWnd, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pEnterEffectWnd->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ShowChristmas(int nType, IGiftItem * pGiftItem, std::wstring strSrcName, uint32 unSrc179ID, std::wstring strDstName, uint32 unDst179ID)
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pChristmasWnd == NULL)
	{
		m_pChristmasWnd = new CFlashWnd;
	}

	m_bChristmas = true;

	flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_CHRISTMAS];

	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);

	int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rc.Width();
	int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rc.Height();

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}
	if (!m_pChristmasWnd->Play(nWidth, nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pChristmasWnd->DestroyWindow();
		delete m_pChristmasWnd;
		m_pChristmasWnd = NULL;
		return;
	}

	std::vector<CString> vecParams;
	CString strName;
	strName.Format(L"%s,%u,%s,%u", strSrcName.c_str(), unSrc179ID, strDstName.c_str(), unDst179ID);
	vecParams.push_back(strName);
	m_pChristmasWnd->CallFunction(L"setParams", vecParams);
	POINT pt;
	pt.x = rc.left + (rc.Width() - nWidth) / 2;
	pt.y = rc.top + (rc.Height() - nHeight) / 2;
	::SetWindowPos(m_pChristmasWnd->m_hWnd, hWnd, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pChristmasWnd->ShowWindow(SW_HIDE);
	}
}

void ScreenEffectManager::ProcessOrderSong( std::wstring strSrcName, std::wstring strDstName, uint32 unSrc179ID, uint32 unDst179ID,std::wstring strSongName, std::wstring strWish )
{
	if (m_bOrderSong || m_bEnterEffect)
	{
		OrderSong_Info * pEffectInfo = new OrderSong_Info;
		pEffectInfo->enmType = EFFECT_TYPE_ORDERSONG;
		pEffectInfo->strSrcName = strSrcName;
		pEffectInfo->unSrc179ID = unSrc179ID;
		pEffectInfo->strDstName = strDstName;
		pEffectInfo->unDst179ID = unDst179ID;
		pEffectInfo->strFlyingText = strSongName;
		pEffectInfo->strSongWish = strWish;
		m_listOrderSong.push_back(pEffectInfo);
	}
	else
	{
		ShowOrderSong(strSrcName, strDstName, unSrc179ID, unDst179ID, strSongName,  strWish);
	}
}

void ScreenEffectManager::ShowOrderSong( std::wstring strSrcName, std::wstring strDstName, uint32 unSrc179ID, uint32 unDst179ID, std::wstring strSongName, std::wstring strWish )
{
	if (!CFlashWnd::IsSupportFlash())
	{
		return;
	}

	if (m_pOrderSongWnd == NULL)
	{
		m_pOrderSongWnd = new CFlashWnd;
	}

	m_bOrderSong = true;

	flash_info * pInfo = s_mapFlashConfig[EFFECT_TYPE_ORDERSONG];

	CRect rc;
	m_pRoomParentDlg->GetWindowRect(&rc);

	int nWidth = pInfo->nWidth != 0 ? pInfo->nWidth : rc.Width();
	int nHeight = pInfo->nHeight != 0 ? pInfo->nHeight : rc.Height();

	HWND hWnd = GetForegroundWindow();
	if (hWnd == m_pRoomParentDlg->m_hWnd)
	{
		hWnd = NULL;
	}
	if (!m_pOrderSongWnd->Play(nWidth, nHeight, pInfo->nPlayTime, pInfo->nInterval, pInfo->strFlashPath.c_str(), m_pRoomParentDlg))
	{
		m_pOrderSongWnd->DestroyWindow();
		delete m_pOrderSongWnd;
		m_pOrderSongWnd = NULL;
		return;
	}

	std::vector<CString> vecParams;
	vecParams.push_back(strDstName.c_str());
	vecParams.push_back(strSrcName.c_str());
	vecParams.push_back(strSongName.c_str());
	vecParams.push_back(strWish.c_str());
	//CString strName;
	//strName.Format(L"%s,%s,%s,%s", strDstName.c_str(), strSrcName.c_str(), strSongName.c_str(), strWish.c_str());
	//vecParams.push_back(strName);
	m_pOrderSongWnd->CallFunction(L"callFlash", vecParams);
	POINT pt;
	pt.x = rc.left + (rc.Width() - nWidth) / 2;
	pt.y = rc.top + (rc.Height() - nHeight) / 2;
	::SetWindowPos(m_pOrderSongWnd->m_hWnd, hWnd, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);

	if (m_pRoomParentDlg->IsIconic())
	{
		m_pOrderSongWnd->ShowWindow(SW_HIDE);
	}
}
