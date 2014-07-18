// HTTPUploadFile.cpp: implementation of the HTTPUploadFile class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../stdafx.h"
#include "HTTPUploadFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define RESEND_COUNT 2


CHTTPUploadFile::CHTTPUploadFile()	
{
		_hHTTPRequest=NULL; 
		_hHTTPOpen=NULL;
		_hHTTPConnection=NULL;	
		memset(_szHTTPResponseHTML,0,__SIZE_HTTP_BUFFER);

}

CHTTPUploadFile::~CHTTPUploadFile()
{
	FreePostArguments();
}

// 连接到服务器	
BOOL CHTTPUploadFile::Connect(LPCTSTR szAddress, LPCTSTR szAgent , unsigned short nPort, LPCTSTR szUserAccount, LPCTSTR szPassword )
{
	//创建一个internet应用句柄
	_hHTTPOpen=::InternetOpen(szAgent,	// agent name
							  INTERNET_OPEN_TYPE_PRECONFIG,	// proxy option
							  _T(""),							// proxy
							  _T(""),							// proxy bypass
							  0);							// flags

	if(!_hHTTPOpen)
	{
		_dwError=::GetLastError();
		return FALSE;
	}	
	
	//打开一个internet连接句柄
	_hHTTPConnection=::InternetConnect(	_hHTTPOpen,	// internet opened handle
										szAddress, // server name
										nPort, // ports
										szUserAccount, // user name
										szPassword, // password 
										INTERNET_SERVICE_HTTP, // service type
										INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,	// service option																														
										0);	// context call-back option

	if(!_hHTTPConnection)
	{		
		_dwError=::GetLastError();
		_stprintf(_strError,_T("(错误代码:%d)"),_dwError);
		::InternetCloseHandle(_hHTTPOpen);
		return FALSE;
	}
	
	//检查句柄
	if(::InternetAttemptConnect(NULL)!=ERROR_SUCCESS)
	{		
		_dwError=::GetLastError();
		_stprintf(_strError,_T("(错误代码:%d)"),_dwError);
		::InternetCloseHandle(_hHTTPConnection);
		::InternetCloseHandle(_hHTTPOpen);
		return FALSE;
	}
	OutputDebugString(_T("internet attempt connection"));
	return TRUE;
}

//关闭连接,释放资源
BOOL CHTTPUploadFile::Close()
{
	if(_hHTTPRequest)
	{
		::InternetCloseHandle(_hHTTPRequest);
		_hHTTPRequest=NULL;
	}
	if(_hHTTPConnection)
	{
		::InternetCloseHandle(_hHTTPConnection);
		_hHTTPConnection=NULL;
	}
	if(_hHTTPOpen)
	{
		::InternetCloseHandle(_hHTTPOpen);
		_hHTTPOpen=NULL;
	}
	return TRUE;

}

//初始化post参数列表
VOID CHTTPUploadFile::InitilizePostArguments()
{
	FreePostArguments();
}


// 添加post参数
VOID CHTTPUploadFile::AddPostArguments(LPCTSTR szName, DWORD nValue){

	HTTPUploadFileArgument	arg;
	::FillMemory(&arg, sizeof(arg), 0x00);

	_tcscpy(arg.szName, szName);
	_stprintf(arg.szValue, _T("%d"), nValue);
	arg.dwType = CHTTPUploadFile::TypeNormal;
	_vArguments.push_back(arg);
	_dwNormalArgvCount++;
	return;
}

VOID CHTTPUploadFile::AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary){

	HTTPUploadFileArgument	struArg;
	::ZeroMemory(&struArg, sizeof(struArg));
	_tcscpy(struArg.szName, szName);
	_tcscpy(struArg.szValue, szValue);

	if(!bBinary)
	{
		struArg.dwType = CHTTPUploadFile::TypeNormal;
		_dwNormalArgvCount++;
	}
	else
	{
		struArg.dwType = CHTTPUploadFile::TypeBinary;
		_dwBinaryArgvCount++;
	}
	_vArguments.push_back(struArg);
	return;
}

//post数据
BOOL CHTTPUploadFile::Request(LPCTSTR szURL ,LPCTSTR szAgent )
{
	memset(_szHTTPResponseHTML, 0 ,__SIZE_HTTP_BUFFER);
	BOOL bReturn  = TRUE;
	DWORD dwPort = 0;
	TCHAR szProtocol[__SIZE_BUFFER] = _T("");
	TCHAR szAddress[__SIZE_BUFFER] = _T( "");	
	TCHAR szURI[__SIZE_BUFFER] = _T("");
	DWORD dwSize = 0;
	_dwError = 0;
	

	//检查url的合法性
	ParseURL(szURL, szProtocol, szAddress, dwPort, szURI);

	if(Connect(szAddress, szAgent, dwPort))
	{
		if(!RequestPostMultiPartsFormData(szURI))
		{
			bReturn=FALSE;
		}
		else
		{
			Close();
		}
		FreeMultiPartsFormData();
		
	}else{
		//::PostMessage(_notify_dlg_handle,WM_UPLOAD_ERROR,GetError(),0);
		bReturn=FALSE;
	}

	return bReturn;
}


BOOL CHTTPUploadFile::RequestPostMultiPartsFormData(LPCTSTR szURI)
{
	DWORD dwTotalSize=AllocMultiPartsFormData(__BROUNDARY);
	DWORD dwTotalSendSize=0;
	DWORD dwCompleteCount=0;
	DWORD dwFileCount=_vPostData.size();

	std::vector<HTTPPostData>::iterator itPostData;
	for(itPostData=_vPostData.begin();itPostData!=_vPostData.end();itPostData++)
	{
		DWORD dwSize=0;
		if(!SendPostRequest(szURI,itPostData->pPostData,itPostData->dwLength,dwTotalSize,dwTotalSendSize)
		   || !Response((PBYTE)_szHTTPResponseHeader, __SIZE_HTTP_BUFFER, (PBYTE)_szHTTPResponseHTML, __SIZE_HTTP_BUFFER, dwSize))
		{
				return FALSE;		
		}
		else
		{
		}
		dwTotalSendSize+=itPostData->dwLength;
		dwCompleteCount++;
		//::PostMessage(_notify_dlg_handle,WM_COMPLETE_FILE_COUNT,dwCompleteCount,dwFileCount);
	}
	
	return TRUE;
}


BOOL CHTTPUploadFile::SendPostRequest(LPCTSTR szURI,PBYTE pPostData,DWORD dwSize, DWORD dwTotalSize,DWORD dwTotalSendSize)
{
	BOOL bReturn=TRUE;

	CONST TCHAR *szAcceptType = __HTTP_ACCEPT_TYPE;
	
	TCHAR szContentType[__SIZE_BUFFER];
	_stprintf(szContentType,_T("Content-Type: multipart/form-data; boundary=%s\r\n"),__BROUNDARY);		
	
	if(_hHTTPRequest!=NULL)
	{
		InternetCloseHandle(_hHTTPRequest);
		_hHTTPRequest=NULL;
	}

	const TCHAR* rgszAccept[] = { _T("*/*"), NULL };	//Accept anything
	
	_hHTTPRequest=::HttpOpenRequest(_hHTTPConnection,
									_T("POST"), // HTTP Verb
									szURI, // Object Name
									NULL, // Version
									NULL, // Reference
									rgszAccept, // Accept Type
									INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT,	// flags
									NULL); // context call-back point
	if(!_hHTTPRequest){
		_dwError=::GetLastError();

		return FALSE;
	}

	// REPLACE HEADER
	if(!::HttpAddRequestHeaders( _hHTTPRequest, __HTTP_ACCEPT, _tcslen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_REPLACE)){
		_dwError=::GetLastError();
		return FALSE;
	}

	
	if(!::HttpAddRequestHeaders( _hHTTPRequest, szContentType, _tcslen(szContentType), HTTP_ADDREQ_FLAG_ADD_IF_NEW)){
		
		_dwError=::GetLastError();
		return FALSE;
	}

	TCHAR	szContentLength[__SIZE_BUFFER]=_T("");
	::ZeroMemory(szContentLength, __SIZE_BUFFER);

	_stprintf(szContentLength, _T("Content-Length: %d\r\n"), dwSize);

	if(!::HttpAddRequestHeaders( _hHTTPRequest, szContentLength, _tcslen(szContentLength), HTTP_ADDREQ_FLAG_ADD_IF_NEW)){
		
		_dwError=::GetLastError();
		return FALSE;
	}

	// SEND REQUEST WITH HttpSendRequestEx and InternetWriteFile
	INTERNET_BUFFERS InternetBufferIn={0};
	InternetBufferIn.dwStructSize=sizeof(INTERNET_BUFFERS);
	InternetBufferIn.Next=NULL;	
	
	if(!::HttpSendRequestEx(_hHTTPRequest, &InternetBufferIn, NULL, HSR_INITIATE, 0)){
		_dwError=::GetLastError();
		return FALSE;
	}

	DWORD dwOutPostBufferLength=0;
	DWORD dwSum=0;
	DWORD dwSendSize=FILE_CHUNK_SZIE;

	while(dwSum<dwSize)
	{
		if(dwSize-dwSum<FILE_CHUNK_SZIE)
			dwSendSize=dwSize-dwSum;
        else
			dwSendSize=FILE_CHUNK_SZIE;

		if(!::InternetWriteFile(_hHTTPRequest, pPostData+dwSum, dwSendSize, &dwOutPostBufferLength) )
		{
			_dwError=::GetLastError();
			
			return FALSE;
		}
		dwSum+=dwOutPostBufferLength;
		PostMessage(_notify_dlg_handle,WM_UPLOAD_PROGRESS,dwTotalSendSize+dwSum,dwTotalSize);
	}

	OutputDebugString(_T("data send complete!"));

	if(!::HttpEndRequest(_hHTTPRequest, NULL, HSR_INITIATE, 0)){
	
		_dwError=::GetLastError();
		return FALSE;
	}
	
	OutputDebugString(_T("send end request complete!"));
	return TRUE;
}


DWORD CHTTPUploadFile::GetNormalPostArgvSize()
{
	DWORD dwReturn=_dwNormalArgvCount*__SIZE_HTTP_HEAD_LINE*4;
	return dwReturn;
}

DWORD CHTTPUploadFile::GenerateNormalPostData(LPCTSTR szBroundary,PBYTE pData)
{
	
	DWORD dwPostSize=0;
	//构造普通post参数
	if(pData!=NULL)
	{
		DWORD dwBufferSize=0;
		std::vector<HTTPUploadFileArgument>::iterator itArgv;
		for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv)
		{

			PBYTE pBuffer=NULL;
			// SET MULTI_PRATS FORM DATA BUFFER
			if(itArgv->dwType==CHTTPUploadFile::TypeNormal)
			{
					pBuffer=(PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE*4);
					_stprintf(	(TCHAR*)pBuffer,							
									_T("--%s\r\n")
									_T("Content-Disposition: form-data; name=\"%s\"\r\n")
									_T("\r\n")
									_T("%s\r\n"),
									szBroundary,
									itArgv->szName,
									itArgv->szValue);

					dwBufferSize=_tcslen((TCHAR*)pBuffer);
					::CopyMemory((pData+dwPostSize), pBuffer, dwBufferSize);
					dwPostSize+=dwBufferSize;
					if(pBuffer!=NULL)
						::LocalFree(pBuffer);
			}

		}
	}
	return dwPostSize;
}



DWORD CHTTPUploadFile::AllocMultiPartsFormData(LPCTSTR szBroundary)
{
	//得到与分配普通post参数缓冲区大小
	DWORD dwPostArgvSize=0;

	PBYTE pArgvsBuffer=NULL;
	DWORD dwArgvBufSize=0;
	//
	dwPostArgvSize=GetNormalPostArgvSize();
	if(dwPostArgvSize!=0)
	{
		pArgvsBuffer=(PBYTE)::LocalAlloc(LPTR,dwPostArgvSize);
		dwArgvBufSize=GenerateNormalPostData(szBroundary,pArgvsBuffer);
	}
	
	//分配post文件上传的数据缓冲区和填充数据
	TCHAR szPostEndFlag[256];
	DWORD dwFileSize=0;
	std::vector<HTTPUploadFileArgument>::iterator itArgv;
	for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv)
	{
		PBYTE pBuffer=NULL;

		int index = itArgv - _vArguments.begin();
		if(itArgv->dwType==CHTTPUploadFile::TypeBinary)
		{
			HANDLE hFile=::CreateFile(itArgv->szValue, 
									  GENERIC_READ, // desired access
									  FILE_SHARE_READ, // share mode
									  NULL, // security attribute
									  OPEN_EXISTING, // create disposition
									  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, // flag and attributes
									  NULL); // template file handle
			
			DWORD	dwSize=::GetFileSize(hFile, NULL);

			pBuffer=(PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE*4+dwArgvBufSize+dwSize+1);
			if(pArgvsBuffer!=NULL)
			{
				::CopyMemory(pBuffer,pArgvsBuffer,dwArgvBufSize);
			}
			_stprintf( (TCHAR*)pBuffer+dwArgvBufSize,
							_T("--%s\r\n")
							_T("Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n")
							_T(	"Content-Type: %s\r\n")
							_T(	"\r\n"),
							szBroundary,
							itArgv->szName, itArgv->szValue,
							GetContentType(itArgv->szValue)
							);

			DWORD	dwBufPosition=_tcslen((TCHAR*)pBuffer);

			BYTE	pBytes[__SIZE_BUFFER+1]="";
			DWORD	dwNumOfBytesToRead=0;
			DWORD	dwTotalBytes=0;

			while(::ReadFile(hFile, pBytes, __SIZE_BUFFER, &dwNumOfBytesToRead, NULL) && dwNumOfBytesToRead>0 && dwTotalBytes<=dwSize)
			{
				::CopyMemory((pBuffer+dwBufPosition+dwTotalBytes), pBytes, dwNumOfBytesToRead);
				::ZeroMemory(pBytes, __SIZE_BUFFER+1);
				dwTotalBytes+=dwNumOfBytesToRead;				
			}
			
			::ZeroMemory(szPostEndFlag,256);
			_stprintf(szPostEndFlag,_T("\r\n--%s--\r\n"),szBroundary);
			::CopyMemory((pBuffer+dwBufPosition+dwTotalBytes), szPostEndFlag, _tcslen(szPostEndFlag));
			::CloseHandle(hFile);

			DWORD dwBufferSize=dwBufPosition+dwTotalBytes+_tcslen(szPostEndFlag);

			AddPostData(pBuffer,dwBufferSize);
			dwFileSize+=dwBufferSize;
		}
	}

	
	if(pArgvsBuffer!=NULL)
		::LocalFree(pArgvsBuffer);
	return dwFileSize;
}

//得到服务器返回数据
BOOL CHTTPUploadFile::Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize)
{

	BYTE pResponseBuffer[__SIZE_BUFFER]="";	
	DWORD dwNumOfBytesToRead=0;

	if(!_hHTTPRequest){
		_dwError=::GetLastError();

		return FALSE;
	}

	::ZeroMemory(pBuffer, dwBufferLength);
	dwResultSize=0;
	while((dwNumOfBytesToRead=ResponseOfBytes(pResponseBuffer, __SIZE_BUFFER))!=NULL && dwResultSize<dwBufferLength){
		::CopyMemory( (pBuffer+dwResultSize), pResponseBuffer, dwNumOfBytesToRead);		
		dwResultSize+=dwNumOfBytesToRead;
	}

	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	if(!::HttpQueryInfo(_hHTTPRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pHeaderBuffer, &dwHeaderBufferLength, NULL)){
		_dwError=::GetLastError();
		return FALSE;
	}

	if(dwResultSize==0)
	{
		OutputDebugString(_T("Responese返回内容长度为0"));
		_tcscpy((TCHAR*)pBuffer,_T(""));
	}
	OutputDebugString((LPCTSTR)pBuffer);

	//return (dwResultSize? TRUE: FALSE);
	return TRUE;
}

//查询服务器返回数据
LPCTSTR CHTTPUploadFile::QueryHTTPResponse()
{
	return _szHTTPResponseHTML;
}

//查询服务器返回数据
LPCTSTR CHTTPUploadFile::QueryHTTPResponseHeader()
{
	return _szHTTPResponseHeader;

}

DWORD CHTTPUploadFile::GetError()
{
	return _dwError;
}

LPCTSTR CHTTPUploadFile::GetContentType(LPCTSTR szName)
{
	static TCHAR	szReturn[1024]=_T("");
	LONG	dwLen=1024;
	DWORD	dwDot=0;

	for(dwDot=_tcslen(szName);dwDot>0;dwDot--){
		if(!_tcsncmp((szName+dwDot),_T("."), 1))
			break;
	}

	HKEY	hKEY;
	LPTSTR	szWord=(TCHAR*)(szName+dwDot);

	if(RegOpenKeyEx(HKEY_CLASSES_ROOT, szWord, 0, KEY_QUERY_VALUE, &hKEY)==ERROR_SUCCESS){
		if(RegQueryValueEx(hKEY, _T("Content Type"), NULL, NULL, (LPBYTE)szReturn, (unsigned long*)&dwLen)!=ERROR_SUCCESS)
			_tcsncpy(szReturn, _T( "application/octet-stream"), _tcslen(_T("application/octet-stream")));
		RegCloseKey(hKEY);
	}else{
		_tcsncpy(szReturn, _T("application/octet-stream"), _tcslen(_T("application/octet-stream")));
	}

	return szReturn;
}


int CHTTPUploadFile::GetArgumentCount()
{
	return _vArguments.size();
	
}

DWORD CHTTPUploadFile::ResponseOfBytes(PBYTE pBuffer, DWORD dwSize)
{
	static DWORD dwBytes=0;

	if(!_hHTTPRequest){
		_dwError=::GetLastError();

		return 0;
	}

	::ZeroMemory(pBuffer, dwSize);
	if(!::InternetReadFile(	_hHTTPRequest,
									pBuffer,
									dwSize,
									&dwBytes)){
		_dwError=::GetLastError();

		return 0;
	}

	return dwBytes;
}


VOID CHTTPUploadFile::FreeMultiPartsFormData()
{
	
	std::vector<HTTPPostData>::iterator itPostData;
	for(itPostData=_vPostData.begin();itPostData!=_vPostData.end();itPostData++)
	{

		if(itPostData->pPostData!=NULL)
		{
			::LocalFree(itPostData->pPostData);
		}
	}
	_vPostData.clear();
	
}

VOID CHTTPUploadFile::AddPostData(PBYTE pData, DWORD dwLen)
{
	_HTTP_POST_DATA pd;
	pd.pPostData=pData;
	pd.dwLength=dwLen;
	_vPostData.push_back(pd);
}


VOID CHTTPUploadFile::ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, DWORD &dwPort, LPTSTR szURI)
{
	TCHAR szPort[__SIZE_BUFFER]=_T("");
	DWORD dwPosition=0;
	BOOL bFlag=FALSE;

	while(_tcslen(szURL)>0 && dwPosition<_tcslen(szURL) && _tcsncmp((szURL+dwPosition), _T(":"), 1))
		++dwPosition;

	if(!_tcsncmp((szURL+dwPosition+1), _T("/"), 1)){	// is PROTOCOL
		if(szProtocol){
			_tcsncpy(szProtocol, szURL, dwPosition);
			szProtocol[dwPosition]=0;
		}
		bFlag=TRUE;
	}else{	// is HOST 
		if(szProtocol){
			_tcsncpy(szProtocol, _T("http"), 4);
			szProtocol[5]=0;
		}
	}

	DWORD dwStartPosition=0;
	
	if(bFlag){
		dwStartPosition=dwPosition+=3;				
	}else{
		dwStartPosition=dwPosition=0;
	}

	bFlag=FALSE;
	while(_tcslen(szURL)>0 && dwPosition<_tcslen(szURL) && _tcsncmp((szURL+dwPosition), _T("/"), 1))
			++dwPosition;

	DWORD dwFind=dwStartPosition;

	for(;dwFind<=dwPosition;dwFind++){
		if(!_tcsncmp((szURL+dwFind), _T(":"), 1)){ // find PORT
			bFlag=TRUE;
			break;
		}
	}

	if(bFlag){
		TCHAR sztmp[__SIZE_BUFFER]=_T("");
		_tcsncpy(sztmp, (szURL+dwFind+1), dwPosition-dwFind);
		dwPort=_ttol(sztmp);
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwFind-dwStartPosition);
	}else if(!_tcscmp(szProtocol,_T("https"))){
		dwPort=INTERNET_DEFAULT_HTTPS_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}else {
		dwPort=INTERNET_DEFAULT_HTTP_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}

	if(dwPosition<_tcslen(szURL)){ // find URI
		_tcsncpy(szURI, (szURL+dwPosition), _tcslen(szURL)-dwPosition);
	}else{
		szURI[0]=0;
	}

	return;
}

DWORD CHTTPUploadFile::GetFilesSize()
{
	
	std::vector<HTTPUploadFileArgument>::iterator itArgv;

	DWORD	dwLength=0;

	for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv){

		switch(itArgv->dwType){
		case	CHTTPUploadFile::TypeNormal:
			break;
		case	CHTTPUploadFile::TypeBinary:
			HANDLE hFile=::CreateFile(itArgv->szValue, 
									GENERIC_READ, // desired access
									FILE_SHARE_READ, // share mode
									NULL, // security attribute
									OPEN_EXISTING, // create disposition
									FILE_ATTRIBUTE_NORMAL, // flag and attributes
									NULL); // template file handle

			dwLength+=::GetFileSize(hFile, NULL);
			::CloseHandle(hFile);			
			break;
		}

	}

	return dwLength;
	
}

void CHTTPUploadFile::FreePostArguments()
{

	_vArguments.clear();
	FreeMultiPartsFormData();
	_dwNormalArgvCount=0;
	_dwBinaryArgvCount=0;
}
void CHTTPUploadFile::CancelUpload()
{
	
	Close();
	FreePostArguments();
}



