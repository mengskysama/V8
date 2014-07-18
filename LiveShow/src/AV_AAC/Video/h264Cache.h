#pragma once
#include <list>

/////////////////////////////////////////////////////////////////////////
//帧数据大小最大可达到25K+ 大部分几百字节 动态申请节省内存 
class CVideoDataCache
{
public:
	CVideoDataCache();
	~CVideoDataCache();
	
	void SetData(char* pData, int Len);
	void GetData(char* pData, int& Len);
	int  GetLength();
	char* GetDataPtr();
	void SetType(int t);
	int  GetType();
protected:
	char* m_pdata;
	int   m_ilen;
	int   m_itype;
};


/////////////////////////////////////////////////////////////////////////
class CVideoDataCacheList
{
typedef std::list<CVideoDataCache*> Cache_List;
public:
	CVideoDataCacheList();
	~CVideoDataCacheList();
	
	void SetListData(CVideoDataCache*);
	int  GetListSize();
	CVideoDataCache* GetListData();
	void ClearList();
protected:
	Cache_List m_list;
};
