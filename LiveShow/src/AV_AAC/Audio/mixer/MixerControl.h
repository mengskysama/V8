#pragma once
#include <MMSystem.h>

class CMixerControl
{
public:
	CMixerControl(void);
	~CMixerControl(void);

public:
	//获取控制器个数
	int GetControlCount(void) ;
	//获取控制器洗洗
	int GetControlInfo(MIXERLINE* pMixerLine, HMIXER hMixer );
	//获取控制器信息
	int GetControlInfoByIndex( int nIndex, MIXERCONTROL* pMC ) ;
	//获取控制状态
	int GetControStateData(DWORD dwControlID, void* pDetails, unsigned int* nSizeOfDetails); 
	//设置控制状态
	int SetControStateData(DWORD dwControlID, void* pDetails, unsigned int* nSizeOfDetails); 
	//设置选择状态
	int SetControSelStateData(DWORD dwID,DWORD dwLineId, unsigned int index,bool bEnable);
	//获取选择状态
	int GetControSelStateData(DWORD dwID,DWORD dwLineId, unsigned int index,bool& bEnable);
private:
	//清理当前控制器分配的堆栈信息
	void CleanupHeap() ;
	//初始化控制明细
	int InitMixerControlDetails() ;

private:
	HMIXER				 mhMixer ;//混音器句柄
	unsigned int		 mnControlCount ;//当前线路多少个控制器
	MIXERLINE			 mpMixerLine ;//当前控制器线路信息
	MIXERLINECONTROLS	 mMixerLineControls ;//当前控制器信息

	bool*				 mpbHaveStateInfo ;//状态信息已经读取
	void*				 mpControlData ;//控制器数据
	MIXERCONTROL*		 mpMixerControl ;//控制信息
	MIXERCONTROLDETAILS* mpMixerControlStateInfo ;//控制明细信息
};
