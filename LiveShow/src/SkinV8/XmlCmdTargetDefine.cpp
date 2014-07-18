#include "StdAfx.h"
#include "XmlCmdTargetDefine.h"
#include "XmlCmdTarget.h"

#ifdef V8_JS_ENGINE_SUPPORT
using namespace v8;
#endif //V8_JS_ENGINE_SUPPORT

SKIN_DISPATCH_INFO* SKIN_DISPATCH_INFO::s_pSkinDispatchInfoListHeader = NULL;

SKIN_DISPATCH_INFO::SKIN_DISPATCH_INFO(LPCTSTR pzClassName, const AFX_DISPMAP_ENTRY* pDispatchEntry, const SKIN_DISPATCH_INFO* pBaseDispInfo)
:m_pzClassName(pzClassName),
m_pDispEntry(pDispatchEntry),
m_pNextSkinDispatchInfo(s_pSkinDispatchInfoListHeader), //添加链表节点
m_pBaseSkinDispatchInfo(pBaseDispInfo),
m_lMinDispid(-1), 
m_lMaxDispid(-1), 
m_lDispSize(0)
{
	//设置链表头节点
	s_pSkinDispatchInfoListHeader = this;

#ifdef V8_JS_ENGINE_SUPPORT
    HandleScope handleScope;

    m_functionTemplate.Dispose();
    m_functionTemplate.Clear();
#endif

	InitializeSkinDispatchInfo();
};

SKIN_DISPATCH_INFO::~SKIN_DISPATCH_INFO()
{
#ifdef V8_JS_ENGINE_SUPPORT
    HandleScope handleScope;

    m_functionTemplate.Dispose();
    m_functionTemplate.Clear();
#endif
}

//获得类名
LPCTSTR SKIN_DISPATCH_INFO::GetClassName() const
{
    return m_pzClassName;
}

const SKIN_DISPATCH_INFO* SKIN_DISPATCH_INFO::GetSkinDispatchInfoHeader()
{
    return s_pSkinDispatchInfoListHeader;
}

const SKIN_DISPATCH_INFO* SKIN_DISPATCH_INFO::GetNextSkinDispatchInfo() const
{
    return m_pNextSkinDispatchInfo;
}

const SKIN_DISPATCH_INFO* SKIN_DISPATCH_INFO::GetBaseSkinDispatchInfo() const
{
    return m_pBaseSkinDispatchInfo;
}

//获得DispatchMap Size 相关信息
BOOL SKIN_DISPATCH_INFO::GetDispatchMapSize(LONG& size, LONG& minId, LONG& maxId) const
{
	size = m_lDispSize;
	minId = m_lMinDispid;
	maxId = m_lMaxDispid;
	return TRUE;
}

//检查Disp Name是否有重复定义
BOOL SKIN_DISPATCH_INFO::CheckRedefinedDispNameOrAlias(std::set<CString>* pSetOfRedefined) const
{
	const AFX_DISPMAP_ENTRY* pDispMapEntry = m_pDispEntry;
	if (pDispMapEntry == NULL)
		return TRUE;

	BOOL bNoRedefined = TRUE;
	std::set<CString> checkSet;
	CString strCheck;
	for (; pDispMapEntry->lpszName != VTS_NONE; pDispMapEntry++) {

		strCheck = pDispMapEntry->lpszName;
		strCheck.MakeLower();		//不区分大小写
		if (checkSet.find(strCheck) != checkSet.end()) {
			bNoRedefined = FALSE;
			if (pSetOfRedefined == NULL) {
				return bNoRedefined;
			} else {
				pSetOfRedefined->insert(strCheck);
			}
		} else {
			checkSet.insert(strCheck);
		}

	}
	return bNoRedefined;
}

//检查Disp ID是否有重复定义，即同一个Disp ID被不同的属性或方法使用，别名除外
BOOL SKIN_DISPATCH_INFO::CheckRedefinedDispID(std::set<int>* pSetOfRedefined) const
{
	const AFX_DISPMAP_ENTRY* pDispMapEntry = m_pDispEntry;
	if (pDispMapEntry == NULL)
		return TRUE;

	BOOL bNoRedefined = TRUE;
	std::map<int, const AFX_DISPMAP_ENTRY*> checkMap;
	int idCheck;
	for (; pDispMapEntry->lpszName != VTS_NONE; pDispMapEntry++) {

		idCheck = pDispMapEntry->lDispID;
		std::map<int, const AFX_DISPMAP_ENTRY*>::const_iterator finded = checkMap.find(idCheck);
		if (finded != checkMap.end()) {
			if (finded->second->pfn != pDispMapEntry->pfn 
				|| finded->second->pfnSet != pDispMapEntry->pfnSet) {
				bNoRedefined = FALSE;
				if (pSetOfRedefined == NULL) {
					return bNoRedefined;
				} else {
					pSetOfRedefined->insert(idCheck);
				}
			}
		} else {
			checkMap.insert(std::pair<int, const AFX_DISPMAP_ENTRY*>(idCheck, pDispMapEntry));
		}

	}
	return bNoRedefined;
}

BOOL SKIN_DISPATCH_INFO::InitializeSkinDispatchInfo()
{
	const AFX_DISPMAP_ENTRY* pDispMapEntry = m_pDispEntry;
	if (pDispMapEntry == NULL)
		return FALSE;

	ASSERT(CheckRedefinedDispNameOrAlias());
	ASSERT(CheckRedefinedDispID());

	//获得最大Dispid，最小Dispid，总Dispid数目
	if (pDispMapEntry->lpszName != VTS_NONE) {
		m_lMinDispid = pDispMapEntry->lDispID;
		m_lMaxDispid = pDispMapEntry->lDispID;
		pDispMapEntry ++;
		m_lDispSize ++;
	}
	for (; pDispMapEntry->lpszName != VTS_NONE; pDispMapEntry++) {

		if (pDispMapEntry->lDispID < m_lMinDispid)
			m_lMinDispid = pDispMapEntry->lDispID;

		if (pDispMapEntry->lDispID > m_lMaxDispid)
			m_lMaxDispid = pDispMapEntry->lDispID;

		m_lDispSize ++;
	}

	pDispMapEntry = m_pDispEntry;
	LONG dispID = -1;
	CString strDispName;
	for (; pDispMapEntry->lpszName != VTS_NONE; pDispMapEntry++) {

		dispID = pDispMapEntry->lDispID;

		strDispName = pDispMapEntry->lpszName;
		strDispName.MakeLower(); //不区分大小写

		//第一步
		//添加 名字(或别名)到ID映射表
		std::map<CString, LONG>::const_iterator finded1 = m_mapNameOrAliasToID.find(strDispName);
		ASSERT(finded1 == m_mapNameOrAliasToID.end());
		if (finded1 == m_mapNameOrAliasToID.end()) {
			m_mapNameOrAliasToID.insert(std::pair<CString, LONG>(strDispName, dispID));
		}

		//第二步
		//添加 名字的别名表，如果名字的对应的Dispid在DispatchMap前面出现过，那么这个名字就是别名
		//通过查找m_mapIDToEntry来判断是否出现过
		std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator finded2 = m_mapIDToEntry.find(dispID);
		CString strTemp;
		if (finded2 != m_mapIDToEntry.end()) {
			strTemp = finded2->second->lpszName;
			strTemp.MakeLower();
			std::map<CString, std::list<CString>>::iterator finded = m_mapNameAlias.find(strTemp);
			if (finded != m_mapNameAlias.end()) {
				finded->second.push_back(strDispName);
			} else {
				std::list<CString> toInsert;
				toInsert.push_back(strDispName);
				m_mapNameAlias.insert(std::pair<CString, std::list<CString>>(strTemp, toInsert));
			}
		}

		//第三步
		//添加 计算ID到名字映射表，注意此表中的名字不是别名
		std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator finded3 = m_mapIDToEntry.find(dispID);
		if (finded3 == m_mapIDToEntry.end()) {
			m_mapIDToEntry.insert(std::pair<LONG, const AFX_DISPMAP_ENTRY*>(dispID, pDispMapEntry));
		}

	}

#ifdef V8_JS_ENGINE_SUPPORT

    if (m_pDispEntry != NULL) {

        HandleScope handle_scope;

        const AFX_DISPMAP_ENTRY* pDispMapEntry = m_pDispEntry;

        Handle<FunctionTemplate> templ;

        templ = FunctionTemplate::New();
 
        //设置模板名
#ifdef UNICODE
        templ->SetClassName(String::New((uint16_t*)m_pzClassName));
#else
        templ->SetClassName(String::New(m_pzClassName));
#endif

        //获得类模板
        Handle<ObjectTemplate> proto = templ->PrototypeTemplate();

        //绑定方法
        //遍历
        pDispMapEntry = m_pDispEntry;
        for (; pDispMapEntry->lpszName != VTS_NONE; pDispMapEntry++) {

            if (pDispMapEntry->pfn != NULL && pDispMapEntry->pfnSet == NULL) {
#ifdef UNICODE
                proto->Set(String::New((uint16_t*)pDispMapEntry->lpszName), FunctionTemplate::New(CXmlCmdTarget::V8CallMethod));
#else
                proto->Set(pDispMapEntry->lpszName, FunctionTemplate::New(CXmlCmdTarget::V8CallMethod));
#endif
            }

        }

        //绑定属性
        Handle<ObjectTemplate> inst = templ->InstanceTemplate();
        inst->SetInternalFieldCount(1);
        //遍历
        pDispMapEntry = m_pDispEntry;
        for (; pDispMapEntry->lpszName != VTS_NONE; pDispMapEntry++) {

            if (pDispMapEntry->lpszParams == NULL && pDispMapEntry->pfn != NULL && pDispMapEntry->pfnSet != NULL) {
#ifdef UNICODE
                proto->SetAccessor(String::New((uint16_t*)pDispMapEntry->lpszName), CXmlCmdTarget::V8GetProperty, CXmlCmdTarget::V8SetProperty);
#else
                proto->SetAccessor(pDispMapEntry->lpszName, CXmlCmdTarget::V8GetProperty, CXmlCmdTarget::V8SetProperty);
#endif
            }

        }

        m_functionTemplate.Dispose();
        m_functionTemplate.Clear();
        m_functionTemplate = Persistent<FunctionTemplate>::New(templ);
    }

#endif //V8_JS_ENGINE_SUPPORT

	return TRUE;
}

//是否是同一DispatchInfo
BOOL SKIN_DISPATCH_INFO::IsSameAs(const SKIN_DISPATCH_INFO& other) const
{
	return (this == &other);
}

//是否是属性
BOOL SKIN_DISPATCH_INFO::IsDispProperty(LONG dispId) const
{
	const AFX_DISPMAP_ENTRY* pInfo = GetDispEntryByDispId(dispId);
	if (pInfo == NULL)
		return FALSE;

	return (pInfo->lpszParams == NULL && pInfo->pfn != NULL && pInfo->pfnSet != NULL);
}

BOOL SKIN_DISPATCH_INFO::IsDispProperty(LPCTSTR dispNameOrAlias) const
{
	const AFX_DISPMAP_ENTRY* pInfo = GetDispEntryByDispNameOrAlias(dispNameOrAlias);
	if (pInfo == NULL)
		return FALSE;

	return (pInfo->lpszParams == NULL && pInfo->pfn != NULL && pInfo->pfnSet != NULL);
}

//是否是方法
BOOL SKIN_DISPATCH_INFO::IsDispMethod(LONG dispId) const
{
	const AFX_DISPMAP_ENTRY* pInfo = GetDispEntryByDispId(dispId);
	if (pInfo == NULL)
		return FALSE;

	return (pInfo->pfn != NULL && pInfo->pfnSet == NULL);
}

BOOL SKIN_DISPATCH_INFO::IsDispMethod(LPCTSTR dispNameOrAlias) const
{
	const AFX_DISPMAP_ENTRY* pInfo = GetDispEntryByDispNameOrAlias(dispNameOrAlias);
	if (pInfo == NULL)
		return FALSE;

	return (pInfo->pfn != NULL && pInfo->pfnSet == NULL);
}

//判断是否是名字
BOOL SKIN_DISPATCH_INFO::IsDispName(LPCTSTR pzDispName) const
{
	CString strDispName = pzDispName;
	strDispName.MakeLower();

	std::map<CString, LONG>::const_iterator finded = m_mapNameOrAliasToID.find(strDispName);
	if (finded == m_mapNameOrAliasToID.end())
		return FALSE;

	std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator finded2 = m_mapIDToEntry.find(finded->second);
	ASSERT(finded2 != m_mapIDToEntry.end());
	if (finded2 == m_mapIDToEntry.end())
		return FALSE;

	return (strDispName.CompareNoCase(finded2->second->lpszName) == 0);
}

//判断是否是名字或别名
BOOL SKIN_DISPATCH_INFO::IsDispNameOrAlias(LPCTSTR pzDispNameOrAlias) const
{
	CString strDispNameOrAlias = pzDispNameOrAlias;
	strDispNameOrAlias.MakeLower();

	std::map<CString, LONG>::const_iterator finded = m_mapNameOrAliasToID.find(strDispNameOrAlias);
	return (finded != m_mapNameOrAliasToID.end());
}

const AFX_DISPMAP_ENTRY* SKIN_DISPATCH_INFO::GetDispEntryByDispId(LONG dispId) const
{
	std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator finded = m_mapIDToEntry.find(dispId);
	if (finded == m_mapIDToEntry.end())
		return NULL;

	return finded->second;
}

const AFX_DISPMAP_ENTRY* SKIN_DISPATCH_INFO::GetDispEntryByDispNameOrAlias(LPCTSTR pzDispNameOrAlias) const
{
	LONG dispId;
	if (! GetDispIdByDispNameOrAlias(pzDispNameOrAlias, dispId))
		return NULL;

	return GetDispEntryByDispId(dispId);
}

const AFX_DISPMAP_ENTRY* SKIN_DISPATCH_INFO::GetDispEntry() const
{
    return m_pDispEntry;
}

//获得名字
BOOL SKIN_DISPATCH_INFO::GetDispNameByDispId(LONG dispId, CString& strName) const
{
	std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator finded = m_mapIDToEntry.find(dispId);
	if (finded == m_mapIDToEntry.end())
		return FALSE;

	strName = finded->second->lpszName;
	strName.MakeLower();
	return TRUE;
}

BOOL SKIN_DISPATCH_INFO::GetDispNameByDispNameOrAlias(LPCTSTR pzDispNameOrAlias, CString& strName) const
{
	CString strDispNameOrAlias = pzDispNameOrAlias;
	strDispNameOrAlias.MakeLower();

	std::map<CString, LONG>::const_iterator finded = m_mapNameOrAliasToID.find(strDispNameOrAlias);
	if (finded == m_mapNameOrAliasToID.end())
		return FALSE;

	std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator finded2 = m_mapIDToEntry.find(finded->second);
	ASSERT(finded2 != m_mapIDToEntry.end());
	if (finded2 == m_mapIDToEntry.end())
		return FALSE;

	strName = finded2->second->lpszName;
	strName.MakeLower();
	return TRUE;
}

//获得Id
BOOL SKIN_DISPATCH_INFO::GetDispIdByDispNameOrAlias(LPCTSTR pzDispNameOrAlias, LONG& dispId) const
{
	CString strDispNameOrAlias = pzDispNameOrAlias;
	strDispNameOrAlias.MakeLower();

	std::map<CString, LONG>::const_iterator finded = m_mapNameOrAliasToID.find(strDispNameOrAlias);
	if (finded == m_mapNameOrAliasToID.end())
		return FALSE;

	dispId = finded->second;
	return TRUE;
}

//获得别名
BOOL SKIN_DISPATCH_INFO::GetDispAliasByDispId(LONG dispId, std::list<CString>& listOfAlias) const
{
	CString strDispName;
	if (! GetDispNameByDispId(dispId, strDispName))
		return FALSE;

	std::map<CString, std::list<CString>>::const_iterator finded =  m_mapNameAlias.find(strDispName);
	if (finded == m_mapNameAlias.end())
		return FALSE;

	listOfAlias = finded->second;
	return TRUE;
}

BOOL SKIN_DISPATCH_INFO::GetDispAliasByDispNameOrAlias(LPCTSTR pzDispNameOrAlias, std::list<CString>& listOfAlias) const
{
	CString strDispName;
	if (! GetDispNameByDispNameOrAlias(pzDispNameOrAlias, strDispName))
		return FALSE;

	std::map<CString, std::list<CString>>::const_iterator finded =  m_mapNameAlias.find(strDispName);
	if (finded == m_mapNameAlias.end())
		return FALSE;

	listOfAlias = finded->second;
	return TRUE;
}

//获得Id列表，无重复
BOOL SKIN_DISPATCH_INFO::GetAllDispId(std::list<LONG>& listOfIds, BOOL bAllProp) const
{
	listOfIds.clear();
	std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator iter = m_mapIDToEntry.begin(),
		last = m_mapIDToEntry.end();
	for (; iter!=last; iter++) {
		if (bAllProp && !IsDispProperty(iter->first))
			continue;
		listOfIds.push_back(iter->first);
	}
	return TRUE;
}

//获得名字列表
BOOL SKIN_DISPATCH_INFO::GetAllDispName(std::list<CString>& listOfNames, BOOL bAllProp) const
{
	listOfNames.clear();
	std::map<LONG, const AFX_DISPMAP_ENTRY*>::const_iterator iter = m_mapIDToEntry.begin(),
		last = m_mapIDToEntry.end();
	CString strTemp;
	for (; iter!=last; iter++) {
		if (bAllProp && !IsDispProperty(iter->first))
			continue;
		strTemp = iter->second->lpszName;
		strTemp.MakeLower();
		listOfNames.push_back(strTemp);
	}
	return TRUE;
}

#ifdef V8_JS_ENGINE_SUPPORT
Handle<FunctionTemplate> SKIN_DISPATCH_INFO::GetClassTemplate() const
{
    HandleScope handleScope;

    return handleScope.Close(m_functionTemplate);
}
#endif

