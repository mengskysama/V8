#pragma once

#include "winhttp.h"
#include "string"
#include <list>
#include <map>
#include <vector>
#include "webdefine.h"
#include "RequestManager.h"

#include "utility/lock.h"
#include "utility/Lock.h"
#include "../CommonLibrary/src/HttpClient/include/httpmanager.h"

using namespace std;
using namespace webinterface;
using namespace httpservice;

class HttpClient  
{
public:
	virtual ~HttpClient( );

	void	SetNoProxy();

	static	HttpClient*	Instance();

	/**
	 *	设置和获取某一序列的上下文
	 */
	HTTPCONTEXT*	GetContext( unsigned int nSeqNo);

	/**
	 * Function		使用Get方法请求特定的URL		
	 * @param		strUrl 是需要请求的URL，比如"http://www.51.com"
	 * @param		nSeqNo 是该请求对应的序列号
	 * @return		0, 成功, 或者 <0 失败，返回的错误码
	 **/
	int32	ReqURL( HTTPCONTEXT* pContext);
	int32	ReReqURL( HTTPCONTEXT* pContext);

	/**
	 * Function		发送HTTP Post请求
	 * @param		strUrl 是需要请求的URL	
	 * @param		nSeqNo 是该请求对应的
	 * @param		rArgument 是post参数
	 * @return 
	 **/
	int32	ReqPostUrl( HTTPCONTEXT* pContext);
	int32	ReReqPostUrl( HTTPCONTEXT* pContext);

	/**
	 * Function		取消特定的任务
	 * @param		nSeqNo	需要取消的SeqNo
	 */
	void	CancelTask( uint32 nSeqNo);

	/**
	 * Function		从返回队列中取出一个响应数据
	 * @param		stContext 取出的响应上下文，其中包括了从服务器返回的报文数据
	 * @return 
	 **/
	HTTPCONTEXT	RetriveResp( );

	/**
	 * Function		通过任务taskid获取该任务对应的内部序列号
	 * @param		
	 */
	int32	GetSeqNo( int32	nTaskId);
	void	ClearSeqIdMap( uint32 nSeqNo);

	void	Cleanup( HTTPCONTEXT* pSession);
    void    DeleteSeqNo( uint32 nSeqNo);
	void	CleanAll();


private:
	HttpClient( );
	static HttpClient*	Singleton;

	/**
	* Function		检测IE的代理欣喜		
	* @return		IE的代理信息保存在参数pProxyInfo中
	**/
	static BOOL DetectIEProxySettings(WINHTTP_PROXY_INFO* pProxyInfo);

private:

	map<uint32, HTTPCONTEXT*>		m_mapRequest;
	map<int32,	uint32>				m_mapTaskid2Seqno;		//	从任务id到内部序列号的映射
	map<uint32, int32>				m_mapSeqNo2TaskId;		//	从内部序列号到任务ID的映射

	list<HTTPCONTEXT>				m_lstResult;
	common::utility::CSWrapper				m_Lock;
	TranferManager*					m_pTransferManager;
	CALL_BACK_S						m_Callback;
};