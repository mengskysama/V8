//#include <winnt.h>
//#include <basetsd.h>
//#include <windef.h>
#include "..\include\utils.h"
#include <winbase.h>
#include <sstream>

#include "ntlmproxy.h"


NtlmProxy::~NtlmProxy()
{

}

bool NtlmProxy::NtlmInit()
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
	AuthData.UserLength = (ULONG)m_strName.size();
	AuthData.Domain = (BYTE*)m_strDomain.c_str();
	AuthData.DomainLength = (ULONG)m_strDomain.size();
	AuthData.Password = (BYTE*)m_strPwd.c_str();
	AuthData.PasswordLength =(ULONG)m_strPwd.size();

	SecurityStatus = m_pFunTable->AcquireCredentialsHandleA(
		0,
		"Negotiate",
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

bool NtlmProxy::NtlmStep1(char* pOutBuffer, int* pOutBufferLen)
{
	memset(pOutBuffer, 0, *pOutBufferLen);
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

bool NtlmProxy::NtlmStep2(const char* authenticate_str, char* pOutBuffer, int* pOutBufferLen)
{
	char temp_auth_str[1024] = {0};
	strncpy(temp_auth_str, authenticate_str, sizeof(temp_auth_str)-1);
	if(!m_pFunTable)
	{
		return false;
	}

	char* token = "Negotiate ";
	char* pCode = strstr(temp_auth_str, token) + strlen(token);

	unsigned char InBuffer[1024];
	unsigned char OutBuffer[1024];
	memset(InBuffer,0,1024);
	memset(OutBuffer,0,1024);

	int nLength = 1024;
	Base64Decode(pCode, (int)strlen(pCode),InBuffer, &nLength);

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

void NtlmProxy::genAuthStep1Pack(const char* szEncode)
{
	sending_data.clear();
	
	stringstream oss;
	
	oss << "CONNECT " << dsthost.c_str() << ":" << dstport << " HTTP/1.1\r\n";
	oss << "Host: " << dsthost.c_str() << ":" << dstport << "\r\n";
	oss << "Proxy-Authorization: NTLM " << szEncode << "\r\n";
	oss << "Accept: */*\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Proxy-Connection: Keep-Alive\r\n"
		<< "Content-length: 0\r\n\r\n";
	
	for(int i = 0; i<(int)oss.str().length(); i++)
	{
		sending_data.push_back(oss.str().c_str()[i]);
	}
	return;
}

void NtlmProxy::genAuthStep2Pack(const char* szEncode)
{
	sending_data.clear();
	
	stringstream oss;
	
	oss << "CONNECT " << ipaddr.c_str() << ":" << ntohs(port) << " HTTP/1.1\r\n";
	oss << "Host: " << dsthost.c_str() << ":" << ntohs(dstport) << "\r\n";
	oss << "Proxy-Authorization: NTLM " << szEncode << "\r\n";
	oss << "Accept: */*\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Proxy-Connection: Keep-Alive\r\n"
		<< "Content-length: 0\r\n\r\n";
	
	for(int i = 0; i<(int)oss.str().length(); i++)
	{
		sending_data.push_back(oss.str().c_str()[i]);
	}
	return;
}

int NtlmProxy::GetReplyCode(char *szHttpReply)
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

