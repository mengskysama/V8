#include "stdafx.h"
#include "SysSettingIni.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"


#define SYSSET_INI_MAXSIZE 280

CString CSysSettingIni::sysSetIniPath =  CString(common::utility::systemhelper::Get179AppPath().c_str()) + SHOWPERSONAL_CONFIG_FILE_SYSSET;

CSysSettingIni::CSysSettingIni()
{
	
}

//获取loginUpdate下某keyName项的URL 
void CSysSettingIni::GetloginUpdateURL(CString keyName,CString &updateUrl)
{
	GetPrivateProfileString(SYSSET_INI_APPNAME_LOGINUPDATE,keyName,_T(""),updateUrl.GetBuffer(SYSSET_INI_MAXSIZE),SYSSET_INI_MAXSIZE,sysSetIniPath);
	updateUrl.ReleaseBuffer();
}

 //设置loginUpdate下某keyName项的URL 
void CSysSettingIni::SetloginUpdateURL(CString keyName,CString updateUrl)
{
	WritePrivateProfileString(SYSSET_INI_APPNAME_LOGINUPDATE,keyName,updateUrl,sysSetIniPath);
}

//获取WebInterfaceUrl下某keyName项的URL
void CSysSettingIni::GetWebInterfaceUrl(CString keyName,CString &interfaceUrl)
{
	GetPrivateProfileString(SYSSET_INI_APPNAME_WEBINTERFACEURL,keyName,_T(""),interfaceUrl.GetBuffer(SYSSET_INI_MAXSIZE),SYSSET_INI_MAXSIZE,sysSetIniPath);
	interfaceUrl.ReleaseBuffer();
}

//获取某AppName键下某keyName项的值
void CSysSettingIni::GetSysSetString(CString AppName,CString keyName,CString &Returned)
{
	GetPrivateProfileString(AppName,keyName,_T(""),Returned.GetBuffer(SYSSET_INI_MAXSIZE),SYSSET_INI_MAXSIZE,sysSetIniPath);
	Returned.ReleaseBuffer();
}

//获取注册渠道号
void GetRegisterChannelNum(CString &channelNum) 
{
	CString channelIniPath =  CString(common::utility::systemhelper::Get179AppPath().c_str()) + CHANNELNUM_FILE;

	GetPrivateProfileString(_T("Channel"),_T("ChannelNumber"),_T(""),channelNum.GetBuffer(SYSSET_INI_MAXSIZE),SYSSET_INI_MAXSIZE,channelIniPath);
	channelNum.ReleaseBuffer();

}