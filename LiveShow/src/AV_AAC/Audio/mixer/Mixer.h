#pragma once
#include "MixerDefine.h"
#include <mmsystem.h>

class CMixerLine;

class CMixer
{
public:
	CMixer(void);
	~CMixer(void);
	
	//句柄类型
	enum HandleTypes {
		h_MIDI_IN  = MIXER_OBJECTF_HMIDIIN, 	
		h_MIDI_OUT = MIXER_OBJECTF_HMIDIOUT, 	
		h_Wave_IN  = MIXER_OBJECTF_HWAVEIN, 	
		h_Wave_OUT = MIXER_OBJECTF_HWAVEOUT 	 
	} ;
	
	//设备类型
	enum DeviceTypes {
		Aux      = MIXER_OBJECTF_AUX, 		
		MIDI_IN  = MIXER_OBJECTF_MIDIIN, 	
		MIDI_OUT = MIXER_OBJECTF_MIDIOUT, 	
		Wave_IN  = MIXER_OBJECTF_WAVEIN, 	
		Wave_OUT = MIXER_OBJECTF_WAVEOUT 
	} ;
	
	//初始化混音器，nIndex混音设备编号 hWnd接收混音控件变化消息的窗口句柄
	int Init( unsigned int nIndex, HWND hWnd );
	//初始化混音器，devHandle设备句柄，handleType设备句柄类型，hWnd接收混音设备变化消息的窗口句柄
	int InitWithDevHandle( unsigned int devHandle, HandleTypes handleType, HWND hWnd ) ;
	//初始化混音器，devID设备标识，devType设备类型，，hWnd接收混音设备变化消息的窗口句柄
	int InitWithDevID(  unsigned int devID, DeviceTypes devType, HWND hWnd ) ;

	//获取混音器设备标识  在Init初始化完成后才能使用
	int GetMixerDevId(unsigned int devID);

	//获取混合器信息
	int GetMixerCaps( MIXERCAPS* pMixerCaps ) ;
	
	//获取线路个数
	int GetLineCount() ;
	//获取线路控制个数
	int GetLineControlCount( int lineIndex ) ;
	//获取连接个数
	int GetConnectionCount( int lineIndex ) ;
	//获取连接控制个数
	int GetConnectionControlCount( int lineIndex, int connectionIndex ) ;

	//获取Line的MIXERLINE信息
	int GetLineInfo( int lineIndex, MIXERLINE* pMixerLine ) ;
	//获取Connection的MIXERLINE信息
	int GetConnectionInfo( int lineIndex, int connectionIndex, MIXERLINE* pMixerLine ) ;
	
	//获取线路控制ID
	int GetLineControlDwID( int lineIndex, int ctrlIndex ) ;
	//获取连接控制ID
	int GetConnControlDwID( int lineIndex, int connIndex, int ctrlIndex ) ;

	//通过control ID获取MIXERCONTROL
	int GetControlMetrics( DWORD dwID, MIXERCONTROL* pMC ) ;
	//通过control ID获取控制状态数据
	int GetControlState( DWORD dwID, sChannelData* pStateData, unsigned int* nBuffSize ) ;
	//通过control ID设置控制状态数据
	int SetControlState( DWORD dwID, sChannelData* pStateData, unsigned int* nBuffSize ) ;
	//通过control ID设置控制是否为选中状态
	int SetControlSelState(DWORD dwID,DWORD dwLineId,unsigned int index,sChannelData* pStateData, unsigned int* nBuffSize );
	//通过control ID LineId 获取是否为选中状态
	int GetControlSelState(DWORD dwID,DWORD dwLineId,unsigned int index,sChannelData* pStateData, unsigned int* nBuffSize );
	//获取控制声道个数
	int GetControlChannelCount( DWORD dwID ) ; 

	//获取线路控制数据
	int GetLineControlMetrics( int lineIndex, int controlIndex, MIXERCONTROL* pMC ) ;
	//设置线路控制数据
	int GetConnectionControlMetrics( int lineIndex, int connectionIndex, int controlIndex, MIXERCONTROL* pMC ) ;
	
	//获取线路控制状态
	int GetLineControlState( unsigned int lineIndex, unsigned int controlIndex,  sChannelData* pChannelData, unsigned int* nBuffSize ) ;
	//设置线路控制状态
	int SetLineControlState( unsigned int lineIndex, unsigned int controlIndex,  sChannelData* pChannelData, unsigned int* nBuffSize ) ;	
	
	//获取连接控制状态
	int GetConnectionControlState( unsigned int lineIndex, unsigned int connectionIndex, 
		unsigned int controlIndex, sChannelData* pChannelData, unsigned int* nBuffSize ) ;
	//设置连接控制状态
	int SetConnectionControlState( unsigned int lineIndex, unsigned int connectionIndex, 
		unsigned int controlIndex, sChannelData* pChannelData, unsigned int* nBuffSize ) ;
private:
	//获取线路详细信息
	int GetLineDetails(void);
	//获取混音器详细
	int GetMixerDetails(void);
	//获取线路信息
	CMixerLine* GetCLine( unsigned int lineIndex ) ;
private:
	UINT			muMxId;//混音器设备标识
	HMIXER			mhMixer ;//混音器句柄
	int				mnMixerCount ; //混音设备的最大数
	unsigned int	mnMixerNumber ;//混音器设备编号 0 - mnMixerCount-1
	MIXERCAPS		mMixerCaps ;//混音器描述信息
	unsigned int	mnLineNums ;//声卡上混音器的目标线路数
	CMixerLine*		mpMixerLines ;	// 混音器线路详细信息封装类
};
