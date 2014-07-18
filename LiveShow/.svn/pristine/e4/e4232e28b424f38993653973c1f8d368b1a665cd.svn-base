#pragma once

#include "FilePathName.h"
#include "FileData.h"

#include <map>
#include <vector>

class CFileRepository
{
public:
	CFileRepository(void);
public:
	virtual ~CFileRepository(void);

public:
	static BOOL LoadFileData(LPCTSTR filePathName, CFileData& fileData);

	//从文件载入
	virtual BOOL LoadFile(LPCTSTR filePathName, BOOL& bExist);
	virtual BOOL LoadFile(LPCTSTR filePathName);

	//直接从数据载入
	virtual BOOL LoadFile(LPCTSTR filePathName, CFileData& fileData, BOOL& bExist);

	//卸载文件
	virtual BOOL UnloadFile(LPCTSTR filePathName);

	//卸载所有文件
	virtual BOOL UnloadAllFile();

	//获得文件
	virtual const char* GetFile(LPCTSTR filePathName, long& lLen);

	//获得文件
	virtual BOOL GetFile(LPCTSTR filePathName, CFileData& fileData);

	//载入目录下所有文件
	virtual BOOL LoadFromDir(LPCTSTR dirPath);

	//从DB文件中载入
	virtual BOOL LoadFromDBFile(LPCTSTR dbFileName);

	//保存到DB文件
	virtual BOOL SaveToDBFile(LPCTSTR dbFileName);

	//获得占用大小
	virtual BOOL GetRepositorySize(LONG& lNum, LONG& lSize) const;

	//获得文件列表
	virtual BOOL GetFileList(std::vector<CString>& vecFileList);

protected:
	std::map<CFilePathName, CFileData> m_mapFileData;	//文件对应表
};
