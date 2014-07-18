#pragma once
#include "..\Resource.h"
#include "..\GlobalRoomDefine.h"
// CPngDlg 对话框

class CRoomParentDlg ;
class CPngDlg : public CDialog
{
	DECLARE_DYNAMIC(CPngDlg)

public:
	CPngDlg( CRoomParentDlg* pParentDlg ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPngDlg();

// 对话框数据
	enum { IDD = IDD_PNG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Init( ENM_DRAW_TYPE nType ) ;
	bool IsFinish() { return m_bFinish; }
private:
	void TimerDrawFireWork() ; //绘制烟花
	void TimerDrawSalvo() ;	   //绘制礼炮
	void TimerDrawPlutus() ;	//绘制财神
	void DrawWhiteGround() ;	//画白色背景
	void Draw( std::wstring& strFilePath ) ;

	HINSTANCE m_hInst ;
	typedef BOOL (WINAPI* FUpdateLayeredWindow)(HWND,HDC,POINT*,SIZE *,HDC,POINT*,COLORREF,BLENDFUNCTION* ,DWORD);
	FUpdateLayeredWindow UpdateLayeredWindow ;
	BLENDFUNCTION m_blend ;
	int m_nPictureIndex ;
	int m_nCount ;
	CRoomParentDlg* m_pParentDlg ;
	bool m_bFinish;
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
