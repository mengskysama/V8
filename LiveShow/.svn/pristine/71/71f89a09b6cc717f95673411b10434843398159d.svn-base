/**
* ConnectionLayerImpl.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "InnerUtility.h"
#include "..\..\include\Net\ConnectionCommon.h"

#include <map>

namespace netlayer
{
#if _MSC_VER <= 1200
#define MADDR(x,y) y
#define TYPENAME
#else
#define MADDR(x,y) &x::y
#define TYPENAME typename
#endif
#define MAP_VALUE(type, key, val) TYPENAME type::value_type(key, val)

class ConnectionBase;

class ConnectionLayerImpl
{
	//类型定义
	typedef std::map<SOCKET, ConnectionBase*> MAP_EVENTS;
	typedef void (ConnectionLayerImpl::*THREADPROC)(void *);
	typedef InnerUtility::BindMember0<ConnectionLayerImpl, THREADPROC> THREAD_FUNC;
	typedef InnerUtility::Thread<THREAD_FUNC> THREAD;
	typedef std::pair<ConnectionBase *, SOCKET> PTRSOCKET;
	typedef std::vector<PTRSOCKET> COMMPTRS; 
	typedef InnerUtility::shared_ptr2<ConnectionBase> SPCOMM;
	typedef std::map<unsigned int, SPCOMM> MAP_COMMS; //id, spcomm
	typedef std::vector<SPCOMM> COMMS; //spcomm的容器
	typedef InnerUtility::LOCK_OBJ LOCK_OBJ;
	typedef InnerUtility::SCOPED_LOCK SCOPED_LOCK;
	typedef InnerUtility::Cache<MsgObj> MSG_CACHE; 

	friend class ConnectionBase;
public:

	enum {
		INVALID_COMMID = 0
	};

	ConnectionLayerImpl();
	virtual ~ConnectionLayerImpl();

	//创建一个通讯实例
	unsigned int Create(COMM_TYPE enType, MSG_BUF& oBuf,
							const IpPort& oAddr, 
							const IpPort& oProxyAddr, 
							const std::string& strName, 
							const std::string& strPass,
							const std::string& strDomain);
	//删除一个Connection实例
	void Delete(unsigned int uiID);
	//发送数据
	bool SendTo(unsigned int uiID, Packet& oPacket, const IpPort& oAddr = IpPort());
	int  GetSockName(unsigned int uiID);

private:
	//线程函数,
	void Process(void *); 
	//设置select set
	int SetSelectSet(fd_set& oRead, fd_set& oWrite, fd_set& oExcept); 
	//触发通讯模块
	void Trigger(fd_set& oRead, fd_set& oWrite, fd_set& oExcept); 
	//由通讯模块调用
	//设置select set
	bool AddToRead(SOCKET oSock, ConnectionBase * pComm);
	bool AddToWrite(SOCKET oSock, ConnectionBase * pComm);
	bool AddToExcept(SOCKET oSock, ConnectionBase * pComm);
	bool RemoveFromRead(SOCKET oSock, ConnectionBase * pComm);
	bool RemoveFromWrite(SOCKET oSock, ConnectionBase * pComm);
	bool RemoveFromExcept(SOCKET oSock, ConnectionBase * pComm);
	//获取临时的BUF
	MsgObj * AllocMsgObj(int nID);
	void CommitMsgObj(int nID);
	//回调消息
	bool Notify(MsgObj * pMsg, ConnectionBase * pComm);

private:

	LOCK_OBJ m_oLockEvents;
	MAP_EVENTS m_oReadEvents;  //select用的socket
	MAP_EVENTS m_oWriteEvents;
	MAP_EVENTS m_oExceptEvents;
	fd_set m_oReadSet; //保存上次的set
	fd_set m_oWriteSet;
	fd_set m_oExceptSet;
	int m_iMaxFd; //最大的socket
	volatile bool m_bChangeEvents; //Event是否发生了变化
	COMMPTRS m_oTmpReadPtrs; //保存被触发对象的临时指针
	COMMPTRS m_oTmpWritePtrs; //保存被触发对象的临时指针
	COMMPTRS m_oTmpExceptPtrs; //保存被触发对象的临时指针

	LOCK_OBJ m_oLockComms;
	MAP_COMMS m_oComms; //通讯容器

	std::map<int, MSG_BUF*> m_mapCycleBuf; //循环BUF,用于回调数据给调用层
	std::map<int, MSG_CACHE> m_mapMsgCahce; //用于回调消息的cache,放入循环BUF中,避免两次拷贝

	volatile bool m_bNeedStop; //通知线程停止运行的标志
	THREAD m_oThread; //线程
};
}; 