// MySocket.cpp: Implementierung der Klasse CMySocket.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "stdafx.h"
#include "MySocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMySocket::CMySocket()
{
	m_pProxyLayer = new CAsyncProxySocketLayer;
	m_hParentHwnd = NULL;
}

CMySocket::~CMySocket()
{
	Close();
	delete m_pProxyLayer;
}

void CMySocket::SetNotifyWnd(HWND hWnd)
{
	m_hParentHwnd = hWnd;
	m_pProxyLayer->SetNotifyWnd( hWnd);
}

void CMySocket::OnReceive(int nErrorCode)
{
	if (nErrorCode)
	{
		Close();
		return;
	}

	char buffer[4097];
	int res = Receive(&buffer, 4096);
	if (res == SOCKET_ERROR || !res)
	{
		if (GetLastError() != WSAEWOULDBLOCK || !res)
		{
			return;
		}
		return;
	}
	buffer[res] = 0;

	Close();
}

void CMySocket::OnConnect(int nErrorCode)
{
	if (nErrorCode)
	{
		Close();
		PostMessage(m_hParentHwnd, WM_PROXY_TEST, 0, 0);
	}
	else
	{
	}
}

void CMySocket::OnTestResponse(bool bRet)
{
	if (!bRet)
	{
		//C51SkinMessageBox::Show(g_OptionModule->m_pSettingDlg, _T("无法连接到代理服务器！"), _T("提示"), MB_OK);
		//AfxMessageBox(_T("无法连接到代理服务器！"));
	}
	else
	{
		//C51SkinMessageBox::Show(g_OptionModule->m_pSettingDlg, _T("已连上代理服务器！"), _T("提示"), MB_OK);
		//AfxMessageBox(_T("已连上代理服务器！"));
	}
	Close();
	
	PostMessage(m_hParentHwnd, WM_PROXY_TEST, 0, 0);
}

int CMySocket::OnLayerCallback(const CAsyncSocketExLayer *pLayer, int nType, int nParam1, int nParam2)
{
	ASSERT(pLayer);
	if (nType == LAYERCALLBACK_STATECHANGE)
	{
		CString str;
		if (pLayer == m_pProxyLayer)
			str.Format(_T("Layer Status: m_pProxyLayer changed state from %d to %d"), nParam2, nParam1);
		else
			str.Format(_T("Layer Status: Layer @ %d changed state from %d to %d"), pLayer, nParam2, nParam1);
		return 1;
	}
	else if (nType == LAYERCALLBACK_LAYERSPECIFIC)
	{
		if (pLayer == m_pProxyLayer)
		{
			switch (nParam1)
			{
			case PROXYERROR_NOCONN:
				break;
			case PROXYERROR_REQUESTFAILED:
				if (nParam2)
				break;
			case PROXYERROR_AUTHTYPEUNKNOWN:
				break;
			case PROXYERROR_AUTHFAILED:
				break;
			case PROXYERROR_AUTHNOLOGON:
				break;
			case PROXYERROR_CANTRESOLVEHOST:
				break;
			default:
				break;
			}
		}
	}

	return 1;
}