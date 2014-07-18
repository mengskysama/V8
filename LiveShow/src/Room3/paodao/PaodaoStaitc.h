#pragma once
#include <deque>
using namespace std;
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\ui\ImageEx.h"
#include "..\usermanager\useritem.h"

class CPaodaoBtnDlg ;
class CRoomParentDlg ;
class CPaodaoStaitc : public CStatic
{
	DECLARE_DYNAMIC(CPaodaoStaitc)

public:
	CPaodaoStaitc( CRoomParentDlg* pParentDlg );
	virtual ~CPaodaoStaitc();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Measure( PaodaoInfo& paodaoinfo , bool bInit ) ;
	void InvalidateCtrl();
	void DoScrollDraw( CDC *pDC ) ;
	void Draw( Graphics& graphics  , PaodaoInfo& PaodaoInfo , float fPosX ) ;
	void DrawWithoutGift( Graphics& graphics  , PaodaoInfo& PaodaoInfo , float& fReturnValue ) ;
	void FirstDraw();
	void InsertPaoDaoText(CString& strSrcName , CString& strDstName ,CString& strGiftWord , uint32 unSrc179ID , 
		uint32 unDst179ID , uint16 unGiftID ,uint16 unCount , CString& strTime ) ;
	void PauseMove() ;
	void StartMove() ;

	void OnDeleteUser( IUserItem * pUserItem ) ;
	void OnClickNameLink( CString& strName , uint32 unUUID ) ;
	void OnRoomRightSendToChange( CString& strSelText ) ;
	void UpdateGiftList();
protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	HRESULT OnMouseLeave( WPARAM wParam , LPARAM lParam ) ;
private:
	int m_nScrollPos ;
	int m_nOldWidth;
	common::ui::ImageEx * m_pPic ;
	//common::ui::ImageEx * m_pBackImage;

	CDC     m_MainDC;
	CBitmap m_MainBitmap;

	CBitmap *m_pOldBitmap;
	CRect m_rectClient ;
	int m_nTextSize ;
	int m_nPicSize ;
	int m_nLastScrollSpeed ;
	int m_nScrollSpeed ;
	int m_nSrcNamePos ;
	deque<PaodaoInfo> m_vPaodaoInfo ;
	std::wstring m_strFont;
	CPaodaoBtnDlg* m_pPaodaoBtnDlg ;
	CRoomParentDlg* m_pParentDlg ;

	std::vector<HDC> m_vecHDC;
private:
	float MeasureImage( uint16 unCount ) ;
	float MeasureGiftName( Graphics& graphics , CString& strTemp ) ;
	float MeasureSrcName( Graphics& graphics , CString& strTemp ) ;
	float MeasureSrcUUID( Graphics& graphics , CString& strTemp ) ;
	float MeasureSendTo( Graphics& graphics , CString strTemp ) ;
	float MeasureDstName( Graphics& graphics , CString& strTemp ) ;
	float MeasureDstUUID( Graphics& graphics , CString& strTemp ) ;
	float MeasureCount( Graphics& graphics , uint16 unCount, CString& strGiftUnit ) ;
	float MeasureGiftSay(Graphics& graphics  ) ;
	float MeasureGiftWord( Graphics& graphics , CString& strTemp ) ;
	float MeasureTime( Graphics& graphics , CString& strTemp ) ;

	float DrawImage( Graphics& graphics ,uint16 unCount , float fPosX ) ;
	float DrawGiftName( Graphics& graphics ,CString& strTemp , float fPosX ) ;
	float DrawSrcName( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawSrcUUID( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawSendTo(  Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawDstName( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawDstUUID( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawCount( Graphics& graphics , uint16 unCount , CString& strGiftUnit , float fPosX ) ;
	float DrawGiftSay(Graphics& graphics , float fPosX ) ;
	float DrawGiftWord( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawTime( Graphics& graphics , CString& strTemp , float fPosX ) ;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};