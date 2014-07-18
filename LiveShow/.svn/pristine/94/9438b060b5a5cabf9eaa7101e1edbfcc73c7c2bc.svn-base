#ifndef __SOCKS_PROXY_H__
#define __SOCKS_PROXY_H__	

#include <errno.h>
#include <assert.h>
#include "Socket.h"

#define NSR_PROCESS_ERROR(expr)  \
	if (expr)	\
		goto errout
	
#define NSR_PROCESS_ERROUT(expr, retval)     \
	if (expr)	\
	{		\
		ret = retval;	\
		goto errout;	\
	}

#define NSR_PROCESS_RETURN(expr, retval)  \
	if (expr)	\
		return retval

#define NSR_PROCESS_SERVICE(expr)         \
	if ((expr) != 0)	\
		goto errout;

#define NSR_PROCESS_SOCKSLEN(ret, size)     \
	if (ret < 0)	\
		goto errout;	\
	else if (ret < (int)(size))	\
	{		\
		ret = -EPROTO;	\
		goto errout;	\
	}


#define SOCKS_BYTE_NULL         ((unsigned char)0x00)
#define SOCKS_WORD_NULL         ((unsigned short)0x0000)

// Username/Passwd 
#define SOCKS_AUTH_OK			((unsigned char)0x00)

// SOCKS Timeout
#define SOCKS_TIME_PEND         ((int)(-1))
#define SOCKS_TIME_RECV         ((int)(120*1000))
#define SOCKS_TIME_SEND         ((int)(120*1000))

// SOCKS Type
#define SOCKS_TYPE_4            ((int)0x01)
#define SOCKS_TYPE_4A           ((int)0x02)
#define SOCKS_TYPE_5            ((int)0x03)

// SOCKS Version
#define SOCKS_VERS_4            ((unsigned char)0x04)
#define SOCKS_VERS_4A           ((unsigned char)0x04)
#define SOCKS_VERS_5            ((unsigned char)0x05)
#define SOCKS_VERS_UNKNOWN      ((unsigned char)0xFF)

// SOCKS Cmd Code
#define SOCKS_CMDS_CONNECT      ((unsigned char)0x01)
#define SOCKS_CMDS_BIND         ((unsigned char)0x02)
#define SOCKS_CMDS_UDP          ((unsigned char)0x03)	// (SOCKS5 ONLY)
#define SOCKS_CMDS_UNKNOWN      ((unsigned char)0xFF)

#define SOCKS_MAXS_REQUEST      (2 * 1024)
#define SOCKS_MAXS_RESPONSE     (2 * 1024)

/* SOCKS4(A) err retcode */
#define SOCKS_REP4_SUCCESS      ((unsigned char)90)
#define SOCKS_REP4_FAIL         ((unsigned char)91)
#define SOCKS_REP4_NOIDENTD     ((unsigned char)92)
#define SOCKS_REP4_BADID		((unsigned char)93)

/* SOCKS5 err retcode */   
#define SOCKS_REP5_SUCCESS      ((unsigned char)0x00)
#define SOCKS_REP5_FAIL         ((unsigned char)0x01)
#define SOCKS_REP5_NOTALLOW     ((unsigned char)0x02)
#define SOCKS_REP5_NETUNREACH   ((unsigned char)0x03)
#define SOCKS_REP5_HOSTUNREACH  ((unsigned char)0x04)
#define SOCKS_REP5_CONNREF      ((unsigned char)0x05)
#define SOCKS_REP5_TTLEXP       ((unsigned char)0x06)
#define SOCKS_REP5_BADCMD       ((unsigned char)0x07)
#define SOCKS_REP5_BADADDR      ((unsigned char)0x08)

// SOCK5 auth SOCKS5 ONLY)
#define SOCKS_AUTH_NONE         ((unsigned char)0x00)
#define SOCKS_AUTH_GSSAPI       ((unsigned char)0x01)
#define SOCKS_AUTH_PASSWD       ((unsigned char)0x02)
#define SOCKS_AUTH_CHAP         ((unsigned char)0x03)
#define SOCKS_AUTH_UNKNOWN      ((unsigned char)0xFF)

// SOCK5 (SOCKS5 ONLY)
#define SOCKS_ADDR_IPV4ADDR     ((unsigned char)0x01)
#define SOCKS_ADDR_HOSTNAME     ((unsigned char)0x03)
#define SOCKS_ADDR_IPV6ADDR     ((unsigned char)0x04)
#define SOCKS_ADDR_UNKNOWN      ((unsigned int)0x10000000)

// SOCKS Request Packet Type
#define SOCKS_PACK_REQUEST      ((int)0x00)
#define SOCKS_PACK_4REQ         ((int)0x01)
#define SOCKS_PACK_4AREQ        ((int)0x02)
#define SOCKS_PACK_5REQ         ((int)0x03)
#define SOCKS_PACK_5AUTHREQ     ((int)0x04)
#define SOCKS_PACK_5PASSREQ     ((int)0x05)

// SOCKS Reply Packet Type
#define SOCKS_PACK_REPLY        ((int)0x20)
#define SOCKS_PACK_4REP         (SOCKS_PACK_REPLY + SOCKS_PACK_4REQ)
#define SOCKS_PACK_4AREP        (SOCKS_PACK_REPLY + SOCKS_PACK_4AREQ)
#define SOCKS_PACK_5REP         (SOCKS_PACK_REPLY + SOCKS_PACK_5REQ)
#define SOCKS_PACK_5AUTHREP     (SOCKS_PACK_REPLY + SOCKS_PACK_5AUTHREQ)
#define SOCKS_PACK_5PASSREP     (SOCKS_PACK_REPLY + SOCKS_PACK_5PASSREQ)

#define SOCKS_PACK_5UDP			((int)0x41)

#define SOCKS_PACK_PENDING      (0)
#define SOCKS_PACK_MINVALUE     SOCKS_PACK_4REQ
#define SOCKS_PACK_MAXVALUE     SOCKS_PACK_5UDP



#pragma pack(push, 1)

/**
		 SOCK4 Proxy request (struct mode)
			Sock4 Request Packet
        +----+----+----+----+----+----+----+----+----+----+....+----+
        | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|  
        +----+----+----+----+----+----+----+----+----+----+....+----+
    	   1    1      2              4           variable       1       
 */
typedef struct __SOCK4_REQUEST
{
	UINT8   vers;     // 版本号 0x04
	UINT8   cmds;     // 命令
	UINT16  port;     // 目标端口
	uint32_t  addr;     // 目标IP地址
	const char *user;	// 用户ID
	UINT8     null;
	
} SOCK4_REQUEST, *PSOCK4_REQUEST;

/**
			SOCK4 Proxy reply packet (binary mode)
        +----+----+----+----+----+----+----+----+
        | VN | CD | DSTPORT |      DSTIP        |
        +----+----+----+----+----+----+----+----+
          1    1      2              4
*/

typedef struct __SOCK4_REPLY
{
	UINT8   vers;
	UINT8   status;   // SOCKS_REP4_RESULT, 0x90-0x93
	UINT16  port;
	uint32_t  addr;     // 反馈地址
} SOCK4_REPLY, SOCK4A_REPLY, *PSOCK4_REPLY, *PSOCK4A_REPLY;

/**
 * SOCK4A Proxy request with proxy-endpoint address retrieve
 */
typedef struct __SOCK4A_REQUEST
{
	UINT8  vers;          // 版本号 0x04
	UINT8  cmds;          // 命令
	UINT16 port;          // 目标端口
	uint32_t addr;          // 目标机器IP地址 0x000000XX
	
	const char *suser;      // 用户ID
	UINT8     null1;      // 分隔用
	const char *shost;      // 目标机器域名
	UINT8     null2;      // 结束0x00
	
} SOCK4A_REQUEST, *PSOCK4A_REQUEST;

/**
 * SOCK5 Proxy request with Auth-Request
 */
typedef struct __SOCK5_AUTHREQUEST
{
	UINT8  vers;
	UINT8  lauth;
	const char *sauth;
} SOCK5_AUTHREQUEST, *PSOCK5_AUTHREQUEST;

/**
 * SOCK5 Proxy reply with Auth-Reply
 */
typedef struct __SOCK5_AUTHREPLY
{
	UINT8  vers;
	UINT8  auths;
} SOCK5_AUTHREPLY, *PSOCK5_AUTHREPLY;

/**
 * SOCK5 Proxy request with Username/Password Auth-Request
 */
typedef struct __SOCK5_PASSREQUEST
{
	UINT8  vers;
	UINT8  luser;
	const char *suser;
	UINT8  lpass;
	const char *spass;
} SOCK5_PASSREQUEST, *PSOCK5_PASSREQUEST;

/**
 * SOCK5 Proxy request with Username/Password Auth-Reply
 */
typedef struct __SOCK5_PASSREPLY
{
	UINT8   vers;
	UINT8   status;
} SOCK5_PASSREPLY, *PSOCK5_PASSREPLY;

/**
 * SOCK5 Proxy request with special command request
 */
typedef struct __SOCK5_REQUEST
{
	UINT8   vers;
	UINT8   cmds;
	UINT8   rsv;      /* reserved, fill null */
	UINT8   type;     /* address type, like SOCKS_ADDR_ */
	
	const char *addr;   /* request address */
	UINT16  port;     /* request service port */
} SOCK5_REQUEST, *PSOCK5_REQUEST;

/**
 * SOCK5 Proxy reply with special command reply
 */
typedef struct __SOCK5_REPLY
{
	UINT8   vers;
	UINT8   status;
	UINT8   rsv;      /* reserved, fill null */
	UINT8   type;
	
	char     *addr;     /* binding address */
	UINT16  port;     /* binding service port */
} SOCK5_REPLY, *PSOCK5_REPLY;

/**
 * SOCK5 Proxy request/reply with UDP command request/reply (Socks5 ONLY)
 */
typedef struct __SOCK5_UDP
{
	UINT16  rsv;      /* reserved, fill null */
	UINT8   frag;
	UINT8   type;
	char     *addr;
	UINT16  port;
	char     *pszData;
} SOCK5_UDP, *PSOCK5_UDP;

/**
 * SOCKS Proxy request common union data 
 *  Request-Packet (TCP ONLY)
 */
typedef struct __SOCKS_REQUEST
{
	int   type;
	
	union {
		SOCK4_REQUEST       sock4;
		SOCK4A_REQUEST      sock4a;

		SOCK5_REQUEST       sock5;
		SOCK5_AUTHREQUEST   sock5auth;
		SOCK5_PASSREQUEST   sock5pass;
	};
} SOCKS_REQUEST, *PSOCKS_REQUEST;

/**
 * SOCKS Proxy response common union data 
 *  Reply-Packet (TCP ONLY)
 */
typedef struct __SOCKS_REPLY
{
	int  type;
	
	union {
		SOCK4_REPLY         sock4;
		SOCK4A_REPLY        sock4a;
		
		SOCK5_UDP           socks5udp;
		
		SOCK5_REPLY         sock5;
		SOCK5_AUTHREPLY     sock5auth;
		SOCK5_PASSREPLY     sock5pass;
	};
} SOCKS_REPLY, *PSOCKS_REPLY;

#pragma pack(pop)


class TSockProxy
{
private:
	TSocket        *m_pSocket;
	int             m_tmoRecv;
	int             m_tmoSend;
	SOCKS_REQUEST   m_stReqPacket;
	SOCKS_REPLY    	m_stRepPacket;
private:
	int   PackGet( int nSize, char *pPackBufs, int nPackType, PSOCKS_REQUEST pPacket);
	int   PackSize(int nPackType, PSOCKS_REQUEST pPacket);
public:
	TSockProxy(TSocket* pSocket);
	virtual ~TSockProxy();
	
	int   Send4Req(UINT8 cmds, UINT16 port, 
								uint32_t destin, const char *user = NULL);
								
	int   Send4aReq(UINT8 cmds, UINT16 port, 
								const char *host, const char *user = NULL);
	
	int   Send5Req(UINT8 cmds, UINT8 type, UINT16 port, const char *destin);
	int   Send5AuthReq( UINT8 lauth, const char *sauth);
	int   Send5PassReq( const char *user, const char *pass);
	
	int   RecvOK( int nPackType, PSOCKS_REPLY pPacket);
	int   RecvPacket( int nPackType, PSOCKS_REPLY pPacket);
	
	int   RecvOK();
	int   RecvPacket( int nPackType);
	
	SOCKS_REPLY* RecvBufs();
	
	int   SendPacket( int nPackType, PSOCKS_REQUEST pPacket);
	
	int   RecvDestroy(PSOCKS_REPLY pPacket);
	int   SendDestroy(PSOCKS_REQUEST pPacket);
	
};


#endif


