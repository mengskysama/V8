#ifndef __SOCKETHANDLER_H__
#define	__SOCKETHANDLER_H__
#ifdef _DEBUG
//#include <vld.h>
#endif
#include "CommonPacket.h"
#include <vector>

class COMMON_OUT_CLASS SocketHandler
{
public:
	SocketHandler():m_iSocketID(-1){}

	virtual ~SocketHandler() {}

	inline	void			setSocketID(SInt32 iID) { m_iSocketID = iID; }

	//////////////////////////////////////////////////////////////////////////
	//连接关闭时执行的函数;
	virtual void			onClosed() {}	

	//////////////////////////////////////////////////////////////////////////
	//接受到数据包之后执行的函数;
	//Packet*的内存由上层用户释放，底层不作释放
	virtual void			onPacketRcved(Packet*) {}

	//一个数据包发送完毕之后执行的函数;
	virtual void			onPacketSent(Packet*) {}

	//////////////////////////////////////////////////////////////////////////
	//数据包发送完毕之后执行的函数;
	virtual void			onDataSent(UInt32 iLength) {}

	//////////////////////////////////////////////////////////////////////////
	//发送失败时的消息通知;
	virtual void			onSendingFailed(Packet* pPacket) 
	{
		pPacket->RemoveReference();
	}

protected:	

	SInt32					m_iSocketID;
};

#endif //__SOCKETHANDLER_H__
