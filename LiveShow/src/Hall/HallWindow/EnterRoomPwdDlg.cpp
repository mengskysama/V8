// EnterRoomPwdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "EnterRoomPwdDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "HallDefine.h"
#include "EnterRoomRequest.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE     1
#define  SKIN_ON_CLICK_OK        2
// CEnterRoomPwdDlg 对话框

IMPLEMENT_DYNAMIC(CEnterRoomPwdDlg, CDialog)

CEnterRoomPwdDlg::CEnterRoomPwdDlg(CEnterRoomRequest *enterRoom,CWnd* pParent /*=NULL*/)
	: CDialog(CEnterRoomPwdDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	m_roomId = 0;
	m_enterRoom = enterRoom;
	m_pParent = pParent;
	::GetSkinEngine(&m_pSkinEngine);
}

CEnterRoomPwdDlg::~CEnterRoomPwdDlg()
{
}

void CEnterRoomPwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROOM_PWD, m_password);
}


BEGIN_MESSAGE_MAP(CEnterRoomPwdDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CEnterRoomPwdDlg 消息处理程序

int CEnterRoomPwdDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if(NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"EnterRoomPwdDlg");
		ASSERT(hr==S_OK && __LINE__);
	}
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);
	return 0;
}

LRESULT CEnterRoomPwdDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_OK:
		{
          OnClickBtnOK();
		}
		break;
	case SKIN_ON_CLICK_CLOSE:
		{
          OnClickBtnClose();
		}
		break;
	default:break;
	}
	return TRUE;
}

void CEnterRoomPwdDlg::SetEnterRoomID(int roomID)
{
	m_roomId = roomID;
}
void CEnterRoomPwdDlg::OnClickBtnClose()
{	
    PostMessage(WM_CLOSE,NULL,NULL);
	m_enterRoom->ClearEnterRoomDataDlg(); //清除数据与销毁进房对放框
}

void CEnterRoomPwdDlg::OnClickBtnOK()
{

	CString password;
	m_password.GetWindowText(password);
	enterRoomInfo roomid;
	roomid.m_RoomID = m_roomId;
	roomid.m_password = password;
	m_enterRoom->EnterRoomEventRequest(roomid);	
    PostMessage(WM_CLOSE,NULL,NULL);
}

BOOL CEnterRoomPwdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			{
				OnClickBtnOK();
			}
			return TRUE ;
		default: break;
		}	
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CEnterRoomPwdDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if(m_pParent != NULL && bShow)
	{
		CenterWindow(m_pParent); //居中显示
	}
	
}
