#pragma once

#include "ILayoutItem.h"

class CSkinBase;

class CSkinLayoutItem :
    public ILayoutItem
{
public:
    CSkinLayoutItem(CSkinBase* pSkin);
    virtual ~CSkinLayoutItem(void);

public:
    virtual tagSIZE SizeHint() = 0;
    virtual tagSIZE MinSize() = 0;
    virtual tagSIZE MaxSize() = 0;
    virtual RECT GetRect() = 0;
    virtual void SetRect(const RECT& rcRect) = 0;

private:
    CSkinBase* m_pSkin;
};
