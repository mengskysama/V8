
// VerifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VerifyDlg.h"
#include "SkinHeader.h"
#include "SkinUtil.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"

#define  SKIN_ON_CLICK_CLOSE   0
#define  SKIN_ON_CLICK_OK	   1
#define  SKIN_ON_CLICK_CANCEL   2
#define  SKIN_ON_CLICK_CHANGE_CODE 3

#define  SKIN_NOTIFY_VERIFY_CODE 0
#define  SKIN_NOTIFY_VERIFY_TEXT 1
#define  SKIN_NOTIFY_VERIFY_COLOR 2

#define  VERIFY_TIME_OUT   5*1000*60


// CVerifyDlg 对话框

IMPLEMENT_DYNAMIC(CVerifyDlg, CDialog)

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyDlg::IDD, pParent)
	,m_pVerifyHander(pParent->GetSafeHwnd())
{
	m_LastVerfiyTime = GetTickCount();
	::GetSkinEngine(&m_pSkinEngine);
}

CVerifyDlg::~CVerifyDlg()
{
}

void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CVerifyDlg 消息处理程序

int CVerifyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (m_pSkinEngine != NULL){
		HRESULT result = m_pSkinEngine->LoadSkin(GetSafeHwnd(), L"verify");
		ASSERT(result == S_OK && __LINE__);
	}
	return 0;
}

LRESULT  CVerifyDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		OnClickClose();
		break;
	case SKIN_ON_CLICK_OK:
		OnClickOk();
		break;
	case SKIN_ON_CLICK_CANCEL:
		OnClickCancel();
		break;
	case SKIN_ON_CLICK_CHANGE_CODE:
		OnClickRefreshCode();
	default:
		return 0;
	}
	return 0;
}

void CVerifyDlg::OnClickRefreshCode()
{
	//更新验证码
	m_pVerifyHander.RefreshCode();
}

void CVerifyDlg::OnClickOk()
{

	if (GetTickCount() - m_LastVerfiyTime >= VERIFY_TIME_OUT)
	{
		NotifySkinText(VERIFY_TEXT_EXPIRED);
		NotifySkinColor(0x0000FF);
		m_pVerifyHander.RefreshCode();
		return;
	}
	CString strCode;
	GetDlgItemText(IDC_EDIT_CODE,strCode);
	if (strCode.GetLength() == 0)
	{
		NotifySkinText(VERIFY_TEXT_EMPTY);
		NotifySkinColor(0x0000FF);
		return;
	}

	m_pVerifyHander.VerifyCode(strCode);
	NotifySkinText(VERIFY_TEXT_VERIFYING);
	NotifySkinColor(0x0000FF);
	//CDialog::OnOK();
}

void CVerifyDlg::OnOK()
{
	OnClickOk();
	return;
}

void CVerifyDlg::OnCancel()
{
	return;
}

void CVerifyDlg::NotifySkinCode(CString strCodePath)
{
	m_LastVerfiyTime = GetTickCount();
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_VERIFY_CODE, strCodePath.GetBuffer(0));
}

void CVerifyDlg::NotifySkinText(CString strText)
{
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_VERIFY_TEXT, strText.GetBuffer(0));
}

void CVerifyDlg::NotifySkinColor(LONG color)
{
	NOTIFY_SKIN_HELPER(SKIN_NOTIFY_VERIFY_COLOR, color);
}

void CVerifyDlg::OnCodeError(CString errorText)
{
	NotifySkinText(errorText);
	NotifySkinColor(0x0000FF);
	OnClickRefreshCode();
	SetDlgItemText(IDC_EDIT_CODE,_T(""));
}


void CVerifyDlg::OnCodeSuccess()
{
	CDialog::OnOK();
}

void CVerifyDlg::GetVerifyCode()
{
	m_pVerifyHander.GetVerifyCode();
}

//请求获取验证码图片返回响应函数
void CVerifyDlg::OnGetVerifyCodeRep(Event const& evt)       
{
	CString verifyPath = m_pVerifyHander.OnGetVerifyCodeRep(evt);
	if(!verifyPath.IsEmpty())
	{
		NotifySkinCode(verifyPath); //通知皮肤验证码,即验证码对话框加载并显示验证码图片
	}
}


void CVerifyDlg::OnClickCancel()
{
	CDialog::OnCancel();	
}

void CVerifyDlg::OnClickClose()
{
	CDialog::OnOK();
}
BOOL CVerifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//标题字体首选微软雅黑
	/*std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);*/

	GetVerifyCode();
	NotifySkinText(VERIFY_TEXT_DEFAULT);

	CEdit* editCode = (CEdit*)GetDlgItem(IDC_EDIT_CODE);
	editCode->LimitText(4);  //

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

