#include "StdAfx.h"
#include ".\filepathname.h"

CFilePathName::CFilePathName(void)
{
}

CFilePathName::CFilePathName(LPCTSTR filePathName)
:m_strFilePathName(filePathName)
{
    m_strFilePathName.MakeLower();
}

CFilePathName::CFilePathName(const CFilePathName& r)
{
	m_strFilePathName = r.m_strFilePathName;
    //m_strFilePathName.MakeLower();
}

CFilePathName::~CFilePathName(void)
{
}

CFilePathName& CFilePathName::operator = (const CFilePathName& r)
{
	if (this != &r){
		m_strFilePathName = r.m_strFilePathName;
	}
	return *this;
}

CFilePathName& CFilePathName::operator = (LPCTSTR filePathName)
{
	m_strFilePathName = filePathName;
    m_strFilePathName.MakeLower();
	return *this;
}

CFilePathName::operator LPCTSTR () const
{
	return (LPCTSTR)m_strFilePathName;
}

bool CFilePathName::operator < (const CFilePathName& r) const
{
	//CString lPathName = m_strFilePathName;
	//CString rPathName = r.m_strFilePathName;

	////windows 下路径不区分大小写
	//lPathName.MakeLower();
	//rPathName.MakeLower();

	////替换为统一的分隔符号
	//lPathName.Replace(_T('/'), _T('\\'));
	//rPathName.Replace(_T('/'), _T('\\'));

	////返回
	//return (lPathName < rPathName);

    return (m_strFilePathName < r.m_strFilePathName);
}

bool CFilePathName::operator == (const CFilePathName& r) const
{
	//CString lPathName = m_strFilePathName;
	//CString rPathName = r.m_strFilePathName;

	////windows 下路径不区分大小写
	//lPathName.MakeLower();
	//rPathName.MakeLower();

	////替换为统一的分隔符号
	//lPathName.Replace(_T('/'), _T('\\'));
	//rPathName.Replace(_T('/'), _T('\\'));

	////返回
	//return (lPathName == rPathName);

    return (m_strFilePathName == r.m_strFilePathName);
}

bool CFilePathName::operator <= (const CFilePathName& r) const
{
	return (*this < r || *this == r);
}

bool CFilePathName::operator >= (const CFilePathName& r) const
{
	return !(*this < r);
}

bool CFilePathName::operator > (const CFilePathName& r) const
{
	return ( !(*this < r) && !(*this == r) );
}
