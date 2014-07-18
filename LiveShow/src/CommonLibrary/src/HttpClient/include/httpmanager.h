/******************************************************************************
 * httptransfer.h - 外部应用通过TranferMgr来使用HttpTranfer的各项功能。
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     本文件声明了TranferMgr类，HttpTranfer的所有外部接口均通过TranferMgr提供，外部应用使用httpDownloaer组件应以此文件为接口标准
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
#pragma once


#include <string>
#include <vector>
#include "..\..\..\include\Common.h"
#include "httptransfer.h"

using namespace std;

class TTask;
class TTransferWorker;

class COMMON_INTERFACE POST_PARAMS
{
private:
	PARAMS_V postParams;

public:	
	//当paramType为TEXT时buffer为param的值字符串，bufferLen为字符串长度+1
	//当paramType为PARAM_BINARY时buffer存储2进制流，大小由bufferLen决定
	//当paramType为PARAM_FILENAME时buffer为文件名，字符串，bufferLen为字符串长度+1
	INT32 add_param(const char* paramName,
					const char* buffer,	
					UINT32 bufferLen,
					PARAM_TYPE paramType = PARAM_TEXT);
	
	PARAMS_V* get_params(){	return &postParams; };	

	// 检查是上传类型
	bool	is_uploadtype();

	bool	delete_param( const string strParamName);
	
	POST_PARAMS::~POST_PARAMS();
	POST_PARAMS(POST_PARAMS &in_post_param);
	POST_PARAMS(){};

};

class THttpHdr
{
private:
    string m_strKey;
    string m_strVal;
public:
    THttpHdr( void );
    THttpHdr( const string& strKey );
    THttpHdr( const string& strKey, const string& strVal );

    const string&  GetKey( void ) const;
    const string&  GetVal( void ) const;
    void  SetVal( const string& strVal );

};


class COMMON_INTERFACE THttpMsg
{
private:
    UINT32          m_nHttpVer;         // HTTP version (hiword.loword)
	string 			m_statusLine;
    THttpHdrList    m_listHdrs;
public:
    THttpMsg( void );
    THttpMsg( const THttpMsg& other );
    virtual ~THttpMsg( void );

    // Total header length for key/val pairs (incl. ": " and CRLF)
    // but NOT separator CRLF
		
    size_t		GetAllHdrLen( void ) ;
	size_t		GetHdrLen( UINT nIndex ) ;
	void		ClearAllHdr(void);

    void		GetHttpVer( UINT* puMajor, UINT* puMinor ) const;
    void		SetHttpVer( UINT uMajor, UINT uMinor );

    size_t		GetHdrCount( void ) const;
    string		GetHdr( const string& strKey ) ;
    THttpHdr*	GetHdr( UINT nIndex ) ;
    void		SetHdr( const string& strKey, const string& strVal );
    void		SetHdr( const THttpHdr& hdrNew );
	void		SetHdrDuplicate( const string& strKey, const string& strVal );
	void		SetHdrDuplicate( const THttpHdr& hdrNew );
	
	void		ShowAllHttpHdr(void);

};


//HttpHdrs 维护一个http头集合，对外提供Set,Delete 接口
class HttpHdrs
{
private:
    THttpHdrList    m_listHdrs;
	
public: 
	//无条件写入http头，即使和已有http头重复也继续写入
	void		SetHdr( const string& strKey, const string& strVal );
	//根据key获取http头的值，如果有多个该key的值，则返回第一个
	string		GetHdr( const string& strKey);	
	//获取相同key的http头的第index个元素，如cookie
	string		GetHdr( const string& strKey,  int index) ;
	//获取内部headers链表指针
	THttpHdrList*	GetHdrs(){return &m_listHdrs;};
	//根据key删除http头，如果有多个该key的值，则删除第一个值
	void		DeleteHdr( const string& strKey ) ;	
};

namespace httpservice
{
class COMMON_INTERFACE TranferManager
{
	
public :
	
/******************************************************************************
 * getInstance - 返回一个TranferManager实例的指针，供外部应用使用。在一个进程仅存在一个TranferManager实例
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	static TranferManager* getInstance();
	static void delInstance(); 
	
private:
	static TranferManager* instance;
public:

/******************************************************************************
 * setMaxRunningTaskNum - 设置最大可同时进行下载的任务数，如果达到此数，后续的任务将排队
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 返回之前的最大任务数
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	UINT32 setMaxRunningTaskNum(UINT32 maxRunningTaskNum);

/******************************************************************************
 * setProxy - 设置代理服务器信息
 * DESCRIPTION: - 
 *	  该接口用户设置新的全局代理服务器，或替换之前的代理服务器 
 *	  如果proxytype为HTTPDOWN_PROXY_BUTT，则为取消当前代理设置，该设置只对之后添加的任务有效
 * Input: 
 * Output: 0成功 <0 错误(如proxyType 类型不支持) 1 -替换之前的代理服务器 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 setProxy(Proxy_t 	proxy);

/******************************************************************************
 * start - 启动HttpTranfer
 * DESCRIPTION: - 
 *	  N/A 
 * Input: 
 * Output: 
 * Returns: 0成功 <0 错误
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 start();

/******************************************************************************
 * addTask - 添加一个任务，并提供回调函数
 * DESCRIPTION: 
 * Input:  
  \param url 目的地址(下载时为要下载的文件，上传时为要上传到的地址)
  \param task_type 任务类型，upload/download
  \param postParams  用于存储Post参数列表, 有上层应用传入，httptransfer会进行深度拷贝，
  				上层可在addTask调用完成释放
  \param httpHdrs    用于存储http头列表, 有上层应用传入，httptransfer负责释放
  \param callbacks	 回调函数集
  \param OPTIONS	 选项集合
 * Output: 
 * Returns: 
 * 
 >0 --- 任务创建成功，返回该任务的索引值
 <0 --- 任务创建失败，返回错误码,参考enum定义ERR_TASK 
 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 addTask(string url, 
					TASK_TYPE	  task_type,
					POST_PARAMS*	  postParams,
					CALL_BACK_S   callbacks,
					OPTIONS 	  options,
					HttpHdrs * httpHdrs = NULL);


/******************************************************************************
 * clear - 清除HttpTranfer中的所有任务
 * DESCRIPTION: - 此处仅将指定任务删除标记置位，因此实际上任务并没有停止，
 * 	真正停止任务由工作线程扫描任务队列时完成. 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 0成功 <0 错误
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 clear();

/******************************************************************************
 * stop - 停止httptransfer
 * DESCRIPTION: - 
 *	  停止工作线程，停止下载中的所有任务，停止动作需要一定时间才能完成，
	  但本函数会立即返回。因此如果stop后立刻start，可能会失败 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 14nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 stop();

/******************************************************************************
 * cancelTask - 删除指定任务。同时获取指定任务的信息
 * DESCRIPTION: - 此处仅将指定任务删除标记置位，因此实际上任务并没有停止，
 * 	真正停止任务由工作线程扫描任务队列时完成
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 0 -- 成功  ERR_TASK_NOT_EXIST ---指定任务不存在(可能已被删除)
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 cancelTask(UINT32 taskID, TaskInfo& taskinfo);



/******************************************************************************
 * getTaskInfo - 获取指定task的信息
 * DESCRIPTION: - 
 *    n/a 
 * Input: 
 * Output: 
 * Returns: 0获取成功 <0 获取错误
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	//INT32 getTaskInfo(UINT32 taskID, TaskInfo& taskinfo);


/******************************************************************************
 * setLogPath - 设置日志路径
 * DESCRIPTION: - 
 *	  local_log_path 为日志文件的绝对路径，httptransfer不对改参数有效性(路径是否存在等)
       进行检查, 在windows下local_log_path必须为如下格式
       "c:\\log\\httptransfer" ---需要是双反斜杠
 * Input: 
 * Output: 
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	void setLogPath(char* local_log_path);
	

	bool mgrStarted;
private:
	TTask** taskQue;
	Proxy_t proxyInfo;
	TTransferWorker* pWorker;

	UINT32 lastTaskQuePos;

	UINT32 waitingTaskCount;
	UINT32 runningTaskCount;
	UINT32 finshedTaskCount;
	UINT32 errorTaskCount;

	UINT32 maxRunTaskNum;
	bool useProxy;

	TranferManager();
	~TranferManager();
	
/******************************************************************************
 * TranferManager.taskParamCheck - 检查任务参数是否满足最基本要求
 * DESCRIPTION: - 
 *	  n/a 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 24nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 taskParamCheck(string url, 
				TASK_TYPE	  task_type,
				POST_PARAMS*  postParams,
				CALL_BACK_S	  callbacks,
				OPTIONS		  options, 
				HttpHdrs * httpHdrs,
				ULONG64& postDataSize);
	
};
};


