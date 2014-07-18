// PhotoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "PhotoDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE                1
#define  SKIN_ON_CLICK_BTN_PHOTODIR         2
#define  SKIN_ON_CLICK_BTN_PREVIEW          3
// CPhotoDlg 对话框


IMPLEMENT_DYNAMIC(CPhotoDlg, CDialog)

CPhotoDlg::CPhotoDlg(const CString& strDirPath ,const CString& strFilePath ,CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoDlg::IDD, pParent),
	m_strDirPath(strDirPath) ,
	m_strFilePath(strFilePath)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CPhotoDlg::~CPhotoDlg()
{
}

void CPhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPhotoDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CPhotoDlg 消息处理程序

int CPhotoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Load Skin
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"PhotoDlg");
		ASSERT(result == S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

LRESULT CPhotoDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{ 
    switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
           DestroyWindow();
		}
		break;
	case SKIN_ON_CLICK_BTN_PHOTODIR:
		{
           OnClickPhotoDir();
		}
		break;
	case SKIN_ON_CLICK_BTN_PREVIEW:
		{
           OnClickPreview();
		}
		break;
	default:break;
	}

	return TRUE;
}

void CPhotoDlg::OnClickPhotoDir()
{
	ShellExecute( NULL , _T("open") , m_strDirPath , NULL , NULL , SW_SHOWNORMAL ) ;
}

void CPhotoDlg::OnClickPreview()
{
	ShellExecute( NULL , _T("open") , m_strFilePath , NULL , m_strDirPath , SW_SHOWNORMAL ) ;
}
void CPhotoDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this ;
	// TODO: 在此处添加消息处理程序代码
}

BOOL CPhotoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽ESC按键和ENTER键
	if ( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:

			return TRUE ;
		default: break;
		}	
	}

	return CDialog::PreTranslateMessage(pMsg);
}
