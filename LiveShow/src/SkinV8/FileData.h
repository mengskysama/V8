#pragma once

#include <fstream>

typedef struct tagFILEDATA{
	long	lLen;
	char*	pFileData;
} FileData, FILEDATA;

class CFileData : public tagFILEDATA
{
public:
	CFileData(void)
	{
		lLen = 0;
		pFileData = NULL;
	};

	CFileData(const CFileData& fileData)
	{
		lLen = 0;
		pFileData = NULL;

		if (this == &fileData)
			return;

		Resize(fileData.lLen);

		_macro_memcpy(pFileData, lLen, fileData.pFileData, fileData.lLen);
	};

	CFileData(const FileData& fileData)
	{
		lLen = 0;
		pFileData = NULL;

		if (this == &fileData)
			return;

		Resize(fileData.lLen);

		_macro_memcpy(pFileData, lLen, fileData.pFileData, fileData.lLen);
	};

	~CFileData(void)
	{
		Free();
	};

public:
	void Clear()
	{
		Free();
	};

	void operator = (const FileData& fileData)
	{
		if (this == &fileData)
			return;

		Resize(fileData.lLen);

		_macro_memcpy(pFileData, lLen, fileData.pFileData, fileData.lLen);
	};

	void operator = (const CFileData& fileData)
	{
		*this = (const FileData&)(fileData);
	};

	operator const char* () const
	{
		return pFileData;
	};

	LONG GetLength()
	{
		return lLen;
	};

	BOOL LoadFromFile(LPCTSTR fileName)
	{
		CFile file;
		if (! file.Open(fileName, CFile::modeRead | CFile::shareDenyNone)) {
			ASSERT(0 && _T("CFileRepository::LoadFile 找不到文件或文件拒绝访问"));
			return FALSE;
		}

		ULONGLONG ullLen = file.GetLength();
		if (ullLen >= UINT_MAX) {
			ASSERT(0 && _T("文件过大"));
			return FALSE;
		}

		UINT fileLen = (UINT)ullLen;
		UINT memLen = fileLen + 2;
		Resize(memLen, true);

		file.Read(pFileData, fileLen);

		return TRUE;
	}

	BOOL WriteToFile(CFile& file) const
	{
		file.Write(&lLen, sizeof(lLen));
		file.Write(pFileData, lLen);
		return TRUE;
	}

	BOOL ReadFromFile(CFile& file)
	{
		LONG len = -1;
		ULONGLONG oldPos = file.GetPosition();
		if ( sizeof(len) != file.Read(&len, sizeof(len)) ) {
			file.Seek(oldPos, CFile::begin);
			return FALSE;
		}

		Resize(len, true);

		UINT readLen = file.Read(pFileData, lLen);
		if (readLen != lLen) {
			ASSERT(0 && _T("文件读取错误"));
			file.Seek(oldPos, CFile::begin);
			return FALSE;
		}
		return TRUE;
	}

	BOOL WriteToFile(std::fstream& fout) const
	{
		if (fout.fail())
			return FALSE;

		fout.write((char*)&lLen, sizeof(lLen));
		fout.write(pFileData, lLen);
		return TRUE;
	}

	BOOL ReadFromFile(std::fstream& fin)
	{
		if (fin.fail())
			return FALSE;

		LONG len = -1;
		fin.read((char*)&lLen, sizeof(lLen));

		Resize(len, true);

		fin.read(pFileData, lLen);

		return TRUE;
	}

	BOOL WriteToBuffer(void* pBuffer, LONG nBufLen, LONG& nWriteLen) const
	{
		nWriteLen = 0;
		LONG size = GetSerializedSize();
		if (size > nBufLen)
			return FALSE;
		char* pBuf = (char*)pBuffer;
		_macro_memcpy(pBuf, nBufLen, &lLen, sizeof(lLen));
		pBuf += sizeof(lLen);
		nBufLen -= sizeof(lLen);
		_macro_memcpy(pBuf, nBufLen, pFileData, lLen);
		nWriteLen = size;
		return TRUE;
	}

	BOOL ReadFromBuffer(const void* pBuffer, LONG nBufLen, LONG& nReadLen)
	{
		nReadLen = 0;
		if (nBufLen < sizeof(lLen))
			return FALSE;
		LONG len;
		char* pBuf = (char*)pBuffer;
		_macro_memcpy(&len, sizeof(len), pBuf, sizeof(len));
		ASSERT(len >= 0);
		pBuf += sizeof(len);
		nBufLen -= sizeof(len);
		if (len > nBufLen)
			return FALSE;
		if (! Resize(len))
			return FALSE;
		_macro_memcpy(pFileData, lLen, pBuf, len);
		nReadLen = sizeof(len) + len;
		return TRUE;
	}

	LONG GetSerializedSize() const
	{
		LONG size = 0;
		size += sizeof(lLen);
		size += lLen;

		return size;
	}

protected:
	void Free()
	{
		ASSERT((lLen == 0 && pFileData == NULL) || (lLen != 0 && pFileData != NULL));
		if (pFileData != NULL) {
			delete [] pFileData;
			pFileData = NULL;
			lLen = 0;
		}
	};

	BOOL Resize(long newSize, bool bZero = false)
	{
		ASSERT((lLen == 0 && pFileData == NULL) || (lLen != 0 && pFileData != NULL));
		if (newSize < 0)
			return FALSE;
		if (lLen == newSize) {
			if (bZero) {
				memset(pFileData, 0, lLen);
			}
			return TRUE;
		}
		Free();
		if (newSize > 0) {
			pFileData = new char [newSize];
			lLen = newSize;
		}
		if (bZero) {
			memset(pFileData, 0, lLen);
		}
		return TRUE;
	};
};
