#pragma once
#include "afxwin.h"


// CSongMarkDlg 对话框
class CRoomParentDlg;

class CSongMarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CSongMarkDlg)

public:
	CSongMarkDlg(CRoomParentDlg* pParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSongMarkDlg();

// 对话框数据
	enum { IDD = IDD_SONGMARK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedRadioGoodMark();
	afx_msg void OnBnClickedRadioBadMark();
	afx_msg LRESULT OnClickMsgBoxGoToRegister(WPARAM wParam, LPARAM lParam);

	void OnClickGetTicket();
	void OnClickBtnOKIn12();
	void OnClickBtnOKIn13();
	void OnClickBtnOrderSong();
	void OnClickBtnOKIn20();

	DECLARE_MESSAGE_MAP()
public:
	void SetRoomID(uint32 unRoomID);
	void SetOrderID(uint32 unOrderID);
	void SetSongNameAndActor(CString strSongName, CString actorName);
	void Show1(CString str1, CString str2, int countDown, int leaveTicket);
	void Show2(CString str1, CString str2, int countDown, int leaveTicket);
	void Show3(CString str1, CString str2, CString str3, int countDown, int leaveTicket);
	void Show4(CString str, int countDown, int leaveTicket);
	void Show5(CString str1, CString str2, int countDown, int leaveTicket);
	void Show6(CString str1, CString str2, CString str3);
	void Show7(CString str1, CString str2);
	void Show8(CString str1, CString str2, CString str3);
	void Show9(CString str1, CString str2);
	void Show10(CString str1, CString str2);
	void Show11(CString str1, CString str2);
	void Show12(CString str1, CString strTip, BOOL bCheck, int markTime, BOOL bGoodMark, CString strGoodNum, CString strBadNum);
	void Show13(int markTime, CString str2, CString strMark, BOOL bGoodMark);
	void Show14(CString str1, int markTime, CString strMark, CString strGoodNum, CString strBadNum);
	void Show15(int markTime, CString str2, CString strMark, CString strGoodNum, CString strBadNum);
	void Show16(int markTime, CString strMark, CString strGoodNum, CString strBadNum);
	void Show17(CString str1, CString str2, CString strMark, CString strGoodNum, CString strBadNum, CString strGiveUpNum);
	void Show18(CString str1, CString str2);
	void Show19(CString str1, CString str2, CString str3);
	void Show20(int markTime, CString strTip, BOOL bGoodMark, CString strGoodNum, CString strBadNum);
	
	int GetCurShow(){ return m_nCurShow; }//当前展示哪个界面
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
    int m_countDownTime;
	int m_nCurShow;

	CButton m_checkUserOne;
	CRoomParentDlg * m_pRoomParentDlg;

	uint32 m_unRoomID;
	uint32 m_unOrderID;
};
