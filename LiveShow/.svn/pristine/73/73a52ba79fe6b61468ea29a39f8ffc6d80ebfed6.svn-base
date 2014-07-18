#pragma once
#include "..\..\CommonLibrary\include\utility\AsynOpenURL.h"


//本机IP地址
class CStatReport
{
public:
	CStatReport();
	~CStatReport();

public:
	void IPStatReportRequest(HWND hRcvWnd,UINT umMessage);  //发送获取本机IP地址申请
	void IPStatReportResp(UINT unParam);  //获取本机IP地址申请的返回函数(请在hRcvWnd 对话框中的umMessage消息中调用本函数以获取本机IP地址)
	
	void OnGetNetState(uint32&iRelayIP,struct Service_Av_Net_Status_All& sansa);
	
	static CString GetIpCity();	

private:
	void OpenIPUrl(HWND hRcvWnd,UINT umMessage,UINT unParam,CString strUrl);
	void IPStatReportRespStep1();  //获取本机IP地址申请 第一步 的返回函数
	void IPStatReportRespStep2();  //获取本机IP地址申请 第二步 的返回函数
	CString AnalysisIPUrl();  //分析获取IP的url
	void ClearAsyOpenURL();
	void ClearBuffer();

private:
	common::utility::CAsynOpenURL *m_AsyOpenURL;
	BYTE *m_Buffer;
	HWND m_hRcvWnd;
	UINT m_umMessage;
	static CString m_ipCity;  //本地IP地址地区
};