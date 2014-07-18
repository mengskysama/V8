// ConvertToPinYin.h: interface for the CPinyin class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class UTILITY_API CPinyin  
{
public:
	CPinyin();
	virtual ~CPinyin();
	CString Change(CString strSource);


protected:
	struct PinYin
	{
		CString strPinYin;
		int nMinValue;
	};
	CArray<PinYin *, PinYin *> m_PinYin;
	void AddPinYin(CString strPinYin, int iMinValue);
	void Init();
};


