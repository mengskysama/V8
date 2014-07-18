#pragma once

#include <mshtml.h>
#include <afxtempl.h>

template<class THtmlElement> class CHtmlElements
{
	typedef CMap<LPDISPATCH, LPDISPATCH, THtmlElement*, THtmlElement*> CMapDispToHtmlElement;
	CMapDispToHtmlElement m_htmlElements;
	BOOL IsSiteConnected( LPDISPATCH pDisp )
	{
		THtmlElement *pElement;
		return m_htmlElements.Lookup( pDisp, pElement );
	}
public:
	CHtmlElements(void)
	{
	}
	~CHtmlElements(void)
	{
	}
public:
	void SetSite( LPDISPATCH pDisp )
	{
		if ( IsSiteConnected( pDisp ) ) //检查以避免多余的Sink
		{
			return;
		}
		THtmlElement *pElement = new THtmlElement; //通过模板类型创建相应的类的实例进行连接
		pElement->SetSite( pDisp );
		m_htmlElements.SetAt( pDisp, pElement );
	}

	//在合适的地方调用Clear释放所管理的内存

	void Clear(void)

	{

		POSITION pos = m_htmlElements.GetStartPosition();

		THtmlElement *pElement = NULL;

		LPDISPATCH pDisp = NULL;

		while (pos != NULL)

		{

			m_htmlElements.GetNextAssoc( pos, pDisp, pElement );

			m_htmlElements.RemoveKey( pDisp );

			delete pElement;

		}

	}

};
