#pragma once
#include "afxhtml.h"
#include <vector>

using namespace std;

#ifdef HTMLEDIT_EXPORTS
#define HTMLEDIT_API __declspec(dllexport)
#else
#define HTMLEDIT_API __declspec(dllimport)
#endif

class HTMLEDIT_API CHtmlFilter
{
public:
	static void Html2Text(CString & strcontent);
	static void Html2TextHasInfo(CString & strcontent);
	static void FilterAttribute(vector<CString> & attri ,CString & content);
	static void DeleteTag(CString & strcontent,CString begintag,CString endtag);
	static bool DeleteTagForSpecialId(CString id,CString & strcontent,CString begintag,CString endtag);
	static void AddaLink(CString & strcontent);
    static void CHtmlFilter::DelTagLeaveContent(CString &msg,CString tag);
	static void  CHtmlFilter::DelLink(CString &content);
//	static void CHtmlFilter::ReplaceTagWith(CString &msg,CString tag,wstring replacetag);
	static void DeleteTable(CString & content);
	
private:
	static void BaseTranslate(CString & content);
	static void FilterOneAttribute(CString oneAttribute ,CString & content);
	static void FindAllLink(CString & strcontent,vector<pair<int ,int > > & vecBE);
	static bool BeInLink(int index ,vector<pair< int ,int > > & vecBE,int &);
	static bool BeInTag(int index ,vector<pair< int ,int > > & vecBE,int & indexEnd);
	static void FindAllTag(CString & strcontent,vector<pair<int ,int > > & vecBE);
	static void SearchRecruit(CString & content,CString & tag,int begintag,int endtag,bool bLeft);	
};