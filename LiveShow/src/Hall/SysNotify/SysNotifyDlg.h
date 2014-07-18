#pragma once

#include "Resource.h"
#include "Skin_i.h"
#include <map>
// CSysNotifyDlg 对话框

struct stSysMsg  //系统消息结构体，可根据需要修改
{
   CString strContent;//内容
   CString strHyperLinkLabel;//链接文字
   CString strHyperLingAddr;//链接地址
};

#define SYSNOTIFY_WIDTH 250   //系统消息对话框的宽度
#define SYSNOTIFY_HIGHT 170   //系统消息对话框的高度

typedef std::map<int,stSysMsg*> SYSMSGMAP;//key指第几条消息,从0开始，value就是消息内容
class CSysNotifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysNotifyDlg)

public:
	CSysNotifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysNotifyDlg();

// 对话框数据
	enum { IDD = IDD_SYSTEM_NOTIFY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	HRESULT OnMouseLeave( WPARAM wParam , LPARAM lParam );
	HRESULT OnNcMouseLeave( WPARAM wParam , LPARAM lParam );

	void ShowSysMsg();

	DECLARE_MESSAGE_MAP()

public:
	void SetSysMsg(SYSMSGMAP mapSysMsg ,LPCTSTR pTitle);//设置系统消息内容和链接,还有标题
	void ClearSysMes();  //清除所有消息
	BOOL IfMouseLeave();
    
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	SYSMSGMAP m_mapSysMsg;
	int m_curPage,m_totalPage;//当前页，总页
	RECT m_rect;  //系统消息对话框显示的位置
	BOOL m_bMouseLeave;
};
