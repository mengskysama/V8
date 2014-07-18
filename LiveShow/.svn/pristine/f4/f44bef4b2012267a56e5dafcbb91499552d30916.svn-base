#pragma once
#include <string>
#include <vector>
#include <map>

#define SET_AUTOREPLY_MENU      3001            //设置自动回复
#define CANCEL_AUTOREPLY_MENU   3002            //取消自动回复

#define DEFAULT_AUTOREPLY_1         _T("您好，我现在有事不在，过会儿联系")
#define DEFAULT_AUTOREPLY_2         _T("我去吃饭了，一会儿回来")
#define DEFAULT_AUTOREPLY_3         _T("工作中，请勿打扰")

struct stAutoReplyMenuItem
{
	std::wstring strMenuItem;
	bool    bChecked;

	stAutoReplyMenuItem()
	{
		strMenuItem = L"";
		bChecked = false;
	}
};

class AutoReplyConfig
{
private:
	AutoReplyConfig();
	~AutoReplyConfig();
public:
	static AutoReplyConfig* GetInstance();
	bool LoadAutoReplyList();
	void SaveAutoReplyList();
	bool GetAutoReplyByIndex(int nIndex, stAutoReplyMenuItem& stMenuItem);
	std::wstring GetCheckedAutoReply();
	void SetCheckedByIndex(int nIndex);
	std::vector<stAutoReplyMenuItem>& GetAutoReplyList();
	void SetAutoReply(bool bAutoReply);
	bool IsAutoReply(){return m_bAutoReply;};
	void UpdateAutoReplyList(std::vector<stAutoReplyMenuItem>& vAutoReplyList);
	void Release();
	void SetRejectAutoMsg(uint32 unWho, bool bReject);
	bool GetRejectAutoMsg(uint32);
protected:
	bool LoadDefaultAutoReplyList();
private:
	static AutoReplyConfig* m_pAutoReplyManager;
	std::vector<stAutoReplyMenuItem> m_vAutoReplyList;
	bool m_bAutoReply;
	std::map<uint32, bool> m_mapReject;//我对谁设置了不再提醒
};

#define AutoReplyMgr         AutoReplyConfig::GetInstance()