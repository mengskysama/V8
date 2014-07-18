/******************************************************************************
 * Socket.cpp - Socket 实现文件
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     实现了tcp/upd socket 通讯的封装
 * modification history
 * --------------------
 * 01a, 19nov2008, Davidfan written
 * --------------------
 ******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#ifndef WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#endif
#include <string>
#include <iostream>

#include "..\include\Types.h"
#include "..\include\Utils.h"
#include "Socket.h"

#ifndef	 HTTPTRANSFER_RELEASE
#include "..\include\log.h"
#else
#include "..\include\log_stun.h"
#endif

#define USE_GET_ADDR_INFO
//#include "dbg.h"
//#include "dget.h"
/********************* Class TSocket Impl **********/

TSocket::TSocket(bool async, int nWaits)
{
	m_sock			= INVALID_SOCKET;
	m_status		= -1;
	m_async			= async;
	m_waitsec		= nWaits;
	m_canRead		= false; 
	m_canWrite		= false;
	m_uSelectFlags  = SF_NONE;
	m_err			= SOCKERR_NONE;
	
}

TSocket::~TSocket(void)
{
	Close();	
}

#ifndef WIN32
void TSocket::SetAsync(bool val)
{
	int	on = 0;
	m_async = val;
	if (m_async) on	= 1;
	ioctl(m_sock, FIONBIO, &on);
}
#endif
void TSocket::SetWaitSec(int val)
{
	m_waitsec = val;
}

void TSocket :: SetSocketEvent(TSocketEvent * psockevent)
{
	m_psockevent = psockevent;
}

TSocketEvent * TSocket :: GetSocketEvent(void)
{
	return m_psockevent;
}

#ifndef 	WIN32	//该函数在win下不支持
void TSocket::SetIPTOS(void)
{
	int Tos = IPTOS_THROUGHPUT;
 	setsockopt(m_sock, IPPROTO_IP, IP_TOS, (char *)&Tos, sizeof(Tos));
}
#endif

void TSocket::SetReuseAddr(void)
{
    int tmp = 1;
	setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR,
							(const char*)&tmp, sizeof(tmp));  
}

#if 0
int TSocket::SetNonBlock(void)
{
	int flags, ret;
	fcntl(m_sock, F_GETFL, &flags);
	flags |= O_NONBLOCK;
	ret = fcntl(m_sock, F_SETFL, &flags);
	return ret;
}
#endif

int TSocket::SetNonBlock(int nonblock)
{
#ifdef WIN32
	u_long n = nonblock;
	return ioctlsocket(m_sock, FIONBIO, &n);
#else
	int val = fcntl(m_sock, F_GETFL, 0);
	if(nonblock)
		return fcntl(m_sock, F_SETFL, val | O_NONBLOCK);
	else
		return fcntl(m_sock, F_SETFL, val & ~O_NONBLOCK);
#endif
}

void TSocket::SetMaskRead(bool mask)
{
	m_canRead = mask;
}

void TSocket::SetMaskWrite(bool mask)
{
	m_canWrite = mask;
}

bool TSocket::GetMaskRead(void)
{
	return m_canRead;
}

bool TSocket::GetMaskWrite(void)
{
	return m_canWrite;
}

void TSocket::CreateTCPSock(void)
{
	Close();
	m_sock = (int)socket(AF_INET, SOCK_STREAM, 0);
	//SetNonBlock();
}

void TSocket::CreateUDPSock(void)
{
	Close();
	m_sock = (int)socket(AF_INET, SOCK_DGRAM, 0);
	//SetNonBlock();
}

int TSocket::Attach(int fd)
{
	m_sock = fd;
	
	return 0;

}

uerr_t TSocket::CreateSock(SockType nType)
{
	if (nType == SKT_TCP)
		CreateTCPSock();
	
	if (nType == SKT_UDP)
		CreateUDPSock();

	if (m_sock < 0)
	{
		LOG_DEBUG("socket() error!\n");
		return CONSOCKERR;
	}

	return NOCONERROR;
}

void TSocket::Close(void)
{
	if (m_sock >= 0)
	{
#ifdef WIN32
		closesocket(m_sock);
#else
		close(m_sock);
#endif
		m_sock = -1;
	}
}


int TSocket::GetHandle(void)
{
	if (m_sock < 0)
		return -ENODATA;
	return m_sock;
}

void TSocket::SetTimeout(int sec)
{
	struct timeval to;
	to.tv_sec	= sec;
	to.tv_usec	= 0;
	setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
}

struct sockaddr_in TSocket :: GetLocalAddr( void ) 
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	if( getsockname( m_sock, (sockaddr*)&addr, &len ) == 0 ){}
	return addr;
}

struct sockaddr_in TSocket :: GetPeerAddr( void )
{
   	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	if( getpeername( m_sock, (sockaddr*)&addr, &len ) == 0 ){}
   	return addr;
}

void TSocket :: Select(UINT32 nWhich)
{
	if(m_uSelectFlags != nWhich)
		m_uSelectFlags = nWhich;
}

UINT32 TSocket :: GetSelectFlags(void)
{
	return m_uSelectFlags;
}

void TSocket::SendTo(CPCHAR ipaddr, UINT16 port, CPCHAR buf, UINT nLen)
{
	struct sockaddr_in addr;
	socklen_t len = (socklen_t)sizeof(addr);
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(port);
	sendto(m_sock, buf, nLen, 0, (struct sockaddr *)&addr, len);
}

void TSocket::RecvFrom(CPCHAR ipaddr, UINT16 port, PCHAR buf, UINT nLen)
{
	struct sockaddr_in addr;
	socklen_t len = (socklen_t)sizeof(addr);
			
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(port);
	recvfrom(m_sock, buf, nLen, 0, (struct sockaddr *)&addr, &len);
}

int TSocket :: select_fd(int maxmillseconds, int writep)
{
	int ret;
    fd_set fds, exceptfds;
    struct timeval timeout;
    FD_ZERO(&fds);
    FD_SET(m_sock, &fds);
    FD_ZERO(&exceptfds);
    FD_SET(m_sock, &exceptfds);
    timeout.tv_sec = 0;
    timeout.tv_usec = maxmillseconds*1000;
	
    ret = select(m_sock + 1, writep ? NULL : &fds, writep ? &fds : NULL,
													 &exceptfds,&timeout);
	
	return ret;
}

int TSocket :: get_socketfd()
{
	return m_sock;
}


int TSocket :: Recv(char *buffer, int size, int flags, int timeout)
{
    int ret;
    int arglen;

    arglen = sizeof(int);
   
    do
    {
		if (timeout)
		{
	    	do
	    	{
				ret = select_fd(timeout, 0);
	    	} while (ret == -1 && socket_errno() == SOCKET_EINTR);

	    	if (ret <= 0)
	    	{
				if (ret == 0)
#ifdef WIN32
					WSASetLastError(ETIMEDOUT);
#else
		    		socket_errno() = ETIMEDOUT;
#endif
				return -1;
	    	}
		}

		ret = recv(m_sock, buffer, size, flags);

    } while (ret == -1 && socket_errno() == SOCKET_EINTR);

    return ret;
	
}

int TSocket :: direct_recv(char *buffer, int size, int flags)
{	
	return recv(m_sock, buffer, size, flags);
}

int TSocket :: direct_send(char *buffer, int size, int flags)
{	
	return send(m_sock, buffer, size, flags);;
}

int TSocket :: Send(char *buffer, ULONG64 size, int flags, int timeout, ULONG64& sentSize)
{
    int ret = 0;
	int err = 0;
	UINT32 tosend = 0;
	sentSize = 0;
	
//	    while (size)
//	    {	
//按照应用要求，上报进度间隔以send为边界，每次send都上报
		do
		{
	    	if (timeout)
	    	{
				do
				{
		    		ret = select_fd(timeout, 1);
					err = socket_errno();
				} while (ret == -1 && err == SOCKET_EINTR);
				
				if (ret <= 0)
				{
					LOG_DEBUG("select_fd error ret=%d err=%d\n", ret, err);
		    		if (ret == 0)
#ifdef WIN32
						WSASetLastError(EWOULDBLOCK);
#else
		    			socket_errno() = EWOULDBLOCK;
#endif
		    		return -1;
				}
	    	}	

			if(size > MAX_MSG_SIZE)
			{
				tosend = MAX_MSG_SIZE;
			}
			else
			{
				tosend = (UINT32)size;				
			}
			ret = send(m_sock, buffer, tosend, flags);

		} while (ret == -1 && socket_errno() == SOCKET_EINTR);
	
		if (ret <= 0)
	    	return ret;

		sentSize += ret;
		buffer += ret;
		size -= ret;
    //}
    return ret;
}

int TSocket :: Send(char *buffer, ULONG64 size, int flags, int timeout)
{
    int ret = 0;
	int err = 0;
	UINT32 tosend = 0;
	
    while (size)
    {
		do
		{
	    	if (timeout)
	    	{
				do
				{
		    		ret = select_fd(timeout, 1);
					err = socket_errno();
				} while (ret == -1 && err == SOCKET_EINTR);
				
				if (ret <= 0)
				{
					LOG_DEBUG("select_fd error ret=%d err=%d\n", ret, err);
		    		if (ret == 0)
#ifdef WIN32
						WSASetLastError(ETIMEDOUT);
#else
		    			socket_errno() = ETIMEDOUT;
#endif
		    		return -1;
				}
	    	}	

			if(size > MAX_MSG_SIZE)
			{
				tosend = MAX_MSG_SIZE;
			}
			else
			{
				tosend = (UINT32)size;				
			}
			ret = send(m_sock, buffer, tosend, flags);

		} while (ret == -1 && socket_errno() == SOCKET_EINTR);
	
		if (ret <= 0)
	    	break;
			
		buffer += ret;
		size -= ret;
    }
    return ret;
}

UINT TSocket::checkConnect()
{
	int sock = -1;
	socklen_t arglen = sizeof(int);	int status; 
	
	sock = GetHandle();
	
	if(getsockopt(sock,SOL_SOCKET,SO_ERROR,(char*)&status, &arglen) < 0)
		status = socket_errno();
	
	return status;
}

#ifndef USE_GET_ADDR_INFO
UINT TSocket::Host2Ip(CPCHAR hostname)
{
	UINT addr = INADDR_NONE;
	
	struct hostent *he = NULL;
	struct in_addr **ip = NULL;
	
	if ((NULL == hostname) || ('\0' == hostname))
		goto errout;
	he = gethostbyname(hostname);
	if (NULL == he)
		goto errout;
	
	ip = (struct in_addr **)(he->h_addr_list);
	
	if (ip && *ip) addr = ((struct in_addr *)*ip)->s_addr;
	
errout:
	return addr;
}


PCHAR TSocket::Host2Str(CPCHAR hostname)
{
	char * addr = NULL;
	struct hostent *he = NULL;
	struct in_addr **ip = NULL;
	
	if ((NULL == hostname) || ('\0' == hostname))
		return NULL;
	
	he = gethostbyname(hostname);
	if (NULL == he)
		return NULL;
	
	ip = (struct in_addr **)(he->h_addr_list);
	if (ip && *ip)
		addr = inet_ntoa(*(struct in_addr *)*ip);
	
	return addr;
}
#else
UINT TSocket::Host2Ip(CPCHAR hostname)
{
	UINT addr = INADDR_NONE;
	
	struct addrinfo hints,*addrinfo = NULL;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	//hints.ai_flags = AI_ADDRCONFIG;
	if ((NULL == hostname) || ('\0' == hostname))
		goto errout;
	
	if(getaddrinfo(hostname,NULL,&hints,&addrinfo) == 0 && addrinfo != NULL)
	{
		addr = ((struct sockaddr_in *)addrinfo->ai_addr)->sin_addr.s_addr;
		freeaddrinfo(addrinfo);
	}
	
	errout:
		return addr;
}

PCHAR TSocket::Host2Str(CPCHAR hostname)
{
	char * addr = NULL;
	
	struct addrinfo hints,*addrinfo = NULL;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	//hints.ai_flags = AI_ADDRCONFIG;
	if ((NULL == hostname) || ('\0' == hostname))
		goto errout;
	
	if(getaddrinfo(hostname,NULL,&hints,&addrinfo) ==0 && addrinfo != NULL)
	{
		addr = inet_ntoa(((struct sockaddr_in*)addrinfo->ai_addr)->sin_addr);
		freeaddrinfo(addrinfo);
	}
	
	errout:
		return addr;
}
#endif

#if 0
void TSocket :: DispAddr(PCHAR Addr)
{
	u_int8_t split[4];
	u_int32_t ip;
	u_int32_t *x = (u_int32_t *) Addr;
	ip = ntohl(*x);
	split[0] = (ip & 0xff000000) >> 24;
	split[1] = (ip & 0x00ff0000) >> 16;
	split[2] = (ip & 0x0000ff00) >> 8;
	split[3] = (ip & 0x000000ff);
	dbgout("%d.%d.%d.%d", split[0], split[1], split[2], split[3]);
}
#endif		
/********************* Class TSocketCli Impl **********/

int TSocketCli::Connect(CPCHAR ipaddr, UINT16 port, int timeout)
{	
	int sock = -1;
	int status;  
	struct sockaddr_in addr;   
	
	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(port);
	
	SetNonBlock(true);
	sock = GetHandle();

	status = connect(sock, (struct sockaddr *)&addr, 
									sizeof(struct sockaddr_in));
	return status;
}


uerr_t TSocketCli::SConnect(CPCHAR ipaddr, UINT16 port, int timeout)
{	
	int sock = -1;
	int status, noblock, opt;  
	struct sockaddr_in addr;   
	
	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(port);
	
	noblock = SetNonBlock(true);
	sock = GetHandle();

	status = connect(sock, (struct sockaddr *)&addr, 
									sizeof(struct sockaddr_in));
	int errcode = socket_errno();
	LOG_DEBUG("status=%d noblock=%d err=%d\n", 
		status, noblock, errcode);

#ifdef WIN32
	if(status == -1 && noblock != -1 && errcode == EWOULDBLOCK)
#else
	if(status == -1 && noblock != -1 && errcode == EINPROGRESS)
#endif
	{
		fd_set writefd;
		struct timeval tv;
		FD_ZERO(&writefd);
		FD_SET(sock, &writefd);
		tv.tv_sec = 0;
		tv.tv_usec = timeout;
		status = select((sock + 1), NULL, &writefd, NULL, &tv);
		if (status > 0)
		{
			socklen_t arglen = sizeof(int);
			if(getsockopt(sock,SOL_SOCKET,SO_ERROR,(char*)&status, &arglen) < 0)
				status = errcode;
			if (status != 0)
			{
#ifdef WIN32
					WSASetLastError(status);
#else
		    		socket_errno() = status;
#endif
				status = -1;
			}
			if (socket_errno() == EINPROGRESS)
			{
#ifdef WIN32
					WSASetLastError(ETIMEDOUT);
#else
		    		socket_errno() = ETIMEDOUT;
#endif
			}
		}
		else if (status == 0)
		{
#ifdef WIN32
					WSASetLastError(ETIMEDOUT);
#else
		    		socket_errno() = ETIMEDOUT;
#endif
			status = -1;
		}
	}
	
	if (status < 0)
	{
		Close();
		if (socket_errno() == ECONNREFUSED)
			return CONREFUSED;
		else  
			return CONERROR;
	} 
	else  
	{ 
	//	SetNonBlock(false);
	}
	
    /* Enable KEEPALIVE, so dead connections could be closed
	 * earlier. Useful in conjuction with TCP kernel tuning
	 * in /proc/sys/net/ipv4/tcp_* files. */
	opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&opt, (int) sizeof(opt));  
	//Select(SF_CONNECT);

	return NOCONERROR;
}

bool TSocketCli::Connect(UINT ipaddr, UINT16 port, int timeout )
{
	struct sockaddr_in addr;
	int sock = -1;
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ipaddr;
	addr.sin_port = htons(port);

	sock = GetHandle();
	if(connect(sock, (struct sockaddr *)&addr, 
							sizeof(struct sockaddr_in)) < 0)
	{
		Close();
		return false;
	}
	return true;
}

#if 0
/********************* Class TSocketSvr Impl **********/

uerr_t TSocketSvr::Bind(CPCHAR ipaddr, UINT16 port)
{
	struct sockaddr_in addr;
	int sock = -1;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(port);

	sock = GetHandle();
 	if(bind(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_in)) < 0)
    {
		perror("bind error");
		Close();
		return BINDERR;
    }
	return BINDOK;
}

uerr_t TSocketSvr::Listen(void)
{
	int sock = GetHandle();
	if(listen(sock, 5) < 0)
	{
		perror("listen error");
		Close();
		return LISTENERR;
	}
	return LISTENOK;
}

int TSocketSvr::Accept(std::string& ipaddr, UINT16& port)
{
	struct sockaddr_in addr;
	socklen_t len = (socklen_t)(sizeof(addr));
	int sock = -1;

	memset(&addr, 0, sizeof(addr));

	sock = GetHandle();
	int sockfd = accept(sock, (struct sockaddr*)&addr, &len);
	if(sockfd != -1)
	{
		ipaddr = inet_ntoa(addr.sin_addr);
		port   = ntohs(addr.sin_port);
	}

	Select(SF_ACCEPT);	
	return sockfd;
}

uerr_t TSocketSvr::Accept(int * sockfd )
{
	struct sockaddr_in addr;
	socklen_t len = (socklen_t)(sizeof(addr));
	int listensock;

	memset(&addr, 0, sizeof(addr));

	listensock = GetHandle();
	*sockfd = accept(listensock, (struct sockaddr*)&addr, &len);
	if(*sockfd < 0)
	{
		perror("accept");
		return ACCEPTERR;
	}
	
	Select(SF_ACCEPT);	
	return ACCEPTOK;
}
#endif
