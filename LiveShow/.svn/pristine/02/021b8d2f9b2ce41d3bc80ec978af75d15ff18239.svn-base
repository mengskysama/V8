

#pragma once

#include "../Common.h"

namespace common { namespace ui
{

enum
{
	WM_OnDragOver = WM_USER + 0x2487,
	WM_OnDragEnd,
	WM_OnDragLeave,
};

// CDropTarget 命令目标

/**
@class CDropTarget
@brief 拖拽目标类
*/
class COMMON_INTERFACE CDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CDropTarget)

public:
    /**
    @class DragOverInfo
    @brief 拖拽经过信息
    */
	struct DragOverInfo
	{
		CWnd* pWnd;
		COleDataObject* pDataObject;
		DWORD dwKeyState;
		CPoint point;

		DragOverInfo()
		{
			pWnd = NULL;
			pDataObject = NULL;
			dwKeyState = 0;
			point = CPoint(0, 0);
		}
	};

    /**
    @class DragEndInfo
    @brief 拖拽结束信息
    */
	struct DragEndInfo
	{
		CWnd* pWnd;
		COleDataObject* pDataObject;
		DROPEFFECT dropEffect;
		CPoint point;

		DragEndInfo()
		{
			pWnd = NULL;
			pDataObject = NULL;
			dropEffect = DROPEFFECT_NONE;
			point = CPoint(0, 0);
		}
	};

    /**
    @fn CDropTarget()
    @brief 构造函数
    */
	CDropTarget();

    /**
    @fn ~CDropTarget()
    @brief 析构函数
    */
    virtual ~CDropTarget();

    /**
    @fn DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
    @brief 拖拽进入响应
    @param [in] pWnd 窗口
    @param [in] pDataObject 拖拽的数据对象
    @param [in] dwKeyState 键盘状态
    @param [in] point 鼠标坐标
    @return 拖拽返回值，决定是否接收等
    */
	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

    /**
    @fn DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
    @brief 拖拽经过响应
    @param [in] pWnd 窗口
    @param [in] pDataObject 拖拽的数据对象
    @param [in] dwKeyState 键盘状态
    @param [in] point 鼠标坐标
    @return 拖拽返回值，决定是否接收等
    */
	DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

    /**
    @fn void OnDragLeave(CWnd* pWnd)
    @brief 拖拽离开响应
    @param [in] pWnd 窗口
    */
	void OnDragLeave(CWnd* pWnd);               

    /**
    @fn BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
    @brief 发生拖拽
    @param [in] pWnd 窗口
    @param [in] pDataObject 拖拽的数据对象
    @param [in] dwKeyState 键盘状态
    @param [in] point 鼠标坐标
    @return 
    */
	BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
};

} } //namespace common::ui
