/******************************************************************************
 * HttpCli.cpp - Http 协议实现类
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     该文件实现了HTTP协议组包，解包，调用网络层对象收发等功能
 * modification history
 * --------------------
 * 01a, 19nov2008, Davidfan written
 * --------------------
 ******************************************************************************/

#ifdef WIN32
#include <time.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <linux/ip.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <assert.h>
#include <iostream>
#include <limits>
#include <list>
#include "..\..\..\include\utility\Base64Coding.h"
#include "..\..\..\include\utility\StringHelper.h"

#include "..\include\HttpCli.h"
#ifndef	 HTTPTRANSFER_RELEASE
#include "..\include\log.h"
#else
#include "..\include\log_stun.h"
#endif

//#include "Config.h"

using namespace std;

/**************************************
 * THttpHdr
 **************************************/

int checkBufLen(char*& buf, ULONG64 needSize, ULONG64& currentSize)
{
	if(needSize > currentSize)
	{	//已分配的缓冲区过小
		buf =	(char*)realloc(buf, (size_t)currentSize*2);
		if(NULL == buf)
		{
			return RET_NO_MEMORY;
		}
		currentSize = currentSize*2;	//记录新的缓冲区空间
	}	
	return RET_OK;
}

THttpHdr::THttpHdr( const string& strKey ) :
    m_strKey(strKey)
{
    // Empty
}

THttpHdr::THttpHdr( const string& strKey, const string& strVal ) :
    m_strKey(strKey),
    m_strVal(strVal)
{
    // Empty
}

const string& THttpHdr::GetKey( void ) const
{
    return m_strKey;
}

const string& THttpHdr::GetVal( void ) const
{
    return m_strVal;
}

void THttpHdr::SetVal( const string& strVal )
{
    m_strVal = strVal;
}

/**************************************
 *
 * HttpHdrs
 *
 **************************************/

//无条件写入http头，即使和已有http头重复也继续写入
void HttpHdrs::SetHdr( const string& strKey, const string& strVal )
{
    m_listHdrs.push_back(  new THttpHdr( strKey, strVal ));
}

//根据key获取http头的值，如果有多个该key的值，则返回第一个
string	 HttpHdrs::GetHdr( const string& strKey)
{
    string strVal;

    THttpHdrList::iterator itr =  m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
            strVal = pHdr->GetVal();
            break;
        }
    }
    return strVal;
}

//获取相同key的http头的第index个元素，如cookie, 当index=0 时同GetHdr( const string& strKey)
string	 HttpHdrs::GetHdr( const string& strKey,	int index)
{
    string strVal;
	int i = 1;
    THttpHdrList::iterator itr =  m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
        	if(i == index)
        	{
	            strVal = pHdr->GetVal();
	            break;
        	}
			i++;
        }
    }
    return strVal;
}

//根据key删除http头，如果有多个该key的值，则删除第一个值
void   HttpHdrs::DeleteHdr( const string& strKey )
{
    string strVal;
    THttpHdrList::iterator itr =  m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
			m_listHdrs.erase(itr);
			break;
        }
    }
}



/**************************************
 *
 * THttpMsg
 *
 **************************************/

THttpMsg::THttpMsg( void ) : m_nHttpVer(0)
{
    // Empty
}

void THttpMsg :: ShowAllHttpHdr(void)
{
	THttpHdr* pHdr;
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        pHdr = *itr;
        LOG_DEBUG("%s: %s", pHdr->GetKey().c_str(), pHdr->GetVal().c_str());
    }
}

THttpMsg::~THttpMsg( void )
{
	while(!m_listHdrs.empty())
    {
    	THttpHdr* pHdr = m_listHdrs.front();
        delete pHdr;
        m_listHdrs.pop_front();
    }
}

void THttpMsg::GetHttpVer( UINT* puMajor, UINT* puMinor ) const
{
    assert( puMajor && puMinor );

    *puMajor = HIWORD( m_nHttpVer );
    *puMinor = LOWORD( m_nHttpVer );
}

void THttpMsg::SetHttpVer( UINT uMajor, UINT uMinor )
{
    assert( uMajor < 10 && uMinor < 10 );

    m_nHttpVer = MAKEDWORD( uMajor,uMinor );
}

size_t THttpMsg::GetHdrCount( void ) const
{
    return m_listHdrs.size();
}

size_t THttpMsg :: GetAllHdrLen( void )
{
    size_t nLen = 0;
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
		//报文为    key: value\r\n   所以按照下面方法计算大小
        nLen += ( pHdr->GetKey().length() + 2 + pHdr->GetVal().length() + 2 );
    }
    return nLen;
}

void THttpMsg :: ClearAllHdr(void)
{
	while(!m_listHdrs.empty())
    {
    	THttpHdr* pHdr = m_listHdrs.front();
        delete pHdr;
        m_listHdrs.pop_front();
    }
}

size_t THttpMsg :: GetHdrLen( UINT nIndex )
{	
	size_t Len;
	THttpHdr * pHdr = GetHdr(nIndex);
	if(pHdr != NULL)
	{
		Len = pHdr->GetKey().length() + 2 + pHdr->GetVal().length() + 2;
		return Len;
	}
	return 0;
} 

string THttpMsg::GetHdr( const string& strKey )
{
    string strVal;

    THttpHdrList::iterator itr =  m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
            strVal = pHdr->GetVal();
            break;
        }
    }
    return strVal;
}

THttpHdr* THttpMsg::GetHdr( UINT nIndex )
{
 	THttpHdrList::iterator itr =  m_listHdrs.begin();
    for( UINT n = 0; n < nIndex; n++ )
    {
        itr++;
    }
    return *itr;
}

void THttpMsg::SetHdr( const string& strKey, const string& strVal )
{
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
            pHdr->SetVal( strVal );
            return;
        }
    }
    m_listHdrs.push_back( new THttpHdr( strKey, strVal ) );
}

void THttpMsg::SetHdr( const THttpHdr& hdrNew )
{
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( hdrNew.GetKey() == pHdr->GetKey() )
        {
            pHdr->SetVal( hdrNew.GetVal() );
            return;
        }
    }
    m_listHdrs.push_back( new THttpHdr( hdrNew ) );
}

void THttpMsg::SetHdrDuplicate( const string& strKey, const string& strVal )
{
    m_listHdrs.push_back( new THttpHdr( strKey, strVal ) );
}

void THttpMsg::SetHdrDuplicate( const THttpHdr& hdrNew )
{
    m_listHdrs.push_back( new THttpHdr( hdrNew ) );
}



//////////////////////////class THttpCli Implement /////////////////////

void THttpCli :: init()
{
	htSkp = new TSocketCli();
	if(NULL == htSkp)
	{
		LOG_DEBUG("new TSocketCli fail.\n");
	}
	m_socks = NULL;
	htRequest = new THttpMsg;
	htResponse = new THttpMsg;
	StatusLine = new char[LINE_BUFFER];
		
	socks_proxy_status = SOCKS_PROXY_INIT;

	ntlm_proxy_status = NTLM_AUTH_DEFAULT;
	m_ntlm = NULL;

	memset(uncompleteLineBuf, 0x0, sizeof(uncompleteLineBuf));
	recvedlen = 0;
	
	getPostBoundary(postBoundary, HTTP_POSTPARAM_BROUNDARY_FIXLEN+1);
	if(strcmp(postBoundary, "") == 0)
	{
		strncpy(postBoundary, __BROUNDARY, HTTP_POSTPARAM_BROUNDARY_FIXLEN);
	}
}

THttpCli :: THttpCli(const char * sUrl)
{
	LOG_DEBUG("new THttpCli without proxy.\n");
	u = new URL();
	u->set_url(sUrl);	
	
	UseProxy = false;
	init();
}

THttpCli :: THttpCli(const char * sUrl, Proxy_t proxy)
{
	LOG_DEBUG("new THttpCli with proxy.\n");
	u = new URL();
	u->set_url(sUrl);
	
	UseProxy = true;
	m_proxy = proxy;
	
	init();
}


	
THttpCli :: ~THttpCli(void)
{
	if(htSkp != NULL) delete htSkp;
	if(u != NULL) delete u;
	if(htRequest != NULL) delete htRequest;
	if(htResponse != NULL) delete htResponse;
	
	if(StatusLine != NULL) 
	{
		delete []StatusLine;
		StatusLine = NULL;
	}

	if(m_ntlm != NULL)
	{
		delete m_ntlm;
		m_ntlm = NULL;
	}
}

int THttpCli :: Connect(void)
{
	uerr_t err;
	int ret = 0;
	
	char *ipaddr = NULL;
	
	if(htSkp->GetHandle() < 0)
	{
		err = htSkp->CreateSock(SKT_TCP);	
		if(err != NOCONERROR)
			return RET_SOCKET_CREATE_FAIL;
	}
		

	ipaddr = htSkp->Host2Str(u->get_host());
	if(ipaddr == NULL)
		return RET_SOCKET_HOST_ERROR;
		
	LOG_DEBUG("Connecting to server %s ......\n", u->get_host());

	ret = htSkp->Connect(ipaddr, u->get_port(), 100);
	
	if(ret == -1)
	{	
		int errcode = socket_errno();
		LOG_DEBUG("[Connect]recv fail errno=%d errinfo=%s\n", 
				  errcode, socket_errinfo());
		if(errcode == EWOULDBLOCK)
		{  
			//非阻塞socket connect 等待
			return RET_SOCKET_CONTINUE;
		}
		else
		{
			 //发生socket错误，迁移任务状态到失败
			 LOG_WARN("[Connect]recv fail errno=%d errinfo=%s\n", 
					   errcode, socket_errinfo());
			 return RET_SOCKET_ERROR;
		}
	}
	else if(0 == ret)
	{
		return RET_OK;
	}

	return RET_SOCKET_ERROR;
}


int THttpCli :: recvAuthResp()
{
	int ret = 0;	
	//int len = 0;
	//char *ipaddr = NULL;
	char * proxyipaddr = NULL;

	//int ch = 0;
	string sDatas;

	switch (m_proxy.type)
	{	
		case HTTPDOWN_PROXY_SOCK5:
		{				
			proxyipaddr = htSkp->Host2Str(m_proxy.host.c_str());

			switch(socks_proxy_status)
			{
				case SOCKS_PROXY_VER_SENT:
				{
					//此处收取socks 版本的响应
					ret = m_socks->RecvPacket(SOCKS_PACK_5AUTHREP);
					if(ret <= 0) 
					{
						printf("RecvPacket SOCKS_PROXY_VER_SENT fail ret=%d\n", ret);
						return RET_SOCKET_ERROR;
					}	

					ret = m_socks->RecvOK();
					if(ret <= 0) 
					{
						printf("RecvOK SOCKS_PROXY_VER_SENT fail ret=%d\n", ret);
						return RET_PROXY_RESQ_CHECK_FAIL;
					}	

					switch (m_socks->RecvBufs()->sock5auth.auths)
					{
						//解析认证方式
						case SOCKS_AUTH_NONE:
						{
							//无需认证，直接发代理请求
							ret = sendProxyRequest();

							return ret;
						}	
						
						case SOCKS_AUTH_PASSWD:
						{
							//以用户名，密码认证，发送认证信息，等待响应
							//如果存在用户名和密码
							if (m_proxy.pwd.length() > 0 
								 	&& m_proxy.user.length() > 0)
							{
								ret = m_socks->Send5PassReq( m_proxy.user.c_str(), m_proxy.pwd.c_str());
								if(ret <= 0) 
								{
									printf("Send5PassReq fail ret=%d for user=%s pwd=%s\n", 
													ret, m_proxy.user.c_str(), m_proxy.pwd.c_str());
									return RET_SOCKET_ERROR;
								}
								socks_proxy_status = SOCKS_PROXY_PASS_SENT;
								
								return RET_PROXY_DOING_AUTH;
							}
							else
							{
								printf("[recvAuthResp]Need user and pwd to auth but user=%s pwd=%s",
										m_proxy.user.c_str(), m_proxy.pwd.c_str());
								return RET_PROXY_AUTH_UNSUPPORTED;
							}
						}	
						
						case SOCKS_AUTH_GSSAPI:
						case SOCKS_AUTH_CHAP:
						case SOCKS_AUTH_UNKNOWN:
						default:
						{
							//其它认证方式，暂不支持
							printf("Unsupported socks5 auth type=%d\n", 
									m_socks->RecvBufs()->sock5auth.auths);
							return RET_PROXY_AUTH_UNSUPPORTED;
						}
					}
					break;
				}
				
				case SOCKS_PROXY_PASS_SENT:
				{
					//此处收取用户名+密码认证的响应					
					ret = m_socks->RecvPacket(SOCKS_PACK_5PASSREP);
					if(ret <= 0) 
					{
						printf("RecvPacket SOCKS_PROXY_PASS_SENT fail ret=%d\n", ret);
						return RET_SOCKET_ERROR;
					}
					
					ret = m_socks->RecvOK();
					if(ret <= 0) 
					{
						printf("RecvOK SOCKS_PROXY_PASS_SENT fail ret=%d\n", ret);
						return RET_PROXY_RESQ_CHECK_FAIL;
					}	

					//用户名密码认证通过，发代理请求
					ret = sendProxyRequest();

					return ret;
				}

				case SOCKS_PROXY_REQ_SENT:
				{
					//此处收取代理请求的响应						
					ret = m_socks->RecvPacket(SOCKS_PACK_5REP);
					if(ret <= 0) 
					{
						printf("RecvPacket SOCKS_PROXY_REQ_SENT fail ret=%d\n", ret);
						return RET_SOCKET_ERROR;
					}
					
					ret = m_socks->RecvOK();
					if(ret <= 0) 
					{
						printf("RecvOK SOCKS_PROXY_REQ_SENT fail ret=%d\n", ret);
						return RET_PROXY_RESQ_CHECK_FAIL;
					}
					
					//所有socks代理建立过程通过，返回RET_OK
					socks_proxy_status = SOCKS_PROXY_OK;
					return RET_OK;
				}
				
				default:	//不应该存在其它状态
				printf("recvAuthResq GOT unreasonable proxy status=%d\n", socks_proxy_status);
				return RET_SOCKET_ERROR;	
			}
		}

		default: 	//其它代理请求不应该到这里来
		LOG_DEBUG("recvAuthResq GOT unreasonable PROXYTYPE =%d\n", m_proxy.type);
		return RET_SOCKET_ERROR;			
		
	}	
	//return RET_SOCKET_ERROR;	
}

int THttpCli :: sendProxyRequest()
{
	int ret = 0;	
	string sDatas;
	string sAuths;
	
	uint32_t dwAddr = htSkp->Host2Ip(u->get_host());
	uint8_t bPack = 0;
	int ch = 0;
	
	sDatas = "";
	if (dwAddr != INADDR_NONE)
	{
		bPack = SOCKS_ADDR_IPV4ADDR;
		sDatas += (char)((dwAddr  ) &0x000000ff);
		sDatas += (char)((dwAddr >> 8 ) &0x000000ff);
		sDatas += (char)((dwAddr >> 16) &0x000000ff);
		sDatas += (char)((dwAddr >> 24) &0x000000ff);
	}
	else
	{
		bPack = (int)SOCKS_ADDR_HOSTNAME;
		ch = (int)strlen(u->get_host());
		sDatas += ch;
		sDatas += u->get_host();
	}
	
	ret = m_socks->Send5Req(SOCKS_CMDS_CONNECT,
						bPack, u->get_port(), sDatas.c_str());
						
	if(ret <= 0) 
	{
		printf("Send5Req fail ret=%d\n", ret);
		return RET_SOCKET_ERROR;
	}
	socks_proxy_status = SOCKS_PROXY_REQ_SENT;
	
	return RET_PROXY_DOING_AUTH;
}

int THttpCli :: sendSocksVersion()
{
	int ret = 0;	
	int len = 0;
	char *ipaddr = NULL;
	char * proxyipaddr = NULL;

	int ch = 0;
	string sDatas;
	//uint32_t dwAddr = 0;

	switch (m_proxy.type)
	{	
		case HTTPDOWN_PROXY_SOCK5:
		{
			if(m_socks != NULL)
			{
				delete m_socks;
				m_socks = NULL;
			}
			m_socks = new TSockProxy(htSkp);	
			if(m_socks == NULL)
			{
				ret = RET_NO_MEMORY;
				break;
			}
			
			proxyipaddr = htSkp->Host2Str(m_proxy.host.c_str());

			if(proxyipaddr == NULL)
			{
				ret = RET_SOCKET_HOST_ERROR;
				break;
			}
			
			printf("SEND VERSION to socks5 proxy server %s:%d...\n", 
						ipaddr, m_proxy.port);
			
			sDatas = "";
			//如果存在用户名和密码
			if (m_proxy.pwd.length() > 0 
				 	&& m_proxy.user.length() > 0)
			{
				ch = SOCKS_AUTH_NONE;
				sDatas += (char)ch;
				ch = SOCKS_AUTH_PASSWD;
				sDatas += ch;
			}
			else
			{
				ch= SOCKS_AUTH_NONE;
				sDatas += ch;
			}
				
			len = (int)sDatas.length();
			ret = m_socks->Send5AuthReq(len, sDatas.c_str());
			if(ret <= 0) 
			{
				printf("Send5AuthReq fail ret=%d\n", ret);
				ret = RET_SOCKET_ERROR;					
			}
			else
			{
				socks_proxy_status = SOCKS_PROXY_VER_SENT;
				ret = RET_OK;
			}
			break;
		}
		default:
			ret = RET_PROXYTYPE_INVALID;
			break;
	}

	return ret;

}

int THttpCli::processNtlmAuth()
{			
	//int ret = 0;
	
	char * ipaddr = htSkp->Host2Str(m_proxy.host.c_str());
	if(ipaddr == NULL)
	{
		return RET_SOCKET_HOST_ERROR;
	}	
	
	switch(ntlm_proxy_status)
	{
		case NTLM_AUTH_DEFAULT:
		{
			if(u->get_host() == NULL)
			{
				ntlm_proxy_status = NTLM_AUTH_ERR;
				LOG_WARN("processNtlmAuth http domain auth with server %s:%d domain:%s, user:%s, pass:%s. fail on Host2Str host:NULL.\n", 
						ipaddr, m_proxy.port, m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str());
				return RET_PROXY_AUTH_UNSUPPORTED;
			}
			
			m_ntlm = new NtlmProxy(ipaddr, m_proxy.port, m_proxy.domain, m_proxy.user, m_proxy.pwd,
								u->get_host(), u->get_port());
			
			if(!m_ntlm->NtlmInit())
			{
				ntlm_proxy_status = NTLM_AUTH_ERR;
				LOG_WARN("processNtlmAuth http domain auth with server %s:%d domain:%s, user:%s, pass:%s. fail on NtlmInit.\n", 
						ipaddr, m_proxy.port, m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str());
				return RET_PROXY_AUTH_UNSUPPORTED;
			}
			else
			{				
				LOG_INFO("processNtlmAuth http domain auth with server %s:%d domain:%s, user:%s, pass:%s. NtlmInit success\n", 
						ipaddr, m_proxy.port, m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str());
				ntlm_proxy_status = NTLM_AUTH_INIT;
			}
			break;
		}
		case NTLM_AUTH_INIT:
		{
			char szEncode[1024] = {0};
			int  nLength = sizeof(szEncode);

			if(!m_ntlm->NtlmStep1(szEncode, &nLength))
			{
				ntlm_proxy_status = NTLM_AUTH_ERR;
				LOG_WARN("processNtlmAuth http domain auth with server %s:%d domain:%s, user:%s, pass:%s. fail on NtlmInit.\n", 
						ipaddr, m_proxy.port, m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str());						
				return RET_PROXY_AUTH_UNSUPPORTED;
			}
			else
			{
				char buf[512] = {0};
				snprintf(buf, sizeof(buf), "Negotiate %s", szEncode);
				m_ntlm->setStep1NegotiateStr(buf);
				//BuildNtlmAuthHdr(szEncode);
				ntlm_proxy_status = NTLM_AUTH_SETP1;
			}			
			break;
		}
		case NTLM_AUTH_SETP1:
		{
			//第一次交互完成，开始第二次交互
			char szEncode[1024] = {0};
			int  nLength = sizeof(szEncode);

			if(StatusCode == HTTP_NEED_PASS && m_ntlm->NtlmStep2(htResponse->GetHdr("Proxy-Authenticate").c_str(), szEncode, &nLength))
			{
				char buf[512] = {0};
				snprintf(buf, sizeof(buf), "Negotiate %s", szEncode);
				m_ntlm->setStep2NegotiateStr(buf);
//					BuildNtlmAuthHdr(szEncode);
				ntlm_proxy_status = NTLM_AUTH_SETP2;
			}	
			else
			{
				//认证失败
				LOG_WARN("recvNtlmResp Http domain auth fail with server domain:%s, user:%s, pass:%s. fail on NtlmStep2, StatusCode:%d.\n", 
						 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), StatusCode);						
				ntlm_proxy_status = NTLM_AUTH_ERR;
				return RET_PROXY_AUTH_UNSUPPORTED;	
			}
			break;
		}
		case NTLM_AUTH_SETP2:
		{
			break;
		}
		case NTLM_AUTH_FINI:
		{
			return RET_OK;
		}
		default:
		{
			LOG_WARN("processNtlmAuth http domain auth with domain:%s, user:%s, pass:%s. fail on auth data send finish but status:%d not ok.\n", 
					m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), ntlm_proxy_status);						
			ntlm_proxy_status = NTLM_AUTH_ERR;
			return RET_PROXY_AUTH_UNSUPPORTED;
		}
	}
	return RET_OK;
}

string THttpCli :: getNtlmNegotiateStr()
{
	if(NTLM_AUTH_SETP1 == ntlm_proxy_status)
	{
		return m_ntlm->getStep1NegotiateStr();
	}
	else if(NTLM_AUTH_SETP2 == ntlm_proxy_status)
	{
		return m_ntlm->getStep2NegotiateStr();		
	}
	return "";
}

int THttpCli :: sendNtlmAuthData()
{
	int ret = RET_OK;
	ULONG64 BytesWritten = 0;
	//发送请求数据
	ret = SendData((char*)(&(m_ntlm->sending_data[0])), 
						m_ntlm->sending_data.size(), 
						0, 100, BytesWritten);
				
	if(ret == RET_SOCKET_CONTINUE)
	{
		m_ntlm->sending_data.erase(m_ntlm->sending_data.begin(), m_ntlm->sending_data.begin()+(int)BytesWritten);
		if(NTLM_AUTH_DEFAULT == ntlm_proxy_status)
		{
			LOG_INFO("sendNtlmAuthData Http domain auth with server domain:%s, user:%s, pass:%s. OK move status to:%d from:%d .\n", 
					 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), NTLM_AUTH_SETP1, ntlm_proxy_status);						
			ntlm_proxy_status = NTLM_AUTH_SETP1;
		}
		else if(NTLM_AUTH_SENT_AUTH_SETP1_DATA == ntlm_proxy_status)
		{
			LOG_INFO("sendNtlmAuthData Http domain auth with server domain:%s, user:%s, pass:%s. OK move status to:%d from:%d .\n", 
					 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), NTLM_AUTH_SETP1, ntlm_proxy_status);						
			ntlm_proxy_status = NTLM_AUTH_SETP2;			
		}
	}
	else if(ret == RET_SOCKET_FINISH)
	{
		//All auth data sent, NEXT STEP is receiving response.
		if(ntlm_proxy_status == NTLM_AUTH_SETP1)
		{
			LOG_INFO("sendNtlmAuthData Http domain auth with server domain:%s, user:%s, pass:%s. OK move status to:%d from:%d .\n", 
					 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), NTLM_AUTH_SETP1, ntlm_proxy_status);						
			ntlm_proxy_status = NTLM_AUTH_SENT_AUTH_SETP1_DATA;	
		}
		else if(ntlm_proxy_status == NTLM_AUTH_SETP2)
		{
			LOG_INFO("sendNtlmAuthData Http domain auth with server domain:%s, user:%s, pass:%s. OK move status to:%d from:%d.\n", 
					 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), NTLM_AUTH_SETP1, ntlm_proxy_status);						
			ntlm_proxy_status = NTLM_AUTH_SENT_AUTH_SETP2_DATA; 
		}
		else	
		{
			LOG_WARN("sendNtlmAuthData http domain auth with domain:%s, user:%s, pass:%s. fail on auth data send finish but status:%d not ok.\n", 
					m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), ntlm_proxy_status);						
			ntlm_proxy_status = NTLM_AUTH_ERR;
			return RET_PROXY_AUTH_UNSUPPORTED;
		}
	}
	else
	{
		LOG_WARN("sendNtlmAuthData http domain auth with server domain:%s, user:%s, pass:%s. fail on auth data send ret:%d.\n", 
				 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), ret);						
		ntlm_proxy_status = NTLM_AUTH_ERR;
		return RET_PROXY_AUTH_UNSUPPORTED;
	}	
	return RET_PROXY_DOING_AUTH;
}
int THttpCli :: recvNtlmResp()
{
	int ret = RET_OK;
	
	m_ntlm->recving_data.clear();
	ret = DirectRecvData(&m_ntlm->recving_data, 
			0, 0);
	
	if(ret == RET_SOCKET_CONTINUE)
	{
		//不定长度，只有返回RET_SOCKET_CONTINUE才是正常情况 			
		if (m_ntlm->recving_data.size() > 0) //收到数据
		{		
			m_ntlm->recving_data.push_back(0);	//插入				
			const char * p = strstr((const char *)(&m_ntlm->recving_data[0]), "\r\n\r\n");
			if(p)
			{
				//httpresponse 接收完毕
				char temp_recv_data[64000] = {0};
				memset(temp_recv_data, 0x0, sizeof(temp_recv_data));
				memcpy(temp_recv_data, 
						(const char *)(&m_ntlm->recving_data[0]),
						m_ntlm->recving_data.size() > sizeof(temp_recv_data)-1 ? sizeof(temp_recv_data)-1:m_ntlm->recving_data.size());
				
				int http_resp_code = m_ntlm->GetReplyCode(temp_recv_data); 
				if(http_resp_code == HTTP_OK || (HTTP_NEED_PASS == HTTP_OK && NTLM_AUTH_SENT_AUTH_SETP1_DATA == ntlm_proxy_status))
				{
					//本次交互交互成功					
					if(NTLM_AUTH_SENT_AUTH_SETP2_DATA == ntlm_proxy_status)
					{
						//第二次交互完成，整个认证完成
						LOG_INFO("recvNtlmResp Http domain auth with server domain:%s, user:%s, pass:%s. OK move status to:%d from:%d .\n", 
								 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), NTLM_AUTH_SETP1, ntlm_proxy_status);						
						ntlm_proxy_status = NTLM_AUTH_FINI;
						return RET_OK;
					}
					else if(NTLM_AUTH_SENT_AUTH_SETP1_DATA == ntlm_proxy_status)
					{
						//第一次交互完成，开始第二次交互
						char szEncode[1024] = {0};
						int  nLength = sizeof(szEncode);

						if(m_ntlm->NtlmStep2(temp_recv_data, szEncode, &nLength))
						{
							m_ntlm->genAuthStep2Pack(szEncode);
							//Send auth data
							ret = sendNtlmAuthData();	
						}	
						else
						{
							//认证失败
							LOG_WARN("recvNtlmResp Http domain auth fail with server domain:%s, user:%s, pass:%s. fail on NtlmStep2.\n", 
									 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str());						
							ntlm_proxy_status = NTLM_AUTH_ERR;
							return RET_PROXY_AUTH_UNSUPPORTED;	
						}
					}
				}
				else
				{
					//认证失败
					LOG_WARN("recvNtlmResp Http domain auth fail with server domain:%s, user:%s, pass:%s. fail on auth response code:%d.\n", 
							 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), http_resp_code);						
					ntlm_proxy_status = NTLM_AUTH_ERR;
					return RET_PROXY_AUTH_UNSUPPORTED;	
				}				
			}
		}
		else
		{
			ret = RET_PROXY_DOING_AUTH;
		}
	}
	else
	{
		LOG_WARN("recvNtlmResp Http domain auth fail with server domain:%s, user:%s, pass:%s. fail on auth response recv ret:%d.\n", 
				 m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str(), ret);						
		ntlm_proxy_status = NTLM_AUTH_ERR;
		return RET_PROXY_AUTH_UNSUPPORTED;		
	}
	return ret;
}

int THttpCli :: proxyAuth()
{
	int ret = 0;
	char *ipaddr = NULL;
	
	switch (m_proxy.type)
	{
		case HTTPDOWN_PROXY_HTTP:
		{
			ipaddr = htSkp->Host2Str(m_proxy.host.c_str());
			if(ipaddr == NULL)
			{
				ret = RET_SOCKET_HOST_ERROR;
				break;
			}			
			LOG_INFO("Connect to http proxy server %s:%d domain:%s, user:%s, pass:%s Success.\n", 
						ipaddr, m_proxy.port, m_proxy.domain.c_str(), m_proxy.user.c_str(), m_proxy.pwd.c_str());
			
			if(m_proxy.domain != "")
			{
#ifdef WIN32
				//Use http proxy with domain, enable new domain auth process.
				ret = processNtlmAuth();
#else
				ret = RET_OK;			
#endif
			}
			else
			{
				ret = RET_OK;
			}
			break;
		}

		case HTTPDOWN_PROXY_SOCK5:
		{
			if(socks_proxy_status != SOCKS_PROXY_INIT)
			{
				return RET_PROXY_DOING_AUTH;
			}
			
			ipaddr = htSkp->Host2Str(m_proxy.host.c_str());
			if(ipaddr == NULL)
			{
				ret = RET_SOCKET_HOST_ERROR;
				break;
			}
			
			LOG_INFO("Connect to socks5 proxy server %s:%d Success, Begin auth...\n", 
						ipaddr, m_proxy.port);
			ret = sendSocksVersion();
			LOG_INFO("sendSocksVersion socks5 proxy ret=%d\n", ret);
			if(ret != RET_OK)
			{
				ret = RET_PROXY_AUTH_UNSUPPORTED;
			}
			else
			{
				ret = RET_PROXY_BEGIN_AUTH;
			}
			break;
		}
		default:
			ret = RET_PROXYTYPE_INVALID;
			break;
	}

	return ret;
}
int THttpCli :: ConnectProxyHost()
{
	uerr_t err;
	//char *ipaddr = NULL;
	char *proxyipaddr = NULL;
	
	int ret = -EINVAL;
	//int len = 0;
	string addr;
	string host;
	
	//int ch = 0;
	//char *ip = NULL;
	
	string sDatas;
	
	LOG_DEBUG("begin do ConnectProxyHost\n");	

	if(htSkp->GetHandle() < 0)
	{
		err = htSkp->CreateSock(SKT_TCP);	
		if(err != NOCONERROR)
			return RET_SOCKET_CREATE_FAIL;
	}	
	
	switch (m_proxy.type)
	{
		case HTTPDOWN_PROXY_HTTP:
		{
			proxyipaddr = htSkp->Host2Str(m_proxy.host.c_str());
			if(proxyipaddr == NULL)
				return HOSTERR;

			LOG_DEBUG("Connecting to http proxy server %s:%d\n", proxyipaddr, m_proxy.port);
			ret = htSkp->Connect(proxyipaddr, m_proxy.port, 100);

			if(ret == -1)
			{		
				int errcode = socket_errno();
				LOG_DEBUG("[Connect] fail errno=%d errinfo=%s\n", 
						  errcode, socket_errinfo());
				if(errcode == EWOULDBLOCK)
				{  
					//非阻塞socket connect 等待
					return RET_SOCKET_CONTINUE;
				}
				else
				{
					 //发生socket错误，迁移任务状态到失败
					 return RET_SOCKET_ERROR;
				}
			}
			else if(0 == ret)
			{
				return RET_OK;
			}

			return RET_SOCKET_ERROR;
		}
	
		case HTTPDOWN_PROXY_SOCK5:
		{							
			proxyipaddr = htSkp->Host2Str(m_proxy.host.c_str());
			LOG_DEBUG("Connecting to socks5 proxy server %s:%d", proxyipaddr, m_proxy.port);
			ret = htSkp->Connect(proxyipaddr, m_proxy.port, 100);
			
			if(ret == -1)
			{		
				int errcode = socket_errno();
				LOG_DEBUG("[Connect] fail errno=%d errinfo=%s\n", 
						  errcode, socket_errinfo());
				if(errcode == EWOULDBLOCK)
				{  
					//非阻塞socket connect 等待
					return RET_SOCKET_CONTINUE;
				}
				else
				{
					 //发生socket错误，迁移任务状态到失败
					 return RET_SOCKET_ERROR;
				}
			}
			else if(0 == ret)
			{
				return RET_OK;
			}

			return RET_SOCKET_ERROR;			
		}
		default:
			break;
	}
	
	return RET_OK;
}

int  THttpCli :: checkConnect(void)
{
	int err;
	err = htSkp->checkConnect();
	if(NOCONERROR == err)
	{
		return RET_OK;
	}
	
	LOG_WARN("[checkConnect]fail ret=%d.\n", err);
	return RET_SOCKET_ERROR;
}


void THttpCli :: Disconnect(void)
{
	//Disconnect 时需要将当前连接的所有状态信息清空
	if(NULL != m_socks)
	{
		LOG_DEBUG("[THttpCli]Disconnect delete socks proxy for %p\n", htSkp);
		delete 	m_socks;
		m_socks = NULL;
		socks_proxy_status = SOCKS_PROXY_INIT;
	} 
	
	if(NULL != htSkp)
	{
		LOG_DEBUG("[THttpCli]Disconnect begin for %p\n", htSkp);
		htSkp->Close();
	}
	else
	{
		LOG_DEBUG("[THttpCli]Disconnect but htSkp is NULL\n");
	}

	htRequest->ClearAllHdr();
	htResponse->ClearAllHdr();
	memset(StatusLine, 0x0, LINE_BUFFER);
}

int THttpCli :: BufReadChar(char *c)
{
    int retval = htSkp->direct_recv(c, 1, 0);
	
    if (retval <= 0)
		return retval;
    return 1;
}

int THttpCli :: BufPeek(char *c)
{
    int retval = htSkp->Recv(c, 1, MSG_PEEK, 180);
	
    if (retval <= 0)
		return retval;
    return 1;
}

int THttpCli :: ReadLine(char *line, int maxsize, int& recved)
{
	int i, retval=0;
	int ret = 0;
	char c;
	char *ptr;
	
	ptr = line;
	
	for(i = 1; i < maxsize; i++)
    {
		retval = BufReadChar(&c);
		
		if (retval <= 0)
		{
			break;
		}

		*ptr = c;
		ptr++;
		recved++;
		if(c == '\n')
			break;
    }


	if(retval == -1)
	{
		//被信号中断，或者需要等待对端数据到来，则忽略错误
		int errcode = socket_errno();
		LOG_WARN("[ParseHttpHdrs]recv fail errno=%d errinfo=%s\n", 
				  errcode, socket_errinfo());
		if(errcode != EAGAIN && socket_errno() != SOCKET_EINTR)
		{	//发生socket错误，迁移任务状态到失败
			ret = RET_SOCKET_ERROR;
		}
		else
		{	//此时应该将当前行已经收到的数据暂存下来		
			memcpy(uncompleteLineBuf+recvedlen, line, recved);			
			recvedlen +=recved;
			
			if(recvedlen >= maxsize)
			{	//收到的数据已经大于LINE_BUFFER，则结束行读取(此时并没有换行符得到)，将缓冲区尾部置为0
				memcpy(line, uncompleteLineBuf, maxsize);	//将旧数据拷入缓冲区
				line[recvedlen - 1] = '\0';
				recved = maxsize - 1;
				ret = RET_SOCKET_FINISH;				
			}
			else
			{
				ret = RET_SOCKET_CONTINUE;
			}			
		}
	}
	else if(0 == retval)
	{
		ret = RET_SOCKET_CLOSE;
	}
	else
	{
		if(recvedlen != 0)	//存在上次readline暂存的数据，则需要拼接
		{
			memcpy(uncompleteLineBuf+recvedlen, line, recved);			
			recvedlen +=recved;
			
			if(recvedlen >= maxsize)
			{	//收到的数据已经大于LINE_BUFFER，则结束行读取(此时并没有换行符得到)，将缓冲区尾部置为0
				memcpy(line, uncompleteLineBuf, maxsize);	//将旧数据拷入缓冲区
				line[maxsize - 1] = '\0';
				recved = maxsize - 1;
			}
			else
			{
				memcpy(line, uncompleteLineBuf, recvedlen);	//将旧数据拷入缓冲区
				line[recvedlen] = '\0';
				recved = recvedlen;
			} 
		}
		else
		{	//没有需要拼接的数据
			line[recved] = '\0';
		}
	
		ret = RET_SOCKET_FINISH;
	}
		

	if(ret != RET_SOCKET_CONTINUE)
	{
		recvedlen = 0; 	//此情况下不需要等待下次读取，将暂存区置空
	}

	return ret;
}

int THttpCli :: ReadLine(char *line, int maxsize)
{
	int i, retval;
	char c;
	char *ptr;
    
	ptr = line;
	
	for(i = 1; i < maxsize; i++)
    {
		retval = BufReadChar(&c);
		
		if (retval < 0)
			return retval;	//HERR

		if (retval == 0)
		{
			if(i == 1)
				return 0; //HEOF
			else
				break;
		}

		*ptr = c;
		ptr++;
		if(c == '\n')
			break;
    }

	*ptr = '\0';

	if(i == maxsize)
		return i - 1;
	else
		return i;
}

int THttpCli :: ParseStatLine(const char *hdr, const char **rp)
{
    int Major, Minor;
    int StatusCode;	

    const char *p;

    *rp = NULL;
    
	if(strncmp(hdr, "HTTP/", 5) != 0)
		return -1;
	hdr += 5;
    p = hdr;
	
    for (Major = 0; isdigit(*hdr); hdr++)
		Major = 10 * Major + (*hdr - '0');
    
	if (*hdr != '.' || p == hdr)
		return -1;
    
	++hdr;
    p = hdr;
    
	for (Minor = 0; isdigit(*hdr); hdr++)
		Minor = 10 * Minor + (*hdr - '0');
	
    if (*hdr != ' ' || p == hdr)
		return -1;
    
	if (Major < 1)
		return -1;
    
	++hdr;
    
	if (!(isdigit(*hdr) && isdigit(hdr[1]) && isdigit(hdr[2])))
		return -1;
    
	StatusCode = 100 * (*hdr - '0') + 10 * (hdr[1] - '0') + (hdr[2] - '0');
    
	if (hdr[3] != ' ')
    {
		if (!hdr[3])
		   	*rp = hdr + 3;
		else
	    	return -1;
    } 
	else
		*rp = hdr + 4;
	
    return StatusCode;
}

int THttpCli :: SkipLws(const char *hdr)
{
    int i;
    for (i = 0; *hdr == ' ' || *hdr == '\t' || *hdr == '\r' || *hdr == '\n'; ++hdr)
	{
		++i;
	}
    return i;
}

//UTC格林威治时间转本地时间
void THttpCli :: UTCtoLocalTime(struct tm &UTCtm,struct tm &Localtm)
{

	TIME_ZONE_INFORMATION tzi;
	GetTimeZoneInformation(&tzi);
	int zone = tzi.Bias/-60;  //时区，如果是中国标准时间则得到8

	Localtm =  UTCtm;
	Localtm.tm_hour = Localtm.tm_hour + zone;  //本地时间 = UTC时间 + 时区

}

//返回服务器最后修改时间，若未取到服务器最后修改时间则返回当前时间
time_t  THttpCli ::GetLastModified(void)
{
	time_t modifiedTime;
	time_t curTime;
	curTime = time(NULL);  //当前时间

	OutputDebugStringA("\nLast-Modified:  ");
	if(htResponse != NULL)
	{
		string strval = htResponse->GetHdr("Last-Modified");		
		OutputDebugStringA(strval.c_str());
		if(strval.size()>0)
		{
			string strAllMonth = "jan,feb,mar,apr,may,jan,jul,aug,sep,oct,nov,dec";
			common::utility::stringhelper::MakeLowerA(strval);
			vector<string> vecDate = common::utility::stringhelper::Split_String_by_DelimiterA(strval,' ');
			if(vecDate.size() == 6)
			{
				string strMonth;
				vector<string> vecTime = common::utility::stringhelper::Split_String_by_DelimiterA(vecDate[4],':');
				int nYear,nMonth,nDay,nHear,nMinute,nSecond;
				nDay = atoi(vecDate[1].c_str());
				strMonth = vecDate[2];
				nYear = atoi(vecDate[3].c_str());
				int monthFind = strAllMonth.find(strMonth);
				nMonth = monthFind/4 +1;
				
				if(vecTime.size() == 3)
				{
					nHear = atoi(vecTime[0].c_str());
					nMinute = atoi(vecTime[1].c_str());
					nSecond = atoi(vecTime[2].c_str());

					struct tm tModified,tModifiedLocal;
					tModified.tm_year = nYear - 1900;
					tModified.tm_mon = nMonth - 1;
					tModified.tm_mday = nDay;
					tModified.tm_hour = nHear;
					tModified.tm_min = nMinute;
					tModified.tm_sec = nSecond;
					UTCtoLocalTime(tModified,tModifiedLocal);
					modifiedTime = mktime(&tModifiedLocal);  //服务器最后修改时间

					return modifiedTime;
				}
			}
		}	
	}	

	OutputDebugStringA( "no LastModified");
	return curTime;
	
}

off_t THttpCli :: GetContentLen(void)
{
	if(htResponse != NULL)
	{
		string strval = htResponse->GetHdr("Content-Length");
		const char * ptr = strval.c_str();
		off_t Len;
		if(ptr != NULL)
		{
			for (Len = 0; isdigit(*ptr); ptr++)
				Len = 10 * Len + (*ptr - '0');
			return Len;
		}
	}	
	return -1;
}

off_t THttpCli :: GetContentRange(void)
{
	if(htResponse != NULL)
	{
		string strval = htResponse->GetHdr("Content-Range");
		const char * ptr = strval.c_str();
		off_t Len;
		if(ptr != NULL)
		{
			if(!strncasecmp(ptr, "Bytes", 5))
    		{
				ptr += 5;
				ptr += SkipLws(ptr);
				if (!*ptr)
	    			return -1;
    		}
			for (Len = 0; isdigit(*ptr); ptr++)
				Len = 10 * Len + (*ptr - '0');
			return Len;
		}
	}
	return -1;
}

const char* THttpCli :: GetLocation(string& location)
{
	if(htResponse != NULL)
	{
		location = htResponse->GetHdr("Location");
		return location.c_str();
	}
	return NULL;
}

int THttpCli :: ProcessHttpResponse( char * strCmd )
{
	int retval;

	retval = ParseHttpHdrs();

	if (retval != RET_OK)
	{
    	LOG_DEBUG("End of file while parsing headers");
    	return retval;
	}

	
	if (H_20X(StatusCode))
		return RET_OK;
   	
	if (H_REDIRECTED(StatusCode))
		return RET_NEWDIRECTION;
			
	if(StatusCode == HTTP_MULTIPLE_CHOICES)
    	return RET_OK;

	if(StatusCode == HTTP_NEED_PASS)
    	return RET_PROXY_AUTH_UNSUPPORTED;

	// Error StatusCode
#if 0
	switch (StatusCode)
	{
	case HTTP_UNAUTHORIZED		:	return HAUTHREQ;
	case HTTP_NOT_FOUND			:	return HTTPNSFOD;
	case HTTP_INTERNAL			:   return INTERNALSERVERR;
	case HTTP_NOT_IMPLEMENTED	:	return UNKNOWNREQ;
	case HTTP_BAD_GATEWAY		:	return BADGATEWAY;
	case HTTP_UNAVAILABLE		:	return SERVICEUNAVAIL;
	case HTTP_GATEWAY_TIMEOUT	:	return GATEWAYTIMEOUT;
	}

	return ERR_TASK_HTTP;
#endif
	return RET_HTTP_ERR;
}

/* function to Parse the http headers from the socket */
int THttpCli :: ParseHttpHdrs(void)
{
	int ret = 0, recved = 0;
	char *ptr, *strKey, *strVal;
	char buf[LINE_BUFFER];
	
	htResponse->ClearAllHdr();

	ret = ReadLine(buf, LINE_BUFFER, recved);

	if(ret != RET_SOCKET_FINISH)
	{
		return ret;
	}

	LOG_DEBUG("%s", buf);
	
	memset(StatusLine, 0, LINE_BUFFER);
	strncpy(StatusLine, buf, LINE_BUFFER - 1);
	ptr = buf;
	
	while(*ptr == ' ') ptr++;
	
	// skip the http version info
	while(*ptr != '\0' && *ptr != ' ' && *ptr != '\r' && *ptr != '\n') ptr++;
	
	if(*ptr != ' ') return -1;
	
	while(*ptr == ' ') ptr++;

	StatusCode = atoi(ptr);
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		recved = 0;
		ret = ReadLine(buf, LINE_BUFFER, recved);
		
		if(ret != RET_SOCKET_FINISH)
		{
			return ret;
		}	
		LOG_DEBUG("%s", buf);
		ptr = buf;
		while(*ptr == ' ') ptr++;
		
		strKey = ptr;
		
		if(*ptr == '\r' || *ptr == '\n') break;
		
		while(*ptr != '\0' && *ptr != ':' && *ptr != '\r' && *ptr != '\n') ptr++;
		
		if(*ptr == ':')
		{
			*ptr = '\0';
			ptr++;
		}
		else
		{
			return RET_PARSE_RESQ_FAIL;
		}

		while(*ptr == ' ') ptr++;
		
		strVal = ptr;
				
		while(*ptr != '\0' && *ptr != '\r' && *ptr != '\n') ptr++;
		
		*ptr = '\0';
		
		htResponse->SetHdr(strKey, strVal);
	}
	
	htResponse->ShowAllHttpHdr();	
	return RET_OK;
}

void THttpCli :: BuildUserAgentHdr(void)
{
  	if (htRequest != NULL)
  	{
		htRequest->SetHdr("User-Agent", "Mozilla/4.0(compatible;MSIE 5.00;Windows 98)");
	}
}

void THttpCli::BuildHostHdr(char * host, int port)
{
	char szBuffer[1024];
  	if (htRequest != NULL)
  	{
		memset(szBuffer, 0, sizeof(szBuffer));

        if(port != 80)
		{
        	snprintf(szBuffer, sizeof(szBuffer), "%s:%d", host, port);
        }
		else
		{
			snprintf(szBuffer, sizeof(szBuffer), "%s", host);
        }

		string sHost((const char *)szBuffer);
		
		htRequest->SetHdr("Host", sHost);
	}
}

void THttpCli :: BuildCustomHdr(const char* strkey, const char* strval)
{
  	if (htRequest != NULL && strkey != NULL && strval != NULL)
  	{
		htRequest->SetHdrDuplicate(strkey, strval);
	}
}

void THttpCli :: BuildCookieHdr(const char* strkey, const char* strval)
{
	char buf[512];
  	if (htRequest != NULL && strkey != NULL && strval != NULL)		
  	{
  		snprintf(buf, sizeof(buf), "%s=%s", strkey, strval);
		htRequest->SetHdrDuplicate("Cookie: ", buf);
	}
}

void THttpCli :: BuildAcceptHdr(void)
{
  	if (htRequest != NULL)
  	{
		htRequest->SetHdr("Accept", "*/*");
	}
}

void THttpCli :: BuildRangeHdr(off_t StartPos, off_t EndPos)
{
	char szBuffer[LINE_BUFFER];
  	if (htRequest != NULL)
  	{
		memset(szBuffer, 0, LINE_BUFFER);
		if(EndPos != -1)
     		sprintf(szBuffer, "bytes=%ld-%ld", StartPos, EndPos);
		else
		    sprintf(szBuffer, "bytes=%ld-", StartPos);

		htRequest->SetHdr("Range", szBuffer);
	}
}

void THttpCli :: BuildRefererHdr(char * Ref)
{
	if (htRequest != NULL)
  	{
 		htRequest->SetHdr("Referer", Ref);
  	}
}

void THttpCli :: BuildBasicAuthHdr( char * UserName, char * Password )
{
	if(htRequest != NULL)
	{
		if( UserName == NULL)
			UserName = 	DEFAULT_USER;
		if(Password == NULL)
			Password = 	DEFAULT_PASSWD;	
		
		std::string strSrc = UserName + std::string(":") + Password;
		string strResult = common::CBase64Coding::Encode( strSrc);
		htRequest->SetHdr("Authorization", strResult);
	}
}

void THttpCli :: BuildProxyAuthHdr(const char * UserName, const char * Password )
{
	if(htRequest != NULL)
	{
		if( UserName == NULL)
			UserName = 	DEFAULT_USER;
		if(Password == NULL)
			Password = 	DEFAULT_PASSWD;	
	
		std::string strSrc = UserName + std::string(":") + Password;
		string strResult = common::CBase64Coding::Encode( strSrc);
		htRequest->SetHdr("Proxy-Authorization", strResult);
	}

}

void THttpCli :: BuildConnectionHdr( void )
{
	if(htRequest != NULL)
	{	
		htRequest->SetHdr("Connection", "Close");
	}
}

void THttpCli :: BuildContentRangeHdr( void )
{
	if(htRequest != NULL)
	{	
		htRequest->SetHdr("Content-Range", "bytes=0-");
	}
}

void THttpCli :: BuildContentType(const char* contenttype)
{
	if(htRequest != NULL)
	{	
		htRequest->SetHdr("Content-Type", contenttype);
	}
}

void THttpCli :: BuildContentType()
{
	char contenttype[128] = {0};
	//此处要跳过postBoundary的前面两个"--"字符
	snprintf(contenttype, sizeof(contenttype), 
		"multipart/form-data; boundary=%s", postBoundary+2);

	if(htRequest != NULL)
	{	
		htRequest->SetHdr("Content-Type", contenttype);
	}
}


void THttpCli :: BuildContentLengthHdr(ULONG64 contentLength )
{
	char buf[32] = {0};
	if(htRequest != NULL)
	{	
		snprintf(buf, sizeof(buf), "%lu", contentLength);
		htRequest->SetHdr("Content-Length", buf);
	}
}

void THttpCli :: BuildPragmaHdr( void )
{
	if(htRequest != NULL)
	{	
		htRequest->SetHdr("Pragma", "no-cache");
	}
}


void THttpCli :: BuildNtlmAuthHdr(const char* szcode)
{
	if (htRequest != NULL)
  	{
 		htRequest->SetHdr("Proxy-Authorization", szcode);
  	}
}

void THttpCli :: ClearRequestHdr()
{
	htRequest->ClearAllHdr();
}

void THttpCli :: ClearResponseHdr()
{
	htResponse->ClearAllHdr();
	memset(StatusLine, 0x0, LINE_BUFFER);
}


#if 0
void THttpCli :: BuildHttpRequest( THttpVerb verb, char * szBuffer)
{
	int pos = 0, nHdr;
	THttpHdr * pHdr;
	
	memset(StatusLine, 0, LINE_BUFFER);

	if(htRequest != NULL && htRequest->GetHdrCount() > 0)
	{
		if(verb == VERB_GET)
		{
			if(UseProxy)
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_GET_TEMPLATE, u->get_url());
				
				snprintf(szBuffer, 1024, HTTP_METHOD_GET_TEMPLATE, u->get_url());
				pos += HTTP_METHOD_GET_FIXLEN+ (int)strlen(u->get_url());
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_GET_TEMPLATE,
													    u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_GET_TEMPLATE,
													u->get_encoded_path());
				pos += HTTP_METHOD_GET_FIXLEN + (int)strlen(u->get_encoded_path());
			}
		}

		if(verb == VERB_POST)
		{
			if(UseProxy)
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_POST_TEMPLATE, u->get_url());
				
				snprintf(szBuffer, 1024, HTTP_METHOD_POST_TEMPLATE, u->get_url());
				pos += HTTP_METHOD_POST_FIXLEN + (int)strlen(u->get_url());
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_POST_TEMPLATE,
														u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_POST_TEMPLATE,
													u->get_encoded_path());
				pos += HTTP_METHOD_POST_FIXLEN + (int)strlen(u->get_encoded_path());
			}
		}
		
		if(verb == VERB_CONNECT)
		{
			if(UseProxy)
			{	
				char sites[LINE_BUFFER];
				snprintf(sites, sizeof(sites) - 2, "%s:%u", 
										u->get_url(), u->get_port());
				
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_CONNECT_TEMPLATE, sites);
						
				snprintf(szBuffer, 1024, HTTP_METHOD_CONNECT_TEMPLATE, sites);
				pos += HTTP_METHOD_CONNECT_FIXLEN + (int)strlen(sites);
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_CONNECT_TEMPLATE,
														 u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_CONNECT_TEMPLATE,
													u->get_encoded_path());
				pos += HTTP_METHOD_CONNECT_FIXLEN + (int)strlen(u->get_encoded_path());
			}
		}

		
		if(verb == VERB_HEAD)
    	{
			if(UseProxy)
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_HEAD_TEMPLATE, u->get_url());

				snprintf(szBuffer, 1024, HTTP_METHOD_HEAD_TEMPLATE, u->get_url());
				pos += HTTP_METHOD_HEAD_FIXLEN + (int)strlen(u->get_url());
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_HEAD_TEMPLATE,
														 u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_HEAD_TEMPLATE,
												u->get_encoded_path());
				pos += HTTP_METHOD_HEAD_FIXLEN + (int)strlen(u->get_encoded_path());
			}
			
		}
		
		nHdr = (int)htRequest->GetHdrCount();
    	for(int i = 0; i < nHdr; i++)
		{
			pHdr = htRequest->GetHdr( i );
		
	        snprintf(szBuffer + pos, LINE_BUFFER, "%s: %s\r\n", 
						pHdr->GetKey().c_str(), pHdr->GetVal().c_str());
	
			pos += (int)htRequest->GetHdrLen(i);
    	}
		sprintf(szBuffer + pos, "\r\n");
	}
}
#endif
INT THttpCli :: BuildHttpRequest( THttpVerb verb, char *& szBuffer, ULONG64& bufferlen, POST_PARAMS* postParams, TASK_TYPE task_type, ULONG64& datalen)
{
	int ret = 0;
	ULONG64 pos = 0, nHdr;
	THttpHdr * pHdr;
	
	memset(StatusLine, 0, LINE_BUFFER);

	if(htRequest != NULL && htRequest->GetHdrCount() > 0)
	{
		/********************************************/
		//读入http 方法行
		/********************************************/
		//方法行部分数据量较小，且固定，不会出现缓冲区空间不够情况
		if(verb == VERB_GET)
		{
			if(UseProxy)
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_GET_TEMPLATE, u->get_url());
				
				snprintf(szBuffer, 1024, HTTP_METHOD_GET_TEMPLATE, u->get_url());
				pos += HTTP_METHOD_GET_FIXLEN+ (int)strlen(u->get_url());
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_GET_TEMPLATE,
													    u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_GET_TEMPLATE,
													u->get_encoded_path());
				pos += HTTP_METHOD_GET_FIXLEN + (int)strlen(u->get_encoded_path());
			}
		}

		if(verb == VERB_POST)
		{
			if(UseProxy)
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_POST_TEMPLATE, u->get_url());
				
				snprintf(szBuffer, 1024, HTTP_METHOD_POST_TEMPLATE, u->get_url());
				pos += HTTP_METHOD_POST_FIXLEN + (int)strlen(u->get_url());
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_POST_TEMPLATE,
														u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_POST_TEMPLATE,
													u->get_encoded_path());
				pos += HTTP_METHOD_POST_FIXLEN + (int)strlen(u->get_encoded_path());
			}				
		}
		
		if(verb == VERB_CONNECT)
		{
			if(UseProxy)
			{	
				char sites[LINE_BUFFER];
				snprintf(sites, sizeof(sites) - 2, "%s:%u", 
										u->get_url(), u->get_port());
				
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_CONNECT_TEMPLATE, sites);
						
				snprintf(szBuffer, 1024, HTTP_METHOD_CONNECT_TEMPLATE, sites);
				pos += HTTP_METHOD_CONNECT_FIXLEN + (int)strlen(sites);
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_CONNECT_TEMPLATE,
														 u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_CONNECT_TEMPLATE,
													u->get_encoded_path());
				pos += HTTP_METHOD_CONNECT_FIXLEN + (int)strlen(u->get_encoded_path());
			}
		}

		
		if(verb == VERB_HEAD)
    	{
			if(UseProxy)
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_HEAD_TEMPLATE, u->get_url());

				snprintf(szBuffer, 1024, HTTP_METHOD_HEAD_TEMPLATE, u->get_url());
				pos += HTTP_METHOD_HEAD_FIXLEN + (int)strlen(u->get_url());
			}
			else
			{
				snprintf(StatusLine, LINE_BUFFER, HTTP_METHOD_HEAD_TEMPLATE,
														 u->get_encoded_path());

				snprintf(szBuffer, 1024, HTTP_METHOD_HEAD_TEMPLATE,
												u->get_encoded_path());
				pos += HTTP_METHOD_HEAD_FIXLEN + (int)strlen(u->get_encoded_path());
			}
			
		}

		/********************************************/
		//读入http 头部
		/********************************************/
		//头部部分数据量较小，不会出现缓冲区空间不够情况
		nHdr = (int)htRequest->GetHdrCount();
    	for(int i = 0; i < nHdr; i++)
		{
			pHdr = htRequest->GetHdr( i );
		
	        snprintf(szBuffer + pos, LINE_BUFFER, "%s: %s\r\n", 
						pHdr->GetKey().c_str(), pHdr->GetVal().c_str());
	
			pos += (int)htRequest->GetHdrLen(i);
    	}
		sprintf(szBuffer + pos, "\r\n");
		pos += (int)strlen("\r\n");

		LOG_DEBUG("[BuildHttpRequest]Status+Head len=%lld\n", pos);
		
		/********************************************/
		//读入Post 数据
		/********************************************/
		//POST参数部分由于存在文件读取，数据量可能变化，因此应动态扩展缓冲区空间
		if(verb == VERB_POST)
		{			
			PARAMS_V* pParams = postParams->get_params();
			char tempBuf[MAX_BUFFER_LEN] = {0};
			int temppos = 0;
			
			//TASK_BINARY 的参数不采用Multi-part的组包方式，直接写入2进制数据
			if(TASK_BINARY == task_type)
			{				
				char* paramBuffer = (*pParams)[0].paramBuffer;
				UINT32 paramBufferLen = (*pParams)[0].paramBufferLen;

				memcpy(szBuffer+pos, paramBuffer, paramBufferLen);
				pos += paramBufferLen;			
			}
			else
			{
				for(UINT i = 0; i < pParams->size(); i++)
				{
					char* paramName = (*pParams)[i].paramName;
					char* paramBuffer = (*pParams)[i].paramBuffer;
					UINT32 paramBufferLen = (*pParams)[i].paramBufferLen;
					PARAM_TYPE paramType = (*pParams)[i].paramType;
					temppos = 0;
					
					switch(paramType)
					{
						case PARAM_TEXT:
						{
							sprintf(tempBuf + temppos, "%s\r\n", postBoundary);
							temppos += (int)strlen(postBoundary) + (int)strlen("\r\n");
							//LOG_DEBUG("paramlen=%d temppos=%d text=%s\n", strlen(tempBuf), temppos,tempBuf);

							sprintf(tempBuf + temppos, 
										POSTPARAM_TEMPLATE_TEXT, paramName);
							temppos += POSTPARAM_TEMPLATE_TEXT_LEN + (int)strlen(paramName);
							//LOG_DEBUG("paramlen=%d temppos=%d text=%s\n", strlen(tempBuf), temppos,tempBuf);

							sprintf(tempBuf + temppos, "%s\r\n", paramBuffer);
							temppos += (int)strlen(paramBuffer) + (int)strlen("\r\n");

							//LOG_DEBUG("paramlen=%d temppos=%d text=%s\n", strlen(tempBuf), temppos,tempBuf);
							break;
						}
						
						case PARAM_FILENAME:	
						{
							sprintf(tempBuf + temppos, "%s\r\n", postBoundary);
							temppos += (int)strlen(postBoundary) + (int)strlen("\r\n");			
							char* mimetype = getMimeType(paramBuffer);
							sprintf(tempBuf + temppos, 
										POSTPARAM_TEMPLATE_FILE, 
										paramName, paramBuffer, mimetype);
							temppos += POSTPARAM_TEMPLATE_FILE_LEN 
										+ (int)strlen(paramName)+(int)strlen(paramBuffer)+(int)strlen(mimetype);
											
							//检查文件是否存在, 文件大小可否放入缓冲区
							ULONG64 filesize = 0;
							ULONG64 realfilesize = 0;	
							LONG64 neadSize = 0;

							if(!IsTargetFileExist(paramBuffer, filesize))
							{
								return RET_NO_FILE;	//Post 参数中指定的文件不存在							
							}						
							if(paramBufferLen > 0 && filesize <= paramBufferLen)
							{
								return RET_FILE_READ;
							}						
							neadSize = (paramBufferLen > 0) ? (filesize - paramBufferLen):filesize;
							
							if(RET_OK != (ret = checkBufLen(szBuffer, pos + temppos + neadSize, bufferlen)))
							{
								return ret;
							}						

							//将组织好的Post参数数据写入缓冲区 --- 写入文件参数描述信息
							memcpy(szBuffer+pos, tempBuf, temppos);
							pos += temppos;
							
							LOG_DEBUG("[BuildHttpRequest]After add head for file=%s len=%lld\n", 
														paramBuffer, pos);

							//下面读入文件数据
							FILE* fp = NULL;
							fp = fopen(paramBuffer, "r+b");
							if(NULL == fp)
							{
								return RET_FILE_CANNOT_OPEN;
							}
							if(paramBufferLen > 0)
							{
								if(0 != fseek(fp, paramBufferLen, SEEK_SET))
								{
									return RET_FILE_READ;
								}
							}
							
							//unsigned char ch = '\0';
							//while(EOF != (ch = fgetc(fp)))
							int numreaded = 0;
							while(0 < (numreaded=(int)fread(tempBuf, sizeof(char), MAX_BUFFER_LEN, fp))) 
							{
						#if 0
								realfilesize ++;
								if(realfilesize > filesize)	//文件大小有变化，需要检查是否超出缓冲区大小
								{
									if(RET_OK != (ret = checkBufLen(szBuffer, pos, bufferlen)))
									{
										return ret;
									}																
								}
								szBuffer[pos++] = ch;
						#endif
								//temppos = MAX_BUFFER_LEN;
								memcpy(szBuffer+pos, tempBuf, numreaded);
								pos += numreaded;
								realfilesize += numreaded;
							}
							
							if(0 != ferror(fp))	//检查读取是否正常终止
							{
								return RET_FILE_READ;
							}						
							
							fclose(fp);
							temppos = 0;
							//此处需要追加"\r\n"
							memcpy(szBuffer+pos, "\r\n", strlen("\r\n"));
							pos += strlen("\r\n");
							LOG_DEBUG("[BuildHttpRequest]After add file=%s len=%lld filesize=%lld realfilesize=%lld\n", 
											paramBuffer, pos, filesize, realfilesize);
							break;
						}
						
						case PARAM_BINARY:
						{						
							sprintf(tempBuf + temppos, "%s\r\n", postBoundary);
							temppos += (int)strlen(postBoundary) + (int)strlen("\r\n");

							sprintf(tempBuf + temppos, 
										POSTPARAM_TEMPLATE_BINARY, paramName);
							temppos += POSTPARAM_TEMPLATE_BINARY_LEN + (int)strlen(paramName);

							//将组织好的Post参数数据写入缓冲区 --- 写入参数描述信息
							memcpy(szBuffer+pos, tempBuf, temppos);
							pos += temppos;

							//由于PARAM_BINARY 长度可能较大，故此处直接写入目的buffer，不使用临时buffer

							memcpy(szBuffer+pos, paramBuffer, paramBufferLen);
							pos += paramBufferLen;
							sprintf(szBuffer+pos, "\r\n");
							pos += (int)strlen("\r\n");
							temppos = 0;
							

//								memcpy(tempBuf + temppos, paramBuffer, paramBufferLen);
//								temppos += paramBufferLen;
							
//								sprintf(tempBuf + temppos, "\r\n");
//								temppos += (int)strlen("\r\n");							

							break;
						}

						default:
							return RET_POSTPARAM_TYPE_INVALID;	//Post 参数的参数类型不合法
					}		

					if(temppos > 0)
					{
						//将组织好的Post参数数据写入缓冲区 --- 此处不读入文件参数的数据(文件时temppos=0)
						if(RET_OK != (ret = checkBufLen(szBuffer, pos + temppos, bufferlen)))
						{
							return ret;
						}

						memcpy(szBuffer+pos, tempBuf, temppos);
						pos += temppos;
						LOG_DEBUG("[BuildHttpRequest]After add param=%s len=%lld\n", paramName, pos);
					}
				}
			
				//Post 数据必须以__BROUNDARY+"--\r\n"结尾
				sprintf(tempBuf, "%s--\r\n", postBoundary);
				temppos = (int)strlen(postBoundary) + (int)strlen("--\r\n");

				if(RET_OK != (ret = checkBufLen(szBuffer, pos + temppos, bufferlen)))
				{
					return ret;
				}
				
				memcpy(szBuffer+pos, tempBuf, temppos);
				pos += temppos;	
			}
		}		
	}	
	datalen = pos;	//记录实际写入数据大小	
	LOG_DEBUG("[BuildHttpRequest]End len=%lld\n", pos);
	return RET_OK;
}

off_t THttpCli :: RecvData(char *buffer, int size, int flags, int timeout)
{
	int	BytesRead = htSkp->Recv(buffer, size, flags, timeout);
	return BytesRead;
}

off_t THttpCli :: DirectRecvData(CHAR_VECTOR* respData, ULONG64 size, int flags)
{
	//int i = 0;
    off_t BytesRead = 0;
	char szBuffer[HTTP_BUF_SIZE];	
    SetIPTOS();

//		do
//	    {	
//			BytesRead = htSkp->direct_recv(szBuffer, HTTP_BUF_SIZE, 0);
//	
//			if(BytesRead > 0)
//			{				
//				for(i = 0; i < BytesRead; i++)
//				{
//					respData->push_back(szBuffer[i]);
//				}
//			}
//	
//	    }while(BytesRead == HTTP_BUF_SIZE);	//收到数据和buffer大小相同，说明可能存在后续数据

	//按照应用要求，上报进度间隔以recv为边界，每次recv都上报
	BytesRead = htSkp->direct_recv(szBuffer, HTTP_BUF_SIZE, 0);
	LOG_DEBUG("[DirectRecvData]recv byte=%d needsize=%d\n", 
				  BytesRead, size);

	if(BytesRead > 0)
	{				
		size_t oriLen = respData->size();
		respData->resize(oriLen+BytesRead);
		memcpy( &((*respData)[oriLen]), szBuffer, BytesRead);

		/*
		for(i = 0; i < BytesRead; i++)
		{
			respData->push_back(szBuffer[i]);
		}
		*/
	}
  
   	if(BytesRead == -1)
	{
		//被信号中断，或者需要等待对端数据到来，则忽略错误
		int errcode = socket_errno();
		LOG_WARN("[DirectRecvData]recv fail errno=%d errinfo=%s\n", 
				  errcode, socket_errinfo());
		if(errcode != EWOULDBLOCK && socket_errno() != SOCKET_EINTR)
		{   //发生socket错误，迁移任务状态到失败
			return RET_SOCKET_ERROR;
		}
	}
	else if(0 == BytesRead || (size > 0 && BytesRead >= size))
    {   //连接被对端关闭，则应迁移状态至成功
  	  //响应数据长度已知，且已接收到相应长度
  	   return RET_SOCKET_FINISH; 	  
    }  

	return RET_SOCKET_CONTINUE;
}


off_t THttpCli :: SendData(char *buffer, ULONG64 size, int flags, int timeout, ULONG64& BytesWritten)
{	
	int ret = 0;
	ret = htSkp->Send(buffer, size, flags, timeout, BytesWritten);

	if(ret < 0)
	{
		int errcode = socket_errno();

		if(errcode == EWOULDBLOCK)
		{
			//socket阻塞，等待下一次发送
			
			return RET_SOCKET_CONTINUE;
		}
		
		LOG_WARN("[SendData]send fail errno=%d errinfo=%s\n", 
				  errcode, socket_errinfo());
		return RET_SOCKET_ERROR;
	}
	else if(ret == 0)
	{
		//socket关闭
		return RET_SOCKET_CLOSE;
	}
	else if(BytesWritten == size)
	{
		//数据发送完
		return RET_SOCKET_FINISH;
	}

	//数据未发送完，等待下一次发送
	return RET_SOCKET_CONTINUE;	
}

void THttpCli :: SetIPTOS(void)
{
#ifndef 	WIN32	//该函数在win下不支持
	htSkp->SetIPTOS();	
#endif
}


char* mimetype[][2] = 
{
	{".3dm", "x-world/x-3dmf"}, 
	{".3dmf", "x-world/x-3dmf"}, 
	{".a", "application/octet-stream"}, 
	{".aab", "application/x-authorware-bin"}, 
	{".aam", "application/x-authorware-map"}, 
	{".aas", "application/x-authorware-seg"}, 
	{".abc", "text/vnd.abc"}, 
	{".acgi", "text/html"}, 
	{".afl", "video/animaflex"}, 
	{".ai", "application/postscript"}, 
	{".aif", "audio/aiff"}, 
	{".aifc", "audio/aiff"}, 
	{".aiff", "audio/aiff"}, 
	{".aim", "application/x-aim"}, 
	{".aip", "text/x-audiosoft-intra"}, 
	{".ani", "application/x-navi-animation"}, 
	{".aos", "application/x-nokia-9000-communicator-add-on-software"}, 
	{".aps", "application/mime"}, 
	{".arc", "application/octet-stream"}, 
	{".arj", "application/arj"}, 
	{".art", "image/x-jg"}, 
	{".asf", "video/x-ms-asf"}, 
	{".asm", "text/x-asm"}, 
	{".asp", "text/asp"}, 
	{".asx", "video/x-ms-asf"}, 
	{".au", "audio/basic"}, 
	{".avi", "video/avi"}, 
	{".avs", "video/avs-video"}, 
	{".bcpio", "application/x-bcpio"}, 
	{".bin", "application/octet-stream"}, 
	{".bm", "image/bmp"}, 
	{".bmp", "image/bmp"}, 
	{".boo", "application/book"}, 
	{".book", "application/book"}, 
	{".boz", "application/x-bzip2"}, 
	{".bsh", "application/x-bsh"}, 
	{".bz", "application/x-bzip"}, 
	{".bz2", "application/x-bzip2"}, 
	{".c", "text/plain"}, 
	{".c++", "text/plain"}, 
	{".cat", "application/vnd.ms-pki.seccat"}, 
	{".cc", "text/plain"}, 
	{".ccad", "application/clariscad"}, 
	{".cco", "application/x-cocoa"}, 
	{".cdf", "application/cdf"}, 
	{".cer", "application/pkix-cert"}, 
	{".cha", "application/x-chat"}, 
	{".chat", "application/x-chat"}, 
	{".class", "application/java"}, 
	{".com", "application/octet-stream"}, 
	{".conf", "text/plain"}, 
	{".cpio", "application/x-cpio"}, 
	{".cpp", "text/x-c"}, 
	{".cpt", "application/x-cpt"}, 
	{".crl", "application/pkcs-crl"}, 
	{".crt", "application/pkix-cert"}, 
	{".csh", "application/x-csh"}, 
	{".css", "text/css"}, 
	{".cxx", "text/plain"}, 
	{".dcr", "application/x-director"}, 
	{".deepv", "application/x-deepv"}, 
	{".def", "text/plain"}, 
	{".der", "application/x-x509-ca-cert"}, 
	{".dif", "video/x-dv"}, 
	{".dir", "application/x-director"}, 
	{".dl", "video/dl"}, 
	{".doc", "application/msword"}, 
	{".dot", "application/msword"}, 
	{".dp", "application/commonground"}, 
	{".drw", "application/drafting"}, 
	{".dump", "application/octet-stream"}, 
	{".dv", "video/x-dv"}, 
	{".dvi", "application/x-dvi"}, 
	{".dwf", "model/vnd.dwf"}, 
	{".dwg", "image/vnd.dwg"}, 
	{".dxf", "image/vnd.dwg"}, 
	{".dxr", "application/x-director"}, 
	{".el", "text/x-script.elisp"}, 
	{".elc", "application/x-elc"}, 
	{".env", "application/x-envoy"}, 
	{".eps", "application/postscript"}, 
	{".es", "application/x-esrehber"}, 
	{".etx", "text/x-setext"}, 
	{".evy", "application/envoy"}, 
	{".exe", "application/octet-stream"}, 
	{".f", "text/plain"}, 
	{".f77", "text/x-fortran"}, 
	{".f90", "text/plain"}, 
	{".fdf", "application/vnd.fdf"}, 
	{".fif", "image/fif"}, 
	{".fli", "video/fli"}, 
	{".flo", "image/florian"}, 
	{".flx", "text/vnd.fmi.flexstor"}, 
	{".fmf", "video/x-atomic3d-feature"}, 
	{".for", "text/x-fortran"}, 
	{".fpx", "image/vnd.fpx"}, 
	{".frl", "application/freeloader"}, 
	{".funk", "audio/make"}, 
	{".g", "text/plain"}, 
	{".g3", "image/g3fax"}, 
	{".gif", "image/gif"}, 
	{".gl", "video/gl"}, 
	{".gsd", "audio/x-gsm"}, 
	{".gsm", "audio/x-gsm"}, 
	{".gsp", "application/x-gsp"}, 
	{".gss", "application/x-gss"}, 
	{".gtar", "application/x-gtar"}, 
	{".gz", "application/x-gzip"}, 
	{".gzip", "application/x-gzip"}, 
	{".h", "text/plain"}, 
	{".hdf", "application/x-hdf"}, 
	{".help", "application/x-helpfile"}, 
	{".hgl", "application/vnd.hp-hpgl"}, 
	{".hh", "text/plain"}, 
	{".hlb", "text/x-script"}, 
	{".hlp", "application/hlp"}, 
	{".hpg", "application/vnd.hp-hpgl"}, 
	{".hpgl", "application/vnd.hp-hpgl"}, 
	{".hqx", "application/binhex"}, 
	{".hta", "application/hta"}, 
	{".htc", "text/x-component"}, 
	{".htm", "text/html"}, 
	{".html", "text/html"}, 
	{".htmls", "text/html"}, 
	{".htt", "text/webviewhtml"}, 
	{".htx", "text/html"}, 
	{".ice", "x-conference/x-cooltalk"}, 
	{".ico", "image/x-icon"}, 
	{".idc", "text/plain"}, 
	{".ief", "image/ief"}, 
	{".iefs", "image/ief"}, 
	{".iges", "application/iges"}, 
	{".igs", "application/iges"}, 
	{".ima", "application/x-ima"}, 
	{".imap", "application/x-httpd-imap"}, 
	{".inf", "application/inf"}, 
	{".ins", "application/x-internett-signup"}, 
	{".ip", "application/x-ip2"}, 
	{".isu", "video/x-isvideo"}, 
	{".it", "audio/it"}, 
	{".iv", "application/x-inventor"}, 
	{".ivr", "i-world/i-vrml"}, 
	{".ivy", "application/x-livescreen"}, 
	{".jam", "audio/x-jam"}, 
	{".jav", "text/plain"}, 
	{".java", "text/plain"}, 
	{".jcm", "application/x-java-commerce"}, 
	{".jfif", "image/jpeg"}, 
	{".jfif-tbnl", "image/jpeg"}, 
	{".jpe", "image/jpeg"}, 
	{".jpeg", "image/jpeg"}, 
	{".jpg", "image/jpeg"}, 
	{".jps", "image/x-jps"}, 
	{".js", "application/x-javascript"}, 
	{".jut", "image/jutvision"}, 
	{".kar", "audio/midi"}, 
	{".ksh", "application/x-ksh"}, 
	{".la", "audio/nspaudio"}, 
	{".lam", "audio/x-liveaudio"}, 
	{".latex", "application/x-latex"}, 
	{".lha", "application/octet-stream"}, 
	{".lhx", "application/octet-stream"}, 
	{".list", "text/plain"}, 
	{".lma", "audio/nspaudio"}, 
	{".log", "text/plain"}, 
	{".lsp", "application/x-lisp"}, 
	{".lst", "text/plain"}, 
	{".lsx", "text/x-la-asf"}, 
	{".ltx", "application/x-latex"}, 
	{".lzh", "application/octet-stream"}, 
	{".lzx", "application/octet-stream"}, 
	{".m", "text/plain"}, 
	{".m1v", "video/mpeg"}, 
	{".m2a", "audio/mpeg"}, 
	{".m2v", "video/mpeg"}, 
	{".m3u", "audio/x-mpequrl"}, 
	{".man", "application/x-troff-man"}, 
	{".map", "application/x-navimap"}, 
	{".mar", "text/plain"}, 
	{".mbd", "application/mbedlet"}, 
	{".mc$", "application/x-magic-cap-package-1.0"}, 
	{".mcd", "application/mcad"}, 
	{".mcf", "text/mcf"}, 
	{".mcp", "application/netmc"}, 
	{".me", "application/x-troff-me"}, 
	{".mht", "message/rfc822"}, 
	{".mhtml", "message/rfc822"}, 
	{".mid", "audio/midi"}, 
	{".midi", "audio/midi"}, 
	{".mif", "application/x-mif"}, 
	{".mime", "message/rfc822"}, 
	{".mjf", "audio/x-vnd.audioexplosion.mjuicemediafile"}, 
	{".mjpg", "video/x-motion-jpeg"}, 
	{".mm", "application/base64"}, 
	{".mme", "application/base64"}, 
	{".mod", "audio/mod"}, 
	{".moov", "video/quicktime"}, 
	{".mov", "video/quicktime"}, 
	{".movie", "video/x-sgi-movie"}, 
	{".mp2", "audio/mpeg"}, 
	{".mp3", "audio/mpeg"}, 
	{".mpa", "audio/mpeg"}, 
	{".mpc", "application/x-project"}, 
	{".mpe", "video/mpeg"}, 
	{".mpeg", "video/mpeg"}, 
	{".mpg", "video/mpeg"}, 
	{".mpga", "audio/mpeg"}, 
	{".mpp", "application/vnd.ms-project"}, 
	{".mpt", "application/vnd.ms-project"}, 
	{".mpv", "application/vnd.ms-project"}, 
	{".mpx", "application/vnd.ms-project"}, 
	{".mrc", "application/marc"}, 
	{".ms", "application/x-troff-ms"}, 
	{".mv", "video/x-sgi-movie"}, 
	{".my", "audio/make"}, 
	{".mzz", "application/x-vnd.audioexplosion.mzz"}, 
	{".nap", "image/naplps"}, 
	{".naplps", "image/naplps"}, 
	{".nc", "application/x-netcdf"}, 
	{".ncm", "application/vnd.nokia.configuration-message"}, 
	{".nif", "image/x-niff"}, 
	{".niff", "image/x-niff"}, 
	{".nix", "application/x-mix-transfer"}, 
	{".nsc", "application/x-conference"}, 
	{".nvd", "application/x-navidoc"}, 
	{".o", "application/octet-stream"}, 
	{".oda", "application/oda"}, 
	{".omc", "application/x-omc"}, 
	{".omcd", "application/x-omcdatamaker"}, 
	{".omcr", "application/x-omcregerator"}, 
	{".p", "text/x-pascal"}, 
	{".p10", "application/pkcs10"}, 
	{".p12", "application/pkcs-12"}, 
	{".p7a", "application/x-pkcs7-signature"}, 
	{".p7c", "application/pkcs7-mime"}, 
	{".p7m", "application/pkcs7-mime"}, 
	{".p7r", "application/x-pkcs7-certreqresp"}, 
	{".p7s", "application/pkcs7-signature"}, 
	{".part", "application/pro_eng"}, 
	{".pas", "text/pascal"}, 
	{".pbm", "image/x-portable-bitmap"}, 
	{".pcl", "application/vnd.hp-pcl"}, 
	{".pct", "image/x-pict"}, 
	{".pcx", "image/x-pcx"}, 
	{".pdb", "chemical/x-pdb"}, 
	{".pdf", "application/pdf"}, 
	{".pfunk", "audio/make"}, 
	{".pgm", "image/x-portable-greymap"}, 
	{".pic", "image/pict"}, 
	{".pict", "image/pict"}, 
	{".pkg", "application/x-newton-compatible-pkg"}, 
	{".pko", "application/vnd.ms-pki.pko"}, 
	{".pl", "text/plain"}, 
	{".plx", "application/x-pixclscript"}, 
	{".pm", "image/x-xpixmap"}, 
	{".pm4", "application/x-pagemaker"}, 
	{".pm5", "application/x-pagemaker"}, 
	{".png", "image/png"}, 
	{".pnm", "application/x-portable-anymap"}, 
	{".pot", "application/vnd.ms-powerpoint"}, 
	{".pov", "model/x-pov"}, 
	{".ppa", "application/vnd.ms-powerpoint"}, 
	{".ppm", "image/x-portable-pixmap"}, 
	{".pps", "application/vnd.ms-powerpoint"}, 
	{".ppt", "application/vnd.ms-powerpoint"}, 
	{".ppz", "application/vnd.ms-powerpoint"}, 
	{".pre", "application/x-freelance"}, 
	{".prt", "application/pro_eng"}, 
	{".ps", "application/postscript"}, 
	{".psd", "application/octet-stream"}, 
	{".pvu", "paleovu/x-pv"}, 
	{".pwz", "application/vnd.ms-powerpoint"}, 
	{".py", "text/x-script.phyton"}, 
	{".pyc", "applicaiton/x-bytecode.python"}, 
	{".qcp", "audio/vnd.qcelp"}, 
	{".qd3", "x-world/x-3dmf"}, 
	{".qd3d", "x-world/x-3dmf"}, 
	{".qif", "image/x-quicktime"}, 
	{".qt", "video/quicktime"}, 
	{".qtc", "video/x-qtc"}, 
	{".qti", "image/x-quicktime"}, 
	{".qtif", "image/x-quicktime"}, 
	{".ra", "audio/x-pn-realaudio"}, 
	{".ram", "audio/x-pn-realaudio"}, 
	{".ras", "application/x-cmu-raster"}, 
	{".rast", "image/cmu-raster"}, 
	{".rexx", "text/x-script.rexx"}, 
	{".rf", "image/vnd.rn-realflash"}, 
	{".rgb", "image/x-rgb"}, 
	{".rm", "application/vnd.rn-realmedia"}, 
	{".rmi", "audio/mid"}, 
	{".rmm", "audio/x-pn-realaudio"}, 
	{".rmp", "audio/x-pn-realaudio"}, 
	{".rng", "application/ringing-tones"}, 
	{".rnx", "application/vnd.rn-realplayer"}, 
	{".roff", "application/x-troff"}, 
	{".rp", "image/vnd.rn-realpix"}, 
	{".rpm", "audio/x-pn-realaudio-plugin"}, 
	{".rt", "text/richtext"}, 
	{".rtf", "text/richtext"}, 
	{".rtx", "text/richtext"}, 
	{".rv", "video/vnd.rn-realvideo"}, 
	{".s", "text/x-asm"}, 
	{".s3m", "audio/s3m"}, 
	{".saveme", "application/octet-stream"}, 
	{".sbk", "application/x-tbook"}, 
	{".scm", "application/x-lotusscreencam"}, 
	{".sdml", "text/plain"}, 
	{".sdp", "application/sdp"}, 
	{".sdr", "application/sounder"}, 
	{".sea", "application/sea"}, 
	{".set", "application/set"}, 
	{".sgm", "text/sgml"}, 
	{".sgml", "text/sgml"}, 
	{".sh", "application/x-sh"}, 
	{".shar", "application/x-shar"}, 
	{".shtml", "text/html"}, 
	{".sid", "audio/x-psid"}, 
	{".sit", "application/x-sit"}, 
	{".skd", "application/x-koan"}, 
	{".skm", "application/x-koan"}, 
	{".skp", "application/x-koan"}, 
	{".skt", "application/x-koan"}, 
	{".sl", "application/x-seelogo"}, 
	{".smi", "application/smil"}, 
	{".smil", "application/smil"}, 
	{".snd", "audio/basic"}, 
	{".sol", "application/solids"}, 
	{".spc", "text/x-speech"}, 
	{".spl", "application/futuresplash"}, 
	{".spr", "application/x-sprite"}, 
	{".sprite", "application/x-sprite"}, 
	{".src", "application/x-wais-source"}, 
	{".ssi", "text/x-server-parsed-html"}, 
	{".ssm", "application/streamingmedia"}, 
	{".sst", "application/vnd.ms-pki.certstore"}, 
	{".step", "application/step"}, 
	{".stl", "application/sla"}, 
	{".stp", "application/step"}, 
	{".sv4cpio", "application/x-sv4cpio"}, 
	{".sv4crc", "application/x-sv4crc"}, 
	{".svf", "image/vnd.dwg"}, 
	{".svr", "application/x-world"}, 
	{".swf", "application/x-shockwave-flash"}, 
	{".t", "application/x-troff"}, 
	{".talk", "text/x-speech"}, 
	{".tar", "application/x-tar"}, 
	{".tbk", "application/toolbook"}, 
	{".tcl", "application/x-tcl"}, 
	{".tcsh", "text/x-script.tcsh"}, 
	{".tex", "application/x-tex"}, 
	{".texi", "application/x-texinfo"}, 
	{".texinfo", "application/x-texinfo"}, 
	{".text", "text/plain"}, 
	{".tgz", "application/x-compressed"}, 
	{".tif", "image/tiff"}, 
	{".tiff", "image/tiff"}, 
	{".tr", "application/x-troff"}, 
	{".tsi", "audio/tsp-audio"}, 
	{".tsp", "application/dsptype"}, 
	{".tsv", "text/tab-separated-values"}, 
	{".turbot", "image/florian"}, 
	{".txt", "text/plain"}, 
	{".uil", "text/x-uil"}, 
	{".uni", "text/uri-list"}, 
	{".unis", "text/uri-list"}, 
	{".unv", "application/i-deas"}, 
	{".uri", "text/uri-list"}, 
	{".uris", "text/uri-list"}, 
	{".ustar", "application/x-ustar"}, 
	{".uu", "application/octet-stream"}, 
	{".uue", "text/x-uuencode"}, 
	{".vcd", "application/x-cdlink"}, 
	{".vcs", "text/x-vcalendar"}, 
	{".vda", "application/vda"}, 
	{".vdo", "video/vdo"}, 
	{".vew", "application/groupwise"}, 
	{".viv", "video/vivo"}, 
	{".vivo", "video/vivo"}, 
	{".vmd", "application/vocaltec-media-desc"}, 
	{".vmf", "application/vocaltec-media-file"}, 
	{".voc", "audio/voc"}, 
	{".vos", "video/vosaic"}, 
	{".vox", "audio/voxware"}, 
	{".vqe", "audio/x-twinvq-plugin"}, 
	{".vqf", "audio/x-twinvq"}, 
	{".vql", "audio/x-twinvq-plugin"}, 
	{".vrml", "application/x-vrml"}, 
	{".vrt", "x-world/x-vrt"}, 
	{".vsd", "application/x-visio"}, 
	{".vst", "application/x-visio"}, 
	{".vsw", "application/x-visio"}, 
	{".w60", "application/wordperfect6.0"}, 
	{".w61", "application/wordperfect6.1"}, 
	{".w6w", "application/msword"}, 
	{".wav", "audio/wav"}, 
	{".wb1", "application/x-qpro"}, 
	{".wbmp", "image/vnd.wap.wbmp"}, 
	{".web", "application/vnd.xara"}, 
	{".wiz", "application/msword"}, 
	{".wk1", "application/x-123"}, 
	{".wmf", "windows/metafile"}, 
	{".wml", "text/vnd.wap.wml"}, 
	{".wmlc", "application/vnd.wap.wmlc"}, 
	{".wmls", "text/vnd.wap.wmlscript"}, 
	{".wmlsc", "application/vnd.wap.wmlscriptc"}, 
	{".word", "application/msword"}, 
	{".wp", "application/wordperfect"}, 
	{".wp5", "application/wordperfect"}, 
	{".wp6", "application/wordperfect"}, 
	{".wpd", "application/wordperfect"}, 
	{".wq1", "application/x-lotus"}, 
	{".wri", "application/mswrite"}, 
	{".wrl", "application/x-world"}, 
	{".wrz", "x-world/x-vrml"}, 
	{".wsc", "text/scriplet"}, 
	{".wsrc", "application/x-wais-source"}, 
	{".wtk", "application/x-wintalk"}, 
	{".xbm", "image/x-xbitmap"}, 
	{".xdr", "video/x-amt-demorun"}, 
	{".xgz", "xgl/drawing"}, 
	{".xif", "image/vnd.xiff"}, 
	{".xl", "application/excel"}, 
	{".xla", "application/vnd.ms-excel"}, 
	{".xlb", "application/vnd.ms-excel"}, 
	{".xlc", "application/vnd.ms-excel"}, 
	{".xld", "application/vnd.ms-excel"}, 
	{".xlk", "application/vnd.ms-excel"}, 
	{".xll", "application/vnd.ms-excel"}, 
	{".xlm", "application/vnd.ms-excel"}, 
	{".xls", "application/vnd.ms-excel"}, 
	{".xlt", "application/vnd.ms-excel"}, 
	{".xlv", "application/vnd.ms-excel"}, 
	{".xlw", "application/vnd.ms-excel"}, 
	{".xm", "audio/xm"}, 
	{".xml", "application/xml"}, 
	{".xmz", "xgl/movie"}, 
	{".xpix", "application/x-vnd.ls-xpix"}, 
	{".xpm", "image/xpm"}, 
	{".x-png", "image/png"}, 
	{".xsr", "video/x-amt-showrun"}, 
	{".xwd", "image/x-xwd"}, 
	{".xyz", "chemical/x-pdb"}, 
	{".z", "application/x-compressed"}, 
	{".zip", "application/zip"}, 
	{".zoo", "application/octet-stream"}, 
	{".zsh", "text/x-script.zsh"}	
};

#define DEFAULT_MIME "application/octet-stream";

char* getMimeType(char* in_filename)
{
	//获取后缀名，后缀最长为.abcd 5个字节
	char* file_suffix = in_filename + strlen(in_filename) - 5;	
	file_suffix = strstr(file_suffix, ".");
	
	for(int i = 0; i < sizeof(mimetype)/sizeof(mimetype[0]); i++)
	{
		if(0 == strcmp(file_suffix, mimetype[i][0]))
		{
			return mimetype[i][1];
		}
	}
	return DEFAULT_MIME;
}

void getPostBoundary(char* boundary, int boundary_len)
{
	if(boundary_len < HTTP_POSTPARAM_BROUNDARY_FIXLEN+1)
	{
		boundary[0] = '\0';	
	}
	else
	{
		time_t cur_time = time(NULL);
		strncpy(boundary, __BROUNDARY, HTTP_POSTPARAM_BROUNDARY_FIXLEN);
		//int time_len = 16;
		snprintf(boundary+HTTP_POSTPARAM_BROUNDARY_FIXLEN-16,
				16+1,
				"%016x", cur_time);
		
		boundary[HTTP_POSTPARAM_BROUNDARY_FIXLEN] = '\0';
	}
	return;
}



