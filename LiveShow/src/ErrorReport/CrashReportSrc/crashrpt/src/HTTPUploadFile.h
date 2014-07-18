/*
 * $ HTTP Upload File
 * ----------------------------------------------------------------------------------------------------------------
 *
 * 名称 :    HTTPUploadFile
 * 版本 :    0.1.0
 * 描述 :    使用 WININET 通过Post方式上传文件,
 * 作者 :    张剑波 
 * 日期 :    2006.02.24
 *
 */

#include <wininet.h>

// use stl
#include <vector>

#pragma 

// PRE-DEFINED CONSTANTS
#define __DEFAULT_AGENT_NAME	_T("MERONG(0.9/;p)")

// PRE-DEFINED BUFFER SIZE
#define	__SIZE_HTTP_ARGUMENT_NAME	256
#define __SIZE_HTTP_ARGUMENT_VALUE	1024

#define __HTTP_VERB_GET	TEXT("GET")
#define __HTTP_VERB_POST TEXT("POST")
#define __HTTP_ACCEPT_TYPE TEXT("*/*\0")
#define __HTTP_ACCEPT TEXT("Accept: */*\r\n")
#define __SIZE_HTTP_BUFFER	10240//100000
#define __SIZE_HTTP_RESPONSE_BUFFER	10240//100000
#define __SIZE_HTTP_HEAD_LINE	2048

#define __SIZE_BUFFER	1024
#define __SIZE_SMALL_BUFFER	256

#define __BROUNDARY  _T("--MULTI-PARTS-FORM-DATA-BOUNDARY")

#define FILE_CHUNK_SZIE 8192*2

/*
  进行WebAccess的过程中可能遇到的消息种类
 */
#define WM_UPLOAD_PROGRESS			WM_USER	+	200		//通知更新对话框中的进度条
#define WM_UPLOAD_ERROR				WM_USER	+	300		//通知上传发生错误
#define WM_COMPLETE_FILE_COUNT		WM_USER	+	400		//通知完成的文件数目
#define WM_START_TRANSPORT			WM_USER	+	600		//通知开始进行文件传输
#define WM_LEFT_SPACE				WM_USER +   700		//通知剩余的硬盘空间

class CHTTPUploadFile 
{
public:
	// post参数结构定义
	
	typedef struct _HTTP_UPLOAD_FILE_ARGUMENT
	{
		TCHAR	szName[__SIZE_HTTP_ARGUMENT_NAME]; //参数名称
		TCHAR	szValue[__SIZE_HTTP_ARGUMENT_VALUE]; //参数值
		DWORD	dwType;                              //参数类型
		bool operator==(const _HTTP_UPLOAD_FILE_ARGUMENT &argV)
		{
			return !_tcscmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
		}

	} HTTPUploadFileArgument;

	typedef struct _HTTP_POST_DATA
	{
		PBYTE  pPostData;
		DWORD  dwLength;
	} HTTPPostData;

	enum TypePostArgument
	{	// POST 参数类型 
		TypeUnknown=0,
		TypeNormal=1,
		TypeBinary=2,
	};


	CHTTPUploadFile();
	virtual ~CHTTPUploadFile();


	// 连接到服务器	
	BOOL Connect(LPCTSTR szAddress, 
		LPCTSTR szAgent = __DEFAULT_AGENT_NAME, 
		unsigned short nPort = INTERNET_DEFAULT_HTTP_PORT, 
		LPCTSTR szUserAccount = NULL, 
		LPCTSTR szPassword = NULL);
	//关闭连接,释放资源
	BOOL Close();
	//初始化post参数列表
	VOID InitilizePostArguments();


	// 添加post参数
	VOID AddPostArguments(LPCTSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCTSTR szName, 
		LPCTSTR szValue, 
		BOOL bBinary = FALSE);


	//post数据
	BOOL Request(LPCTSTR szURL ,LPCTSTR szAgent = __DEFAULT_AGENT_NAME);
	//得到服务器返回数据
	BOOL Response(PBYTE pHeaderBuffer, 
		DWORD dwHeaderBufferLength, 
		PBYTE pBuffer, 
		DWORD dwBufferLength, 
		DWORD &dwResultSize);	

	//查询服务器返回数据
	LPCTSTR QueryHTTPResponse();

	//查询服务器返回数据
	LPCTSTR QueryHTTPResponseHeader();

	//得到error代码
	DWORD GetError();

	//得到Content类型
	LPCTSTR GetContentType(LPCTSTR szName);

	//解析URL
	VOID ParseURL(LPCTSTR szURL, 
		LPTSTR szProtocol, 
		LPTSTR szAddress, 
		DWORD &dwPort, 
		LPTSTR szURI);

	//得到post参数个数
	int GetArgumentCount();
	
	//取消上传
	void CancelUpload();

	//得到文件总大小
	DWORD GetFilesSize();

	TCHAR _strError[256];	//错误信息
	HWND _notify_dlg_handle; //接收通知消息的窗口句柄


protected:

	std::vector<HTTPUploadFileArgument> _vArguments;				// POST参数列表
	std::vector<HTTPPostData> _vPostData;

	TCHAR		_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// 接收服务器返回HTML数据的缓冲区
	TCHAR		_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	    // 接收服务器返回http头数据的缓冲区

	HINTERNET _hHTTPOpen;				// internet open 句柄
	HINTERNET _hHTTPConnection;		// internet connection 句柄
	HINTERNET _hHTTPRequest;		// internet request 句柄

	DWORD		_dwError;					// last error code
	LPCTSTR		_szHost;					 //	 主机名
	DWORD		_dwPort;					//  端口
	
	DWORD       _dwBinaryArgvCount;			//上传文件总数
	DWORD       _dwNormalArgvCount;			//普通post参数个数
	
	DWORD ResponseOfBytes(PBYTE pBuffer, DWORD dwSize);
	
	//post文件数据,一个表单一个文件
	BOOL  RequestPostMultiPartsFormData(LPCTSTR szURI);
	//分配上传数据的缓冲区
	DWORD AllocMultiPartsFormData(LPCTSTR szBroundary);
	//得到普通Post参数的大小
	DWORD GetNormalPostArgvSize();
	//释放上传数据缓冲区
	VOID FreeMultiPartsFormData();
	
    VOID AddPostData(PBYTE pData, DWORD dwLen);
	
	DWORD GenerateNormalPostData(LPCTSTR szBroundary,PBYTE pData);

	BOOL SendPostRequest(LPCTSTR szURL,
		PBYTE pPostData,
		DWORD dwSize, 
		DWORD dwTotalSize,
		DWORD dwTotalSendSize);

	void FreePostArguments();
};