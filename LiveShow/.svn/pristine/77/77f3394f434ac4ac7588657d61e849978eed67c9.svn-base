#pragma once

#include "FilePathName.h"

#include <map>
#include <set>

class CImageBase;
class CImageTransformParam;
/*
@brief 图像库
*/
class CImageRepository
{
public:
	/*
	@brief 构造函数
	*/
	CImageRepository(void);

	/*
	@brief 析构函数
	*/
	virtual ~CImageRepository(void);

	/*
	@brief 从文件载入图片
	@param filePathName 文件的全路径名
	@return 失败返回NULL
	*/
	virtual CImageBase* LoadImage(LPCTSTR filePathName);

	/*
	@brief 从内存文件载入图片
	@param filePathName 文件的全路径名
	@param pzData 文件的数据指针
	@param size 文件的长度
	@return 失败返回NULL
	*/
	virtual CImageBase* LoadImage(LPCTSTR filePathName, const char* pzData, long size);

	static BOOL TransformColor(const CImageTransformParam* pParam, LONG color, LONG& transformColor);

	/*
	@brief 测试
	*/
	void TextOut(LPCTSTR textName);

	virtual BOOL GetRepositorySize(LONG& lNum, LONG& lSize) const;
protected:
	std::map<CFilePathName, CImageBase*> m_mapImageRepository;
};
