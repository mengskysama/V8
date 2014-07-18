#pragma once

#include "resource.h"
#include "Skin_i.h"
#include "TrackerCtrl.h"
#include "PicShowCtrl.h"
#include "VDCSliderCtrl.h"
#include "..\src\CommonLibrary\thirdlibrary\CxImage\include\ximage.h"
#include "afxcmn.h"

// CPicTracker 对话框

class CPicTracker : public CDialog
{
	DECLARE_DYNAMIC(CPicTracker)

public:
	CPicTracker(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicTracker();

// 对话框数据
	enum { IDD = IDD_PIC_TRACKER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	void OnClickBtnZoomOut();//缩小 
	void OnClickBtnZoomIn();//放大
	void OnClickBtnOrigin();//原始大小
	void OnClickBtnFit();//最适应
	void OnClickBtnBrowser();//浏览
	void OnClickBtnCorfirm();//确定
	void OnClickBtnClose();//取消，关闭

	DECLARE_MESSAGE_MAP()

public:
	CPicShowCtrl             m_picShowCtrl;
	CPicShowCtrl             m_picShowCtrl2;
	void                     SetPath(CString strPath);//设置取图片的路径
	void					 SetDir(CString strDir);//设置存图片的路径
	CString					 GetPath();

	void					 UpdateSliderPos();

private:
	CTrackerCtrl               m_PicCtrlBig;                  //大图控件

	CxImage                    m_PicImg;                      //大图片

	CString                    m_strPath;					//设置和获取都用此路径

	CString                    m_strSaveDir;					//存储路径

	CComPtr<ISkinEngine> m_pSkinEngine;    //skin Engine

	CVDCSliderCtrl  m_SliderZoom;
	int          m_iSliderPos;

};
