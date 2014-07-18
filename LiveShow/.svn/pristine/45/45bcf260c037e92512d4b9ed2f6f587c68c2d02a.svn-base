#pragma once
#include "51ktv.h"
#include "resource.h"
#include "searchedit.h"
#include "Skin_i.h"
#include "../../include/ui/mlhtmlview.h"
#include "ExpatHallTree.h"

class  CSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchDlg();

// Dialog Data
	enum { IDD = IDD_SEARCH_DLG };	
	
public:		
	void OnEventRespSearch(Event const& evt);//搜索请求返回响应函数
	bool IsSearchKeyOK();   //搜索关键字是否正确
	void ClearSearchKey();  //清除搜索关键字
	
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void OnMove(int x, int y);
	void OnLogOff();

	void OnSearchClick();
	void EventResSearch(CString searchKey);  //发送搜索请求
	void ParserSearchTree(std::string searchResp); //解释搜索结果以生成搜索目录树
	void ReleaseSearcTree(std::list<hallNode*> &RoomNode);  //释放搜索目录树数据

	DECLARE_MESSAGE_MAP()	

private:
	CString m_strEdit;
	CSearchEdit m_editSearch;
    CBrush m_EditBrush;
	CComPtr<ISkinEngine> m_pSkinEngine;
	int32 m_nSeqNo;
	CString m_pngNormal;
	CString m_pngSelect;
	bool m_searchKeyOK;  //关键字是否正确

public:
	std::list<hallNode*> m_SearchNode;  //搜索目录树数据	
	
};
