#pragma once
#include "..\Resource.h"
#include "Skin_i.h"


// CSofaNoFlashDlg 对话框

class CSofaNoFlashDlg : public CDialog
{
	DECLARE_DYNAMIC(CSofaNoFlashDlg)

public:
	CSofaNoFlashDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSofaNoFlashDlg();

// 对话框数据
	enum { IDD = IDD_SOFANOFLASH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
