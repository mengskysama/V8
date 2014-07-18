#pragma once

#include <map>
#include "IRender.h"


class CRenderFactory
{
public:
	typedef ICommonRender *(*RenderCreateCallback)(HWND hWnd, long lWidth, long lHeight);

public:
	bool RegisterRender(IRender::En_Used_Order uo,RenderCreateCallback Callback)
	{
		return m_Callback.insert ( RCallbackMap::value_type(uo,Callback) ).second ;
	}
	
	ICommonRender* CreateRender(IRender::En_Used_Order uo,HWND hWnd, long lWidth, long lHeight)
	{
		RCallbackMap::const_iterator it = m_Callback.find ( uo ) ;
		if ( it == m_Callback.end () )
		{
			return NULL ;
		}
		return it->second ( hWnd, lWidth, lHeight ) ;
	}

private:
	typedef std::map<IRender::En_Used_Order, RenderCreateCallback> RCallbackMap ;
	RCallbackMap m_Callback ;
};

extern CRenderFactory g_RenderFactory;