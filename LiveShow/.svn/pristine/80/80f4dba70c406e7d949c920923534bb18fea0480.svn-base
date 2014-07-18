#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"


// CPublicMicDlg 对话框

class CVideoDlg;
class CMicPopupDlg ;

class CPublicMicDlg : public CDialog
{
	DECLARE_DYNAMIC(CPublicMicDlg)

public:
	CPublicMicDlg( CRoomParentDlg* pParentDlg , int16 nIndex ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPublicMicDlg();

// 对话框数据
	enum { IDD = IDD_PUBLIC_MIC_DLG };
	CRoomParentDlg* m_pParentDlg ;

	CMicPopupDlg* m_pMicPopupDlg ;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void NotifyCreateSkinItem();

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT ChangeVideo( WPARAM wParam ,LPARAM lParam ) ;	
	afx_msg LRESULT ChangeSize( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT Photo( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ClickVideoDlg( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT MicPopupDlgClose( WPARAM wParam , LPARAM lParam ) ;

	void OnClickOrderSong();
public:
	void Init( uint32 unUIN , uint32 un179ID , uint16 unStatus , CString strName ,uint32 unTime = 0, bool bActor = false) ;
	afx_msg LRESULT ChangeAudio( WPARAM wParam ,LPARAM lParam ) ;
	HWND GetMicHandle() ;
	void OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN ) ;
	void OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) ;
	void SetDefault();//设置为默认图片
	uint32 GetUserUIN(){ return m_unUserUIN ;} ;
	void CleanAVLogic() ;

	void CloseVideo() ;
	void OpenVideo() ;

	void SetBtnOrderSongVisible(bool bShow);
private:
	void OpenVideoRsp( uint32 unUIN ) ;
	void OpenVideoNotify( uint32 unUIN ) ;

	void OnCloseVideo( uint32 unUIN ) ;

	void OpenAudioRsp( uint32 unUIN ) ;
	void OpenAudioNotify( uint32 unUIN ) ;

	void OnCloseAudio( uint32 unUIN ) ;	
	void SetPublicMicEffect( BOOL bOnMic) ;
	CComPtr<ISkinEngine> m_pSkinEngine;
	CString m_strName ;
    uint32 m_unUserUIN ;
	uint32 m_unUser179ID ;
	int m_nPublicMicTime ;
	CVideoDlg * m_pVideoDlg;//视频显示窗口
	int16 m_nIndex ;			//第几个公麦
	
};
