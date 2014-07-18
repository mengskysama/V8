#pragma  once

#include <string>
#include <map>
#include "httpmanager.h"
#include "Common.h"


// 描述一次请求的上下文
struct HTTPCONTEXT
{
	HTTPCONTEXT(string& url)
	{
		strURL = url;
		bAllowRepeated = false;
		uTaskId = 0;
		pTaskInfo = NULL;
	}

	string		strURL;				//	当前请求的URL
	bool		bAllowRepeated;		//	该URL是否允许重复请求

	uint32		uTaskId;			//	任务的ID
	TaskInfo*	pTaskInfo;			//	该URL对应的任务结构
};

class HttpRequest
{

};