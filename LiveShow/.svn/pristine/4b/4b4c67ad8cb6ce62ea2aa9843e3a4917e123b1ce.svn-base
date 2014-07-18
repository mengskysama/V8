#include "StdAfx.h"
#include "LoginVerifyHander.h"
#include "webdefine.h"
#include "../Hall.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"
#include "..\CommonLibrary\include\utility\FileHelper.h"
#define WM_AUTO_LOGIN (WM_USER+123)


CLoginVerifyHander::CLoginVerifyHander(HWND hwndLogin)
:m_loginHwnd(hwndLogin)
{
	
}

CLoginVerifyHander::~CLoginVerifyHander(void)
{
}

void CLoginVerifyHander::GetVerifyCode()
{	
	webinterface::get_login_identify_code_service_req getIdendtifyCodeReq;
	getIdendtifyCodeReq.srcMId	=	MODULE_ID_HALL;
	getIdendtifyCodeReq.desMId	=	MODULE_ID_WEBSERVICE;
	IModuleManager * pModuleManager = g_HallModule->m_pModuleManager;
	if(pModuleManager != NULL)
	{
		m_respId = pModuleManager->CallService(getIdendtifyCodeReq.eventValue,(param)&getIdendtifyCodeReq);
	}

	return;
}

//请求获取验证码图片返回响应函数
CString CLoginVerifyHander::OnGetVerifyCodeRep(Event const& evt)       
{
	CString verifyPath;
	if( evt.param0 == webinterface::WEB_RET_SUCCESS)
	{
		webinterface::get_login_identify_code_event_resp* pCodeEvent =(webinterface::get_login_identify_code_event_resp* ) (evt.m_pstExtraInfo);
		if (pCodeEvent == NULL)
		{
			return verifyPath;
		}	
		
		if (m_respId != pCodeEvent->param2)
		{
			//MessageBox(NULL,L"response id error!",L"",MB_OK);
			return verifyPath;
		}

		CString strPath;
		strPath.Format(_T("%s/VerifyCode"),common::utility::systemhelper::Get179AppPath().c_str());
		common::utility::filehelper::CreateAllDirectory(strPath.GetBuffer());		
		verifyPath.Format(L"%s/%d.jpg",strPath, pCodeEvent->param2);	

		common::utility::filehelper::WriteFile((BYTE*)(pCodeEvent->strImageData.c_str()),
			pCodeEvent->strImageData.length(),std::wstring(verifyPath.GetBuffer()));  //把strImageData存为图片文件
		
		m_strKey = pCodeEvent->strKey;
	}

	return verifyPath;
	
}

void CLoginVerifyHander::RefreshCode()
{	
	GetVerifyCode();
	return;
}
//////////////////////////////////////////////////////////////////////////

void CLoginVerifyHander::VerifyCode(CString strVerifyCode)
{		
	m_strVerifyCode = strVerifyCode;
	::SendMessage(m_loginHwnd,WM_AUTO_LOGIN,(WPARAM)this,NULL);
	
	return;
}


string CLoginVerifyHander::GetKey()
{
	return m_strKey;
}

CString CLoginVerifyHander::GetStrVerifyCode()
{
	return m_strVerifyCode;
}

