#pragma once
#include "..\usermanager\useritem.h"

struct RichLevelInfo 
{
	uint8 unRichLevel;
	std::wstring strLevelName;
	std::wstring strImgPath;
};

typedef std::map<uint8, RichLevelInfo> MapRichInfoList;

class RichLevelManager
{
public:
	RichLevelManager(void);
	~RichLevelManager(void);

	static bool LoadRichInfo();
	static void Release();

	static std::wstring GetRichLevelName(IUserItem * pUserItem);
	static std::wstring GetRichLevelImgPath(IUserItem * pUserItem);
	static std::wstring GetNextRichLevelName(IUserItem * pUserItem);
	static std::wstring GetRichLevelNameByLevel(uint8 unRichLevel);
	static std::wstring GetRichLevelImgPathByLevel(uint8 unRichLevel);
	static void GetAllRichImg(std::map<uint8, std::wstring> & mapRichImgPath);

protected:
	static MapRichInfoList g_mapRichInfoList;
};
