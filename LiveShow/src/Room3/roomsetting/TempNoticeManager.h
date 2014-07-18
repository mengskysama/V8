#pragma once
#include "coredefine.h"

struct NoticeInfo
{
	std::wstring strContent;
	core::MessageFont stFont;
};

typedef std::vector<NoticeInfo*> VecNoticeInfo;

class CTempNoticeManager
{
public:
	CTempNoticeManager(void);
	~CTempNoticeManager(void);

	static bool SaveXML(std::wstring strPath, std::wstring strFile, VecNoticeInfo & noticelist);
	static bool LoadXML(std::wstring strFullPath,VecNoticeInfo & noticelist);

//protected:
//	vecNoticeInfo m_vecNoticeInfo;
};
