#ifndef __HTTPCLI_H
#define __HTTPCLI_H

#include <list>
#include <string>

using namespace std;

#include "httptransfer.h"
#include "httpmanager.h"

#include "Url.h"
#include "Socket.h"
#include "Utils.h"
#include "SockProxy.h"

//NTLM PROXY only support on windows nt domain
#ifdef WIN32
#include "ntlmproxy.h"
#endif

/* Some status code validation macros: */
#define H_20X(x)        (((x) >= 200) && ((x) < 300))
#define H_PARTIAL(x)    ((x) == HTTP_PARTIAL_CONTENTS)
#define H_REDIRECTED(x) (((x) == HTTP_MOVED_PERMANENTLY) || ((x) == HTTP_MOVED_TEMPORARILY))

/* HTTP/1.1 status codes from RFC1945*/

/* Successful 2xx. */
#define HTTP_OK                		200
#define HTTP_CREATED           		201
#define HTTP_ACCEPTED          		202
#define HTTP_NO_CONTENT        		204
#define HTTP_PARTIAL_CONTENTS  		206

/* Redirection 3xx. */
#define HTTP_MULTIPLE_CHOICES  		300
#define HTTP_MOVED_PERMANENTLY 		301
#define HTTP_MOVED_TEMPORARILY 		302
#define HTTP_NOT_MODIFIED      		304

/* Client error 4xx. */
#define HTTP_BAD_REQUEST       		400
#define HTTP_UNAUTHORIZED     		401
#define HTTP_FORBIDDEN         		403
#define HTTP_NOT_FOUND         		404
#define HTTP_NEED_PASS         		407


/* Server errors 5xx. */
#define HTTP_INTERNAL          500
#define HTTP_NOT_IMPLEMENTED   501
#define HTTP_BAD_GATEWAY       502
#define HTTP_UNAVAILABLE       503
#define HTTP_GATEWAY_TIMEOUT   504


//The fix len of http method, dont include url len.
//"GET %s HTTP/1.1\r\n"
#define HTTP_METHOD_GET_TEMPLATE    "GET %s HTTP/1.0\r\n"
#define HTTP_METHOD_GET_FIXLEN		((int)strlen(HTTP_METHOD_GET_TEMPLATE)-2)	
//"POST %s HTTP/1.1\r\n"
#define HTTP_METHOD_POST_TEMPLATE    "POST %s HTTP/1.0\r\n"
#define HTTP_METHOD_POST_FIXLEN		((int)strlen(HTTP_METHOD_POST_TEMPLATE)-2)	
//"HEAD %s HTTP/1.1\r\n"
#define HTTP_METHOD_HEAD_TEMPLATE    "HEAD %s HTTP/1.0\r\n"
#define HTTP_METHOD_HEAD_FIXLEN		((int)strlen(HTTP_METHOD_HEAD_TEMPLATE)-2)		
//"CONNECT %s HTTP/1.1\r\n"
#define HTTP_METHOD_CONNECT_TEMPLATE    "CONNECT %s HTTP/1.0\r\n"
#define HTTP_METHOD_CONNECT_FIXLEN	((int)strlen(HTTP_METHOD_CONNECT_TEMPLATE)-2)			

#define __HEAD_BROUNDARY  "--BOUNDARY_0xaaaaaaaaaaaaaaaa"	// 各个Post参数之间的分隔标记，一个固定头加上一个定长16进制的uint64_t随机数
#define __BROUNDARY  "----BOUNDARY_0xaaaaaaaaaaaaaaaa"		// 各个Post参数之间的分隔标记
#define HTTP_POSTPARAM_BROUNDARY_FIXLEN	((int)strlen(__BROUNDARY))

#define POSTPARAM_TEMPLATE_TEXT 		"Content-Disposition: form-data; name=\"%s\"\r\n\r\n"
//POSTPARAM_TEMPLATE_TEXT_LEN 为对应模板除去格式化输入符的长度,即除去"%s"的长度
#define POSTPARAM_TEMPLATE_TEXT_LEN 	((int)strlen(POSTPARAM_TEMPLATE_TEXT) - 2)	
//POSTPARAM_TEMPLATE_BINARY_LEN 为对应模板除去格式化输入符的长度,即除去"%s"的长度
//	#define POSTPARAM_TEMPLATE_BINARY 		"Content-Disposition: form-data; name=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n" 
//	#define POSTPARAM_TEMPLATE_BINARY_LEN 	((int)strlen(POSTPARAM_TEMPLATE_BINARY) - 2*1)	
#define POSTPARAM_TEMPLATE_BINARY 		 "Content-Disposition: form-data; name=\"%s\"; filename=\"partfile.tmp\"\r\nContent-Type: application/octet-stream\r\n\r\n"
#define POSTPARAM_TEMPLATE_BINARY_LEN 	((int)strlen(POSTPARAM_TEMPLATE_BINARY) - 2*1)	
//POSTPARAM_TEMPLATE_FILE_LEN 为对应模板除去格式化输入符的长度,即除去"%s"的长度
//#define POSTPARAM_TEMPLATE_FILE 		"Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n"
#define POSTPARAM_TEMPLATE_FILE 		"Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: %s\r\n\r\n"
#define POSTPARAM_TEMPLATE_FILE_LEN 	((int)strlen(POSTPARAM_TEMPLATE_FILE) - 2*3)	


#define DEFAULT_USER  			"anonymous"
#define DEFAULT_PASSWD 			"galaxyfxstar@gmail.com"

extern char* getMimeType(char* file_suffix);
extern void getPostBoundary(char* boundary, int boundary_len);


typedef enum  {
    VERB_HEAD,
    VERB_GET,
	VERB_POST,
	VERB_CONNECT,
    VERB_NONE
}THttpVerb;

typedef enum
{
	SOCKS_PROXY_INIT = 0,
//	SOCKS_PROXY_VER_SENDING,
	SOCKS_PROXY_VER_SENT,	//发送了socks代理版本信息
//	SOCKS_PROXY_AUTH_SENDING,
	SOCKS_PROXY_PASS_SENT,	//发送了待验证的用户名，密码
	SOCKS_PROXY_REQ_SENT,	//发送了代理请求
	SOCKS_PROXY_OK,
}SOCKS_PROXY_STATUS;


class POST_PARAMS;

class THttpCli 
{
private :	
	URL * u;
	Proxy_t m_proxy;
	SOCKS_PROXY_STATUS socks_proxy_status;
	TSocket * htSkp;
	TSockProxy * m_socks;	
	
#ifdef WIN32
	NTLM_PROXY_STATUS ntlm_proxy_status;
	NtlmProxy * m_ntlm;
#endif	

	
	THttpMsg * htRequest;
	THttpMsg * htResponse;
	
	char * StatusLine;
	int StatusCode;
	bool UseProxy;
	
	char uncompleteLineBuf[LINE_BUFFER];	
	char postBoundary[100];
	int recvedlen;	

	void init();
	
public  :
	THttpCli(const char * sUrl);
	THttpCli(const char * sUrl, Proxy_t proxy);
	
	void ResetUrl(const char * sUrl) { u->reset_url(sUrl); }
	char * GetStatusLine( void ) { return StatusLine; }
	
	~THttpCli(void);

	TSocket* getSocket() { return htSkp;};
	
	Proxy_t GetProxy() { return m_proxy; }
	bool IsUseProxy() { return UseProxy; }
	
	int Connect(void);
	int   checkConnect(void);
	
	int ConnectProxyHost();
	int recvAuthResp();	
	int sendProxyRequest();	
	int sendSocksVersion();	
	int proxyAuth();

	void Disconnect(void);
/////////////////////////////////////////////////////////////////
	void SetIPTOS();
	off_t RecvData(char *buffer, int size, int flags, int timeout );
	off_t DirectRecvData(char *buffer, int size, int flags);
	off_t DirectRecvData(CHAR_VECTOR* respData, ULONG64 size, int flags);
	//off_t SendData(char *buffer, int size, int flags, int timeout );
	off_t SendData(char *buffer, ULONG64 size, int flags, int timeout, ULONG64& BytesWritten);

///////////////////////////////////////////////////////////////////		
	size_t GetHttpRequestLen( void ) { return htRequest->GetAllHdrLen(); 	}
	size_t GetHttpResponseLen(void ) { return htResponse->GetAllHdrLen(); 	}
	
	THttpMsg * GetHttpRequest( void ) { return htRequest; 	}
	THttpMsg * GetHttpResponse( void ) { return htResponse; }
	int	getStatusCode( void ) { return StatusCode; }
///////////////////////////////////////////////////////////////////
	
	int BufReadChar(char * c);
	int BufPeek(char * c);
	int ReadLine(char *line, int maxsize);	
	int ReadLine(char *line, int maxsize, int& recved);
	int ParseStatLine(const char * hdr, const char * * rp);
	int SkipLws(const char * hdr);	
	char * GetAuthStr(void);	
	off_t GetContentLen(void);
	off_t GetContentRange(void);	
	const char * GetLocation(string& location);	
	time_t GetLastModified(void);
	void UTCtoLocalTime(struct tm &UTCtm,struct tm &Localtm);
	int GetAcceptRanges(void);	

	int ParseHttpHdrs(void);
	int ProcessHttpResponse( char * Cmd );
	//////////////////////////////////////////
	void BuildUserAgentHdr( void );
	void BuildConnectionHdr( void );
	void BuildContentRangeHdr( void );
	void BuildHostHdr(char * host, int port);
	void BuildAcceptHdr( void );
	void BuildRangeHdr(off_t StartPos, off_t EndPos);
	void BuildRefererHdr(char * Ref);
	void BuildBasicAuthHdr( char * UserName, char * Password );
	void BuildProxyAuthHdr(const char * UserName, const char * Password);
	void BuildPragmaHdr( void );
	void BuildContentType(const char* contenttype);	
	void BuildContentType();
	void BuildCustomHdr(const char* strkey, const char* strval);
	void BuildCookieHdr(const char* strkey, const char* strval);
	void BuildContentLengthHdr(ULONG64 contentLength );
	//void BuildHttpRequest( THttpVerb verb, char * szBuffer);	
	//void BuildHttpRequest( THttpVerb verb, char * szBuffer, ULONG64 bufferlen, POST_PARAMS* postParams, ULONG64& datalen);
	INT BuildHttpRequest( THttpVerb verb, char *& szBuffer, ULONG64& bufferlen, POST_PARAMS* postParams, TASK_TYPE task_type, ULONG64& datalen);

	int processNtlmAuth();
	int sendNtlmAuthData();
	int recvNtlmResp();	
	string getNtlmNegotiateStr();
	void BuildNtlmAuthHdr(const char* szcode);	
	void ClearRequestHdr();	
	void ClearResponseHdr();
	///////////////////////////////////////////////	
};
#endif
