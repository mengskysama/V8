#pragma once

#include "CmdTargetPtr.h"

#include <set>
#include <map>
#include <list>

//////////////////////////////////////////////////////////////////////////
//Dispatch Map 信息
//////////////////////////////////////////////////////////////////////////
//Skin的Dispatch Map信息
//利用了静态全局变量的构造函数来初始化这个结构
struct SKIN_DISPATCH_INFO
{
public:
	explicit SKIN_DISPATCH_INFO(LPCTSTR pzClassName, const AFX_DISPMAP_ENTRY* pDispatchEntry, const SKIN_DISPATCH_INFO* pBaseDispInfo);
    ~SKIN_DISPATCH_INFO();

    //获得类名
    LPCTSTR GetClassName() const;

    //获得SKIN_DISPATCH_INFO链上下一个
    static const SKIN_DISPATCH_INFO* GetSkinDispatchInfoHeader();

    //获得SKIN_DISPATCH_INFO链上下一个
    const SKIN_DISPATCH_INFO* GetNextSkinDispatchInfo() const;

    //获得基类的SKIN_DISPATCH_INFO
    const SKIN_DISPATCH_INFO* GetBaseSkinDispatchInfo() const;

	//获得DispatchMap Size 相关信息
	BOOL GetDispatchMapSize(LONG& size, LONG& minId, LONG& maxId) const;

	//检查Disp 名字或别名是否有重复定义
	BOOL CheckRedefinedDispNameOrAlias(std::set<CString>* pSetOfRedefined = NULL) const;

	//检查Disp ID是否有重复定义，即同一个Disp ID被不同的属性或方法使用，别名除外
	BOOL CheckRedefinedDispID(std::set<int>* pSetOfRedefined = NULL) const;

	//是否是同一DispatchInfo
	BOOL IsSameAs(const SKIN_DISPATCH_INFO& other) const;

	//是否是属性
	BOOL IsDispProperty(LONG dispId) const;
	BOOL IsDispProperty(LPCTSTR dispNameOrAlias) const;

	//是否是方法
	BOOL IsDispMethod(LONG dispId) const;
	BOOL IsDispMethod(LPCTSTR dispNameOrAlias) const;

	//判断是否是名字
	BOOL IsDispName(LPCTSTR pzDispName) const;

	//判断是否是名字或别名
	BOOL IsDispNameOrAlias(LPCTSTR pzDispNameOrAlias) const;

	//获得DispEntry
    const AFX_DISPMAP_ENTRY* GetDispEntry() const;
	const AFX_DISPMAP_ENTRY* GetDispEntryByDispId(LONG dispId) const;
	const AFX_DISPMAP_ENTRY* GetDispEntryByDispNameOrAlias(LPCTSTR pzDispNameOrAlias) const;

	//获得名字
	BOOL GetDispNameByDispId(LONG dispId, CString& strName) const;
	BOOL GetDispNameByDispNameOrAlias(LPCTSTR pzDispNameOrAlias, CString& strName) const;

	//获得Id
	BOOL GetDispIdByDispNameOrAlias(LPCTSTR pzDispNameOrAlias, LONG& dispId) const;

	//获得别名
	BOOL GetDispAliasByDispId(LONG dispId, std::list<CString>& listOfAlias) const;
	BOOL GetDispAliasByDispNameOrAlias(LPCTSTR pzDispNameOrAlias, std::list<CString>& listOfAlias) const;

	//获得Id列表，无重复
	BOOL GetAllDispId(std::list<LONG>& listOfIds, BOOL bAllProp = TRUE) const;

	//获得名字列表
	BOOL GetAllDispName(std::list<CString>& listOfNames, BOOL bAllProp = TRUE) const;

#ifdef V8_JS_ENGINE_SUPPORT
    v8::Handle<v8::FunctionTemplate> GetClassTemplate() const;
public:
    v8::Persistent<v8::FunctionTemplate> m_functionTemplate;
#endif //V8_JS_ENGINE_SUPPORT

protected:
	BOOL InitializeSkinDispatchInfo();

protected:
	//全局链表头
	static SKIN_DISPATCH_INFO* s_pSkinDispatchInfoListHeader;
protected:
	const AFX_DISPMAP_ENTRY* const m_pDispEntry;
	const SKIN_DISPATCH_INFO* const m_pNextSkinDispatchInfo;
    const SKIN_DISPATCH_INFO* const m_pBaseSkinDispatchInfo;
	LPCTSTR m_pzClassName;
	LONG m_lMinDispid;
	LONG m_lMaxDispid;
	LONG m_lDispSize;
	std::map<LONG, const AFX_DISPMAP_ENTRY*> m_mapIDToEntry;	//把ID映射到DispEntry，注意不是别名，名字是在Dispatch Map中第一此出现的
	std::map<CString, LONG> m_mapNameOrAliasToID;		//把名字(或别名)映射到ID
	std::map<CString, std::list<CString>> m_mapNameAlias;	//名字的别名表
};

//声明SkinDispatchMap
#define DECLARE_SKIN_DISPATCH_MAP() \
	DECLARE_DISPATCH_MAP() \
private: \
	static const SKIN_DISPATCH_INFO _skinDispatchInfo; \
    static const SKIN_DISPATCH_INFO* GetBaseSkinDispatchInfo(); \
protected: \
    static const SKIN_DISPATCH_INFO* GetThisSkinDispatchInfo(); \
	virtual const SKIN_DISPATCH_INFO* GetSkinDispatchInfo() const; \

#define SKIN_DISPID(theClass, propName)  theClass##Dispid##propName

#define SKIN_DISPID_WITH_COMMA(theClass, propName)  theClass##Dispid##propName, 

//开始SkinDispatchMap
#define BEGIN_SKIN_DISPATCH_MAP_XMLCMDTARGET(theClass, baseClass) \
    const SKIN_DISPATCH_INFO* theClass::GetBaseSkinDispatchInfo() \
    { return NULL; } \
    const SKIN_DISPATCH_INFO* theClass::GetThisSkinDispatchInfo() \
    { return &theClass::_skinDispatchInfo; } \
    const SKIN_DISPATCH_INFO* theClass::GetSkinDispatchInfo() const \
    { return &theClass::_skinDispatchInfo; } \
    BEGIN_DISPATCH_MAP(theClass, baseClass) \

#define BEGIN_SKIN_DISPATCH_MAP(theClass, baseClass) \
    const SKIN_DISPATCH_INFO* theClass::GetBaseSkinDispatchInfo() \
    { return baseClass::GetThisSkinDispatchInfo(); } \
    const SKIN_DISPATCH_INFO* theClass::GetThisSkinDispatchInfo() \
    { return &theClass::_skinDispatchInfo; } \
	const SKIN_DISPATCH_INFO* theClass::GetSkinDispatchInfo() const \
	{ return &theClass::_skinDispatchInfo; } \
	BEGIN_DISPATCH_MAP(theClass, baseClass) \

//定义一个属性
#define SKIN_DISP_PROPERTY(theClass, propName, propType)	\
	DISP_PROPERTY_EX_ID(theClass, #propName, SKIN_DISPID(theClass, propName), Get##propName, Set##propName, propType)

//定义一个函数
#define SKIN_DISP_FUNCTION(theClass, funcName, retType, params)	\
	DISP_FUNCTION_ID(theClass, #funcName, SKIN_DISPID(theClass, funcName), funcName, retType, params)

//定义一个属性的别名
//注意：别名的定义必须在对应属性定义的后面
#define SKIN_DISP_PROPERTY_ALIAS(theClass, propAlias, propName, propType)	\
	DISP_PROPERTY_EX_ID(theClass, #propAlias, SKIN_DISPID(theClass, propName), Get##propName, Set##propName, propType)

//定义一个函数的别名
//注意：别名的定义必须在对应函数的定义后面
#define SKIN_DISP_FUNCTION_ALIAS(theClass, funcAlias, funcName, retType, params)	\
	DISP_FUNCTION_ID(theClass, #funcAlias, SKIN_DISPID(theClass, funcName), funcName, retType, params)

//结束SkinDispatchMap
//注意：此处假定了全局变量theClass::_dispatchEntries的初始化顺序在theClass::_skinDispatchInfo之前
#define END_SKIN_DISPATCH_MAP(theClass) \
	END_DISPATCH_MAP() \
    const SKIN_DISPATCH_INFO theClass::_skinDispatchInfo(_T(#theClass), theClass::_dispatchEntries, theClass::GetBaseSkinDispatchInfo()); \


//属性
#define SKIN_DISP_PROPERTY_BOOL(theClass, propName)  SKIN_DISP_PROPERTY(theClass, propName, VT_BOOL)
#define SKIN_DISP_PROPERTY_BSTR(theClass, propName)  SKIN_DISP_PROPERTY(theClass, propName, VT_BSTR)
#define SKIN_DISP_PROPERTY_LONG(theClass, propName)  SKIN_DISP_PROPERTY(theClass, propName, VT_I4)


//////////////////////////////////////////////////////////////////////////
//默认值表，默认值用字符串表示，字串不需要加_T
//定义在后面的默认值将覆盖前面的默认值
//////////////////////////////////////////////////////////////////////////
//默认值对的结构
struct SKIN_DEFAULT_VALUE_PAIR
{
	BOOL m_bEnd;
	LONG m_lDispid;
	LPCTSTR m_pzDefaultValue;
};

//默认值信息
struct SKIN_DEFAULT_VALUE_INFO
{
	explicit SKIN_DEFAULT_VALUE_INFO(const SKIN_DEFAULT_VALUE_PAIR* pDefaultValuePair)
		:m_pDefaultValuePair(pDefaultValuePair)
	{

	}
protected:
	const SKIN_DEFAULT_VALUE_PAIR* const m_pDefaultValuePair;
};

//声明默认值表
#define DECLARE_SKIN_DEFAULT_VALUE_MAP() \
private: \
	static const SKIN_DEFAULT_VALUE_INFO _skinDefaultValueInfo; \
	static const SKIN_DEFAULT_VALUE_PAIR _skinDefaultValuePair[]; \
protected: \
	virtual const SKIN_DEFAULT_VALUE_INFO* GetSkinDefaultValueInfo() const; \

//开始默认值表
#define BEGIN_SKIN_DEFAULT_VALUE_MAP(theClass, baseClass) \
	const SKIN_DEFAULT_VALUE_INFO* theClass::GetSkinDefaultValueInfo() const \
	{return &theClass::_skinDefaultValueInfo;}; \
	const SKIN_DEFAULT_VALUE_PAIR theClass::_skinDefaultValuePair[] = { \

//定义一个默认值表项，dispName必须和定义该Dispid的name一致
#define SKIN_DEFALUT_VALUE(theClass, dispName, dispValue) \
	{FALSE, SKIN_DISPID(theClass, dispName), _T(dispValue)}, \

//结束默认值表
//注意：此处假定了全局变量theClass::_skinDefaultValuePair的初始化顺序在theClass::_skinDefaultValueInfo之前
#define END_SKIN_DEFAULT_VALUE_MAP(theClass) \
	{TRUE, 0, NULL} }; \
	const SKIN_DEFAULT_VALUE_INFO theClass::_skinDefaultValueInfo(_skinDefaultValuePair); \

