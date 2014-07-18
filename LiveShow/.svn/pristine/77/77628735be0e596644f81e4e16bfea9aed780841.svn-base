// ShowMyRoomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "ShowMyRoomDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "ui\C179MsgBox.h"
#include "HallDefine.h"


// CShowMyRoomDlg 对话框

IMPLEMENT_DYNAMIC(CShowMyRoomDlg, CDialog)

CShowMyRoomDlg::CShowMyRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowMyRoomDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CShowMyRoomDlg::~CShowMyRoomDlg()
{
}

void CShowMyRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowMyRoomDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_MISSION_HYPERLINK_ENTERROOM,	&CShowMyRoomDlg::OnEnterRoom)	
END_MESSAGE_MAP()


// CShowMyRoomDlg 消息处理程序

int CShowMyRoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"ShowMyRoomDlg_AtRoom");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}


LRESULT CShowMyRoomDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{


	return TRUE;
}

void CShowMyRoomDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}

void CShowMyRoomDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnCancel();
}

BOOL CShowMyRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//创建显示对应所有房间浏览器
	m_MyRoom.CreateFromStatic(IDC_ATROOM_HTML_MYROOM,this);
	CComVariant var = (LONG)m_MyRoom.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlMyRoom",L"hwnd",var);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//内嵌页面中接口发送的进房请求
LRESULT CShowMyRoomDlg::OnEnterRoom(WPARAM wParam,LPARAM lParam)
{
	g_RoomModule->m_pModuleManager->PushEvent(MakeEvent<MODULE_ID_ROOM>()(Hall::EVENT_VALUE_HALL_ENTER_ROOM_REQUEST, MODULE_ID_HALL, wParam));  //请申进房
	return S_OK;
}

void CShowMyRoomDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		Init();
	}

}


LRESULT CShowMyRoomDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_NOTIFY_ROOM_SYSSET_CLICKOK:  //“确定”按扭的消息		
	case WM_NOTIFY_ROOM_SYSSET_CLICKAPPLI:  // "应用"按扭的消息
		{
			
		}
		break;
	case WM_MISSION_SHOW_MESSAGEBOX:  //拦截messagebox消息
		{
			CString sMessage = (LPCTSTR)wParam;
			CString strFind = _T("您确定要放弃对");
			int nFind = sMessage.Find(strFind);
			if( nFind!= -1)
			{
				CString sMes1,sMes2,roomID;
				sMes1 = sMessage.Left(sMessage.Find(_T("（")));
				sMes2 = sMessage.Mid(sMessage.Find(_T("（")));
				roomID = sMessage.Mid(sMessage.Find(_T("（"))+1,sMessage.Find(_T("）"))-sMessage.Find(_T("（"))-1);				
				if ( C179MsgBox::Show( this, NewMsgBox::MSGBOX_ICO_OK_CANCEL_TWO_STRING , NewMsgBox::ICONTYPE_WARNING , sMes1 , sMes2 ) == RET_OK)
				{
					//确定退出管理
					_variant_t vResult;
					m_MyRoom.CallJScript(_T("doAction"),roomID,&vResult);
				}			
				
			}
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CShowMyRoomDlg::Init()
{
	Hall::WebInterfaceUrl weburl;
	weburl.m_WebWithPara = true;
	weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_MENUSETMYROOM;
	g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
	m_MyRoom.SetUrl(weburl.m_WebInterfaceUrl,false,true);
	m_MyRoom.UpdateURL();
}

BOOL CShowMyRoomDlg::PreTranslateMessage(MSG* pMsg)
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

void CShowMyRoomDlg::SetApplyEffect(BOOL bEffect)
{
	CComVariant var = bEffect;
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgApplyEffect", L"visible", var);
}