#ifndef _NTLMPROXY_H
#define _NTLMPROXY_H
#include <string>

#ifndef SECURITY_WIN32	//Need for sspi.h
#define SECURITY_WIN32
#endif
#include <sspi.h>

#include "..\include\httptransfer.h"

using namespace std;

typedef enum
{
	NTLM_AUTH_DEFAULT = 0,
	NTLM_AUTH_INIT = 1,
	NTLM_AUTH_SETP1 = 2,
	NTLM_AUTH_SENT_AUTH_SETP1_DATA = 3,
	NTLM_AUTH_SETP2 = 4,
	NTLM_AUTH_SENT_AUTH_SETP2_DATA = 5,
	NTLM_AUTH_FINI = 6,
	NTLM_AUTH_ERR = 7,
	NTLM_AUTH_BUTT = 255,
}NTLM_PROXY_STATUS;


class NtlmProxy
{
public:
	NtlmProxy(string in_ipaddr, int in_port, string in_domain, string in_user, string in_pwd,
					string in_dst_host, int in_dst_port)
	{
		ipaddr = in_ipaddr;
		port = in_port;
		m_strDomain = in_domain;
		m_strName = in_user;
		m_strPwd = in_pwd;
		dsthost = in_dst_host;
		dstport = in_dst_port;
	};
	
	~NtlmProxy();
	bool NtlmInit();
	bool NtlmStep1(char* pOutBuffer, int* pOutBufferLen);
	bool NtlmStep2(const char* szHttpReply, char* pOutBuffer, int* pOutBufferLen);
	void genAuthStep1Pack(const char* szEncode);
	void genAuthStep2Pack(const char* szEncode);
	int GetReplyCode(char *szHttpReply);
	void setStep1NegotiateStr(char* step1Negotiate) {step1NegotiateStr = step1Negotiate;};
	void setStep2NegotiateStr(char* step2Negotiate) {step2NegotiateStr = step2Negotiate;};
	string getStep1NegotiateStr() {return step1NegotiateStr;};
	string getStep2NegotiateStr() {return step2NegotiateStr;};


	CHAR_VECTOR sending_data;
	CHAR_VECTOR recving_data;
	
private:
	string ipaddr;
	int  port;
	string dsthost;
	int  dstport;
	string m_strDomain;
	string m_strName;
	string m_strPwd;
	string step1NegotiateStr;
	string step2NegotiateStr;

	
	HINSTANCE m_hInstSecuLib;
	SecurityFunctionTableA* m_pFunTable;
	CredHandle m_hCredentials;
	CtxtHandle m_SecurityContext;
	TimeStamp m_ts;
	ULONG m_ulContextAttributes;
};
#endif  /* _NTLMPROXY_H */
