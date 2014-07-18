// MainDlg.cpp : implementation file
//

#include ".\stdafx.h"
#include "ErrorReport.h"
#include "MainDlg.h"
#include ".\maindlg.h"
#include "CrashReportSrc\crashrpt\src\Utility.h"
#include "CrashReportSrc\crashrpt\src\crashhandler.h"
#include "Resource.h"

extern REPORTOPTION g_ReportOption;
// CMainDlg dialog

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
	, m_sDescription(_T(""))
{
	m_fontHeader.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("宋体"));

	m_Icon = NULL;
	m_TitleIcon = NULL;
}

CMainDlg::~CMainDlg()
{
	m_fontHeader.DeleteObject();
	if ( m_Icon )
	{
		DeleteObject(m_Icon);
	}

	if(m_TitleIcon)
	{
		DeleteObject(m_TitleIcon);
	}
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DESCRIPTION, m_sDescription);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainDlg message handlers

void CMainDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(TRUE);
	EndDialog(IDOK);
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect rect,rectHeader;
	CFont * pOld=NULL;
	if(m_fontHeader.m_hObject)
	{
		pOld=dc.SelectObject(&m_fontHeader);
	}

	GetDlgItem(IDC_STATIC_HEADER)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetClientRect(rectHeader);
	rectHeader.bottom=rect.top;

	dc.FillSolidRect(&rectHeader,RGB(227,240,248));

	dc.SetTextColor(RGB(0,0,0));
	dc.SetBkMode(TRANSPARENT);

	rectHeader.DeflateRect(50,10,10,10);

	dc.DrawText(m_sText,&rectHeader,DT_LEFT|DT_WORDBREAK);

	if ( pOld )
	{
		dc.SelectObject(pOld);
	}
	
	if ( m_Icon!=NULL )
	{
		::DrawIconEx(dc.GetSafeHdc(),0,5,m_Icon,48,48,0,NULL,DI_NORMAL);
	}
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	if(g_ReportOption.strDlgTitle.IsEmpty())
		SetWindowText(CUtility::getAppName());
	else SetWindowText(g_ReportOption.strDlgTitle);

	m_sText = "baituKTV遇到未知错误需要关闭，我们对此引起的不便深表歉意。"
		"希望您选择发送此错误报告，以帮助我们改善白兔KTV的质量。";

	m_Icon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_CRY), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR);
	m_TitleIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_TITLE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	
	SetIcon(m_TitleIcon, FALSE);

	SetWindowText("白兔KTV错误报告");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}
