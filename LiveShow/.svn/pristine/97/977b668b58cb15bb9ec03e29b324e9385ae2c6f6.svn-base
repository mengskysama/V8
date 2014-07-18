#pragma once

#ifdef _DEBUG
//#include <vld.h>
#endif

#include "Common.h"
#include <map>
using namespace std;

UInt32 GetLocalIP(char* strInternetIP);

class Setting
{
public:
	Setting() : m_iMyUin(0),
				m_iMyIP(0),
				m_iRelayIP(0),
				m_sRelayPort(0),
				m_iMaxConnTime(0),
				m_iLostRate(0.0f)
	{}

	~Setting()
	{
	}

	void  Config(UInt32 iMyUin, UInt32 iRelayIP, UInt16 sRelayPort,UInt32 iMaxConnTime);

	inline UInt32        getMyUin()                    {  return m_iMyUin;       }
	inline UInt32        getMyIP()                     {  return m_iMyIP;        }
	inline UInt32        getRelayIP()                  {  return m_iRelayIP;     }
	inline UInt16        getRelayPort()                {  return m_sRelayPort;   }

	inline UInt32		 getMaxConnTime()			   {  return m_iMaxConnTime; } 
	inline void          setAVKey(UInt32 iRoomId,UInt32 ikey) {  m_mapKey[iRoomId]=ikey;   }	
	inline UInt32        getAVKey(UInt32 iRoomId)		{       
		std::map<UInt32,UInt32>::iterator iter;
		if ((iter=m_mapKey.find(iRoomId))!=m_mapKey.end()){
			return iter->second;
		}
		return 0;
	}

	float			 m_iLostRate;         //丢包率
private:

	UInt32           m_iMyIP;             //本地的ip地址
	UInt32           m_iMyUin;            //本地的Uin

	UInt32           m_iRelayIP;          //Relay服务器IP
	UInt16           m_sRelayPort;        //Relay服务器Port

	UInt32           m_iMaxConnTime;	  //最长的直连时间  超过时间没有收到心跳包

	std::map<UInt32,UInt32> m_mapKey;
};

extern Setting  g_Setting;

UInt64 MakeSession(UInt32 iRoomId,UInt32 iFromUin);

#define BAD_MEM_WRITE(a,b,c)  if (IsBadWritePtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可写 time = %I64d \n",(c),GetTickCount());  return;  }

#define BAD_MEM_WRITE_EX(a,b,c)  if (IsBadWritePtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可写 time = %I64d \n",(c),GetTickCount());  }

#define BAD_MEM_READ(a,b,c)  if (IsBadReadPtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可读 time = %I64d \n",(c),GetTickCount());  return;  }

#define BAD_MEM_WRITE_RETURN(a,b,c,d) if (IsBadWritePtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可写 time = %I64d \n",(c),GetTickCount());  return d;  }

#define BAD_MEM_READ_RETURN(a,b,c,d)  if (IsBadReadPtr((a),(b))) \
{  Assert(0); ErrTrace("%s: 内存不可读 time = %I64d \n",(c),GetTickCount());  return d;  }