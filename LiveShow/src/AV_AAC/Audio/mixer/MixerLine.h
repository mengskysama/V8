#pragma once
#include <MMSystem.h>

class CMixerControl;
class CMixerLine
{
public:
	CMixerLine(void);
	~CMixerLine(void);
	//当前线路初始化 nLineIndex线路索引号，hMixer混音器句柄
	int Init(unsigned int nLineIndex, HMIXER hMixer);

	//返回当前线路控制器个数
	int GetLineControlCount() ;
	//返回当前线路连接个数
	int GetConnectionCount() ;
	//返回当前线路连接的控制器个数  一般情况  一个connection =  一个control
	int GetConnectionControlCount( int connectionIndex ) ;

	//获取控制信息 
	int GetControlMetrics( DWORD dwID, MIXERCONTROL* pMC ) ;
	//获取控制状态信息
	int GetControlState( DWORD dwID, void* pStateData, unsigned int* nBuffSize ) ;
	//设置控制器状态
	int SetControlState( DWORD dwID, void* pStateData, unsigned int* nBuffSize ) ; 
	//设置控制器选择状态
	int SetControlSelState( DWORD dwID,DWORD dwLineId,unsigned int index, bool bEnable ) ;
	//获取控制器选中状态
	int GetControlSelState( DWORD dwID,DWORD dwLineId,unsigned int index, bool& bEnable ) ;
	//获取线路控制信息
	int GetLineControlMetrics( int controlIndex, MIXERCONTROL* pMC ) ;
	//获取线路控制状态
	int GetLineControlState( int controlIndex, void* pStateData, unsigned int* nBuffSize ) ; 
	//设置线路控制状态
	int SetLineControlState( int controlIndex, void* pStateData, unsigned int* nBuffSize ) ; 
	
	//获取连接控制信息
	int GetConnectionControlMetrics( int connectionIndex, int controlIndex, MIXERCONTROL* pMC ) ;
	//获取连接控制状态
	int GetConnectionControlState( unsigned int connectionIndex, unsigned int controlIndex, void* pStateData, unsigned int* nBuffSize ) ; 
	//设置连接控制状态
	int SetConnectionControlState( unsigned int connectionIndex, unsigned int controlIndex, void* pStateData, unsigned int* nBuffSize ) ; 
	
	//获取连接、线路控制index
	int GetConnControlDwID( int connectionIndex, int controlIndex ) ;
	//获取线路控制index
	int GetLineControlDwID( int controlIndex ) ;
	
	//获取本线路 混音线路信息
	int GetLineMixerLineInfo( MIXERLINE* pMixerLineInfo ) ;
	//获取指定连接index 混音线路信息
	int GetConnectionMixerLineInfo( unsigned int connectionIndex, MIXERLINE* pMixerLineInfo ) ;
private:
	//获取当前线路控制器信息
	int GetConnectionsControlData(void);
private:
	HMIXER	mhMixer ;//混音设备句柄
	int		mnLineIndex ; // 混音器线路编号 0..N
	MIXERLINE mMixerLineInfo ;//混音器线路一实例
	MIXERLINE* mpConnectionsMixerLineInfo ;//本线路 每个控制器 一个混音器线路信息 数组
	unsigned int mnLineControlNum ;//混音线路控制器个数
	unsigned int mnConnections;//本线路有多少控制器数
	CMixerControl* mpLineControls;//混音线路控制器
	CMixerControl* mpControls ;//本线路 一个控制器数 一个控制器信息  数组
};
