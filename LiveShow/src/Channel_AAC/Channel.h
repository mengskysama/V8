// Channel.h : Channel DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "51ktv.h"
#include "StatisticDataDef.h"
#include "resource.h"		// 主符号

#ifdef CHANNEL_EXPORTS
#define CHANNEL_OUT_INTERFACE  _declspec(dllexport)
#else
#define CHANNEL_OUT_INTERFACE  _declspec(dllimport)
#endif

namespace ChannelSpace
{
	extern "C" 
	{
		CHANNEL_OUT_INTERFACE IModule * __stdcall GetModule();
	}
}

class CChannelApp : public CWinApp
{
public:
	CChannelApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
