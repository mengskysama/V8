#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"

// CPrivateMicDlg 对话框

class CVideoDlg;
class CMicPopupDlg ;

class CPrivateMicDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrivateMicDlg)

public:
	CPrivateMicDlg( CRoomParentDlg* pParentDlg ,int nIndex ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrivateMicDlg();

// 对话框数据
	enum { IDD = IDD_PRIVATE_MIC_DLG };
	CRoomParentDlg* m_pParentDlg ;
	CVideoDlg * m_pVideoDlg;
	CMicPopupDlg* m_pMicPopupDlg ;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void NotifyCreateSkinItem();
	

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT ChangeVideo( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ChangeAudio( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ChangeSize( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT Photo( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT ClickVideoDlg( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT MicPopupDlgClose( WPARAM wParam , LPARAM lParam ) ;
private:
	void OpenVideoRsp( uint32 unUIN ) ;
	void OpenVideoNotify( uint32 unUIN ) ;

	void CloseVideo( uint32 unUIN ) ;

	void OpenAudioRsp( uint32 unUIN ) ;
	void OpenAudioNotify( uint32 unUIN ) ;

	void CloseAudio( uint32 unUIN ) ;

	CComPtr<ISkinEngine> m_pSkinEngine;
 
	uint32 m_unUserUIN ;	
	uint32 m_unUser179ID ;
	CString m_strName ;
	int m_nIndex ;
public:
	void Init( uint32 unUIN  ,uint32 un179ID , uint16 unStatus , CString strName ) ;
	void OnOpVideoAudioRsp( uint32 nOpAVType ,uint32 unUIN ) ;
	void OnOpVideoAudioNotify( uint32 nOpAVType ,uint32 unUIN ) ;	
	HWND GetMicHandle() ;
	void SetDefault();//设置为默认图片
	int GetIndex() ;
	void CleanAVLogic() ;
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};
