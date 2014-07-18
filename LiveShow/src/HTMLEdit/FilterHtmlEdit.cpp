#include "StdAfx.h"
#include "FilterHtmlEdit.h"
#include "tchar.h"
#include "globaluidefine.h"
#include <xstring>
#include <boost/xpressive/xpressive.hpp>
#include "..\..\include\GGHelper.h"

#define NO_RELEASE 1 

using namespace boost::xpressive;

void CHtmlFilter::Html2Text(CString & strcontent)
{
	BaseTranslate( strcontent);
	 while(TRUE)
	 {
		 int iPositionBegin;
		 int iPositionEnd;
		 iPositionBegin= strcontent.Find(_T("<"));
		 if (iPositionBegin == -1)
		 {
			 return;
		 }

		 if(strcontent.GetAt(iPositionBegin+1) == _T('B') && strcontent.GetAt(iPositionBegin+2) == _T('R'))
		 {
			 iPositionBegin+= 3;
			 iPositionEnd = iPositionBegin;
			 continue;
		 }

		 iPositionEnd= strcontent.Find(_T(">"),iPositionBegin);
		 if(iPositionEnd == -1)
		 {
			 strcontent.Delete(iPositionBegin,strcontent.GetLength()-iPositionBegin);
		 }
		 else
		 {
			 strcontent.Delete(iPositionBegin,iPositionEnd-iPositionBegin+1);
		 }
	 }
}

//void CHtmlFilter::Html2TextHasInfo(CString & strcontent)
//{
//    TCHAR tag[] = _T("[Í¼Æ¬]");
//	BOOL flag = FALSE;
//	BaseTranslate( strcontent);
//	while(TRUE)
//	{
//		int iPositionBegin;
//		int iPositionEnd;
//		iPositionBegin= strcontent.Find(_T("<"));
//		if (iPositionBegin == -1)
//		{
//			return;
//		}
//
//		if(strcontent.GetAt(iPositionBegin+1) == _T('B') && strcontent.GetAt(iPositionBegin+2) == _T('R'))
//		{
//			iPositionBegin+= 3;
//			iPositionEnd = iPositionBegin;
//			continue;
//		}
//
//		if((iPositionBegin+3)<=strcontent.GetLength())
//		{
//			if (strcontent.GetAt(iPositionBegin+1) == 'I'  && strcontent.GetAt(iPositionBegin+2) == 'M' && strcontent.GetAt(iPositionBegin+3) == 'G')
//			{
//				flag = TRUE;
//			}
//		}
//        
//		iPositionEnd= strcontent.Find(_T(">"),iPositionBegin);
//		if(iPositionEnd == -1)
//		{
//			strcontent.Delete(iPositionBegin,strcontent.GetLength()-iPositionBegin);
//			if (flag == TRUE)
//			{
//				strcontent.Append(tag);
//				flag = FALSE;
//			}
//			
//		}
//		else
//		{
//			strcontent.Delete(iPositionBegin,iPositionEnd-iPositionBegin+1);
//			if (flag == TRUE)
//			{
//				strcontent.Insert(iPositionBegin,tag);
//				flag = FALSE;
//			}
//            
//		}
//	}
//}
//
//



void CHtmlFilter::Html2TextHasInfo(CString & strcontent)
{
	BaseTranslate( strcontent);
	while(TRUE)
	{
		int iPositionBegin;
		int iPositionEnd;
		iPositionBegin= strcontent.Find(_T("<"));
		if (iPositionBegin == -1)
		{
			return;
		}

		iPositionEnd = strcontent.Find(_T(">"),iPositionBegin);
		if(iPositionEnd == -1)
		{
			strcontent = strcontent.Left(iPositionBegin);
			return;
		}
		strcontent.Delete(iPositionBegin,iPositionEnd - iPositionBegin+1);

	}



}




void CHtmlFilter::DeleteTag(CString & strcontent,CString begintag,CString endtag)
{
	while(TRUE)
	{
		int iPositionBegin;
		int iPositionEnd;
		iPositionBegin= strcontent.Find(begintag);
		if (iPositionBegin == -1)
		{
			return;
		}
		iPositionEnd= strcontent.Find(endtag);
		if (iPositionEnd == -1)
		{
			return;
		}
        strcontent.Delete(iPositionBegin,iPositionEnd-iPositionBegin+endtag.GetLength());
	}
}



int gSearchBegin=0;
int gSearchEnd=0;
int gResultBegin=0;
int gResultEnd=0;

void CHtmlFilter::DeleteTable(CString & content)
{
 gSearchBegin=0;
 gSearchEnd=0;
 gResultBegin=0;
 gResultEnd=0;

	 CString be = _T("<TABLE");
	CString en = _T("</TABLE>");
	std::vector<std::pair<int ,int> > vecmark;
	while(1)
	{
		gResultBegin = gSearchBegin = content.Find(be,gResultBegin);
		gSearchEnd = content.Find(en,gResultEnd);
		if(gSearchBegin == -1 || gSearchEnd == -1)
		{
			break;
		}
		else
		{
            
             SearchRecruit(content,be,gSearchBegin+1,gSearchEnd,true);
			 SearchRecruit(content,en,gSearchBegin,gSearchEnd+1,false);
			 //std::cout<<"<"<<gResultBegin<<" "<<gResultEnd<<">"<<std::endl;
			 vecmark.push_back(std::make_pair<int,int>(gResultBegin,gResultEnd));
             //gSearchBegin = gResultBegin+1;
			 gResultBegin = gResultEnd+1;
		}
	}
    CString  result ;
	int index =0;
	for(int i=0;i<vecmark.size();i++)
	{
		result.Append(
			content.Mid(
			index,

			vecmark[i].first-index

			)
			);
		index = vecmark[i].second+en.GetLength()-1;
	}
	result.Append(content.Mid(
			index,

			content.GetLength()-index

			));
	content = result;
	//std::cout<<result;
}

void CHtmlFilter::SearchRecruit(CString & content,CString & tag,int begintag,int endtag,bool bLeft)
{
	if(bLeft == true)
	{
		gSearchBegin = content.Find(tag,begintag);
		if(gSearchBegin == -1)
		{
			//gResultBegin = begintag;
			return ;
		}
		else
		{
			if(gSearchBegin < endtag)
			{
				return SearchRecruit(content,tag,gSearchBegin+1, endtag,true);
			}
			else
			{
				//gResultBegin = begintag;
				return ;
			}
		}
	}
	else
	{
		gSearchEnd = content.Find(tag,endtag);
		if(gSearchEnd == -1)
		{
			gResultEnd = endtag;
			return ;
		}
		else
		{
			if(gSearchEnd < begintag)
			{
				return SearchRecruit(content,tag,begintag, gSearchEnd+1,false);
			}
			else
			{
				gResultEnd = endtag;
				return ;
			}
		}
	}
	
}


bool CHtmlFilter::DeleteTagForSpecialId(CString id,CString & strcontent,CString begintag,CString endtag)
{
	bool re = false;
	int iPositionBegin = 0;
	int iPositionEnd;
	int iPositionId;
	while(TRUE)
	{
		
		iPositionBegin= strcontent.Find(begintag,iPositionBegin);
		if (iPositionBegin == -1)
		{
			return re;
		}
		iPositionEnd= strcontent.Find(endtag,iPositionBegin);
		if (iPositionEnd == -1)
		{
			return re;
		}
		CString substr = strcontent.Left(iPositionEnd+1+5).Right(iPositionEnd-iPositionBegin+1+5);

		if((iPositionId = substr.Find(id))!= -1)
		{
			re = true;
			strcontent.Delete(iPositionBegin,iPositionEnd-iPositionBegin+endtag.GetLength());

		}
		else
		{
            iPositionBegin = iPositionEnd+1;
		}
		
		if(iPositionBegin >= strcontent.GetLength())
		{
			return re;
		}
		//strcontent.Delete(iPositionBegin,iPositionEnd-iPositionBegin+endtag.GetLength());
	}
}

void CHtmlFilter::BaseTranslate(CString & content)
{
	content.Replace(_T("&nbsp;"),_T(" "));
	content.Replace(_T("&lt;"),_T("<"));
	content.Replace(_T("&gt;"),_T(">"));
}


void CHtmlFilter::FilterAttribute(vector<CString> & attri ,CString & content)
{
     for (int i=0;i<attri.size();i++)
     {
		 FilterOneAttribute(attri[i] , content);
     }
}


void CHtmlFilter::FilterOneAttribute(CString oneAttribute ,CString & strcontent)
{
	int iPositionBegin =0;
	int iPositionEnd=0;
	int iPositionAttribute;
	int iPositionComaBegin ;
	int iPositionComaEnd ;
	while(TRUE)
	{
		
		iPositionBegin=strcontent.Find(_T("<"),((iPositionEnd == 0 ) ? 0 :iPositionEnd+1));
		if (iPositionBegin == -1)
		{
			return;
		}

		if(strcontent.GetAt(iPositionBegin+1) == _T('B') && strcontent.GetAt(iPositionBegin+2) == _T('R'))
		{
           iPositionBegin+= 3;
		   iPositionEnd = iPositionBegin;
		   continue;
		}

		if((iPositionBegin+1)<=strcontent.GetLength())
		{
			if (strcontent.GetAt(iPositionBegin+1) == '/' )
			{
				iPositionEnd= strcontent.Find(_T(">"),iPositionBegin+1);
				continue;
			}
		}

		iPositionEnd= strcontent.Find(_T(">"),iPositionBegin+1);
		if(iPositionEnd == -1)
		{
			return;
		}
		else
		{
			iPositionAttribute = strcontent.Find(oneAttribute,iPositionBegin+1);
			if(iPositionAttribute != -1 && iPositionAttribute < iPositionEnd)
			{
                 iPositionComaBegin = strcontent.Find(_T("\""),iPositionAttribute+1);
				 if (iPositionComaBegin != -1 && iPositionComaBegin < iPositionEnd)
				 {
                       iPositionComaEnd = strcontent.Find(_T("\""),iPositionComaBegin+1);
						   if (iPositionComaEnd != -1 && iPositionComaEnd < iPositionEnd)
						   {
                                 strcontent.Delete(iPositionAttribute,iPositionComaEnd +1 - iPositionAttribute + 1);
						   }
				 }
			}
		}
	}
}


void CHtmlFilter::AddaLink(CString & strcontent)
{
	TCHAR  * urls[] =	{		L"http://",		L"https://",	L"ftp://",	L"www."}; 


   int indexBegin = 0;
  
	int count = sizeof(urls) / sizeof(TCHAR *);	
	for( int i=0 ; i<count ; i++ )	
	{		
		TCHAR  * pCurrent  = urls[i];		
		DWORD firstwhile =0,secondwhile =0;
		while( (indexBegin=strcontent.Find(pCurrent, indexBegin))!=-1 )			
		{	
#ifdef NO_RELEASE
			firstwhile++;
			if(firstwhile>100000)
			{
				/// ÌáÊ¾¹æ·¶»¯[12/24/2007 ÎÂ»ÔÃô]
				/// ::MessageBox(NULL,_T("AddaLink firstwhile"),_T("51IM"),0);
				AfxMessageBox(_T("AddaLink firstwhile"));
				break;
			}
#endif
			vector<pair <int ,int > > vecBE;				
			vector<pair <int ,int > > vecBE2;			
			FindAllLink(strcontent,vecBE);	
			FindAllTag(strcontent,vecBE2);	
			int index;
			int index2;
			bool re = BeInLink(indexBegin,vecBE,index);			
			bool re2 = BeInTag(indexBegin,vecBE2,index2);
			if(re)				
			{
				indexBegin=index+1;
				continue;
			}		
			else if(re2)
			{
				indexBegin=index2+1;
				continue;
			}
			else				
			{
				CString s;					
				s = strcontent.Right(strcontent.GetLength()-indexBegin);					
				int len=0;					
				while(  ( isdigit(s[len]) || isalpha(s[len]) 	|| s[len]=='.' || s[len]==':' || s[len]=='/' || s[len]=='-'|| s[len]=='_' || s[len]=='?' || s[len]=='=' || s[len]=='&' || s[len]=='%' || s[len]=='~' || s[len]=='+' || s[len]==',') )					
				{	
#ifdef NO_RELEASE
					secondwhile++;
					if(secondwhile>100000)
					{
						/// ÌáÊ¾¹æ·¶»¯[12/24/2007 ÎÂ»ÔÃô]
						/// ::MessageBox(NULL,_T("AddaLink secondwhile"),_T("51IM"),0);
						AfxMessageBox( _T("AddaLink secondwhile") );
						//MessageBox(_T("AddaLink secondwhile"));
						break;
					}
#endif
					if(len < s.GetLength())						
					{
						len++;
					}						
					else
					{
						break;
					}					
				}                  
				CString sLeft,sRight;
				sLeft = s.Left(len);
				sRight = s.Right(s.GetLength()- len);
				CString sHead;
				sHead = _T("<A href=\"http://") + sLeft + _T("\">") + sLeft + _T("</A>");
				CString strContentLeft  = strcontent.Left(indexBegin);
				strcontent = strContentLeft + sHead + sRight;
				indexBegin += sHead.GetLength();// + sRight.GetLength();
		}							
	}	
		indexBegin = 0;
	}
}




void CHtmlFilter::FindAllLink(CString & strcontent,vector<pair<int ,int > > & vecBE)
{
	int nBegin = 0;
	int nEnd = 0;

	DWORD firstwhile = 0;
	while(true)
	{
#ifdef NO_RELEASE
		firstwhile++;
		if(firstwhile>100000)
		{
			/// ÌáÊ¾¹æ·¶»¯[12/24/2007 ÎÂ»ÔÃô]
			/// ::MessageBox(NULL,_T("FindAllLink firstwhile"),_T("51IM"),0);
			AfxMessageBox(_T("FindAllLink firstwhile"));
			break;
		}
#endif
		nBegin = strcontent.Find(_T("<A"),nBegin);
		if(nBegin != -1)
		{
           nEnd = strcontent.Find(_T("</A>"),nBegin);
		   vecBE.push_back(make_pair(nBegin,nEnd));
		   nBegin = nEnd+ 1;
		}
		else
		{
			return;
		}
	} 
     
	 
}

bool CHtmlFilter::BeInLink(int index ,vector<pair< int ,int > > & vecBE,int & indexEnd)
{
     for(int i=0;i<vecBE.size();i++)   
	 {
		 if(index > vecBE[i].first && index < vecBE[i].second)
		 {
			 indexEnd = vecBE[i].second;
			 return true;
		 }
	 }
     return false;
}



void CHtmlFilter::FindAllTag(CString & strcontent,vector<pair<int ,int > > & vecBE)
{
	int nBegin = 0;
	int nEnd = 0;

	DWORD firstwhile = 0;
	while(true)
	{
#ifdef NO_RELEASE
		firstwhile++;
		if(firstwhile>100000)
		{
			/// ÌáÊ¾¹æ·¶»¯[12/24/2007 ÎÂ»ÔÃô]
			/// ::MessageBox(NULL,_T("FindAllTag firstwhile"),_T("51IM"),0);
			AfxMessageBox(_T("FindAllTag firstwhile"));
			break;
		}
#endif
		nBegin = strcontent.Find(_T("<"),nBegin);
		if(nBegin != -1)
		{
			nEnd = strcontent.Find(_T(">"),nBegin);
			vecBE.push_back(make_pair(nBegin,nEnd));
			nBegin = nEnd+ 1;
		}
		else
		{
			return;
		}
	} 


}




bool CHtmlFilter::BeInTag(int index ,vector<pair< int ,int > > & vecBE,int & indexEnd)
{
	for(int i=0;i<vecBE.size();i++) 
	{
		if(index > vecBE[i].first && index < vecBE[i].second)
		{
			indexEnd = vecBE[i].second;
			return true;
		}
	}
	return false;
}

void CHtmlFilter::DelTagLeaveContent(CString &msg,CString tag)
{
	std::wstring wstrmsg = msg.GetBuffer();
	std::wstring wstrpath;

	CString begintag = _T("<")+tag;
	CString endtag = _T("</")+tag+_T(">");
	CString reg = begintag;
	reg.Append(_T("(.*?)>"));
	reg.Append(_T("(.*?)"));
	reg.Append(endtag);
	wstring wsreg = reg.GetBuffer();

	wsregex date = wsregex::compile(wsreg );

	wstring format = L"$2";

	std::wstring cmsg = wstrmsg;
	cmsg = regex_replace( cmsg, date, format );

	msg = cmsg.c_str();
}

void  CHtmlFilter::DelLink(CString &content)
{
	std::wstring wstrmsg = content.GetBuffer();
	std::wstring wstrpath;

	CString reg5 = _T("<A\\s+href=\"(.*?)\"\\s*>(.*?)</A>");
	wstring wsreg5 = reg5.GetBuffer();

	wsregex date5 = wsregex::compile(wsreg5 );

	std::wstring format5( L"$1" );

	wstring cmsg = wstrmsg;
	cmsg = regex_replace( cmsg, date5, format5 );
	content = cmsg.c_str();
}

