#pragma once

#include "51ktv.h"
#include <vector>

using namespace std;

namespace webinterface
{
	/**
	* 定义请求处理的响应结果
	*/
	#define INVALID_SEQNO	0		// 无效的序列号
	#define READY_SEQNO		1		// 数据已经就绪，不需要去网站访问
	#define NOT_READY_SEQNO	2		// 不需要访问网站，但是数据不存在

	/**
	 * 小助手编号定义     
	 */
	#define ASS_SHORTHAND	1		// 记事本
	#define ASS_REMIND		2		// 提醒器
	#define ASS_NETDISK		3		// 网络硬盘
	#define ASS_MUSIC		4		// 音乐
	#define ASS_MANAGER_CENTER	5	// 51空间
	#define ASS_PHOTO		6		// 相册

	enum TypePostArgument
	{	// POST 参数类型 
		UNKNOWN	=	0,
		TEXT,			// 普通文本
		BINARY,			// 二进制数据
		TFILE,			// 上传的是本地的文件
	};	
	
	//事件的相关定义，消息属于消费型，webinterface中获取所有的非持久型数据均通过事件接口完成
	enum webinterface_event_value
	{	
		// 所有的RESP定义如下
		// param0 WEB_RET_SUCCESS为成功，其余为失败
		// param1 下载的速度
		// param2 对应响应的请求序列SeqNo

		// 设定： resp消息总是比对应的req消息的值大一
		event_value_web_begin = EVENT_VALUE_WEBSERVICE_BEGIN, //0x10005001	

		event_value_web_get_update_config_xml_req,			//	获取XML升级配置信息
		event_value_web_get_update_config_xml_resp,			//	响应
		event_value_web_get_filter_words_req,				//	获取敏感词汇请求
		event_value_web_get_filter_words_resp,				//	获取敏感词汇响应
		event_value_web_judge_filter_words_req,				//	判断给定的字符串中是否包含有敏感词汇
			
		// 用户头像
		//event_value_web_upload_head_image_req,			//	上传用户头像
		event_value_web_upload_head_image_resp,				//	上传用户头像响应

		event_value_web_download_head_image_resp,			//   下载用户头像响应
		
		// 获取文件接口
		//event_value_web_download_file_req,				//	获取文件接口
		event_value_web_download_file_resp,					//	获取文件响应
		//event_value_web_query_download_process_req,		//	查询文件的下载进度
		
		// 通用接口
		//event_value_web_get_http_req,						//	通用的接口，Web模块并不对其进行解析，上层对其进行处理
		event_value_web_get_http_resp,		
		event_value_web_cancel_http_task,					//	取消HTTP TASK

		//event_value_web_get_login_identify_code_req,		//	获取登陆验证码
		event_value_web_get_login_identify_code_resp,		//	获取登陆验证码响应
					
		event_value_web_end = EVENT_VALUE_WEBSERVICE_END ,	//  所有的事件结束
	};

	
	// CallService相应的值
	enum webinterface_service_value
	{
		service_value_web_begin = SERVICE_VALUE_BEGIN(MODULE_ID_WEBSERVICE),

		service_value_web_get_login_identify_code_req,		//	获取登陆验证码
		service_value_web_download_file_req,                //	获取文件接口	
		service_value_web_query_download_process_req,       //  查询文件的下载进度
		service_value_web_get_http_req,                     //	通用的接口，Web模块并不对其进行解析，上层对其进行处理
		service_value_web_upload_head_image_req,			//	上传用户头像
		service_value_web_download_head_image_req,		    //  下载用户头像

		service_value_web_end = SERVICE_VALUE_END(MODULE_ID_WEBSERVICE),
	};


	struct serviceValueResp
	{
		EventValue GetServiceValueResp(ServiceValue const x)  //获取某service_value事件值的对应返回event_value事件值
			{
				EventValue respEvent = 0;
				switch(x)
				{
				case service_value_web_get_login_identify_code_req:    //获取登陆验证码
					respEvent = event_value_web_get_login_identify_code_resp;  //获取登陆验证码响应
					break;

				case service_value_web_download_file_req:           //	获取文件接口
					respEvent = event_value_web_download_file_resp; //	获取文件响应
					break;

				case service_value_web_get_http_req:                //	通用的接口 
					respEvent = event_value_web_get_http_resp;      //	通用的接口响应
					break;

				case service_value_web_upload_head_image_req:			//	上传用户头像
					respEvent = event_value_web_upload_head_image_resp; //	上传用户头像响应
					break;
					
				case service_value_web_download_head_image_req:    //下载用户头像
					respEvent = event_value_web_download_head_image_resp;  //下载用户头像响应
					break;


				default:
					break;

				}

				return respEvent;

			}

	};
	
	
	//消息的相关定义，消息属于广播型，webinterface中获取所有的持久型数据均通过消息接口完成
	enum webinterface_msg_value
	{
		// 所有的Message定义如下
		// param0 WEB_RET_SUCCESS为成功，其余为失败

		message_value_web_begin = MESSAGE_VALUE_WEBSERVICE_BEGIN,

		message_value_web_assistant_image_arrive,			//	小助手的头像已经到达
															//	param0, WEB_RET_SUCCESS，表示上传成功，其余为失败
															//	param1, 小助手的ID

		message_value_web_end = MESSAGE_VALUE_WEBSERVICE_END,
	};

	// 返回给上层调用者的错误响应信息
	enum web_resp_define
	{
		WEB_RET_SUCCESS						=		5000,			// 成功获取到用户的信息
		WEB_RET_INNER_USED					=		5002,
		WEB_RET_DOWNLOAD_ERROR				=		5003,			// 下载失败，重新下载
		WEB_REC_DOWNLOAD_EXISTS             =       5004,			// 要下载的文件已经在存且与远程文件一至，不用下载


		// 网络相关的信息
		WEB_RET_NET_ERROR_URL				=		900,	// 请求的URL错误
		WEB_RET_NET_ERROR_CONNECT			=		901,	// 无法连接远程服务器
		WEB_RET_NET_ERROR_OPEN_REQUEST		=		902,	// 无法打开请求
		WEB_RET_NET_ERROR_SETCALLBACK		=		903,	// 设置Callback函数失败
		WEB_RET_NET_ERROR_SENDREQUEST		=		904,	// 发送请求失败
		WEB_RET_NET_ERROR_SET_OPTION		=		905,	// 设置选项错误
		WEB_RET_NET_ERROR_QUERY_HEADER		=		906,	// 查询HTTP头失败
		WEB_RET_NET_ERROR_QUERY_BODY_DATA	=		907,	// 查询报文体数据失败
		WEB_RET_NET_ERROR_READ_DATA			=		908,	// 读取报文体数据失败
		WEB_RET_NET_ERROR_RECEIVE_BODY_DATA	=		909,	// 接受报文体数据失败
		WEB_RET_NET_ERROR_TIMEOUT			=		910,	// 接受报文超时
		WEB_RET_NET_ERROR_ADDHEADER			=		911,	// 增加HTTP头失败
		WEB_RET_NET_ERROR_OPEN_SESSION		=		912,	// 打开HTTP会话失败

		// 通用错误类型
		WEB_RET_COMMON_NO_URL				=		1000,	// 请求的URL不存在
		WEB_RET_COMMON_METHOD_NOT_SUPPORT	=		1001,	// 客户端请求的方法不支持
		WEB_RET_COMMON_NO_PERMIT			=		1002,	// 客户端请求的资源无权限访问
		WEB_RET_COMMON_NO_AUTH				=		1003,	// 客户端没有通过授权
		WEB_RET_COMMON_SERVER_INTERNAL_ERROR=		1004,	// 服务器内部错误
		WEB_RET_COMMON_SERVER_NOT_AVAIL		=		1005,	// 服务器暂时不可用
		WEB_RET_COMMON_SERVER_NOT_IMPL		=		1006,	// 服务器尚未实现请求方法
		WEB_RET_COMMON_PROXY_NEED_AUTH		=		1007,	// 代理服务器需要验证

		WEB_RET_COMMON_NO_BODY_DATA			=		1020,	// 没有接收到报文体数据
		WEB_RET_COMMON_NO_WEB_DATA			=		1021,	
		WEB_RET_COMMON_NOT_LOGIN			=		1022,	// 用户尚未登录
		WEB_RET_COMMON_ERROR_PARAM			=		1023,	// 传入的参数错误
		WEB_RET_COMMON_ERROR_TOKEN			=		1024,	// 传入的TOKEN错误
		WEB_RET_COMMON_ERROR_AUTH			=		1025,	// 传入的auth错误
		WEB_RET_COMMON_TOKEN_TIMEOUT		=		1026,	// Token超时
		WEB_RET_COMMON_ERROR_INTERNAL		=		1030,	// XML内部解析错
		WEB_RET_COMMON_WRITE_FILE_ERROR		=		1031,	// 写入文件失败

		// 业务相关错误
		WEB_RET_NO_DATA						=		1100,	// 数据库中没有内容
		WEB_RET_NO_UIN						=		1101,	// 没有提供uin
		WEB_RET_UIN_TOO_LONG				=		1102,	// 提供的uin太长
		WEB_RET_INALID_TOKEN				=		1103,	// 提供的token无效
		WEB_RET_UPLOAD_FAILED				=		1104,	// 上传失败
		WEB_RET_ERROR_OLD_PASSWORD			=		1105,	// 提供的旧额密码错误
		WEB_RET_INVALID_OLD_PASSWORD		=		1106,	// 修改密码时候给定的旧密码非法无效
		WEB_RET_INVALID_NEW_PASSWORD		=		1107,	// 修改密码时候给定的旧密码非法无效
		WEB_RET_IMAGE_TOO_LARGE				=		1108,	// 上传头像太大
		WEB_RET_IMAGE_TYPE_ERROR			=		1109,	// 图像类型错误		
		WEB_RET_UNKNOWN						=		1120,	// 未知的错误
		WEB_REC_NONEED_DOWN				    =		1121,   // 服务器返回文件无需下载
	};

	/*
	* 对于下面的Event，如没有特殊说明，则遵循下面的约定
	* param0 为 返回码,WEB_RET_SUCCESS为返回成功
	* param2 为返回请求对应的SeqNo
	*/

	//=======================================================================//
	//                   WebInterface中所使用到的event						 //
	//=======================================================================//

	struct Web_Req_Event : public ExtraInfo, public Event
	{
	public:
		Web_Req_Event()
		{
			bAllowOffline = FALSE;
			bAllowRepeated = TRUE;
			srcMId	=	0;
			desMId	=	MODULE_ID_WEBSERVICE;
		}

		bool	bAllowOffline;		//	是否允许离线操作, TRUE为允许，FALSE为不允许
		bool	bAllowRepeated;		//	该URL是否允许重复
	};

	struct Web_Resp_Event :	public ExtraInfo, public Event
	{
	public:
		Web_Resp_Event()
		{
			srcMId = MODULE_ID_WEBSERVICE;
			param0	= WEB_RET_SUCCESS;
		}

	};

	/**
	 *  获取更新配置文件
	 *  event_value_web_get_update_config_xml_req    
	 */
	struct get_update_config_xml_event_req	:	
		public	Web_Req_Event
	{
	public:
		get_update_config_xml_event_req()
		{
			eventValue = event_value_web_get_update_config_xml_req;
			srcMId = MODULE_ID_HALL;
			bAllowOffline = TRUE;
		}
	};
	
	/**
	 *	获取配置文件的返回值
	 *  event_value_web_get_update_config_xml_resp    
	 *	param0  为返回值，WEB_RET_SUCCESS为成功，其余为失败
	 *  param2 为返回请求对应的SeqNo
	 */
	struct get_update_config_xml_event_resp	:	
		public	Web_Resp_Event
	{
	public:
		get_update_config_xml_event_resp()
		{
			eventValue = event_value_web_get_update_config_xml_resp;
			desMId = MODULE_ID_HALL;
		}

		wstring	strUpdateXml;			//	获取的更新XML
	};

	/**
	 *	获取敏感词汇
	 *	event_value_web_get_face_script_req
	 */
	struct get_filter_words_req	: public Web_Req_Event
	{
	public:
		get_filter_words_req()
		{
			eventValue = event_value_web_get_filter_words_req;
		}
	};

	/**
	 *	获取敏感词汇响应
	 *	event_value_web_get_face_script_req
	 */
	struct get_filter_words_resp	:	
		public Web_Req_Event
	{
	public:
		get_filter_words_resp()
		{
			eventValue = event_value_web_get_filter_words_resp;
			wstrFilterWords = "";
		}

		string	wstrFilterWords;		//	Utf-8格式的字符串
	};

	/**
	*	判断给定的语句中是否包含有敏感词汇
	*	event_value_web_judge_filter_words_req
	*/
	struct judge_filter_words_req	:	
		public Web_Req_Event
	{
	public:
		judge_filter_words_req()
		{
			eventValue = event_value_web_judge_filter_words_req;
		}

		wstring	wstrFiltingWords;		//	需要过滤的词汇
		uint8	nType;					//	0,正常，1，敏感，2，禁用
	};

	/**
	 *  上传用户头像 
	 *  service_value_web_upload_head_image_req
	 */
	struct upload_head_image_event_req	:	
		public Web_Req_Event
	{
	public:
		upload_head_image_event_req()
		{
			eventValue = service_value_web_upload_head_image_req;
		}

		wstring wstrUrl;				//	上传的url
		wstring	wstrImagePath;			//	上传的头像路径
	};

	/**
	 *  上传用户头像的响应
	 *  event_value_web_upload_head_image_resp
	 *  param0 描述返回结果，WEB_RET_SUCCESS为成功，其余为失败
	 *  param2 为返回请求对应的SeqNo
	 */
	struct upload_head_image_event_resp	:	
		public Web_Resp_Event
	{
	public:
		upload_head_image_event_resp()
		{
			eventValue = event_value_web_upload_head_image_resp;
			desMId = MODULE_ID_HALL;
		}
		time_t uploadTime;   //上传时间

	};	

	/**
	 *  下载用户头像 
	 *  service_value_web_download_head_image_req
	 */
	struct download_head_image_event_req	:	
		public Web_Req_Event
	{
	public:
		download_head_image_event_req()
		{
			eventValue = service_value_web_download_head_image_req;
		}

		uint32  unUIN;                  //  下载头像的unUIN
		wstring wstrUrl;				//	下载的url
		wstring	wstrImagePath;			//	下载的头像保存路径
	};

	/**
	 *  下载用户头像的响应
	 *  event_value_web_download_head_image_resp
	 *  param0 描述返回结果，WEB_RET_SUCCESS为成功，其余为失败
	 *  param2 为返回请求对应的SeqNo
	 */
	struct download_head_image_event_resp	:	
		public Web_Resp_Event
	{
	public:
		download_head_image_event_resp()
		{
			eventValue = event_value_web_download_head_image_resp;
			desMId = MODULE_ID_HALL;
		}	

		uint32 unUIN;			   //下载的unUIN
		wstring wstrImagePath;     //下载的头像保存路径

	};	

	/**
	 * 通用的Web访问接口
	 * service_value_web_get_http_req
	 */
	struct get_http_url_event_req	:	
		public Web_Req_Event
	{
	public:
		get_http_url_event_req()
		{
			eventValue = service_value_web_get_http_req;
			nTimeout = 30;
		}

		wstring wstrUrl;					//	请求的URL
		UINT nTimeout;
	};

	/**
	 *  通用的Web访问接口
	 *	event_value_web_get_http_resp
	 *	param0	表示成功还是失败，WEB_RET_SUCCESS为成功，其余为失败
	 *  param2 为返回请求对应的SeqNo
	 */
	struct get_http_url_event_resp	:	
		public Web_Resp_Event
	{
	public:
		get_http_url_event_resp()
		{
			eventValue = event_value_web_get_http_resp;
		}

		string	strBuffer;					//	返回的报文
	};

	/**
	*	取消正在执行的任务
	*/
	struct cancel_http_task_req	:	
		public	Web_Resp_Event
	{
	public:
		cancel_http_task_req()
		{
			eventValue = event_value_web_cancel_http_task;
			nSeqNo = 0;
		}

		int32	nSeqNo;						//	请求序列号
	};

	/**
	* 下载文件
	*	service_value_web_download_file_req
	*  param0 描述返回结果，WEB_RET_SUCCESS为成功，其余为失败
	*/
	struct get_download_file_event_req	:	
		public	Web_Req_Event
	{
	public:
		get_download_file_event_req()
		{
			eventValue = service_value_web_download_file_req;
			nTimeout = 0;
			bResumable = FALSE;
		}

		wstring wstrUrl;				//	需要下载的文件的
		wstring	wstrFileName;			//	下载后文件的保存名称
		UINT nTimeout;//超时时间 秒
		BOOL bResumable;  //是否断点续传
	};

	/**
	*  文件下载后响应
	*  event_value_web_download_file_resp
	*  param2 为返回请求对应的SeqNo
	*/
	struct get_download_file_event_resp	:	
		public	Web_Resp_Event
	{
	public:
		get_download_file_event_resp()
		{
			eventValue = event_value_web_download_file_resp;
		}
	};

	/**
	* 查询文件的下载状态                                         
	*/
	struct query_download_file_process_req	:	
		public	Web_Req_Event
	{
	public:
		query_download_file_process_req()
		{
			eventValue = service_value_web_query_download_process_req;
		}

		uint32	nSeqNo;						//	当前下载任务的序列号

		uint32	uPercent;					//	下载进度， 50 表示 完成50%
		uint32	uTotalLength;				//	下载的总长度
		uint32	uFinishedLength;			//	已经下载完毕的总长度
		uint32	uSpeed;						//	速率，
		int32	uRemainedTime;				//	

	};

	/**
	*	获取验证码
	*	event_value_web_get_identify_code_req
	*/
	struct get_login_identify_code_service_req	:public	Web_Req_Event
	{
	public:
		get_login_identify_code_service_req()
		{
			eventValue = service_value_web_get_login_identify_code_req;
			bAllowOffline = TRUE;
			nWidth = 160;
			nHeight = 45;
			nCharNum = 4;
			strImageType = "jpeg";
		}

		uint32	nWidth;			//	验证码图片的宽度,单位为px
		uint32	nHeight;		//	验证码图片的高度，单位为px
		uint32	nCharNum;		//	需要的验证码的字符数
		string	strImageType;	//	返回的图片的类型
	};

	/**
	*	获取验证码响应
	*	event_value_web_get_identify_code_resp
	*	param0 为WEB_RET_SUCCESS成功，其余为失败
	*  param2 为返回请求对应的SeqNo
	*/
	struct get_login_identify_code_event_resp	:	
		public	Web_Resp_Event
	{
	public:
		get_login_identify_code_event_resp()
		{
			eventValue = event_value_web_get_login_identify_code_resp;
		}

		string	strKey;				
		string	strImageData;		//图片数据
	};
}
