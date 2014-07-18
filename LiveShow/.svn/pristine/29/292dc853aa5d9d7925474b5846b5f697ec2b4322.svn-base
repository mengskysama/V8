#pragma  once

#include "51ktvtype.h"

namespace netlayer
{
	class ConnectionLayerImpl;

	template <typename T>
	class Sock5Proxy : public T  	
	{
// SOCKS Cmd Code
#define SOCKS_CMDS_CONNECT      ((unsigned char)0x01)
#define SOCKS_CMDS_BIND         ((unsigned char)0x02)
#define SOCKS_CMDS_UDP          ((unsigned char)0x03)	// (SOCKS5 ONLY)
#define SOCKS_CMDS_UNKNOWN      ((unsigned char)0xFF)

// SOCKS Version
#define SOCKS_VERS_4            ((unsigned char)0x04)
#define SOCKS_VERS_4A           ((unsigned char)0x04)
#define SOCKS_VERS_5            ((unsigned char)0x05)
#define SOCKS_VERS_UNKNOWN      ((unsigned char)0xFF)

// SOCK5 auth SOCKS5 ONLY)
#define SOCKS_AUTH_NONE         ((unsigned char)0x00)
#define SOCKS_AUTH_GSSAPI       ((unsigned char)0x01)
#define SOCKS_AUTH_PASSWD       ((unsigned char)0x02)
#define SOCKS_AUTH_CHAP         ((unsigned char)0x03)
#define SOCKS_AUTH_UNKNOWN      ((unsigned char)0xFF)

// SOCKS Request Packet Type
#define SOCKS_PACK_REQUEST      ((int)0x00)
#define SOCKS_PACK_4REQ         ((int)0x01)
#define SOCKS_PACK_4AREQ        ((int)0x02)
#define SOCKS_PACK_5REQ         ((int)0x03)
#define SOCKS_PACK_5AUTHREQ     ((int)0x04)
#define SOCKS_PACK_5PASSREQ     ((int)0x05)

// SOCK5 (SOCKS5 ONLY)
#define SOCKS_ADDR_IPV4ADDR     ((unsigned char)0x01)
#define SOCKS_ADDR_HOSTNAME     ((unsigned char)0x03)
#define SOCKS_ADDR_IPV6ADDR     ((unsigned char)0x04)
#define SOCKS_ADDR_UNKNOWN      ((unsigned int)0x10000000)

	public:
		Sock5Proxy(ConnectionLayerImpl& oCommMgr, Packet::HEADTYPE enmRecvHeadType)
			: T(oCommMgr, enmRecvHeadType),m_nAuths(SOCKS_AUTH_UNKNOWN)
		{

		}
		virtual ~Sock5Proxy()
		{
		}	

		virtual bool Create(const IpPort& oAddr, 
							const IpPort& oProxyAddr, 
							const InnerUtility::String& strName, 
							const InnerUtility::String& strPwd,
							const InnerUtility::String& strDomain = "")
		{
			//保存信息
			m_oDstAddr = oAddr;
			m_strName = strName;
			m_strPwd = strPwd;
			//创建SOCKET
			return T::Create(oProxyAddr);
		}

		virtual void OnRead(SOCKET oSock)
		{
			if(SOCK5VERSION == m_enState) //收到版本的应答
			{
				RecvAll(m_oRecvData);

				if(m_oRecvData.size() >= 2)
				{
					m_nAuths = (uint8)m_oRecvData[1];

					m_enState = SOCK5AUTH;

					m_oRecvData.clear();

					OnWrite(m_oSock);
				}
			}
			else if(SOCK5AUTHSENT == m_enState)
			{
				RecvAll(m_oRecvData);

				if(m_oRecvData.size() >= 2)
				{
					if((uint8)m_oRecvData[1] == 0)
					{
						m_enState = SOCK5AUTHOK;

						m_oRecvData.clear();

						OnWrite(m_oSock);
					}
					else
					{
						//用户名密码错误
						Notify(COMM_CLOSE_NEED_PS, TCP_COMM );
						Close();
					}
				}
			}
			else if (SOCK5IP == m_enState)
			{
				RecvAll(m_oRecvData);

				if(m_oRecvData.size() >= 4) //收到数据
				{
					if(m_oRecvData[0] == SOCKS_VERS_5 && m_oRecvData[1] == 0 && m_oRecvData[2] == 0
						&& (m_oRecvData[3] == SOCKS_ADDR_IPV4ADDR || m_oRecvData[3] == SOCKS_ADDR_HOSTNAME || 
						m_oRecvData[3] == SOCKS_ADDR_IPV6ADDR))
					{
						m_enState = CONNECTED;
						Notify(COMM_CONNECT, TCP_COMM);

						m_oRecvData.clear();
					}
					else
					{
						//出错
						return;
					}
				}
			}
			else
			{
				T::OnRead(oSock);
			}			
		}

		void SendIPInfo()
		{
			unsigned int unIP = m_oDstAddr.GetIp();

			if(unIP == 0)
			{
				unIP = inet_addr(m_oDstAddr.GetIpStr().c_str());
			}

			char pSendBufs[12];

			pSendBufs[0] = SOCKS_VERS_5;
			pSendBufs[1] = SOCKS_CMDS_CONNECT;
			pSendBufs[2] = 0;
			pSendBufs[3] = SOCKS_ADDR_IPV4ADDR;
			pSendBufs[4] = (char)(unIP & 0x000000ff);
			pSendBufs[5] = (char)((unIP >> 8 ) &0x000000ff);
			pSendBufs[6] = (char)((unIP >> 16) &0x000000ff);
			pSendBufs[7] = (char)((unIP >> 24) &0x000000ff);
			pSendBufs[8] = m_oDstAddr.GetPort() & 0x00ff;
			pSendBufs[9] = (m_oDstAddr.GetPort() >> 8) & 0x00ff;
			pSendBufs[10] = 0;

			nethelper::Send(m_oSock, pSendBufs, 10);
		}

		virtual void OnWrite(SOCKET oSock)
		{
			if (CONNECTING == m_enState) //连接代理服务器成功
			{
				RemoveFromWrite(m_oSock, this);
				AddToRead(m_oSock, this);
				
				char pSendBufs[3];

				pSendBufs[0] = SOCKS_VERS_5;
				pSendBufs[1] = 1;
			
				//如果存在用户名和密码
				if (0 != m_strName.length() && 0 != m_strPwd.length())
				{
					pSendBufs[2] = SOCKS_AUTH_PASSWD;
				}
				else
				{
					pSendBufs[2] = SOCKS_AUTH_NONE;
				}

				nethelper::Send(m_oSock, pSendBufs, 3);

				m_enState = SOCK5VERSION;
			}
			else if(SOCK5AUTH == m_enState)
			{
				RemoveFromWrite(m_oSock, this);
				AddToRead(m_oSock, this);

				if(m_nAuths == SOCKS_AUTH_NONE) //无需认证，直接发代理请求
				{
					SendIPInfo();

					m_enState = SOCK5IP;
				}
				else if(m_nAuths == SOCKS_AUTH_PASSWD)//以用户名，密码认证，发送认证信息，等待响应
				{
					if (0 != m_strName.length() && 0 != m_strPwd.length())
					{
						char* pSendBufs = (char*)malloc(5 + m_strName.length() + m_strPwd.length());

						pSendBufs[0] = 0x01;
						pSendBufs[1] = (uint8)m_strName.length();
						int i = 0;
						for(; i < m_strName.length(); ++i)
						{
							pSendBufs[i+2] = m_strName[i];
						}
						pSendBufs[i+2] = (uint8)m_strPwd.length();
						int j = 0;
						for(; j < m_strPwd.length(); ++j)
						{
							pSendBufs[i+3+j] = m_strPwd[j];
						}
						pSendBufs[i+3+j] = 0;

						nethelper::Send(m_oSock, pSendBufs, i+3+j);

						free(pSendBufs);

						m_enState = SOCK5AUTHSENT;
					}
					else
					{
						//需要密码
						Notify(COMM_CLOSE_NEED_PS, TCP_COMM );
						Close();
					}
				}
				else
				{
					//其他方式，暂不支持
				}
			}
			else if(SOCK5AUTHOK == m_enState)
			{
				SendIPInfo();

				m_enState = SOCK5IP;
			}
			else
			{
				T::OnWrite(oSock);
			}
		}

	private:

		int m_nAuths;//认证方式
		IpPort m_oDstAddr;	//对方的地址
		InnerUtility::String m_strName; //用户名
		InnerUtility::String m_strPwd;	//密码
	};
};