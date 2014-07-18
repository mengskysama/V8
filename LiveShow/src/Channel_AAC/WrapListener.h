#pragma once
#ifdef _DEBUG
//#include <vld.h>
#endif
#include "ILogicListener.h"
#include "RC.h"

#define BAD_MEM_READ(a,b,c)  if (IsBadReadPtr((a),(b))) \
	{  Assert(0); ErrTrace("%s: 内存不可读 time = %I64d \n",(c),GetTickCount());  return;  }


/************************************************************************/
/* 实现对语音视频Channel类再封装，接收数据接口                          */
/************************************************************************/
class WrapListener : public RCObject
{
public:

	WrapListener(ILogicListener* pLogicListener) : m_pLogicListener(pLogicListener) {}
	~WrapListener()  { m_pLogicListener=NULL; }

	void  onConnected() {
		BAD_MEM_READ(m_pLogicListener,sizeof(m_pLogicListener),"onConnected()");
		if (m_pLogicListener) m_pLogicListener->onConnected(); 
	}
	void  onReceive(const UInt8* pBuf, UInt32 iLen,UInt32 iSeq)   
	{ 
		BAD_MEM_READ(m_pLogicListener,sizeof(m_pLogicListener),"onReceive()");
		if (m_pLogicListener) m_pLogicListener->onReceive(pBuf, iLen,iSeq);  
	}
	void  onDisconnect(UInt8 cReason)             
	{ 
		BAD_MEM_READ(m_pLogicListener,sizeof(m_pLogicListener),"onDisconnect()");
		if (m_pLogicListener) m_pLogicListener->onDisconnect(cReason);
	}
	void  onStatus(UInt16 sStatus)               
	{ 
		BAD_MEM_READ(m_pLogicListener,sizeof(m_pLogicListener),"onStatus()");
		if (m_pLogicListener) m_pLogicListener->onStatus(sStatus);
	}

private:

	ILogicListener*              m_pLogicListener;
};