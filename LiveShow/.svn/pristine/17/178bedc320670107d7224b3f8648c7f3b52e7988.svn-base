/******************************************************************************
 * task.h - 保存任务配置，数据，进度等信息
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
#ifndef _TASK_H
#define _TASK_H

#include <string>

#include "httptransfer.h"
//#include "httpmanager.h"


using namespace std;
//#define ShowMsg(str) printf("\n%s\n", str)

#define MAX_REDIRECTIONS 5
#define MAX_RETRY_CONNECT 3
#define MAX_CONNECT_TIMEOUT 10		//unit-seconds

class URL;
class POST_PARAMS;
class HttpHdrs;
class THttpCli;
//cocurrent Task status
typedef enum 
{
	TSK_IDLE,
	TSK_CONNECT,
	TSK_CONNECTWAIT,
	TSK_AUTH,
	TSK_START,				// Get File Size 
	TSK_RUNNING,			
	TSK_FINISHED,			
	TSK_ERROR,
	TSK_UNKNOWN
}TaskStatus; 

typedef enum
{
	CONNECT_SUCCESS = 1,		//所有链接成功，等待收发数据
	CONNECT_ASYNCWAIT = 2,		//异步connect，等待socket可用
	CONNECT_ERROR = 3,			//connect失败
	CONNECT_PROXYAUTH = 4,		//proxy链接成功，认证中
}CONNECT_STATUS;

class TTask
{

public:
	TTask(string url, TASK_TYPE task_type);
	TTask(string url, TASK_TYPE task_type, Proxy_t proxyInfo);
	~TTask();

	INT32 sendHttpReq();
	int sendHttpReqContinue();
	
	int processHttpResp();
	INT32 processHttpRespHead();	
	INT32 getRealData(); 
	INT32 tryConnect();	
	INT32 tryAuth();	
	int onAuthResp();
	int onConnected(); 
	INT32 checkConnectTimeout();
	void updateTaskInfo();
	INT32 dumpDataToFile();
	void checkTimeOut();

	TASK_TYPE task_type;
	//NV_MAP* 	  cookies;
	//NV_MAP* 	  postParams;
	//FILENAMES*	  fileNames;
	POST_PARAMS*	  postParams;
	HttpHdrs * httpHdrs;

	handle_func_t handle_init;
	handle_func_t handle_process;
	handle_func_t handle_headcomplete;
	handle_func_t handle_finish;
	handle_func_t handle_error;
	
	CHAR_VECTOR* respData;
	CHAR_VECTOR  respDataContent;
	//CHAR_VECTOR  postContent;
	ULONG64 postDataSize;


	
	THttpCli* hcp;	
	URL * url;
	TaskStatus taskState;
	int errcode;
	TaskInfo taskInfo;
	
	int timeoutlimit;
	ULONG64 last_active_time;
	ULONG64	startSize;			//按段读取http响应体的起始字节，如为0则代表不启用
	ULONG64	endSize;			//按段读取http响应体的结束字节，如为0则代表不启用
	bool dataClean;			//是否在每次回调之后将已收到的数据清空，已节省内存

	int connectTimes;
	int redirectionTimes;
	bool shouldGetFileInfo;
	bool del;
	
	bool respHeadCompleted;			//响应消息头是否接收完毕
	bool respContentCompleted;		//响应数据体是否接收完毕
	bool dataChanged;				//上传或下载数据是否有变化 

private:
	void init();	
	int setHttpHeader();
	int generateSendDataBuffer();
	
	bool useProxy;
	
	ULONG64 requestSize;			//实际要发送的数据流大小
	ULONG64 requestBufferSize;		//分配的发送缓冲区的大小
	char* requestBuffer;
	ULONG64 requestSentSize;		//已经发送的数据的大小	
};
#endif  /* _TASK_H */
