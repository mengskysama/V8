#pragma once


// CTheMoreDlg 对话框
class CRoomParentDlg;
class CTheMoreDlg : public CDialog
{
	DECLARE_DYNAMIC(CTheMoreDlg)

public:
	CTheMoreDlg( CRoomParentDlg* pParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTheMoreDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_THE_MORE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void OnClickBtnStamp();
	void OnClickBtnCleanScreen();
	void OnClickBtnCopyUrl();

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pRoomParentDlg ;
	
};
