#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "coredefine.h"

#define DEFER_PIC_CONFIG_PATH	L"resource\\defer\\deferconfig.xml"
#define DEFER_PIC_DIR			L"resource\\defer\\"
class DeferPicPathManager
{
private:
	DeferPicPathManager() ;
	~DeferPicPathManager() ;
public:
	static DeferPicPathManager* GetInstance() ;
	bool LoadDeferPicPathList() ;
	void Release() ;
	CString GetDefaultPicPathRandom() ;
private:
	static DeferPicPathManager* m_pDeferPicPathManager ;
	vector<CString> m_vDeferPicPathList ;
};

#define DeferPicPathMgr       DeferPicPathManager::GetInstance() 
