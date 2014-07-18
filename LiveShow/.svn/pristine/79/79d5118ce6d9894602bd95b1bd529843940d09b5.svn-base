#pragma once
#include "Skin_i.h"
#include "afxwin.h"


// CSpotSongDlg 对话框

class CSpotSongDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpotSongDlg)

public:
	CSpotSongDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpotSongDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_SPOT_SONG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void OnClickOK();

	DECLARE_MESSAGE_MAP()
public:
	void SetNoEnoughMoney(BOOL bShow, int songNum);
	void SetTheSongPrice(CString songPrice);
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CEdit m_editSongNum;	
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
