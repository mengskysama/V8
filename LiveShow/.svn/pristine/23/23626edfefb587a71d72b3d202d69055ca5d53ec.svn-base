/**
* utility.h
* 工具包
*
* @version 1.0 (06 ,10, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "51ktv.h"
#include "../Common.h"
#include <boost/function.hpp>

namespace common
{
	namespace utility
	{
		class COMMON_INTERFACE _Timers
		{
			friend class ModuleManagerImpl;

		public:
			static _Timers& instance();

			enum ENM_Timer_Name
			{
				ENM_TIMER_TOKEN,
				ENM_TIMER_CYCLE_TOKEN,
				ENM_TIMER_HALL_LOGIN,
				ENM_TIMER_HALL_HEARTBEAT,			
				ENM_TIMER_HALL_SYSTEMMESSAGE,
				ENM_TIMER_HALL_ENTERROOM,				
				ENM_TIMER_ROOM_NETSTATE,
				ENM_TIMER_HALL_MAXROOM,
				ENM_TIMER_ROOM_CPU_PERFORMANCE,
				ENM_TIMER_LOGIN_RETRY,
			};

		public:
			typedef boost::function<uintptr(uintptr)> IntervalFunc;

			struct IntervalFunctor
			{
				void Compute()
				{
					if(intervalFunc != NULL)
						interval = intervalFunc(interval);
				}

				IntervalFunctor(uint32 interval=-1, IntervalFunc intervalFunc = NULL)
					:interval(interval),intervalFunc(intervalFunc){}

				IntervalFunctor(IntervalFunctor & intervalFunctor)
					:interval(intervalFunctor.interval),
					intervalFunc(intervalFunctor.intervalFunc){}
				
				uint32 interval;
				IntervalFunc intervalFunc;
			};
			
			virtual ~_Timers();
			void Dump();
			typedef boost::function<void(uintptr,bool)> TimerCallback;//true表示最后一次timer到达
			uint32 CreateTimer(uint32 elpase,TimerCallback & callback,uint32 name,int32 repeatTimes=-1);
			uint32 CreateTimer(IntervalFunctor &intervalFunctor,TimerCallback & callback,uint32 name,int32 repeatTimes=-1);
			static uint32 GetNowTime();
			void DestroyTimer(uint32);

		protected:
			void OnTimer();//由ModuleManagerImpl周期调用

		private:

			_Timers();
			uint32 AllocNode();
			uint32 FreeNode(uint32 node);
			bool Empty(){return usedHead == -1;}
			bool Full(){return freeHead == -1;}

		private:
			
			struct TimerNode
			{
				TimerNode();
				TimerNode(uint32 interval,TimerCallback & callback,int32 repeatTimes,uint32 name);
				TimerNode(IntervalFunctor &intervalFunctor,TimerCallback & callback,int32 repeatTimes,uint32 name);
				TimerCallback callback;
				int32 repeatTimes;
				IntervalFunctor intervalFunctor;
				uint32 basePoint;
				uint32 timeName;
			};

			struct TimerUnit
			{
				TimerNode timerNode;
				uint32 next;
				TimerUnit(){}
				TimerUnit(TimerUnit const& timerUnit)
				{
					timerNode=timerUnit.timerNode;
					next=timerUnit.next;
				}
			};

			std::vector<TimerUnit> timerUnitVec;
			uint32 freeHead;
			uint32 usedHead;
			bool destroyHappend;//用于标明一个销毁操作发生了。
		};
	}
}




