#pragma  once

#include <deque>

#include "Mutex.h"
#include "Event.h"

class EventPool
{
public:

	//获取一个Event
	static  LPEvent        getLPEvent();

	//回收一个Event
	static  void           putLPEvent(LPEvent pEvent);

	//销毁EventPool资源
	static  void           destroy();

private:

	static Mutex                       m_Mutex;
	static std::deque<LPEvent>         m_LPEventPool;
};