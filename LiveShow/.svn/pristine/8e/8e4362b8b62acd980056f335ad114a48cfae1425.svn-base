#include "StdAfx.h"

#include "SkinPackEncoder.h"

#include "FileRepository.h"

#include <vector>
#include <map>

using namespace std;

CFileRepository::CFileRepository(void)
{
}

CFileRepository::~CFileRepository(void)
{
	UnloadAllFile();
}

BOOL CFileRepository::LoadFileData(LPCTSTR filePathName, CFileData& fileData)
{
	return fileData.LoadFromFile(filePathName);
}

//从文件载入
BOOL CFileRepository::LoadFile(LPCTSTR filePathName, BOOL& bExist)
{
	bExist = FALSE;
	//查找是否已经载入过
	CFilePathName pathName(filePathName);
	map<CFilePathName, CFileData>::iterator finded = m_mapFileData.find(pathName);
	if (finded != m_mapFileData.end()) {
		bExist = TRUE;
		return TRUE;
	}

	pair<CFilePathName, CFileData> toInsert;
	toInsert.first = filePathName;
	if (! LoadFileData(filePathName, toInsert.second))
		return FALSE;

	//加入表中
	m_mapFileData.insert(toInsert);

	return TRUE;
}

BOOL CFileRepository::LoadFile(LPCTSTR filePathName)
{
	BOOL bExist = FALSE;
	return LoadFile(filePathName, bExist);
}

//直接从数据载入
BOOL CFileRepository::LoadFile(LPCTSTR filePathName, CFileData& fileData, BOOL& bExist)
{
	bExist = FALSE;
	//查找是否已经载入过
	CFilePathName pathName(filePathName);
	map<CFilePathName, CFileData>::iterator finded = m_mapFileData.find(pathName);
	if (finded != m_mapFileData.end()) {
		bExist = TRUE;
		return TRUE;
	}

	pair<CFilePathName, CFileData> toInsert;
	toInsert.first = filePathName;
	toInsert.second = fileData;

	//加入表中
	m_mapFileData.insert(toInsert);

	return TRUE;
}

//卸载文件
BOOL CFileRepository::UnloadFile(LPCTSTR filePathName)
{
	//查找是否已经载入过
	CFilePathName pathName(filePathName);
	map<CFilePathName, CFileData>::iterator finded = m_mapFileData.find(pathName);
	if (finded == m_mapFileData.end())
		return FALSE;
	
	m_mapFileData.erase(finded);
	return TRUE;
}

//卸载所有文件
BOOL CFileRepository::UnloadAllFile()
{
	m_mapFileData.clear();
	return TRUE;
}

//获得文件
const char* CFileRepository::GetFile(LPCTSTR filePathName, long& lLen)
{
	CFilePathName pathName(filePathName);
	lLen = 0;
	map<CFilePathName, CFileData>::iterator finded = m_mapFileData.find(pathName);
	if (finded == m_mapFileData.end())
		return NULL;
	lLen = finded->second.lLen;
	return finded->second.pFileData;
}

//获得文件
BOOL CFileRepository::GetFile(LPCTSTR filePathName, CFileData& fileData)
{
	fileData.Clear();
	CFilePathName pathName(filePathName);
	map<CFilePathName, CFileData>::iterator finded = m_mapFileData.find(pathName);
	if (finded == m_mapFileData.end())
		return FALSE;
	fileData = finded->second;
	return TRUE;
}

//载入目录下所有文件
BOOL CFileRepository::LoadFromDir(LPCTSTR dirPath)
{
	return FALSE;
}

//从DB文件中载入
BOOL CFileRepository::LoadFromDBFile(LPCTSTR dbFileName)
{
	CSkinPackEncoder pack;
	if (! pack.LoadPack(dbFileName))
		return FALSE;

	CString filePath;
	pack.GetPackPath(filePath);

	BOOL bAllSuc = TRUE;

	vector<CString> vecFiles;
	pack.GetFileList(vecFiles);
	CFileData fileData;
	for (std::size_t i=0; i<vecFiles.size(); i++) {
		if (! pack.GetFileData(vecFiles[i], fileData)) {
			bAllSuc = FALSE;
			continue;
		}
		CString filePathName = filePath + _T("\\") + vecFiles[i];
		BOOL bExist = FALSE;
		if (! LoadFile(filePathName, fileData, bExist) 
			|| bExist) {
			bAllSuc = FALSE;
		}
	}
	return TRUE;
}

//保存到DB文件
BOOL CFileRepository::SaveToDBFile(LPCTSTR dbFileName)
{
	return FALSE;
}

//保存到DB文件
BOOL CFileRepository::GetRepositorySize(LONG& lNum, LONG& lSize) const
{
	lNum = (LONG)m_mapFileData.size();
	lSize = 0;
	CString pathName;
	map<CFilePathName, CFileData>::const_iterator iter = m_mapFileData.begin();
	while(iter!=m_mapFileData.end()) {
		const pair<CFilePathName, CFileData>& Pair = (*iter);
		lSize += sizeof(pair<CFilePathName, CFileData>);
		pathName = Pair.first;
		lSize += pathName.GetLength() * sizeof(TCHAR);
		lSize += Pair.second.lLen;
		iter ++;
	}
	return TRUE;
}

//获得文件列表
BOOL CFileRepository::GetFileList(std::vector<CString>& vecFileList)
{
	map<CFilePathName, CFileData>::const_iterator iter = m_mapFileData.begin();
	while(iter!=m_mapFileData.end()) {
		const pair<CFilePathName, CFileData>& Pair = (*iter);
		vecFileList.push_back((CString)Pair.first);
		iter ++;
	}
	return TRUE;
}
