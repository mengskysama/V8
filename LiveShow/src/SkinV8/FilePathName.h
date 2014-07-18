#pragma once

class CFilePathName
{
public:
	CFilePathName(void);
	CFilePathName(const CFilePathName& r);
	CFilePathName(LPCTSTR filePathName);
	~CFilePathName(void);
	CFilePathName& operator = (const CFilePathName& r);
	CFilePathName& operator = (LPCTSTR filePathName);
	operator LPCTSTR () const;
	bool operator < (const CFilePathName& r) const;
	bool operator == (const CFilePathName& r) const;
	bool operator <= (const CFilePathName& r) const;
	bool operator >= (const CFilePathName& r) const;
	bool operator > (const CFilePathName& r) const;
protected:
	CString m_strFilePathName;
};
