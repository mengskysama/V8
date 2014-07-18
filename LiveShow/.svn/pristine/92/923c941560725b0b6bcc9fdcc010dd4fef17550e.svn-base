#pragma once
#include "..\flash\flashWnd.h"

class CGuideFlash : public CFlashWnd
{
public:
	CGuideFlash(CString strParam);
	~CGuideFlash(void);

	virtual void FlashCall(const CString& request );

private:
	CString m_strParam;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
