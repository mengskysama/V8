

#pragma once

#include "../Common.h"

namespace common { namespace ui
{

enum
{
	WM_DropSource_Canceled = WM_USER + 0x4291,
	WM_DropSource_Droped,
};

// CDropSource 命令目标

/**
@class CDropSource
@brief 拖拽源类
*/
class COMMON_INTERFACE CDropSource : public COleDropSource
{
	DECLARE_DYNAMIC(CDropSource)

public:
    /**
    @fn CDropSource()
    @brief 构造函数
    */
	CDropSource();

    /**
    @fn ~CDropSource()
    @brief 析构函数
    */
	virtual ~CDropSource();

    /**
    @fn BOOL OnBeginDrag(CWnd* pWnd,CRect rcItem)
    @brief 开始拖拽
    @param [in] pWnd
    @param [in] rcItem
    @return 
    */
	virtual BOOL OnBeginDrag(CWnd* pWnd, CRect rcItem);

    /**
    @fn SCODE GiveFeedback(DROPEFFECT dropEffect)
    @brief 
    @param [in] dropEffect
    @return 
    */
	virtual SCODE GiveFeedback(DROPEFFECT /*dropEffect*/);

    /**
    @fn SCODE QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState)
    @brief 
    @param [in] bEscapePressed
    @param [in] dwKeyState
    @return 
    */
	virtual SCODE QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState);

    /**
    @fn void Register(CWnd* pWnd)
    @brief 注册
    @param [in] pWnd 注册拖拽的窗口
    */
	void Register(CWnd* pWnd);

protected:
	DECLARE_MESSAGE_MAP()

private:
    BOOL m_bLBtnUp;
    CWnd* m_pwnd;
};

} } //namespace common::ui
