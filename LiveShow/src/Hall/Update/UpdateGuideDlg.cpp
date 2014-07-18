// UpdateGuideDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "UpdateGuideDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/ui/C179MsgBox.h"
#include "../HallWindow/HallDlg.h"
#include "../SysSettingIni.h"

#define  SKIN_ON_CLICK_CLOSE                1
#define  SKIN_ON_CLICK_MINIMIZE             2
#define  SKIN_NOTIFY_IF_DOWNLOADING         3
// CUpdateGuideDlg 对话框

IMPLEMENT_DYNAMIC(CUpdateGuideDlg, CDialog)

CUpdateGuideDlg::CUpdateGuideDlg(CAutoUpdate *autoUpdate,CWnd* pParent /*=NULL*/)
: CDialog(CUpdateGuideDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
	m_autoUpdate = autoUpdate;
}

CUpdateGuideDlg::~CUpdateGuideDlg()
{
}

void CUpdateGuideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateGuideDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_DOWNLOAD_PROCESS_FULL,&CUpdateGuideDlg::OnDownloadProgessFull)
	ON_MESSAGE(UM_UPDATE_TEXT,&CUpdateGuideDlg::OnUpdateText)
END_MESSAGE_MAP()


// CUpdateGuideDlg 消息处理程序

//设置更新版本
void CUpdateGuideDlg::SetUpdateVersion(CString sCurVer,CString sNewVer) 
{
	CString verStr;
	verStr.Format(_T("您当前的版本是%s，更新版本后将提升到%s"),sCurVer,sNewVer);
	CComVariant var = verStr;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTopPrompt",L"text",var);
}

LRESULT CUpdateGuideDlg::OnDownloadProgessFull(WPARAM wParam,LPARAM lParam)
{
	DrawUpdateProgess(wParam,lParam);
	OutputDebugString(_T("\n OnDownloadProgessFull"));
	KillTimer(TIMER_UPDATE_PROGESS_IDEVENT);
	return S_OK;
}

LRESULT CUpdateGuideDlg::OnUpdateText(WPARAM wParam,LPARAM lParam)
{
	
	CComVariant var;

	if(wParam == UPDATE_TEXT_AUTOUPDATE)
	{
		//显示"正在下载更新程序"
		var = L"正在下载更新程序";
	}
	else if(wParam == UPDATE_TEXT_FAIL)
	{
		//显示"更新失败"
		var = L"更新失败";
		KillTimer(TIMER_UPDATE_PROGESS_IDEVENT);

		CloseEnabled(true); //"关闭"按扭可用
		

	}
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textUpdateFail",L"text",var);
	return S_OK;
}

//下载进度条
void CUpdateGuideDlg::DownProgess(uint32 &nTotalTime,uint32 &nFinishTime,uint32 &nProcess)
{
	nTotalTime = 0;
	nFinishTime = 0;
	nProcess = 0;

	bool downProcess = m_autoUpdate->GetTatalDownProcess(nTotalTime,nFinishTime,nProcess);
	if(downProcess && nTotalTime > 0 && nFinishTime>= 0 && nFinishTime<=nTotalTime  )
	{
		CString text;
		text.Format(_T("\n-----nTotalTime:%d   nFinishTime:%d   "),nTotalTime,nFinishTime);
		OutputDebugString(text);

		if(nProcess >= 98)  //下载时最多显示至98%  自动更新程序解压之后发送消息再显示至100%
		{
			DrawUpdateProgess(100,98);
			OutputDebugString(_T("\n-----  showProgess: 98%"));
		}
		else
		{
			DrawUpdateProgess(nTotalTime,nFinishTime);
		}		
		
	}
	
}

BOOL CUpdateGuideDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	SetWindowText(L"白兔KTV更新向导");
	m_htmlUpdateGuide.CreateFromStatic(IDC_HTML_UPDATEGUIDE,this);
	CComVariant var = (LONG)m_htmlUpdateGuide.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlUpdateGuide",L"hwnd",var);
	CString strUrl;
	CSysSettingIni::GetWebInterfaceUrl(SYSSET_INI_KEYNAME_AUTOUPDATE,strUrl);
	m_htmlUpdateGuide.SetUrl(std::wstring(strUrl));
	m_htmlUpdateGuide.UpdateURL();
	DrawUpdateProgess(100,0);
	SetTimer(TIMER_UPDATE_PROGESS_IDEVENT,TIMER_UPDATE_PROGESS_INTERVALTIME,NULL);

	CloseEnabled(false); //"关闭"按扭不可用

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CUpdateGuideDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:

			return TRUE ;
		default: break;
		}	
	}

	return CDialog::PreTranslateMessage(pMsg);
}

int CUpdateGuideDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"UpdateGuideDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

//关闭按扭是否可用
void CUpdateGuideDlg::CloseEnabled(bool isEnabled)
{

	if(isEnabled)  
	{
		CMenu* pMenu = GetSystemMenu(FALSE);
		pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND| MF_ENABLED);  //系统关闭按扭可用

		CComVariant var =FALSE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnSysClose",L"disabled",var);  //界面“关闭”按扭可用
	}
	else
	{
		CMenu* pMenu = GetSystemMenu(FALSE);
		pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND| MF_GRAYED);   //系统关闭按扭不可用

		CComVariant var =TRUE;
		m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"btnSysClose",L"disabled",var);  //界面“关闭”按扭不可用
	}

	
}

//ALT+F4组合键关闭
void CUpdateGuideDlg::OnClose()
{
	CComVariant var;
	m_pSkinEngine->GetSubSkinProp(GetSafeHwnd(), L"btnSysClose", L"disabled", &var); //关闭”按扭是否可用
	if(!var.boolVal)
	{
		m_autoUpdate->ExitAutoUpdate();
		::SendMessage(m_autoUpdate->m_clickHWND,WM_NOTIFY_HALL_EXIT,0,0);//发送客户端退出消息
		
		CWnd::OnClose();
	}	
	
}

LRESULT CUpdateGuideDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE: 
		{
			OnClose();			
		}
		break;
	case SKIN_ON_CLICK_MINIMIZE:
		{
			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		break;
	default:break;
	}

	return TRUE;
}

void CUpdateGuideDlg::DrawUpdateProgess(uint32 nTotalTime,uint32 nFinishTime) //绘制进度条
{
	CRect rcTemp,rcLevelProgress;
	m_pSkinEngine->GetSubSkinPos(GetSafeHwnd(),L"progBack",&(rcTemp.left),&(rcTemp.top),&(rcTemp.right),&(rcTemp.bottom));	

	if(nTotalTime == 0)
		nTotalTime = 1;
	uint32 nWidth = rcTemp.right - rcTemp.left;
	uint32 nFullLength = (int)((( nFinishTime*100.0)/nTotalTime*nWidth)/100.0);
	uint32 nWhiteLength = nWidth - nFullLength;

	CComVariant varFullLen,varWhiteLen,varLevel;
	CString tempLength;
	tempLength.Format(_T("%d"),nFullLength);
	varFullLen = (CComVariant) tempLength;
	tempLength.Format(_T("%d"),nWhiteLength);
	varWhiteLen = (CComVariant) tempLength;

	uint32 ratio = nFinishTime*1.0/nTotalTime * 100;
	tempLength.Format(_T("%d%s"),ratio,L"%");
	varLevel = (CComVariant) tempLength;

	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"progFull",L"width",varFullLen);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"progWhite",L"width",varWhiteLen);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textProg",L"text",varLevel);

	CString text;
	text.Format(_T("\n----- tempLength:%s    "),tempLength);
	OutputDebugString(text);
}

void CUpdateGuideDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_UPDATE_PROGESS_IDEVENT)
	{
		if(m_autoUpdate)
		{
			uint32 nTotalTime,nFinishTime,nProgess;
			DownProgess(nTotalTime,nFinishTime,nProgess);
			if(nProgess >= 100)
			{
				KillTimer(TIMER_UPDATE_PROGESS_IDEVENT);
				OutputDebugString(_T("\n-----DownProgess  KillTimer"));
			}
		}
	}
	else if(nIDEvent == TIMER_UPDATE_TRAY)
	{
		m_autoUpdate->LoadTrayUpdate();  //加载更新时的动态托盘图标
	}

}
