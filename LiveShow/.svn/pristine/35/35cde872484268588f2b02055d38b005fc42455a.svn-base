#ifndef	__SOCKET_H__
#define	__SOCKET_H__


#include <sys/types.h>
#include <errno.h>

#ifdef WIN32
/************************************************************************/
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <process.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#define EINPROGRESS 	WSAEINPROGRESS
#define ETIMEDOUT 		WSAETIMEDOUT
#define ECONNREFUSED 	WSAECONNREFUSED
#define ECONNREFUSED 	WSAECONNREFUSED
#define EWOULDBLOCK 	WSAEWOULDBLOCK
#define ENOBUFS         WSAENOBUFS
#define SOCKET_EINTR 	WSAEINTR

#define	EPERM		 	1	/* Operation not permitted */
#define EPROTO          WSAEPROTOTYPE
#define ENODATA         61

#define socket_errno() 		WSAGetLastError()
#define socket_errinfo() 	"N/A"
#define socklen_t       int
#else
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#define socket_errno() errno
#define socket_errinfo() strerror(errno)
#define strerror_s(x) strerror(x)
#define SOCKET_EINTR 			EINTR

#endif



#include <string>

#include "..\include\Types.h"

typedef int sockobj_t;
typedef int sockerr_t;

#if 0
// some custom bits (we rely on pollfd events being a 16-bit type)
// note that these are combined with POLLIN/POLLOUT
#define XPOLLACC    0x00010000  /* accept */
#define XPOLLCNX    0x00020000  /* connect */
#define INFTIM (-1)
#endif

#define WAITOBJ_MAX          0xffff
#define WAITOBJ_IS_VALID(obj)       (obj.fd != -1)
#if 0
#define WAIT_EVENT_ACCEPT(wevt)     (wevt & XPOLLACC)
#define WAIT_EVENT_CONNECT(wevt)    (wevt & XPOLLCNX)
#define WAIT_EVENT_READ(wevt)       (wevt & EPOLLIN)
#define WAIT_EVENT_WRITE(wevt)      (wevt & EPOLLOUT)
#define WAIT_EVENT_EXCEPT(wevt)     (wevt & EPOLLPRI)
#endif
#ifndef WIN32
#define INVALID_SOCKET -1
#endif
#define SF_NONE     0
#if 0
#define SF_ACCEPT   (XPOLLACC|EPOLLIN)
#define SF_CONNECT  (XPOLLCNX|EPOLLOUT)
#define SF_READ     EPOLLIN
#define SF_WRITE    EPOLLOUT
#define SF_EXCEPT   EPOLLPRI
#define SF_ERR		EPOLLERR
#define SF_HUP		EPOLLHUP
#define SF_ALL      (XPOLLACC|XPOLLCNX|EPOLLIN|EPOLLOUT|EPOLLPRI)
#endif
#define SOCKERR_NONE        0
#define SOCKERR_WOULDBLOCK  EAGAIN
#define SOCKERR_INPROGRESS  EINPROGRESS
#define SOCKERR_CONNRESET   EPIPE
#define SOCKERR_EOF         0x7FFFFFFF
		
/********************* Class TSocket Definition **********/
typedef enum __SockType
{
	SKT_TCP			= 0,
	SKT_UDP			= 1,
	SKT_RAW			= 2
} SockType;

class TSocket;

class TSocketEvent
{
public:
    virtual void OnConnect(TSocket * skp) = 0;
	virtual void OnRead(TSocket * skp) = 0;
	virtual void OnWrite(TSocket * skp) = 0;
	virtual void OnSend(TSocket * skp) = 0;
	virtual void OnRecv(TSocket * skp) = 0;
	virtual void OnAccept(TSocket * skp) = 0;
	virtual void OnExcept(TSocket * skp) = 0;
    virtual void OnClosed(TSocket * skp ) = 0;
};

class TSocket 
{
private:
	int		m_sock;
	int 	m_waitsec;
	int 	m_status;
	
	bool	m_async;
	bool 	m_canRead;
	bool 	m_canWrite;
	
	TSocketEvent * m_psockevent;

	UINT32      m_uSelectFlags;
	sockerr_t   m_err; 
	

public:
	TSocket(bool async = false, int nWaits = 3000);
	virtual	~TSocket(void);
	
	void CreateTCPSock(void);
	void CreateUDPSock(void);
	uerr_t CreateSock(SockType nType = SKT_TCP);

	void SetSocketEvent(TSocketEvent * psockevent);
	TSocketEvent * GetSocketEvent(void);
	
	int SetNonBlock(int nonblock);	
	void SetReuseAddr(void);
#ifndef 	WIN32	//该函数在win下不支持
	void SetIPTOS(void);
#endif
	
	int Attach(int fd);
	int GetHandle(void);

	
	/*****************************************/
	struct sockaddr_in GetLocalAddr( void ); 
	struct sockaddr_in GetPeerAddr( void );
	void Select(UINT32 nWhich);
	UINT32	GetSelectFlags(void);
	/*****************************************/
	//for Client
	virtual bool Connect(UINT ipaddr, UINT16 port, int timeout = 0) { return true;}
	virtual int Connect(CPCHAR ipaddr, UINT16 port, int timeout = 0)
	{ 
		return UNKNOWNERROR;
	}
	
	virtual UINT checkConnect();
	//for Server
	virtual uerr_t Bind(CPCHAR ipaddr, UINT16 port) { return UNKNOWNERROR; }
	virtual uerr_t Accept(int * datasock) { return UNKNOWNERROR;}
	virtual uerr_t Listen(void) { return UNKNOWNERROR; }

	virtual int Accept(std::string& ipaddr, UINT16& port){ return -1;}
	/*****************************************/

	void Close(void);

//	TPeerConnection * GetOwner(void);
//	void SetOwner(TPeerConnection * pconn);
	void SetMaskRead(bool mask);
	void SetMaskWrite(bool mask);
	bool GetMaskRead(void);
	bool GetMaskWrite(void);
//	Reserved for PeerConnection 
	virtual void HandleClose(void) {};
	virtual void HandleRead(void) {};
	virtual void HandleWrite(void) {};

public :


#if 0
	void DispAddr(PCHAR Addr);
#endif
	UINT	Host2Ip(CPCHAR hostname);
	PCHAR	Host2Str(CPCHAR hostname);
#ifndef WIN32
	void	SetAsync(bool async);
#endif
	void 	SetWaitSec(int val);
	void	SetTimeout(int sec);

	int 	get_socketfd();

	// for UDPSocket
	void 	SendTo(CPCHAR ipaddr, UINT16 port, CPCHAR buf, UINT nLen);
	void 	RecvFrom(CPCHAR ipaddr, UINT16 port, PCHAR buf, UINT nLen);

	// for TCPSocket
	int 	select_fd(int maxmillseconds, int writep);	//---unit millseconds
	int 	Send(char *buffer, ULONG64 size, int flags, int timeout = 100); 
	int 	Send(char *buffer, ULONG64 size, int flags, int timeout, ULONG64& sentSize); 
	int 	Recv(char *buffer, int size, int flags, int timeout = 100);	
	int 	direct_recv(char *buffer, int size, int flags);
	int 	direct_send(char *buffer, int size, int flags);
};

/********************* Class TSocketCli Definition **********/
class TSocketCli : public TSocket
{
public :
	int Connect(CPCHAR ipaddr, UINT16 port, int timeout = 100);
	uerr_t SConnect(CPCHAR ipaddr, UINT16 port, int timeout = 100);
	bool Connect(UINT ipaddr, UINT16 port, int timeout = 100);
};


#endif
