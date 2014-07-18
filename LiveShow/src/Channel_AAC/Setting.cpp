#include "stdafx.h"
#include "Setting.h"

#include "NetInterface.h"
#include "MyAssert.h"

#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")

//解决多网卡的问题
UInt32 GetLocalIP(char* strInternetIP)
{
	//static UInt32 s_localaddr=0; 
	UInt32 s_localaddr=0; 

	if(0==s_localaddr)
	{ 
		DWORD nIndex = 0;
 		DWORD nNum = 0;
 
		//获得最佳的网卡序号
 		if (NO_ERROR == ::GetBestInterface(::inet_addr(strInternetIP), &nIndex))
 		{
			Trace("GetLocalIP: best interfaces index = %d \n", nIndex);

			if(NO_ERROR == ::GetNumberOfInterfaces(&nNum))
			{
				Trace("GetLocalIP: all interfaces num = %d \n", nNum);
			}

			PIP_ADAPTER_INFO pAdapterInfo;
		
			pAdapterInfo = (IP_ADAPTER_INFO *) ::malloc( sizeof(IP_ADAPTER_INFO) );
			ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

			// Make an initial call to GetAdaptersInfo to get
			// the necessary size into the ulOutBufLen variable
			if (ERROR_BUFFER_OVERFLOW == ::GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) 
			{
				::free(pAdapterInfo);
				pAdapterInfo = (IP_ADAPTER_INFO *) ::malloc (ulOutBufLen); 
			}

			if (NO_ERROR == ::GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) 
			{
				PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
				while (pAdapter) 
				{
					if(nIndex==pAdapter->Index)
					{
						Trace("GetLocalIP: interface[index=%d] IP Address: %s\n", pAdapter->Index, pAdapter->IpAddressList.IpAddress.String);
						s_localaddr = ::inet_addr(pAdapter->IpAddressList.IpAddress.String);
						break;
					}
					
					pAdapter = pAdapter->Next;
				}
			}
			else 
			{
				Trace("GetLocalIP: Call to GetAdaptersInfo failed.\n");
			}

			::free(pAdapterInfo);
		}

		if (0==s_localaddr)
		{
			Trace("GetLocalIP: IP help get local IP failure, now use the gethostbyname()! \n");
			Assert(0);
			char name[256];
			if(::gethostname(name, 256)>=0)
			{
				hostent *ent= ::gethostbyname(name);
				if (ent && (ent->h_length > 0) )
				{
					s_localaddr= *(UInt32*)ent->h_addr_list[0]; 
				}
			} 
		}
	}
	
	return s_localaddr;
} 

Setting  g_Setting;

void  Setting::Config(UInt32 iMyUin, UInt32 iRelayIP, UInt16 sRelayPort,UInt32 iMaxConnTime)
{
	m_iMyIP       = GetLocalIP("60.190.39.200");
	m_iMyUin      = iMyUin;
	m_iRelayIP    = iRelayIP;
	m_sRelayPort  = sRelayPort;

	m_iMaxConnTime  = iMaxConnTime;

	Trace("Setting: P2P module config [local IP = %s relayIp = %s relayport = %d  maxConnTime=%d] \n", 
		NetInterface::IPUInt32ToString(m_iMyIP).c_str(),
		NetInterface::IPUInt32ToString(m_iRelayIP).c_str(),
		m_sRelayPort, 
		m_iMaxConnTime
		);
}

UInt64 MakeSession(UInt32 iRoomId,UInt32 iFromUin)
{
	UInt64 ret = iRoomId;
	return (ret<<32)+iFromUin;
}