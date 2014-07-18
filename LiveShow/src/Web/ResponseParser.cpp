#include "StdAfx.h"
#include "webdefine.h"
#include "ResponseParser.h"
#include "ximage.h"
#include "RequestManager.h"
#include <vector>
#include "shlwapi.h"
#include "WebModule.h"
#include "../../../include/Log.h"
#include "utility/expat++/expat_document.h"
#include "assert.h"
#include "winsock2.h"
#include <map>
#include "../../CommonLibrary/include/utility/URLEncode.h"
#include "../../CommonLibrary/include/utility/Base64Coding.h"
#include "../../CommonLibrary/include/utility/StringHelper.h"
#include "../../CommonLibrary/include/utility/FileHelper.h"

using namespace common::utility;

#pragma comment(lib,"shlwapi.lib")

#define LOG_RESP() \
	LOG_TRACE(L"\n\n\n"); \
	LOG_TRACE(L"===================== %s ==================", __FUNCTIONW__); \
	LOG_TRACE(L"URL :%s", stringhelper::AsciiToUnicode(pContext->strURL).c_str()); \
	LOG_TRACE(L"SeqNo :%d", pContext->nReqSeq); \
	LOG_TRACE(L"Response Content:"); \
	LOG_TRACE(stringhelper::AsciiToUnicode( pContext->strContentData).c_str()); \
	LOG_TRACE(L"===============================================================\n\n"); \

#define LOG_PARSE_ERROR() \
	LOG_TRACE(L"错误行:%d", xmlDoc.get_error()->get_expat_error_line()); \
	LOG_TRACE(L"错误列:%d", xmlDoc.get_error()->get_expat_error_column()); \
	LOG_TRACE(L"WARNING:%s  解析XML错误", __FUNCTIONW__ ); \

#define LOG_ERROR( S) \
	LOG_TRACE(L"%s ERROR: %s", __FUNCTIONW__, S); \

#define NEW_RESP( RESP, pE) \
	RESP* pE = new RESP;  \
	pE->param0 = WEB_RET_SUCCESS; \
	pE->param1 = pContext->nSpeed; \
	std::map<uint32, uint32>::iterator itr = g_WebModule->m_mapSeqNo2ModuleId.find(pContext->nReqSeq);	\
	if( itr != g_WebModule->m_mapSeqNo2ModuleId.end()) \
	{ \
		pE->desMId = itr->second;	\
	} \
	pE->param2 = pContext->nReqSeq; \


CResponseParser::HandlerTableEntry CResponseParser::m_tableDriven[] =
{
	{ event_value_web_get_update_config_xml_req,				CResponseParser::ParseGetUpdateConfigXml },
	{ event_value_web_get_filter_words_req,						CResponseParser::ParseGetFilterWords},
	{ service_value_web_upload_head_image_req,				CResponseParser::ParseUploadHeadImage },
	{ service_value_web_download_head_image_req,				CResponseParser::ParseDownloadHeadImage },
	{ service_value_web_download_file_req,						CResponseParser::ParseGetDownloadedFile },
	{ service_value_web_get_http_req,								CResponseParser::ParsePassThroughRawData },	
	{ service_value_web_get_login_identify_code_req,				CResponseParser::ParseGetLoginIdentifyCode },
	{ 0, NULL}
};

CResponseParser::CResponseParser(void)
{
	m_pRequestManager = NULL;
}

CResponseParser::~CResponseParser(void)
{
}

string 
CResponseParser::ConvertCharVector( CHAR_VECTOR* pVector)
{
	string strData;
	strData.append( (*pVector).begin(), (*pVector).end());

	return strData;
}

DWORD
CResponseParser::CheckImageFileHeader(const BYTE *pBuf, uint32 nBufLen)
{
	DWORD dwTemp = 0;
	
	if( pBuf == NULL || nBufLen < 4)
		return  CXIMAGE_FORMAT_UNKNOWN;

	memcpy(&dwTemp,pBuf,4);

	if( (dwTemp & 0xFFFFFF)==0xFFD8FF)
	{
		return CXIMAGE_FORMAT_JPG;
	}
	if(dwTemp == 0x38464947)
	{
		return CXIMAGE_FORMAT_GIF;
	}
	if(dwTemp == 0x474E5089)
	{
		return CXIMAGE_FORMAT_PNG;
	}
	if((dwTemp & 0xFFFF)==0x4D42)
	{
		return CXIMAGE_FORMAT_BMP;
	}

	return CXIMAGE_FORMAT_UNKNOWN;
}

DWORD
CResponseParser::CheckImageFileHeader(const wstring wstrImagePath)
{
	FILE* fp = _wfopen(wstrImagePath.c_str(), L"r");
	if( fp == NULL)
		return CXIMAGE_FORMAT_UNKNOWN;

	char pTempBuffer[10] = {0};
	size_t nNum = fread(pTempBuffer, (size_t)sizeof(char), 10, fp) ;
	fclose(fp);

	if( nNum > 0)
		return CheckImageFileHeader((BYTE*)pTempBuffer, (uint32)nNum);

	return CXIMAGE_FORMAT_UNKNOWN;
}

DWORD 
CResponseParser::GetReturnError( UINT	nErrorCode)
{
	// 公共错误码
	// 100 Token不正确
	// 101 缺少参数或者参数错误
	// 103 auth验证不正确
	if( nErrorCode == 100)
		return WEB_RET_COMMON_ERROR_TOKEN;

	if( nErrorCode == 101)
		return WEB_RET_COMMON_ERROR_PARAM;

	if( nErrorCode == 103)
		return WEB_RET_COMMON_ERROR_AUTH;

	if( nErrorCode == 104)
		return WEB_RET_COMMON_TOKEN_TIMEOUT;

	return WEB_RET_COMMON_ERROR_INTERNAL;
}

uint32	
CResponseParser::ParseErrorXml( string strSrc, string& strContent)
{
	if( strSrc.substr(0,5) != "<?xml")
	{
		return WEB_RET_COMMON_ERROR_INTERNAL;
	}

	strSrc = stringhelper::UnicodeToUTF8( stringhelper::UTF8ToUnicode(strSrc));

	expat_document xmlDoc;
	if (!xmlDoc.load_from_buffer(strSrc.c_str(), (UINT32)strSrc.size())) 
	{
		return WEB_RET_COMMON_ERROR_INTERNAL;
	}

	expat_element *pRoot = xmlDoc.get_root();
	if (!pRoot)  
	{
		return WEB_RET_COMMON_ERROR_INTERNAL;
	}

	expat_element* pvelement = pRoot->get_element_by_name(L"v");
	if( pvelement == NULL)
	{
		return WEB_RET_INNER_USED;
	}

	wstring pszError = (wstring)pvelement->get_attribute_value(L"Error");
	if( pszError == L"")
	{
		return WEB_RET_COMMON_ERROR_INTERNAL;
	}

	int nErrorCode;
	BOOL bRet = stringhelper::StringToInt(pszError, &nErrorCode);
	int nResult = 0;

	if( nErrorCode == 0)
	{
		wstring pszID = (wstring)pvelement->get_element_contents();
		strContent = stringhelper::UnicodeToAscii(pszID);
	}
		
	return nErrorCode;
}

void	
CResponseParser::SetReuqestManager( CRequestManager*	pRequestManager)
{
	m_pRequestManager = pRequestManager;
}

/**
* Function		获取给定结点下的特定子结点的值
* @param		pElem 是需要获取的结点，
* @param		strChildNodeName 是pElem下的需要获取的结点的名称
* @return		返回结点元素的值
**/


/**
* Function		获取结点的属性值
* @param		pElem 需要获取属性的节点	
* @param		strAttributeName 需要获取的属性名称
* @return		返回属性的值
**/

BOOL	
CResponseParser::SaveAsImage( const wchar_t* lpszPath,const char* lpszContentData, 
									 uint32 dwContentSize, DWORD dwType)
{
	CxImage image;

	bool bRet = false;

	DWORD dwFileType = CResponseParser::CheckImageFileHeader( (BYTE*)lpszContentData, dwContentSize);
	if( dwFileType == CXIMAGE_FORMAT_BMP )
	{
		bRet = image.Decode( (BYTE*)lpszContentData, dwContentSize, CXIMAGE_FORMAT_BMP);
	}
	else if( dwFileType == CXIMAGE_FORMAT_GIF)
	{
		bRet = image.Decode( (BYTE*)lpszContentData, dwContentSize, CXIMAGE_FORMAT_GIF);
	}
	else if( dwFileType == CXIMAGE_FORMAT_JPG )
	{
		bRet = image.Decode( (BYTE*)lpszContentData, dwContentSize, CXIMAGE_FORMAT_JPG);
	}
	else if( dwFileType == CXIMAGE_FORMAT_PNG)
	{
		bRet = image.Decode( (BYTE*)lpszContentData, dwContentSize, CXIMAGE_FORMAT_PNG);
	}

	if( bRet == true)
	{
		if( dwType == CXIMAGE_FORMAT_BMP)
		{
			return image.SaveW( lpszPath, CXIMAGE_FORMAT_BMP);
		}
		else if( dwType == CXIMAGE_FORMAT_GIF)
		{
			return image.SaveW( lpszPath, CXIMAGE_FORMAT_GIF);
		}
		else if( dwType == CXIMAGE_FORMAT_JPG)
		{
			return image.SaveW( lpszPath, CXIMAGE_FORMAT_JPG);
		}
		else if( dwType == CXIMAGE_FORMAT_PNG)
		{
			return image.SaveW( lpszPath, CXIMAGE_FORMAT_PNG);
		}
		else
			return image.SaveW( lpszPath, CXIMAGE_FORMAT_BMP);
	}

	return FALSE;
}

void	
CResponseParser::ParseGetUpdateConfigXml(CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	LOG_RESP();
	NEW_RESP(get_update_config_xml_event_resp, pE);
	pE->strUpdateXml = stringhelper::UTF8ToUnicode( pContext->strContentData);
	pE->m_pstExtraInfo = (ExtraInfo*)pE;

	PUSH_EVENT(*pE);
}

void	
CResponseParser::ParseUploadHeadImage( CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	LOG_RESP();

	NEW_RESP( upload_head_image_event_resp, pEvent);

	upload_head_image_event_req* pReq = NULL;

	std::map<int32, Event*>::iterator seq_iter = g_WebModule->m_mapRequestEvent.find(pContext->nReqSeq);
	if( seq_iter != g_WebModule->m_mapRequestEvent.end())
	{
		pReq = (upload_head_image_event_req*)(seq_iter->second);
	}

	string strUploadTime;
	uint32 nResult = pParser->ParseErrorXml( pContext->strContentData, strUploadTime);
	if( nResult == 0)
	{
		pEvent->param0 = WEB_RET_SUCCESS;
		pEvent->uploadTime = atol(strUploadTime.c_str());
	}
	else if( nResult == 5)
	{
		pEvent->param0 = WEB_RET_IMAGE_TYPE_ERROR;
	}
	else if( nResult == 7)
	{
		pEvent->param0 = WEB_RET_IMAGE_TOO_LARGE;
	}
	else
	{
		pEvent->param0 = WEB_RET_COMMON_ERROR_INTERNAL;
	}

	pEvent->m_pstExtraInfo = (ExtraInfo*)pEvent;

	PUSH_EVENT( *pEvent);

	delete pReq;
	g_WebModule->m_mapRequestEvent.erase( seq_iter);
}

//服务器端返回的pContext->strContentData格式为
//第一字节为是否需要下载头像图片  0为需要下载  1为不需要下载
//第二字节开始之后的10个字节为修改时间
//第十一字节开始之后的为图片数据
void
CResponseParser::ParseDownloadHeadImage(CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	LOG_RESP();

	NEW_RESP( download_head_image_event_resp, pEvent);

	uint32 nErrorCode = atoi(pContext->strContentData.substr(0,1).c_str());  //是否需要下载头像  0为需要下载， 1为不需要下载
	if(nErrorCode == 0)  
	{
		pEvent->param0 = WEB_RET_SUCCESS;
		uint32 nMidfyTime = atoi(pContext->strContentData.substr(1,10).c_str());  //获取返回的修改时间
		std::string strImageDate = pContext->strContentData.substr(11);			   //获取下载的图片数据

		HANDLE hFileHandle = 0;
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		hFileHandle = CreateFile(pContext->wstrWritedFileName.c_str(), GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

		DWORD dwWrite = 0 ;
		BOOL bRet = ::WriteFile( hFileHandle, strImageDate.c_str(), (DWORD)strImageDate.size(), &dwWrite, NULL);    //将图片数据写入文件
		
		FILETIME lastWriteTime;
		common::utility::filehelper::UinxTimeToFileTime(nMidfyTime,&lastWriteTime);
		SetFileTime(hFileHandle,&lastWriteTime,&lastWriteTime,&lastWriteTime);  //将修改时间写入图片属性

		CloseHandle( hFileHandle);

		pEvent->unUIN = pContext->uParam0;
		pEvent->wstrImagePath = pContext->wstrWritedFileName;

	}
	else
	{
		pEvent->param0 = WEB_RET_UNKNOWN;
	}

	pEvent->m_pstExtraInfo = (ExtraInfo*)pEvent;
	PUSH_EVENT( *pEvent); 
	

}
void	
CResponseParser::ParseGetDownloadedFile( CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	NEW_RESP( get_download_file_event_resp, pEvent);
	if( pContext->uResult == HTTP_SUCCESS)
		pEvent->param0 = WEB_RET_SUCCESS;
	else if(pContext->uResult == HTTP_EXISTS)
	    pEvent->param0 = WEB_REC_DOWNLOAD_EXISTS;
	else if( pContext->uResult == HTTP_FAILED && pContext->nErrorCode == ERR_TASK_WRITE_FILE_ERROR)
		pEvent->param0 = WEB_RET_COMMON_WRITE_FILE_ERROR;
	else
		pEvent->param0 = WEB_RET_UNKNOWN;	

	pEvent->m_pstExtraInfo = (ExtraInfo*)pEvent;

	PUSH_EVENT( *pEvent);
}

void	
CResponseParser::ParsePassThroughRawData( CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	LOG_RESP();
	NEW_RESP( get_http_url_event_resp, pEvent);
	pEvent->strBuffer = pContext->strContentData;
	pEvent->m_pstExtraInfo = (ExtraInfo*)pEvent;

	PUSH_EVENT(*pEvent);
}	

void 
CResponseParser::ParseGetFilterWords( CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	LOG_RESP();

	string strUtf8 = stringhelper::UnicodeToUTF8( stringhelper::UTF8ToUnicode(pContext->strContentData));

	expat_document xmlDoc;
	if (!xmlDoc.load_from_buffer(strUtf8.c_str(), (UINT32)strUtf8.size())) 
	{
		LOG_PARSE_ERROR();
		return ;
	}

	expat_element *pRoot = xmlDoc.get_root();
	if (!pRoot)  
	{
		return ;
	}

	expat_element *pkeyWord = pRoot->get_element_by_name(L"keyword");
	while (pkeyWord)
	{
		wstring szTitle = (wstring)pkeyWord->get_child_element_contents_by_name(L"title");
		if (szTitle == L"")
		{
			pkeyWord = pkeyWord->get_sibling_element(L"keyword");
			continue;
		}

		wstring szType = (wstring)pkeyWord->get_child_element_contents_by_name(L"level");
		if (szType == L"")
		{
			pkeyWord = pkeyWord->get_sibling_element(L"keyword");
			continue;
		}

		/*STFilterWords stFilterWord;
		stFilterWord.wstrFilterWord = szTitle;
		stringhelper::StringToInt(szType,(int*)&stFilterWord.nType);
		pParser->m_pWebData->AddFilterWord( stFilterWord);*/
		
		pkeyWord = pkeyWord->get_sibling_element(L"keyword");
	}

	/*Event e;
	e.srcMId = MODULE_ID_WEBSERVICE;
	e.desMId = MODULE_ID_IM;
	e.eventValue = EVENT_VALUE_IM_DOWNLOAD_FILTERWORDS_COMPLETE;
	PUSH_EVENT(e);*/
}

/**
* Function		根据给定的EventValue值对返回的响应进行解析
* @param		nEventValue 对应的事件值
* @param		lpszContentData 对应的内容
* @param		dwContentSize 读取的报文的长度
* @return 
**/
void	
CResponseParser::ParseResponse( HTTPCONTEXT* pContext )
{
	if( pContext == NULL)
		return;

	HandlerTableEntry* pEntry = &CResponseParser::m_tableDriven[0];
	while( pEntry)
	{
		
		if (pEntry->m_nEventValue==service_value_web_get_login_identify_code_req)
		{
			
		}
		if( pEntry->m_nEventValue == 0)
			break;

		if( pEntry->m_nEventValue == pContext->nEventValue
			&& pEntry->m_hHandler != NULL)
		{
			(*pEntry->m_hHandler)(this, pContext);
			break;
		}

		++pEntry;
	}
}

void	
CResponseParser::ParseGetLoginIdentifyCode( CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	//MessageBox(NULL,L"CResponseParser类中的ParseGetLoginIdentifyCode!",L"提示",MB_OK);
	LOG_RESP();
	if( pContext->strContentData.size() < 2)
		return ;

	NEW_RESP( get_login_identify_code_event_resp, pE);	

	uint32 nErrorCode = (uint32)pContext->strContentData[0]*256 + (uint32)pContext->strContentData[1];
	if( nErrorCode != 0)
	{
		pE->param0 = WEB_RET_COMMON_ERROR_INTERNAL;
		pE->m_pstExtraInfo = (ExtraInfo*)pE;

		PUSH_EVENT(*pE);

		return;
	}

	string strLength = pContext->strContentData.substr(2,2);
	unsigned short nLength = *(unsigned short*)strLength.c_str();
	nLength = ntohs( nLength);

	string strImageData = pContext->strContentData.substr(4, nLength);
	pE->strImageData.assign( strImageData.c_str(), strImageData.size());

	string strKeyLength = pContext->strContentData.substr( 4 + nLength, 2);
	unsigned short nKeyLength = *(unsigned short*)strKeyLength.c_str();
	nKeyLength = ntohs( nKeyLength);

	string strKeyData = pContext->strContentData.substr( 4 + nLength + 2, nKeyLength);
	pE->strKey.assign( strKeyData.c_str(), nKeyLength);

	// 第一位为0表示成功
	pE->param0 = WEB_RET_SUCCESS;
	pE->m_pstExtraInfo = (ExtraInfo*)pE;

	PUSH_EVENT( *pE);
}