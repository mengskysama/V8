// NetLayer.cpp : 定义 DLL 应用程序的入口点。
//

#include "..\..\include\Net\NetLayer.h"

unsigned int bindError;
netlayer::ConnectionLayer* m_ConnectionLayer = NULL;

namespace netlayer
{
	netlayer::ConnectionLayer* GetConnectionLayer()
	{
		if(m_ConnectionLayer == NULL)
		{
			m_ConnectionLayer = new netlayer::ConnectionLayer();
		}

		return m_ConnectionLayer;
	}

	void DestroyConnectionLayer()
	{
		if(m_ConnectionLayer != NULL)
		{
			delete m_ConnectionLayer;
			m_ConnectionLayer = NULL;
		}
	}
}