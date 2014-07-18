#pragma once

#include <vector>
#include <map>
#include <fstream>

class CFileData;
class CSkinPackEncoder
{
public:
	CSkinPackEncoder(void);
	~CSkinPackEncoder(void);
public:
	static BOOL PathStripToPath(CString& strPath);

	static LONG GetStringSerializedSize(const CString& str);

	static BOOL WriteStringToFile(const CString& str, CFile& fout);
	static BOOL ReadStringFromFile(CString& str, CFile& fin);

	static BOOL WriteStringToFile(const CString& str, std::fstream& fout);
	static BOOL ReadStringFromFile(CString& str, std::fstream& fin);

	static BOOL WriteStringToBuffer(const CString& str, void* pBuffer, LONG nBufLen, LONG& nWriteLen);
	static BOOL ReadStringFromBuffer(CString& str, const void* pBuffer, LONG nBufLen, LONG& nReadLen);

public:
	//从目录创建皮肤包，绝对路径
	BOOL CreatePack(LPCTSTR strDir);
	//加载皮肤包
	BOOL LoadPack(LPCTSTR strPackFileName);
	//保存皮肤包
	BOOL SavePack(LPCTSTR strPackFileName) const;
	//获得皮肤包的路径
	BOOL GetPackPath(CString& path) const;
	//设置皮肤包的路径
	BOOL SetPackPath(LPCTSTR path);
	//获得皮肤包中文件列表
	BOOL GetFileList(std::vector<CString>& vecFileNames) const;
	//获得文件数据
	BOOL GetFileData(LPCTSTR fileName, CFileData& fileData) const;

protected:
	CString m_strPackDir;
	std::map<CString, CFileData> m_mapFileData;
};
