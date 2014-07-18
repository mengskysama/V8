#include "StdAfx.h"
#include "ImageBase.h"
#include "SkinProxy.h"
#include "SkinItem.h"
#include "SkinEngine.h"
#include "RenderEngine.h"
#include "SkinClass.h"
#include ".\skinbase.h"
#include "SkinLayout.h"

#include "IconMaker.h"
#include "Types.h"
#include "SkinHelper.h"

#include <algorithm>
#include <list>
#include <fstream>

using namespace std;

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

IMPLEMENT_DYNCREATE(CSkinBase, CXmlCmdTarget)
IMPLEMENT_SKINBASE_PROPERTIES(CSkinBase)

#ifdef _DEBUG
int CSkinBase::s_iSkinBaseCount = 0;
#endif

#ifdef USE_V8_JS_ENGINE
#define FIRE(funcName) V8Fire_##funcName
#else
#ifdef USE_MS_JS_ENGINE
#define FIRE(funcName) Fire_##funcName
#endif //USE_MS_JS_ENGINE
#endif //USE_V8_JS_ENGINE

CSkinBase::CSkinBase()
:
m_pEngine(NULL),
m_pSkinParent(NULL),
m_pSkinOwner(NULL),
m_pSkinNext(NULL),
m_pSkinPrev(NULL),
m_pSkinProxy(NULL),
m_bVisible(TRUE),
m_bDisabled(FALSE), 
m_bTabStop(FALSE),
m_bGroup(FALSE),
m_bBackground(FALSE),
m_bMouseEvent(TRUE),
m_bTransparent(FALSE),
m_bColorTransform(TRUE),
m_bAlpha(FALSE),
m_hCursor(NULL),
m_hSysCursor(NULL),
m_bSizeDelay(FALSE),
m_bSizeDelayFlag(FALSE),
m_bScriptHandleEvent(FALSE)
{
	//m_strName = _T("");
	//m_strText = _T("");
	//m_strCursor = _T("");

	memset(&m_rcSkinRect, 0, sizeof(CRect));

	m_hSkinRgn = NULL;
	m_nUpCornerRadius = 0;
	m_nDownCornerRadius = 0;

	m_nBorderLineWidth = 0;
	m_clrBorderLineColor = -1;

	m_clrTransparentColor = RGB(255, 0, 255);

#ifdef _DEBUG
	//对象计数
	s_iSkinBaseCount ++;
	TRACE(("Construct Skin count %d\n"), s_iSkinBaseCount);
#endif
}

CSkinBase::~CSkinBase()
{
	if (m_hCursor != NULL){
		//因为Cursor创建自CreateIconIndirect，故用DestroyIcon销毁
		::DestroyIcon(m_hCursor);
		m_hCursor = NULL;
	}

	if (m_hSkinRgn){
		::DeleteObject(m_hSkinRgn);
		m_hSkinRgn = NULL;
	}

	//销毁子皮肤对象
	if (GetEngine() != NULL){
		list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(),
			last = m_listSkinBase.end();
		for (; iter!=last; iter++){
			CSkinBase* pBase = (*iter);
			if (! GetEngine()->IsSkinDestroyed(pBase))
				GetEngine()->DestroySkin(pBase);
		}
	}
	m_listSkinBase.clear();

	//销毁布局
	LayoutArray::iterator iter = m_vecLayouts.begin(),
		last = m_vecLayouts.end();
	for (; iter!=last; iter++) {
		delete iter->second;
	}
	m_vecLayouts.clear();

#ifdef V8_JS_ENGINE_SUPPORT
	V8ClearAllFunction();
#endif //V8_JS_ENGINE_SUPPORT

#ifdef _DEBUG
	//对象计数
	s_iSkinBaseCount --;
	USES_CONVERSION;
	TRACE(("Destruct Skin count %d : SkinName: %s \n"), s_iSkinBaseCount, T2A(GetSkinName()));
#endif
}

void CSkinBase::Initialize()
{
	CXmlCmdTarget::Initialize();

	//获得基类链
	vector<const SKIN_DISPATCH_INFO*> vecBaseClass;
	const SKIN_DISPATCH_INFO* pDispInfo = GetSkinDispatchInfo();
	for (; pDispInfo!= CSkinBase::GetBaseSkinDispatchInfo(); pDispInfo=pDispInfo->GetBaseSkinDispatchInfo())
		vecBaseClass.push_back(pDispInfo);

	//从基类到子类加载默认设置
	vector<const SKIN_DISPATCH_INFO*>::const_reverse_iterator rIter = vecBaseClass.rbegin(), 
		rLast = vecBaseClass.rend();
	for (; rIter!=rLast; rIter++) {
		const SKIN_DISPATCH_INFO* pInfo = *rIter;
		LoadSkinClassHelper(pInfo->GetClassName(), _T("default"));
	}
}

DWORD CSkinBase::ReleaseSkin()
{
	CString strName = m_strName;
	DWORD dwRef = ExternalRelease();
	USES_CONVERSION;
	TRACE(("ReleaseSkin: %s, 引用计数变为: %d\n"), T2A(strName), dwRef);
	return dwRef;
}

void CSkinBase::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSkinBase, CXmlCmdTarget)
END_MESSAGE_MAP()

BEGIN_SKIN_DISPATCH_MAP(CSkinBase, CXmlCmdTarget)
	SKINBASE_DISP_PROPERTIES(CSkinBase)
END_SKIN_DISPATCH_MAP(CSkinBase)

// Note: we add support for IID_ISkinBase to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {F51592EC-DB76-4872-B417-457FEFCBE61C}
static const IID IID_ISkinBase =
{ 0xF51592EC, 0xDB76, 0x4872, { 0xB4, 0x17, 0x45, 0x7F, 0xEF, 0xCB, 0xE6, 0x1C } };
BEGIN_INTERFACE_MAP(CSkinBase, CXmlCmdTarget)
	INTERFACE_PART(CSkinBase, IID_ISkinBase, Dispatch)
END_INTERFACE_MAP()

#ifdef MS_JS_ENGINE_SUPPORT

HRESULT CSkinBase::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti)
{
	if( (dwReturnMask&SCRIPTINFO_IUNKNOWN) && ppiunkItem == NULL)
		return E_POINTER;

	if( (dwReturnMask&SCRIPTINFO_ITYPEINFO) && ppti == NULL)
		return E_POINTER;

	//初始化指针为NULL
	if(ppiunkItem != NULL){
		*ppiunkItem = NULL;
	}

	if(ppti != NULL){
		*ppti = NULL;
	}

	const CString strItemName(pstrName);
	HRESULT hrUnknown = E_NOINTERFACE;
	//查询IUNKNOWN
	if (dwReturnMask&SCRIPTINFO_IUNKNOWN){
		//Window对象
		if (strItemName == _T("Window")){
			hrUnknown = GetInterface(ppiunkItem);
		}else{
			//子对象
			list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(),
				last = m_listSkinBase.end();
			for (; iter!=last; iter++){
				CSkinBase* pBase = (*iter);
				if (strItemName != pBase->GetSkinName())
					continue;
				hrUnknown = pBase->GetInterface(ppiunkItem);
				break;
			}
		}
	}

	//查询TypeInfo
	HRESULT hrTypeInfo = E_NOINTERFACE;
	if (dwReturnMask&SCRIPTINFO_ITYPEINFO){
		//???
	}

	HRESULT hr = hrUnknown;
	return hr;
}

HRESULT CSkinBase::OnScriptError(IActiveScriptError *pscripterror)
{
	EXCEPINFO	excepInfo;
	DWORD		sourceContext;
	ULONG		lineNumber;
	LONG		charPosition;
	BSTR		bstrLineText = NULL;

	//获取错误信息
	pscripterror->GetExceptionInfo(&excepInfo);
	pscripterror->GetSourcePosition(&sourceContext, &lineNumber, &charPosition);
	pscripterror->GetSourceLineText(&bstrLineText);

	//字串转换
	CString strEISource(excepInfo.bstrSource);
	CString strEIDescription(excepInfo.bstrDescription);
	CString strEIHelpFile(excepInfo.bstrHelpFile);
	CString strLineText(bstrLineText);

	//调试输出错误信息
	USES_CONVERSION;
	TRACE(("错误JS文件: %s\n 错误源: %s\n 错误描述: %s\n 错误HelpFile: %s\n 错误行: %s\n 错误行号: %d\n 错误位置: %d\n"),
		T2A(GetScriptFileName()), T2A(strEISource), T2A(strEIDescription), T2A(strEIHelpFile), T2A(strLineText), lineNumber, charPosition);

	//对话框输出错误信息
	CString strError;
	strError.Format(_T("错误JS文件: %s\n 错误源: %s\n 错误描述: %s\n 错误HelpFile: %s\n 错误行: %s\n 错误行号: %d\n 错误位置: %d\n"),
		GetScriptFileName(), strEISource, strEIDescription, strEIHelpFile, strLineText, lineNumber, charPosition);
	HWND hParentWnd = GetSkinHwnd();
	hParentWnd = (::IsWindow(hParentWnd)) ? hParentWnd : NULL;
	::MessageBox(hParentWnd, strError, _T("Skin Script Error"), MB_OK | MB_TOPMOST);

	//输出调用堆栈
	TRACE(("Skin Script Error: 函数调用堆栈如下:\n"));
	AfxDumpStack();

	return S_OK;
}

HRESULT CSkinBase::GetWindow(HWND *phwnd)
{
	*phwnd = GetSkinHwnd();
	return S_OK;
}

BOOL CSkinBase::AddNameItemToSrcipt(CComPtr<IActiveScript> pScript)
{
	if (pScript == NULL)
		return FALSE;

	HRESULT hr = S_OK;
	//添加Window名字
	try{
		CComBSTR bstrView = _T("Window");
		hr = pScript->AddNamedItem(bstrView, 
			SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE | SCRIPTITEM_GLOBALMEMBERS);
	}catch(...) {

		TRACE(("%s: AddNameItem异常\n"), CT2A(GetScriptFileName()));
	}
	if(FAILED(hr)){
		return FALSE;
	}

	//添加子控件名字
	list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(),
		last = m_listSkinBase.end();
	for (; iter!=last; iter++){
		CSkinBase* pBase = (*iter);
		try{
			BSTR bstrItem = pBase->Getname();
			hr = pScript->AddNamedItem(bstrItem,
				SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE | SCRIPTITEM_GLOBALMEMBERS);
			::SysFreeString(bstrItem);
		}catch(...){
			TRACE(("%s: AddNameItem异常\n"), CT2A(pBase->GetSkinName()));
		}
		if (FAILED(hr))
			return FALSE;
	}

	return TRUE;
}

#endif //MS_JS_ENGINE_SUPPORT

#ifdef V8_JS_ENGINE_SUPPORT

BOOL CSkinBase::AddObjectToSrcipt(Handle<Object> object)
{
	HandleScope handleScope;

	Handle<Value> targetObj = v8::Null();

	//添加Window名字
	targetObj = GetV8Object();
	Handle<String> windowName = CStringToString(_T("Window"));
	object->Set(windowName, targetObj);

	//添加子控件名字
	list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(),
		last = m_listSkinBase.end();
	for (; iter!=last; iter++) {
		CSkinBase* pBase = (*iter);
		targetObj = pBase->GetV8Object();
		Handle<String> skinName = CStringToString(pBase->GetSkinName());
		object->Set(skinName, targetObj);
	}

	return TRUE;
}

#endif //V8_JS_ENGINE_SUPPORT

//////////////////////////////////////////////////////////////////////////
//初始化
//////////////////////////////////////////////////////////////////////////
BOOL CSkinBase::SetConfigFileName(LPCTSTR strConfigFileName)
{
	if (! m_strConfigName.IsEmpty())
		return FALSE;

	m_strConfigName = strConfigFileName;
	return TRUE;
}

CString CSkinBase::GetXmlFileName() const
{
	return m_strConfigName + _T(".xml");
}

CString CSkinBase::GetScriptFileName() const
{
	return m_strConfigName + _T(".js");
}

//////////////////////////////////////////////////////////////////////////
//XML解析
//////////////////////////////////////////////////////////////////////////
BOOL CSkinBase::PreLoadXml(LPCTSTR lpzPreLoad)
{
	CString strPreLoad = lpzPreLoad;
	if (strPreLoad.IsEmpty())
		return FALSE;

	CString strPreLoadFileName = strPreLoad + _T(".xml");
	CString strPreLoadPathName = GetEngine()->GetSkinPath() + strPreLoadFileName;

	CFileData fileData;
	if (! GetEngine()->GetFile(strPreLoadPathName, fileData))
		return FALSE;

	IXMLDOMElementPtr ptrRoot;
	if (! GetXmlRoot(fileData, TRUE, &ptrRoot))
		return FALSE;

	BOOL bCreate = CreateSubSkinFromXml(ptrRoot, FALSE);
	ASSERT(bCreate);
	if (!bCreate)
		return FALSE;

	BOOL bLoad = LoadSubSkinPropFromXml(ptrRoot);
	ASSERT(bLoad);
	if (!bLoad)
		return FALSE;

	return TRUE;
}

BOOL CSkinBase::CreateSubSkinFromXml(IXMLDOMElementPtr ptrDocumentRoot, BOOL bPreLoad)
{
	if (ptrDocumentRoot == NULL)
		return FALSE;

	//检查第一级的元素是否是SkinItemDialog
	IXMLDOMNodePtr ptrSkinItemDialogNode;
	ptrDocumentRoot->get_firstChild(&ptrSkinItemDialogNode);
	if (ptrSkinItemDialogNode == NULL)
		return FALSE;

	//过滤掉#comment
	do {

		BSTR bstrSkinItemDialogName = NULL;
		ptrSkinItemDialogNode->get_nodeName(&bstrSkinItemDialogName);
		CString strSkinItemDialogName(bstrSkinItemDialogName);
		::SysFreeString(bstrSkinItemDialogName);
		if (strSkinItemDialogName.CompareNoCase(_T("Skin_Window")) == 0)
			break;

		IXMLDOMNodePtr ptrTempNode;
		ptrSkinItemDialogNode->get_nextSibling(&ptrTempNode);
		ptrSkinItemDialogNode = ptrTempNode;

	} while (ptrSkinItemDialogNode != NULL);

	if (ptrSkinItemDialogNode == NULL)
		return FALSE;

	//预加载属性
	CComVariant varRemoveCaption;
	IXMLDOMElementPtr ptrSkinItemDialogElement = ptrSkinItemDialogNode;
	if (ptrSkinItemDialogElement->getAttribute(L"removeNC", &varRemoveCaption) == S_OK) {
		SetProperty(L"removeNC", varRemoveCaption);
	}
	varRemoveCaption.Clear();
	if (ptrSkinItemDialogElement->getAttribute(L"skinScrollBar", &varRemoveCaption) == S_OK) {
		SetProperty(L"skinScrollBar", varRemoveCaption);
	}
	varRemoveCaption.Clear();
	if (ptrSkinItemDialogElement->getAttribute(L"useOriginPosition", &varRemoveCaption) == S_OK) {
		SetProperty(L"useOriginPosition", varRemoveCaption);
	}
	varRemoveCaption.Clear();
	if (ptrSkinItemDialogElement->getAttribute(L"useOrigin", &varRemoveCaption) == S_OK) {
		SetProperty(L"useOrigin", varRemoveCaption);
	}
	varRemoveCaption.Clear();
	if (ptrSkinItemDialogElement->getAttribute(L"useOriginText", &varRemoveCaption) == S_OK) {
		SetProperty(L"useOriginText", varRemoveCaption);
	}
	varRemoveCaption.Clear();
	if (ptrSkinItemDialogElement->getAttribute(L"preLoad", &varRemoveCaption) == S_OK) {
		SetProperty(L"preLoad", varRemoveCaption);
	}

	CString strPreLoad = GetSkinPreLoad();
	if (bPreLoad)
		PreLoadXml(strPreLoad);

	//解析子皮肤元素
	IXMLDOMNodePtr ptrSubSkinNode;
	ptrSkinItemDialogNode->get_firstChild(&ptrSubSkinNode);
	while (ptrSubSkinNode != NULL){

		//获得子皮肤元素类别名字
		BSTR bstrSubSkinXmlName = NULL;
		ptrSubSkinNode->get_nodeName(&bstrSubSkinXmlName);
		CString strSubSkinXmlName(bstrSubSkinXmlName);
		::SysFreeString(bstrSubSkinXmlName);

		//创建皮肤元素
		CSkinBase* pSkin = GetEngine()->CreateSkinByXmlName(strSubSkinXmlName);
		if (pSkin != NULL){
			//设置name属性
			IXMLDOMElementPtr ptrSubSkinElement = ptrSubSkinNode;
			if (pSkin->IsFiltered(FILTER_SKINITEM, FILTER_SKINITEM)) {
				CComVariant varNameValue;
				if (ptrSubSkinElement->getAttribute(L"name", &varNameValue) == S_OK) {
					pSkin->SetProperty(L"name", varNameValue);
				}
			} else {
				pSkin->LoadPropertyFromXml(ptrSubSkinElement);
			}
			pSkin->SetSkinParent(this);
			AddSubSkin(pSkin);
		}

		//下一节点
		IXMLDOMNodePtr ptrTempNode;
		ptrSubSkinNode->get_nextSibling(&ptrTempNode);
		ptrSubSkinNode = ptrTempNode;

	}

	return TRUE;
}

BOOL CSkinBase::LoadSubSkinPropFromXml(IXMLDOMElementPtr ptrDocumentRoot)
{
	if (ptrDocumentRoot == NULL)
		return FALSE;

	//检查第一级的元素是否是SkinItemDialog
	IXMLDOMNodePtr ptrSkinItemDialogNode;
	ptrDocumentRoot->get_firstChild(&ptrSkinItemDialogNode);
	if (ptrSkinItemDialogNode == NULL)
		return FALSE;

	//过滤掉#comment
	do {

		BSTR bstrSkinItemDialogName = NULL;
		ptrSkinItemDialogNode->get_nodeName(&bstrSkinItemDialogName);
		CString strSkinItemDialogName(bstrSkinItemDialogName);
		::SysFreeString(bstrSkinItemDialogName);
		if (strSkinItemDialogName.CompareNoCase(_T("Skin_Window")) == 0)
			break;

		IXMLDOMNodePtr ptrTempNode;
		ptrSkinItemDialogNode->get_nextSibling(&ptrTempNode);
		ptrSkinItemDialogNode = ptrTempNode;

	} while (ptrSkinItemDialogNode != NULL);

	if (ptrSkinItemDialogNode == NULL)
		return FALSE;

	//载入SkinItemDialog属性
	IXMLDOMElementPtr ptrSkinItemDialogElementNode = ptrSkinItemDialogNode;
	ptrSkinItemDialogElementNode->removeAttribute(L"preLoad");
	if (! LoadPropertyFromXml(ptrSkinItemDialogNode))
		return FALSE;

	//解析子皮肤元素
	IXMLDOMNodePtr ptrSubSkinNode;
	ptrSkinItemDialogNode->get_firstChild(&ptrSubSkinNode);
	while (ptrSubSkinNode != NULL){

		//获得子皮肤元素的名字
		DOMNodeType nodeType = NODE_INVALID;
		ptrSubSkinNode->get_nodeType(&nodeType);
		if (nodeType == NODE_ELEMENT) {
			IXMLDOMElementPtr ptrSubSkinElement = ptrSubSkinNode;
			CComVariant varNameValue;
			if ( ptrSubSkinElement->getAttribute(L"name", &varNameValue) == S_OK) {
				//获得子皮肤元素
				CSkinBase* pSkin = GetSubSkin(varNameValue.bstrVal);
				if (pSkin != NULL){
					//删除name属性
					ptrSubSkinElement->removeAttribute(L"name");
					if (pSkin->IsFiltered(FILTER_SKINITEM, FILTER_SKINITEM)) {
						pSkin->LoadPropertyFromXml(ptrSubSkinElement);
					}
				}
			}
		}

		//下一节点
		IXMLDOMNodePtr ptrTempNode;
		ptrSubSkinNode->get_nextSibling(&ptrTempNode);
		ptrSubSkinNode = ptrTempNode;

	}

	return TRUE;
}

BOOL CSkinBase::GetXmlRoot(const char* pzXml, BOOL bUnicode, IXMLDOMElement** ppXmlRoot)
{
	if (ppXmlRoot == NULL)
		return FALSE;

	*ppXmlRoot = NULL;

	if (pzXml == NULL)
		return FALSE;

	//转换为UNICODE
	CComBSTR bstrXmlDoc;
	if (! bUnicode) {
		bstrXmlDoc = pzXml;
	} else {
		bstrXmlDoc.Attach(W2BSTR((WCHAR*)(pzXml+2)));
	}

	//创建XML解析器
	IXMLDOMDocumentPtr pXmlDoc = NULL;
	HRESULT hr = S_OK;
	hr = ::CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
		IID_IXMLDOMDocument, (void**)&pXmlDoc);
	if (pXmlDoc == NULL || FAILED(hr)){
		ASSERT(0 && _T("XML解析器创建失败!"));
		return FALSE;
	}

	//载入XML
	VARIANT_BOOL bXmlSuc = VARIANT_FALSE;
	hr = pXmlDoc->loadXML(bstrXmlDoc, &bXmlSuc);
	if (FAILED(hr) || bXmlSuc != VARIANT_TRUE){
		IXMLDOMParseErrorPtr parseError;
		pXmlDoc->get_parseError(&parseError);
		CString strError;
		if (parseError != NULL) {
			LONG errorCode = 0;
			parseError->get_errorCode(&errorCode);
			LONG filePos = -1;
			parseError->get_filepos(&filePos);
			LONG line = -1;
			parseError->get_line(&line);
			LONG linePos = -1;
			parseError->get_linepos(&linePos);
			CComBSTR bstrReason;
			parseError->get_reason(&bstrReason);
			CString strReason = bstrReason;
			CComBSTR bstrSrcText;
			parseError->get_srcText(&bstrSrcText);
			CString strSrcText = bstrSrcText;
			CComBSTR bstrUrl;
			parseError->get_url(&bstrUrl);
			CString strUrl = bstrUrl;
			strError.Format(_T("错误码：%d, 错误文件位置：%d, 错误行：%d, 错误行位置：%d, 错误原因：%s, 错误源：%s, 错误url：%s"), 
				errorCode, 
				filePos,
				line,
				linePos,
				strReason,
				strSrcText, 
				strUrl
				);
		}
		ASSERT(0 && _T("XML载入错误"));
#ifdef _DEBUG
		::MessageBox(NULL, strError, _T("XML解析错误"), MB_OK | MB_TOPMOST);
#endif
		return FALSE;
	}

	//获得XML文档的Root
	if (pXmlDoc->get_documentElement(ppXmlRoot) != S_OK)
		return FALSE;

	return TRUE;
}

BOOL CSkinBase::CreateFromXml()
{
	CString strXmlPathName = GetEngine()->GetSkinPath() + GetXmlFileName();

	CFileData fileData;
	if (! GetEngine()->GetFile(strXmlPathName, fileData))
		return FALSE;

	IXMLDOMElementPtr ptrRoot;
	if (! GetXmlRoot(fileData, TRUE, &ptrRoot))
		return FALSE;

	return CreateSubSkinFromXml(ptrRoot);
}

BOOL CSkinBase::LoadPropFromXml()
{
	CString strXmlPathName = GetEngine()->GetSkinPath() + GetXmlFileName();

	CFileData fileData;
	if (! GetEngine()->GetFile(strXmlPathName, fileData))
		return FALSE;

	IXMLDOMElementPtr ptrRoot;
	if (! GetXmlRoot(fileData, TRUE, &ptrRoot))
		return FALSE;

	return LoadSubSkinPropFromXml(ptrRoot);
}

BOOL CSkinBase::ParseXmlHelper(IXMLDOMElementPtr ptrDocumentRoot)
{
	if (! CreateFromXml())
		return FALSE;

	if (! LoadPropFromXml())
		return FALSE;

	return TRUE;
}

BOOL CSkinBase::LoadXML(const char* pzXml, BOOL bUnicode)
{
	if (pzXml == NULL)
		return FALSE;

	//转换为UNICODE
	CComBSTR bstrXmlDoc;
	if (! bUnicode) {
		bstrXmlDoc = pzXml;
	} else {
		bstrXmlDoc.Attach(W2BSTR((WCHAR*)(pzXml+2)));
	}

	//创建XML解析器
	IXMLDOMDocumentPtr pXmlDoc = NULL;
	HRESULT hr = S_OK;
	hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
		IID_IXMLDOMDocument, (void**)&pXmlDoc);
	if (pXmlDoc == NULL || FAILED(hr)){
		ASSERT(0 && _T("XML解析器创建失败!"));
		return FALSE;
	}

	//载入XML
	VARIANT_BOOL bXmlSuc = VARIANT_FALSE;
	hr = pXmlDoc->loadXML(bstrXmlDoc, &bXmlSuc);
	if (FAILED(hr) || bXmlSuc != VARIANT_TRUE){
		ASSERT(0 && _T("XML载入错误"));
		return FALSE;
	}

	//获得XML文档的Root
	IXMLDOMElementPtr ptrDocumentRoot;
	pXmlDoc->get_documentElement(&ptrDocumentRoot);
	if (ptrDocumentRoot == NULL)
		return FALSE;

	//解析XML
	if (! ParseXmlHelper(ptrDocumentRoot)) {
		ASSERT(0 && _T("XML解析错误"));
		return FALSE;
	}

	return TRUE;
}

BOOL CSkinBase::ParseXml()
{
	CString strXmlPathName = GetEngine()->GetSkinPath() + GetXmlFileName();

	CFileData fileData;
	if (! GetEngine()->GetFile(strXmlPathName, fileData))
		return FALSE;

	return LoadXML(fileData);
}

CString CSkinBase::GetSkinPath() const
{
	ASSERT(GetEngine() != NULL);
	return GetEngine()->GetSkinPath();
}

BOOL CSkinBase::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	return TRUE;
}

void CSkinBase::DrawParentBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
	CSkinBase* pParent = (CSkinBase*)GetSkinParent();
	if (pParent == NULL)
		return;

	CRect skinRect = GetSkinRect();
	//偏移InvalidRect
	CRect newInvalidRect;
	LPRECT pNewInvalidRect = NULL;
	if (lpInvalidRect != NULL) {
		newInvalidRect = *lpInvalidRect;
		newInvalidRect.OffsetRect(skinRect.left, skinRect.top);
		pNewInvalidRect = &newInvalidRect;
	}
	//偏移InvalidRgn
	CRgn newInvalidRgn;
	CRgn* pNewInvalidRgn = NULL;
	if (pInvalidRgn != NULL) {
		newInvalidRgn.CreateRectRgn(0, 0, 0, 0);
		newInvalidRgn.CopyRgn(pInvalidRgn);
		newInvalidRgn.OffsetRgn(skinRect.left, skinRect.top);
		pNewInvalidRgn = &newInvalidRgn;
	}

	//保存DC
	int iSaveDC = pDC->SaveDC();

	//偏移DC的原点坐标
	CPoint oldViewOrg = pDC->OffsetViewportOrg(-skinRect.left, -skinRect.top);
	//CPoint oldViewOrg = pDC->OffsetWindowOrg(skinRect.left, skinRect.top);

	//不需偏移剪切区域，OffsetViewportOrg会偏移剪切区域
	//pDC->OffsetClipRgn(skinRect.left, skinRect.top);

	pParent->DrawBackground(pDC, pNewInvalidRect, pNewInvalidRgn, TRUE);

	//恢复DC
	pDC->RestoreDC(iSaveDC);
}

void CSkinBase::DrawBackground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn, BOOL bChildSkin)
{
	if (GetTransparent()) {
		DrawParentBackground(pDC, lpInvalidRect, pInvalidRgn);
	}
}

void CSkinBase::DrawForeground(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
}

void CSkinBase::DrawFrame(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
}

void CSkinBase::Draw(CDC* pDC, LPRECT lpInvalidRect, CRgn* pInvalidRgn)
{
}

BOOL CSkinBase::InvalidateRect(LPCRECT lpRect, BOOL bErase)
{
	POINT offset;
	HWND hSkinHwnd = GetSkinHwnd(&offset);
	CSkinItem* pItem = CSkinItem::FromHandlePermanent(hSkinHwnd);
	if (pItem != NULL) {
		CRect invalidRect;
		if (lpRect != NULL)
			invalidRect = *lpRect;
		else
			invalidRect = GetSkinAbsRect();
		invalidRect.OffsetRect(offset);
		return pItem->InvalidateRect(&invalidRect, bErase);
	}

	return FALSE;
}

//区域更新
BOOL CSkinBase::UpdateSkinRgn(LONG width, LONG height, BOOL bReDraw)
{
	if (m_hSkinRgn != NULL){
		::DeleteObject(m_hSkinRgn);
		m_hSkinRgn = NULL;
	}
	m_hSkinRgn = CRenderEngine::CreateUpDownRoundRectRgn(width, height, 
		m_nUpCornerRadius, m_nDownCornerRadius);

	if (bReDraw)
		InvalidateRect();

	return TRUE;
}

BOOL CSkinBase::ApplyDelayedSize(BOOL bRepaint)
{
	MoveWindow(&m_rcSkinRect, bRepaint);
	UpdateSkinRgn(GetWidth(), GetHeight(), bRepaint);
	return TRUE;
}

BOOL CSkinBase::IsPointInSkinRect(POINT pt)
{
	return GetSkinRect().PtInRect(pt);
}

BOOL CSkinBase::IsPointInSkinRect(LONG x, LONG y)
{
	POINT pt = {x, y};
	return IsPointInSkinRect(pt);
}

BOOL CSkinBase::IsPointInSkinRgn(POINT pt)
{
	if (m_hSkinRgn != NULL) {
		pt.x -= GetLeft();
		pt.y -= GetTop();
		return ::PtInRegion(m_hSkinRgn, pt.x, pt.y);
	} else {
		return IsPointInSkinRect(pt);
	}
}

BOOL CSkinBase::IsPointInSkinRgn(LONG x, LONG y)
{
	POINT pt = {x, y};
	return IsPointInSkinRgn(pt);
}

//代理窗口
CSkinProxy* CSkinBase::GetSkinProxy()
{
	return m_pSkinProxy;
}

//父窗口
CSkinBase* CSkinBase::GetSkinParent()
{
	CSkinProxy* pProxy = GetSkinProxy();
	if (pProxy != NULL)
		return pProxy->GetSkinParent();
	return m_pSkinParent;
}

void CSkinBase::SetSkinParent(CSkinBase* pSkinParent)
{
	m_pSkinParent = pSkinParent;
}

//Owner窗口
CSkinBase* CSkinBase::GetSkinOwner()
{
	return m_pSkinOwner;
}

void CSkinBase::SetSkinOwner(CSkinBase* pSkinOwner)
{
	m_pSkinOwner = pSkinOwner;
}

//Owner窗口
CSkinBase* CSkinBase::GetSkinNext()
{
	return m_pSkinNext;
}

void CSkinBase::SetSkinNext(CSkinBase* pSkinNext)
{
	m_pSkinNext = pSkinNext;
}

//Owner窗口
CSkinBase* CSkinBase::GetSkinPrev()
{
	return m_pSkinPrev;
}

void CSkinBase::SetSkinPrev(CSkinBase* pSkinPrev)
{
	m_pSkinPrev = pSkinPrev;
}

CSkinBase* CSkinBase::CreateSkinChild(LPCTSTR lpzClassName, LPCTSTR lpzChildName)
{
	CSkinBase* pBase = GetSubSkin(lpzChildName);
	if (pBase != NULL)
		return NULL;

	CString strChildName = lpzChildName;
	if (strChildName.IsEmpty())
		return NULL;

	pBase = GetEngine()->CreateSkinByClassName(lpzClassName);
	if (pBase == NULL)
		return NULL;

	pBase->SetName(lpzChildName);
	BOOL bSuc = AddSubSkin(pBase);
	ASSERT(bSuc);

	return pBase;
}

//祖先窗口
CSkinBase* CSkinBase::GetSkinAncestor()
{
	CSkinBase* pAncestor = this;
	CSkinBase* pNextParent = pAncestor->GetSkinParent();
	while (pNextParent != NULL) {
		pAncestor = pNextParent;
		pNextParent = pAncestor->GetSkinParent();
	}
	return pAncestor;
}

//获得皮肤对象所依附的窗口句柄
HWND CSkinBase::GetSkinHwnd(LPPOINT lpOffset)
{
	POINT offset = {0, 0};
	CSkinBase* pSkinParent = this;
	while (pSkinParent != NULL && 
		! pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem))) {
			CRect skinRect = pSkinParent->GetSkinRect();
			offset.x += skinRect.left;
			offset.y += skinRect.top;
			pSkinParent = pSkinParent->GetSkinParent();
	}

	if (lpOffset != NULL)
		*lpOffset = offset;

	if (pSkinParent == NULL ||
		! pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem)) ) {
			return NULL;
	} else {
		CSkinItem* pSkinParentItem = (CSkinItem*)pSkinParent;
		return pSkinParentItem->GetSafeHwnd();
	}
}

BOOL CSkinBase::GetSkinHwndOffset(LPPOINT lpOffset)
{
	POINT offset = {0, 0};
	CSkinBase* pSkinParent = this;
	while (pSkinParent != NULL && 
		! pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem))) {
			CRect skinRect = pSkinParent->GetSkinRect();
			offset.x += skinRect.left;
			offset.y += skinRect.top;
			pSkinParent = pSkinParent->GetSkinParent();
	}
	*lpOffset = offset;
	return (pSkinParent != NULL && 
		pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem)));
}

CSkinItem* CSkinBase::GetSkinHwndItem(LPPOINT lpOffset)
{
	POINT offset = {0, 0};
	CSkinBase* pSkinParent = this;
	while (pSkinParent != NULL && 
		! pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem))) {
			CRect skinRect = pSkinParent->GetSkinRect();
			offset.x += skinRect.left;
			offset.y += skinRect.top;
			pSkinParent = pSkinParent->GetSkinParent();
	}
	if (lpOffset != NULL)
		*lpOffset = offset;
	ASSERT(pSkinParent != NULL && pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem)) );
	if (pSkinParent == NULL ||
		! pSkinParent->IsKindOf(RUNTIME_CLASS(CSkinItem)) ) {
			return NULL;
	} else {
		return (CSkinItem*)pSkinParent;
	}
}

CSkinEngine* CSkinBase::GetEngine() const
{
	return m_pEngine;
}

//获得图像变换参数
const CImageTransformParam* CSkinBase::GetImageParam() const
{
	CSkinEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return NULL;
	return pEngine->GetDefaultImageParam();
}

BOOL CSkinBase::SetImageHelper(CImageBase*& pImage, CString& strImage, LPCTSTR newVal, BOOL bSetSize)
{
	if (strImage.CompareNoCase(newVal) == 0)
		return TRUE;

	CString strNewImage = newVal;
	if (strNewImage.IsEmpty()) {
		pImage = NULL;
		strImage.Empty();
		return TRUE;
	}

	CString filePathName = GetSkinPath() + newVal;
	CImageBase* pDib = GetEngine()->LoadImage(filePathName);
	if (pDib == NULL)
		return FALSE;

	pImage = pDib;
	strImage = newVal;
	if (bSetSize) {
		Setwidth(pDib->Width());
		Setheight(pDib->Height());
	}

	InvalidateRect();

	return TRUE;
}

HICON CSkinBase::GetIconHelper(LPCTSTR iconFileName, LONG iconSize)
{
	CString iconFilePathName = GetSkinPath() + iconFileName;

	CFileData fileData;
	if (! GetEngine()->GetFile(iconFilePathName, fileData))
		return NULL;

	return MakeIconFromBuf((LPBYTE)fileData.pFileData, fileData.GetLength(), iconSize);
}

HCURSOR CSkinBase::GetCursorHelper(LPCTSTR curFileName)
{
	CString cursorFilePathName = GetSkinPath() + curFileName;

	CFileData fileData;
	if (! GetEngine()->GetFile(cursorFilePathName, fileData))
		return NULL;

	return MakeCursorFromBuf((LPBYTE)fileData.pFileData, fileData.GetLength());
}

BOOL CSkinBase::DrawImageAlphaSideHelper(CImageBase* pImage, HDC hDC, LPCRECT lpDrawRect, LPRECT lpClipRect, BOOL bAlpha, BOOL bSideStretch, LONG sideWidth, LONG sideHeight)
{
	if (pImage == NULL)
		return FALSE;
	BOOL ret = FALSE;
	if (GetColorTransform())
		ret = pImage->Draw(hDC, lpDrawRect, lpClipRect, GetImageParam(), GetTransformedTransparentColor(), bAlpha, bSideStretch, sideWidth, sideHeight);
	else 
		ret = pImage->Draw(hDC, lpDrawRect, lpClipRect, NULL, GetTransparentColor(), bAlpha, bSideStretch, sideWidth, sideHeight);

	return ret;
}

BOOL CSkinBase::DrawImageAlphaHelper(CImageBase* pImage, HDC hDC, LPCRECT lpDrawRect, LPRECT lpClipRect, BOOL bAlpha)
{
	return DrawImageAlphaSideHelper(pImage, hDC, lpDrawRect, lpClipRect, bAlpha, TRUE, 0, 0);
}

BOOL CSkinBase::DrawImageHelper(CImageBase* pImage, HDC hDC, LPCRECT lpDrawRect, LPRECT lpClipRect)
{
	return DrawImageAlphaSideHelper(pImage, hDC, lpDrawRect, lpClipRect, FALSE, TRUE, 0, 0);
}

DWORD CSkinBase::GetComctl32Version()
{
	ASSERT(GetEngine() != NULL);
	if (GetEngine() == NULL)
		return 0;

	return GetEngine()->GetComctl32Version();
}

BOOL CSkinBase::LoadSkinClassHelper(LPCTSTR lpzClassName, LPCTSTR lpzSkinClassName)
{
	CString strClassName;
	if (lpzClassName == NULL) 
		strClassName = GetClassName();
	else 
		strClassName = lpzClassName;

	CSkinClass* pClass = GetEngine()->GetSkinClass(strClassName, lpzSkinClassName);
	if (pClass == NULL)
		return FALSE;

	pClass->ApplyProp(this);
	return TRUE;
}

IDispatchPtr CSkinBase::GetFunction(LPCTSTR strDispName)
{
	CString sDispName = strDispName;
	sDispName.MakeLower();	//不区分大小写
	map<CString, IDispatchPtr>::const_iterator finded = m_mapDispatch.find(sDispName);
	if (finded != m_mapDispatch.end()) {
		return finded->second;
	} else {
		return NULL;
	}
}

BOOL CSkinBase::SetFunction(LPCTSTR strDispName, IDispatchPtr ptrDisp)
{
	CString sDispName = strDispName;
	sDispName.MakeLower();	//不区分大小写
	if (ptrDisp != NULL) {
		m_mapDispatch[sDispName] = ptrDisp;
	} else {
		m_mapDispatch.erase(sDispName);
	}
	return TRUE;
}

BOOL CSkinBase::InternalAttachEvent(LPCTSTR strFuncName, IDispatch* pFuncDisp)
{
	CString strName = strFuncName;

	if (strName.CompareNoCase(_T("OnNotify")) == 0) {	//别名
		return SetFunction(_T("OnSkinNotify"), pFuncDisp);
	} else if (strName.CompareNoCase(_T("OnInit")) == 0) {	//别名
		return SetFunction(_T("OnInitDialog"), pFuncDisp);
	}

	return SetFunction(strName, pFuncDisp);
}

BOOL CSkinBase::InternalDetachEvent(LPCTSTR strFuncName)
{
	return SetFunction(strFuncName, NULL);
}

//parent
IUnknown* CSkinBase::GetParent()
{
	IUnknown* pUnkown = NULL;
	CSkinBase* pParent = GetSkinParent();
	if (pParent)
		pParent->GetInterface(&pUnkown);

	return pUnkown;
}

//Child
IUnknown* CSkinBase::GetChild(LPCTSTR strChildName)
{
	IUnknown* pUnkown = NULL;
	CSkinBase* pChild = GetSubSkin(strChildName);
	if (pChild != NULL)
		pChild->GetInterface(&pUnkown);

	return pUnkown;
}

IUnknown* CSkinBase::CreateChild(LPCTSTR lpzXmlName, LPCTSTR lpzChildName)
{
	IUnknown* pUnkown = NULL;
	CString strClassName = GetEngine()->GetClassName(lpzXmlName);
	CSkinBase* pChild = CreateSkinChild(strClassName, lpzChildName);
	if (pChild != NULL)
		pChild->GetInterface(&pUnkown);

	return pUnkown;
}

VARIANT_BOOL CSkinBase::AttachEvent(LPCTSTR strFuncName, IDispatch* pFuncDisp)
{
	BOOL bRet = InternalAttachEvent(strFuncName, pFuncDisp);
	return bRet ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

VARIANT_BOOL CSkinBase::DetachEvent(LPCTSTR strFuncName)
{
	BOOL bRet = InternalDetachEvent(strFuncName);
	return bRet ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

void CSkinBase::DebugBreak(void)
{
#ifdef _DEBUG
	::DebugBreak();
#endif
}

UINT_PTR CSkinBase::SetTimer(UINT nIDEvent, UINT nEclapse)
{
	return GetEngine()->SetTimer(nIDEvent, nEclapse, this);
}

VARIANT_BOOL CSkinBase::KillTimer(UINT nIDEvent)
{
	return (GetEngine()->KillTimer(nIDEvent, this)) ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

IUnknown* CSkinBase::GetActive()
{
	CSkinBase* pActive = NULL;
	if (! GetActiveSkin(&pActive))
		return NULL;
	if (pActive == NULL)
		return NULL;
	IUnknown* pUnknown = NULL;
	pActive->GetInterface(&pUnknown);
	return pUnknown;
}

IUnknown* CSkinBase::SetActive()
{
	CSkinBase* pActive = NULL;
	if (! SetActiveSkin(&pActive))
		return NULL;
	if (pActive == NULL)
		return NULL;
	IUnknown* pUnknown = NULL;
	pActive->GetInterface(&pUnknown);
	return pUnknown;
}

IUnknown* CSkinBase::GetFocus()
{
	CSkinBase* pFocus = NULL;
	if (! GetFocusSkin(&pFocus))
		return NULL;
	if (pFocus == NULL)
		return NULL;
	IUnknown* pUnknown = NULL;
	pFocus->GetInterface(&pUnknown);
	return pUnknown;
}

IUnknown* CSkinBase::SetFocus()
{
	CSkinBase* pFocus = NULL;
	if (! SetFocusSkin(&pFocus))
		return NULL;
	if (pFocus == NULL)
		return NULL;
	IUnknown* pUnknown = NULL;
	pFocus->GetInterface(&pUnknown);
	return pUnknown;
}

IUnknown* CSkinBase::GetCapture()
{
	CSkinBase* pCapture = NULL;
	if (! GetCaptureSkin(&pCapture))
		return NULL;
	if (pCapture == NULL)
		return NULL;
	IUnknown* pUnknown = NULL;
	pCapture->GetInterface(&pUnknown);
	return pUnknown;
}

IUnknown* CSkinBase::SetCapture()
{
	CSkinBase* pCapture = NULL;
	if (! SetCaptureSkin(&pCapture))
		return NULL;
	if (pCapture == NULL)
		return NULL;
	IUnknown* pUnknown = NULL;
	pCapture->GetInterface(&pUnknown);
	return pUnknown;
}

VARIANT_BOOL CSkinBase::ReleaseCapture()
{
	return ReleaseCaptureSkin() ? (VARIANT_TRUE) : (VARIANT_FALSE);
}

IUnknown* CSkinBase::CreateLayout(LPCTSTR lpzLayoutClass, LPCTSTR lpzLayoutName)
{
	CXmlCmdTarget* pObj = InternalCreateLayout(lpzLayoutClass, lpzLayoutName);

	if (pObj == NULL)
		return NULL;

	IUnknown* pUnknown = NULL;
	pObj->GetInterface(&pUnknown);

	return pUnknown;
}

IUnknown* CSkinBase::GetLayout(LPCTSTR lpzLayoutName)
{
	CXmlCmdTarget* pObj = InternalGetLayout(lpzLayoutName);

	if (pObj == NULL)
		return NULL;

	IUnknown* pUnknown = NULL;
	pObj->GetInterface(&pUnknown);

	return pUnknown;
}

VARIANT_BOOL CSkinBase::DestroyLayout(LPCTSTR lpzLayoutName)
{
	return InternalDestroyLayout(lpzLayoutName);
}

#ifdef V8_JS_ENGINE_SUPPORT

//Debug
Handle<Value> CSkinBase::V8DebugBreak(const Arguments& args)
{
	HandleScope handleScope;

#ifdef _DEBUG
	::DebugBreak();
#endif

	return handleScope.Close(v8::Undefined());
}

//Parent and Child
Handle<Value> CSkinBase::V8GetParent(const Arguments& args)
{
	HandleScope handleScope;

	CSkinBase* pParent = GetSkinParent();

	Handle<Value> object = v8::Null();
	if (pParent != NULL)
		object = pParent->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8GetChild(const Arguments& args)
{
	HandleScope handleScope;

	CSkinBase* pChild = GetSubSkin(ValueToCString(args[0]));

	Handle<Value> object = v8::Null();
	if (pChild != NULL)
		object = pChild->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8CreateChild(const Arguments& args)
{
	HandleScope handleScope;

	CSkinBase* pChild = NULL;

	const int argsLen = args.Length();
	if (argsLen >= 2) {
		CString strXmlName = ValueToCString(args[0]);
		CString strChildName = ValueToCString(args[1]);
		CString strClassName = GetEngine()->GetClassName(strXmlName);
		pChild = CreateSkinChild(strClassName, strChildName);
	}

	Handle<Value> object = v8::Null();
	if (pChild != NULL)
		object = pChild->GetV8Object();

	return handleScope.Close(object);
}

//Event
Handle<Value>  CSkinBase::V8AttachEvent(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> ret = V8InternalAttachEvent(Handle<String>::Cast(args[0]), Handle<Function>::Cast(args[1])) ? (Boolean::New(true)) : (Boolean::New(false));

	return handleScope.Close(ret);
}

Handle<Value>  CSkinBase::V8DetachEvent(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> ret = V8InternalDetachEvent(Handle<String>::Cast(args[0])) ? (Boolean::New(true)) : (Boolean::New(false));

	return handleScope.Close(ret);
}

//Timer
Handle<Value> CSkinBase::V8SetTimer(const Arguments& args)
{
	HandleScope handleScope;

	UINT_PTR timer = GetEngine()->SetTimer(Handle<Integer>::Cast(args[0])->Uint32Value(), Handle<Integer>::Cast(args[1])->Uint32Value(), this);

	Handle<Value> ret = Integer::New((int32_t)timer);

	return handleScope.Close(ret);
}

Handle<Value> CSkinBase::V8KillTimer(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> ret = (GetEngine()->KillTimer(Handle<Integer>::Cast(args[0])->Uint32Value(), this)) ? (Boolean::New(true)) : (Boolean::New(false));

	return handleScope.Close(ret);
}

//
Handle<Value> CSkinBase::V8GetActive(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();
	CSkinBase* pActive = NULL;
	if (! GetActiveSkin(&pActive))
		return handleScope.Close(object);

	if (pActive != NULL)
		object = pActive->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8SetActive(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();
	CSkinBase* pActive = NULL;
	if (! SetActiveSkin(&pActive))
		return handleScope.Close(object);

	if (pActive != NULL)
		object = pActive->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8GetFocus(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();
	CSkinBase* pFocus = NULL;
	if (! GetFocusSkin(&pFocus))
		return handleScope.Close(object);

	if (pFocus != NULL)
		object = pFocus->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8SetFocus(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();
	CSkinBase* pFocus = NULL;
	if (! SetFocusSkin(&pFocus))
		return handleScope.Close(object);

	if (pFocus != NULL)
		object = pFocus->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8GetCapture(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();
	CSkinBase* pCapture = NULL;
	if (! GetCaptureSkin(&pCapture))
		return handleScope.Close(object);

	if (pCapture != NULL)
		object = pCapture->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8SetCapture(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();
	CSkinBase* pCapture = NULL;
	if (! SetCaptureSkin(&pCapture))
		return handleScope.Close(object);

	if (pCapture != NULL)
		object = pCapture->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8ReleaseCapture(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> ret = ReleaseCaptureSkin() ? (Boolean::New(true)) : (Boolean::New(false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinBase::V8CreateLayout(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();

	CString strClass = ValueToCString(Handle<String>::Cast(args[0]));
	CString strName = ValueToCString(Handle<String>::Cast(args[1]));
	CXmlCmdTarget* pObj = InternalCreateLayout(strClass, strName);

	if (pObj != NULL)
		object = pObj->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8GetLayout(const Arguments& args)
{
	HandleScope handleScope;

	Handle<Value> object = v8::Null();

	CString strName = ValueToCString(Handle<String>::Cast(args[0]));

	CXmlCmdTarget* pObj = InternalGetLayout(strName);

	if (pObj != NULL)
		object = pObj->GetV8Object();

	return handleScope.Close(object);
}

Handle<Value> CSkinBase::V8DestroyLayout(const Arguments& args)
{
	HandleScope handleScope;

	CString strName = ValueToCString(Handle<String>::Cast(args[0]));

	Handle<Value> ret = InternalDestroyLayout(strName) ? (Boolean::New(true)) : (Boolean::New(false));

	return handleScope.Close(ret);
}

Handle<Value> CSkinBase::V8InternalCallMethod(const CString& funcName, const Arguments& args)
{
	if (funcName.CompareNoCase(_T("DebugBreak")) == 0) {
		return V8DebugBreak(args);
	} else if (funcName.CompareNoCase(_T("GetParent")) == 0) {
		return V8GetParent(args);
	} else if (funcName.CompareNoCase(_T("GetChild")) == 0) {
		return V8GetChild(args);
	} else if (funcName.CompareNoCase(_T("CreateChild")) == 0) {
		return V8CreateChild(args);
	} else if (funcName.CompareNoCase(_T("AttachEvent")) == 0) {
		return V8AttachEvent(args);
	} else if (funcName.CompareNoCase(_T("DetachEvent")) == 0) {
		return V8DetachEvent(args);
	} else if (funcName.CompareNoCase(_T("SetTimer")) == 0) {
		return V8SetTimer(args);
	} else if (funcName.CompareNoCase(_T("KillTimer")) == 0) {
		return V8KillTimer(args);
	} else if (funcName.CompareNoCase(_T("GetActive")) == 0) {
		return V8GetActive(args);
	} else if (funcName.CompareNoCase(_T("SetActive")) == 0) {
		return V8SetActive(args);
	} else if (funcName.CompareNoCase(_T("GetFocus")) == 0) {
		return V8GetFocus(args);
	} else if (funcName.CompareNoCase(_T("SetFocus")) == 0) {
		return V8SetFocus(args);
	} else if (funcName.CompareNoCase(_T("GetCapture")) == 0) {
		return V8GetCapture(args);
	} else if (funcName.CompareNoCase(_T("SetCapture")) == 0) {
		return V8SetCapture(args);
	} else if (funcName.CompareNoCase(_T("ReleaseCapture")) == 0) {
		return V8ReleaseCapture(args);
	} else if (funcName.CompareNoCase(_T("CreateLayout")) == 0) {
		return V8CreateLayout(args);
	} else if (funcName.CompareNoCase(_T("GetLayout")) == 0) {
		return V8GetLayout(args);
	} else if (funcName.CompareNoCase(_T("DestroyLayout")) == 0) {
		return V8DestroyLayout(args);
	}

	return CXmlCmdTarget::V8InternalCallMethod(funcName, args);
}

#endif //V8_JS_ENGINE_SUPPORT

BSTR CSkinBase::GetCursor()
{
	return m_strCursor.AllocSysString();
}

void CSkinBase::SetCursor(LPCTSTR newVal)
{
	if (m_strCursor == newVal)
		return;

	HCURSOR hCursor = GetCursorHelper(newVal);

	if (_tcscmp(newVal, L"") != 0 && hCursor == NULL)
		return;
	if (m_hCursor != NULL){	
		//因为Cursor创建自CreateIconIndirect，故用DestroyIcon销毁
		::DestroyIcon(m_hCursor);
		m_hCursor = NULL;
		m_strCursor = _T("");
	}
	m_hCursor = hCursor;
	m_strCursor = newVal;
}

BSTR CSkinBase::GetSysCursor()
{
	return m_strSysCursor.AllocSysString();
}

void CSkinBase::SetSysCursor(LPCTSTR newVal)
{
	if (m_strSysCursor == newVal)
		return;

	CString strCursor = newVal;
	HCURSOR hCursor = NULL;

	if (strCursor.IsEmpty()) {
		m_hSysCursor = NULL;
		m_strSysCursor = _T("");
	}

	if (strCursor.CompareNoCase(_T("ARROW")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_ARROW);
	} else if (strCursor.CompareNoCase(_T("IBEAM")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_IBEAM);
	} else if (strCursor.CompareNoCase(_T("WAIT")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_WAIT);
	} else if (strCursor.CompareNoCase(_T("CROSS")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_CROSS);
	} else if (strCursor.CompareNoCase(_T("UPARROW")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_UPARROW);
	} else if (strCursor.CompareNoCase(_T("SIZE")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_SIZE);
	} else if (strCursor.CompareNoCase(_T("ICON")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_ICON);
	} else if (strCursor.CompareNoCase(_T("SIZENWSE")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_SIZENWSE);
	} else if (strCursor.CompareNoCase(_T("SIZENESW")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_SIZENESW);
	} else if (strCursor.CompareNoCase(_T("SIZEWE")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_SIZEWE);
	} else if (strCursor.CompareNoCase(_T("SIZENS")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_SIZENS);
	} else if (strCursor.CompareNoCase(_T("SIZEALL")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
	} else if (strCursor.CompareNoCase(_T("HAND")) == 0) {
		hCursor = ::LoadCursor(NULL, IDC_HAND);
	} 

	m_hSysCursor = hCursor;
	m_strSysCursor = newVal;
}

BSTR CSkinBase::GetName()
{
	return m_strName.AllocSysString();
}

void CSkinBase::SetName(LPCTSTR newVal)
{
	ASSERT(m_strName.IsEmpty());
	if (! m_strName.IsEmpty())
		return;
	if (m_strName == newVal)
		return;
	m_strName = newVal;
}

BSTR CSkinBase::GetSkinClass()
{
	return m_strSkinClass.AllocSysString();
}

void CSkinBase::SetSkinClass(LPCTSTR newVal)
{
	if (m_strSkinClass.CompareNoCase(newVal) == 0)
		return;

	BOOL bSuc = LoadSkinClassHelper(NULL, newVal);
	ASSERT(bSuc);
	if (! bSuc)
		return;

	m_strSkinClass = newVal;
}

BSTR CSkinBase::GetPreLoad()
{
	return m_strPreLoad.AllocSysString();
}

void CSkinBase::SetPreLoad(LPCTSTR newVal)
{
	ASSERT(m_strPreLoad.IsEmpty());
	if (! m_strPreLoad.IsEmpty())
		return;

	if (m_strPreLoad.CompareNoCase(newVal) == 0)
		return;

	m_strPreLoad = newVal;
}

BSTR CSkinBase::GetText()
{
	return m_strText.AllocSysString();
}

void CSkinBase::SetText(LPCTSTR newVal)
{
	if (m_strText == newVal)
		return;
	m_strText = newVal;
	InvalidateRect();
	OnSetText();
}

LONG CSkinBase::GetLeft()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_rcSkinRect.left;
}

void CSkinBase::SetLeft(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(newVal == m_rcSkinRect.left){
		return;
	}

	CRect oldRect = m_rcSkinRect;

	//刷新旧区域
	InvalidateRect();

	LONG width = m_rcSkinRect.Width();
	m_rcSkinRect.left = newVal;
	m_rcSkinRect.right = m_rcSkinRect.left + width;

	if (! m_bSizeDelayFlag) {
		MoveWindow(NULL, FALSE);
		UpdateSkinRgn(GetWidth(), GetHeight(), TRUE);
	}

	CSkinBase::OnMove(m_rcSkinRect.left, m_rcSkinRect.top);
}

LONG CSkinBase::GetTop()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_rcSkinRect.top;
}

void CSkinBase::SetTop(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_rcSkinRect.top == newVal){
		return;
	}

	CRect oldRect = m_rcSkinRect;

	//刷新旧区域
	InvalidateRect();

	int height = m_rcSkinRect.Height();
	m_rcSkinRect.top = newVal;
	m_rcSkinRect.bottom = m_rcSkinRect.top + height;

	if (! m_bSizeDelayFlag) {
		MoveWindow(NULL, FALSE);
		UpdateSkinRgn(GetWidth(), GetHeight(), TRUE);
	}

	CSkinBase::OnMove(m_rcSkinRect.left, m_rcSkinRect.top);
}

LONG CSkinBase::GetWidth()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_rcSkinRect.Width();
}

void CSkinBase::SetWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LONG oldWidth = m_rcSkinRect.Width();
	if(newVal < 0 || oldWidth == newVal){
		return;
	}

	CRect oldRect = m_rcSkinRect;

	//刷新旧区域
	InvalidateRect();

	m_rcSkinRect.right = m_rcSkinRect.left + newVal;

	if (! m_bSizeDelayFlag) {
		MoveWindow(NULL, FALSE);
		UpdateSkinRgn(GetWidth(), GetHeight(), TRUE);
	}

	CSkinBase::OnSize(m_rcSkinRect.Width(), m_rcSkinRect.Height(), 0);
}

LONG CSkinBase::GetHeight()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_rcSkinRect.Height();
}

void CSkinBase::SetHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LONG oldHeight = m_rcSkinRect.Height();
	if(newVal < 0 || oldHeight == newVal){
		return;
	}

	CRect oldRect = m_rcSkinRect;

	//刷新旧区域
	InvalidateRect();

	m_rcSkinRect.bottom = m_rcSkinRect.top + newVal;

	if (! m_bSizeDelayFlag) {
		MoveWindow(NULL, FALSE);
		UpdateSkinRgn(GetWidth(), GetHeight(), TRUE);
	}

	CSkinBase::OnSize(m_rcSkinRect.Width(), m_rcSkinRect.Height(), 0);
}

BOOL CSkinBase::GetSizeDelay()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bSizeDelay != FALSE);
}

void CSkinBase::SetSizeDelay(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bSizeDelay){
		return;
	}
	m_bSizeDelay = newVal;
}

BOOL CSkinBase::GetVisible()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bVisible != FALSE);
}

void CSkinBase::SetVisible(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bVisible){
		return;
	}
	m_bVisible = newVal;
	OnShowWindow(m_bVisible);
	InvalidateRect();
}

BOOL CSkinBase::GetDisabled()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bDisabled != FALSE);
}

void CSkinBase::SetDisabled(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bDisabled){
		return;
	}
	m_bDisabled = newVal;
	OnEnable(!m_bDisabled);
	InvalidateRect();
}

BOOL CSkinBase::GetTabStop()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bTabStop != FALSE);
}

void CSkinBase::SetTabStop(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bTabStop){
		return;
	}
	m_bTabStop = newVal;
	InvalidateRect();
}

BOOL CSkinBase::GetGroup()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bGroup != FALSE);
}

void CSkinBase::SetGroup(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bGroup){
		return;
	}
	m_bGroup = newVal;
	InvalidateRect();
}

BOOL CSkinBase::GetBackground()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bBackground != FALSE);
}

void CSkinBase::SetBackground(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bBackground){
		return;
	}
	m_bBackground = newVal;
	InvalidateRect();
}

BOOL CSkinBase::GetMouseEvent()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bMouseEvent != FALSE);
}

void CSkinBase::SetMouseEvent(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bMouseEvent){
		return;
	}
	m_bMouseEvent = newVal;
	InvalidateRect();
}

BOOL CSkinBase::GetTransparent()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bTransparent != FALSE);
}

void CSkinBase::SetTransparent(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bTransparent){
		return;
	}
	m_bTransparent = newVal;
	InvalidateRect();
}

BOOL CSkinBase::GetColorTransform()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bColorTransform != FALSE);
}

void CSkinBase::SetColorTransform(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bColorTransform){
		return;
	}
	m_bColorTransform = newVal;
	InvalidateRect();
}

BOOL CSkinBase::GetAlpha()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (m_bAlpha != FALSE);
}

void CSkinBase::SetAlpha(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	newVal = (newVal != FALSE);
	if (newVal == m_bAlpha){
		return;
	}
	m_bAlpha = newVal;
	InvalidateRect();
}

//上边角半径
LONG CSkinBase::GetUpCornerRadius()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nUpCornerRadius;
}

//上边角半径
void CSkinBase::SetUpCornerRadius(LONG newVal)
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_nUpCornerRadius == newVal)
		return;

	m_nUpCornerRadius = newVal;
	UpdateSkinRgn(GetWidth(), GetHeight());
	InvalidateRect();
}

//下边角半径
LONG CSkinBase::GetDownCornerRadius()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nDownCornerRadius;
}

//下边角半径
void CSkinBase::SetDownCornerRadius(LONG newVal)
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_nDownCornerRadius == newVal)
		return;

	m_nDownCornerRadius = newVal;
	UpdateSkinRgn(GetWidth(), GetHeight());
	InvalidateRect();
}

//边线宽
LONG CSkinBase::GetBorderLineWidth()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nBorderLineWidth;
}

//边线宽
void CSkinBase::SetBorderLineWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_nBorderLineWidth == newVal)
		return;

	m_nBorderLineWidth = newVal;
	InvalidateRect();
}

//边线颜色
LONG CSkinBase::GetBorderLineColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_clrBorderLineColor;
}

//边线颜色
void CSkinBase::SetBorderLineColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_clrBorderLineColor == newVal)
		return;

	m_clrBorderLineColor = newVal;
	InvalidateRect();
}

//边线颜色
LONG CSkinBase::GetTransparentColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_clrTransparentColor;
}

//边线颜色
void CSkinBase::SetTransparentColor(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_clrTransparentColor == newVal)
		return;

	m_clrTransparentColor = newVal;
	InvalidateRect();
}

//边线颜色
BOOL CSkinBase::GetScriptHandleEvent()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_bScriptHandleEvent;
}

//边线颜色
void CSkinBase::SetScriptHandleEvent(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_bScriptHandleEvent == newVal)
		return;

	m_bScriptHandleEvent = newVal;
	InvalidateRect();
}

CXmlCmdTarget* CSkinBase::InternalCreateLayout(LPCTSTR lpzLayoutClass, LPCTSTR lpzLayoutName)
{
	LayoutArray::iterator iter = m_vecLayouts.begin(),
		last = m_vecLayouts.end();
	for (; iter!=last; iter++) {
		if (iter->first.CompareNoCase(lpzLayoutName) == 0)
			return iter->second;
	}

	CSkinLayout* pLayout = new CSkinLayout();
	pLayout->SetHost(this);
	m_vecLayouts.push_back(std::pair<CString, CXmlCmdTarget*>(lpzLayoutName, pLayout));

	return pLayout;
}

CXmlCmdTarget* CSkinBase::InternalGetLayout(LPCTSTR lpzLayoutName)
{
	LayoutArray::iterator iter = m_vecLayouts.begin(),
		last = m_vecLayouts.end();
	for (; iter!=last; iter++) {
		if (iter->first.CompareNoCase(lpzLayoutName) == 0)
			return iter->second;
	}

	return NULL;
}

BOOL CSkinBase::InternalDestroyLayout(LPCTSTR lpzLayoutName)
{
	LayoutArray::iterator iter = m_vecLayouts.begin(),
		last = m_vecLayouts.end();
	for (; iter!=last; iter++) {
		if (iter->first.CompareNoCase(lpzLayoutName) == 0) {
			delete iter->second;
			m_vecLayouts.erase(iter);
			return TRUE;
		}
	}

	return FALSE;
}

CString CSkinBase::GetSkinName()
{
	return m_strName;
}

CString CSkinBase::GetSkinClassName()
{
	return m_strSkinClass;
}

CString CSkinBase::GetSkinPreLoad()
{
	return m_strPreLoad;
}

//获得皮肤的类名
CString CSkinBase::GetClassName()
{
	const SKIN_DISPATCH_INFO* pSkinDispInfo = GetSkinDispatchInfo();
	return pSkinDispInfo->GetClassName();
}

CString CSkinBase::GetSkinText()
{
	return m_strText;
}

BOOL CSkinBase::GetSizeDelayFlag()
{
	return m_bSizeDelayFlag;
}

void CSkinBase::SetSizeDelayFlag(BOOL bTrue)
{
	m_bSizeDelayFlag = bTrue;
}

void CSkinBase::SetSubSkinSizeDelayFlag(BOOL bTrue)
{
	list<CSkinBase*>::iterator iter = m_listSkinBase.begin(),
		last = m_listSkinBase.end();
	if (bTrue) {
		//只对SizeDelay属性为true的对象设置SizeDelayFlag
		for (; iter!=last; iter++) {
			CSkinBase* pBase = *iter;
			if (pBase->GetSizeDelay())
				pBase->SetSizeDelayFlag(bTrue);
		}
	} else {
		//对所有的对象清除SizeDelayFlag
		for (; iter!=last; iter++) {
			CSkinBase* pBase = *iter;
			pBase->SetSizeDelayFlag(bTrue);
		}
	}
}

void CSkinBase::ApplySubSkinDelayedSize(BOOL bRepaint)
{
	list<CSkinBase*>::iterator iter = m_listSkinBase.begin(),
		last = m_listSkinBase.end();
	for (; iter!=last; iter++) {
		CSkinBase* pBase = *iter;
		if (pBase->GetSizeDelayFlag())
			pBase->ApplyDelayedSize(bRepaint);
	}
}

//skin坐标转屏幕坐标
BOOL CSkinBase::SkinToScreen(POINT& point)
{
	point.x += GetLeft();
	point.y += GetTop();

	if (! GetSkinParent()->SkinToScreen(point))
		return FALSE;

	return TRUE;
}

BOOL CSkinBase::SkinToScreen(LONG& x, LONG& y)
{
	CPoint p(x, y);
	if (! SkinToScreen(p))
		return FALSE;

	x = p.x;
	y = p.y;

	return TRUE;
}

BOOL CSkinBase::SkinToScreen(RECT& rect)
{
	::OffsetRect(&rect, GetLeft(), GetTop());

	if (! GetSkinParent()->SkinToScreen(rect))
		return FALSE;

	return TRUE;
}

//屏幕坐标转skin坐标
BOOL CSkinBase::ScreenToSkin(POINT& point)
{
	point.x -= GetLeft();
	point.y -= GetTop();

	if (! GetSkinParent()->ScreenToSkin(point))
		return FALSE;

	return TRUE;
}

BOOL CSkinBase::ScreenToSkin(LONG& x, LONG& y)
{
	CPoint p(x, y);
	if (! ScreenToSkin(p))
		return FALSE;

	x = p.x;
	y = p.y;

	return TRUE;
}

BOOL CSkinBase::ScreenToSkin(RECT& rect)
{
	::OffsetRect(&rect, -GetLeft(), -GetTop());

	if (! GetSkinParent()->ScreenToSkin(rect))
		return FALSE;

	return TRUE;
}
//skin坐标转父窗口坐标
BOOL CSkinBase::SkinToSkinParent(LONG& x, LONG& y)
{
	CRect skinRect = GetSkinRect();
	x += skinRect.left;
	y += skinRect.top;
	return (GetSkinParent() != NULL);
}

BOOL CSkinBase::SkinToSkinParent(POINT& point)
{
	return SkinToSkinParent(point.x, point.y);
}

BOOL CSkinBase::SkinToSkinParent(RECT& rect)
{
	CRect skinRect = GetSkinRect();
	rect.left += skinRect.left;
	rect.right += skinRect.left;
	rect.top += skinRect.top;
	rect.bottom += skinRect.top;
	return (GetSkinParent() != NULL);
}

//父窗口坐标skin坐标
BOOL CSkinBase::SkinParentToSkin(LONG& x, LONG& y)
{
	CRect skinRect = GetSkinRect();
	x -= skinRect.left;
	y -= skinRect.top;
	return (GetSkinParent() != NULL);
}

BOOL CSkinBase::SkinParentToSkin(POINT& point)
{
	return SkinParentToSkin(point.x, point.y);
}

BOOL CSkinBase::SkinParentToSkin(RECT& rect)
{
	CRect skinRect = GetSkinRect();
	rect.left -= skinRect.left;
	rect.right -= skinRect.left;
	rect.top -= skinRect.top;
	rect.bottom -= skinRect.top;
	return (GetSkinParent() != NULL);
}

//skin坐标转skin依附的窗口坐标
BOOL CSkinBase::SkinToSkinHwnd(LONG& x, LONG& y)
{
	POINT offset = {0, 0};
	if (! GetSkinHwndOffset(&offset))
		return FALSE;
	x += offset.x;
	y += offset.y;
	return TRUE;
}

BOOL CSkinBase::SkinToSkinHwnd(POINT& point)
{
	return SkinToSkinHwnd(point.x, point.y);
}

BOOL CSkinBase::SkinToSkinHwnd(RECT& rect)
{
	POINT offset = {0, 0};
	if (! GetSkinHwndOffset(&offset))
		return FALSE;
	rect.left += offset.x;
	rect.right += offset.x;
	rect.top += offset.y;
	rect.bottom += offset.y;
	return TRUE;
}

//skin依附的窗口坐标转skin坐标
BOOL CSkinBase::SkinHwndToSkin(LONG& x, LONG& y)
{
	POINT offset = {0, 0};
	if (! GetSkinHwndOffset(&offset))
		return FALSE;
	x -= offset.x;
	y -= offset.y;
	return TRUE;
}

BOOL CSkinBase::SkinHwndToSkin(POINT& point)
{
	return SkinHwndToSkin(point.x, point.y);
}

BOOL CSkinBase::SkinHwndToSkin(RECT& rect)
{
	POINT offset = {0, 0};
	if (! GetSkinHwndOffset(&offset))
		return FALSE;
	rect.left -= offset.x;
	rect.right -= offset.x;
	rect.top -= offset.y;
	rect.bottom -= offset.y;
	return TRUE;
}

CRect CSkinBase::GetSkinRect()
{
	return m_rcSkinRect;
}

CRect CSkinBase::GetSkinAbsRect()
{
	CRect rect = GetSkinRect();
	rect.OffsetRect(-rect.left, -rect.top);
	return rect;
}

//带相对父窗口的偏移
HRGN CSkinBase::GetSkinRgn()
{
	CRect rect = GetSkinRect();
	HRGN hSkinRgn = ::CreateRectRgnIndirect(&rect);
	if (m_hSkinRgn != NULL){
		::CombineRgn(hSkinRgn, m_hSkinRgn, NULL, RGN_COPY);
		::OffsetRgn(hSkinRgn, rect.left, rect.top);
	}
	return hSkinRgn;
}

//不带相对父窗口的偏移
HRGN CSkinBase::GetSkinAbsRgn()
{
	CRect rect = GetSkinRect();
	rect.OffsetRect(-rect.left, -rect.top);
	HRGN hSkinRgn = ::CreateRectRgnIndirect(&rect);
	if (m_hSkinRgn != NULL){
		::CombineRgn(hSkinRgn, m_hSkinRgn, NULL, RGN_COPY);
	}
	return hSkinRgn;
}

HCURSOR CSkinBase::GetSkinCursor(LPPOINT lpPoint)
{
	if (m_hCursor == NULL && ! m_strCursor.IsEmpty()){
		m_hCursor = GetCursorHelper(m_strCursor);
	}

	if (m_hCursor)
		return m_hCursor;

	if (m_hSysCursor)
		return m_hSysCursor;

	return NULL;
}

LONG CSkinBase::GetTransformedTransparentColor()
{
	if (m_clrTransparentColor == -1)
		return m_clrTransparentColor;

	LONG transformColor = m_clrTransparentColor;
	GetEngine()->TransformColor(m_clrTransparentColor, transformColor);
	return transformColor;
}

//////////////////////////////////////////////////////////////////////////
//消息响应
//////////////////////////////////////////////////////////////////////////
BOOL CSkinBase::OnSetText()
{
	return FIRE(OnSetText)();
}

BOOL CSkinBase::OnMouseMove(LONG x, LONG y, UINT flag)
{
	return FIRE(OnMouseMove)(x, y, flag);
}

BOOL CSkinBase::OnLButtonDown(LONG x, LONG y, UINT flag)
{
	return FIRE(OnLButtonDown)(x, y, flag);
}

BOOL CSkinBase::OnLButtonUp(LONG x, LONG y, UINT flag)
{
	return FIRE(OnLButtonUp)(x, y, flag);
}

BOOL CSkinBase::OnLButtonDblClk(LONG x, LONG y, UINT flag)
{
	return FIRE(OnLButtonDblClk)(x, y, flag);
}

BOOL CSkinBase::OnRButtonDown(LONG x, LONG y, UINT flag)
{
	return FIRE(OnRButtonDown)(x, y, flag);
}

BOOL CSkinBase::OnRButtonUp(LONG x, LONG y, UINT flag)
{
	return FIRE(OnRButtonUp)(x, y, flag);
}

BOOL CSkinBase::OnRButtonDblClk(LONG x, LONG y, UINT flag)
{
	return FIRE(OnRButtonDblClk)(x, y, flag);
}

BOOL CSkinBase::OnMButtonDown(LONG x, LONG y, UINT flag)
{
	return FIRE(OnMButtonDown)(x, y, flag);
}

BOOL CSkinBase::OnMButtonUp(LONG x, LONG y, UINT flag)
{
	return FIRE(OnMButtonUp)(x, y, flag);
}

BOOL CSkinBase::OnMButtonDblClk(LONG x, LONG y, UINT flag)
{
	return FIRE(OnMButtonDblClk)(x, y, flag);
}

BOOL CSkinBase::OnMouseWheel(LONG x, LONG y, UINT flag)
{
	return FIRE(OnMouseWheel)(x, y, flag);
}

BOOL CSkinBase::OnMouseHover(LONG x, LONG y, UINT flag)
{
	return FIRE(OnMouseHover)(x, y, flag);
}

BOOL CSkinBase::OnMouseEnter()
{
	return FIRE(OnMouseEnter)();
}

BOOL CSkinBase::OnMouseLeave()
{
	return FIRE(OnMouseLeave)();
}

BOOL CSkinBase::OnShowWindow(BOOL bShow)
{
	return FIRE(OnShowWindow)(bShow);
}

BOOL CSkinBase::OnMove(LONG x, LONG y)
{
	return FIRE(OnMove)(x, y);
}

BOOL CSkinBase::OnFrameSize(LONG x, LONG y, UINT flag)
{
	return FIRE(OnFrameSize)(x, y, flag);
}

BOOL CSkinBase::OnSize(LONG x, LONG y, UINT flag)
{
	return FIRE(OnSize)(x, y, flag);
}

BOOL CSkinBase::OnEnterSizeMove(BOOL bSize)
{
	return FIRE(OnEnterSizeMove)(bSize);
}

BOOL CSkinBase::OnExitSizeMove(BOOL bSize)
{
	return FIRE(OnExitSizeMove)(bSize);
}

BOOL CSkinBase::OnEnable(BOOL bEnable)
{
	return FIRE(OnEnable)(bEnable);
}

BOOL CSkinBase::OnSetFocus(CSkinBase* pOldFocus)
{
	return FIRE(OnSetFocus)(pOldFocus);
}

BOOL CSkinBase::OnKillFocus(CSkinBase* pNewFocus)
{
	return FIRE(OnKillFocus)(pNewFocus);
}

BOOL CSkinBase::OnActivate(CSkinBase* pOther, UINT flag)
{
	return FIRE(OnActivate)(pOther, flag);
}

BOOL CSkinBase::OnCaptureChanged(CSkinBase* pNewCapture)
{
	return FIRE(OnCaptureChanged)(pNewCapture);
}

BOOL CSkinBase::OnKeyDown(UINT VKCode, UINT flag)
{
	return FIRE(OnKeyDown)(VKCode, flag);
}

BOOL CSkinBase::OnKeyUp(UINT VKCode, UINT flag)
{
	return FIRE(OnKeyUp)(VKCode, flag);
}

BOOL CSkinBase::OnChar(UINT VKCode, UINT flag)
{
	return FIRE(OnChar)(VKCode, flag);
}

BOOL CSkinBase::OnTimer(UINT nIDEvent)
{
	return FIRE(OnTimer)(nIDEvent);
}

BOOL CSkinBase::OnClick(LONG iTab)
{
	return FIRE(OnClick)(iTab);
}

BOOL CSkinBase::OnSkinNotify(UINT nCode, VARIANT param)
{
	return FIRE(OnSkinNotify)(nCode, param);
}

BOOL CSkinBase::OnInitDialog(CSkinBase* pFocus)
{
	return FIRE(OnInitDialog)(pFocus);
}

BOOL CSkinBase::OnSetState(UINT uState)
{
	return FIRE(OnSetState)(uState);
}

BOOL CSkinBase::OnSetCheck(UINT uCheck)
{
	return FIRE(OnSetCheck)(uCheck);
}

BOOL CSkinBase::OnUpdateHwnd()
{
	return FIRE(OnUpdateHwnd)();
}

//////////////////////////////////////////////////////////////////////////
//脚本触发
//////////////////////////////////////////////////////////////////////////
#ifdef MS_JS_ENGINE_SUPPORT

BOOL CSkinBase::Fire_OnSetText()
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnSetText"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper0(this, ptrDisp, &varResult) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMouseMove(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMouseMove"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnLButtonDown(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnLButtonDown"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnLButtonUp(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnLButtonUp"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnLButtonDblClk(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnLButtonDblClk"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnRButtonDown(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnRButtonDown"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnRButtonUp(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnRButtonUp"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnRButtonDblClk(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnRButtonDblClk"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMButtonDown(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMButtonDown"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMButtonUp(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMButtonUp"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMButtonDblClk(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMButtonDblClk"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMouseWheel(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMouseWheel"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMouseHover(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMouseHover"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMouseEnter()
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMouseEnter"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper0(this, ptrDisp, &varResult) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMouseLeave()
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMouseLeave"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper0(this, ptrDisp, &varResult) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnShowWindow(BOOL bShow)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnShowWindow"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, bShow) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnMove(LONG x, LONG y)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnMove"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper2(this, ptrDisp, &varResult, x, y) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnEnterSizeMove(BOOL bSize)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnEnterSizeMove"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, bSize) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnExitSizeMove(BOOL bSize)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnExitSizeMove"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, bSize) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnFrameSize(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnFrameSize"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnSize(LONG x, LONG y, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnSize"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper3(this, ptrDisp, &varResult, x, y, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnEnable(BOOL bEnable)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnEnable"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, bEnable) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnSetFocus(CSkinBase* pOldFocus)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnSetFocus"));
	if (ptrDisp == NULL)
		return FALSE;

	IUnknown* pUnknown = NULL;
	if (pOldFocus != NULL)
		pOldFocus->GetInterface(&pUnknown);

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, pUnknown) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnKillFocus(CSkinBase* pNewFocus)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnKillFocus"));
	if (ptrDisp == NULL)
		return FALSE;

	IUnknown* pUnknown = NULL;
	if (pNewFocus != NULL)
		pNewFocus->GetInterface(&pUnknown);

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, pUnknown) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnActivate(CSkinBase* pOther, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnActivate"));
	if (ptrDisp == NULL)
		return FALSE;

	IUnknown* pUnknown = NULL;
	if (pOther != NULL)
		pOther->GetInterface(&pUnknown);

	CComVariant varResult;
	if (FireScriptHelper2(this, ptrDisp, &varResult, pUnknown, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnCaptureChanged(CSkinBase* pNewCapture)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnCaptureChanged"));
	if (ptrDisp == NULL)
		return FALSE;

	IUnknown* pUnknown = NULL;
	if (pNewCapture != NULL)
		pNewCapture->GetInterface(&pUnknown);

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, pUnknown) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnKeyDown(UINT VKCode, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnKeyDown"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper2(this, ptrDisp, &varResult, VKCode, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnKeyUp(UINT VKCode, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnKeyUp"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper2(this, ptrDisp, &varResult, VKCode, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnChar(UINT VKCode, UINT flag)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnChar"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper2(this, ptrDisp, &varResult, VKCode, flag) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnTimer(UINT nIDEvent)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnTimer"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, nIDEvent) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnClick(LONG iTab)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnClick"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, iTab) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnSkinNotify(UINT nCode, VARIANT param)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnSkinNotify"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper2(this, ptrDisp, &varResult, nCode, param) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnInitDialog(CSkinBase* pFocus)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnInitDialog"));
	if (ptrDisp == NULL)
		return FALSE;

	IUnknown* pUnknown = NULL;
	if (pFocus != NULL)
		pFocus->GetInterface(&pUnknown);

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, pUnknown) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnSetState(UINT uState)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnSetState"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, uState) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnSetCheck(UINT uCheck)
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnSetCheck"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper1(this, ptrDisp, &varResult, uCheck) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

BOOL CSkinBase::Fire_OnUpdateHwnd()
{
	IDispatchPtr ptrDisp = GetFunction(_T("OnUpdateHwnd"));
	if (ptrDisp == NULL)
		return FALSE;

	CComVariant varResult;
	if (FireScriptHelper0(this, ptrDisp, &varResult) != S_OK)
		return FALSE;

	return VariantToBool(varResult);
}

#endif //MS_JS_ENGINE_SUPPORT

#ifdef V8_JS_ENGINE_SUPPORT

CV8ContextFunction* CSkinBase::V8GetFunction(LPCTSTR strDispName)
{
	HandleScope handleScope;

	CString sDispName = strDispName;
	sDispName.MakeLower();	//不区分大小写
	map<CString, CV8ContextFunction*>::iterator finded = m_mapV8ContextFunction.find(sDispName);
	if (finded != m_mapV8ContextFunction.end()) {
		return finded->second;
	}

	return NULL;
}

BOOL CSkinBase::V8SetFunction(LPCTSTR strDispName, Handle<Function> object)
{
	HandleScope handleScope;

	Handle<Context> context = Context::GetCurrent();
	ASSERT(! context.IsEmpty());

	CString sDispName = strDispName;
	sDispName.MakeLower();	//不区分大小写
	//先删除原有的
	map<CString, CV8ContextFunction*>::iterator finded = m_mapV8ContextFunction.find(sDispName);
	if (finded != m_mapV8ContextFunction.end()) {
		delete (finded->second);
		m_mapV8ContextFunction.erase(sDispName);
	}
	//添加新的
	if (! object.IsEmpty()) {
		CV8ContextFunction* pContextFunc = new CV8ContextFunction(context, object);
		m_mapV8ContextFunction[sDispName] = pContextFunc;
	}

	return TRUE;
}

void CSkinBase::V8ClearAllFunction()
{
	map<CString, CV8ContextFunction*>::iterator iter = m_mapV8ContextFunction.begin(),
		last = m_mapV8ContextFunction.end();
	for (; iter!=last; iter++) {
		delete (iter->second);
	}

	m_mapV8ContextFunction.clear();
}

BOOL CSkinBase::V8InternalAttachEvent(Handle<String> strFuncName, Handle<Function> pFunc)
{
	HandleScope handleScope;

	CString strName = ValueToCString(strFuncName);

	if (strName.CompareNoCase(_T("OnNotify")) == 0) {	//别名
		return V8SetFunction(_T("OnSkinNotify"), pFunc);
	} else if (strName.CompareNoCase(_T("OnInit")) == 0) {	//别名
		return V8SetFunction(_T("OnInitDialog"), pFunc);
	}

	return V8SetFunction(strName, pFunc);
}

BOOL CSkinBase::V8InternalDetachEvent(Handle<String> strFuncName)
{
	HandleScope handleScope;

	CString strName = ValueToCString(strFuncName);

	Handle<Function> nullFunc;
	return V8SetFunction(strName, nullFunc);
}

//V8实现
BOOL CSkinBase::V8Fire_OnSetText()
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnSetText"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper0(this, pContextFunc->first->Global(), pContextFunc->second);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMouseMove(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMouseMove"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnLButtonDown(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnLButtonDown"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnLButtonUp(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnLButtonUp"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnLButtonDblClk(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnLButtonDblClk"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnRButtonDown(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnRButtonDown"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnRButtonUp(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnRButtonUp"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnRButtonDblClk(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnRButtonDblClk"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMButtonDown(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMButtonDown"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMButtonUp(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMButtonUp"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMButtonDblClk(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMButtonDblClk"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMouseWheel(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMouseWheel"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMouseHover(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMouseHover"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMouseEnter()
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMouseEnter"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper0(this, pContextFunc->first->Global(), pContextFunc->second);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMouseLeave()
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMouseLeave"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper0(this, pContextFunc->first->Global(), pContextFunc->second);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnShowWindow(BOOL bShow)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnShowWindow"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, bShow);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnMove(LONG x, LONG y)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnMove"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper2(this, pContextFunc->first->Global(), pContextFunc->second, x, y);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnEnterSizeMove(BOOL bSize)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnEnterSizeMove"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, bSize);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnExitSizeMove(BOOL bSize)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnExitSizeMove"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, bSize);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnFrameSize(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnFrameSize"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnSize(LONG x, LONG y, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnSize"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper3(this, pContextFunc->first->Global(), pContextFunc->second, x, y, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnEnable(BOOL bEnable)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnEnable"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, bEnable);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnSetFocus(CSkinBase* pOldFocus)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnSetFocus"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, pOldFocus);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnKillFocus(CSkinBase* pNewFocus)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnKillFocus"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, pNewFocus);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnActivate(CSkinBase* pOther, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnActivate"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper2(this, pContextFunc->first->Global(), pContextFunc->second, pOther, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnCaptureChanged(CSkinBase* pNewCapture)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnCaptureChanged"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, pNewCapture);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnKeyDown(UINT VKCode, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnKeyDown"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper2(this, pContextFunc->first->Global(), pContextFunc->second, VKCode, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnKeyUp(UINT VKCode, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnKeyUp"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper2(this, pContextFunc->first->Global(), pContextFunc->second, VKCode, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnChar(UINT VKCode, UINT flag)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnChar"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper2(this, pContextFunc->first->Global(), pContextFunc->second, VKCode, flag);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnTimer(UINT nIDEvent)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnTimer"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, nIDEvent);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnClick(LONG iTab)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnClick"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, iTab);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnSkinNotify(UINT nCode, VARIANT param)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnSkinNotify"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	CComVariant var(param);

	Handle<Value> valResult = V8FireScriptHelper2(this, pContextFunc->first->Global(), pContextFunc->second, nCode, VariantToValue(var));

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnInitDialog(CSkinBase* pFocus)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnInitDialog"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, pFocus);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnSetState(UINT uState)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnSetState"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, uState);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnSetCheck(UINT uCheck)
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnSetCheck"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper1(this, pContextFunc->first->Global(), pContextFunc->second, uCheck);

	return ValueToBool(valResult);
}

BOOL CSkinBase::V8Fire_OnUpdateHwnd()
{
	HandleScope handleScope;

	CV8ContextFunction* pContextFunc = V8GetFunction(_T("OnUpdateHwnd"));
	if (pContextFunc == NULL || pContextFunc->first.IsEmpty() || pContextFunc->second.IsEmpty())
		return FALSE;

	Context::Scope contextScope(pContextFunc->first);

	Handle<Value> valResult = V8FireScriptHelper0(this, pContextFunc->first->Global(), pContextFunc->second);

	return ValueToBool(valResult);
}

#endif //V8_JS_ENGINE_SUPPORT

//////////////////////////////////////////////////////////////////////////
//子元素属性函数
//////////////////////////////////////////////////////////////////////////
HRESULT CSkinBase::GetSubSkinProp(BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT* pVarValue)
{
	CSkinBase* pSubSkin = GetSubSkin(bstrSubSkinName);
	if (pSubSkin != NULL) {
		return pSubSkin->GetProperty(bstrPropName, pVarValue);
	}
	return S_FALSE;
}

HRESULT CSkinBase::SetSubSkinProp(BSTR bstrSubSkinName, BSTR bstrPropName, VARIANT varValue)
{
	CSkinBase* pSubSkin = GetSubSkin(bstrSubSkinName);
	if (pSubSkin != NULL) {
		return pSubSkin->SetProperty(bstrPropName, varValue);
	}
	return S_FALSE;
}

HRESULT CSkinBase::CallSubSkinMethod(BSTR bstrSubSkinName, BSTR bstrFuncName, VARIANT* pVarResult, DISPPARAMS* pDispParams)
{
	CSkinBase* pSubSkin = GetSubSkin(bstrSubSkinName);
	if (pSubSkin != NULL) {
		return pSubSkin->CallMethod(bstrFuncName, pVarResult, pDispParams);
	}
	return S_FALSE;
}

HRESULT CSkinBase::GetSubSkinPos(BSTR bstrSubSkinName, LONG* pLeft, LONG* pTop, LONG* pRight, LONG* pBottom)
{
	CSkinBase* pSubSkin = GetSubSkin(bstrSubSkinName);
	if (pSubSkin != NULL) {
		CRect skinRect = pSubSkin->GetSkinRect();
		*pLeft = skinRect.left;
		*pTop = skinRect.top;
		*pRight = skinRect.right;
		*pBottom = skinRect.bottom;
		return S_OK;
	}
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////////
//子元素相关函数
//////////////////////////////////////////////////////////////////////////
BOOL CSkinBase::AddSubSkin(CSkinBase* pBase)
{
	//验证该元素是否已经存在
	list<CSkinBase*>::iterator finded = ::find(m_listSkinBase.begin(), m_listSkinBase.end(), pBase);
	ASSERT(finded == m_listSkinBase.end());
	if (finded != m_listSkinBase.end())
		return FALSE;

	//验证同名元素是否已经存在
	CString strParentClass = GetClassName();
	CString strParentName = GetSkinName();
	CString strSubSkinClass = pBase->GetClassName();
	CString strSubSkinName = pBase->GetSkinName();

	CSkinBase* pExist = GetSubSkin(strSubSkinName);
	if (pExist != NULL) {
		USES_CONVERSION;
		TRACE(("Add Skin Error(Skin Name Already Exist!): ParentClass:%s ParentName:%s SubSkinClass:%s SubSkinName:%s \n"), T2A(strParentClass), 
			T2A(strParentName), T2A(strSubSkinClass), T2A(strSubSkinName));
		ASSERT(0 && _T("同名皮肤已经存在"));
		return FALSE;
	}

	//验证通过
	CSkinBase* pBack = NULL;
	if (m_listSkinBase.size() > 0)
		pBack = m_listSkinBase.back();
	if (pBack != NULL)
		pBack->SetSkinNext(pBase);
	pBase->SetSkinParent(this);
	pBase->SetSkinPrev(pBack);
	pBase->SetSkinNext(NULL);
	m_listSkinBase.push_back(pBase);

	USES_CONVERSION;
	TRACE(("Add Skin: ParentClass:%s ParentName:%s SubSkinClass:%s SubSkinName:%s \n"), T2A(strParentClass), 
		T2A(strParentName), T2A(strSubSkinClass), T2A(strSubSkinName));

	return TRUE;
}

BOOL CSkinBase::RemoveSubSkin(CSkinBase* pBase)
{
	list<CSkinBase*>::iterator finded = ::find(m_listSkinBase.begin(), m_listSkinBase.end(), pBase);
	ASSERT(finded != m_listSkinBase.end());
	if (finded == m_listSkinBase.end())
		return FALSE;

	CSkinBase* pPrev = NULL;
	if (finded != m_listSkinBase.begin()) {
		list<CSkinBase*>::iterator prev = finded;
		pPrev = *(--prev);
	}
	if (pPrev != NULL)
		pPrev->SetSkinNext(pBase->GetSkinNext());
	pBase->SetSkinParent(NULL);
	pBase->SetSkinNext(NULL);
	pBase->SetSkinPrev(NULL);

	m_listSkinBase.erase(finded);

#ifdef _DEBUG
	CString strParentClass = GetClassName();
	CString strParentName = GetSkinName();
	CString strSubSkinClass = pBase->GetClassName();
	CString strSubSkinName = pBase->GetSkinName();
	USES_CONVERSION;
	TRACE(("Remove Skin: ParentClass:%s ParentName:%s SubSkinClass:%s SubSkinName:%s \n"), T2A(strParentClass), 
		T2A(strParentName), T2A(strSubSkinClass), T2A(strSubSkinName));
#endif

	return TRUE;
}

BOOL CSkinBase::IsSubSkin(CSkinBase* pSkin) const
{
	list<CSkinBase*>::const_iterator finded = ::find(m_listSkinBase.begin(), m_listSkinBase.end(), pSkin);
	return (finded != m_listSkinBase.end());
}

//是否是子窗口
BOOL CSkinBase::IsChildOf(CSkinBase* pParent)
{
	return (GetSkinParent() == pParent);
}

//是否是子孙窗口
BOOL CSkinBase::IsDescendantOf(CSkinBase* pAncestor)
{
	CSkinBase* pAscendant = this;
	while (pAscendant != NULL
		&& pAscendant != pAncestor) {
			pAscendant = pAscendant->GetSkinParent();
	}
	return (pAscendant == pAncestor);
}

//是否是亲属窗口，即有共同的祖先窗口
BOOL CSkinBase::IsRelativeOf(CSkinBase* pRelative)
{
	CSkinBase* pAncestor = NULL;
	if (pRelative != NULL)
		pRelative->GetSkinAncestor();
	CSkinBase* pThisAncestor = GetSkinAncestor();
	return (pAncestor == pThisAncestor);
}

BOOL CSkinBase::ReplaceSubSkin(CSkinBase* pOldSkin, CSkinBase* pNewSkin)
{
	list<CSkinBase*>::iterator finded = ::find(m_listSkinBase.begin(), m_listSkinBase.end(), pOldSkin);
	if (finded == m_listSkinBase.end() || 
		pNewSkin == NULL)
		return FALSE;

	CSkinBase* pPrev = pOldSkin->GetSkinPrev();
	CSkinBase* pNext = pOldSkin->GetSkinNext();

	CSkinBase* pCheckPrev = NULL;
	list<CSkinBase*>::iterator iterPrev = finded;
	if (iterPrev != m_listSkinBase.begin()) {
		pCheckPrev = *(-- iterPrev);
	}
	ASSERT(pPrev == pCheckPrev);

	CSkinBase* pCheckNext = NULL;
	list<CSkinBase*>::iterator iterNext = finded;
	iterNext ++;
	if (iterNext != m_listSkinBase.end()) {
		pCheckNext = *(iterNext);
	}
	ASSERT(pNext == pCheckNext);

	if (pPrev != NULL)
		pPrev->SetSkinNext(pNewSkin);

	pNewSkin->SetSkinParent(this);
	pNewSkin->SetSkinPrev(pPrev);
	pNewSkin->SetSkinNext(pNext);
	(*finded) = pNewSkin;

	if (pNext != NULL)
		pNext->SetSkinPrev(pNewSkin);

	pOldSkin->SetSkinParent(NULL);
	pOldSkin->SetSkinNext(NULL);
	pOldSkin->SetSkinPrev(NULL);

	return TRUE;
}

CSkinBase* CSkinBase::GetSubSkin(LPCTSTR strSubSkinName)
{
	static const CString strWindow = _T("Window");

	if (strWindow.CompareNoCase(strSubSkinName) == 0)
		return this;

	list<CSkinBase*>::iterator iter = m_listSkinBase.begin(), 
		last = m_listSkinBase.end();
	for (; iter!=last; iter++) {
		CSkinBase* pBase = (*iter);
		if (pBase->m_strName.CompareNoCase(strSubSkinName) == 0)
			return pBase;
	}

	return NULL;
}

CSkinBase* CSkinBase::GetSubSkin(BSTR bstrSubSkinName)
{
	CString strName(bstrSubSkinName);

	return GetSubSkin(strName);
}

CSkinBase* CSkinBase::GetFirstSubSkin(UINT uFilterBits, UINT uFilterValues, BOOL bForward)
{
	if (bForward) {
		list<CSkinBase*>::iterator iter = m_listSkinBase.begin(), last = m_listSkinBase.end();
		for (; iter!=last; iter++) {
			CSkinBase* pBase = (*iter);
			if (pBase->IsFiltered(uFilterBits, uFilterValues))
				return pBase;
		}
		return NULL;
	} else {
		list<CSkinBase*>::reverse_iterator iter = m_listSkinBase.rbegin(), last = m_listSkinBase.rend();
		for (; iter!=last; iter++) {
			CSkinBase* pBase = (*iter);
			if (pBase->IsFiltered(uFilterBits, uFilterValues))
				return pBase;
		}
		return NULL;
	}
}

//获得兄弟元素
CSkinBase* CSkinBase::GetNextSkin(UINT uFilterBits, UINT uFilterValues, BOOL bForward)
{
	if (bForward) {
		CSkinBase* pNext = GetSkinNext();
		while (pNext != NULL &&
			! pNext->IsFiltered(uFilterBits, uFilterValues)) {
				pNext = pNext->GetSkinNext();
		}
		ASSERT(pNext == NULL || pNext->GetSkinParent() == GetSkinParent());
		return pNext;
	} else {
		CSkinBase* pPrev = GetSkinPrev();
		while (pPrev != NULL &&
			! pPrev->IsFiltered(uFilterBits, uFilterValues)) {
				pPrev = pPrev->GetSkinPrev();
		}
		ASSERT(pPrev == NULL || pPrev->GetSkinParent() == GetSkinParent());
		return pPrev;
	}
}

BOOL CSkinBase::WalkSubSkinTree(CSkinBase* pRoot, CSkinBase*& pFind, 
								UINT uFilterBits, UINT uFilterValues, 
								UINT uNodeFilterBits, UINT uNodeFilterValues, 
								BOOL bForward)
{
	if (pRoot == NULL)
		return FALSE;

	if (! pRoot->IsFiltered(uNodeFilterBits, uNodeFilterValues))
		return FALSE;

	CSkinBase* pNextSubSkin = pRoot->GetFirstSubSkin(0, 0, bForward);
	while (pNextSubSkin != NULL) {
		if (WalkSubSkinTree(pNextSubSkin, pFind, 
			uFilterBits, uFilterValues, 
			uNodeFilterBits, uNodeFilterValues, 
			bForward))
			return TRUE;
		pNextSubSkin = pNextSubSkin->GetNextSkin(0, 0, bForward);
	}

	if (pRoot->IsFiltered(uFilterBits, uFilterValues)) {
		pFind = pRoot;
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CSkinBase::WalkSkinTree(CSkinBase* pStart, CSkinBase*& pFind,
							 UINT uFilterBits, UINT uFilterValues, 
							 UINT uNodeFilterBits, UINT uNodeFilterValues, 
							 BOOL bForward)
{
	if (pStart == NULL)
		return FALSE;

	CSkinBase* pNext = pStart->GetNextSkin(0, 0, bForward);
	while (pNext != NULL) {
		if (WalkSubSkinTree(pNext, pFind, 
			uFilterBits, uFilterValues, 
			uNodeFilterBits, uNodeFilterValues, 
			bForward))
			return TRUE;
		pNext = pNext->GetNextSkin(0, 0, bForward);
	}

	pStart = pStart->GetSkinParent();
	if (pStart == NULL)
		return FALSE;

	if (pStart->IsFiltered(uFilterBits, uFilterValues)) {
		pFind = pStart;
		return TRUE;
	} else {
		if (pStart->IsFiltered(uNodeFilterBits, uNodeFilterValues)) {
			return WalkSkinTree(pStart, pFind, 
				uFilterBits, uFilterValues,
				uNodeFilterBits, uNodeFilterValues, 
				bForward);
		} else {
			return FALSE;
		}
	}
}

CSkinBase* CSkinBase::GetNextTabStopSkinInSkinTree(BOOL bForward)
{
	CSkinBase* pNext = NULL;
	CSkinBase* pAncestor = GetSkinAncestor();
	CSkinBase::WalkSkinTree(this, pNext, 
		FILTER_TABSTOP | FILTER_VISIBLE, FILTER_TABSTOP | FILTER_VISIBLE,
		FILTER_VISIBLE, FILTER_VISIBLE,
		bForward
		);
	if (pNext != NULL &&
		! this->IsFiltered(FILTER_TABSTOP, FILTER_TABSTOP)) {
			CSkinBase::WalkSkinTree(pNext, pNext, 
				FILTER_TABSTOP | FILTER_VISIBLE, FILTER_TABSTOP | FILTER_VISIBLE,
				FILTER_VISIBLE, FILTER_VISIBLE,
				bForward
				);
	}
	if (pNext == pAncestor || (pNext == NULL && pAncestor != NULL)) {
		CSkinBase::WalkSubSkinTree(pAncestor, pNext,
			FILTER_TABSTOP | FILTER_VISIBLE, FILTER_TABSTOP | FILTER_VISIBLE,
			FILTER_VISIBLE, FILTER_VISIBLE,
			bForward
			);
	}
	return pNext;
}

CSkinBase* CSkinBase::GetPointInSubSkin(LONG x, LONG y, BOOL bReverse, UINT uFilterBits, UINT uFilterValues)
{
	if (bReverse) 
	{
		list<CSkinBase*>::const_reverse_iterator iter = m_listSkinBase.rbegin(), last = m_listSkinBase.rend();
		for (; iter!=last; iter++) {
			CSkinBase* pBase = (*iter);
			if (pBase != NULL && 
				pBase->IsFiltered(uFilterBits, uFilterValues) &&
				pBase->IsPointInSkinRgn(x, y)
				) {
					return pBase;
			}
		}

		return NULL;
	}
	else 
	{
		list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(), last = m_listSkinBase.end();
		for (; iter!=last; iter++) {
			CSkinBase* pBase = (*iter);
			if (pBase != NULL && 
				pBase->IsFiltered(uFilterBits, uFilterValues) &&
				pBase->IsPointInSkinRgn(x, y)
				) {
					return pBase;
			}
		}

		return NULL;
	}
}

CSkinBase* CSkinBase::GetPointInSubSkin(POINT pt, BOOL bReverse, UINT uFilterBits, UINT uFilterValues)
{
	return GetPointInSubSkin(pt.x, pt.y, bReverse, uFilterBits, uFilterValues);
}

BOOL CSkinBase::IsFiltered(UINT uFilterBits, UINT uFilterValues)
{
	BOOL bFilterValue = TRUE;
	BOOL bValue = FALSE;
	if (uFilterBits&FILTER_SKINITEM) {
		bFilterValue = ( (uFilterValues&FILTER_SKINITEM) != 0 );
		bValue = ( IsKindOf(RUNTIME_CLASS(CSkinItem)) != FALSE );
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_VISIBLE) {
		bFilterValue = ( (uFilterValues&FILTER_VISIBLE) != 0 );
		bValue = (GetVisible() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_DISABLED) {
		bFilterValue = ( (uFilterValues&FILTER_DISABLED) != 0 );
		bValue = (GetDisabled() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_TABSTOP) {
		bFilterValue = ( (uFilterValues&FILTER_TABSTOP) != 0 );
		bValue = (GetTabStop() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_GROUP) {
		bFilterValue = ( (uFilterValues&FILTER_GROUP) != 0 );
		bValue = (GetGroup() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_MOUSEEVENT) {
		bFilterValue = ( (uFilterValues&FILTER_MOUSEEVENT) != 0 );
		bValue = (GetMouseEvent() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_BACKGROUND) {
		bFilterValue = ( (uFilterValues&FILTER_BACKGROUND) != 0 );
		bValue = (GetBackground() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}
	if (uFilterBits&FILTER_TRANSPARENT) {
		bFilterValue = ( (uFilterValues&FILTER_TRANSPARENT) != 0 );
		bValue = (GetTransparent() != FALSE);
		if (bFilterValue^bValue)
			return FALSE;
	}

	return TRUE;
}

HRGN CSkinBase::GetAllSubSkinRgn(UINT uFilterBits, UINT uFilterValues)
{
	HRGN hAllItemRgn = ::CreateRectRgnIndirect(CRect(0, 0, 0, 0));
	list<CSkinBase*>::const_iterator iter = m_listSkinBase.begin(), last = m_listSkinBase.end();
	for (; iter!=last; iter++) {

		CSkinBase* pBase = *iter;

		//Filter
		if (! pBase->IsFiltered(uFilterBits, uFilterValues))
			continue;

		HRGN tempRgn = pBase->GetSkinRgn();
		::CombineRgn(hAllItemRgn, hAllItemRgn, tempRgn, RGN_OR);
		::DeleteObject(tempRgn);
	}
	if(hAllItemRgn == NULL)
	{
		assert(0);
	}
	return hAllItemRgn;
}

BOOL CSkinBase::GetActiveSkin(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->GetActiveSubSkin(NULL, ppSkin);
}

BOOL CSkinBase::SetActiveSkin(CSkinBase** ppPrevSkin)
{
	if (ppPrevSkin != NULL)
		*ppPrevSkin = NULL;

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->SetActiveSubSkin(this, ppPrevSkin);
}

BOOL CSkinBase::GetFocusSkin(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->GetFocusSubSkin(NULL, ppSkin);
}

BOOL CSkinBase::SetFocusSkin(CSkinBase** ppPrevSkin)
{
	if (ppPrevSkin != NULL)
		*ppPrevSkin = NULL;

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->SetFocusSubSkin(this, ppPrevSkin);
}

BOOL CSkinBase::GetCaptureSkin(CSkinBase** ppSkin)
{
	if (ppSkin != NULL)
		*ppSkin = NULL;

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->GetCaptureSubSkin(NULL, ppSkin);
}

BOOL CSkinBase::SetCaptureSkin(CSkinBase** ppPrevSkin)
{
	if (ppPrevSkin != NULL)
		*ppPrevSkin = NULL;

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->SetCaptureSubSkin(this, ppPrevSkin);
}

BOOL CSkinBase::ReleaseCaptureSkin()
{
	HWND hSkinWnd = GetSkinHwnd();

	CSkinItem* pItem = GetSkinHwndItem();
	if (pItem == NULL)
		return FALSE;

	return pItem->ReleaseCaptureSubSkin(this);
}
