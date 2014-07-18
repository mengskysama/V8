// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 WEBSERVICE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// WEBSERVICE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#include "51ktv.h"
#include "webdefine.h"
#include "RequestManager.h"
#include <map>
#include "HttpClient.h"
#include "ResponseParser.h"
#include "imodulemanager.h"
#include "coredefine.h"
#include "imodule.h"
#include "log.h"

using namespace std;
using namespace webinterface;

extern "C" 
{
	__declspec( dllexport ) IModule *  GetModule();
	__declspec( dllexport ) void  ReleaseModule();
}

inline uint32	CALLSERVICE(ModuleId moduleId, param wparam, param lparam);
void			PUSH_MESSAGE( EventValue event_value, param param0, param param1,param param2);
void			PUSH_EVENT( Event& event_);
core::CurInfo	GetCurInfo();


struct IModuleManager;
class  WebModule : public IModule
{
public:
	WebModule(void);
	~WebModule(void);

	virtual void	Load(IModuleManager *);
	virtual void	Unload();
	virtual void	ProcessEvent(Event const &);
	virtual void	ProcessMessage(Message const & );
	virtual void	PushEvent_( Event* event_);
	virtual void	PaybackExtraInfo(uint32  ,void *  );
	virtual int32	Invoke(param const ,param lparam);

	
	wstring	GetLogPath(uin nUin);

	/**
	 * outImagePath  是处理后图片的保存路径，如果路径不存在，则会自动创建	
	 */
	bool	ProcessImage( wstring inImagePath, wstring& outImagePath);

private:
	//=================================================================
	//            内部请求函数
	//=================================================================
	
	static uint32 ProcessReqGetUpdateConfigXml( WebModule* pModule, Event* pEvent);
	static uint32 ProcessGetFilterWords( WebModule* pModule, Event* pEvent);	
	static uint32 ProcessReqUploadHeadImage(WebModule* pModule,  Event*	pEvent);
	static uint32 ProcessReqDownloadHeadImage(WebModule* pModule,  Event*	pEvent);
	static uint32 ProcessReqGetFileReq( WebModule* pModule, Event*	pEvent);
	static uint32 ProcessReqQueryDownloadProcess( WebModule* pModule, Event* pEvent);	
	static uint32 ProcessReqGetHttp( WebModule* pModule, Event* pEvent);	
	static uint32 ProcessCancelHttpTask( WebModule* pModule, Event* pEvent);		
	static uint32 ProcessGetLoginIdentifyCode( WebModule* pModule, Event* pEvent);
	/** 
	 * 消息处理函数
	 */
	void	OnMessagePreLoginResp( Message const& _message);
	void	OnMessageOffline( );

	// 获取特定级别的URL需要延迟发送的时间
	void	ProcessGetResponse();
	void	ProcessSendUrl( );
	bool	ProcessHeadImage( wstring wstrImagePath, wstring& wstrOutputPath);


public:	
	IModuleManager*		m_pModuleManager;
	CRequestManager*	m_pRequestManager;				// 对HTTP请求的管理，负责向后台的Web服务器发送请求，

	map<int32, Event*>	m_mapRequestEvent;				// 记录用户请求的Event
	
	bool				m_bLogout;						// 是否已经进行了离线操作

	std::map<uint32, uint32>	m_mapSeqNo2ModuleId;	//	请求序列号与模块ID的对应关系

	// 处理句柄指针
	typedef uint32 (*CallHandler)( WebModule* pParser, Event* pEvent );

	typedef struct _HanderTable
	{
		EventValue		m_nEventValue;		//	对应的事件值
		CallHandler		m_hHandler;			//	处理句柄

	} HandlerTableEntry;
	static HandlerTableEntry m_tableDriven[];
};

namespace webinterface
{
	extern WebModule*	g_WebModule;
	EXTERN_LOG();
}