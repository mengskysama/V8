#pragma once
#include "51ktv.h"
#include <vector>
#include <map>

using namespace std;

//头像下载维护类
class CHeadDownload
{
public:
	CHeadDownload();
	virtual ~CHeadDownload();

public:
	void EventSendNewHeadDown(uint32 unUIN); //发送最新头像下载请求，
	void OnEventRespNewHeadDown(Event const& e); //最新头像下载请求返回响应函数，若最新头像下载成功则自动发送消息 MESSAGE_VALUE_HALL_HEAD_IMAGE_UPDATE
	CString GetLocalHeadImage(uint32 unUIN,bool bMan);  //获取本地头像图片路径，若未找到指定unUIN的对应头像图片则使用默认男女图片
	CString GetGuestHeadImage();  //获取本地默认游客图片

private:
	void MessageHeadDownResp(uint32 unUIN); //发送 MESSAGE_VALUE_HALL_NEW_HEAD_IMAGE 
	bool FindSeqNoInMapHeadImage(uint32 nSeqNo); //查询nSeqNo序列号是否在头像下载序列号表中
private:
	vector<uint32> m_VecHeadSeqNO;  //头像下载序列号表
	CString strHeadImageDir; //头像目录

};