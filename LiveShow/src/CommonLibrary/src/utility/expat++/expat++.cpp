// expat++.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "expat_document.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "process.h"
#include "stringhelper.h"
#include <string>

using namespace std;

void	parse_xml_buffer()
{
	const char* pbuffer = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><a><v><w p='1'>我爱你</w></a>";

	string strUtf8 = stringhelper::from_unicode(string_helper::from(pbuffer));

	expat_document document;
	int result = document.load_from_buffer( strUtf8.c_str(), strUtf8.size());

	if( result == 0)
	{
		printf("Parse failed\n");
		printf("Error Info:\n");
		printf("    line :%d ; column : %d\n", document.get_error()->get_expat_error_line(), document.get_error()->get_expat_error_column());
		wprintf(L"	Msg: %s\n", document.get_error()->get_expat_error_string(document.get_error()->get_expat_error()));

		system("pause");
		return ;
	}
	else
		printf("Parse success\n");

	expat_element*	root = document.get_root();
	printf("Root Element Name: %s\n", root->get_element_name());

	expat_element*	velement = root->get_element_by_name(L"v");
	if( velement )
	{
		expat_attribute_list* pattribute = velement->get_attribute_list();
		if( pattribute)
		{
			expat_attribute*	att = pattribute->get_attribute_by_name(L"error");
			if( att)
				wprintf(L"Attribute Name : %s, Attribute Value: %s\n", att->get_attribute_name(),att->get_attribute_value());
		}

		velement = velement->get_first_child_element();
		while( velement)
		{
			wprintf(L"  Name: %s , Contents: %s\n", velement->get_element_name(), velement->get_element_contents());
			velement = velement->get_sibling_element();
		}
	}

	system("pause");
}

void	load_from_file( )
{
	expat_document document;
	int result = document.load_from_file( L"f:\\测试\\2.xml");

	if( result == 0)
	{
		printf("Parse failed\n");
		printf("Error Info:\n");
		printf("    line :%d ; column : %d\n", document.get_error()->get_expat_error_line(), document.get_error()->get_expat_error_column());
		wprintf(L"	Msg: %s\n", document.get_error()->get_expat_error_string(document.get_error()->get_expat_error()));

		system("pause");
		return ;
	}
	else
		printf("Parse success\n");

	system("pause");

}

void	write_xml_file()
{
	expat_document document;
	expat_element* pelement = document.add_root(L"a");

	expat_element* pvelement = pelement->append_child_element(L"v");
	pvelement->add_attribute(L"error",L"1");
	pvelement->add_attribute(L"p", L"2002111");
	pvelement->set_element_contents(L"我爱你");

	bool b = document.save_to_file(L"F:\\测试\\1.xml");

}

int _tmain(int argc, _TCHAR* argv[])
{
	load_from_file();

	return 0;
}

