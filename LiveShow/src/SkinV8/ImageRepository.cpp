#include "StdAfx.h"

#include <map>
#include <set>
#include <fstream>

using namespace std;

#include ".\imagerepository.h"

#include "ImageType.h"

CImageRepository::CImageRepository(void)
{
}

CImageRepository::~CImageRepository(void)
{
	map<CFilePathName, CImageBase*>::iterator iter = m_mapImageRepository.begin(), last = m_mapImageRepository.end();
	for (; iter!=last; iter++) {
		CImageBase* pImageBase = iter->second;
		delete pImageBase;
	}
	m_mapImageRepository.clear();
}

CImageBase* CImageRepository::LoadImage(LPCTSTR filePathName)
{
	CFilePathName pathName(filePathName);

    map<CFilePathName, CImageBase*>::iterator finded = m_mapImageRepository.find(pathName);
	//图像库中已经存在
	if (finded != m_mapImageRepository.end()) {
		return finded->second;
	}
	//图像库中不存在，则新建
	CImageBase* pImageImpl = new CImageType;
	if (pImageImpl == NULL || ! pImageImpl->LoadFromFile(filePathName)){
		delete pImageImpl;
		pImageImpl = NULL;
		return NULL;
	}
	//加入图像库
	m_mapImageRepository.insert(pair<CFilePathName, CImageBase*>(pathName, pImageImpl));

	return pImageImpl;
}

CImageBase* CImageRepository::LoadImage(LPCTSTR filePathName, const char* pzData, long size)
{
	CFilePathName pathName(filePathName);

    map<CFilePathName, CImageBase*>::iterator finded = m_mapImageRepository.find(pathName);
	//图像库中已经存在
	if (finded != m_mapImageRepository.end()) {
		return finded->second;
	}
	//图像库中不存在，则新建
	CImageBase* pImageImpl = new CImageType;
	if (pImageImpl == NULL || ! pImageImpl->LoadFromData(pzData, size)){
		delete pImageImpl;
		pImageImpl = NULL;
		return NULL;
	}
	//加入图像库
	m_mapImageRepository.insert(pair<CFilePathName, CImageBase*>(pathName, pImageImpl));

	return pImageImpl;
}

BOOL CImageRepository::TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor)
{
	return CImageType::TransformColor(pParam, color, transformColor);
}

void CImageRepository::TextOut(LPCTSTR textName)
{
	fstream fout(textName, ios_base::out);
	map<CFilePathName, CImageBase*>::iterator iter = m_mapImageRepository.begin(), last = m_mapImageRepository.end();
	for (; iter!=last; iter++) {
		fout << iter->first << endl;
	}
}

BOOL CImageRepository::GetRepositorySize(LONG& lNum, LONG& lSize) const
{
	lNum = (LONG)m_mapImageRepository.size();
	lSize = 0;
	BOOL bAllSuc = TRUE;
	map<CFilePathName, CImageBase*>::const_iterator iter = m_mapImageRepository.begin(), last = m_mapImageRepository.end();
	for (; iter!=last; iter++) {
		LONG temp = 0;
		if (! iter->second->GetSize(temp))
			bAllSuc = FALSE;
		lSize += temp;
	}
	return bAllSuc;
}
