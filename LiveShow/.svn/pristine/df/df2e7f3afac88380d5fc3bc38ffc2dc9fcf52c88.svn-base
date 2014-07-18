/**
* ConnectionLayer.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author plushu
*
*/
#pragma  once

#include "..\Common.h"
#include "ConnectionCommon.h"

namespace netlayer
{
	class ConnectionLayerImpl;

	class COMMON_INTERFACE ConnectionLayer
	{
	public:
		ConnectionLayer();
		virtual ~ConnectionLayer();

		/*
		* 创建一个通讯实例
		* @param [in] enType:链接类型
		* @param [in,out] oBuf:循环buffer
		* @param [in] oAddr:目的地址
		* @param [in] oProxyAddr:代理地址
		* @param [in] strName:代理用户名
		* @param [in] strPass:代理密码
		* @param [in] strDomain:代理域
		* @return 返回整数标示 (等于0表示失败)
		*/

		unsigned int Create(COMM_TYPE enType, MSG_BUF& oBuf,
								const IpPort& oAddr = IpPort(), 
								const IpPort& oProxyAddr = IpPort(), 
								const std::string& strName = "", 
								const std::string& strPass = "",
								const std::string& strDomain = "");
		/*
		* 删除一个链接
		* @param [in] uiID:Create时返回的链接标示
		*/

		void Delete(unsigned int uiID);

		/*
		* 发送数据
		* @param [in] uiID:Create时返回的链接标示
		* @param [in] oPacket:发送的包
		* @param [in] oAddr:目的地址
		* @return 返回发送是否成功
		*/

		bool SendTo(unsigned int uiID, Packet& oPacket, const IpPort& oAddr = IpPort());

	private:

		ConnectionLayerImpl* m_pImpl;
	};
}; 