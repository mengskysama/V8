/**********************************************************
* 版权所有 (C)2009, 51.com
*
* 文件名称：CommonWndOleDropTarget.h
* 内容摘要：拖拽COleDropTarget扩展类，用于实现在拖拽目的方实现拖拽功能
* 其它说明：
* 当前版本：
* 作    者：温辉敏
* 完成日期：2009-10-29
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
**********************************************************/

#pragma once

#include "../Common.h"

namespace common { namespace ui
{

// CCommonWndOleDropTarget 命令目标

/** CCommonWndOleDropTarget 负责完成拖拽目的方相应拖拽功能
1.相应的CWnd的子类窗口或控件(以下简称为CWndDropTargetClient)实现ICommonWndOleDropTarget接口(标准的拖拽接口)。
2.CWndDropTargetClient中定义一个CCommonWndOleDropTarget类型成员变量m_commonWndDropTarget。
3.CWndDropTargetClient::OnCreate中调用m_commonWndDropTarget.Register(this);
4.CWndDropTargetClient头文件爱你中必须有：
	DECLARE_DYNAMIC(CWndDropTargetClient)
	实现文件中必须有：
	IMPLEMENT_DYNAMIC(CWndDropTargetClient, COleDropTarget)
*/
class COMMON_INTERFACE CCommonWndOleDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CCommonWndOleDropTarget)
public:
	CCommonWndOleDropTarget();
	virtual ~CCommonWndOleDropTarget();

	virtual DROPEFFECT OnDragEnter(
		CWnd* pWnd, 
		COleDataObject* pDataObject,
		DWORD dwKeyState,
		CPoint point 
		);

	virtual DROPEFFECT OnDragOver(
		CWnd* pWnd, 
		COleDataObject* pDataObject, 
		DWORD dwKeyState, 
		CPoint point
		);

	virtual void OnDragLeave(CWnd* pWnd);               

	virtual BOOL OnDrop(
		CWnd* pWnd, 
		COleDataObject* pDataObject, 
		DROPEFFECT dropEffect, 
		CPoint point
		);
protected:
	DECLARE_MESSAGE_MAP()
};

/** 实现COleDropTarget的标准接口
*/
class COMMON_INTERFACE ICommonWndOleDropTarget:public CObject 
{
	DECLARE_DYNAMIC(ICommonWndOleDropTarget)
public:
	virtual DROPEFFECT OnDragEnter(
		CWnd* pWnd, 
		COleDataObject* pDataObject,
		DWORD dwKeyState,
		CPoint point 
		) = 0;

	virtual DROPEFFECT OnDragOver(
		CWnd* pWnd, 
		COleDataObject* pDataObject, 
		DWORD dwKeyState, 
		CPoint point
		) = 0;

	virtual void OnDragLeave(
		CWnd* pWnd
		) = 0;               

	virtual BOOL OnDrop(
		CWnd* pWnd, 
		COleDataObject* pDataObject, 
		DROPEFFECT dropEffect, 
		CPoint point
		) = 0;
};

} } //namespace common::ui
