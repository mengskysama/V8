#pragma once

#include "Skin_i.h"
#include "Resource.h"
#include "..\CommonLibrary\include\ui\ImageEx.h"
// CEnteringRoomDlg 对话框
#define ENTER_ROOM_TIMER						10086

class CHallDlg;
class CEnterRoomRequest;
class CEnteringRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnteringRoomDlg)

public:
	CEnteringRoomDlg(CHallDlg *hallDlg,CEnterRoomRequest *enterRoom,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEnteringRoomDlg();

// 对话框数据
	enum { IDD = IDD_ENTERING_ROOM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()
private:
	CHallDlg *m_pHallDlg; 
	common::ui::ImageEx *m_pPic ;	
	common::ui::ImageEx *m_imgTopLeft;
	common::ui::ImageEx *m_imgTopMid;
	common::ui::ImageEx *m_imgTopRight;
	common::ui::ImageEx *m_imgMidLeft;
	common::ui::ImageEx *m_imgMidRight;
	common::ui::ImageEx *m_imgBottomLeft;
	common::ui::ImageEx *m_imgBottomMid;
	common::ui::ImageEx *m_imgBottomRight;
	CEnterRoomRequest *m_enterRoom;	
	HRGN m_hrgn;
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
