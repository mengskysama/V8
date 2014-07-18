#pragma once
#include <deque>
using namespace std;
#include "..\GlobalRoomDefine.h"
#include "..\..\CommonLibrary\include\ui\ImageEx.h"
#include "..\usermanager\useritem.h"

class CRoomParentDlg ;
class CFlyLuxuryWnd : public CWnd
{
	DECLARE_DYNAMIC(CFlyLuxuryWnd)

public:
	CFlyLuxuryWnd( CRoomParentDlg* pParentDlg );
	virtual ~CFlyLuxuryWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Measure( PaodaoInfo& paodaoinfo , bool bInit ) ;
	void InvalidateCtrl();
	void DoScrollDraw( CDC *pDC ) ;
	void Draw( Graphics& graphics  , PaodaoInfo& PaodaoInfo , float fPosX ) ;
	void DrawWithoutGift( Graphics& graphics  , PaodaoInfo& PaodaoInfo , float& fReturnValue ) ;
	void InsertPaoDaoText(CString& strSrcName , CString& strDstName , uint32 unSrc179ID , 
		uint32 unDst179ID , uint16 unGiftID ,uint16 unCount ) ;
	bool IsFinish() { return m_bFinish; }

private:
	int m_nScrollPos ;
	int m_nOldWidth;
	common::ui::ImageEx *m_pFlyLuxuryImg ;
	common::ui::ImageEx *m_pGiftImg;

	BLENDFUNCTION m_blend;

	CRect m_rectClient ;
	int m_nTextSize ;
	int m_nPicSize ;
	int m_nLastScrollSpeed ;
	int m_nScrollSpeed ;
	int m_nSrcNamePos ;
	PaodaoInfo m_PaodaoInfo;
	std::wstring m_strFont;
	CRoomParentDlg* m_pParentDlg ;
	bool m_bFinish;

	std::vector<HDC> m_vecHDC;
	bool m_bInit;
private:
	float MeasureImage( uint16 unCount ) ;
	float MeasureImage( common::ui::ImageEx * pImage ) ;
	float MeasureGiftName( Graphics& graphics , CString& strTemp ) ;
	float MeasureSrcName( Graphics& graphics , CString& strTemp ) ;
	float MeasureSrcUUID( Graphics& graphics , CString& strTemp ) ;
	float MeasureSendTo( Graphics& graphics , CString strTemp ) ;
	float MeasureDstName( Graphics& graphics , CString& strTemp ) ;
	float MeasureDstUUID( Graphics& graphics , CString& strTemp ) ;
	float MeasureCount( Graphics& graphics , uint16 unCount , CString& strGiftUnit) ;
	float MeasureTime( Graphics& graphics , CString& strTemp ) ;

	float DrawImage( Graphics& graphics ,uint16 unCount , float fPosX ) ;
	float DrawImage( Graphics& graphics ,common::ui::ImageEx * pImage , float fPosX ) ;
	float DrawGiftName( Graphics& graphics ,CString& strTemp , float fPosX ) ;
	float DrawSrcName( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawSrcUUID( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawSendTo(  Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawDstName( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawDstUUID( Graphics& graphics , CString& strTemp , float fPosX ) ;
	float DrawCount( Graphics& graphics , uint16 unCount , CString& strGiftUnit , float fPosX ) ;
	float DrawTime( Graphics& graphics , CString& strTemp , float fPosX ) ;
};