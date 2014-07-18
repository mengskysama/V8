#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

// CPublicMicToolBarDlg 对话框

typedef int BTN_STATUS;//按键状态，-1表示disable(点击无效)，0表示打开状态，1表示关闭状态,默认都为0
enum eBtnStatus{
	BTN_STATUS_CLICKDISABLE    = -1     ,
	BTN_STATUS_OPEN            = 0      ,			//用户音视频已经打开
	BTN_STATUS_CLOSE           = 1      ,			//用户音视频已经关闭
};

class CVideoDlg ;
class CMicToolBarDlg : public CDialog
{
	DECLARE_DYNAMIC(CMicToolBarDlg)

public:
	CMicToolBarDlg(CVideoDlg* pVidepDlg ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMicToolBarDlg();

// 对话框数据
	enum { IDD = IDD_MIC_TOOLBAR };
	CVideoDlg* m_pVideoDlg ;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnClickPopup();
	void OnClickVideo();
	void OnClickAudio();
	void OnClickTakePhoto();

	DECLARE_MESSAGE_MAP()
public:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void InitBtn( uint32 unUIN , uint16 uUserStatus ) ;
	void SetBtnPopupStatus(eBtnStatus btnStatus , BOOL bHoveImage = TRUE );
	void SetBtnVideoStatus(eBtnStatus btnStatus , BOOL bHoveImage = TRUE );
	void SetBtnAudioStatus(eBtnStatus btnStatus , BOOL bHoveImage = TRUE );
	void SetBtnPhotoStatus(eBtnStatus btnStatus , BOOL bHoveImage = TRUE );
private:
	HINSTANCE m_hInst;
	CComPtr<ISkinEngine> m_pSkinEngine;
	uint16 m_uUserStatus ;
	uint32 m_unUserUIN ;
	BTN_STATUS m_btnPopup,m_btnVideo,m_btnAudio,m_btnPhoto;

};
