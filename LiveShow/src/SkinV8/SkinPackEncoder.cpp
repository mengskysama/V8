#include "StdAfx.h"
#include "FileData.h"
#include "Shlwapi.h"
#include "TEAEncryptor.h"
#include "SkinPackEncoder.h"

#include <vector>
#include <map>

using namespace std;

CSkinPackEncoder::CSkinPackEncoder(void)
{
}

CSkinPackEncoder::~CSkinPackEncoder(void)
{
}

BOOL CSkinPackEncoder::PathStripToPath(CString& strPath)
{
	int nPos = max(strPath.ReverseFind(_T('\\')), strPath.ReverseFind(_T('/')));
	if (nPos >= 0)
		strPath = strPath.Left(nPos);
	return TRUE;
}

LONG CSkinPackEncoder::GetStringSerializedSize(const CString& str)
{
	LONG size = 0;
	size += sizeof(LONG);
	size += (str.GetLength() + 1) * sizeof(TCHAR);
	return size;
}

BOOL CSkinPackEncoder::WriteStringToFile(const CString& str, CFile& fout)
{
	LONG strLen = (str.GetLength() + 1) * sizeof(TCHAR);
	fout.Write(&strLen, sizeof(strLen));
	fout.Write(str, strLen);
	return TRUE;
}

BOOL CSkinPackEncoder::ReadStringFromFile(CString& str, CFile& fin)
{
	LONG strLen = -1;
	fin.Read(&strLen, sizeof(strLen));
	fin.Read(str.GetBuffer(strLen), strLen);
	str.ReleaseBuffer();
	return TRUE;
}

BOOL CSkinPackEncoder::WriteStringToFile(const CString& str, std::fstream& fout)
{
	return FALSE;
}

BOOL CSkinPackEncoder::ReadStringFromFile(CString& str, std::fstream& fin)
{
	return FALSE;
}

BOOL CSkinPackEncoder::WriteStringToBuffer(const CString& str, void* pBuffer, LONG nBufLen, LONG& nWriteLen)
{
	nWriteLen = 0;
	LONG size = GetStringSerializedSize(str);
	if (size > nBufLen)
		return FALSE;

	LONG strLen = (str.GetLength() + 1) * sizeof(TCHAR);
	BYTE* pBuf = (BYTE*)pBuffer;
	_macro_memcpy(pBuf, nBufLen, &strLen, sizeof(strLen));

	pBuf += sizeof(strLen);
	nBufLen -= sizeof(strLen);
	_macro_memcpy(pBuf, nBufLen, str, strLen);
	nWriteLen = sizeof(strLen) + strLen;
	return TRUE;
}

BOOL CSkinPackEncoder::ReadStringFromBuffer(CString& str, const void* pBuffer, LONG nBufLen, LONG& nReadLen)
{
	nReadLen = 0;
	if (nBufLen < sizeof(LONG))
		return FALSE;

	LONG strLen = -1;
	BYTE* pBuf = (BYTE*)pBuffer;
	_macro_memcpy(&strLen, sizeof(strLen), pBuf, sizeof(strLen));
	if (strLen < 0)
		return FALSE;

	pBuf += sizeof(strLen);
	nBufLen -= sizeof(strLen);
	if (strLen > nBufLen)
		return FALSE;

	_macro_memcpy(str.GetBuffer(strLen), strLen, pBuf, strLen);
	str.ReleaseBuffer();
	nReadLen = sizeof(strLen) + strLen;
	return TRUE;
}

//从目录创建皮肤包，绝对路径
BOOL CSkinPackEncoder::CreatePack(LPCTSTR strDir)
{
	if (! PathIsDirectory(strDir))
		return FALSE;

	m_strPackDir = _T("");
	m_mapFileData.clear();

	CFileFind finder;
	CString strWildcard = strDir;
	strWildcard += _T("\\*.*");

	BOOL beWorking = finder.FindFile(strWildcard);
	BOOL bAllSuc = TRUE;

	pair<CString, CFileData> toInsert;
	while (beWorking) {
		beWorking = finder.FindNextFile();
		
		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		CString filePathName = finder.GetFilePath();
		CString ext = PathFindExtension(filePathName);
		ext.MakeLower();
		if (   ext.IsEmpty()
			|| ext.Compare(_T(".skn")) == 0
			|| ext.Compare(_T(".skin")) == 0
			|| ext.Compare(_T(".db")) == 0
            || ext.Compare(_T(".py")) == 0
			)
			continue;

		toInsert.first = finder.GetFileName().MakeLower();
		if (! toInsert.second.LoadFromFile(filePathName)) {
			bAllSuc = FALSE;
			continue;
		}

		m_mapFileData.insert(toInsert);
	}

	return TRUE;
}

//加载皮肤包
BOOL CSkinPackEncoder::LoadPack(LPCTSTR strPackFileName)
{
	CFile file;
	if (! file.Open(strPackFileName, CFile::modeRead | CFile::shareDenyNone)) {
		ASSERT(0 && _T("CFileRepository::LoadFile 找不到文件或文件拒绝访问"));
		return FALSE;
	}

	ULONGLONG ullLen = file.GetLength();
	if (ullLen >= UINT_MAX) {
		ASSERT(0 && _T("文件过大"));
		return FALSE;
	}

	LONG fileLen = (LONG)ullLen;
	LONG fileNum = -1;
	if (fileLen < sizeof(fileNum))
		return FALSE;

	file.Read(&fileNum, sizeof(fileNum));
	if (fileNum < 0)
		return FALSE;

	if (fileNum == 0)
		return TRUE;

	LONG lLen = -1;
	if (fileLen < sizeof(fileNum) + sizeof(lLen))
		return FALSE;
	file.Read(&lLen, sizeof(lLen));
	if (lLen <= 0)
		return FALSE;

	//分配缓冲区
	vector<BYTE> vecBuffer;
	LONG leftFileLen = fileLen - sizeof(fileNum) - sizeof(lLen);
	vecBuffer.resize(leftFileLen, 0);
	if (leftFileLen != file.Read(&(vecBuffer[0]), leftFileLen)) {
		return FALSE;
	}

	//解密
	vector<BYTE> vecDecryptBuffer;
	int decryptBufferLen = (int)vecBuffer.size();
	vecDecryptBuffer.resize(decryptBufferLen, 0);
	if (! TEAEncryptor::TEADecrypt(&(vecBuffer[0]), (int)vecBuffer.size(), &(vecDecryptBuffer[0]), &decryptBufferLen))
		return FALSE;

	BYTE* pBuf = &(vecDecryptBuffer[0]);

	m_mapFileData.clear();
	pair<CString, CFileData> toInsert;
	LONG nReadLen = 0;
	for (int i=0; i<fileNum; i++) {
		//读取文件名
		if (! ReadStringFromBuffer(toInsert.first, pBuf, lLen, nReadLen))
			return FALSE;
		//移动游标
		pBuf += nReadLen;
		lLen -= nReadLen;
		toInsert.first.MakeLower();
		//读取文件数据
		if (! toInsert.second.ReadFromBuffer(pBuf, lLen, nReadLen))
			return FALSE;
		//移动游标
		pBuf += nReadLen;
		lLen -= nReadLen;
		//插入数据
		m_mapFileData.insert(toInsert);
	}

	ASSERT(lLen == 0);

	m_strPackDir = strPackFileName;
	PathStripToPath(m_strPackDir);

	return TRUE;
}

//保存皮肤包
BOOL CSkinPackEncoder::SavePack(LPCTSTR strPackFileName) const
{
	CFile file;
	if (! file.Open(strPackFileName, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone)) {
		ASSERT(0 && _T("CFileRepository::LoadFile 找不到文件或文件拒绝访问"));
		return FALSE;
	}

	LONG fileNum = (LONG)m_mapFileData.size();
	file.Write(&fileNum, sizeof(fileNum));

	if (fileNum == 0)
		return TRUE;

	//获取总包长
	LONG lLen = 0;
	map<CString, CFileData>::const_iterator iter = m_mapFileData.begin(),
		last = m_mapFileData.end();
	for (; iter!=last; iter++) {
		lLen += GetStringSerializedSize(iter->first);
		lLen += iter->second.GetSerializedSize();
	}

	if (lLen <= 0)
		return FALSE;

	file.Write(&lLen, sizeof(lLen));

	//分配缓冲区
	vector<BYTE> vecBuffer;
	vecBuffer.resize(lLen, 0);
	BYTE* pBuf = &(vecBuffer[0]);

	iter = m_mapFileData.begin();
	last = m_mapFileData.end();
	LONG nWriteLen = 0;
	for (; iter!=last; iter++) {
		//读取文件名
		if (! WriteStringToBuffer(iter->first, pBuf, lLen, nWriteLen))
			return FALSE;
		//移动游标
		pBuf += nWriteLen;
		lLen -= nWriteLen;
		//读取文件数据
		if (! iter->second.WriteToBuffer(pBuf, lLen, nWriteLen))
			return FALSE;
		//移动游标
		pBuf += nWriteLen;
		lLen -= nWriteLen;
	}

	ASSERT(lLen == 0);

	//加密
	vector<BYTE> vecEncryptBuffer;
	int encryptBufferLen = (int)vecBuffer.size();
	encryptBufferLen += 8;
	vecEncryptBuffer.resize(encryptBufferLen, 0);
	if (! TEAEncryptor::TEAEncrypt(&(vecBuffer[0]), (int)vecBuffer.size(), &(vecEncryptBuffer[0]), &encryptBufferLen))
		return FALSE;

	file.Write(&(vecEncryptBuffer[0]), encryptBufferLen);

	return TRUE;
}

//获得皮肤包的路径
BOOL CSkinPackEncoder::GetPackPath(CString& path) const
{
	path = m_strPackDir;
	return TRUE;
}

//设置皮肤包的路径
BOOL CSkinPackEncoder::SetPackPath(LPCTSTR path)
{
	if (! PathIsDirectory(path))
		return FALSE;

	m_strPackDir = path;
	return TRUE;
}

//获得皮肤包中文件列表
BOOL CSkinPackEncoder::GetFileList(std::vector<CString>& vecFileNames) const
{
	vecFileNames.clear();
	map<CString, CFileData>::const_iterator iter = m_mapFileData.begin(),
		last = m_mapFileData.end();
	for (; iter!=last; iter++) {
		vecFileNames.push_back(iter->first);
	}

	return TRUE;
}

//获得文件数据
BOOL CSkinPackEncoder::GetFileData(LPCTSTR fileName, CFileData& fileData) const
{
	map<CString, CFileData>::const_iterator finded = m_mapFileData.find(fileName);
	if (finded == m_mapFileData.end())
		return FALSE;

	fileData = finded->second;
	return TRUE;
}
