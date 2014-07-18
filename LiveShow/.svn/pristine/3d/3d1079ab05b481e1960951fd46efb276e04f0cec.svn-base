#pragma once

/****************************************************************************
* ClassName:   CResponseParser												*
* Author:	   Zhang Z.Q													*
* Using:	   该类负责对响应的报文数据进行XML解析，然后将其保存到数据中心	*
*																			*
* Verion:																	*
*		Created			2007,12,12											*
*****************************************************************************/
#include "webdefine.h"
#include <string>
#include <map>
#include "ximage.h"
#include "HttpContext.h"
#include "../CommonLibrary/src//HttpClient/include/httpmanager.h"

using namespace std;
using namespace webinterface;
using namespace httpservice;

#define	RET_ERROR_XML_FILE		1			// 返回的XML文件错误，无法解析
#define	RET_ERROR_RESPONSE		2			// 发生错误的时候服务器的返回报文，通常是执行不成功
#define RET_ERROR_NO_WEBDATA	3			// 解析过程中没有指定解析数据仓库
#define RET_ERROR_NO_HANDLER	4			// 没有找到对应的解析函数
#define RET_INNER_USED			5			// 该解析过程内部使用，不需要对外提供
											// 比如对于用户形象照的获取就应该返回该值
#define RET_SUCCESS				7			// 执行成功，数据解析后放入到数据中心


class CWebData;
class CRequestManager;
class CResponseParser
{
public:
	CResponseParser(void);
	~CResponseParser(void);

	void			InitHandlerTableEntry();

	static string	ConvertCharVector( CHAR_VECTOR* pVector);
	static DWORD	CheckImageFileHeader(const BYTE *pBuf, uint32 nBufLen);
	static DWORD	CheckImageFileHeader(const wstring wstrImagePath);

	// 将网站返回的错误码翻译成返回的内部错误码
	static DWORD	GetReturnError( UINT	nErrorCode);

	void			SetUin( uin nUin) { m_nUin = nUin;}
	uin				GetUin() { return m_nUin;}

	void				SetReuqestManager( CRequestManager*	pRequestManager);
	CRequestManager*	GetRequestManager() { return m_pRequestManager;}

	/**
	 * 统一解析类似于如下的XML格式
	 * <a>
		<v Error="11">dd</v>
	 * </a>
	 * 函数返回Error的值，只有返回值为0的时候strContent才是有效值
	 */
	uint32	ParseErrorXml( string strSrc, string& strContent);

	/**
	 * Function		根据给定的EventValue值对返回的响应进行解析
	 * @param		nEventValue 对应的事件值
	 * @param		lpszContentData 对应的内容
	 * @param		dwContentSize 读取的报文的长度
	 * @param		nEltsNum 对于获取好友列表而言，需要返回一个值，只是当前的好友的数目，以便调用者分配内存空间
	 * @return 
	 **/
	void	ParseResponse( HTTPCONTEXT* pContext);

private:
	//==========================================================
	//              内部使用的辅助函数
	//==========================================================

	static void	ParseGetUpdateConfigXml( CResponseParser* pParser, HTTPCONTEXT* pContext);
	static void ParseGetFilterWords( CResponseParser* pParser, HTTPCONTEXT* pContext);

	/**
	 * 头像相关     
	 */	
	static void	ParseUploadHeadImage( CResponseParser* pParser, HTTPCONTEXT* pContext);
	static void ParseDownloadHeadImage(CResponseParser* pParser, HTTPCONTEXT* pContext);

	/**
	 * 文件下载     
	 */
	static void	ParseGetDownloadedFile( CResponseParser* pParser, HTTPCONTEXT* pContext);

	/**
	 * 通用Get接口，处理透传数据    
	 */
	static void	ParsePassThroughRawData( CResponseParser* pParser, HTTPCONTEXT* pContext);


	static void ParseGetLoginIdentifyCode( CResponseParser* pParser, HTTPCONTEXT* pContext);
	
	BOOL	SaveAsImage( const wchar_t* lpszPath, const char* lpszContentData, 
			uint32 dwContentSize, DWORD dwType = CXIMAGE_FORMAT_BMP);


private:

	CRequestManager*	m_pRequestManager;		// 处理器在获取到图片URL的时候需要调用CReuqestManager，
												// 请求其返回图片数据
	uin					m_nUin;					// 正在处理的用户的Uin

	// 处理句柄指针
	typedef void (*ParseHandler)( CResponseParser* pParser, HTTPCONTEXT* pContext );

	typedef struct _HanderTable
	{
		EventValue		m_nEventValue;		//	对应的事件值
		ParseHandler	m_hHandler;			//	处理句柄

	} HandlerTableEntry;
	static HandlerTableEntry m_tableDriven[];
};
