#include "stdafx.h"
#include "HtmlEditLogic.h"
#include "afxadv.h"
#include "../CommonLibrary\thirdlibrary\CxImage\include\ximage.h"
#include "../CommonLibrary/include/utility/URLEncode.h"
#include "C51SkinMessageBox.h"
#include "..\CommonLibrary\include\utility\StringHelper.h"
#include "..\CommonLibrary\include\utility\Filehelper.h"
#include "../CommonLibrary/include/utility/systemhelper.h"
#include "../CommonLibrary/include/utility/md5.h"
using namespace common;

HtmlEditLogic::HtmlEditLogic(int iEditable)
{
	m_Setting.m_nEditable = iEditable;
	m_Setting.Construct();
}

HtmlEditLogic::~HtmlEditLogic()
{

}

void HtmlEditLogic::SetHtmlEditImpl(HtmlEditImpl* pHtmlEditImp)
{
	m_pHtmlEditImp = pHtmlEditImp;
}

//清空公聊窗口的内容
void HtmlEditLogic::ClearOutput() 
{
	if ( m_pHtmlEditImp )
	{
		m_pHtmlEditImp->ClearMsg() ;
	}
}

void HtmlEditLogic::OnSetFocus(HtmlEditImpl* pHtmlEditImp)
{

}

void HtmlEditLogic::OnSendMessage(HtmlEditImpl* pHtmlEditImp)
{

}

void HtmlEditLogic::OnOutputReady(HtmlEditImpl* pHtmlEditImp)
{

}

void HtmlEditLogic::OnCustomEmotion(HtmlEditImpl* pHtmlEditImp, LPCTSTR lpszEmotionPath)
{
	common::CURLEncode urlEncode;
	std::wstring strPath = urlEncode.URLEncodeLocalPath(std::wstring(lpszEmotionPath));

	CString str;
	str.Format(_T("%s%s%s"), _T("<img src=\""), strPath.c_str(), _T("\">"));

    if(pHtmlEditImp != NULL)
    {
        pHtmlEditImp->InsertAtSelectionArea(str);
    }
}

void HtmlEditLogic::OnEditCopy(HtmlEditImpl* pHtmlEditImp)
{
	CString strText ,strHtmlText ;
	pHtmlEditImp->OnEditCopy( strHtmlText , strText ) ;
	CopyDataToClipbord(strHtmlText,strText);
}

void HtmlEditLogic::OnEditSaveToNote(HtmlEditImpl* pHtmlEditImp)
{

}

void HtmlEditLogic::OnEditShowInputStatus()
{

}

void HtmlEditLogic::OnEditCut(HtmlEditImpl* pHtmlEditImp)
{
	CString strText,strHtmlText;

	if(pHtmlEditImp != NULL)
	{
		pHtmlEditImp->OnEditCut(strHtmlText,strText);
	}

	CopyDataToClipbord(strHtmlText,strText);
}

int HtmlEditLogic::OnEditPaste(HtmlEditImpl* pHtmlEditImp)
{
	CString strHtmlText;

	LONG type = GetDataFromClipboard(pHtmlEditImp, strHtmlText);

	if( ( type == CF_UNICODETEXT || type == CF_TEXT ) && strHtmlText.GetLength() > 0)
	{
		if(pHtmlEditImp != NULL)
		{
			int re = pHtmlEditImp->OnEditPaste(strHtmlText,type);
		}
	}

	return 0;
}

BOOL HtmlEditLogic::OnKeyDown(HtmlEditImpl* pHtmlEditImp, long key)
{
	if(pHtmlEditImp == NULL)
	{
		return TRUE;
	}
	if((GetKeyState(VK_CONTROL)&0x8000))
	{
		if ((key == 0x43) || (key == 0x63))//拷贝
		{
			OnEditCopyWord(pHtmlEditImp);

			return FALSE;
		}
	}


	if(key == 0x20)
	{
		CString content;
		pHtmlEditImp->GetContent(content);

		if(content == _T("<P>&nbsp;</P>"))
		{
			pHtmlEditImp->PutBodyInnerHTML(_T("&nbsp;"));

			return FALSE;
		}
	}

	//重新判断 Ctrl+Enter ||  Enter
	if(VK_RETURN == key)
	{
		OnSendMessage( pHtmlEditImp ) ;			//ENTER 就发送信息 lfp
		return FALSE ;
	}

	if((GetKeyState(VK_MENU)&0x8000))
	{
		if ((key == 0x53) || (key == 0x73))
		{
			//发送信息
			if(pHtmlEditImp->HasContent() == true)
			{
				OnSendMessage(pHtmlEditImp);
			}

			return FALSE;
		}
	}	

	if ( (key >= 0x21 && key <=0x2E) || key == 0x08 )
	{
		//0x21为PAGE UP ,0x2F为DEL ,0x08为BACKSPACE
		return TRUE ;
	}
	if ( !OnInput(pHtmlEditImp) )
	{
		return FALSE ;
	}
	return TRUE;
}

bool HtmlEditLogic::OnInput(HtmlEditImpl* pHtmlEditImp)
{
	//派生类实现
	return true ;
}

BOOL HtmlEditLogic::OnClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj)
{
	//todo
	return FALSE;
}

BOOL HtmlEditLogic::OnDBClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj)
{
	return FALSE;
}

BOOL HtmlEditLogic::OnMouseOut(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) 
{
	return TRUE ;
}

BOOL HtmlEditLogic::OnMouseMove(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect )
{
	return TRUE ;
}

void HtmlEditLogic::OnSettingMenu()
{
	//派生类实现
}

bool HtmlEditLogic::CanSendNum(HtmlEditImpl* pHtmlEditImp,int sendNum)
{
	return true;
	//派生类实现
}

void HtmlEditLogic::OnCopyFile(HtmlEditImpl* pHtmlEditImp, LPCTSTR lpszSourceFile)
{
	//派生类实现
}

//复制文字
void HtmlEditLogic::OnEditCopyWord(HtmlEditImpl* pHtmlEditImp ) 
{
	CString strText ;

	m_pHtmlEditImp->OnEditCopyWord( strText ) ;

	CopyDataToClipbord(strText) ;
}

void HtmlEditLogic::OnTrackHrefLinkMenu( uint32 unDstUin ) 
{
}


void HtmlEditLogic::CopyDataToClipbord(CString& strText ) 
{
	bool flag = false ;
	COleDataSource* pSource = new COleDataSource ;
	if ( pSource != NULL )
	{
		if(strText.GetLength() > 0)
		{

			CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
			sf.Write(strText.GetBuffer(), 2*strText.GetLength());

			HGLOBAL hMem = sf.Detach();
			if(hMem == NULL) return;

			pSource->CacheGlobalData(CF_UNICODETEXT, hMem);

			flag = true;
		}

		if(flag)
		{
			pSource->SetClipboard();
		}
	}
}

void HtmlEditLogic::CopyDataToClipbord(CString strHtmlText, CString strText)
{
	bool flag = false;
	bool bTransfer = true;

	COleDataSource* pSource = new COleDataSource(); 

	if(pSource != NULL)
	{
		if(strHtmlText.GetLength() > 0)
		{
			if(strHtmlText.Find(L"</SPAN><BR><FONT") >= 0)
			{
				bTransfer = true;
			}
		}
		if(strText.GetLength() > 0)
		{
			CString strTemp = strText;
			if(bTransfer)
			{
				int nPos = strText.Find(L"\r\n\r\n");
				if(nPos >= 0)
				{
					strTemp = strText.Left(nPos);
					nPos += 2;
					strTemp += strText.Right(strText.GetLength() - nPos);
					strText = strTemp;
				}
			}

			CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
			sf.Write(strText.GetBuffer(), 2*strText.GetLength());

			HGLOBAL hMem = sf.Detach();
			if(hMem == NULL) return;

			pSource->CacheGlobalData(CF_UNICODETEXT, hMem);

			flag = true;
		}

		if(flag)
		{
			pSource->SetClipboard();
		}

		//这个地方千万不能delete -_-!
		//delete pSource;
	}
}

long HtmlEditLogic::GetDataFromClipboard(HtmlEditImpl* pHtmlEditImp, CString& strInfo)
{
	COleDataObject obj;

	if(!obj.AttachClipboard()) return 0;

	UINT format = -1;
	
	if(obj.IsDataAvailable(CF_UNICODETEXT))
	{
		format = CF_UNICODETEXT;
	}
	else if(obj.IsDataAvailable(CF_TEXT))
	{
		format = CF_TEXT;
	}

	if(format != -1)
	{
		ProcessBaseFormat(obj, format, strInfo);
		return CF_TEXT;
	}

	if(obj.IsDataAvailable(CF_DIB))
	{
		ProcessDIBFormat(obj, strInfo);

		return CF_DIB;
	}

	return 0;
}

void HtmlEditLogic::ProcessBaseFormat(COleDataObject& obj, UINT format, CString& strInfo)
{
	USES_CONVERSION;

	HGLOBAL hmem = obj.GetGlobalData(format);

	int nlength = ::GlobalSize(hmem);

	CMemFile sf((BYTE*)::GlobalLock(hmem), nlength);

	if(format == CF_TEXT)
	{
		char* pTemp = new char[nlength+1];
		memset(pTemp, 0, nlength+1);

		sf.Read(pTemp, nlength);

		strInfo = A2T(pTemp);

		delete[] pTemp;
	}
	else
	{
		wchar_t* pTemp = new wchar_t[nlength+1];
		memset(pTemp, 0, sizeof(wchar_t)*(nlength+1));

		sf.Read(pTemp, nlength);

		strInfo = pTemp;

		delete[] pTemp;
	}

	::GlobalUnlock(hmem);
}

void HtmlEditLogic::ProcessDIBFormat(COleDataObject& obj, CString& strInfo)
{
	USES_CONVERSION;

	HGLOBAL hGMem = obj.GetGlobalData(CF_DIB);

	CxImage xImagebmp(CXIMAGE_FORMAT_BMP);

	if(hGMem != NULL)
	{
		if(!xImagebmp.CreateFromHANDLE((HANDLE)hGMem))
		{
			errno_t err;
			_get_errno(&err);

			CString strError;
			strError.Format(L"截图读取数据失败！错误码: %d", err);
			C51SkinMessageBox::Show(strError,_T("提示"),MB_OK|MB_ICONEXCLAMATION);

			return;
		}
	}
	else
	{
		bool bSucc = false;

		if(OpenClipboard(NULL))
		{
			HANDLE handle = GetClipboardData(CF_BITMAP);		

			if(xImagebmp.CreateFromHBITMAP((HBITMAP)handle))
			{
				bSucc = true;
			}
			
			CloseClipboard();
		}

		if(!bSucc)
		{
			C51SkinMessageBox::Show(L"粘贴板数据读取失败！",_T("提示"), MB_OK|MB_ICONEXCLAMATION);	

			return;
		}
	}	

	wstring strModulePath = utility::systemhelper::Get179AppPath();

	CString strFullPath;
	strFullPath.Format(_T("%s%s/%s/"), strModulePath.c_str(),
		m_Setting.str_ShowUin, PICTURE_CUSTOM_FOLDER);

	utility::filehelper::CreateAllDirectory(std::wstring(strFullPath.GetBuffer()));

	CString strName;
	strName.Format(_T("%u.jpg"),  GetTickCount());

	strFullPath += strName;

	if(!xImagebmp.IsGrayScale()) xImagebmp.IncreaseBpp(24);

	if(!xImagebmp.SaveW(strFullPath.GetBuffer(), CXIMAGE_FORMAT_JPG))
	{
		errno_t err;
		_get_errno(&err);

		if (err == ENOSPC)
		{
			C51SkinMessageBox::Show(_T("磁盘空间不足，截图失败!"),_T("提示"),MB_OK|MB_ICONEXCLAMATION);

			return;
		}
		else
		{
			strFullPath.Replace(L".jpg", L".bmp");

			if(!xImagebmp.SaveW(strFullPath.GetBuffer(), CXIMAGE_FORMAT_BMP))
			{
				CString strError;
				strError.Format(L"截图失败 错误码：%d！", err);
				C51SkinMessageBox::Show(strError,_T("提示"),MB_OK|MB_ICONEXCLAMATION);

				return;
			}
			else
			{
				wstring strFileTemp = strFullPath.GetBuffer();

				CxImage xImagebmp2(strFullPath.GetBuffer(), CXIMAGE_FORMAT_BMP);
				strFullPath.Replace(L".bmp", L".jpg");

				if(!xImagebmp2.SaveW(strFullPath.GetBuffer(), CXIMAGE_FORMAT_JPG))
				{
					CString strError;
					strError.Format(L"截图失败 错误码：%d！", err);
					C51SkinMessageBox::Show(strError,_T("提示"),MB_OK|MB_ICONEXCLAMATION);

					return;
				}
				else
				{
					DeleteFile(strFileTemp.c_str());
				}
			}
		}
	}

	//重命名

	CString md5Path;
	md5Path.Format(_T("%s%s/%s/"), strModulePath.c_str(), m_Setting.str_ShowUin, PICTURE_CUSTOM_FOLDER);

	string strTempMD5Name = utility::md5::GetFileMd5(wstring(strFullPath.GetBuffer()));
	
	wstring strMd5 = common::utility::stringhelper::AsciiToUnicode(strTempMD5Name);

	md5Path.Append(strMd5.c_str());

	md5Path.Append(_T(".jpg"));

	try
	{
		CFile::Rename(strFullPath, md5Path);
	}
	catch (...) {
	}

	//OutputDebugString(md5Path);

	common::CURLEncode urlEncode;
	md5Path = (urlEncode.URLEncodeLocalPath(std::wstring(md5Path.GetBuffer()))).c_str();

	//OutputDebugString(md5Path);

	strInfo.Format(_T("%s%s%s%s"),_T("<img "),
		_T("src=\""), md5Path.GetBuffer(), _T("\">"));
}

void HtmlEditLogic::MakeItClean()
{
	/*CString strText;
	m_pHtmlEditImp->GetContent(strText);

	ReplaceImgForInput(strText);
	MsgFormatInput(strText);
	TranslateMsgForInput(strText);

	if(strText.IsEmpty() || strText == _T("<P>&nbsp;</P>"))
	{
		m_pHtmlEditImp->PutBodyInnerHTML(_T(""));
	}*/
}

int HtmlEditLogic::GetInputStatus()
{
	return 0;
}

void HtmlEditLogic::FilterHtmlText(CString& strHtmlText)
{
	return;
}

BOOL HtmlEditLogic::IsCanConFile(HtmlEditImpl* pHtmlEditImp)
{
	return FALSE;
}
