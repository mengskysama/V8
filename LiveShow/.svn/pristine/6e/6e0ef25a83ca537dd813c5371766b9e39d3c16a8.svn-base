// MyGoodsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "MyGoodsDlg.h"
#include "SkinUtil.h"


// CMyGoodsDlg 对话框

IMPLEMENT_DYNAMIC(CMyGoodsDlg, CDialog)

CMyGoodsDlg::CMyGoodsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyGoodsDlg::IDD, pParent)
{
	m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CMyGoodsDlg::~CMyGoodsDlg()
{
}

void CMyGoodsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MYGOODS, m_MyGoodsListCtrl);
}


BEGIN_MESSAGE_MAP(CMyGoodsDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMyGoodsDlg 消息处理程序

int CMyGoodsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(m_pSkinEngine != NULL);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"MyGoodsDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	return 0;
}

BOOL CMyGoodsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_MyGoodsListCtrl.InitGiftListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
