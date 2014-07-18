// PicTracker.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "PicTracker.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\utility\SystemHelper.h"

#define SKIN_ON_CLICK_CLOSE            0
#define SKIN_ON_CLICK_ZOOMIN           1
#define SKIN_ON_CLICK_ZOOMOUT          2
#define SKIN_ON_CLICK_FULLSCREEN       3
#define SKIN_ON_CLICK_ORIGINAL         4
#define SKIN_ON_CLICK_BROWSER          5
#define SKIN_ON_CLICK_OK               6
#define SKIN_ON_CLICK_CANCEL           7

#define TRACKER_CTRL_LEFT              15
#define TRACKER_CTRL_TOP			   68
#define TRACKER_CTRL_WIDTH			   365
#define TRACKER_CTRL_HEIGHT			   239

#define PIC_SHOW_CTRL_LEFT			   409
#define PIC_SHOW_CTRL_TOP			   180//66
#define PIC_SHOW_CTRL_WIDTH			   100
#define PIC_SHOW_CTRL_HEIGHT		   100	
#define PIC_SHOW_CTRL_SMALL_LEFT	   416//409
#define PIC_SHOW_CTRL_SMALL_TOP		   66//240
#define PIC_SHOW_CTRL_SMALL_WIDTH	   85
#define PIC_SHOW_CTRL_SMALL_HEIGHT	   85	

// CPicTracker 对话框

IMPLEMENT_DYNAMIC(CPicTracker, CDialog)

CPicTracker::CPicTracker(CWnd* pParent /*=NULL*/)
	: CDialog(CPicTracker::IDD, pParent)
{
	m_strPath = _T("");
	::GetSkinEngine(&m_pSkinEngine);
}

CPicTracker::~CPicTracker()
{
}

void CPicTracker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACKER_SLIDER, m_SliderZoom);
}


BEGIN_MESSAGE_MAP(CPicTracker, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPicTracker 消息处理程序

int CPicTracker::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"PicTracker");
		ASSERT(result == S_OK && __LINE__);
	}
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

BOOL CPicTracker::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化			
	m_PicCtrlBig.Create(this,CRect(CPoint(TRACKER_CTRL_LEFT, TRACKER_CTRL_TOP), CSize(TRACKER_CTRL_WIDTH, TRACKER_CTRL_HEIGHT)),11111);
	m_picShowCtrl.Create(this,CRect(CPoint(PIC_SHOW_CTRL_LEFT,PIC_SHOW_CTRL_TOP),CSize(PIC_SHOW_CTRL_WIDTH,PIC_SHOW_CTRL_HEIGHT)),11112);
	m_picShowCtrl2.Create(this,CRect(CPoint(PIC_SHOW_CTRL_SMALL_LEFT,PIC_SHOW_CTRL_SMALL_TOP),CSize(PIC_SHOW_CTRL_SMALL_WIDTH,PIC_SHOW_CTRL_SMALL_HEIGHT)),11113);
	if(!m_PicImg.LoadW(m_strPath.GetBuffer(0)))
	{
		::MessageBox(m_hWnd,_T("装载图片失败"),_T("提示"),MB_OK);
		return FALSE;
	}

	m_PicCtrlBig.SetBitmap(m_PicImg.MakeBitmap());
	m_SliderZoom.SetRange(1,100);
	m_SliderZoom.SetPageSize(1);//按下PgUp或PgDown时的移动量
	
	UpdateSliderPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPicTracker::UpdateSliderPos()
{
	float factor = m_PicCtrlBig.GetZoomFactor();
	if (factor <= MIN_ZOOM_FACTOR)
	{
		m_SliderZoom.SetPos(0);
	}
	else
	{
		m_SliderZoom.SetPos(50*factor);
	}
	GetDlgItem(IDC_TRACKER_SLIDER)->Invalidate(FALSE);
}

//缩小
void CPicTracker::OnClickBtnZoomOut()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PicCtrlBig.OnZoomOut();
	m_SliderZoom.SetPos(m_SliderZoom.GetPos()-1);
	GetDlgItem(IDC_TRACKER_SLIDER)->Invalidate(FALSE);
	//UpdateSliderPos();
}

//放大
void CPicTracker::OnClickBtnZoomIn()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PicCtrlBig.OnZoomIn();
	//UpdateSliderPos();
	m_SliderZoom.SetPos(m_SliderZoom.GetPos()+1);
	GetDlgItem(IDC_TRACKER_SLIDER)->Invalidate(FALSE);
}


//原始大小
void CPicTracker::OnClickBtnOrigin()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PicCtrlBig.OnOriginal();
	UpdateSliderPos();
	
}


//最适应
void CPicTracker::OnClickBtnFit()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PicCtrlBig.OnFullScreen();
	UpdateSliderPos();
}


//浏览
void CPicTracker::OnClickBtnBrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	static const TCHAR szFilter[] = _T("图片文件(*.jpg;*.bmp;*.png;*.gif)|*.jpg;*.bmp;*.png;*.gif||");
	CFileDialog fileDlg(
		TRUE, NULL, NULL,
		OFN_HIDEREADONLY |  OFN_PATHMUSTEXIST,
		szFilter, this );

	CString strFile;
	if( fileDlg.DoModal() == IDOK )
	{
		strFile = fileDlg.GetPathName();
		m_PicImg.LoadW(strFile.GetBuffer(0));
		m_PicCtrlBig.SetBitmap(m_PicImg.MakeBitmap());
		UpdateSliderPos();
		Invalidate(TRUE);
	}
}

//确定
void CPicTracker::OnClickBtnCorfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	core::CurInfo m_MyCurInfo = GetCurInfo();
	CString strPath;
	strPath.Format(_T("%s%s%dUpload.jpg"),m_strSaveDir,_T("resource\\head\\"),m_MyCurInfo.unUIN);//裁剪后的图像保存路径及命名
	if(m_picShowCtrl.SaveFile(strPath))
	{
		m_strPath = strPath;
	}
	this->OnOK();
}

//取消
void CPicTracker::OnClickBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnCancel();
}

void CPicTracker::SetPath(CString strPath)
{
	m_strPath = strPath;
}

void CPicTracker::SetDir(CString strDir)
{
	m_strSaveDir = strDir;
}

CString CPicTracker::GetPath()
{
	return m_strPath;
}

LRESULT  CPicTracker::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			OnClickBtnClose();
		}
		break;
	case   SKIN_ON_CLICK_ZOOMIN:
		{
			OnClickBtnZoomIn();
		}
		break;
	case SKIN_ON_CLICK_ZOOMOUT:
		{
			OnClickBtnZoomOut();
		}
		break;
	case SKIN_ON_CLICK_FULLSCREEN:
		{
			OnClickBtnFit();
		}
		break;
	case SKIN_ON_CLICK_ORIGINAL:
		{
			OnClickBtnOrigin();
		}
		break;
	case SKIN_ON_CLICK_BROWSER:
		{
			OnClickBtnBrowser();
		}
		break;
	case SKIN_ON_CLICK_OK:	
		{
			OnClickBtnCorfirm();
		}
		break;
	case SKIN_ON_CLICK_CANCEL:
		{
			OnClickBtnClose();
		}
		break;
	default:break;
	}

	return TRUE;
}


void CPicTracker::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl* pSlide = (CSliderCtrl*)pScrollBar;
	UINT uId = pScrollBar->GetDlgCtrlID();
	if (uId == IDC_TRACKER_SLIDER)
	{
		GetDlgItem(IDC_TRACKER_SLIDER)->Invalidate(FALSE);
		m_iSliderPos = pSlide->GetPos();
		m_PicCtrlBig.SetZoomFactor(m_iSliderPos);
		
	}
}