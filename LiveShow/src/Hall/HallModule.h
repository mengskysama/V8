#pragma once

#include "51ktv.h"
#include "imodule.h"
#include "../../include/ui/TrayMgr.h"
#include "coredefine.h"
#include "Update/AutoUpdate.h"
#include "HallDefine.h"
#include "SysNotify/SysMessagePopup.h"
#include "HeadSetting\HeadDownload.h"
#include "HeadSetting\HeadSettingDlg.h"
#include "..\CommonLibrary\include\ui\ImageImpl.h"

core::CurInfo GetCurInfo();
CString AppendUrlParamet(CString url); //在url之后加参数uin,tokentype,token
void CloseNonModalDlg(CDialog*& dlg); //关闭非模态对放框

class CHallDlg;
class CLoginDlg;

class HallModule : public IModule, public ITrayEvent
{
public:
	HallModule(void);
	~HallModule(void);

	virtual void Load(IModuleManager *);
	virtual void Unload();
	virtual void ProcessEvent(Event const&);
	virtual void ProcessMessage(Message const&);
	virtual int32 Invoke(ServiceValue const serviceValue, param);

	virtual void OnTrayEvent(WPARAM w, LPARAM l);
	virtual void OnFlashStart(DWORD dwParam);
	virtual void OnFlashEnd(DWORD dwParam);	

	void ShowLoginDialog(bool bShow);
	void HideLoginHallDialog();
	void ShowLoginHallDialog();
	CWnd* GetLoginDlgWnd();
	void CloseHallRegisterDlg();
	void CloseHallGameDlg();
	CString GetLoaclHeadImage(uint32 unUIN,bool bMan);  //获取本地头像图片路径，若未找到指定unUIN的对应头像图片则使用默认男女图片， 当unUIN为0时，使用默认游客图片
	void LoginingTrayMgr();   //登录过程中大厅托盘动态切换显示， 因登录时的托盘图标是动态的，由多个ico组成，因此该函数应放在定时器中
	void FirstAutoLogin(); //启动程序后的自动登录

	//加载图片,返回skin使用的CComVariant格式
	CComVariant LoadImageWrapper(CString imagePath,bool isUpload = false);
	void OnWebPluginCommand(CString strCommand);
	void ExcuteCommandCaihong();

private:

	bool InitShareMemInfo(HWND hwndHall, HWND hwndLogin);//设置HWND进共享内存
	void ProcessHallCreateAndShow(Event const &evt);	
	void ProcessKicked(Event const &evt);
	void ProcessWebGetLoginIdentifyResp(Event const &evt);
	void ProcessLoginResp(Event const &evt);
	void ProcessWebDownFileResp(Event const &evt);
	void ProcessWebHeadImageResp(Event const &evt);
	void ProcessWebGetHttpResp(Event const &evt);	
	void ProcessPassWordSafe(Event const &evt ) ;  //设置了(安全中心)密保
	void PrcoessHallHeadUpload(Event const &evt);  //头像上传
	void ProcessWebHeadUpLoadResp(Event const &evt); //头像上传完毕响应函数
	void PrcoessMyInfoUpdate(Event const &evt);
	void ProcessDisconnected(Event const &evt);
	void ProcessRegisterResp(Event const &evt);
	void ProcessRichUpdateMoneyResp(Event const &evt);
	void PrcoessHallActivateShow(Event const &evt);
	void PrcoessInLineURLShow(Event const &evt);


	void ExcuteWebPluginCommand(CString strCommand);	
	void ProcessCommandLine();
	void ProcessLoginSuccess();

	void MessageMyVipLevelUpdate(Message const& msg);  //vip等级变更
	void MessageMyRichLevelUpdate(Message const& msg);  //富豪等级变更

	int32 InvokeGetServiceWebInterfaceUrl(param st);
	int32 InvokeGetTokenURL(param st);
	int32 InvokeOpenTokenURL(param st);
	int32 InvokeMsgBoxShow(param st);
	int32 InvokeGetHeadImagePath(param st);  //SERVICE 获取指定用户unUIN的头像路径
	int32 InvokeUpDateHeadImagePath(param st);  //SERVICE 获取指定用户unUIN的头像路径并下载最新头像

	void InitTrayMgr();
	void CreateHallTrayMenu();	
	void LoadTrayMgr(BOOL isLogined = FALSE); //加载大厅图标

private:
    CHallDlg * m_pHallDlg;
	CLoginDlg * m_pLoginDlg;
	CAutoUpdate m_AutoUpdate;  //自动更新类		
	CTrayMgr m_trayMgr;
	vector<HICON> m_trayMgrPath;  //大厅托盘图标地址
	int m_trayMgrCur;  //登录时的大厅动态托盘图标当前下标
	CSysMessagePopup m_SysMessagePopup; //系统弹窗
	CHeadDownload m_headDownload; //头像下载维护
	CHeadSettingDlg *m_pHeadSettingDlg;
	
	bool m_bCommandLineProcessed;//exe的启动参数是否已经执行过
	CString m_cacheWebPluginCommand;//缓存从web过来的启动命令

	std::map<CString, common::ui::CImageImpl*> m_mapPath2Image;

public:
	IModuleManager* m_pModuleManager;	
};

