#pragma once

#include "51ktv.h"

struct IModuleManager
{
	virtual bool  PushEvent(Event const& )        = 0;//在主线程调用，  异步操作
	virtual bool  PushMessage(Message const&)     = 0;//在主线程调用，  异步操作
	virtual bool  PushAsynEvent(Event const&)     = 0;//在非主线程调用，异步操作
	virtual bool  PushAsynMessage(Message const&) = 0;//在非主线程调用，异步操作
	virtual int32 CallService(ServiceValue const serviceValue, param) = 0;//在主线程调用，同步操作
};