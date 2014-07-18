#include "stdafx.h"
#include "h264Cache.h"

#define SAFE_DELETE(ptr)  if ((ptr)) { delete   (ptr); (ptr) = NULL; }
#define SAFE_DELETEA(ptr) if ((ptr)) { delete[] (ptr); (ptr) = NULL; }


/////////////////////////////////////////////////////////////////////////CVideoDataCache

CVideoDataCache::CVideoDataCache()
{
	m_pdata = NULL;
	m_ilen	 = 0;
}

CVideoDataCache::~CVideoDataCache()
{
	m_ilen = 0;
	SAFE_DELETEA(m_pdata);
}

void CVideoDataCache::SetData(char* pData, int Len)
{
	if (!m_pdata)
	{
		m_pdata = new char[Len];
	}
	m_ilen = Len;
	memcpy(m_pdata,pData,Len);
}

void CVideoDataCache::GetData(char* pData, int& Len)
{
	if (m_pdata)
	{
		memcpy(pData,m_pdata,m_ilen);
		Len = m_ilen;
		return;
	}

	Len = 0;
}

char* CVideoDataCache::GetDataPtr()
{
	return m_pdata;
}


int  CVideoDataCache::GetLength()
{
	return m_ilen;
}

void CVideoDataCache::SetType(int t)
{
	m_itype = t;
}

int  CVideoDataCache::GetType()
{
	return m_itype;
}

/////////////////////////////////////////////////////////////////////////
CVideoDataCacheList::CVideoDataCacheList()
{
	m_list.clear();
}

CVideoDataCacheList::~CVideoDataCacheList()
{
	ClearList();
}

void CVideoDataCacheList::SetListData(CVideoDataCache* p)
{
	m_list.push_back(p);
}

CVideoDataCache* CVideoDataCacheList::GetListData()
{
	if (GetListSize()==0)
	{
		return NULL;
	}
	CVideoDataCache* p = m_list.front();
	m_list.pop_front();
	return p;
}

int  CVideoDataCacheList::GetListSize()
{
	return m_list.size();
}

void CVideoDataCacheList::ClearList()
{
	while (m_list.size()>0)
	{
		delete m_list.front();
		m_list.pop_front();
	}
	m_list.clear();
}