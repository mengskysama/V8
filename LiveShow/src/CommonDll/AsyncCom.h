#ifndef  __ASYNCCOM_H__
#define	 __ASYNCCOM_H__

#include "Common.h"
#include "Mutex.h"
#include "SocketContext.h"
#include "Event.h"
#include <vector>

LRESULT CALLBACK select_wndproc(HWND inWindow, UINT inMsg, WPARAM inParam, LPARAM inOtherParam);

class AsyncCom
{
public:
	AsyncCom(){}

	~AsyncCom(){}

    //////////////////////////////////////////////////////////////////////////
    //功能描述：初始化异步通信接口；
	//返回值：  无；
	static void		initialize(); 

	//////////////////////////////////////////////////////////////////////////
	//功能描述：释放异步通信接口的资源；
	//输入参数：无；
	//返回值：  无；
	static void		destroy();

	//////////////////////////////////////////////////////////////////////////
	//功能描述：在一个Fd上请求事件；
	//输入参数：@参数1：iFd, Fd的文件描述符;
	//          @参数2：iUniqueID, 该Fd的UniqueID;
	//          @参数3：iEventMask, 请求的事件掩码;
	//返回：成功或者失败;
	static Bool		requestEvent(SInt32 iFd, SInt32 iUniqueID, UInt32 iEventMask);
    //////////////////////////////////////////////////////////////////////////
    //功能描述：清除一个Fd上的所有事件请求;
	//输入参数：@参数1：iFd; 被清除的Fd；
	//返回值：  无；
	static void		clearEventRequest(SInt32 iFd);
    //////////////////////////////////////////////////////////////////////////
    //功能描述：等待事件的触发；该函数诸塞；
	//输入参数：@参数1：已经触发了的事件列表；
	//放回值：  0，成功， 否则，错误码；
	static	SInt32	waitEvents(std::vector<LPEvent>& vCurComEvents);

	static  HWND    getMsgWindow() { return s_hMsgWindow;}

private:

	static SInt32						s_iWndAtom;		//避免 WndClass Name 出现重复 
	//消息窗口的句柄；
	static HWND							s_hMsgWindow;	

};

#endif //__ASYNCCOM_H__
