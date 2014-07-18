/**
* initlist.h
* this file define the IModule initlist
*
* @version 1.0 (08 ,10, 2011)
* @author plushu
*
*/

#pragma  once

#include "51ktv.h"
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>
#include <string>
#include <vector>
#include <map>
#include <xstring>

using namespace boost::assign;

#ifdef DEBUG
#define MODULE_NAME(X) L#X##L"d.dll"
#else
#define MODULE_NAME(X) L#X##L".dll"
#endif
struct InitList
{
	InitList()
	{
		insert( m_stDllModuleList )
			( MODULE_NAME(Core),		MODULE_ID_CORE)
			( MODULE_NAME(Web),		    MODULE_ID_WEBSERVICE)
			( MODULE_NAME(AV),		    MODULE_ID_AV)
			( MODULE_NAME(Hall),        MODULE_ID_HALL)
			( MODULE_NAME(Room),		MODULE_ID_ROOM);

	}

	typedef std::map<std::wstring, ModuleId> DllModuleList;
	DllModuleList m_stDllModuleList;
};