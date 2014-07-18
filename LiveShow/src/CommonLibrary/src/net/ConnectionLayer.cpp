/**
* ConnectionLayer.cpp
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/

#include "ConnectionLayerImpl.h"
#include "..\..\include\Net\ConnectionLayer.h"
#include <cassert>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace netlayer
{
ConnectionLayer::ConnectionLayer()
{
	m_pImpl = new ConnectionLayerImpl();
}

ConnectionLayer::~ConnectionLayer()
{
	if(m_pImpl != NULL)
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}
}

unsigned int ConnectionLayer::Create(COMM_TYPE enType, MSG_BUF& oBuf,
					const IpPort& oAddr, 
					const IpPort& oProxyAddr, 
					const std::string& strName, 
					const std::string& strPass,
					const std::string& strDomain)
{
	if(m_pImpl != NULL)
	{
		return m_pImpl->Create(enType, oBuf, oAddr, oProxyAddr, strName, strPass, strDomain);
	}

	return 0;
}

void ConnectionLayer::Delete(unsigned int uiID)
{
	if(m_pImpl != NULL)
	{
		m_pImpl->Delete(uiID);
	}
}

bool ConnectionLayer::SendTo(unsigned int uiID, Packet& oPacket, const IpPort& oAddr)
{
	if(m_pImpl != NULL)
	{
		return m_pImpl->SendTo(uiID, oPacket, oAddr);
	}

	return false;
}
}; //end namespace netlayer
