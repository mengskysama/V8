#include "utility\ConfigManager.h"
#include "utility\IniFile.h"
#include <assert.h>
#include <Shlwapi.h>
#include "../CommonLibrary/include/utility/Md5.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/utility/StringHelper.h"
#include "../CommonLibrary/include/utility/Base64Coding.h"
#include "../CommonLibrary/include/utility/FileHelper.h"
#define  BOOL2String(param) ((param)?(_T("1")):(_T("0")))
#define CONFIG_FILE_NAME _T("config\\config.ini")

#define INI_SIZE_8 8
#define INI_SIZE_16 16
#define INI_SIZE_280 280

CConfigManager *CConfigManager::m_pInstance=NULL;

CConfigManager::CGarbageCollector CConfigManager::Garbo;

//////////////////////////////////////////////////////////////////////////
CConfigManager::CConfigManager(void)
{
	m_strConfigFileName=_T("");
}

CConfigManager::~CConfigManager(void)
{

}

CConfigManager * CConfigManager::Instance()
{
	if (m_pInstance==NULL)
	{
		m_pInstance=new CConfigManager();
		m_pInstance->Init();
	} 
	return m_pInstance;
}

void CConfigManager::Init()
{
	CString strPath = common::utility::systemhelper::Get179AppPath().c_str();	
	m_strConfigFileName.Format(_T("%s%s"),strPath,CONFIG_FILE_NAME);
	
	if(!common::utility::filehelper::IsFileExist(m_strConfigFileName))
	{
		common::utility::filehelper::CreateUnicodeFile(m_strConfigFileName);
	}	
	
}

//////////////////////////////////////////////////////////////////////////

void CConfigManager::GetUserInfo_Login(unsigned int  unUIN,UserInfoLogin &info_Login)
{
	CString userUinKey;
	userUinKey.Format(_T("%d"),unUIN);
	info_Login.unUIN = unUIN;

	WCHAR strTemp[INI_SIZE_280];
	GetPrivateProfileString(userUinKey,USERINFO_LOGIN_PASSWORD,_T(""),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_Login.strLoginPassword = common::CBase64Coding::Decode(strTemp).c_str();

	GetPrivateProfileString(userUinKey,USERINFO_LOGIN_AUTOLGOIN,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_Login.bAutoLogin = (bool)_ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_LOGIN_SAVEPASSWORD,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_Login.bSavePassword = (bool)_ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_LOGIN_OPENSOUND,_T("1"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_Login.bOpenSound = (bool)_ttoi(strTemp);	
	
}

void CConfigManager::GetUserInfo_Head(CString strAccount,UserInfoHead &info_Head)
{
	WCHAR strTemp[INI_SIZE_280];
	CString uinKey,nickNameKey,accountKey,bManKey,LoginAccount;	

	GetPrivateProfileString(USERLIST_LOGIN_179KEY,USERLIST_LOGIN_COUNT,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	int listCount = _ttoi(strTemp);

	for(int i=0; i<listCount;i++)
	{			
		uinKey.Format(_T("%s%d"),USERLIST_LOGIN_UIN,i);
		nickNameKey.Format(_T("%s%d"),USERLIST_LOGIN_NICKNAME,i);
		accountKey.Format(_T("%s%d"),USERLIST_LOGIN_ACCOUNT,i);
		bManKey.Format(_T("%s%d"),USERLIST_LOGIN_BMAN,i);

		GetPrivateProfileString(USERLIST_LOGIN_179KEY,accountKey,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
		LoginAccount.Format(_T("%s"),strTemp);
		if(LoginAccount == strAccount)
		{ 			
			GetPrivateProfileString(USERLIST_LOGIN_179KEY,uinKey,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
			info_Head.unUIN = _ttoi(strTemp);			
			info_Head.strLoginAccount = LoginAccount;
			GetPrivateProfileString(USERLIST_LOGIN_179KEY,nickNameKey,_T(""),info_Head.nickName.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strConfigFileName);
			info_Head.nickName.ReleaseBuffer();			
			GetPrivateProfileString(USERLIST_LOGIN_179KEY,bManKey,_T("1"),strTemp,INI_SIZE_280,m_strConfigFileName);			
			info_Head.bMan =(bool)_ttoi(strTemp);

			break;

		}	

	}
	
}

void CConfigManager::GetUserInfo_List(list<UserInfoHead> &info_List)
{	
	WCHAR strTemp[INI_SIZE_280];
	CString uinKey,nickNameKey,accountKey,bManKey;	

	GetPrivateProfileString(USERLIST_LOGIN_179KEY,USERLIST_LOGIN_COUNT,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	int listCount = _ttoi(strTemp);

	for(int i=0; i<listCount;i++)
	{			
		UserInfoHead newHead;
		uinKey.Format(_T("%s%d"),USERLIST_LOGIN_UIN,i);
		nickNameKey.Format(_T("%s%d"),USERLIST_LOGIN_NICKNAME,i);
		accountKey.Format(_T("%s%d"),USERLIST_LOGIN_ACCOUNT,i);
		bManKey.Format(_T("%s%d"),USERLIST_LOGIN_BMAN,i);		
					
		GetPrivateProfileString(USERLIST_LOGIN_179KEY,uinKey,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
		newHead.unUIN = _ttoi(strTemp);		
		GetPrivateProfileString(USERLIST_LOGIN_179KEY,accountKey,_T(""),strTemp,INI_SIZE_280,m_strConfigFileName);
		newHead.strLoginAccount.Format(_T("%s"),strTemp);
		GetPrivateProfileString(USERLIST_LOGIN_179KEY,nickNameKey,_T(""),strTemp,INI_SIZE_280,m_strConfigFileName);
		newHead.nickName.Format(_T("%s"),strTemp);		
		GetPrivateProfileString(USERLIST_LOGIN_179KEY,bManKey,_T("1"),strTemp,INI_SIZE_280,m_strConfigFileName);			
		newHead.bMan =(bool)_ttoi(strTemp);

		info_List.push_back(newHead);	

	}
	
}

void CConfigManager::GetUserInfo_PersonalSetting( unsigned int unUIN,UserInfo_PersonalSetting &info_PersonalSetting )
{	

	CString userUinKey;
	userUinKey.Format(_T("%d"),unUIN);
	WCHAR strTemp[INI_SIZE_280];

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ONMICPURPOSE,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.unOnMicPurpose = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_GENERALVOLUME,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);		
	info_PersonalSetting.unGeneralVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ACCOMPANYVOLUME,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.unAccompanyVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_MICVOLUME,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.unMicVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_STRENGTHENMIC,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.bStrengthenMic = (bool)_ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_RECORDMIXVOLUME,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.unRecordMixVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_RECORDMICVOLUME,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.unRecordMicVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_USEVIDEODEVICE,_T(""),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.strUseVideoDevice.Format(_T("%s"),strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_USEAUDIODEVICE,_T(""),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.strUseAudioDevice.Format(_T("%s"),strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_AUTOVIRTUALVIDEO,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.bAutoVirtualVideo = (bool)_ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ROOMSPEAKERVOLUME,_T("255"),strTemp,INI_SIZE_280,m_strConfigFileName);	
	info_PersonalSetting.unRoomSpeakerVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ROOMMICVOLUME,_T("255"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.unRoomMicVolume = _ttoi(strTemp);	

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_TEMPFONTSIZE,_T("14"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.TempMessageFont.font_size = _ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_TEMPFONTCOLOR,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.TempMessageFont.font_color = _ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_TEMPFONTSTYLE,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.TempMessageFont.font_style = _ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_TEMPFONTNAME,_T("宋体"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.TempMessageFont.font_type = strTemp ;
	//聊天字体信息
	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_CHATFONTSIZE,_T("14"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.ChatMessageFont.font_size = _ttoi(strTemp);
	
	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_CHATFONTCOLOR,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.ChatMessageFont.font_color = _ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_CHATFONTSTYLE,_T("0"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.ChatMessageFont.font_style = _ttoi(strTemp);

	GetPrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_CHATFONTNAME,_T("宋体"),strTemp,INI_SIZE_280,m_strConfigFileName);
	info_PersonalSetting.ChatMessageFont.font_type = strTemp ;
}



void CConfigManager::SetUserInfo_Login(const UserInfoLogin &info_Login)
{	

	//保存到文件	
	CString userUinKey;
	userUinKey.Format(_T("%d"),info_Login.unUIN);

	wstring strStoredPassword = common::CBase64Coding::Encode(wstring(info_Login.strLoginPassword));
	WritePrivateProfileString(userUinKey,USERINFO_LOGIN_PASSWORD,strStoredPassword.c_str(),m_strConfigFileName);	
	WritePrivateProfileString(userUinKey,USERINFO_LOGIN_AUTOLGOIN,BOOL2String(info_Login.bAutoLogin),m_strConfigFileName);
	WritePrivateProfileString(userUinKey,USERINFO_LOGIN_SAVEPASSWORD,BOOL2String(info_Login.bSavePassword),m_strConfigFileName);
	WritePrivateProfileString(userUinKey,USERINFO_LOGIN_OPENSOUND,BOOL2String(info_Login.bOpenSound),m_strConfigFileName);	

}

void CConfigManager::SetUserInfo_List(list<UserInfoHead> &info_List)
{	
	CString uinKey,nickNameKey,accountKey,bManKey,uinValue,listCount;
	WritePrivateProfileString(USERLIST_LOGIN_179KEY,NULL,NULL,m_strConfigFileName);	  //删除整个179登录列表

	//重新添加179登录列表
	listCount.Format(_T("%d"),info_List.size());
	WritePrivateProfileString(USERLIST_LOGIN_179KEY,USERLIST_LOGIN_COUNT,listCount,m_strConfigFileName);

	list<UserInfoHead>::iterator iter;
	int listNum = 0;
	for(iter = info_List.begin(); iter != info_List.end(); iter++)
	{
		UserInfoHead &userHead = *iter;		
		uinKey.Format(_T("%s%d"),USERLIST_LOGIN_UIN,listNum);
		nickNameKey.Format(_T("%s%d"),USERLIST_LOGIN_NICKNAME,listNum);
		accountKey.Format(_T("%s%d"),USERLIST_LOGIN_ACCOUNT,listNum);
		bManKey.Format(_T("%s%d"),USERLIST_LOGIN_BMAN,listNum);	
		
		uinValue.Format(_T("%d"),userHead.unUIN); 
		WritePrivateProfileString(USERLIST_LOGIN_179KEY,uinKey,uinValue,m_strConfigFileName);
		WritePrivateProfileString(USERLIST_LOGIN_179KEY,accountKey,userHead.strLoginAccount,m_strConfigFileName);
		WritePrivateProfileString(USERLIST_LOGIN_179KEY,nickNameKey,userHead.nickName,m_strConfigFileName);
		WritePrivateProfileString(USERLIST_LOGIN_179KEY,bManKey,BOOL2String(userHead.bMan),m_strConfigFileName);		

		listNum ++;
		
	}	
	
}

void CConfigManager::SetUserInfo_PersonalSetting( unsigned int unUIN,const UserInfo_PersonalSetting &info_PersonalSetting )
{
	CString userUinKey,strTemp;
	userUinKey.Format(_T("%d"),unUIN);

	strTemp.Format(_T("%d"),info_PersonalSetting.unOnMicPurpose);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ONMICPURPOSE,strTemp,m_strConfigFileName);

	strTemp.Format(_T("%d"),info_PersonalSetting.unGeneralVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_GENERALVOLUME,strTemp,m_strConfigFileName);	

	strTemp.Format(_T("%d"),info_PersonalSetting.unAccompanyVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ACCOMPANYVOLUME,strTemp,m_strConfigFileName);	

	strTemp.Format(_T("%d"),info_PersonalSetting.unMicVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_MICVOLUME,strTemp,m_strConfigFileName);	

	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_STRENGTHENMIC,BOOL2String(info_PersonalSetting.bStrengthenMic),m_strConfigFileName);	

	strTemp.Format(_T("%d"),info_PersonalSetting.unRecordMixVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_RECORDMIXVOLUME,strTemp,m_strConfigFileName);	

	strTemp.Format(_T("%d"),info_PersonalSetting.unRecordMicVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_RECORDMICVOLUME,strTemp,m_strConfigFileName);

	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_USEVIDEODEVICE,info_PersonalSetting.strUseVideoDevice,m_strConfigFileName);

	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_USEAUDIODEVICE,info_PersonalSetting.strUseAudioDevice,m_strConfigFileName);	

	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_AUTOVIRTUALVIDEO,BOOL2String(info_PersonalSetting.bAutoVirtualVideo),m_strConfigFileName);

	strTemp.Format(_T("%d"),info_PersonalSetting.unRoomSpeakerVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ROOMSPEAKERVOLUME,strTemp,m_strConfigFileName);	

	strTemp.Format(_T("%d"),info_PersonalSetting.unRoomMicVolume);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_ROOMMICVOLUME,strTemp,m_strConfigFileName);
	

	strTemp.Format(_T("%d"),info_PersonalSetting.TempMessageFont.font_size);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_TEMPFONTSIZE,strTemp,m_strConfigFileName);		

	strTemp.Format( L"%u" , info_PersonalSetting.TempMessageFont.font_style ) ;
	WritePrivateProfileString(userUinKey , USERINFO_PERSONALSETTING_TEMPFONTSTYLE , strTemp , m_strConfigFileName ) ;

	strTemp.Format( L"%u" , info_PersonalSetting.TempMessageFont.font_color ) ;
	WritePrivateProfileString(userUinKey , USERINFO_PERSONALSETTING_TEMPFONTCOLOR , strTemp , m_strConfigFileName ) ;

	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_TEMPFONTNAME,info_PersonalSetting.TempMessageFont.font_type.c_str(),m_strConfigFileName);
	//聊天字体信息
	strTemp.Format(_T("%d"),info_PersonalSetting.ChatMessageFont.font_size);
	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_CHATFONTSIZE,strTemp,m_strConfigFileName);		

	strTemp.Format( L"%u" , info_PersonalSetting.ChatMessageFont.font_style ) ;
	WritePrivateProfileString(userUinKey , USERINFO_PERSONALSETTING_CHATFONTSTYLE , strTemp , m_strConfigFileName ) ;

	strTemp.Format( L"%u" , info_PersonalSetting.ChatMessageFont.font_color ) ;
	WritePrivateProfileString(userUinKey , USERINFO_PERSONALSETTING_CHATFONTCOLOR , strTemp , m_strConfigFileName ) ;

	WritePrivateProfileString(userUinKey,USERINFO_PERSONALSETTING_CHATFONTNAME,info_PersonalSetting.ChatMessageFont.font_type.c_str(),m_strConfigFileName);
	
}


//从配置文件中删除该用户对应的数据
void CConfigManager::RemoveUserInfo(unsigned int unUIN)
{	

	list<UserInfoHead> userList;
	UserInfoHead deleteHead;

	deleteHead.unUIN = unUIN;	
	GetUserInfo_List(userList);	//获取用户列表
	userList.remove(deleteHead); //从用户列表中删除unUIN

	SetUserInfo_List(userList);  //将删除unUIN后的用户列表重新写入配置文件		
	CString userUinKey;
	userUinKey.Format(_T("%d"),deleteHead.unUIN);
	WritePrivateProfileString(userUinKey,NULL,NULL,m_strConfigFileName);	 //删除unUIN所对应的用户信息	

	
}


void CConfigManager::GetUserInfo_Setting( unsigned int unUIN, CString strKey, CString &strValue )
{

	CString userUinKey;
	userUinKey.Format(_T("%d"),unUIN);

	GetPrivateProfileString(userUinKey,strKey,_T(""),strValue.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strConfigFileName);
	strValue.ReleaseBuffer();
	
}

 //切换QQ提示标签是否可以显示
void CConfigManager::GetKeyValue(CString sectionKey, CString strKey, CString &strValue,CString strDefault)
{	
	GetPrivateProfileString(sectionKey,strKey,strDefault,strValue.GetBuffer(INI_SIZE_280),INI_SIZE_280,m_strConfigFileName);
	strValue.ReleaseBuffer();
	
}

void CConfigManager::SetUserInfo_Setting( unsigned int unUIN, CString strKey, CString strValue )
{
	CString userUinKey;
	userUinKey.Format(_T("%d"),unUIN);
	WritePrivateProfileString(userUinKey,strKey,strValue,m_strConfigFileName);
	
}

 //设置QQ提示标签是否可以显示
void CConfigManager::SetKeyValue(CString sectionKey, CString strKey, CString strValue)
{	
	WritePrivateProfileString(sectionKey,strKey,strValue,m_strConfigFileName);	

}
