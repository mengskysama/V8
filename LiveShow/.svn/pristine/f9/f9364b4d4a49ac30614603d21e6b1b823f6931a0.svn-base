#pragma once

#include "base/time.h"
#include <boost/circular_buffer.hpp>

class DumpNode
{
public:
	WORD  m_wMinute;
	WORD  m_wSecond;
	WORD  m_wMilliseconds;
	int32 m_nValue;
	DumpNode(int32 v):m_nValue(v)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		m_wMilliseconds = st.wMilliseconds;
		m_wSecond = st.wSecond;
		m_wMinute = st.wMinute;
	}
};

typedef std::list<DumpNode>  DumpNodeBuffer;