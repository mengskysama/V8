// SongMarkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "SongMarkDlg.h"
#include "..\RoomParentDlg.h"
#include "..\RoomRightDlg.h"
#include "..\SendEventManager.h"
#include "..\..\CommonLibrary\include\ui\C179MsgBox.h"
#include "SongMarkManager.h"
#include "..\RoomBottomLeftDlg.h"

#define FLASH_VERSION	0x90000
#define MARK_COUNTDOWN_TIMER        1
#define PER_COUNTDOWN_TIME          1 * 1000

#define  NOFITY_SKIN_SHOW_MARK             2
#define  SKIN_ON_CLICK_BTN_GETTICKET	   3
#define  SKIN_ON_CLICK_BTN_OK_IN_12        4
#define  SKIN_ON_CLICK_BTN_OK_IN_13        5
#define  SKIN_ON_CLICK_BTN_ORDERSONG       6
#define  SKIN_ON_CLICK_BTN_OK_IN_20        8

// CSongMarkDlg 对话框

IMPLEMENT_DYNAMIC(CSongMarkDlg, CDialog)

CSongMarkDlg::CSongMarkDlg(CRoomParentDlg* pParentDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CSongMarkDlg::IDD, pParent), m_unRoomID(0), m_unOrderID(0), m_pRoomParentDlg(pParentDlg)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);

	m_countDownTime = 0;
	m_nCurShow = 0;
}

CSongMarkDlg::~CSongMarkDlg()
{
}

void CSongMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MARK_CHECK_USE_ONE, m_checkUserOne);
}


BEGIN_MESSAGE_MAP(CSongMarkDlg, CDialog)
	ON_SKIN_MESSAGE
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_GOOD_MARK, &CSongMarkDlg::OnBnClickedRadioGoodMark)
	ON_BN_CLICKED(IDC_RADIO_BAD_MARK, &CSongMarkDlg::OnBnClickedRadioBadMark)
	ON_MESSAGE(COMMONLIB_UM_NONDOMODAL_MSGBOX_GOTOREGISTER, &CSongMarkDlg::OnClickMsgBoxGoToRegister)
END_MESSAGE_MAP()


// CSongMarkDlg 消息处理程序

int CSongMarkDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SongMarkDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	return 0;
}

BOOL CSongMarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSongMarkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CSongMarkDlg::SetSongNameAndActor( CString strSongName, CString actorName )
{
	CComVariant var = strSongName.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textSongName", L"text", var);
	var = actorName.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textActorName", L"text", var);
}

void CSongMarkDlg::Show1(CString str1, CString str2, int countDown, int leaveTicket)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 1;

	m_countDownTime = countDown;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	CString strTemp;
	strTemp.Format(L"%d秒", countDown);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownTime", L"text", var);
	strTemp.Format(L"%d张", leaveTicket);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textLeaveTicket", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)1);
}

void CSongMarkDlg::Show2(CString str1, CString str2, int countDown, int leaveTicket)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	Show1(str1, str2, countDown, leaveTicket);
	m_nCurShow = 2;
}

void CSongMarkDlg::Show3(CString str1, CString str2, CString str3, int countDown, int leaveTicket)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 3;
	m_countDownTime = countDown;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = str3.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textDownConPrompt", L"text", var);
	CString strTemp;
	strTemp.Format(L"%d秒", countDown);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownTime", L"text", var);
	strTemp.Format(L"%d张", leaveTicket);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textLeaveTicket", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)3);
}

void CSongMarkDlg::Show4(CString str, int countDown, int leaveTicket)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 4;
	m_countDownTime = countDown;
	CComVariant var = str.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);

	CString strTemp;
	strTemp.Format(L"%d秒", countDown);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownTime", L"text", var);
	strTemp.Format(L"%d张", leaveTicket);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textLeaveTicket", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)4);
}

void CSongMarkDlg::Show5(CString str1, CString str2, int countDown, int leaveTicket)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 5;

	m_countDownTime = countDown;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	CString strTemp;
	strTemp.Format(L"%d秒", countDown);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownTime", L"text", var);
	strTemp.Format(L"%d张", leaveTicket);
	var = strTemp.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textLeaveTicket", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)5);
}

void CSongMarkDlg::Show6(CString str1, CString str2, CString str3)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 6;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownPrompt", L"text", var);
	var = str3.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textLeaveTicketPrompt", L"text", var);
	
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)6);
}

void CSongMarkDlg::Show7(CString str1, CString str2)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 7;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownPrompt", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)7);
}

void CSongMarkDlg::Show8(CString str1, CString str2, CString str3)
{	
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 8;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = str3.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownPrompt", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)8);
}

void CSongMarkDlg::Show9(CString str1, CString str2)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 9;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownPrompt", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)9);
}

void CSongMarkDlg::Show10(CString str1, CString str2)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 10;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)10);
}

void CSongMarkDlg::Show11(CString str1, CString str2)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 11;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)11);
}

void CSongMarkDlg::Show12(CString str1, CString strTip, BOOL bCheck, int markTime, BOOL bGoodMark, CString strGoodNum, CString strBadNum)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = markTime;
	m_nCurShow = 12;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = strTip.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textSuperTicket", L"tip", var);
	m_checkUserOne.SetCheck(bCheck);
	CString str2;
	str2.Format(L"请在%d秒内评价", markTime);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon3", L"text", var);
	if (bGoodMark)
	{
		((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(FALSE);
	}else
	{
		((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(FALSE);
	}
	var = strGoodNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGoodMark", L"text", var);
	var = strBadNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textBadMark", L"text", var);
	
	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)12);
}

void CSongMarkDlg::Show13(int markTime, CString str2, CString strMark, BOOL bGoodMark)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = markTime;
	m_nCurShow = 13;
	CComVariant var;
	CString str1;
	str1.Format(L"节目满意吗？请在%d秒内评价", markTime);
	var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = strMark.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textMark2", L"text", var);
	if (bGoodMark)
	{
		((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(FALSE);
	}else
	{
		((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(FALSE);
	}

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)13);
}

void CSongMarkDlg::Show14(CString str1, int markTime, CString strMark, CString strGoodNum, CString strBadNum)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = markTime;
	m_nCurShow = 14;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	CString str2;
	str2.Format(L"最终结果将在%d秒内公布", markTime);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = strMark.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textMark2", L"text", var);
	var = strGoodNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGoodMark", L"text", var);
	var = strBadNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textBadMark", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)14);
}

void CSongMarkDlg::Show15(int markTime, CString str2, CString strMark, CString strGoodNum, CString strBadNum)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = markTime;
	m_nCurShow = 15;
	CComVariant var;
	CString str1;
	str1.Format(L"评委将在%d秒内完成评价", markTime);
	var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = strMark.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textMark2", L"text", var);
	var = strGoodNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGoodMark", L"text", var);
	var = strBadNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textBadMark", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)15);
}

void CSongMarkDlg::Show16(int markTime, CString strMark, CString strGoodNum, CString strBadNum)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = markTime;
	m_nCurShow = 16;
	CComVariant var;
	CString str1;
	str1.Format(L"评委们将在%d秒内完成评价", markTime);
	var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = strMark.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textMark2", L"text", var);
	var = strGoodNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGoodMark", L"text", var);
	var = strBadNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textBadMark", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)16);
}

void CSongMarkDlg::Show17(CString str1, CString str2, CString strMark, CString strGoodNum, CString strBadNum, CString strGiveUpNum)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 17;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = strMark.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textMark2", L"text", var);
	var = strGoodNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGoodMark", L"text", var);
	var = strBadNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textBadMark", L"text", var);
	var = strGiveUpNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGiveupMark", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)17);
}

void CSongMarkDlg::Show18(CString str1, CString str2)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 18;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);

	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)18);
}

void CSongMarkDlg::Show19(CString str1, CString str2, CString str3)
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = 0;
	m_nCurShow = 19;
	CComVariant var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
	var = str2.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
	var = str3.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownPrompt", L"text", var);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)19);
}

void CSongMarkDlg::Show20( int markTime, CString strTip, BOOL bGoodMark, CString strGoodNum, CString strBadNum )
{
	KillTimer(MARK_COUNTDOWN_TIMER);
	m_countDownTime = markTime;
	m_nCurShow = 20;
	CComVariant var;
	CString str1;
	str1.Format(L"请在%d秒内评价", markTime);
	var = str1.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon3", L"text", var);
	var = strTip.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textSuperTicket", L"tip", var);
	if (bGoodMark)
	{
		((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(FALSE);
	}else
	{
		((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(FALSE);
	}
	var = strGoodNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textGoodMark", L"text", var);
	var = strBadNum.GetBuffer(0);
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textBadMark", L"text", var);

	SetTimer(MARK_COUNTDOWN_TIMER, PER_COUNTDOWN_TIME, NULL);
	NOTIFY_SKIN_HELPER(NOFITY_SKIN_SHOW_MARK, (LONG)20);
}

void CSongMarkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case MARK_COUNTDOWN_TIMER:
		{
			if (--m_countDownTime >= 0)
			{   //很硬！
				CString strTemp;
				CComVariant var;
				if (m_nCurShow == 12)
				{
					strTemp.Format(L"请在%d秒内评价", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon3", L"text", var);
				}else if (m_nCurShow == 13)
				{		
					strTemp.Format(L"节目满意吗？请在%d秒内评价", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
				}else if (m_nCurShow == 14)
				{
					strTemp.Format(L"最终结果将在%d秒内公布", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon2", L"text", var);
				}else if (m_nCurShow == 15)
				{
					strTemp.Format(L"评委将在%d秒内完成评价", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
				}else if (m_nCurShow == 16)
				{
					strTemp.Format(L"评委们将在%d秒内完成评价", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon1", L"text", var);
				}else if (m_nCurShow == 20)
				{
					strTemp.Format(L"请在%d秒内评价", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textUpCon3", L"text", var);
				}else
				{
					strTemp.Format(L"%d秒", m_countDownTime);
					var = strTemp.GetBuffer(0);
					m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"textCountDownTime", L"text", var);
				}
			}else
			{
				KillTimer(MARK_COUNTDOWN_TIMER);
				m_countDownTime = 0;
			}
		}
		break;
	default:break;
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CSongMarkDlg::OnSkinMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_BTN_GETTICKET:
		 {
			OnClickGetTicket();
		 }
		 break;
	case SKIN_ON_CLICK_BTN_OK_IN_12:
		{
			OnClickBtnOKIn12();
		}
		break;
	case SKIN_ON_CLICK_BTN_OK_IN_13:
		{
			OnClickBtnOKIn13();
		}
		break;
	case SKIN_ON_CLICK_BTN_ORDERSONG:
		{
			OnClickBtnOrderSong();
		}
		break;
	case SKIN_ON_CLICK_BTN_OK_IN_20:
		{
			OnClickBtnOKIn20();
		}
		break;
    default: break;
	}

	return TRUE;
}

void CSongMarkDlg::OnBnClickedRadioGoodMark()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(FALSE);
}

void CSongMarkDlg::OnBnClickedRadioBadMark()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_BAD_MARK))->SetCheck(TRUE);
}


void CSongMarkDlg::OnClickGetTicket()
{
	if (GetCurInfo().enmAccountType == ACCOUNTTYPE_VISITOR)//游客
	{
		C179MsgBox::Show( this, NewMsgBox::MSGBOX_GUEST_OPERATE , L"游客不能抢票，" , L"想要畅玩白兔KTV？" , L"" , L"快去注册吧" , 
			m_pRoomParentDlg->GetRoomBusiness()->GetRegisteUrl().GetBuffer() ); 
		return;
	}

	if (m_unRoomID != 0 && m_unOrderID != 0)
	{
		CSendEventManager::SendMarkGetTicketEvent(m_unRoomID, m_unOrderID);
	}
}

void CSongMarkDlg::OnClickBtnOKIn12()
{
	if (m_checkUserOne.GetCheck())
	{
		if (((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->GetCheck())
		{
			CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 1, CSongMarkManager::GetUnitTicket());
		}
		else
		{
			CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 2, CSongMarkManager::GetUnitTicket());
		}
	}
	else
	{
		if (((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->GetCheck())
		{
			CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 1, 1);
		}
		else
		{
			CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 2, 1);
		}
	}
}

void CSongMarkDlg::OnClickBtnOKIn13()
{
	if (((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->GetCheck())
	{
		CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 1, 1);
	}
	else
	{
		CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 2, 1);
	}
}

void CSongMarkDlg::OnClickBtnOrderSong()
{
	//((CRoomRightDlg*)GetParent())->OnClickOrderSong();
	((CRoomBottomLeftDlg*)GetParent())->OnClickOrderSong();
}

void CSongMarkDlg::OnClickBtnOKIn20()
{
	if (((CButton*)GetDlgItem(IDC_RADIO_GOOD_MARK))->GetCheck())
	{
		CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 1, 1);
	}
	else
	{
		CSendEventManager::SendMarkSongEvent(m_unRoomID, m_unOrderID, 2, 1);
	}
}

void CSongMarkDlg::SetRoomID( uint32 unRoomID )
{
	m_unRoomID = unRoomID;
}

void CSongMarkDlg::SetOrderID( uint32 unOrderID )
{
	m_unOrderID = unOrderID;
}

LRESULT CSongMarkDlg::OnClickMsgBoxGoToRegister( WPARAM wParam, LPARAM lParam )
{
	if((CSongMarkDlg*)wParam == this)
	{
		m_pRoomParentDlg->OnClickGotoRegister();
	}

	return TRUE;
}