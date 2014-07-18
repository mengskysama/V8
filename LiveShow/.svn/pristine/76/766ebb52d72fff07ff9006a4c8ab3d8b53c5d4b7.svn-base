#include "stdafx.h"
#include "..\Room.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "utility\SystemHelper.h"
#include "utility\StringHelper.h"
#include "TaskDlg.h"
#include "HallDefine.h"
#include "ui\PngButton.h"
#include "..\RoomParentDlg.h"

#define USER_GUIDE_PATH L"resource\\html\\guide.html"

IMPLEMENT_DYNAMIC(CTaskDlg, CDialog)

CTaskDlg::CTaskDlg(CRoomParentDlg* pParent /*=NULL*/)
: CDialog(CTaskDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_pRoomParentDlg = pParent;
}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTaskDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_MISSION_COMPLETED,			&CTaskDlg::OnLoadHTMLComplete)	
END_MESSAGE_MAP()

// CTaskDlg 消息处理程序

int CTaskDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"TaskDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

LRESULT CTaskDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

void CTaskDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//	CDialog::OnCancel();
}

void CTaskDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//	CDialog::OnOK();
}

BOOL CTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_htmlTaskInfo.CreateFromStatic(IDC_ROOM_HTML_TASK,this);
	
	CComVariant var = (LONG)m_htmlTaskInfo.GetSafeHwnd();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"itemHtmlTask",L"hwnd",var);

	InitBrowser();

	return TRUE;
}

void CTaskDlg::VerifyEmail()
{
	if (GetCurInfo().enmAccountType != ACCOUNTTYPE_VISITOR)
	{
		Hall::WebInterfaceUrl weburl;
		weburl.m_WebWithPara = true;
		if (GetCurInfo().enmAccountType == core::ACCOUNTTYPE_QQ)
		{
			weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_QQ_BIND_MAIL;
		}else
		{
			weburl.m_WebInterfaceKeyName = SYSSET_INI_KEYNAME_SAFESETTING;
		}
		g_RoomModule->m_pModuleManager->CallService(Hall::SERVICE_VALUE_HALL_GET_WEBINTERFACEURL, (param)&weburl);
		common::utility::systemhelper::OpenUrl(weburl.m_WebInterfaceUrl) ;
	}else//游客提示
	{
		if (m_pRoomParentDlg != NULL && m_pRoomParentDlg->GetSafeHwnd() != NULL)
		{
			 m_pRoomParentDlg->TaskToRegister();
		}
	}
	
}

void CTaskDlg::GetReward(CString& name)
{
	wchar_t t = name.GetAt(CString(L"mong_").GetLength());
	int task = t - L'0';

	if(task >=1 && task <= core::TASK_SYSTEM_TASKNUM)
	{
		g_RoomModule->m_pModuleManager->PushEvent(
			MakeEvent<MODULE_ID_ROOM>()(core::EVENT_VALUE_CORE_GET_TASK_REWARD, 
			MODULE_ID_CORE, task, 0, 0, NULL));
	}
	else
	{
		assert(0);
	}	
}

void CTaskDlg::OnClick(LPARAM lParam)
{
	IHTMLEventObj *pEvtObj = (IHTMLEventObj *)lParam;

	if(pEvtObj == NULL)
	{
		assert(false);
		return ;
	}

	CComPtr<IHTMLElement> pElement;
	pEvtObj->get_srcElement(&pElement);

	if(pElement == NULL)
	{
		assert(false);
		return ;
	}	

	CComBSTR tag;
	pElement->get_tagName(&tag);
	if(tag == _T("A") || tag == _T("a"))
	{
		CString strName = m_htmlTaskInfo.GetValue( pElement , _T("name") ) ;
		if(strName == "verify")
		{
			VerifyEmail();//弹验证邮箱
		}
		else if(strName == "register")
		{
			//弹注册
			if (m_pRoomParentDlg != NULL && m_pRoomParentDlg->GetSafeHwnd() != NULL)
			{
				m_pRoomParentDlg->TaskToRegister();
			}
		}
		else if(strName == "close")
		{
			if (m_pRoomParentDlg != NULL && m_pRoomParentDlg != NULL)
			{
				m_pRoomParentDlg->TaskToClose();
			}
		}
		else if(strName.Find(L"mong_") == 0)
		{
			GetReward(strName);//领币
		}
	}
}

BOOL CTaskDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;

		default: 
			break;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

void CTaskDlg::InitBrowser()
{
	std::wstring strUrl = common::utility::systemhelper::Get179AppPath() + USER_GUIDE_PATH;
	m_htmlTaskInfo.SetUrl(strUrl,true);
	m_htmlTaskInfo.UpdateURL();
}

LRESULT CTaskDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MISSION_HYPERLINK_ONCLICK:
		{
			OnClick(lParam);
		}
		break;

	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CTaskDlg::UpdateTaskInfo()
{
	CurInfo myInfo = GetCurInfo();

	_variant_t pwdResult;
	if (GetCurInfo().enmAccountType != ACCOUNTTYPE_VISITOR)//注册用户
	{
		m_htmlTaskInfo.CallJScript(_T("guide_switch"), L"1", &pwdResult);
	}else//游客
	{
		m_htmlTaskInfo.CallJScript(_T("guide_switch"), L"2", &pwdResult);
	}

	for (int indexTask = 0; indexTask < TASK_SYSTEM_TASKNUM; ++indexTask)
	{
		CString strCmd = L"";
		strCmd.Format(L"%d:", indexTask + 1);
		if (myInfo.unarrTaskStat[indexTask] == core::TASKSTATTYPE_UNOPEN)//未开启
		{
			strCmd += L"no:no";
		}else if (myInfo.unarrTaskStat[indexTask] == core::TASKSTATTYPE_UNFINISH)//未完成
		{
			strCmd += L"no:no";
		}else if (myInfo.unarrTaskStat[indexTask] == core::TASKSTATTYPE_CANNOT_REWARD)//已完成，不可领取状态
		{
			strCmd += L"yes:no";
		}else if (myInfo.unarrTaskStat[indexTask] == core::TASKSTATTYPE_CAN_REWARD)//已完成，可领取状态
		{
			strCmd += L"yes:yes";
		}else if (myInfo.unarrTaskStat[indexTask] == core::TASKSTATTYPE_ISREWARD)//已完成，已领取状态
		{
			strCmd += L"yes:dis";
		}
		_variant_t pwdResult;
		m_htmlTaskInfo.CallJScript(_T("guide_task"), strCmd, &pwdResult);
	}
}

LRESULT CTaskDlg::OnLoadHTMLComplete(WPARAM wParam,LPARAM lParam)
{
	UpdateTaskInfo();
	return S_OK;
}