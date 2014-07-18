#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include <map>

// CMicPopupDlg 对话框

enum eMicPopupDlgSize//窗口大小变化范围
{
    Size_Level_1    =  0, //此时变量curLevel对应为0
	Size_Level_2    ,    
	Size_Level_3    ,    
	Size_Level_4    ,    
	Size_Level_5    ,     
	Size_Level_6    ,     
	Size_Level_7    ,     
};

struct strVideoSize 
{
	LONG lWidth;   //视频宽
	LONG lHeight;  //视频高
};

class CVideoDlg;

class CMicPopupDlg : public CDialog
{
	DECLARE_DYNAMIC(CMicPopupDlg)

public:
	CMicPopupDlg( HWND hParent , RoomAVLogic *pRoomAVLogic ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMicPopupDlg();

// 对话框数据
	enum { IDD = IDD_MIC_POPUP_DLG };

	CVideoDlg * m_pVideoDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void OnClickZoomIn();
	void OnClickZoomOut();
	void ShowSizeLevel(int sizeLevel);
	void InitVideoSize();

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT ClickVideoDlg( WPARAM wParam ,LPARAM lParam ) ;
private:
	HWND m_hParent ;

	CComPtr<ISkinEngine> m_pSkinEngine;
	RoomAVLogic *m_pRoomAVLogic ;
	
	int curSizeLevel;
	uint32 m_unUserUIN ;

	std::map<int, strVideoSize*> m_mapVideoSize;
public:
	HWND GetMicHandle();//上麦时，获取视频显示窗口的句柄
	void SetDefault();//设置为默认图片
	void Init( CString& strName , uint32 unUIN , uint32 un179ID ) ;
};
