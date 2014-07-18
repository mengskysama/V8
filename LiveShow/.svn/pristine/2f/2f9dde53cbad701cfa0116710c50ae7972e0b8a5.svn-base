#include "stdafx.h"
#include "AutoUpdate.h"
#include "../Hall.h"
#include "webdefine.h"
#include "../SysSettingIni.h"
#include "../CommonLibrary/include/utility/StringHelper.h"
#include "../CommonLibrary/include/utility/FileHelper.h"
#include "XUnzip.h"
#include "../HallWindow/HallDlg.h"
#include "tlhelp32.h"


CUpdateMap::CUpdateMap()
{
	m_hShareMem = NULL;
	m_pViewData = NULL;

}
CUpdateMap::~CUpdateMap()
{

}

//==========================================================================================
// 类 方 法：CreateFileMap
// 功能描述：创建或打开内存映射
//
// 输    入：CString MapName  内存映射名
// 输    出：无
// 返 回 值：无
//==========================================================================================
void CUpdateMap::CreateFileMap(CString MapName)
{
	if(m_pViewData == NULL)
	{
		CString mutexName;
		mutexName = MapName + _T("Mutex");
		//创建或打开名为MapName的内存映射
		m_hShareMem = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, sizeof(UPDATEMAP), MapName);
		m_DateMutex = CreateMutex(NULL, FALSE,mutexName); //创建或打开名为UpdateMapMutex的互斥量

		if(m_hShareMem != NULL)
		{
			m_pViewData = MapViewOfFile(m_hShareMem, FILE_MAP_WRITE, 0, 0, 0);
			if(m_pViewData == NULL)
			{
				CloseHandle(m_hShareMem);
				m_hShareMem = NULL;
			}
		}

	}


}

//==========================================================================================
// 类 方 法：UseUpdateMapDateBeginLock
// 功能描述：开始使用内存映射数据并加锁
//
// 输    入：无
// 输    出：PUPDATEMAP pDate  内存映射数据指针
// 返 回 值：true 成功      false 失败
//==========================================================================================
bool CUpdateMap::UseUpdateMapDateBeginLock(PUPDATEMAP &pDate)
{
	bool getOK = false;
	WaitForSingleObject(m_DateMutex, INFINITE); //一直等待以获取互斥量	
	if(m_pViewData != NULL)
	{
		pDate = (PUPDATEMAP)m_pViewData;

		getOK = true;
	}
	return getOK;

}
//==========================================================================================
// 类 方 法：UseUpdateMapDateEndUnLock
// 功能描述：结束使用内存映射数据并解锁
//
// 输    入：PUPDATEMAP &pDate  内存映射数据
// 输    出：无
// 返 回 值：true 成功      false 失败
//==========================================================================================
bool CUpdateMap::UseUpdateMapDateEndUnLock(PUPDATEMAP &pDate)
{
	ReleaseMutex(m_DateMutex);  //释放互斥量
	return true;
}

//==========================================================================================
// 类 方 法：CloseFileMap
// 功能描述：关闭内存映射
//
// 输    入：无
// 输    出：无
// 返 回 值：无
//==========================================================================================
void CUpdateMap::CloseFileMap()
{
	if(m_pViewData != NULL)
	{
		UnmapViewOfFile(m_pViewData); //从进程的地址空间撤消文件数据的映像
		CloseHandle(m_hShareMem); //关闭内存映射
		CloseHandle(m_DateMutex); //关闭内存映射的互斥量
		m_pViewData = NULL;
		m_hShareMem = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAutoUpdate::CAutoUpdate()
:updateGudie(this)
{		
	m_strMapName = _T("179KTVAutoUpDate");
	m_NewVersionNSeqNO = 0;
	m_iAutoState = STATE_AUTO;
	m_updatetype = VER_NO;
	m_MapDate179 = NULL;

	m_Map179.CreateFileMap(m_strMapName);  //创建内存映射，客户端与自动升级之间使用内存映射进行进程通信
}

CAutoUpdate::CAutoUpdate(CString MapName)
:updateGudie(this)
{	
	m_strMapName = MapName;
	m_NewVersionNSeqNO = 0;
	m_iAutoState = STATE_AUTO;
	m_updatetype = VER_NO;
	m_MapDate179 = NULL;

	m_Map179.CreateFileMap(m_strMapName);  //创建内存映射，客户端与自动升级之间使用内存映射进行进程通信

}

CAutoUpdate::~CAutoUpdate()
{	
	if(m_iAutoState == STATE_NOUPDATE)  //不需要升级
	{
		ClearDir(m_NewVersionDownDir);  //不需要升级时，删除下载目录 
	}
	ReleaseTrayUpdate();
	m_Map179.CloseFileMap();
}


//初始化，以及从web下载最新版本文件	
void CAutoUpdate::InitAutoUpdate(HWND clickHWND)
{
	m_clickHWND = clickHWND;
	
	CString NewVersionUrl,s179AppDate;
	CSysSettingIni::GetloginUpdateURL(SYSSET_INT_KEYNAME_NEWVERSIONFILE,NewVersionUrl);  //获取登录之前要下载的NewVersion.ini配置文件的url

	s179AppDate = common::utility::systemhelper::Get179AppPath().c_str();
	m_NewVersionDownDir.Format(_T("%s%s"),s179AppDate,VERDOWN_LOCAL_DIR);	
	m_strNewVerFile.Format(_T("%s\\%s"),m_NewVersionDownDir,VERSION_FILE_NEW);//NewVersion.ini最新版本配置文件路径
	m_strCurVerFile.Format(_T("%sconfig\\%s"),s179AppDate,VERSION_FILE_CUR);//CurVersion.ini当前版本配置文件路径	

	m_iAutoState = STATE_AUTO;
	m_NewVersionNSeqNO = SendWebDown(NewVersionUrl,m_strNewVerFile,6,TRUE);	//从web下载最新版本配置文件，下载时间最多6秒


	CString strModuleDir = common::utility::systemhelper::GetModulePath().c_str();
	CString UpdateProcPath,UpdateProcNewPath;		
	UpdateProcPath.Format(_T("%s%s"),strModuleDir,_T("baituKTVUpdate.exe"));
	UpdateProcNewPath.Format(_T("%s%s"),strModuleDir,_T("baituKTVUpdate.exe.new"));
	if(common::utility::filehelper::IsFileExist(UpdateProcNewPath))  //上次有部分主版本更新时，有更新baituKTVUpdate.exe自动更新文件，
	{
		DeleteFile(UpdateProcPath);
		MoveFile(UpdateProcNewPath,UpdateProcPath);
	}
	


}

int32 CAutoUpdate::SendWebDown(CString formUrl, CString ToLocal, int nTimeOut,BOOL bResumable)
{
	int32 nSeqNo;  //	当前下载任务的序列号

	nSeqNo = -1;
	if(formUrl.IsEmpty() || ToLocal.IsEmpty())
	{
		return nSeqNo; 
	}

	webinterface::get_download_file_event_req getDownloadEventReq;
	getDownloadEventReq.srcMId	=	MODULE_ID_HALL;
	getDownloadEventReq.desMId	=	MODULE_ID_WEBSERVICE;
	getDownloadEventReq.wstrUrl = formUrl;
	getDownloadEventReq.wstrFileName = ToLocal;
	getDownloadEventReq.bAllowOffline = true;  //允许离线操作
	getDownloadEventReq.nTimeout = nTimeOut;
	getDownloadEventReq.bResumable = bResumable;

	IModuleManager* pModuleManager = g_HallModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		nSeqNo = pModuleManager->CallService(getDownloadEventReq.eventValue,(param)&getDownloadEventReq);

		webDownTask newDown;
		newDown.m_nSeqNo = nSeqNo;
		newDown.m_strDownUrl = formUrl;
		newDown.m_strDownLocal = ToLocal;
		m_downTask.m_mapWebDown.insert(pair<uint32, webDownTask>(newDown.m_nSeqNo, newDown));
	}	

	return nSeqNo;
}

//获取总的下载进度
bool CAutoUpdate::GetTatalDownProcess(uint32 &nTotalTime,uint32 &nFinishTime,uint32 &nProcess)
{
	uint32 uAllPercent = 0;                  //总下载进度， 50 表示 完成50%
	uint32 uAllTotalLength = 0;              //总下载长度
	uint32 uAllFinishedLength = 0;           //总的已经下载完毕的长度

	map<uint32,webDownTask> ::iterator iter;
	for(iter = m_downTask.m_mapWebDown.begin(); iter != m_downTask.m_mapWebDown.end(); iter++)
	{
		webDownTask &downTask = iter->second;
		if(downTask.m_nSeqNo == m_NewVersionNSeqNO)
		{
			continue;
		}
		webinterface::query_download_file_process_req getDownloadProcessEventReq;
		getDownloadProcessEventReq.srcMId	=	MODULE_ID_HALL;
		getDownloadProcessEventReq.desMId	=	MODULE_ID_WEBSERVICE;
		getDownloadProcessEventReq.nSeqNo   =   downTask.m_nSeqNo;
		getDownloadProcessEventReq.bAllowOffline = true;  //允许离线操作

		IModuleManager* pModuleManager = g_HallModule->m_pModuleManager;
		if (pModuleManager != NULL)
		{

			uint32 req = pModuleManager->CallService(getDownloadProcessEventReq.eventValue,(param)&getDownloadProcessEventReq);  //获取nSeqNo下载序列号的下载进度	
			if(req == INVALID_SEQNO || getDownloadProcessEventReq.uPercent > 100 )  //获取下载进度错误
			{
				return false;
			}			

			downTask.m_uPercent = getDownloadProcessEventReq.uPercent;    //下载进度
			downTask.m_uTotalLength = getDownloadProcessEventReq.uTotalLength;  //下载的总长度
			downTask.m_uFinishedLength = getDownloadProcessEventReq.uFinishedLength;  //已经下载完毕的总长度

			uAllTotalLength = uAllTotalLength  + downTask.m_uTotalLength;
			uAllFinishedLength = uAllFinishedLength + downTask.m_uFinishedLength;
			if(uAllTotalLength != 0)
			{
				uAllPercent = uAllFinishedLength * 1.0 / uAllTotalLength * 100;
			}			

		}	

	}

	
	m_downTask.m_uAllPercent = uAllPercent;
	m_downTask.m_uAllTotalLength = uAllTotalLength;
	m_downTask.m_uAllFinishedLength = uAllFinishedLength;

	nProcess = m_downTask.m_uAllPercent;
	nTotalTime = m_downTask.m_uAllTotalLength;
	nFinishTime = m_downTask.m_uAllFinishedLength;

	return true;	

}

//web下载完毕时的返回函数，e.param2表示 已下载完毕的序列号
void CAutoUpdate::OnEventRespWebDown(Event const& e)
{
	uint32 nSeqNo = e.param2;
	if(!FindSeqNoInDownTask(nSeqNo)) //查询nSeqNo序列号是否在下载任务中
	{
		return ;
	}

	if( e.param0 == webinterface::WEB_RET_SUCCESS || e.param0 == webinterface::WEB_REC_DOWNLOAD_EXISTS)
	{
		webinterface::get_download_file_event_resp* pDownEvent =(webinterface::get_download_file_event_resp* ) (e.m_pstExtraInfo);

		SetWebDownEnd(nSeqNo);  //设置nSeqNo序列号任务下载完毕
		if(m_iAutoState == STATE_AUTO)    //当前状态为准备中时，下载的为登录成功之后的最新配置文件
		{
			if(nSeqNo == m_NewVersionNSeqNO) //NewVersion.ini新版本配置文件下载完毕
			{
				GetVersionInfo(); //获取版本信息
				m_updatetype = IsMainConfigUpdate();   //判断是主版本还是配置文件升级
				if(m_updatetype == VER_NO)  //不需要升级
				{
					m_iAutoState = STATE_NOUPDATE;  //设置当前状态为不升级				
					CloseCheckUpdateDlg(true); //关闭检测更新对话框，显示大厅登录

				}
				else
				{
					DownNewPackage();   //下载升级包或更新包					
				}

			}

		}
		else if(m_iAutoState != STATE_NOUPDATE)//否则，下载的为升级包或更新包
		{			

			if(IsWebDownTotalEnd()) //所有升级包或更新包都已下载完毕
			{
				m_iAutoState = STATE_UPLOADEND;  //设置当前状态为下载结束
				::PostMessage(updateGudie.GetSafeHwnd(),UM_DOWNLOAD_PROCESS_FULL,100,98);//发送下载进度条满消息  下载时最多显示至98%  自动更新程序解压之后发送消息再显示至100%
				UpdateBegin();  //调用自动升级程序开始进行主版本的升级或配置文件的更新
			}

		}

	}	
	else
	{		
		if(m_iAutoState == STATE_AUTO)
		{
			CloseCheckUpdateDlg(true); //关闭检测更新对话框，显示大厅登录
		}
		m_iAutoState = STATE_UPLOADERROR;  //设置当前状态为下载失败
		if(updateGudie.GetSafeHwnd() != NULL)
		{
			updateGudie.SendMessage(UM_UPDATE_TEXT,UPDATE_TEXT_FAIL,0);//显示更新失败
		}		
		
	}

	return ;

}

//所有下载任务是否都已下载完毕
bool CAutoUpdate::IsWebDownTotalEnd()
{
	bool totalend = true;

	map<uint32,webDownTask>::iterator iter;
	for(iter = m_downTask.m_mapWebDown.begin(); iter != m_downTask.m_mapWebDown.end(); iter++)
	{
		webDownTask &downTask = iter->second;
		if(!downTask.m_downEnd)
		{
			totalend = false;
		}
	}

	return totalend;
}

//从NewVersion.ini最新版本配置文件和CurVersion.ini当前版本配置文件中获取版本信息
void CAutoUpdate::GetVersionInfo()
{
	GetPrivateProfileString(_T("Version"),_T("AppVersion"),_T(""),m_VerInfo.m_sCurMainVerNum.GetBuffer(INI_SIZE_16),INI_SIZE_16,m_strCurVerFile);
	m_VerInfo.m_sCurMainVerNum.ReleaseBuffer();  //获取当前主版号
	GetPrivateProfileString(_T("Version"),_T("ResVersion"),_T(""),m_VerInfo.m_sCurConfigVerNum.GetBuffer(INI_SIZE_16),INI_SIZE_16,m_strCurVerFile);
	m_VerInfo.m_sCurConfigVerNum.ReleaseBuffer();  //获取当前配置版本号


	GetPrivateProfileString(_T("App"),_T("AppVersion"),_T(""),m_VerInfo.m_NewMain.m_sVerNum.GetBuffer(INI_SIZE_16),INI_SIZE_16,m_strNewVerFile);
	m_VerInfo.m_NewMain.m_sVerNum.ReleaseBuffer();  //获取最新主版本中的版本号

	GetPrivateProfileString(_T("App"),m_VerInfo.m_sCurMainVerNum,_T(""),m_VerInfo.m_NewMain.m_sVerURl.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strNewVerFile);
	m_VerInfo.m_NewMain.m_sVerURl.ReleaseBuffer();  //获取最新主版本中的web下载url地址
	if(m_VerInfo.m_NewMain.m_sVerURl.IsEmpty())
	{
		GetPrivateProfileString(_T("App"),m_VerInfo.m_NewMain.m_sVerNum,_T(""),m_VerInfo.m_NewMain.m_sVerURl.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strNewVerFile);
		m_VerInfo.m_NewMain.m_sVerURl.ReleaseBuffer();  //获取最新主版本中的web下载url地址
	}

	
	GetPrivateProfileString(_T("Res"),_T("ResVersion"),_T(""),m_VerInfo.m_NewConfig.m_sVerNum.GetBuffer(INI_SIZE_16),INI_SIZE_16,m_strNewVerFile);
	m_VerInfo.m_NewConfig.m_sVerNum.ReleaseBuffer();  //获取最新配置版本中的版本号

	GetPrivateProfileString(_T("Res"),m_VerInfo.m_sCurConfigVerNum,_T(""),m_VerInfo.m_NewConfig.m_sVerURl.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strNewVerFile);
	m_VerInfo.m_NewConfig.m_sVerURl.ReleaseBuffer();  //获取最新配置版本中的web下载url地址	
	if(m_VerInfo.m_NewConfig.m_sVerURl.IsEmpty())
	{
		GetPrivateProfileString(_T("Res"),m_VerInfo.m_NewConfig.m_sVerNum,_T(""),m_VerInfo.m_NewConfig.m_sVerURl.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strNewVerFile);
		m_VerInfo.m_NewConfig.m_sVerURl.ReleaseBuffer();  //获取最新配置版本中的web下载url地址	
	}

}


//================================================================================================
// 类 方 法：IsMainConfigUpdate
// 功能描述：返回当前客户端是主版本号升级 还是 配置版本号升级
//
// 输    入：无
// 输    出：无
// 返 回 值：VerUpdateType
//================================================================================================
VerUpdateType CAutoUpdate::IsMainConfigUpdate()
{
	VerUpdateType verTemp = VER_NO;  //不需要升级

	int curMainNum,curConfigNum;  //当前主版本号、当前配置版本号
	int newMainNum,newConfigNum;  //最新主版本号、最新配置版本号

	curMainNum = _tstoi(m_VerInfo.m_sCurMainVerNum);	
	curConfigNum = _tstoi(m_VerInfo.m_sCurConfigVerNum);
	
	newMainNum = _tstoi(m_VerInfo.m_NewMain.m_sVerNum);	
	newConfigNum = _tstoi(m_VerInfo.m_NewConfig.m_sVerNum);

	if(curMainNum < newMainNum)
	{
		verTemp = VER_MAIN;  //主版本升级
	}
	else if(curConfigNum < newConfigNum)
	{
		verTemp = VER_CONFIG;  //配置文件更新
	}

	return verTemp;

}

//下载升级包或更新包
void CAutoUpdate::DownNewPackage()
{
	CString packUrl, downLocal;
	CString temp;

	if(m_updatetype == VER_MAIN )  //主版本升级， 下载升级包
	{		
		CloseCheckUpdateDlg(false); //关闭检测更新对话框，不显示大厅登录
		ShowUpdateGuideDlg();  //显示更新向导
		InitTrayUpdate();  //主版本更新时的初始化托盘图标

		temp = GetNameFormUrl(m_VerInfo.m_NewMain.m_sVerURl);
		m_VerInfo.m_NewMain.m_sVerLocal.Format(_T("%s\\%s"),m_NewVersionDownDir,temp);

		packUrl = m_VerInfo.m_NewMain.m_sVerURl;
		downLocal = m_VerInfo.m_NewMain.m_sVerLocal;

		SendWebDown(packUrl,downLocal,0,TRUE);	

		m_iAutoState = STATE_UPLOAD;  //设置当前状态为下载中
		return;
	}
	if(m_updatetype == VER_CONFIG )  //配置文件更新， 下载更新包
	{
		CloseCheckUpdateDlg(true); //关闭检测更新对话框，显示大厅登录

		temp = GetNameFormUrl(m_VerInfo.m_NewConfig.m_sVerURl);
		m_VerInfo.m_NewConfig.m_sVerLocal.Format(_T("%s\\%s"),m_NewVersionDownDir,temp);

		packUrl = m_VerInfo.m_NewConfig.m_sVerURl;
		downLocal = m_VerInfo.m_NewConfig.m_sVerLocal;

		SendWebDown(packUrl,downLocal,0,TRUE);	

		m_iAutoState = STATE_UPLOAD;  //设置当前状态为下载中
		return;
	}
}

//调用自动升级程序开始进行主版本的升级或配置文件的更新
void CAutoUpdate::UpdateBegin()
{

	CString strModulePath = common::utility::filehelper::GetCurPath().c_str();
	CString strModuleDir = common::utility::systemhelper::GetModulePath().c_str();
	CString str179AppDataPath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString UpdateProcPath;
	
	UpdateProcPath.Format(_T("%s%s"),strModuleDir,_T("baituKTVUpdate.exe"));		

	m_Map179.UseUpdateMapDateBeginLock(m_MapDate179);//开始使用内存映射数据并加锁

	m_MapDate179->CallUpdate_ClientHWnd = m_clickHWND;
	m_MapDate179->DownLoad_ClientHWnd = updateGudie.GetSafeHwnd();

	if(m_updatetype == VER_MAIN)
	{
		m_MapDate179->usUpdateType = VER_MAIN;  
		wcscpy(m_MapDate179->DownPath,m_VerInfo.m_NewMain.m_sVerLocal);  //主版本安装包下载存放路径
	} 
	else if(m_updatetype == VER_CONFIG)
	{
		m_MapDate179->usUpdateType = VER_CONFIG;  
		wcscpy(m_MapDate179->DownPath,m_VerInfo.m_NewConfig.m_sVerLocal);   //配置文件更新包下载存放路径
	}
	
	wcscpy(m_MapDate179->ClientPath,strModulePath); //客户端运行程序路径
	wcscpy(m_MapDate179->c179AppDataPath,str179AppDataPath); //客户端数据资源路径
	m_trayMgrUpdate.GetTrayNotifyIconDate(m_MapDate179->trayNid);

	m_Map179.UseUpdateMapDateEndUnLock(m_MapDate179);//结束使用内存映射数据并解锁	

	if(m_updatetype == VER_CONFIG || m_updatetype == VER_MAIN)
	{
		HINSTANCE Execut = ShellExecute(NULL, _T("Open"), UpdateProcPath, NULL, NULL, SW_SHOW); //启动自动升级程序进行主版本或配置文件升级  
		if((int)Execut <= 32)  //返回值小于等于32,表示调用程序失败
		{
			OutputDebugString(_T("\n  UpdateBegin: run baituKTVUpdate.exe  error"));
			updateGudie.SendMessage(UM_UPDATE_TEXT,UPDATE_TEXT_FAIL,0);//显示更新失败
		}
		else
		{
			OutputDebugString(_T("\n  UpdateBegin: run baituKTVUpdate.exe  ok"));
		}
		
	}

}



//从配置文件中获取当前显示版本号
CString CAutoUpdate::GetCurVersionInfo()
{
	CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString CurVerFile,CurVersionName;
	CurVerFile.Format(_T("%sconfig\\%s"),strModulePath,VERSION_FILE_CUR);//CurVersion.ini当前版本配置文件路径
	
	GetPrivateProfileString(_T("Version"),_T("VersionName"),_T(""),CurVersionName.GetBuffer(INI_SIZE_16),INI_SIZE_16,CurVerFile);
	CurVersionName.ReleaseBuffer();  //获取版本名

	return CurVersionName;

}

//获取NewVersion.ini文件中AppTip的tip值
CString CAutoUpdate::GetAppTip()
{
	CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString NewVerFile,AppTip;
	NewVerFile.Format(_T("%s%s\\%s"),strModulePath,VERDOWN_LOCAL_DIR,VERSION_FILE_NEW);//NewVersion.ini最新版本配置文件路径

	GetPrivateProfileString(_T("AppTip"),_T("tip"),_T(""),AppTip.GetBuffer(INI_SIZE_280),INI_SIZE_280,NewVerFile);
	AppTip.ReleaseBuffer();  //获取tip值

	return AppTip;
}

//从最新配置文件中获取最新显示版本号
CString CAutoUpdate::GetNewVersionInfo()
{
	CString newVersionName;
	GetPrivateProfileString(_T("App"),_T("VersionName"),_T(""),newVersionName.GetBuffer(INI_SIZE_16),INI_SIZE_16,m_strNewVerFile);
	newVersionName.ReleaseBuffer();  //获取最新主版本中的显示版本号

	return newVersionName;
}
//根据下载序列号字键字查询下载任务
bool CAutoUpdate::FindWebDownTask(uint32 nSeqNo,webDownTask &downTask)
{	
	bool findok = false;	
	map<uint32,webDownTask>::iterator finded = m_downTask.m_mapWebDown.find(nSeqNo);
	if(finded != m_downTask.m_mapWebDown.end())
	{
		downTask = finded->second;
		findok = true;
	}
	return findok;
}

//设置nSeqNo序列号任务下载完毕
void CAutoUpdate::SetWebDownEnd(uint32 nSeqNo)
{
	map<uint32,webDownTask>::iterator finded = m_downTask.m_mapWebDown.find(nSeqNo);
	if(finded != m_downTask.m_mapWebDown.end())
	{
		webDownTask &downTask = finded->second;
		downTask.m_downEnd = true;  //nSeqNo序列号任务下载完毕
	}
}

//查询nSeqNo序列号是否在下载任务中
bool CAutoUpdate::FindSeqNoInDownTask(uint32 nSeqNo)
{
	bool seqNoOK = false;
	map<uint32,webDownTask>::iterator finded = m_downTask.m_mapWebDown.find(nSeqNo);
	if(finded != m_downTask.m_mapWebDown.end())
	{
		webDownTask &downTask = finded->second;
		seqNoOK = true;
	}

	return seqNoOK;
}

//从url地址中获取文件名
CString CAutoUpdate::GetNameFormUrl(CString Url)
{
	CString decName;
	int find1 = Url.ReverseFind(_T('/'));
	if(find1 != -1)
	{
		decName = Url.Mid(find1+1);
	}

	return decName;

}

//递归删除指定目录及目录下的所有文件
void CAutoUpdate::ClearDir(CString deleteDir)
{
	CFileFind file;
	CString delFile = deleteDir;

	if(delFile.Right(1) != _T("\\"))
	{
		delFile += _T("\\*.*");
	}
	else
	{
		delFile += _T("*.*");
	}

	BOOL bf = file.FindFile(delFile);
	while(bf)
	{
		bf = file.FindNextFile();
		CString filePath = file.GetFilePath();
		if(!file.IsDots())  //IsDots()是否为 "."当前目录 或 ".."上一级目录
		{
			if(file.IsDirectory())  //是否为目录
			{
				ClearDir(filePath);   //递归删除指定目录及目录下的所有文件
				RemoveDirectory(filePath) ; //删除空目录
			}
			else
			{
				SetFileAttributes(filePath,FILE_ATTRIBUTE_NORMAL);
				::DeleteFile(filePath);  //删除文件

			}
		}

	}

}


//打开本地指定目录，若没有该目录则自动创建
void CAutoUpdate::OpenDirectory(CString PositionDir)
{
	WIN32_FIND_DATA fileInfo;
	CString ReFileDir;

	if(PositionDir != _T(""))
	{
		CString FindPositionDir = PositionDir;

		if(PositionDir.Right(1) != _T("\\") )
			PositionDir += "\\";

		FindPositionDir = FindPositionDir.Left(FindPositionDir.GetLength() - 1);

		//以下为查找PositionDir指定目录，若没有，则创建PositionDir目录
		HANDLE hFind = FindFirstFile(FindPositionDir, &fileInfo);

		if(hFind == INVALID_HANDLE_VALUE)  //未找到PositionDir目录，需创建目录
		{
			CString CreatePath;
			int beginFind = 0 ;

			int DirFind =  PositionDir.Find(_T("\\"), beginFind);

			while(DirFind != -1)
			{
				CreatePath = PositionDir.Left(DirFind + 1);

				if(CreatePath != _T(""))
				{
					CreateDirectory(CreatePath, NULL);
				}

				beginFind = DirFind + 1;
				DirFind = PositionDir.Find(_T("\\"), beginFind);

			}

			ReFileDir = CreatePath;
		}
		else  //找到PositionDir目录
		{
			ReFileDir = PositionDir;
		}

		//以上为查找PositionDir指定目录，若没有，则创建PositionDir目录
		FindClose(hFind);

	}
}

//主版本更新时的初始化托盘图标
void CAutoUpdate::InitTrayUpdate()
{
	if(m_updatetype != VER_MAIN )  //主版本升级
	{
		return;
	}

	HICON icon;
	CString strModulePath	= common::utility::systemhelper::Get179AppPath().c_str();	
	CString updateTray;
	//icon = AfxGetApp()->LoadIcon(IDI_TRAY_UPDATE1);
	updateTray.Format(_T("%sresource\\listicon\\updateTray1.ico"),strModulePath);
	icon = (HICON)LoadImage( AfxGetResourceHandle(), updateTray, 
		IMAGE_ICON,	16, 16, LR_LOADFROMFILE );
	m_trayUpdatePath.push_back(icon);
	//icon = AfxGetApp()->LoadIcon(IDI_TRAY_UPDATE2);
	updateTray.Format(_T("%sresource\\listicon\\updateTray2.ico"),strModulePath);
	icon = (HICON)LoadImage( AfxGetResourceHandle(), updateTray, 
		IMAGE_ICON,	16, 16, LR_LOADFROMFILE );
	m_trayUpdatePath.push_back(icon);

	m_trayUpdateCur = 0;

	m_trayMgrUpdate.Init(m_clickHWND, eTNMode_Null, eTFSMode_Click);

	SetTimer(updateGudie.m_hWnd,TIMER_UPDATE_TRAY,300,NULL);  //开启更新时的托盘图标动态显示

}
//加载更新时的托盘
void CAutoUpdate::LoadTrayUpdate()
{	
	// 托盘创建 
	if(m_updatetype != VER_MAIN )  //主版本升级
	{
		return;
	}

	m_trayMgrUpdate.Add(m_trayUpdatePath[m_trayUpdateCur], _T("客户端正在更新数据"),UM_TRAY_ICON_NOTIFY_MESSAGE);	

	m_trayUpdateCur ++ ;
	if(m_trayUpdateCur >= m_trayUpdatePath.size())
	{
		m_trayUpdateCur = 0;
	}

}

void CAutoUpdate::ExitAutoUpdate()
{
	if(m_MapDate179 != NULL && m_MapDate179->Update_Hwnd != NULL)
	{
		::SendMessage(m_MapDate179->Update_Hwnd,WM_CLOSE,0,0);//发送自动更新退出消息
	}

}

//主版本更新时的释放托盘图标	
void CAutoUpdate::ReleaseTrayUpdate()
{
	if(m_updatetype != VER_MAIN )  //主版本升级
	{
		return;
	}
	KillTimer(updateGudie.m_hWnd,TIMER_UPDATE_TRAY);
	if (NULL != m_trayMgrUpdate.GetWindowHandle())
	{		
		m_trayMgrUpdate.Delete();	
	}	

}

//显示更新向导
void CAutoUpdate::ShowUpdateGuideDlg()
{
	g_HallModule->HideLoginHallDialog();
	SwitchResourceToModule(_T("Hall"));		
	updateGudie.Create(CUpdateGuideDlg::IDD);
	updateGudie.CenterWindow(CWnd::GetDesktopWindow());  //显示更新向导对话框居中显示
	updateGudie.ShowWindow(SW_SHOW);
	CString curVersion = GetCurVersionInfo();
	CString newVersion = GetNewVersionInfo();
	updateGudie.SetUpdateVersion(curVersion,newVersion);
}

//显示检测更新对话框
void CAutoUpdate::ShowCheckUpdateDlg(CWnd *pHall)
{	
	SwitchResourceToModule(_T("Hall"));		
	m_checkUpdateDlg.Create(CCheckUpdateDlg::IDD);
	m_checkUpdateDlg.CenterWindow(pHall);  //检测更新对话框居中显示
	m_checkUpdateDlg.ShowWindow(SW_SHOW);
	InitAutoUpdate(pHall->m_hWnd);  //初始化，自动更新检测开始，从web下载最新配置文件	

}

//检测更新完毕之后的关闭检测更新对话框
void CAutoUpdate::CloseCheckUpdateDlg(bool isShowHall)
{
	::SendMessage(m_checkUpdateDlg.m_hWnd,WM_CLOSE,0,0);

	if(isShowHall)  //显示大厅登录
	{
		g_HallModule->ShowLoginHallDialog();
		g_HallModule->FirstAutoLogin();
	}
}