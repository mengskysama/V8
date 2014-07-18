// VeritySliderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "VeritySliderDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\ui\UIHelper.h"

// CVeritySliderDlg 对话框

IMPLEMENT_DYNAMIC(CVeritySliderDlg, CDialog)

CVeritySliderDlg::CVeritySliderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVeritySliderDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CVeritySliderDlg::~CVeritySliderDlg()
{
}

void CVeritySliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERITY_SLIDER, m_VeritySlider);
}


BEGIN_MESSAGE_MAP(CVeritySliderDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_SKIN_MESSAGE
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()


// CVeritySliderDlg 消息处理程序

int CVeritySliderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"VeritySliderDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

BOOL CVeritySliderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_VeritySlider.ModifyStyle(0, TBS_REVERSED);
	m_VeritySlider.CenterWindow(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CVeritySliderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TRACKMOUSEEVENT tme ;
	tme.cbSize = sizeof( tme ) ;
	tme.dwFlags = TME_LEAVE ;
	tme.dwHoverTime = 1 ;
	tme.hwndTrack = m_hWnd ;
	::TrackMouseEvent(&tme) ;

	CDialog::OnMouseMove(nFlags, point);
}


LRESULT CVeritySliderDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{

	default:break;
	}

	return TRUE;
}

HRESULT CVeritySliderDlg::OnMouseLeave( WPARAM wParam , LPARAM lParam )
{
	common::ui::CCursorPos pt;
	CWnd* pWnd = WindowFromPoint(pt);//鼠标所在窗口
	if (pWnd != (&m_VeritySlider))
	{
		ShowWindow(SW_HIDE);
	}
	
	return S_OK;
}

void CVeritySliderDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	CSliderCtrl* pSlide = (CSliderCtrl*)pScrollBar;
	UINT uId = pScrollBar->GetDlgCtrlID();
	if (uId == IDC_VERITY_SLIDER)
	{
		if (m_pParentWnd != NULL && m_pParentWnd->GetSafeHwnd() != NULL)
		{
			::PostMessage(m_pParentWnd->GetSafeHwnd(), WM_ONVSCROLL_VERTI_SLIDER, (WPARAM)this, 0);
		}	
		m_VeritySlider.Invalidate(FALSE);
	}
}

void CVeritySliderDlg::SetRange( int iMin, int iMax, BOOL bRedraw /*= TRUE*/ )
{
	m_VeritySlider.SetRange(-iMax, -iMin, bRedraw);//设置为负数，取值时取绝对值，让滑动条有上大下小的效果
}

void CVeritySliderDlg::SetPageSize( int iSize )
{
	m_VeritySlider.SetPageSize(iSize);
}

void CVeritySliderDlg::SetPos( int nPos )
{
	int nMax = m_VeritySlider.GetRangeMax();
	int realPos = m_VeritySlider.GetRangeMax() - nPos;
	m_VeritySlider.SetPos(realPos);
}

int CVeritySliderDlg::GetPos()
{
	int realPos = m_VeritySlider.GetRangeMax() - m_VeritySlider.GetPos();
	return realPos;
}