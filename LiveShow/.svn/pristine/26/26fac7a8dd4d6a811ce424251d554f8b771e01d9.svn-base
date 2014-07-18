#pragma once
#include "ILayoutItem.h"
#include <vector>

class ILayout :
    public ILayoutItem
{
public:
    ILayout()
        :
    m_rcRect(0, 0, 0, 0)
    {
    };

    virtual ~ILayout()
    {
    };

public:
    void AddLayer(ILayout* pLayer)
    {
        pLayer->SetLayout(this);
        m_vecLayers.push_back(pLayer);
    };

public:
    virtual tagSIZE SizeHint()
    {
        return m_rcRect.Size();
    }

    virtual tagSIZE MinSize()
    {
        return CSize(0, 0);
    }

    virtual tagSIZE MaxSize()
    {
        return CSize(-1, -1);
    }

    virtual RECT GetRect()
    {
        return m_rcRect;
    };

    virtual void SetRect(const RECT& rcRect)
    {
        m_rcRect = rcRect;

        //²¼¾Ö²ã
        CRect rcAbs = m_rcRect;
        rcAbs.OffsetRect(-rcAbs.left, -rcAbs.top);
        std::vector<ILayout*>::iterator iter = m_vecLayers.begin(),
            last = m_vecLayers.end();
        for (; iter!=last; iter++) {
            ILayout* pLayer = *iter;
            pLayer->SetRect(rcAbs);
        }
    };

protected:
    CRect m_rcRect;
    std::vector<ILayout*> m_vecLayers;
};
