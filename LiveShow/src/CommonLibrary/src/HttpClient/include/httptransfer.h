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
#ifndef _HTTPTRANSFERMGR_H
#define _HTTPTRANSFERMGR_H

/*********使用必读*****************************/
/*********发布说明*****************************
*********编译模块控制宏说明*******************
***** STATIC_COMPILE   --- 如定义此宏，则为编译静态库版本，
* 否则默认为动态库版本

***** HTTPTRANSFER_RELEASE --- 如定义此宏，则不启用LOG模块，
* 不需要加载LOG模块所用的utils-mt-gd.lib库，
* 也不启用自动将响应数据写文件功能
**********************************************
**********************************************
**********************************************/


#ifdef WIN32

#ifndef STATIC_COMPILE
#ifdef HTTPCLIENT_EXPORTS
#define HTTPCLIENT_EXPORTS __declspec(dllexport)
#else
#define HTTPCLIENT_EXPORTS __declspec(dllimport)
#endif
#else
#define HTTPCLIENT_EXPORTS
#endif
#else
#define HTTPCLIENT_EXPORTS 
#endif

#ifdef WIN32
#include <process.h>
//#include <windows.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <list>


#include "Types.h"


using namespace std;

//#define MAX_TASKQUE_SIZE 1024

typedef vector<char> CHAR_VECTOR;		//用于二进制数据流存储

/******************************************************************************
* TASK_TYPE - 定义任务类型
* 注:
* TASK_DOWNLOAD 请求包只有http头，无请求体
* TASK_QUERY 和 TASK_UPLOAD 为使用Multi-parts方式组请求体
* TASK_BINARY 为使用2进制数据方式组请求体, 参数值存储在POST_PARAMS中，
  只能有一个参数，且参数类型必须为 PARAM_BINARY, 参数名为anonymous
* DESCRIPTION: - 
***/
typedef enum
{
	TASK_DOWNLOAD = 0,		//使用Get从服务器下载文件
	TASK_QUERY    = 1,		//使用Post向服务器查询相关数据
	TASK_UPLOAD   = 2,		//使用Post向服务器上传文件
	TASK_BINARY   = 3,		//使用Post向服务器发起单2进制参数请求---需要特殊处理
	TASK_HEAD     = 4,		//使用HEAD向服务器获取http响应头
	TASK_BUTT = 255,
}TASK_TYPE;

/******************************************************************************
* ProxyType - 定义使用的代理类型
* DESCRIPTION: - 
***/
typedef enum 
{	
	HTTPDOWN_PROXY_HTTP = 0,			//使用http代理
	HTTPDOWN_PROXY_SOCK4 = 1,		//使用sock4代理
	HTTPDOWN_PROXY_SOCK5 = 2,		//使用sock5代理	
	HTTPDOWN_PROXY_BUTT = 255,
}HTTPDOWN_ProxyType;


/******************************************************************************
* Proxy_t - 代理服务器的信息，暂不启用
* DESCRIPTION: - 
***/
typedef struct _Proxy_t
{
	HTTPDOWN_ProxyType	type;		//必填
	string		host;		//必填,代理服务器地址
	string		user;		//选填
	string		pwd;		//选填
	string		domain;		//选填
	int			port;		//必填
}Proxy_t;

class THttpMsg;

typedef struct _TaskInfo
{
		//TaskStatus status;		//任务当前状态
		//UINT32 taskid;			//任务序号
		TASK_TYPE	taskType;		//任务类型
		LONG64		contentLength;	//目标文件大小 --- -1代表未知大小
		time_t		lastModified;   //目标文件最后修改时间
		ULONG64		doneSize;		//任务已完成的字节数
		ULONG64		transferSpeed; 	//传输速率，以Byte为单位
		LONG64		remainTime;		//估计的剩余时间 --- -1代表未知大小
		ULONG64		addTime;			//任务添加时间
		ULONG64		startTime;		//任务开始时间
		ULONG64		endTime;		   	//任务结束时间
		INT32		errcode;			//任务的错误码 --- 仅在调用错误回调函数是有效
		INT32		httpCode;		//http 响应码
		THttpMsg*	htrequest;	//响应头信息		
		THttpMsg*	htresponse;	//响应头信息
		char		infomsg[1024];  	//任务的文本信息
		CHAR_VECTOR* respData;	//响应数据体 
}TaskInfo;

typedef enum
{
	ERR_TASK_OK = 0,
	ERR_TASK_FULL,
	ERR_TASK_PROXY_CONNECT_FAIL,
	ERR_TASK_PROXY_AUTH_FAIL,
	ERR_TASK_THREAD_START_FAIL,
	ERR_TASK_MGR_NOT_START,		//---5

	ERR_TASK_ALREADY_STARTED,	//重复启动错误，可能是之前的stop动作尚未完成导致
	ERR_TASK_SOCKET_INIT_FAIL,
	ERR_TASK_SOCKET_WRONG,
	ERR_TASK_UNKNOWN_STATUS,
	ERR_TASK_NOT_EXIST,			//---10
								
	ERR_TASK_SOCKET_SEND,
	ERR_TASK_SOCKET_RECV,
	ERR_TASK_CONNECT_FAIL,
	ERR_TASK_REDIRECTION_EXCEED,
	ERR_TASK_REDIRECTION_NOTARGET,	//---15
	
	ERR_TASK_PARSE_RESQ_FAIL,
	ERR_TASK_READ_RESQ_FAIL,

	
	ERR_TASK_PARAM_URL,				//URL 不合法
	ERR_TASK_PARAM_RESPDATA,		//返回数据容器为空
	ERR_TASK_PARAM_CALLBACK,		//缺少回调函数  ---20
	
	ERR_TASK_PARAM_POSTPARAM_NONE,	//POST参数为空
	ERR_TASK_PARAM_POSTPARAM_EXCEED,//POST参数超出限制
	ERR_TASK_PARAM_POSTPARAM_NAME,	//Post 参数的参数名不合法
	ERR_TASK_PARAM_POSTPARAM_LEN,	//Post 参数的参数值长度不合法
	ERR_TASK_PARAM_POSTPARAM_VALUE, //Post 参数的参数值不合法					---25

	ERR_TASK_PARAM_POSTPARAM_FILENOTEXIST,	//Post 参数中指定的文件不存在		
	ERR_TASK_POSTPARAM_TYPE_INVALID,	//Post 参数的参数类型不合法
	ERR_TASK_OPTION_SIZE_INVALID,
	ERR_TASK_NO_FILE,		
	ERR_TASK_NO_MEMORY,
	ERR_TASK_FILE_READ,							//---30

	ERR_TASK_FILE_CANNOT_OPEN,			
	
	ERR_TASK_FILE_CHANGED,
	ERR_TASK_TIMEOUT,

	//后续根据开发情况补充
	ERR_TASK_WRITE_FILE_ERROR,			
}ERR_TASK;


typedef enum
{
	PARAM_TEXT = 0,
	PARAM_BINARY = 1,
	PARAM_FILENAME = 2,
}PARAM_TYPE;

/******************************************************************************
* handle_func_t 任务回调函数类型定义，上层应用应该按此定义中的约定实现回调函数
* DESCRIPTION: - 回调函数应该尽量简单，迅速返回, 且保证安全性，tranferMgr不对调用回调函数中的问题负责
* param1: INT --- 为任务ID  param2: TaskInfo* --- 为任务详细信息指针
* 不建议回调函数中对TaskInfo进行修改
* 返回值:约定 0 ---成功 <0 ---失败 >0 ---其它情况
***/
typedef INT32     (*handle_func_t)     (INT32, const TaskInfo*);

struct CALL_BACK_S			//回调函数集合
{
	CALL_BACK_S()
	{
		handle_init = NULL;
		handle_headcomplete = NULL;
		handle_process = NULL;
		handle_finish = NULL;
		handle_error = NULL;
	}

	handle_func_t handle_init;			//任务初始化完成后的回调函数
	handle_func_t handle_headcomplete;	//收到完整响应头时的回调函数
	handle_func_t handle_process;		//任务进行中的回调函数	
	handle_func_t handle_finish;		//任务完成后的回调函数 ---必须存在
	handle_func_t handle_error;			//任务出错时调用的回调函数 ---必须存在
};

struct OPTIONS		//任务选项集合
{
	OPTIONS()
	{
		tasktimeout = 0;
		startSize = 0;
		endSize = 0;
		dataClean = 0;
	}

	UINT		  tasktimeout;
	ULONG64		  startSize;			//按段读取http响应体的起始字节，如为0则代表不启用
	ULONG64		  endSize;				//按段读取http响应体的结束字节，如为0则代表不启用
	bool          dataClean;			//是否在每次回调之后将已收到的数据清空，已节省内存
};

const int MAX_TASK_NUM = 1024;


#define MAX_PARAM_NAME_LEN 64
typedef struct
{
	char paramName[MAX_PARAM_NAME_LEN]; 	//存储参数名

	//PARAM_TEXT:此处存储该param的值
	//PARAM_FILE:此处存储上传文件的文件路径
	//PARAM_BINARY:此处存储欲上传的2进制数据
	char* paramBuffer;
	
	//PARAM_TEXT:无效
	//PARAM_FILE:此处存储上传该文件时的偏移 ---不得超出文件大小
	//PARAM_BINARY:此处存储paramBuffer中2进制数据的长度
	UINT32 paramBufferLen;
	PARAM_TYPE paramType;
}POST_PARAM;

typedef vector<POST_PARAM> PARAMS_V;//用于存储Post的参数列表


class THttpHdr;
typedef std::list<THttpHdr*> THttpHdrList;


#define HTTPTRANSFER_LOG_LEVEL 5

#endif  /* _HTTPTRANSFERMGR_H */

