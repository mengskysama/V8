#pragma once

#include "SysSettingIniKey.h"
#define SHOWPERSONAL_CONFIG_FILE_SYSSET	 L"config\\SysSet.ini"
#define CHANNELNUM_FILE L"config\\Channel.ini"

//SysSet.ini配置文件中 loginUpdate 

class CSysSettingIni
{
public:
	CSysSettingIni();
	virtual ~CSysSettingIni(){};

public:	

	static void GetloginUpdateURL(CString keyName,CString &updateUrl);  //获取loginUpdate下某keyName项的URL       
	static void SetloginUpdateURL(CString keyName,CString updateUrl);  //设置loginUpdate下某keyName项的URL 

	static void GetWebInterfaceUrl(CString keyName,CString &interfaceUrl);  //获取WebInterfaceUrl下某keyName项的URL
	static void GetSysSetString(CString AppName,CString keyName,CString &Returned); //获取某AppName键下某keyName项的值
private:
	static CString sysSetIniPath;


};

void GetRegisterChannelNum(CString &channelNum); //获取注册渠道