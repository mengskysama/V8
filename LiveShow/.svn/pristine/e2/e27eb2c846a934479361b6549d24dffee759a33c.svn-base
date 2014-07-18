#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "flash.h"


// CSofaDlg 对话框

class CSofaDlg : public CDialog
{
	DECLARE_DYNAMIC(CSofaDlg)

public:
	CSofaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSofaDlg();

	void InitSofaList(uint32 unSelf179ID, bool bGuest, CString strInitInfo);
	void UpdateSofa(uint8 unIndex, CString strSofaInfo);				//单个沙发用户上座
	void UpdateSofaInfo(CString strSofaInfo);			//沙发用户信息更新
	void UpdateSize(int cx);
	void ClearSofaList();
	void HideSendPanel();
	bool IsInited();

// 对话框数据
	enum { IDD = IDD_SOFA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CFlash m_flash;
	DECLARE_EVENTSINK_MAP()
	void FlashCallFlash(LPCTSTR request);
	void FSCommandFlash(LPCTSTR command, LPCTSTR args);
	virtual BOOL OnInitDialog();
	void CallFunction(CString func, std::vector<CString> vecParams);

	CComPtr<ISkinEngine> m_pSkinEngine;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnClickFlash(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlashKillFocus(WPARAM wParam, LPARAM lParam);

protected:
	void ParseParams(CString strParam, CString& strFunc, std::vector<CString>& vecParam);

private:
	bool m_bVersionLow;
	bool m_bInited;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
