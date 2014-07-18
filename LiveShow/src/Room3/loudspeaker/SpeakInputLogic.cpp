#include "StdAfx.h"
#include "SpeakInputLogic.h"
#include "..\..\CommonLibrary\include\utility\StringHelper.h"
CSpeakInputLogic::CSpeakInputLogic(IHtmlSink * pHtmlSink)
:/*HtmlEditLogic(2),*/m_pHtmlSink(pHtmlSink)
{
}

CSpeakInputLogic::~CSpeakInputLogic(void)
{
}

//输入字符时的字符个数判断
bool CSpeakInputLogic::OnInput(HtmlEditImpl* pHtmlEditImp)
{
	CString strText = m_pHtmlEditImp->GetContentText();
	int nLen = common::utility::stringhelper::CalcStringAsciiLength(strText.GetBuffer(0));
	if (m_pHtmlSink)
	{
		return m_pHtmlSink->OnInput(nLen);
	}
	return true ;
}