#pragma once
#include "51ktv.h"
#include <map>
#include "../../include/ui/TrayMgr.h"
#include "UpdateGuideDlg.h"
#include "CheckUpdateDlg.h"

using namespace std;

#define UM_TRAY_ICON_NOTIFY_MESSAGE   WM_USER + 300   //更新时托盘的消息
#define UM_DOWNLOAD_PROCESS_FULL      WM_USER + 301   //下载进度条满
#define UM_UPDATE_TEXT			  WM_USER + 302   //更新提示消息
#define UM_EXIT_AUTOUPDATE        WM_USER + 303   //退出自动更新
#define UM_RES_UPDATE_SUCCESS     WM_USER + 304   //配置文件自动更新成功

#define VERDOWN_LOCAL_DIR _T("VersionDown")   //新版本下载至的本地目录
#define VERSION_FILE_NEW  _T("NewVersion.ini")  //最新版本配置文件
#define VERSION_FILE_CUR  _T("CurVersion.ini")  //当前版本配置文件

#define INI_SIZE_16 16
#define INI_SIZE_280 280

//当前状态
#define STATE_AUTO 0   //准备中
#define STATE_NOUPDATE 1 //不升级
#define STATE_UPLOAD 2  //下载中
#define STATE_UPLOADEND 3 //下载结束
#define STATE_UPLOADERROR 4 //下载失败  
//当前状态

//更新提示
enum UpdateText
{
	UPDATE_TEXT_AUTOUPDATE = 0, //更新提示  _T("正在下载更新程序")
	UPDATE_TEXT_FAIL = 1, //更新提示 _T("更新失败")

};
enum VerUpdateType
{
	VER_NO = 0, //不需要升级
	VER_MAIN = 1, //主版本升级   安装
	VER_CONFIG = 2, //配置文件更新  覆盖
};

//版本结构
struct NewVersion
{
	CString m_sVerURl;//http服务器新版本下载url
	CString m_sVerLocal;  //下载至本地目录   
	CString m_sVerNum; //版本号
	CString m_sVerFile; //文件名
};

//版本信息
struct VersionInfo
{
	CString m_sCurMainVerNum  ;  //当前主版号
	CString m_sCurConfigVerNum ; //当前配置版本号

	NewVersion m_NewMain;    //最新主版本信息
	NewVersion m_NewConfig;  //最新配置版本信息
};

//下载任务
struct webDownTask
{
	webDownTask()
	{
		m_nSeqNo = 0;
		m_uPercent = 0;
		m_uTotalLength = 0;
		m_uFinishedLength = 0;
		m_downEnd = false;
	}

	uint32	m_nSeqNo;		    		//	下载任务的序列号

	CString m_strDownUrl;               //  http下载url
	CString m_strDownLocal;             //  下载至本地路径
	uint32	m_uPercent;					//	下载进度， 50 表示 完成50%
	uint32	m_uTotalLength;				//	下载的总长度
	uint32	m_uFinishedLength;			//	已经下载完毕的总长度
	bool    m_downEnd;                  //  是否下载完毕
};

//所有下载任务
struct TotalWebDownTask
{
	map<uint32,webDownTask> m_mapWebDown;  //所有下载任务
	uint32 m_uAllPercent;                  //总下载进度， 50 表示 完成50%
	uint32 m_uAllTotalLength;              //总下载长度
	uint32 m_uAllFinishedLength;           //总的已经下载完毕的长度

};

//内存映射的数据结构
typedef struct tagUpdateMap  //客户端自动升级时，客户端与自动升级之间内存映射时所需的数据结构
{	
	unsigned short  usUpdateType;  //更新类型  1-主版本更新   2-配置文件更新	
	WCHAR			DownPath[MAX_PATH];   //下载存放路径
	WCHAR			ClientPath[MAX_PATH];   //客户端运行程序路径
	WCHAR           c179AppDataPath[MAX_PATH];  //客户端数据资源路径
	HWND            CallUpdate_ClientHWnd;    //调用自动更新程序的客户端句柄
	HWND            DownLoad_ClientHWnd;      //调用自动更新程序的客户端下载对话框句柄
	HWND            Update_Hwnd;             //自动更新程序句柄
	NOTIFYICONDATA  trayNid;         //更新时的托盘图标数据
} UPDATEMAP, *PUPDATEMAP;
//
//==========================================================================================
// 类    名：CUpdateMap   内存映射类
// 功能描述：创建、打开或获取内存映射类的数据
// 创建日期：2012-1-5
// 修改日期：
// 用    法：使用内存映射前，先调用CreateFileMap()创建或打开内存映射，
//			 再调用UseUpdateMapDateBeginLock() 开始使用内存映射数据并加锁
//           使用完之后调用UseUpdateMapDateEndUnLock() 结束使用内存映射数据并解锁			 
//           最后调用CloseFileMap()关闭内存映射
//	         注：UseUpdateMapDateBeginLock()一定要与UseUpdateMapDateEndUnLock()成对使用
// 目    地：用于客户端进程与自动升级程序进程间的数据通信
//==========================================================================================

class CUpdateMap
{
public:
	CUpdateMap();
	virtual ~CUpdateMap();

	void CreateFileMap(CString MapName); //创建或打开内存映射	
	bool UseUpdateMapDateBeginLock(PUPDATEMAP &pDate);  //开始使用内存映射数据并加锁
	bool UseUpdateMapDateEndUnLock(PUPDATEMAP &pDate);  //结束使用内存映射数据并解锁
	void CloseFileMap();  //关闭内存映射

private:
	HANDLE m_hShareMem;
	void *m_pViewData;	//内存映射的数据
	HANDLE m_DateMutex;  //内存映射数据的互斥量   用以互斥多个进程同时访问内存映射

};

//==========================================================================================
// 类    名：CAutoUpdate   自动更新类
// 功能描述：自动从web下载所有登录成功之后的最新配置文件，
//			 下载完毕之后自动判断是否需要升级客户端(新版本安装)或更新文件(覆盖配置文件)
//  	     若需要升级或更新则下载升级包或更新包后自动调用升级程序进行安装或覆盖
// 创建日期：2012-1-10
// 修改日期：
// 用    法：
// 目    地：用户客户端自动更新时的更新包下载
//==========================================================================================
class CUpdateGuideDlg;
class CAutoUpdate 
{
public:
	CAutoUpdate();
	CAutoUpdate(CString MapName);
	virtual ~CAutoUpdate();
	
	void ShowCheckUpdateDlg(CWnd *pHall); //显示检测更新对话框，并开始检测最新版本
	void OnEventRespWebDown(Event const& e);  //web下载完毕时的返回函数,  e.param2表示 已下载完毕的序列号
	bool GetTatalDownProcess(uint32 &nTotalTime,uint32 &nFinishTime,uint32 &nProcess); //获取总的下载进度	
	static CString GetCurVersionInfo();  //从配置文件中获取当前显示版本号
	void LoadTrayUpdate();  //加载更新时的托盘图标， 因更新时的托盘图标是动态的，由多个ico组成，因此该函数应放在定时器中
	static CString GetAppTip(); //获取NewVersion.ini文件中AppTip的tip值
	void ExitAutoUpdate();

private:
	void InitAutoUpdate(HWND clickHWND);  //初始化，以及从web下载最新版本文件	
	void DownNewPackage();  //下载升级包或更新包
	void UpdateBegin();    //调用自动升级程序开始进行主版本的升级或配置文件的更新

	CString GetNewVersionInfo();  //从最新配置文件中获取最新显示版本号
	int32 SendWebDown(CString formUrl, CString ToLocal, int nTimeOut=0,BOOL bResumable = FALSE);  //发送web下载请求，从formUrl的url地址下载至本地ToLocal地址，返回该下载任务的序列号, nTimeOut:秒
	CString GetNameFormUrl(CString Url); //从url地址中获取文件名
	void ClearDir(CString deleteDir);  //删除指定目录及目录下的所有文件
	void OpenDirectory(CString PositionDir); //打开本地指定目录，若没有该目录则自动创建
	bool FindWebDownTask(uint32 nSeqNo,webDownTask &downTask);  //根据下载序列号字键字查询下载任务
	bool FindSeqNoInDownTask(uint32 nSeqNo); //查询nSeqNo序列号是否在下载任务中
	void SetWebDownEnd(uint32 nSeqNo);   //设置nSeqNo序列号任务下载完毕
	bool IsWebDownTotalEnd();  //所有下载任务是否都已下载完毕
	void GetVersionInfo(); //从NewVersion.ini最新版本配置文件和CurVersion.ini当前版本配置文件中获取版本信息
	VerUpdateType IsMainConfigUpdate();  //判断是主版本还是配置文件升级	

	void InitTrayUpdate();  //主版本更新时的初始化托盘图标
	void ReleaseTrayUpdate();  //主版本更新时的释放托盘图标	
	void ShowUpdateGuideDlg(); //显示更新向导
	void CloseCheckUpdateDlg(bool isShowHall); //检测更新完毕之后的关闭检测更新对话框

private:
	PUPDATEMAP m_MapDate179;  //客户端与自动升级之间的内存映射数据指针
	CString m_strMapName; //客户端与自动升级之间的内存映射名
	CUpdateMap m_Map179; //内存映射
	VersionInfo m_VerInfo;  //版本信息
	TotalWebDownTask  m_downTask; //所有下载任务
	uint32 m_NewVersionNSeqNO; //NewVersion.ini最新版本配置文件的下载序列号
	int m_iAutoState;//自动升级状态：0-准备中 1-不升级  2-下载中  3-下载结束  4-下载失败 
	CString m_strNewVerFile; //NewVersion.ini最新版本配置文件路径
	CString m_strCurVerFile; //CurVersion.ini当前版本配置文件路径	
	CString m_NewVersionDownDir;  //新版本下载至本地目录
	CTrayMgr m_trayMgrUpdate; //更新时的托盘
	vector<HICON> m_trayUpdatePath;  //更新时的动态托盘图标地址
	int m_trayUpdateCur;  //更新时的动态托盘图标当前下标
	VerUpdateType m_updatetype;
	CUpdateGuideDlg updateGudie;  //更新向导
	CCheckUpdateDlg m_checkUpdateDlg;  //检测更新对话框

public:
	HWND m_clickHWND;  //客户端窗口


};