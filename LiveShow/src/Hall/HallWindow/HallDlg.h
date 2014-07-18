#pragma once

#include "Skin_i.h"
#include "../../include/ui/mlhtmlview.h"
//#include "../../include/ui/splitter.h"
#include "VividTree.h"
#include "ExpatHallTree.h"
#include "EnterRoomRequest.h"
#include "ChangeExitDlg.h"
#include "..\LoginWindow\RegisterDlg.h"
#include "..\LoginWindow\GameDlg.h"
#include "..\HeadSetting\HeadSettingDlg.h"
#include "..\CommonLibrary\include\ui\TickerStatic.h"
#include "..\CommonLibrary\include\ui\GifButton.h"
#include "ProcessReport.h"

#define WM_NOTIFY_HALL_CHANGE_USER WM_USER+5678
#define WM_NOTIFY_HALL_EXIT WM_USER+5679
#define WM_NOTIFY_HALL_CLICK_SEARCH WM_USER+5681
#define WM_NOTIFY_HALL_TREE_CLICK WM_USER+5682
#define WM_NOTIFY_HALL_IS_GUEST_LOGIN  WM_USER+5683
#define WM_NOTIFY_HALL_NO_GUEST_LOGIN  WM_USER+5684
#define UM_QQFRISTLOGIN_INFO  WM_USER+5685   //QQ首次登录欢迎对话框
#define UM_USERINFO_UPDATE WM_USER + 5686    //用户资料变更
#define WM_MODULE_MSGBOX_SHOW WM_USER+5687  //
#define WM_TREECTRL_CLEAR WM_USER+5688  


// CHallDlg 对话框
class CSearchDlg;
class CSysSettingDlg;
class C179TipCtrl ;

class CHallDlg : public CDialog
{
	DECLARE_DYNAMIC(CHallDlg)

public:
	CHallDlg(IModuleManager* pModuleManager,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHallDlg();

// 对话框数据
	enum { IDD = IDD_LOBBY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnClose();	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);//头像框外发光
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPostPaint(WPARAM wParam, LPARAM lParam);//来自皮肤消息
	afx_msg LRESULT OnTipClickClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);

    //托盘菜单响应函数
    afx_msg void OnTraymenuHideWnd();//隐藏窗口
    afx_msg void OnTraymenuUserSet();//个人设置
    afx_msg void OnTraymenuVedioSet();//视频设置
	afx_msg void OnTraymenuHelp();//帮助
	afx_msg void OnTraymenuAbout();//关于软件
	afx_msg void OnTraymenuChangeUser();//更改用户
    afx_msg void OnTraymenuExit();//安全退出
	
	afx_msg LRESULT OnLoadHTMLComplete(WPARAM wParam,LPARAM lParam);  
	afx_msg LRESULT OnEnterRoom(WPARAM wParam,LPARAM lParam);  //大厅右侧内嵌页面中接口发送的进房请求
	afx_msg LRESULT OnOpenURL(WPARAM wParam,LPARAM lParam);    //大厅右侧内嵌页面中接口发送的浏览器打开新页面(带tocken参数)请求
	afx_msg LRESULT OnInLineURL(WPARAM wParam,LPARAM lParam);  //大厅右侧内嵌页面中接口发送的内嵌页面打开新页面(带tocken参数)请求

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	afx_msg void OnMove(int x, int y) ;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus) ;	
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);

	void ShowSysSettingDlg(SysSettingDlgType dlgType); //显示个人设置对话框
	
	void GetImgLevelPath(core::CurInfo &myCurInfo,CString &imglevel);  //根据等级在线时长获取头像部分等级图标的路径
	void GetVIPPath(uint8 vipLevel,CString &imgVip,CString &vipTip);  //根据冠获取头像部分VIP图标的路径以及提示语
    void DrawLevelProgess(int nTotalTime,int nFinishTime); //绘制等级进度条 		
	
	void NotifyCreateSkinItem();
	void DeleteVisitorsRoomList();  //游客退出客户端时删除当前用户最近访问列表
	void SafeSettingTimer() ;//(安全中心)密保

	void UpdateHallTree(); //大厅目录树文件更新	
	void HallTreeSkinSet(bool isSearch,bool searchFind = false); //大厅目录树皮肤设置   
	void SetRoomList();//设置大厅列表数据
	void SetResultList(); //设置房间搜索列表数据
	void DrawTreeView();  //绘制目录树
	void OnTreeClick(HTREEITEM pItem); //点击目录树项的响应函数
	void OnClickSearch();   //点击房间搜索后的结果响应函数
	void OnClickSearchBack(); //点击房间搜索中返回按扭时的响应函数

	void OnClickSysSetting();//点击了头像信息栏边的设置按扭响应函数
	void OnClickHead();   //点击头像时的响应函数，以上传用户图片对话框
	void OnClickBtnLogin(); //游客登录时头像部分可点击的"登录"按扭响应函数 
	void OnClickBtnRegister(); //游客登录时头像部分可点击的"注册"按扭响应函数	

	void OnClickBtnSafeSetting() ;	//安全中心	
	void ShowHeadUserInfo();  //显示头部用户信息
	int32 Module179MsgBoxShow(Hall::MsgBox179 *pMsgbox);
	void MoveTicker();  //大厅底部跑马灯位置调整	
	void ParserUpdateTicker(std::string tickerResp); //解释大厅底部跑马灯获取的最新数据

	void DrawGifButton();
	void MoveGifButton(BOOL bShow);
	void MoveSafeCenterTipCtrl();

	bool IsHallHome(); //当前大厅右侧内嵌IE为大厅首页

	DECLARE_MESSAGE_MAP()
	
private:
	IModuleManager* m_pModuleManager;
	CComPtr<ISkinEngine> m_pSkinEngine;
    CMLHtmlView m_htmlRight;  //大厅右侧的内嵌IE
	bool m_htmlIsLoadComplete; //大厅右侧的内嵌IE是否加载完成
	_variant_t phtmlRightResult;
	common::ui::CTickerStatic m_BottomTicker; //大厅底部跑马灯
	uint32 m_TickerSeqNo;//大厅底部跑马灯数据获取序列号
	CSearchDlg* m_pSearchDlg;  //大厅目录树上方的房间搜索
	CChangeExitDlg* m_pChangeExitDlg;//退出程序对话框
	CRegisterDlg* m_RegisterDlg;  //注册对话框(或QQ首次登录欢迎对话框)
	CGameDlg* m_GameDlg;
	C179TipCtrl* m_pTipCtrl ;

	VividTree         m_treeCtrl;
	UINT m_showType; //当前目录树类型  (大厅目录树，搜索结果)
	CExpatHallTree m_halltree; //房间列表数据
	uint32	m_nHallTeeSeqNo;  //大厅目录树文件下载序列号
	CString m_ResListiconPath; 
	CString m_ResHeadPath; 
	BOOL m_bGraySafeSetting ;
	BOOL m_bSearchResult;//搜索是否有结果,用于控制左侧列表位置
	CProcessReport m_processReport;

	common::ui::GifButton *m_pRichLevelBtn;

public://接口	
	CSearchDlg* GetSearchDlg();
	
	void OnHeadImageUpdate(uint32 unUIN); //有头像更新
	void HeadInit(); //头部信息初始化	
	void CloseAllDialog(); //关闭所有对话框	
	void CloseRegisterDlg(); //关闭注册对话框(或QQ首次登录欢迎对话框)
	void CloseHallGameDlg();
	void OnUpdateHallTreeResp(Event const& evt);//大厅目录树文件更新返回响应函数
	void OnUpdateTickerResp(Event const& evt);//大厅底部跑马灯获取最新数据返回响应函数
	void EventUpdateTicker(); //发送大厅底部跑马灯获取最新数据请求
	void ChangeUserInit(); //切换用户之后的大厅初始化	
	void CreateAndShowRegisterDlg(bool bRegister,CWnd *pParent=NULL); //创建注册对话框(或QQ用户首次登录时的完善资料对话框)
	void CreateAndShowGameDlg(CWnd *pParent=NULL);
	void OnPassWordSafe() ;	//设置了(安全中心)密保
	void OnLoginSuccess() ; //登录成功
	void OnLoginFailed() ; //登录失败
	void SetLoginToServer(bool isLogin); //是否已登录服务器
	bool QQDlgIsShow();  //QQ首次登录欢迎对话框是否正在显示
	void SetRoomRegister(); //设置为房间注册框
	bool RoomRegisterDlgIsShow(); //房间注册框是否正在显示
	void InitAllRoomBrowser();
	CString GetCurNickName(); //获取当前用户昵称
	void SetRickLevelTip();  //设置富豪等级TIP提示语
	void SetAllRoomBrowser(std::wstring url); //设置并打开大厅右侧内嵌指定页面

	CEnterRoomRequest m_EnterRoomRequest; //进房请求
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
