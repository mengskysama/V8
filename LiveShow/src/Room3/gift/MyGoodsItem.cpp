#include "StdAfx.h"
#include "MyGoodsItem.h"

CMyGoodsItem::CMyGoodsItem(void)
	: m_nCount(0), m_pGiftItem(NULL)
{
}

CMyGoodsItem::~CMyGoodsItem(void)
{
}

int CMyGoodsItem::GetCount()
{
	return m_nCount;
}

void CMyGoodsItem::SetCount( int nCount )
{
	m_nCount = nCount;
}

IGiftItem * CMyGoodsItem::GetGiftItem()
{
	return m_pGiftItem;
}

void CMyGoodsItem::SetGiftItem( IGiftItem * pGiftItem )
{
	m_pGiftItem = pGiftItem;
}