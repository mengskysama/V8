/**
* 51ktv.h
*
* @version 1.0 (2011-09-30)
* @author plushu
*
*/

#pragma once

#define WIN32_LEAN_AND_MEAN

#include "51ktvtype.h"
#include "windows.h"
#include <vector>
#include <utility>
#include <string>
#include <xstring>
#include <list>
#include <set>
#include <map>

enum ModuleId
{
	MODULE_ID_INVALID = -1,
	
	MODULE_ID_CORE = 0X01,
	MODULE_ID_HALL,
	MODULE_ID_WEBSERVICE,
    MODULE_ID_ROOM,
	MODULE_ID_AV,
	MODULE_ID_RELAY,

	MODULE_ID_END,//最后一个，边界判断
};

enum 
{
	EVENT_VALUE_RANGE	= 0XFFF,
	MESSAGE_VALUE_RANGE	= 0XFFF,
	SERVICE_VALUE_RANGE	= 0XFFF,

	EVENT_VALUE_INVALID	= -1,
	EVENT_VALUE_BASE	= 0x10001000,
	EVENT_VALUE_END		= 0X1FFFFFFF,

	MESSAGE_VALUE_INVALID	= -1,
	MESSAGE_VALUE_BASE		= 0x20001000,
	MESSAGE_VALUE_END		= 0X2FFFFFFF,

	SERVICE_VALUE_INVALID	= -1,
	SERVICE_VALUE_BASE		= 0x30001000,
	SERVICE_VALUE_END		= 0X3FFFFFFF,
};

#define EVENT_VALUE_BEGIN(X)		((X*0X1000)+EVENT_VALUE_BASE+0X01)
#define EVENT_VALUE_END(X)			((X*0X1000)+EVENT_VALUE_BASE+EVENT_VALUE_RANGE)

#define MESSAGE_VALUE_BEGIN(X)		((X*0X1000)+MESSAGE_VALUE_BASE+0X01)
#define MESSAGE_VALUE_END(X)		((X*0X1000)+MESSAGE_VALUE_BASE+MESSAGE_VALUE_RANGE)

#define SERVICE_VALUE_BEGIN(X)		((X*0X1000)+SERVICE_VALUE_BASE+0X01)
#define SERVICE_VALUE_END(X)		((X*0X1000)+SERVICE_VALUE_BASE+SERVICE_VALUE_RANGE)

#define MODULIE_ID_FROM_EVENT_VALUE(X)			((X-EVENT_VALUE_BASE-0X01)/0X1000)
#define MODULIE_ID_FROM_MESSAGE_VALUE(X)		((X-MESSAGE_VALUE_BASE-0X01)/0X1000)
#define MODULIE_ID_FROM_SERVICE_VALUE(X)		((X-SERVICE_VALUE_BASE-0X01)/0X1000)

__forceinline bool IsEventValue(EventValue const x)
{
	return (((x)&0xF0000000) == 0x10000000);
}
__forceinline bool IsMessageValue(MessageValue const x)
{
	return (((x)&0xF0000000) == 0x20000000);
}
__forceinline bool IsServiceValue(ServiceValue const x)
{
	return (((x)&0xF0000000) == 0x30000000);
}

enum EventValueRanges
{
	EVENT_VALUE_SYS_MAIN_LOOP_EXIT = EVENT_VALUE_BASE,

	EVENT_VALUE_CORE_BEGIN			= EVENT_VALUE_BEGIN(MODULE_ID_CORE),	
	EVENT_VALUE_CORE_END			= EVENT_VALUE_END(MODULE_ID_CORE),
	
	EVENT_VALUE_WEBSERVICE_BEGIN	= EVENT_VALUE_BEGIN(MODULE_ID_WEBSERVICE),
	EVENT_VALUE_WEBSERVICE_END		= EVENT_VALUE_END(MODULE_ID_WEBSERVICE),
   
	EVENT_VALUE_HALL_BEGIN			= EVENT_VALUE_BEGIN(MODULE_ID_HALL),//0x10003001
	EVENT_VALUE_HALL_END			= EVENT_VALUE_END(MODULE_ID_HALL),

	EVENT_VALUE_ROOM_BEGIN			= EVENT_VALUE_BEGIN(MODULE_ID_ROOM),
	EVENT_VALUE_ROOM_END			= EVENT_VALUE_END(MODULE_ID_ROOM),

	EVENT_VALUE_AV_BEGIN			= EVENT_VALUE_BEGIN(MODULE_ID_AV),
	EVENT_VALUE_AV_END				= EVENT_VALUE_END(MODULE_ID_AV),
};

enum MessageValueRanges
{
	MESSAGE_VALUE_SYS_ALL_MODULE_LOADED = MESSAGE_VALUE_BASE,
	MESSAGE_VALUE_SYS_CYCLE_TRIGGER,
	MESSAGE_VALUE_SYS_PRELOGOUT,
	MESSAGE_VALUE_SYS_LOGOUT,
	MESSAGE_VALUE_SYS_CHANGE_USER,

	MESSAGE_VALUE_CORE_BEGIN		= MESSAGE_VALUE_BEGIN(MODULE_ID_CORE),
	MESSAGE_VALUE_CORE_END			= MESSAGE_VALUE_END(MODULE_ID_CORE),
	
	MESSAGE_VALUE_WEBSERVICE_BEGIN	= MESSAGE_VALUE_BEGIN(MODULE_ID_WEBSERVICE),
	MESSAGE_VALUE_WEBSERVICE_END	= MESSAGE_VALUE_END(MODULE_ID_WEBSERVICE),	

	MESSAGE_VALUE_AV_BEGIN			= MESSAGE_VALUE_BEGIN(MODULE_ID_AV),
	MESSAGE_VALUE_AV_END			= MESSAGE_VALUE_END(MODULE_ID_AV),

	MESSAGE_VALUE_HALL_BEGIN		= MESSAGE_VALUE_BEGIN(MODULE_ID_HALL),
	MESSAGE_VALUE_HALL_END			= MESSAGE_VALUE_END(MODULE_ID_HALL),

};

enum ServiceValueRanges
{
	SERVICE_VALUE_SYS_STAT_NETCMD	= SERVICE_VALUE_BASE,

	SERVICE_VALUE_CORE_BEGIN		= SERVICE_VALUE_BEGIN(MODULE_ID_CORE),  
	SERVICE_VALUE_CORE_END			= SERVICE_VALUE_END(MODULE_ID_CORE),

	SERVICE_VALUE_AV_BEGIN			= SERVICE_VALUE_BEGIN(MODULE_ID_AV),
	SERVICE_AVLUE_AV_END			= SERVICE_VALUE_END(MODULE_ID_AV) ,

	SERVICE_VALUE_ROOM_BEGIN		= SERVICE_VALUE_BEGIN(MODULE_ID_ROOM),
	SERVICE_VALUE_ROOM_END			= SERVICE_VALUE_END(MODULE_ID_ROOM) ,

	SERVICE_VALUE_HALL_BEGIN		= SERVICE_VALUE_BEGIN(MODULE_ID_HALL),
	SERVICE_VALUE_HALL_END			= SERVICE_VALUE_END(MODULE_ID_HALL) ,
};

struct ExtraInfo;

struct Event
{
	Event()
		:eventValue(EVENT_VALUE_INVALID),
		param0(0),
		param1(0),
		param2(0),
		m_pstExtraInfo(NULL),
		srcMId(MODULE_ID_INVALID),
		desMId(MODULE_ID_INVALID)
	{
	}

	Event(EventValue  const eventValue,
		ModuleIdentity const srcMId,
		ModuleIdentity const desMId,
		param const param0=0,
		param const param1=0,
		param const param2=0,
		ExtraInfo *extraInfo=NULL)
		:eventValue(eventValue),
		param0(param0),
		param1(param1),
		param2(param2),
		srcMId(srcMId),
		desMId(desMId),
		m_pstExtraInfo(extraInfo)
	{
	}
	EventValue  eventValue;
	ModuleIdentity srcMId;
	ModuleIdentity desMId;
	param param0;
	param param1;
	param param2;
	ExtraInfo* m_pstExtraInfo;
};


template<ModuleIdentity id>
struct  MakeEvent
{
	Event operator()(EventValue  const eventValue,
		ModuleIdentity const desMId,
		param const param0=0,
		param const param1=0,
		param const param2=0,
		ExtraInfo *extraInfo=NULL)
	{
		return Event(eventValue,
			id,
			desMId,
			param0,
			param1,
			param2,
			extraInfo);
	}
};

struct ExtraInfo
{
	virtual ~ExtraInfo(){} ;
};

struct Message
{
	Message()
		:messageValue(MESSAGE_VALUE_INVALID),
		param0(0),
		param1(0),
		param2(0),
		m_pstExtraInfo(NULL),
		srcMId(MODULE_ID_INVALID)
	{
	}
	Message(MessageValue  const messageValue,
		ModuleIdentity const srcMId,
		param const param0 = 0,
		param const param1 = 0,
		param const param2 = 0,
		ExtraInfo* extraInfo = NULL)
		:messageValue(messageValue),
		param0(param0),
		param1(param1),
		param2(param2),
		srcMId(srcMId),
		m_pstExtraInfo(extraInfo)
	{
	}

	MessageValue  messageValue;
	ModuleIdentity srcMId;
	param param0;
	param param1;
	param param2;
	ExtraInfo* m_pstExtraInfo;
};

template<ModuleIdentity id>
struct MakeMessage
{
	Message operator ()(
			MessageValue  const messageValue,
			param const param0 = 0,
			param const param1 = 0,
			param const param2 = 0,
			ExtraInfo* extraInfo = NULL)
	{
		return Message(messageValue,
			id,
			param0,
			param1,
			param2,
			extraInfo);
	}
};

#include "imodule.h"
#include "imodulemanager.h"

//warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss
#pragma warning (disable:4819)
//warning C4227: anachronism used : qualifiers on reference are ignored
#pragma warning (disable:4227)
#pragma warning (default:4819)
#pragma warning (default:4227)

#define SYS_CYCLE_ELAPSE 100//ms
#define TOKEN_MAX_LEN (1024)
#define ACCOUNT_MAX_LEN (70)//字符为单位
#define SHOWUIN_MAX_LEN (11)//字符为单位
#define PASSWORD_MAX_LEN (33)//字符为单位
#define NICK_MAX_LEN (17)//字符为单位

struct PreLogoutParam
{
	int bSuc;	// bool
	int bOffline;	// bool
	int bForce; //bool

	PreLogoutParam()
	{
		bSuc = true;
		bOffline = false;
		bForce = false;
	}
};

#define SHARE_MEMORY_INFO_VERSION	0

struct ShareMemInfo
{
	int unVersion;
	HWND hwndHall;
	HWND hwndLogin;

	ShareMemInfo()
	{
		unVersion = SHARE_MEMORY_INFO_VERSION;
		hwndHall = hwndLogin = NULL;
	}
};