#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "..\screeneffect\FlyingInputRichEdit.h"

// CSongWishDlg 对话框

class CSongWishDlg : public CDialog
{
	DECLARE_DYNAMIC(CSongWishDlg)

public:
	CSongWishDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSongWishDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_SONG_WISH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
public:
	void SetSongInfo(uint32 unRoomID, int nSongerID, CString strName, int nCount = 1);
	CString GetSongWish();

protected:
	void OnClickOK();
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CFlyingInputRichEdit m_InputCtrl;
	uint32 m_unRoomID;
	int m_nSongerID;
	CString m_strSongName;
	int m_nCount;
};
