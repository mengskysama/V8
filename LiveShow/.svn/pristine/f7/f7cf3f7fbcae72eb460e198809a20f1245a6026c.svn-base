#pragma once
#include <vector>

class ILayout;

enum AlignmentFlag
{
    LEFT = 0x00000001,
    CENTER = 0x00000002,
    RIGHT = 0x00000004,
    FILL = 0x00000010,
    TOP = 0x00000002,
    VCENTER = 0x00000001,
    BOTTOM = 0x00000008,
    VFILL = 0x00000002,
};

class ILayoutItem
{
public:
    ILayoutItem();
    virtual ~ILayoutItem();

public:
    virtual tagSIZE SizeHint() = 0;
    virtual tagSIZE MinSize() = 0;
    virtual tagSIZE MaxSize() = 0;
    virtual RECT GetRect() = 0;
    virtual void SetRect(const RECT& rcRect) = 0;

public:
    ILayout* GetLayout();
    void SetLayout(ILayout* pLayout);
    tagSIZE GetRootOffset();
    AlignmentFlag GetAlignment();
    void SetAlignment(AlignmentFlag enAlign);

private:
    ILayout* m_pLayout;
    AlignmentFlag m_enAlignFlag;
};
