/**
* httpproxy.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#define SECURITY_WIN32
#include <sspi.h>

namespace netlayer
{
class ConnectionLayerImpl;
//http代理，只支持tcp

template <typename T>
class HttpProxy : public T  
{
	enum {
		BASE64_FLAG_NONE = 0,
		BASE64_FLAG_NOPAD = 1,
		BASE64_FLAG_NOCRLF = 2
	};
	enum  HTTP_PROXY_CONNECT_RE
	{
		HTTPPROXY_AUTHOK = 200,
		HTTPPROXY_NEED_PS = 407,
	};

public:
	HttpProxy(ConnectionLayerImpl& oCommMgr, Packet::HEADTYPE enmRecvHeadType)
		: T(oCommMgr, enmRecvHeadType)
	{
	}
	virtual ~HttpProxy()
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
		m_strDomain = strDomain;
		//创建SOCKET
		return T::Create(oProxyAddr);
	}

	virtual void OnRead(SOCKET oSock)
	{
		if (HTTPAUTH == m_enState || HTTPNTLMAUTH1 == m_enState || HTTPNTLMAUTH2 == m_enState) //收到认证的应答
		{
			if(m_enState == HTTPAUTH)
			{
				OutputDebugString(L"plushu OnRead HTTPAUTH");
			}
			else if(m_enState == HTTPNTLMAUTH1)
			{
				OutputDebugString(L"plushu OnRead HTTPNTLMAUTH1");
			}
			else if(m_enState == HTTPNTLMAUTH2)
			{
				OutputDebugString(L"plushu OnRead HTTPNTLMAUTH2");
			}

			RecvAll(m_oRecvData);
			if (m_oRecvData.size() > 0) //收到数据
			{
				m_oRecvData.push_back(0);				
				const char * p = strstr(reinterpret_cast<const char *>(&m_oRecvData[0]), "\r\n\r\n");
				if (p) //已经收到结束符
				{
					HTTP_PROXY_CONNECT_RE re=(HTTP_PROXY_CONNECT_RE)GetReplyCode(reinterpret_cast<char *>(&m_oRecvData[0])) ;
					if (re == HTTPPROXY_AUTHOK || (re == HTTPPROXY_NEED_PS && HTTPNTLMAUTH1 == m_enState))
					{
						if(HTTPAUTH == m_enState || HTTPNTLMAUTH2 == m_enState)
						{
							OutputDebugString(L"plushu OnRead HTTPAUTH or HTTPNTLMAUTH2 COMM_CONNECT");

							m_enState = CONNECTED;
							Notify(COMM_CONNECT, TCP_COMM);
						}
						else if(HTTPNTLMAUTH1 == m_enState)
						{
							OutputDebugString(L"plushu OnRead HTTPNTLMAUTH1");

							char szEncode[1024] = {0};
							int  nLength = 0;

							memset(szEncode, 0, sizeof(szEncode));
							nLength = sizeof(szEncode);

							const char *pCode = reinterpret_cast<const char *>(&m_oRecvData[0]);

							if(m_oRecvData.size() > (strlen(pCode)+1))
							{
								pCode += (strlen(pCode)+1);
							}

							if(NtlmStep2((char*)pCode, szEncode, &nLength))
							{
								OutputDebugString(L"plushu OnRead NtlmStep2");

								m_enState = HTTPNTLMAUTH2;

								sockaddr_in oDstAdr = nethelper::IpPortToAddr(m_oDstAddr.GetIp(), m_oDstAddr.GetPort());
								InnerUtility::OStringStream oss;

								oss << "CONNECT " << m_oDstAddr.GetIpStr().c_str() << ":" << ntohs(oDstAdr.sin_port) << " HTTP/1.1\r\n";
								oss << "Host: " << m_oDstAddr.GetIpStr().c_str() << ":" << ntohs(oDstAdr.sin_port) << "\r\n";
								oss << "Proxy-Authorization: NTLM " << szEncode << "\r\n";
								oss << "Accept: */*\r\n"
									<< "Content-Type: text/html\r\n"
									<< "Proxy-Connection: Keep-Alive\r\n"
									<< "Content-length: 0\r\n\r\n";
								nethelper::Send(m_oSock, oss.str().c_str(), oss.str().length());
							}
						}
					}
					else if (re== HTTPPROXY_NEED_PS)
					{
						OutputDebugString(L"plushu OnRead HTTPPROXY_NEED_PS");

						const char *pCode = reinterpret_cast<const char *>(&m_oRecvData[0]);

						if(m_oRecvData.size() > (strlen(pCode)+1))
						{
							pCode += (strlen(pCode)+1);
						}

						if(strstr(pCode, "Proxy-Authenticate: NTLM") != NULL ||
							HTTPNTLMAUTH1 == m_enState || HTTPNTLMAUTH2 == m_enState)
						{
							OutputDebugString(L"plushu OnRead HTTPPROXY_NEED_PS 1");

							Close();
						}
						else
						{
							OutputDebugString(L"plushu OnRead HTTPPROXY_NEED_PS 2");

							//如果需要密码，结束当前连接，通知上层重新来过
							Notify(COMM_CLOSE_NEED_PS, TCP_COMM );
							Close();
						}
					}
					m_oRecvData.clear();
				}
				else
				{
					OutputDebugString(L"plushu OnRead 未收到结束符");

					m_oRecvData.erase(m_oRecvData.end() - 1);
				}
			}
			else
			{
				OutputDebugString(L"plushu OnRead 0");
			}
		}
		else
		{
			T::OnRead(oSock);
		}
	}

	bool NtlmInit()
	{
		typedef PSecurityFunctionTableA(*PInitSecurityInterface)(void);

		m_hInstSecuLib = LoadLibraryA("Security.dll");
		if(!m_hInstSecuLib)
		{
			return false;
		}

		PInitSecurityInterface pInitInterface = (PInitSecurityInterface)GetProcAddress( m_hInstSecuLib, "InitSecurityInterfaceA");
		if(!pInitInterface)
		{
			return false;
		}

		m_pFunTable = pInitInterface();
		if(!m_pFunTable)
		{
			return false;
		}

		SECURITY_STATUS SecurityStatus = SEC_E_OK;

		SEC_WINNT_AUTH_IDENTITY_A AuthData;
		memset(&AuthData, 0, sizeof(AuthData));
		
		AuthData.Flags = SEC_WINNT_AUTH_IDENTITY_ANSI;
		AuthData.User = (BYTE*)m_strName.c_str();
		AuthData.UserLength = m_strName.size();
		AuthData.Domain = (BYTE*)m_strDomain.c_str();
		AuthData.DomainLength = m_strDomain.size();
		AuthData.Password = (BYTE*)m_strPwd.c_str();
		AuthData.PasswordLength = m_strPwd.size();

		SecurityStatus = m_pFunTable->AcquireCredentialsHandleA(
			0,
			"NTLM",
			SECPKG_CRED_OUTBOUND,
			0,
			&AuthData,
			0,
			0,
			&m_hCredentials,
			&m_ts
			);

		return SecurityStatus == SEC_E_OK;
	}

	bool NtlmStep1(char* pOutBuffer, int* pOutBufferLen)
	{
		if(!m_pFunTable)
		{
			return false;
		}

		SECURITY_STATUS SecurityStatus = SEC_E_OK;

		BYTE Buffer[1024];
		memset(Buffer,0,1024);

		SecBuffer sBuffer;
		sBuffer.cbBuffer = 1024;
		sBuffer.BufferType = SECBUFFER_TOKEN;
		sBuffer.pvBuffer = Buffer;

		SecBufferDesc OutBufferDesc;
		OutBufferDesc.ulVersion = SECBUFFER_VERSION;
		OutBufferDesc.cBuffers = 1;
		OutBufferDesc.pBuffers = &sBuffer;

		SecurityStatus = m_pFunTable->InitializeSecurityContextA(
			&m_hCredentials,
			0,
			"NTLM",
			ISC_REQ_USE_DCE_STYLE | ISC_REQ_DELEGATE |
			ISC_REQ_MUTUAL_AUTH |ISC_REQ_REPLAY_DETECT |
			ISC_REQ_SEQUENCE_DETECT |ISC_REQ_CONFIDENTIALITY |
			ISC_REQ_CONNECTION,
			0,
			0,
			0,
			0,
			&m_SecurityContext,
			&OutBufferDesc,
			&m_ulContextAttributes,
			&m_ts
			);

		if (IS_ERROR(SecurityStatus))
			return false;

		Base64Encode((unsigned char*)sBuffer.pvBuffer, sBuffer.cbBuffer, pOutBuffer, pOutBufferLen, BASE64_FLAG_NOCRLF);

		return true;
	}

	bool NtlmStep2(char* szHttpReply, char* pOutBuffer, int* pOutBufferLen)
	{
		if(!m_pFunTable)
		{
			return false;
		}

		char* token = "Proxy-Authenticate: NTLM ";
		char* p = strstr(szHttpReply,token);
		char* pCode = NULL;
		bool b = false;

		//从Http头中取出Challenge
		if( p )
		{
			p += strlen(token);
			pCode = p;
			while( *p )
			{
				if(*p==' ' || *p=='\r' || *p=='\n')
				{
					*p = 0;
					b = true;
					break;
				}
				p ++;
			}
		}

		if(!b)
		{
			return false;
		}

		BYTE InBuffer[1024];
		BYTE OutBuffer[1024];
		memset(InBuffer,0,1024);
		memset(OutBuffer,0,1024);

		int nLength = 1024;
		Base64Decode(pCode, strlen(pCode),InBuffer, &nLength);

		SECURITY_STATUS SecurityStatus = SEC_E_OK;

		SecBuffer sInBuffer;
		sInBuffer.cbBuffer = nLength;
		sInBuffer.BufferType = SECBUFFER_TOKEN;
		sInBuffer.pvBuffer = InBuffer;
		SecBufferDesc InBufferDesc;
		InBufferDesc.ulVersion = SECBUFFER_VERSION;
		InBufferDesc.cBuffers = 1;
		InBufferDesc.pBuffers = &sInBuffer;

		SecBuffer sOutBuffer;
		sOutBuffer.cbBuffer = 1024;
		sOutBuffer.BufferType = SECBUFFER_TOKEN;
		sOutBuffer.pvBuffer = OutBuffer;
		SecBufferDesc OutBufferDesc;
		OutBufferDesc.ulVersion = SECBUFFER_VERSION;
		OutBufferDesc.cBuffers = 1;
		OutBufferDesc.pBuffers = &sOutBuffer;

		SecurityStatus = m_pFunTable->InitializeSecurityContextA(
			0,
			&m_SecurityContext,
			"NTLM",
			0,
			0,
			0 ,
			&InBufferDesc,
			0,
			&m_SecurityContext,
			&OutBufferDesc,
			&m_ulContextAttributes,
			&m_ts
			);

		if (IS_ERROR(SecurityStatus))
		{
			return false;
		}

		Base64Encode((unsigned char*)sOutBuffer.pvBuffer, sOutBuffer.cbBuffer, pOutBuffer, pOutBufferLen, BASE64_FLAG_NOCRLF);

		return true;
	}

	virtual void OnWrite(SOCKET oSock)
	{
		if (CONNECTING == m_enState) //连接代理服务器成功
		{
			if(0 == m_strDomain.length())
			{
				m_enState = HTTPAUTH;
				RemoveFromWrite(m_oSock, this);
				AddToRead(m_oSock, this);

				sockaddr_in oDstAdr = nethelper::IpPortToAddr(m_oDstAddr.GetIp(), m_oDstAddr.GetPort()) ;
				InnerUtility::OStringStream oss;
				/*oss << "CONNECT " << inet_ntoa(oDstAdr.sin_addr) << ":" << ntohs(oDstAdr.sin_port) << " HTTP/1.1\r\n";*/
				//  [11/6/2008 ken]
				oss << "CONNECT " << m_oDstAddr.GetIpStr().c_str() << ":" << ntohs(oDstAdr.sin_port) << " HTTP/1.1\r\n";
				if (0 != m_strName.length())
				{
					//需要身份验证
					char Encode[1024] = {0};
					int  nLength = sizeof(Encode) - 1;
					InnerUtility::OStringStream ossName;
					ossName << m_strName << ":" << m_strPwd;
					Base64Encode((unsigned char*)ossName.str().c_str(), ossName.str().length(), Encode, &nLength, BASE64_FLAG_NOCRLF);
					oss << "Proxy-Authorization: Basic " << Encode << "\r\n";
				}
				oss << "Accept: */*\r\n"
					<< "Content-Type: text/html\r\n"
					<< "Proxy-Connection: Keep-Alive\r\n"
					<< "Content-length: 0\r\n\r\n";
				nethelper::Send(m_oSock, oss.str().c_str(), oss.str().length());
			}
			else
			{
				OutputDebugString(L"plushu OnWrite");

				RemoveFromWrite(m_oSock, this);
				AddToRead(m_oSock, this);

				if(NtlmInit())
				{
					OutputDebugString(L"plushu OnWrite NtlmInit");

					char szEncode[1024] = {0};
					int  nLength = 0;

					memset(szEncode, 0, sizeof(szEncode));
					nLength = sizeof(szEncode);

					if(NtlmStep1(szEncode, &nLength))
					{
						OutputDebugString(L"plushu OnWrite NtlmStep1");

						m_enState = HTTPNTLMAUTH1;

						sockaddr_in oDstAdr = nethelper::IpPortToAddr(m_oDstAddr.GetIp(), m_oDstAddr.GetPort());
						InnerUtility::OStringStream oss;

						oss << "CONNECT " << m_oDstAddr.GetIpStr().c_str() << ":" << ntohs(oDstAdr.sin_port) << " HTTP/1.1\r\n";
						oss << "Host: " << m_oDstAddr.GetIpStr().c_str() << ":" << ntohs(oDstAdr.sin_port) << "\r\n";
						oss << "Proxy-Authorization: NTLM " << szEncode << "\r\n";
						oss << "Accept: */*\r\n"
							<< "Content-Type: text/html\r\n"
							<< "Proxy-Connection: Keep-Alive\r\n"
							<< "Content-length: 0\r\n\r\n";
						nethelper::Send(m_oSock, oss.str().c_str(), oss.str().length());
					}
				}
			}
		}
		else
		{
			T::OnWrite(oSock);
		}
	}

private:
	static int Base64Encode(const unsigned char *pbSrcData, int nSrcLen, char* szDest, int *pnDestLen, unsigned int dwFlags)
	{
		static const char s_chBase64EncodingTable[64] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
			'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',	'h',
			'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
			'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

			int nWritten = 0;
			int nLen1 = (nSrcLen/3)*4 ;
			int nLen2 = nLen1/76;
			int nLen3 = 19;
			int i = 0;
			int j = 0;
			int n = 0;
			int k = 0;

			if (!pbSrcData || !szDest || !pnDestLen)
			{
				return 0;
			}

			for (i=0; i<=nLen2; i++)
			{
				if (i==nLen2)
					nLen3 = (nLen1%76)/4;

				for (j=0; j<nLen3; j++)
				{
					unsigned int dwCurr = 0;
					for (n=0; n<3; n++)
					{
						dwCurr |= *pbSrcData++;
						dwCurr <<= 8;
					}
					for (k=0; k<4; k++)
					{
						unsigned char b = (unsigned char)(dwCurr>>26);
						*szDest++ = s_chBase64EncodingTable[b];
						dwCurr <<= 6;
					}
				}
				nWritten+= nLen3*4;

				if ((dwFlags & BASE64_FLAG_NOCRLF)==0)
				{
					*szDest++ = '\r';
					*szDest++ = '\n';
					nWritten+= 2;
				}
			}

			if (nWritten && (dwFlags & BASE64_FLAG_NOCRLF)==0)
			{
				szDest-= 2;
				nWritten -= 2;
			}

			nLen2 = nSrcLen%3 ? nSrcLen%3 + 1 : 0;
			if (nLen2)
			{
				unsigned int dwCurr = 0;
				for (n=0; n<3; n++)
				{
					if (n<(nSrcLen%3))
						dwCurr |= *pbSrcData++;
					dwCurr <<= 8;
				}
				for (k=0; k<nLen2; k++)
				{
					unsigned char b = (unsigned char)(dwCurr>>26);
					*szDest++ = s_chBase64EncodingTable[b];
					dwCurr <<= 6;
				}
				nWritten+= nLen2;
				if ((dwFlags & BASE64_FLAG_NOPAD)==0)
				{
					nLen3 = nLen2 ? 4-nLen2 : 0;
					for (j=0; j<nLen3; j++)
					{
						*szDest++ = '=';
					}
					nWritten += nLen3;
				}
			}

			*pnDestLen = nWritten;

			return 1;
	}

	static int DecodeBase64Char(unsigned int ch)
	{
		// returns -1 if the character is invalid
		// or should be skipped
		// otherwise, returns the 6-bit code for the character
		// from the encoding table
		if (ch >= 'A' && ch <= 'Z')
			return ch - 'A' + 0;	// 0 range starts at 'A'
		if (ch >= 'a' && ch <= 'z')
			return ch - 'a' + 26;	// 26 range starts at 'a'
		if (ch >= '0' && ch <= '9')
			return ch - '0' + 52;	// 52 range starts at '0'
		if (ch == '+')
			return 62;
		if (ch == '/')
			return 63;
		return -1;
	}

	static bool Base64Decode(LPCSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)
	{
		// walk the source buffer
		// each four character sequence is converted to 3 bytes
		// CRLFs and =, and any characters not in the encoding table
		// are skiped

		if (!szSrc || !pbDest || !pnDestLen)
		{
			return false;
		}

		LPCSTR szSrcEnd = szSrc + nSrcLen;
		int nWritten = 0;
		while (szSrc < szSrcEnd)
		{
			DWORD dwCurr = 0;
			int i;
			int nBits = 0;
			for (i=0; i<4; i++)
			{
				if (szSrc >= szSrcEnd)
					break;
				int nCh = DecodeBase64Char(*szSrc);
				szSrc++;
				if (nCh == -1)
				{
					// skip this char
					i--;
					continue;
				}
				dwCurr <<= 6;
				dwCurr |= nCh;
				nBits += 6;
			}
			// dwCurr has the 3 bytes to write to the output buffer
			// left to right
			dwCurr <<= 24-nBits;
			for (i=0; i<nBits/8; i++)
			{
				*pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
				dwCurr <<= 8;
				pbDest++;
				nWritten++;
			}
		}

		*pnDestLen = nWritten;
		return true;
	}

	static int GetReplyCode(char *szHttpReply)
	{
		int n = 0;
		char* p = szHttpReply;
		char* pCode = NULL;
		while(*p)
		{
			if(*p == ' ')
			{
				p ++;
				pCode = p;
				while(*p)
				{
					if(*p == ' '|| *p == '\r' || *p == '\n')
					{
						*p = 0;
						break;
					}
					p ++;
				}
				n = atoi(pCode);
				break;
			}
			p ++;
		}
		return n;
	}

private:
	IpPort m_oDstAddr;	//对方的地址
	InnerUtility::String m_strName; //用户名
	InnerUtility::String m_strPwd;	//密码
	InnerUtility::String m_strDomain;	//域
	HINSTANCE m_hInstSecuLib;
	SecurityFunctionTableA* m_pFunTable;
	CredHandle m_hCredentials;
	CtxtHandle m_SecurityContext;
	TimeStamp m_ts;
	ULONG m_ulContextAttributes;
};
};