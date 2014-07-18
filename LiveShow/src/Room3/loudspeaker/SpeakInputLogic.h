#pragma once
#include "..\iectrl\HtmlInputLogic.h"

using namespace boost::xpressive;

struct IHtmlSink
{
	virtual bool OnInput(int nLen) = 0;
};

class CSpeakInputLogic : public CHtmlInputLogic
{
public:
	CSpeakInputLogic(IHtmlSink * pHtmlSink);
	virtual ~CSpeakInputLogic(void);

public:
	virtual bool OnInput(HtmlEditImpl* pHtmlEditImp);
private:
	IHtmlSink * m_pHtmlSink;
};
