// HeadSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../Hall.h"
#include "HeadSettingDlg.h"
#include "skin_i.h"
#include "SkinUtil.h"
#include "PicTracker.h"
#include "../CommonLibrary/include/utility/SystemHelper.h"
#include "../CommonLibrary/include/utility/FileHelper.h"
#include "../CommonLibrary/thirdlibrary/CxImage/include/ximage.h"
#include "webdefine.h"
#include "../CommonLibrary/include/ui/C179MsgBox.h"
#include "winnt.h"
#include "WinBase.h"
#include "time.h"
#include "../HallWindow/HallDlg.h"

#define SKIN_ON_CLICK_CLOSE                       0
#define SKIN_ON_CLICK_BTN_UPLOAD_LOCAL            1
#define SKIN_ON_CLICK_BTN_CONFIRM                 2

#define HEAD_SETTING_DLG_WIDTH                    400
#define HEAD_SETTING_DLG_HEIGHT                   300

// CHeadSettingDlg 对话框

IMPLEMENT_DYNAMIC(CHeadSettingDlg, CDialog)

CHeadSettingDlg::CHeadSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeadSettingDlg::IDD, pParent)
{
	m_picTracker = NULL;
    m_pSkinEngine = NULL;
	::GetSkinEngine(&m_pSkinEngine);
}

CHeadSettingDlg::~CHeadSettingDlg()
{
	if (m_picTracker)
	{
		delete m_picTracker;
		m_picTracker = NULL;
	}
}

void CHeadSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHeadSettingDlg, CDialog)
	ON_WM_CREATE()
	ON_SKIN_MESSAGE
END_MESSAGE_MAP()


// CHeadSettingDlg 消息处理程序

int CHeadSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//LoadSkin
	ASSERT(NULL != m_pSkinEngine);
	if (NULL != m_pSkinEngine)
	{
		HRESULT hr = m_pSkinEngine->LoadSkin(GetSafeHwnd(),L"HeadSettingDlg");
		ASSERT(hr== S_OK && __LINE__);
	}

	this->SetWindowPos(NULL, 0,0,HEAD_SETTING_DLG_WIDTH,HEAD_SETTING_DLG_HEIGHT,SWP_SHOWWINDOW);
    this->CenterWindow();
	//标题字体首选微软雅黑
	std::wstring strFont;
	common::utility::systemhelper::GetDefaultFontName(strFont);
	CComVariant var = (CComVariant) strFont.c_str();
	m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(),L"textTitle",L"fontFamily",var);

	core::CurInfo m_MyCurInfo = GetCurInfo();
	m_strHeadImage = g_HallModule->GetLoaclHeadImage(m_MyCurInfo.unUIN,m_MyCurInfo.bMan);
	SetHeaderImage(m_strHeadImage);
	m_upload = false;

	return 0;
}

LRESULT CHeadSettingDlg::OnSkinMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SKIN_ON_CLICK_CLOSE:
		{
			OnClose();//关闭与取消
		}
		break;
	case SKIN_ON_CLICK_BTN_UPLOAD_LOCAL:
		{
			OnClickBtnUploadLocal();//点击本地上传
		}
		break;
	case SKIN_ON_CLICK_BTN_CONFIRM:
		{
			OnClickBtnConfirm();//点击确定
		}
		break;
	default:break;
	}

	return TRUE;
}

void CHeadSettingDlg::OnClose()
{
	ShowWindow(SW_HIDE);	
}

BOOL CHeadSettingDlg::PreTranslateMessage(MSG* pMsg)
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

void CHeadSettingDlg::OnClickBtnUploadLocal()
{
	
	CString strFilePath, strPicPath;
	TCHAR szPath[255];  

	SHGetSpecialFolderPath(0, szPath, CSIDL_PERSONAL, 0);
	strPicPath = szPath;
	strPicPath+= _T("\\My Pictures");
	if (IDOK == ShowOpenFileDlg2(strFilePath, strPicPath, NULL, NULL, NULL, _T("图像文件(*.bmp;*.gif;*.jpg;*.png)|*.bmp;*.gif;*.jpg;*.png||"), OFN_ENABLESIZING, NULL))
	{
		if (PopPicTracker(strFilePath))
		{
			SetHeaderImage(m_strHeadImage);
			m_upload = true;  //需要上传用户头像
		}	
	}
}

void CHeadSettingDlg::OnClickBtnConfirm()
{
	if(m_strHeadImage.IsEmpty())
	{
		C179MsgBox::Show(this,NewMsgBox::MSGBOX_OK_ONE_STRING,_T("请先选择要上传的头像"));
		return;
	}
	if(m_upload)
	{
		EventSendHeadUpload();
	}
	
	OnClose();
}

void CHeadSettingDlg::EventSendHeadUpload()
{
	webinterface::upload_head_image_event_req getuploadEventReq;
	getuploadEventReq.srcMId	=	MODULE_ID_HALL;
	getuploadEventReq.desMId	=	MODULE_ID_WEBSERVICE;
	getuploadEventReq.wstrImagePath = m_strHeadImage;
	getuploadEventReq.wstrUrl = _T("http://interface.179.com/user/userThumb");


	IModuleManager* pModuleManager = g_HallModule->m_pModuleManager;
	if (pModuleManager != NULL)
	{
		nHeadSeqNo = pModuleManager->CallService(getuploadEventReq.eventValue,(param)&getuploadEventReq);

	}	
}

//上传自定义头像请求返回响应函数
void CHeadSettingDlg::OnEventRespHeadUpload(Event const& e)
{
	webinterface::upload_head_image_event_resp* pUploadEventResp =(webinterface::upload_head_image_event_resp* ) (e.m_pstExtraInfo);
	if(nHeadSeqNo != e.param2) 
	{
		return ;
	}

	if( e.param0 == webinterface::WEB_RET_SUCCESS)
	{	
		ModifyHeadImageTime(pUploadEventResp->uploadTime);  //修改头像文件时间属性		
		
		CString appPath = common::utility::systemhelper::Get179AppPath().c_str();
		core::CurInfo m_MyCurInfo = GetCurInfo();
		CString headName,headUpload;
		headUpload.Format(_T("%s%s%dUpload.jpg"),appPath,_T("resource\\head\\"),m_MyCurInfo.unUIN);//当前用户头像上传路径		
		headName.Format(_T("%s%s%d.jpg"),appPath,_T("resource\\head\\"),m_MyCurInfo.unUIN);//当前用户头像路径	
		CopyFile(headUpload,headName,FALSE); //拷贝文件， 若目标文件存在则覆盖，(当目标文件存在且为只读或隐藏时覆盖失败，否则覆盖成功)
		IModuleManager * pModuleManager = g_HallModule->m_pModuleManager;	
		if (pModuleManager != NULL)
		{
			pModuleManager->PushMessage(MakeMessage<MODULE_ID_HALL>()(Hall::MESSAGE_VALUE_HALL_HEAD_IMAGE_UPDATE,(param)m_MyCurInfo.unUIN , 0, 0, 0));
		}

		C179MsgBox::Show(this,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING,NewMsgBox::ICONTYPE_BINGO, _T("头像上传成功              "));

	}
	else
	{
		C179MsgBox::Show(this,NewMsgBox::MSGBOX_ICO_OK_ONE_STRING,NewMsgBox::ICONTYPE_ERROR, _T("头像上传失败              "));
		
	}	

}

//获取用户头像路径
CString CHeadSettingDlg::GetHeadImagePath()
{
	CString appPath = common::utility::systemhelper::Get179AppPath().c_str();
	core::CurInfo m_MyCurInfo = GetCurInfo();
	CString imagePath;
	imagePath.Format(_T("%s%s%dUpload.jpg"),appPath,_T("resource\\head\\"),m_MyCurInfo.unUIN);//当前用户头像路径

	return imagePath;

}


//修改头像文件时间
void CHeadSettingDlg::ModifyHeadImageTime(time_t imageTime) 
{	

	HANDLE hFile = CreateFile(m_strHeadImage, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	else
	{		
		
		FILETIME lastWriteTime;
		common::utility::filehelper::UinxTimeToFileTime(imageTime,&lastWriteTime);
		SetFileTime(hFile,&lastWriteTime,&lastWriteTime,&lastWriteTime);

		CloseHandle(hFile);		
		
	}
	
}

int CHeadSettingDlg::ShowOpenFileDlg2(CString& sFilePath,
						  CString  sInitPath,
						  PBOOL    pbReadOnly/* = NULL*/, 
						  LPCTSTR  lpszDefExt/* = NULL*/,
						  LPCTSTR  lpszFileName/* = NULL*/,
						  LPCTSTR  lpszFilter, // = NULL"图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
						  DWORD    dwFlags/* = OFN_ENABLESIZING*/,
						  CWnd*    pParentWnd/* = NULL*/)
{
	sFilePath.Empty();
	if (pbReadOnly){ *pbReadOnly = FALSE; }
	int nRet = IDCANCEL;

	CFileDialog dlg(TRUE, lpszDefExt, lpszFileName, dwFlags | OFN_ENABLESIZING, lpszFilter, pParentWnd);
	dlg.m_ofn.lpstrInitialDir = sInitPath; 
	if ((nRet = dlg.DoModal()) == IDOK)
	{
		sFilePath = dlg.GetPathName();
		if (pbReadOnly)
		{
			*pbReadOnly = dlg.GetReadOnlyPref();//返回只读复选框是否被选择
		}
	}

	return nRet;
}

BOOL CHeadSettingDlg::PopPicTracker(CString strImgPath)
{
	if (m_picTracker == NULL)
	{
		CxImage img;
		if(!img.LoadW(strImgPath))
		{
			C179MsgBox::Show(this,NewMsgBox::MSGBOX_OK_ONE_STRING,_T("装载图片失败"));	
			return false;
		}
		/*if (img.GetHeight() <= 110 && img.GetWidth()<=110 &&img.GetType() == CXIMAGE_FORMAT_GIF)  //GIF图在高110宽110之内的不需要裁剪，直接使用
		{
			m_strHeadImage = strImgPath;
			return true;		
			
		}*/
		SwitchResourceToModule(L"Hall");
		CPicTracker pPicTracker(this);
		CString savePath = common::utility::systemhelper::Get179AppPath().c_str();
		pPicTracker.SetPath(strImgPath);
		pPicTracker.SetDir(savePath);
		if(pPicTracker.DoModal() == IDOK)
		{
			m_strHeadImage = pPicTracker.GetPath();
			m_picTracker == NULL;//确保下次再打开能进来， 因为本函数开头是用NULL来判断的
		}
		else
		{
			return false;
		}

		return true;
	}

	return TRUE;
}

BOOL CHeadSettingDlg::SetHeaderImage(CString strImagePath)
{
	LONG lValue = (LONG)((IUnknown*)NULL);
	CComVariant varValue = lValue;
	HRESULT hr = m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgHead", L"externalImage", varValue);

	varValue = strImagePath;
	DISPPARAMS dispParam = {0, NULL, 0, 0};
	m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(), L"imgHead", L"Stop", NULL, &dispParam);
	hr = m_pSkinEngine->SetSubSkinProp(GetSafeHwnd(), L"imgHead", L"absoluteImage", varValue);
	m_pSkinEngine->CallSubSkinMethod(GetSafeHwnd(), L"imgHead", L"Play", NULL, &dispParam);

	return TRUE;
}
