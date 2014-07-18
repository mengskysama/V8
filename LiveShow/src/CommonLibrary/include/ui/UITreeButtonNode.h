#pragma once

#include "../Common.h"
#include "iuitreenode.h"

namespace common { namespace ui
{

class IUITreeContainer;

class COMMON_INTERFACE CUITreeButtonNode :
    public IUITreeNode
{
public:
    CUITreeButtonNode(IUITreeContainer* pContainer);
    virtual ~CUITreeButtonNode(void);

    virtual HCURSOR GetCursor(POINT pt);
    virtual void OnLButtonDown(POINT pt, UINT uFlag);
    virtual void OnLButtonUp(POINT pt, UINT uFlag);
    virtual void OnMouseEnter(POINT pt, UINT uFlag);
	virtual	void OnMouseLeave(POINT pt, UINT uFlag);

    // Ù–‘…Ë÷√
    int GetCursor();
    void SetCursor(int iCursor);
    CString GetTip();
    void SetTip(LPCTSTR lpzTip);

protected:
    int m_iCursor;
    CString m_strTip;
};

} } //namespace common::ui
