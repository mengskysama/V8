#include "StdAfx.h"
#include "SofaManager.h"
#include "..\RoomParentDlg.h"
#include "HallDefine.h"
#include "..\richlevel\RichLevelManager.h"
#include "..\userlist/ListIconManager.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"

CSofaManager::CSofaManager(uint32 unRoomID, ISofaManagerSink * pSofaManagerSink)
: m_unRoomID(unRoomID),
  m_pSofaManagerSink(pSofaManagerSink)
{
}

CSofaManager::~CSofaManager(void)
{
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem != NULL)
		{
			delete pSofaItem;
			pSofaItem = NULL;
		}
	}
	m_mapSofaList.clear();
}

void CSofaManager::InitList( core::GetSofaListRspEvent::SofaInfo sofainfo[] )
{
	for (int i = 0; i < 5; i++)
	{
		if (sofainfo[i].uin != 0)
		{
			CSofaItem * pSofaItem = new CSofaItem(i + 1);
			pSofaItem->SetSofaInfo(i + 1, sofainfo[i]);

			m_mapSofaList[i+1] = pSofaItem;
		}
	}
}

void CSofaManager::UpdateSofa( uint8 unIndex, uint32 unPrice, IUserItem * pUserItem )
{
	CSofaItem * pSofaItem = NULL;
	if (m_mapSofaList.find(unIndex) != m_mapSofaList.end())
	{
		pSofaItem = m_mapSofaList[unIndex];
		pSofaItem->UpdateSofaInfo(unPrice, pUserItem);
	}
	else
	{
		pSofaItem = new CSofaItem(unIndex);
		pSofaItem->UpdateSofaInfo(unPrice, pUserItem);

		m_mapSofaList[unIndex] = pSofaItem;
	}

	m_pSofaManagerSink->OnBuySofaNotify(pSofaItem);
}

bool CSofaManager::IsOnSofa( uint32 unUIN )
{
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem->GetUserUIN() == unUIN)
		{
			return true;
		}
	}

	return false;
}

void CSofaManager::UpdateSofaHead( uint32 unUIN )
{
	if (!IsOnSofa(unUIN))
	{
		return;
	}

	std::vector<CSofaItem*> vecSofaItem;
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem->GetUserUIN() == unUIN)
		{
			pSofaItem->UpdateHead();
			vecSofaItem.push_back(pSofaItem);
		}
	}

	m_pSofaManagerSink->OnSofaInfoUpdate(vecSofaItem);
}

CString CSofaManager::GetInitString()
{
	CString strInit = L"";
	for (uint8 nIndex = 1; nIndex <= 5; nIndex++)
	{
		if (m_mapSofaList.find(nIndex) != m_mapSofaList.end())
		{
			CSofaItem * pSofaItem = m_mapSofaList[nIndex];
			strInit += pSofaItem->GetStringParam() + L"|";
		}
		else
		{
			CString strIndex = common::utility::stringhelper::IntToString(nIndex).c_str();
			strInit += strIndex + L"|";
		}
	}
	return strInit;
}

CString CSofaManager::GetOneSofaString(uint8 unIndex, bool bJustUpdate/* = false*/)
{
	if (m_mapSofaList.find(unIndex) != m_mapSofaList.end())
	{
		return m_mapSofaList[unIndex]->GetStringParam(bJustUpdate);
	}
	return L"";
}

void CSofaManager::FindSofaIndexByUIN( uint32 unUIN, std::vector<uint8> & vecIndex )
{
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem->GetUserUIN() == unUIN)
		{
			vecIndex.push_back(pSofaItem->GetIndex());
		}
	}
}

void CSofaManager::UpdateSofaVipLevel( uint32 unUIN, uint8 unVipLevel )
{
	if (!IsOnSofa(unUIN))
	{
		return;
	}

	std::vector<CSofaItem*> vecSofaItem;
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem->GetUserUIN() == unUIN)
		{
			pSofaItem->UpdateVipLevel(unVipLevel);
			vecSofaItem.push_back(pSofaItem);
		}
	}

	m_pSofaManagerSink->OnSofaInfoUpdate(vecSofaItem);
}

void CSofaManager::UpdateSofaRichLevel( uint32 unUIN, uint8 unRichLevel )
{
	if (!IsOnSofa(unUIN))
	{
		return;
	}

	std::vector<CSofaItem*> vecSofaItem;
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem->GetUserUIN() == unUIN)
		{
			pSofaItem->UpdateRichLevel(unRichLevel);
			vecSofaItem.push_back(pSofaItem);
		}
	}

	m_pSofaManagerSink->OnSofaInfoUpdate(vecSofaItem);
}

void CSofaManager::ClearSofaList()
{
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem != NULL)
		{
			delete pSofaItem;
			pSofaItem = NULL;
		}
	}
	m_mapSofaList.clear();
}

CSofaItem * CSofaManager::GetSofaItemByIndex( uint8 unIndex )
{
	if (m_mapSofaList.find(unIndex) != m_mapSofaList.end())
	{
		return m_mapSofaList[unIndex];
	}

	return NULL;
}

void CSofaManager::UpdateSofaUserInfo( IUserItem * pUserItem )
{
	if (!IsOnSofa(pUserItem->GetUserUIN()))
	{
		return;
	}

	std::vector<CSofaItem*> vecSofaItem;
	for (std::map<uint8, CSofaItem*>::iterator it = m_mapSofaList.begin(); it != m_mapSofaList.end(); ++it)
	{
		CSofaItem * pSofaItem = it->second;
		if (pSofaItem->GetUserUIN() == pUserItem->GetUserUIN())
		{
			pSofaItem->UpdateSofaInfo(pUserItem);
			vecSofaItem.push_back(pSofaItem);
		}
	}

	m_pSofaManagerSink->OnSofaInfoUpdate(vecSofaItem);
}