/**
* NetHelper.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include <winsock2.h>

extern unsigned int bindError;

namespace nethelper
{
	inline void SetSocketBuf(SOCKET oSock, int iBufLen) {
		if (oSock != INVALID_SOCKET) {
			setsockopt(oSock, SOL_SOCKET, SO_SNDBUF, (const char *)&iBufLen, sizeof(iBufLen));
			setsockopt(oSock, SOL_SOCKET, SO_RCVBUF, (const char *)&iBufLen, sizeof(iBufLen));
		}
	}

	inline SOCKET CreateSocket(bool bUdp) {
		SOCKET oSock;
		if (bUdp) oSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		else oSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		SetSocketBuf(oSock, 64 * 1024);
		return oSock;
	}

	inline void CloseSocket(SOCKET oSock) {
		if (oSock != INVALID_SOCKET) {

			closesocket(oSock);
		}
	}

	inline sockaddr_in IpPortToAddr(unsigned int uiNetIp, unsigned short usNetPort) {
		sockaddr_in oAddr;
		memset(&oAddr, 0, sizeof(oAddr));
		oAddr.sin_family = AF_INET;
		oAddr.sin_port = usNetPort;

		oAddr.sin_addr.S_un.S_addr = uiNetIp;

		return oAddr;
	}

	inline bool BindPort(SOCKET oSock, sockaddr_in& oAddr) {
		int re=bind(oSock, (sockaddr *)&oAddr, sizeof(oAddr));
		int err=WSAGetLastError();
		bindError=err;
		return  re!= SOCKET_ERROR;
	}

	inline bool SetReuse(SOCKET oSock, bool bResue = true) {
		int iReuse = bResue;
		return setsockopt(oSock, SOL_SOCKET, SO_REUSEADDR, (char*)&iReuse, sizeof(iReuse)) == 0;
	}

	inline bool SetNonBlock(SOCKET oScok, bool bNonBlock = true) {

			unsigned long v = bNonBlock;
			if (ioctlsocket(oScok, FIONBIO, &v) == SOCKET_ERROR) return false;
			else return true;

	}

	inline int RecvFrom(SOCKET oSock, void *pBuf, unsigned int uiBufLen, sockaddr_in& sa) {
	
		int iFromAdd = sizeof(sa);

		return recvfrom(oSock, (char *)pBuf, uiBufLen, 0, (struct sockaddr*)&sa, &iFromAdd);
	}

	inline int Recv(SOCKET oSock, void * pBuf, unsigned int uiBufLen) {
		if (oSock != INVALID_SOCKET) {

			return recv(oSock, (char *)pBuf, uiBufLen, 0);

		}
		return -1;
	}

	inline SOCKET CreateUdpSocket(unsigned int uiIP, unsigned short usPort, bool bSetNBlock = true, bool bSetReuse = true) {

		int count = 0;
		unsigned short tmp_port = 6000;
		sockaddr_in localaddr;
		// 确定一个值稍大的端口
		while(count++ < 40)
		{
			if(usPort == 0)
				localaddr = IpPortToAddr(uiIP, htons(tmp_port));
			else
				localaddr = IpPortToAddr(uiIP, usPort);

			SOCKET oSocket = CreateSocket(true);
			if (bSetNBlock) {
				if (!SetNonBlock(oSocket)) {
					CloseSocket(oSocket);
					return INVALID_SOCKET;
				}
			}
#ifndef WIN32
			if (bSetReuse) {
				if (!SetReuse(oSocket)) {
					CloseSocket(oSocket);
					return INVALID_SOCKET;
				}
			}
#endif
			if (!BindPort(oSocket, localaddr)) {
				tmp_port += rand() * 3 / RAND_MAX+1;
				CloseSocket(oSocket);
				continue;
			}
			SetSocketBuf(oSocket, 64 * 1024);

			return oSocket;
		}

		return INVALID_SOCKET;
	}

	inline int SendTo(SOCKET oSock, unsigned int uiDstIP, unsigned short usDstPort, void const * pBuf, unsigned int uiLen) {
		if (oSock != INVALID_SOCKET) {
			sockaddr_in sa = IpPortToAddr(uiDstIP, usDstPort);
			return sendto(oSock,
				(const char *)pBuf,
				uiLen,
				0,
				(sockaddr *)&sa,
				sizeof(sa));
		}
		return -1;
	}

	inline int Send(SOCKET oSock, const void * pBuf, unsigned int uiLen) {
		if (oSock != INVALID_SOCKET) {

			return send(oSock, (const char *)pBuf, uiLen, 0);

		}
		return -1;
	}
}; //end namespace netlayer;


