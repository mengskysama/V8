// SelfDefSentenceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Room.h"
#include "SelfDefSentenceDlg.h"
#include "Skin_i.h"
#include "SkinUtil.h"
#include "SelfDefInputDlg.h"
#include "..\GlobalManager.h"
#include "..\..\CommonLibrary\include\utility\SystemHelper.h"


#define  SKIN_ON_CLICK_CLOSE          1
#define  SKIN_ON_CLICK_BTN_SAVE       2
#define  SKIN_ON_CLICK_BTN_ADD        3
#define  SKIN_ON_CLICK_BTN_DELETE     4
// CSelfDefSentenceDlg 对话框

IMPLEMENT_DYNAMIC(CSelfDefSentenceDlg, CDialog)

CSelfDefSentenceDlg::CSelfDefSentenceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelfDefSentenceDlg::IDD, pParent)
{
    m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CSelfDefSentenceDlg::~CSelfDefSentenceDlg()
{
}

void CSelfDefSentenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELFDEF_SENTENCE, m_SentenceListCtrl);
}


BEGIN_MESSAGE_MAP(CSelfDefSentenceDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SELFDEF_SENTENCE, &CSelfDefSentenceDlg::OnLvnItemchangedListSelfdefSentence)
	ON_NOTIFY(NM_HOVER, IDC_LIST_SELFDEF_SENTENCE, &CSelfDefSentenceDlg::OnNMHoverListSelfdefSentence)
END_MESSAGE_MAP()


// CSelfDefSentenceDlg 消息处理程序

int CSelfDefSentenceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"SelfDefSentenceDlg");
		ASSERT(hr == S_OK && __LINE__);
	}

	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	return 0;
}

BOOL CSelfDefSentenceDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CSelfDefSentenceDlg::OnSkinMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
           PostMessage(WM_CLOSE,0,0);
		}
		break;
	case SKIN_ON_CLICK_BTN_SAVE:
		{ //保存
			ShortCutMsgMgr->UpdateShortCutMsgList( m_vShortCutMsgList ) ;
		}
		break;
	case SKIN_ON_CLICK_BTN_ADD:
		{ //添加
			SwitchResourceToModule(_T("Room"));
		   CSelfDefInputDlg pSelfDefInputDlg;
		   if ( pSelfDefInputDlg.DoModal() == IDOK )
		   {
			   CString strTemp = pSelfDefInputDlg.GetInPutString() ;
			   m_SentenceListCtrl.InsertItem( m_SentenceListCtrl.GetItemCount() , strTemp ) ;
			   m_vShortCutMsgList.push_back( strTemp.GetBuffer(0) ) ;
		   }
		}
		break;
	case SKIN_ON_CLICK_BTN_DELETE:
		{ //删除
			POSITION pos = NULL ;
			vector<int> vSelectedItem ;
			vector<int>::reverse_iterator iter ;
			pos = m_SentenceListCtrl.GetFirstSelectedItemPosition() ;
			while( pos != NULL )
			{
				int nItem = m_SentenceListCtrl.GetNextSelectedItem(pos) ;
				vSelectedItem.push_back( nItem ) ;
			}
			for ( iter = vSelectedItem.rbegin() ; iter != vSelectedItem.rend() ; ++iter )
			{
				m_SentenceListCtrl.DeleteItem( *iter ) ;
				if ( *iter < m_vShortCutMsgList.size() )
				{
					vector<wstring>::iterator shortcut_iter = m_vShortCutMsgList.begin();
					int npos= *iter ;
					advance(  shortcut_iter ,(*iter) ) ;
					m_vShortCutMsgList.erase( shortcut_iter ) ;
				}
			}
		}
		break;
	default:break;
	}

	return TRUE;
}

void CSelfDefSentenceDlg::InitSentenceList()
{
	DWORD dwStyle = 0;
	LONG lStyle; 
	lStyle = GetWindowLong(m_SentenceListCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle = lStyle | LVS_REPORT; //设置style 
	SetWindowLong(m_SentenceListCtrl.m_hWnd, GWL_STYLE, lStyle);
	dwStyle = m_SentenceListCtrl.GetExtendedStyle();
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES; 
	m_SentenceListCtrl.SetExtendedStyle(dwStyle); //设置扩展风格
	m_SentenceListCtrl.SetBkColor(RGB(255,255,255));
	m_SentenceListCtrl.SetTextBkColor(RGB(255,255,255));
	/*LVCOLUMN lvcHeader;
	lvcHeader.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvcHeader.fmt = LVCFMT_CENTER;
	lvcHeader.cx = 320;
	lvcHeader.pszText = _T("内容");*/
	//m_SentenceListCtrl.InsertColumn(0,&lvcHeader);
    
	//标题无法居中显示，先用空格填充(好土的办法，呃。。)，后期需优化
	CString strTitle = _T(" ");
	for (int i = 0; i < 48; ++i)//48个空格刚好
	{
		strTitle.Append(_T(" "));
	}
	strTitle.Append(_T("内容"));
	m_SentenceListCtrl.InsertColumn(0,strTitle,LVCFMT_CENTER,320);
   
	//设置第一列居中
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT;
	m_SentenceListCtrl.GetColumn(0,&lvc);
	lvc.fmt &= ~LVCFMT_JUSTIFYMASK;
	lvc.fmt |= LVCFMT_CENTER;
	m_SentenceListCtrl.SetColumn(0,&lvc);

	m_vShortCutMsgList = ShortCutMsgMgr->GetShortCutMsgList() ;
	vector<wstring>::iterator iter ;
	for ( iter = m_vShortCutMsgList.begin() ; iter != m_vShortCutMsgList.end() ; ++iter )
	{
		 m_SentenceListCtrl.InsertItem( m_SentenceListCtrl.GetItemCount() , iter->c_str() ) ;
	}
}

BOOL CSelfDefSentenceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSentenceList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelfDefSentenceDlg::OnLvnItemchangedListSelfdefSentence(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CSelfDefSentenceDlg::OnNMHoverListSelfdefSentence(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}


