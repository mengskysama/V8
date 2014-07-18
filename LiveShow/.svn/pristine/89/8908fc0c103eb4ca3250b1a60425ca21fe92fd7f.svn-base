#include "StdAfx.h"
#include "Timer.h"

map<UINT_PTR, UINT> CTimer::m_mapTimerId2EventId;
vector<ITimerEvent*> CTimer::m_vecTimerEvent;

CTimer::CTimer(void)
{
	m_bSupportMultiTimer = true;
}

CTimer::~CTimer(void)
{
}

void CTimer::SetSupportMultiTimerFlage(bool bSupportMultiTimer)
{
	m_bSupportMultiTimer = bSupportMultiTimer;
}

UINT_PTR CTimer::SetTimer(UINT uTimerId, UINT uTimeElapse)
{
	if (m_bSupportMultiTimer)
	{
		if (IsTimerExist(uTimerId))
		{
			return -1;
		}

		UINT_PTR uRetEventId = ::SetTimer(NULL, uTimerId, uTimeElapse, CTimer::TimerProc);
		m_mapTimerId2EventId[uRetEventId] = uTimerId;
		return uRetEventId;
	}
	else
	{
		if (IsTimerExist(uTimerId))	// "单timer重复利用模式"下，先停掉原来的timer，然后起一个相同timerid的timer
		{
			KillTimer(uTimerId);
		}

		UINT_PTR uRetEventId = ::SetTimer(NULL, uTimerId, uTimeElapse, CTimer::TimerProc);
		m_mapTimerId2EventId[uRetEventId] = uTimerId;
		return uRetEventId;
	}
}

VOID CTimer::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	UINT uTimerId = m_mapTimerId2EventId[idEvent];

	for (int i = 0; i < m_vecTimerEvent.size(); i++)
	{
		ITimerEvent* p = m_vecTimerEvent[i];
		p->OnTimer(uTimerId);
	}
}

void CTimer::AddTimerHandler(ITimerEvent* pHandler)
{
	if (FindHandler(pHandler) > -1)
	{
		return;
	}

	m_vecTimerEvent.push_back(pHandler);
}


bool CTimer::IsTimerHandlerExist(ITimerEvent* pHandler)
{
	return FindHandler(pHandler) >= 0;
}

void CTimer::RemoveTimerHandler(ITimerEvent* pHandler)
{
	int idx = FindHandler(pHandler);

	if (idx < 0)
	{
		return;
	}

	m_vecTimerEvent.erase(m_vecTimerEvent.begin() + idx);

	if (!m_bSupportMultiTimer)
	{
		if (m_vecTimerEvent.size() == 0 && m_mapTimerId2EventId.size() == 1)
		{
			map<UINT_PTR, UINT>::iterator it = m_mapTimerId2EventId.begin();
			UINT uTimerId = it->second;
			KillTimer(uTimerId);
		}
	}
}

int CTimer::FindHandler(ITimerEvent* pHandler)
{
	vector<ITimerEvent*>::iterator itFound = find(m_vecTimerEvent.begin(), m_vecTimerEvent.end(), pHandler);
	
	if (itFound == m_vecTimerEvent.end())
	{
		return -1;
	}
	else
	{
		return itFound - m_vecTimerEvent.begin();
	}
}

bool CTimer::IsTimerExist(UINT uTimerId, UINT_PTR* pEventId/* = NULL*/)
{
	for (map<UINT_PTR, UINT>::iterator it = m_mapTimerId2EventId.begin(); it != m_mapTimerId2EventId.end(); it++)
	{
		if (it->second == uTimerId)
		{
			if (pEventId)
			{
				*pEventId = it->first;
			}

			return true;
		}
	}

	return false;
}

void CTimer::KillTimer(UINT uTimerId)
{
	UINT_PTR uEventId = -1;
	if (!IsTimerExist(uTimerId, &uEventId))
	{
		return;
	}

	::KillTimer(NULL, uEventId);

	m_mapTimerId2EventId.erase(uEventId);
}