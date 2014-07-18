// NumEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "NumEdit.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"


// CNumEdit

IMPLEMENT_DYNAMIC(CNumEdit, CEdit)

CNumEdit::CNumEdit()
{

}

CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, &CNumEdit::OnEnChange)
END_MESSAGE_MAP()



// CNumEdit 消息处理程序



void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bCanInput;
	bCanInput = ((nChar >= '0' && nChar <= '9') || nChar == VK_BACK || nChar == VK_DELETE);
	if (!bCanInput)
	{
		return;
	}

	CString strText;
	GetWindowText(strText);
	int nLen = strText.GetLength();
	if (nLen > 6)
	{
		return;
	}

	//if (strText == L"" && nChar == '0')
	//{
	//	return;
	//}

	//std::wstring strCount = strText.GetBuffer(0);
	//int nCount = 0;
	//if (!common::utility::stringhelper::StringToInt(strCount, &nCount))
	//{
	//	return;
	//}

	//if (nCount == 0)
	//{
	//	return;
	//}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CNumEdit::OnEnChange()
{
	CString strText;
	GetWindowText(strText);

	std::wstring strCount = strText.GetBuffer(0);
	int nCount = 0;
	if (common::utility::stringhelper::StringToInt(strCount, &nCount))
	{
		if (nCount == 0)
		{
			SetWindowText(L"");
		}
	}
	else
	{
		SetWindowText(L"");
	}
}
