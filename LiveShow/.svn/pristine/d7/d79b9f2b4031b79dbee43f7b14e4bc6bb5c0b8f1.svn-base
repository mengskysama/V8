/**
* utility.cpp
* 工具包
*
* @version 1.0 (06 ,10, 2008)
* @author reinhardt ken
*
*/
#include "stdafx.h"
#include "utility\timers.h"
#include <boost/format.hpp>

namespace common
{
	namespace utility
	{
		_Timers& _Timers::instance()
		{
			static _Timers s_timer;
			return s_timer;
		}

		_Timers::_Timers()
			:timerUnitVec(50),
			freeHead(0),
			usedHead(-1),
			destroyHappend(false)
		{
			int i=0;
			for(;i<timerUnitVec.size()-1;i++)
				timerUnitVec[i].next=i+1;
			timerUnitVec[i].next=-1;
		}

		_Timers::~_Timers()
		{
		}

		void _Timers::Dump()
		{
// 			_LOG(C(L"TimerDump")<<L"Used List"<<OVER);
// 			for(uint32 i=usedHead;i!=-1;i=timerUnitVec[i].next)
// 			{
// 				_LOG(C(L"TimerDump")<<boost::wformat(L"(%d,%d)")%i%timerUnitVec[i].timerNode.timeName<<OVER);	
// 			}
// 			_LOG(C(L"TimerDump")<<L"Free List"<<OVER);
// 			for(uint32 i=freeHead;i!=-1;i=timerUnitVec[i].next)
// 			{
// 				_LOG(C(L"TimerDump")<<boost::wformat(L"(%d,%d)")%i%timerUnitVec[i].timerNode.timeName<<OVER);	
// 			}
		}

		uint32 _Timers::CreateTimer(uint32 interval,TimerCallback & callback,uint32 name,int32 repeatTimes)
		{
			uint32 allocatedNode=AllocNode();
			if(allocatedNode==-1)
				return -1;
			else
			{
				timerUnitVec[allocatedNode].timerNode=TimerNode(interval,callback,repeatTimes,name);
				Dump();
				return allocatedNode;
			}
		}

		uint32 _Timers::CreateTimer(IntervalFunctor &intervalFunctor,TimerCallback & callback,uint32 name,int32 repeatTimes)
		{
			uint32 allocatedNode=AllocNode();
			if(allocatedNode==-1)
				return -1;
			else
			{
				timerUnitVec[allocatedNode].timerNode=TimerNode(intervalFunctor,callback,repeatTimes,name);
				Dump();
				return allocatedNode;
			}
		}

		uint32 _Timers::GetNowTime()
		{
			return ::GetTickCount()/1000;
		}

		void _Timers::DestroyTimer(uint32 node)
		{
			FreeNode(node);
			Dump();
			destroyHappend=true;
		}

		void _Timers::OnTimer()
		{
			std::set<uint32> callBackedNode;

			uint32 now = GetNowTime();
			for(uint32 i=usedHead;i!=-1;)
			{
				//先记录下一个定时器的index，因为在回调中，定时器可能被删除。。。
				uint32 j=timerUnitVec[i].next;
				if( (timerUnitVec[i].timerNode.repeatTimes!=0)
					&&(timerUnitVec[i].timerNode.intervalFunctor.interval+ timerUnitVec[i].timerNode.basePoint<now)
					&& (callBackedNode.find(i)==callBackedNode.end())
				   )
				{
					callBackedNode.insert(i);

					destroyHappend=false;
					if(timerUnitVec[i].timerNode.callback != NULL)
					{
						timerUnitVec[i].timerNode.callback(i, timerUnitVec[i].timerNode.repeatTimes==1);
					}
					else
					{
						assert(0);
					}
				
					if(timerUnitVec[i].timerNode.repeatTimes!=-1)//-1表示周期
					{
						timerUnitVec[i].timerNode.repeatTimes--;
						timerUnitVec[i].timerNode.basePoint=now;
						timerUnitVec[i].timerNode.intervalFunctor.Compute();
					}
					else//周期
					{
						timerUnitVec[i].timerNode.basePoint=now;
						timerUnitVec[i].timerNode.intervalFunctor.Compute();
					}

					if(destroyHappend==true)
					{
						i=usedHead;
						continue;
					}
					
				}
					
				i=timerUnitVec[i].next;
			}
		}

		uint32 _Timers::AllocNode()
		{
			if(Full())
			{
				assert(0);
				return -1;
			}
			else
			{
				uint32 allocatedNode=freeHead;
				//free list op
				freeHead=timerUnitVec[freeHead].next;

				//used list op
				if(usedHead==-1)
					timerUnitVec[allocatedNode].next=-1;
				else
					timerUnitVec[allocatedNode].next=usedHead;
				usedHead=allocatedNode;

				return allocatedNode;
			}
		}

		uint32 _Timers::FreeNode(uint32 node)
		{
			assert(node<timerUnitVec.size() && node>=0);

			if(Empty() || (node>timerUnitVec.size()) || (node<0))
			{
				assert(0);
				return -1;
			}
			else
			{
				//used list op
				uint32 fronterNode=usedHead;
				uint32 resultNode=timerUnitVec[node].next;//dangerous
				if(fronterNode==node)
					usedHead=timerUnitVec[node].next;
				else
				{
					while(timerUnitVec[fronterNode].next!=node 
						&& timerUnitVec[fronterNode].next!=-1)
						fronterNode=timerUnitVec[fronterNode].next;

					assert(timerUnitVec[fronterNode].next!=-1);//meaning wrong free node

					timerUnitVec[fronterNode].next=timerUnitVec[node].next;
				}
				
				//free list op
				if(freeHead==-1)
					timerUnitVec[node].next=-1;
				else
					timerUnitVec[node].next=freeHead;
				freeHead=node;

				return resultNode;
			}
		}

		_Timers::TimerNode::TimerNode()
			:callback(NULL)
		{}

		_Timers::TimerNode::TimerNode(uint32 interval,TimerCallback & callback,int32 repeatTimes,uint32 name)
			:callback(callback),
			repeatTimes(repeatTimes),
			intervalFunctor(interval),
			basePoint(_Timers::GetNowTime()),
			timeName(name)
		{
		}

		_Timers::TimerNode::TimerNode(IntervalFunctor &intervalFunctor,TimerCallback & callback,int32 repeatTimes,uint32 name)
			:callback(callback),
			repeatTimes(repeatTimes),
			intervalFunctor(intervalFunctor),
			basePoint(_Timers::GetNowTime()),
			timeName(name)
		{
		}
	}
}