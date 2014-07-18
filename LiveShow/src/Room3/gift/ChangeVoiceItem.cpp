#include "StdAfx.h"
#include "ChangeVoiceItem.h"

CChangeVoiceItem::CChangeVoiceItem(void)
{
}

CChangeVoiceItem::~CChangeVoiceItem(void)
{
	m_vecStrVoice.clear();
}

void CChangeVoiceItem::AddVoiceWords( std::wstring strVoice )
{
	m_vecStrVoice.push_back(strVoice);
}

std::wstring CChangeVoiceItem::GetVoiceWords()
{
	if (m_vecStrVoice.size() != 0)
	{
		int nSize = m_vecStrVoice.size();
		int nIndex = rand() % nSize;
		return m_vecStrVoice[nIndex];
	}

	return L"";
}