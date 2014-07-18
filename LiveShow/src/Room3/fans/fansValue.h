#pragma once
#include "..\include\ui\mlhtmlview.h"
#include "..\usermanager\UserManager.h"

class  CFansValue
{
public:
	CFansValue();
	~CFansValue(){};

public:

	void Init(CMLHtmlView *htmlFans);
	void OnGetFans(uint32 unUin);  //获取粉丝数据
	void OnGetFansRsp(Event const&evt,CString failText);  //获取粉丝数据返回响应函数

private:
	uint32 m_unAnchorUin;
	CMLHtmlView* m_htmlFans;
};