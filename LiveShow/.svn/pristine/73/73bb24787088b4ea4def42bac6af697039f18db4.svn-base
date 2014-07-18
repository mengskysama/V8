#pragma once

#include "HtmlEditCore2.h"

#ifdef HTMLEDIT_EXPORTS
#define HTMLEDIT_API __declspec(dllexport)
#else
#define HTMLEDIT_API __declspec(dllimport)
#endif

class HTMLEDIT_API HtmlEditLogic
{
protected:
	HtmlEditImpl* m_pHtmlEditImp;

public:
	HtmlEditImplSetting m_Setting;

	HtmlEditLogic(int iEditable);
	virtual ~HtmlEditLogic();
	void SetHtmlEditImpl(HtmlEditImpl* pHtmlEditImp);
	virtual void ClearOutput() ;			//清空公聊窗口的内容
	virtual void OnSendMessage(HtmlEditImpl* pHtmlEditImp);//快捷键发送消息
	virtual void OnOutputReady(HtmlEditImpl* pHtmlEditImp);
	virtual void OnCustomEmotion(HtmlEditImpl* pHtmlEditImp, LPCTSTR lpszEmotionPath);//选择了自定义表情
	virtual void OnEditCopy(HtmlEditImpl* pHtmlEditImp);
	virtual void OnEditSaveToNote(HtmlEditImpl* pHtmlEditImp);
	virtual void OnEditShowInputStatus();
	virtual void OnEditCut(HtmlEditImpl* pHtmlEditImp);
	virtual int  OnEditPaste(HtmlEditImpl* pHtmlEditImp);

	virtual BOOL OnKeyDown(HtmlEditImpl* pHtmlEditImp, long);
	virtual BOOL OnClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj);
	virtual BOOL OnDBClick(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj);
	virtual BOOL OnMouseOut(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj , CRect& CWindowRect ) ;
	virtual BOOL OnMouseMove(HtmlEditImpl* pHtmlEditImp, IHTMLEventObj *pEvtObj, CRect& CWindowRect ) ;
	virtual void OnSettingMenu();	
	virtual bool CanSendNum(HtmlEditImpl* pHtmlEditImp,int sendNum);   //是否可以发送sendNum个头像
	virtual void OnCopyFile(HtmlEditImpl* pHtmlEditImp, LPCTSTR lpszSourceFile);
	virtual	void MakeItClean();
	virtual int  GetInputStatus();
	virtual void FilterHtmlText(CString& strHtmlText);

	virtual void OnSetFocus(HtmlEditImpl* pHtmlEditImp);
	virtual bool OnInput(HtmlEditImpl* pHtmlEditImp);

	virtual BOOL IsCanConFile(HtmlEditImpl* pHtmlEditImp);

	void CopyDataToClipbord(CString strHtmlText, CString strText);	

	virtual void OnEditCopyWord(HtmlEditImpl* pHtmlEditImp ); //复制文字

	void CopyDataToClipbord(CString& strText ) ;
	virtual void OnTrackHrefLinkMenu( uint32 unDstUin ) ;
private:

	void ProcessBaseFormat(COleDataObject& obj, UINT format, CString& strInfo);
	void ProcessDIBFormat(COleDataObject& obj, CString& strInfo);
	long GetDataFromClipboard(HtmlEditImpl* pHtmlEditImp, CString& strInfo);
};