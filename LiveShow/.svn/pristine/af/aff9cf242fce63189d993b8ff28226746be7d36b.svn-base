#include <vector>
#include <string>
#include <string.h>

#include "..\include\SockProxy.h"

TSockProxy :: TSockProxy(TSocket *pSocket)
{
	m_pSocket = pSocket;
	m_stRepPacket.type = SOCKS_PACK_PENDING;
}

TSockProxy :: ~TSockProxy()
{
	if (m_stRepPacket.type != SOCKS_PACK_PENDING)
		RecvDestroy(&m_stRepPacket);
}

int TSockProxy :: PackGet(int nSize, char *pPackBufs, 
								int nPacketType, PSOCKS_REQUEST pPacket)
{
	int ret = -EINVAL;
	
	int n = 0;
	int nLen = 0;
	int nIndex = 0;
	
	UINT16  wData;
	uint32_t  dwData;
	
#define DATA_APPEND(vch)        \
	do {	\
		if (nIndex >= nSize)	\
		{	\
			ret = -ENOBUFS;		\
			goto errout;	\
		}	\
		if (pPackBufs != NULL)	\
			((UINT8 *)pPackBufs)[nIndex] = (UINT8)(vch);	\
		nIndex++;	\
	} while (0)


	switch (nPacketType)
	{
	case SOCKS_PACK_4REQ:
		DATA_APPEND(pPacket->sock4.vers);
		DATA_APPEND(pPacket->sock4.cmds);
		// 端口
		wData = htons(pPacket->sock4.port);
		DATA_APPEND(wData & 0x00ff);
		DATA_APPEND((wData >> 8) & 0x00ff);
		
		// IP地址(不需要再进行htonl转换,由inet_addr出来的IP地址已经是网络字节序)
		dwData = pPacket->sock4.addr;
		DATA_APPEND((dwData ) & 0x000000ff);
		DATA_APPEND((dwData >> 8 ) &0x000000ff);
		DATA_APPEND((dwData >> 16) &0x000000ff);
		DATA_APPEND((dwData >> 24) &0x000000ff);
		// UserID
		nLen = (int)strlen(pPacket->sock4.user);
		for (n = 0; n < nLen; n++)
			DATA_APPEND(pPacket->sock4.user[n]);
		// NULL UINT8
		DATA_APPEND(pPacket->sock4.null);
		pPackBufs[nIndex] = '\0';
		break;
	case SOCKS_PACK_4AREQ:
		DATA_APPEND(pPacket->sock4a.vers);
		DATA_APPEND(pPacket->sock4a.cmds);
		// 端口
		wData = htons(pPacket->sock4a.port);
		DATA_APPEND(wData & 0x00ff);
		DATA_APPEND((wData >> 8) & 0x00ff);
		
		// IP地址(不需要再进行htonl转换,由inet_addr出来的IP地址已经是网络字节序)
		dwData = pPacket->sock4a.addr;
		DATA_APPEND((dwData ) &0x000000ff);
		DATA_APPEND((dwData >> 8 ) &0x000000ff);
		DATA_APPEND((dwData >> 16) &0x000000ff);
		DATA_APPEND((dwData >> 24) &0x000000ff);
		// UserID
		nLen = (int)strlen(pPacket->sock4a.suser);
		for (n = 0; n < nLen; n++)
			DATA_APPEND(pPacket->sock4a.suser[n]);
		// NULL1 UINT8
		DATA_APPEND(pPacket->sock4a.null1);
		// DestHostName
		nLen = (int)strlen(pPacket->sock4a.shost);
		for (n = 0; n < nLen; n++)
			DATA_APPEND(pPacket->sock4a.shost[n]);
		// NULL2 UINT8
		DATA_APPEND(pPacket->sock4a.null2);
		pPackBufs[nIndex]		= '\0';
		break;
	case SOCKS_PACK_5REQ:
		DATA_APPEND(pPacket->sock5.vers);
		DATA_APPEND(pPacket->sock5.cmds);
		DATA_APPEND(pPacket->sock5.rsv);
		DATA_APPEND(pPacket->sock5.type);
		
		switch (pPacket->sock5.type)
		{
		case SOCKS_ADDR_IPV4ADDR:
			/**
				如果目标地址IP = "123.222.111.222"
				则传进来的参数sock5.pPackBufsAddr的形式应该是这样的:
				uint32_t dwIP = inet_addr("123.222.111.222");
				char*  szAddr = new char[5];
				szAddr[0] = (char)( dwIP      & 0x000000ff);
				szAddr[1] = (char)((dwIP>>8 ) & 0x000000ff);
				szAddr[2] = (char)((dwIP>>16) & 0x000000ff);
				szAddr[3] = (char)((dwIp>>24) & 0x000000ff);
				szAddr[4] = '\0';
				sock5.pPackBufsAddr = szAddr;
			*/
			nLen = 4;
			break;
		case SOCKS_ADDR_IPV6ADDR:
			nLen = 16;
			break;
		case SOCKS_ADDR_HOSTNAME:
			/**
			 * 
				如果目标地址DM = "www.yahoo.com"
				则传进来的参数sock5.pPackBufsAddr的形式应该是这样的:
				int nLen = strlen("www.yahoo.com");
				char* szAddr = new char[nLen+2];
				szAddr[0] = (char)nLen;			// 第一个字节保存域名的长度
				for( int i =0;i<nLen;i++)		// 从第二个字节开始才是真正的域名
					szAddr[i+1] = DM[i];
				szAddr[nLen+1] = '\0';

				sock5.pPackBufsAddr = szAddr;	
			*/
			nLen =  1;
			nLen += (UINT8) (pPacket->sock5.addr[0]);
			break;
		default:
			nLen = (int)strlen(pPacket->sock5.addr);
			break;
		}
		for (n = 0; n < nLen; n++)
			DATA_APPEND(pPacket->sock5.addr[n]);
		wData = htons(pPacket->sock5.port);
		DATA_APPEND(wData & 0x00ff);
		DATA_APPEND((wData >> 8) & 0x00ff);
		pPackBufs[nIndex] = '\0';
		break;
	case SOCKS_PACK_5AUTHREQ:
		DATA_APPEND(pPacket->sock5auth.vers);
		DATA_APPEND(pPacket->sock5auth.lauth);
		nLen = pPacket->sock5auth.lauth;
		for (n = 0; n < nLen; n++)
			DATA_APPEND(pPacket->sock5auth.sauth[n]);
		pPackBufs[nIndex] = '\0';
		break;
	case SOCKS_PACK_5PASSREQ:
		DATA_APPEND(pPacket->sock5pass.vers);
		DATA_APPEND(pPacket->sock5pass.luser);
		nLen = pPacket->sock5pass.luser;
		for (n = 0; n < nLen; n++)
			DATA_APPEND(pPacket->sock5pass.suser[n]);
		DATA_APPEND(pPacket->sock5pass.lpass);
		nLen = pPacket->sock5pass.lpass;
		for( n = 0; n < nLen; n++ )
			DATA_APPEND(pPacket->sock5pass.spass[n]);
		pPackBufs[nIndex] = '\0';
		break;
	default:
		NSR_PROCESS_ERROUT(1, -EINVAL);
		break;
	}
	NSR_PROCESS_ERROUT(nIndex >= nSize, -ENOSPC);
	
	//组装请求数据包成功
	ret = nIndex;
	
errout:
	return ret;
}

/**
 * 计算请求数据包的尺寸
 */
int TSockProxy :: PackSize( int nPacketType, PSOCKS_REQUEST pPacket)
{
	int ret = -EINVAL;
	int nLen = 0;
	
	switch (nPacketType)
	{
	case SOCKS_PACK_4REQ:
		nLen = 9 + (int)strlen(pPacket->sock4.user);
		break;
	case SOCKS_PACK_4AREQ:
		nLen = 10 + (int)strlen(pPacket->sock4a.suser) + (int)strlen(pPacket->sock4a.shost);
		break;
	case SOCKS_PACK_5REQ:
		nLen = 6;
		switch (pPacket->sock5.type)
		{
		case SOCKS_ADDR_IPV4ADDR:
			nLen += 4;
			break;
		case SOCKS_ADDR_IPV6ADDR:
			nLen += 16;
			break;
		case SOCKS_ADDR_HOSTNAME:
			nLen += ((UINT8)pPacket->sock5.addr[0]);
			nLen += 2;
			break;
		default:
			nLen +=(int) strlen(pPacket->sock5.addr);
			nLen += 2;
			break;
		}
		break;
	case SOCKS_PACK_5AUTHREQ:
		nLen = 4 + (UINT8)pPacket->sock5auth.lauth;
		break;
	case SOCKS_PACK_5PASSREQ:
		nLen = 4 + pPacket->sock5pass.luser + pPacket->sock5pass.lpass;
		break;
	default:
		NSR_PROCESS_ERROUT(1, -EINVAL);
		break;
	}
	nLen += 4;
	
	ret = nLen;
	
errout:
	return ret;
}


int TSockProxy :: SendPacket(int nPackType, PSOCKS_REQUEST pPacket)
{
	int ret = -EINVAL;
	int nSize = 0;
	char *pSendBufs = NULL;
	
	if (nPackType >= SOCKS_PACK_MINVALUE)
		pPacket->type = nPackType;
	else
		nPackType = pPacket->type;
	
	ret = PackSize(nPackType, pPacket);
	if (ret <= 0)
	{
		if (ret >= 0)
			ret = -EINVAL;
		goto errout;
	}
	nSize = ret;
	
	pSendBufs = (char *)malloc(nSize + 4);
	if (NULL == pSendBufs)
	{
		ret = -ENOMEM;
		goto errout;
	}
	
	ret = PackGet(nSize, pSendBufs, nPackType, pPacket);
	if (ret <= 0)
	{
		if (ret >= 0)
			ret = -ENODATA;
		goto errout;
	}
	nSize = ret;
	
	//ret = m_pSocket->Send(pSendBufs, nSize);
	ret = m_pSocket->Send(pSendBufs, nSize, 0, 100);
	if (ret != nSize)
	{
		if (ret >= 0)
			ret = -ENOSPC;
		goto errout;
	}
	
errout:
	if (pSendBufs != NULL)
	{
		free(pSendBufs);
		pSendBufs = NULL;
	}
	
	return (ret);
}

int TSockProxy :: Send4Req(UINT8 cmds, UINT16 port,uint32_t destin,const char *user)
{
	SOCKS_REQUEST pack;
	
	pack.sock4.vers = SOCKS_VERS_4;
	pack.sock4.cmds = cmds;
	pack.sock4.port = port;
	pack.sock4.addr = destin;
	pack.sock4.user = user;
	pack.sock4.null = SOCKS_BYTE_NULL;
	
	return SendPacket(SOCKS_PACK_4REQ, &pack);
}

int TSockProxy :: Send4aReq(UINT8 cmds, UINT16 port, 
									const char *host, const char *user)
{
	SOCKS_REQUEST pack;
	
	pack.sock4a.vers = SOCKS_VERS_4A;
	pack.sock4a.cmds = cmds;
	pack.sock4a.port = port;
	pack.sock4a.addr = SOCKS_ADDR_UNKNOWN;
	pack.sock4a.suser = user;
	pack.sock4a.null1 = SOCKS_BYTE_NULL;
	pack.sock4a.shost = host;
	pack.sock4a.null2 = SOCKS_BYTE_NULL;
	
	return SendPacket(SOCKS_PACK_4AREQ, &pack);
}

/**
 * 发送 SOCKS5 请求 
 * 
 * @param type: 地址类型
 *  SOCKS_ADDR_IPV4ADDR	pszDestAddr = IPV4_Address (4 Octects)
 *  SOCKS_ADDR_IPV6ADDR	pszDestAddr = IPV6_Address (16 Octects) 	
 *  SOCKS_ADDR_HOSTNAME	pszDestAddr = [1BYTE:(HostName)Length]+[HostName]	
 */
int TSockProxy :: Send5Req(UINT8 cmds, UINT8 type,UINT16 port, const char *destin)
{
	SOCKS_REQUEST pack;
	
	pack.sock5.vers = SOCKS_VERS_5;
	pack.sock5.cmds = cmds;
	pack.sock5.rsv  = SOCKS_BYTE_NULL;
	pack.sock5.type = type;
	
	pack.sock5.port	= port;
	pack.sock5.addr	= destin;
	
	return SendPacket(SOCKS_PACK_5REQ, &pack);
}

/**
 * 发送SOCKS5认证请求 
 * 
 * @param lauth
 * @param sauth
 */
int TSockProxy :: Send5AuthReq(UINT8 lauth, const char *sauth)
{
	SOCKS_REQUEST pack;
	
	pack.sock5auth.vers  = SOCKS_VERS_5;
	pack.sock5auth.lauth = lauth;
	pack.sock5auth.sauth = sauth;
	
	return SendPacket(SOCKS_PACK_5AUTHREQ, &pack);
}

int  TSockProxy :: Send5PassReq(const char *user, const char *pass)
{
	int ret = 0;
	SOCKS_REQUEST pack;
	
	pack.sock5pass.vers  = 0x01;
	
	ret = ((NULL == user) ? 0 : (int)strlen(user));
	pack.sock5pass.luser = (UINT8)ret;
	ret = ((NULL == pass) ? 0 : (int)strlen(pass));
	pack.sock5pass.lpass = (UINT8)ret;
	
	pack.sock5pass.suser = user;
	pack.sock5pass.spass = pass;
	
	return SendPacket(SOCKS_PACK_5PASSREQ, &pack);
}

int TSockProxy :: RecvOK(int nPackType, PSOCKS_REPLY pPacket)
{
	int ret = -EPROTO;
	
	// 取出类型
	if (nPackType < SOCKS_PACK_MINVALUE)
		nPackType = pPacket->type;
	
	switch (nPackType)
	{
	case SOCKS_PACK_4REP:
		ret = (SOCKS_REP4_SUCCESS == pPacket->sock4.status);
		break;
	case SOCKS_PACK_4AREP:
		ret = (SOCKS_REP4_SUCCESS == pPacket->sock4a.status);
		break;
	case SOCKS_PACK_5REP:
		ret = (SOCKS_REP5_SUCCESS == pPacket->sock5.status);
		break;
	case SOCKS_PACK_5AUTHREP:
		ret = ((SOCKS_AUTH_NONE == pPacket->sock5auth.auths) || 
			 (SOCKS_AUTH_GSSAPI == pPacket->sock5auth.auths) || 
			 (SOCKS_AUTH_PASSWD == pPacket->sock5auth.auths) || 
			 (SOCKS_AUTH_CHAP == pPacket->sock5auth.auths)) ;
		break;
	case SOCKS_PACK_5PASSREP:
		ret = (SOCKS_AUTH_OK == pPacket->sock5pass.status);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	
//	if (ret > 0)			---Correct Bug.		
//	if (ret < 0)
//		ret = -EPERM;
//	else if (0 == ret)
//		ret = -EPROTO;
//	return ret;
	return ret;
}

int TSockProxy :: RecvPacket(int nPackType, PSOCKS_REPLY pPacket)
{
	int    ret = -EINVAL;
	int    nCount = 0;
	
	char  *pData = NULL;
	char   pPackBufs[SOCKS_MAXS_RESPONSE];
	
	UINT16 wData = 0;
	uint32_t dwData	= 0;
	
	if (pPacket->type != SOCKS_PACK_PENDING)
		RecvDestroy(pPacket);
	
	// 保存类型
	if (nPackType >= SOCKS_PACK_MINVALUE)
		pPacket->type = nPackType;
	else
		nPackType = pPacket->type;
	
	//接收数据
	switch (nPackType)
	{
	case SOCKS_PACK_4REP:
		ret = m_pSocket->Recv(pPackBufs, 2, 0, 100);	
		NSR_PROCESS_SOCKSLEN(ret, 2);
		
		pPacket->sock4.vers = (UINT8)pPackBufs[0];
		pPacket->sock4.status = (UINT8)pPackBufs[1];
		
		ret = m_pSocket->Recv(pPackBufs, sizeof(UINT16), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(UINT16));
		
		wData = ((UINT8)pPackBufs[0] << 8) | ((UINT8)pPackBufs[1]);
		pPacket->sock4.port = ntohs(wData);
		
		ret = m_pSocket->Recv(pPackBufs, sizeof(uint32_t), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(uint32_t));
		
		dwData = ((UINT8)pPackBufs[0] << 24) | ((UINT8)pPackBufs[1] << 16) | 
			 ((UINT8)pPackBufs[2] << 8) | ((UINT8)pPackBufs[3]);
		pPacket->sock4.addr = ntohl(dwData);
		break;
		
	case SOCKS_PACK_4AREP:
		ret = m_pSocket->Recv(pPackBufs, 2, 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, 2);
		pPacket->sock4a.vers = (UINT8)pPackBufs[0];
		pPacket->sock4a.status = (UINT8)pPackBufs[1];
		
		ret = m_pSocket->Recv(pPackBufs, sizeof(UINT16), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(UINT16));
		wData = ((UINT8)pPackBufs[0] << 8) | ((UINT8)pPackBufs[1]);
		pPacket->sock4a.port = ntohs(wData);
		
		ret = m_pSocket->Recv(pPackBufs, sizeof(uint32_t), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(uint32_t));
		dwData = ((UINT8)pPackBufs[0] << 24) | ((UINT8)pPackBufs[1] << 16) | 
			 ((UINT8)pPackBufs[2] << 8) | ((UINT8)pPackBufs[3]);
		pPacket->sock4a.addr = ntohl(dwData);
		break;
	case SOCKS_PACK_5REP:
		ret = m_pSocket->Recv(pPackBufs, 4, 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, 4);
		pPacket->sock5.vers = (UINT8)pPackBufs[0];
		pPacket->sock5.status = (UINT8)pPackBufs[1];
		pPacket->sock5.rsv = (UINT8)pPackBufs[2];
		pPacket->sock5.type = (UINT8)pPackBufs[3];
		if (pPacket->sock5.status != SOCKS_REP5_SUCCESS)
		{
			pPacket->sock5.addr = NULL;
			pPacket->sock5.port = SOCKS_WORD_NULL;
			break;
		}
		
		switch (pPacket->sock5.type)
		{
		case SOCKS_ADDR_IPV4ADDR:
			pData = (char *)malloc(sizeof(uint32_t) + 2);
			NSR_PROCESS_ERROUT(NULL == pData, -ENOMEM);
			ret = m_pSocket->Recv(pData, sizeof(uint32_t), 0, 100);
			NSR_PROCESS_SOCKSLEN(ret, sizeof(uint32_t));
			pData[4] = '\0';
			pPacket->sock5.addr = pData;
			pData = NULL;
			break;
		case SOCKS_ADDR_IPV6ADDR:
			pData = (char *)malloc(16 + 2);
			NSR_PROCESS_ERROUT(NULL == pData, -ENOMEM);
			ret = m_pSocket->Recv(pData, 16, 0, 100);
			NSR_PROCESS_SOCKSLEN(ret, 16);
			pData[16] = '\0';
			pPacket->sock5.addr = pData;
			pData = NULL;
			break;
		case SOCKS_ADDR_HOSTNAME:
			ret = m_pSocket->Recv(pPackBufs, sizeof(UINT8), 0, 100);
			NSR_PROCESS_SOCKSLEN(ret, sizeof(UINT8));
			nCount = (UINT8)pPackBufs[0];
			pData = (char *)malloc(nCount + 2);
			NSR_PROCESS_ERROUT(NULL == pData, -ENOMEM);
			ret = m_pSocket->Recv(pData, nCount, 0, 100);
			NSR_PROCESS_SOCKSLEN(ret, nCount);
			pData[nCount] = '\0';
			pPacket->sock5.addr = pData;
			pData = NULL;
			break;
		default:
			pPacket->sock5.addr = NULL;
			break;
		}
		
		ret = m_pSocket->Recv(pPackBufs, sizeof(UINT16), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(UINT16));
		wData = ((UINT8)pPackBufs[0] << 8) | ((UINT8)pPackBufs[1]);
		pPacket->sock5.port = ntohs(wData);
		break;
	case SOCKS_PACK_5AUTHREP:
		ret = m_pSocket->Recv(pPackBufs, sizeof(UINT16), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(UINT16));
		pPacket->sock5auth.vers = (UINT8)pPackBufs[0];
		pPacket->sock5auth.auths = (UINT8)pPackBufs[1];
		break;
	case SOCKS_PACK_5PASSREP:
		ret = m_pSocket->Recv(pPackBufs, sizeof(UINT16), 0, 100);
		NSR_PROCESS_SOCKSLEN(ret, sizeof(UINT16));
		pPacket->sock5pass.vers = (UINT8)pPackBufs[0];
		pPacket->sock5pass.status = (UINT8)pPackBufs[1];
		break;
	default:
		NSR_PROCESS_ERROUT(1, -EINVAL);
		break;
	}
	
	ret = EPERM;
	
errout:
	if (pData != NULL)
	{
		free(pData);
		pData = NULL;
	}
	
	return ret;
}		

int TSockProxy :: RecvOK()
{
	return RecvOK(SOCKS_PACK_PENDING, &m_stRepPacket);
}

int TSockProxy :: RecvPacket(int nPacketType = SOCKS_PACK_PENDING)
{
	return RecvPacket(nPacketType, &m_stRepPacket);
}

	
SOCKS_REPLY* TSockProxy :: RecvBufs()
{
	return &m_stRepPacket;
}

int TSockProxy :: RecvDestroy(PSOCKS_REPLY pPacket)
{
	int ret = -EINVAL;
	
	switch (pPacket->type)
	{
	case SOCKS_PACK_4REP:
	case SOCKS_PACK_4AREP:
		break;
	case SOCKS_PACK_5REP:
		if (pPacket->sock5.addr != NULL)
		{
			free(pPacket->sock5.addr);
			pPacket->sock5.addr = NULL;
		}
		break;
	case SOCKS_PACK_5AUTHREP:
	case SOCKS_PACK_5PASSREP:
		break;
	case SOCKS_PACK_5UDP:
		if (pPacket->socks5udp.addr != NULL)
		{
			free(pPacket->socks5udp.addr);
			pPacket->socks5udp.addr = NULL;
		}
		break;
	default:
		break;
	}
	
	memset(pPacket, 0, sizeof(PSOCKS_REPLY));
	pPacket->type = SOCKS_PACK_PENDING;
	
	ret = 0;
	return ret;
}

int TSockProxy :: SendDestroy(PSOCKS_REQUEST pPacket)
{
	int ret = -EINVAL;
	
	switch (pPacket->type)
	{
	case SOCKS_PACK_4REQ:
	case SOCKS_PACK_4AREQ:
	case SOCKS_PACK_5REQ:
	case SOCKS_PACK_5AUTHREQ:
	case SOCKS_PACK_5PASSREQ:
		break;
	default:
		break;
	}
	
	memset(pPacket, 0, sizeof(PSOCKS_REQUEST));
	pPacket->type = SOCKS_PACK_PENDING;
	
	ret = 0;
	return ret;
}


