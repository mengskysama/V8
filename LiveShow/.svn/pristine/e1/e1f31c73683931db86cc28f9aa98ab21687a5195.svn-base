#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

// CP2PChatDlg 对话框
class CRoomManagerDlg ;
class CVideoDlg;
class CMicPopupDlg ;
class CP2PChatDlg : public CDialog
{
	DECLARE_DYNAMIC(CP2PChatDlg)

public:
	CP2PChatDlg( CRoomParentDlg* pParentDlg ,uint32 unSrcUIN , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PChatDlg();

// 对话框数据
	enum { IDD = IDD_P2P_VIDEO_DLG };
	CRoomParentDlg* m_pParentDlg ;
	CMicPopupDlg* m_pOpMicPopupDlg ;
	CMicPopupDlg* m_pSelfMicPopupDlg ;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void NotifyCreateSkinItem();
    void OnClickClose();
	void OnClickEndChat();

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT ChangeVideo( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ChangeAudio( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ChangeSize( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT Photo( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ClickVideoDlg( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT MicPopupDlgClose( WPARAM wParam , LPARAM lParam ) ;
public:
	void Init( uint32 unUIN , uint32 un179ID ,uint16 unStatus , CString strName ) ;
	void InitMySelf( uint32 unUIN ,uint16 unStatus ) ;
	void OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN ) ;
	void OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) ;
	HWND GetMicHandle(BOOL bSelf);//获取视频显示窗口的句柄,bSelf为TRUE则是自己的视频窗口，FALSE为对方
	void SetDefault(BOOL bSelf);//设置为默认图片,bSelf为TRUE则是自己的视频窗口，FALSE为对方
	uint32 GetOppsiteUIN() ;
	void CleanAVLogic() ;
private:
	void OpenVideo( uint32 unUIN ) ;
	void CloseVideo( uint32 unUIN ) ;
	void OpenAudio( uint32 unUIN ) ;
	void CloseAudio( uint32 unUIN ) ;
	CComPtr<ISkinEngine> m_pSkinEngine;

	CVideoDlg *m_pOpVideoDlg;
	CVideoDlg *m_pSelfVideoDlg;
	uint32 m_unOpSrcUIN ;
	uint32 m_unOp179ID ;
	CString m_strOpName ;
public:
	
	afx_msg void OnNcDestroy();
};
