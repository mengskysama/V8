#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include "LoginDlg.h"
#include "../../include/ui/mlhtmlview.h"
// CRegisterDlg 对话框

struct Register179Account
{
	CString m_Account;
	CString m_Password;  //原始密码
};

enum eDlgType
{
     Dlg_Type_For_Register = 1,
	 Dlg_Type_For_CompleteInfo
};

class CRegisterDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(HWND sendMesHWND,eDlgType dlgType = Dlg_Type_For_Register,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterDlg();

// 对话框数据
	enum { IDD = IDD_REGISTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnClick(WPARAM wParam,LPARAM lParam);   //载取点击m_htmlRegister注册页面中超链接时的响应函数
	afx_msg LRESULT OnOpenURL(WPARAM wParam,LPARAM lParam);

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnSysCommand(UINT nID,LPARAM lParam);
	
	void Register179Success();  //179注册成功页面时，调整注册框大小
	void RegisterElementSetFocus(); //设置内嵌页面的焦点

	void SetWindowSize(int wndWidth,int wndHeight);//当注册成功或完善资料完成时，调用此接口以改变窗口大小
	void InitRegisterBrowser();

	DECLARE_MESSAGE_MAP()

public:	
	int GetDlgType();  //获取对话框类型 
	void SetRoomRegister(bool isRoom);  //设置是否为房间注册框
	bool RoomRegisterDlgIsShow();  //房间注册框是否正在显示
	
	
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMLHtmlView m_htmlRegister;
	int m_dlgType;
	HWND m_sendMesHWND;	
	CWnd* m_pParent;
	bool m_bRoomRegister;  //是否为房间注册框


	
};
