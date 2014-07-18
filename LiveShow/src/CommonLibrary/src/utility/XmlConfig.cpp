#include "utility/XmlConfig.h"
#include "utility/StringHelper.h"

namespace common
{
namespace utility
{
	void XmlConfig::PreHandle(std::string& strInput)
	{
		std::string strOutput;

		char sz[] = {0x0e, 0x11};
		int nLen = sizeof(sz)/sizeof(sz[0]);

		std::string::iterator it = strInput.begin();

		while(it != strInput.end())
		{
			int i = 0;

			for(; i < nLen; ++i)
			{
				if(*it == sz[i])
				{
					break;
				}
			}

			if(i < sizeof(sz)/sizeof(sz[0]))
			{
				strOutput.push_back('%');
				strOutput.push_back('1');
				strOutput.push_back(')');
				strOutput.push_back('(');
				strOutput.push_back('2');
				strOutput.push_back('@');
				strOutput.push_back('1'+i);
				strOutput.push_back(';');
			}
			else
			{
				strOutput.push_back(*it);
			}

			++it;
		}

		strOutput.push_back(0);

		strInput = strOutput;
	}

	void XmlConfig::PostHandle(std::wstring& strInput)
	{
		std::wstring strOutput;

		char sz[] = {0x0e, 0x11};
		int nLen = sizeof(sz)/sizeof(sz[0]);

		int nStart = 0;
		int nIndex = strInput.find(L"%1)(2@", nStart);

		while(nIndex >= 0 && strInput.size() > nIndex + 7)
		{
			if(strInput[nIndex+6] - '1' >= 0 && strInput[nIndex+6] - '1' < nLen &&
				strInput[nIndex+7] == ';')
			{
				for(int i = nStart; i < nIndex; ++i)
				{
					strOutput.push_back(strInput[i]);
				}

				strOutput.push_back(sz[strInput[nIndex+6] - '1']);

				nStart = nIndex + 8;

				if(nStart >= strInput.size())
				{
					break;
				}

				nIndex = strInput.find(L"%1)(2@", nStart);
			}
			else
			{
				break;
			}
		}

		for(int i = nStart; i < strInput.size(); ++i)
		{
			strOutput.push_back(strInput[i]);
		}			

		strOutput.push_back(0);

		strInput = strOutput;
	}

	bool XmlConfig::ParserXml(char const* p)
	{
		if(p == NULL) return false;

		std::string strXml = p;

		PreHandle(strXml);

		p = strXml.c_str();			

		return xmlDoc.load_from_buffer(p,strlen(p));
	}

	bool XmlConfig::LoadFile(std::wstring& file)
	{
		return xmlDoc.load_from_file(file.c_str());
	}

	bool XmlConfig::LoadConfig(std::wstring  parents,std::wstring brothers,bool multi,ConfigResult & result,bool emptyOk)
	{
		bool noBrothers=false;

		if(brothers==L"")
			noBrothers=true;

		std::vector<std::wstring> parentsVec = stringhelper::Split_String_by_Delimiter(parents, '.');
		std::vector<std::wstring> brothersVec = stringhelper::Split_String_by_Delimiter(brothers, ',');

		bool re;

		if(/*parentsVec.size()!=0 &&*/ (brothersVec.size()!=0 || noBrothers) )
		{
			common::utility::expat_element*	pTiXmlElement = xmlDoc.get_root();
			if(pTiXmlElement)
			{
				int i=0;
				for(;i<parentsVec.size();i++)
				{
					pTiXmlElement=pTiXmlElement->get_element_by_name(parentsVec[i].c_str());
					if(pTiXmlElement==NULL)
						goto error;
				}

				std::map<std::wstring ,std::wstring> tempMap;
				if(noBrothers==false)
				{
					for(int j=0;j<brothersVec.size();j++)
					{
						common::utility::expat_element * p=pTiXmlElement->get_element_by_name(brothersVec[j].c_str());
						if(p)
						{
							std::wstring temp= p->get_element_contents();
							if(temp!=L"")
							{
								PostHandle(temp);
								tempMap[brothersVec[j]]=temp;									
							}
							else
								if(emptyOk)
									tempMap[brothersVec[j]]=temp;
								else
									goto error;
						}
						else 
							goto error;
					}
				}
				else
				{
					std::wstring temp=pTiXmlElement->get_element_contents();
					if(temp!=L"")
					{
						PostHandle(temp);
						tempMap[L"value"]=temp;
					}
					else
						if(emptyOk)
							tempMap[L"value"]=temp;
						else
							goto error;
				}

				result.push_back(tempMap);

				while(multi)
				{
					pTiXmlElement=pTiXmlElement->get_next_element(parentsVec[i-1].c_str());
					if(pTiXmlElement)
					{
						std::map<std::wstring ,std::wstring> tempMap;
						if(noBrothers==false)
						{
							for(int j=0;j<brothersVec.size();j++)
							{
								common::utility::expat_element * p=pTiXmlElement->get_element_by_name(brothersVec[j].c_str());
								if(p)
								{
									std::wstring temp=p->get_element_contents();
									if(temp!=L"")
									{
										PostHandle(temp);
										tempMap[brothersVec[j]]=temp;
									}
									else
										if(emptyOk)
											tempMap[brothersVec[j]]=temp;
										else
											goto error;
								}
								else 
									goto error;
							}
						}
						else
						{
							std::wstring temp=pTiXmlElement->get_element_contents();
							if(temp!=L"")
							{
								PostHandle(temp);
								tempMap[L"value"]=temp;
							}
							else
								if(emptyOk)
									tempMap[L"value"]=temp;
								else
									goto error;
						}
						result.push_back(tempMap);
					}
					else
						break;
				}

				re = true;
				goto xmlReplace;
			}
		}
error:
		re = false;

xmlReplace:
		/*			for(ConfigResult::iterator it=result.begin();it!=result.end();++it)
		for(std::map<std::string,std::string>::iterator it2=(*it).begin();it2!=(*it).end();++it2)
		StrHelp::XmlDecode((*it2).second);*/

		return re;
	}


}
}
