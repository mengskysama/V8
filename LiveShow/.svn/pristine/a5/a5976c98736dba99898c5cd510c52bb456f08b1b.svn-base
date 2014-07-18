#include "StdAfx.h"
#include "Mixer.h"
#include "MixerLine.h"

#pragma comment(lib,"Winmm.lib")

CMixer::CMixer(void)
{
	mnMixerNumber = 0 ;
	mhMixer = (HMIXER)-1 ;
	mnLineNums = 0 ;
 	mpMixerLines = NULL ;
 	mnMixerCount = mixerGetNumDevs();
 	memset( (VOID*)&mMixerCaps, 0, sizeof(MIXERCAPS) ) ;
}

CMixer::~CMixer(void)
{
	if( mhMixer != (HMIXER)-1 )
		mixerClose( mhMixer ) ;
	mhMixer = (HMIXER)-1;

 	if( mpMixerLines )
 		delete [] mpMixerLines ;

	mpMixerLines = NULL;
}

//获取混音器设备标识
int CMixer::GetMixerDevId(unsigned int devID)
{
	if (MMSYSERR_NOERROR == 
		mixerGetID((HMIXEROBJ)mhMixer, &muMxId, MIXER_OBJECTF_HMIXER))
		//(HMIXEROBJ)m_hMixer是一个最小为0的整数，设备的标识符
	{
		return muMxId;
	}
	muMxId = -1;
	return muMxId;

}

int CMixer::Init( unsigned int nIndex, HWND hWnd )
{
	int nRet =	-1 ; 
	MMRESULT	mrResult ;
	DWORD		fdwOpen ;

	// 检查混音设备编号是否合法
	if( mnMixerCount == 0  ||  nIndex >= (unsigned int)mnMixerCount  )
		return -1 ;

	mnMixerNumber = nIndex ;

	//if( mhMixer != (HMIXER)-1 ) mixerClose( mhMixer ) ;
	mhMixer = (HMIXER)-1;

	fdwOpen = MIXER_OBJECTF_MIXER ;
	if( hWnd != NULL ) fdwOpen |= CALLBACK_WINDOW ;
	mrResult = mixerOpen( &mhMixer, mnMixerNumber, (DWORD_PTR)hWnd,
		NULL, fdwOpen ) ;
	if( mrResult != MMSYSERR_NOERROR )
	{
		mhMixer = (HMIXER)-1 ;
		return -1 ;
	}

	nRet = GetLineDetails() ;
	if( nRet < 0 )
		return nRet ;

	return mnMixerCount ;
}

int CMixer::InitWithDevHandle( unsigned int devHandle, HandleTypes devType, HWND hWnd ) 
{
	int nRet =	-1 ; 
	MMRESULT	mrResult ;
	DWORD		fdwOpen ;

	if( mhMixer != (HMIXER)-1 )	 mixerClose( mhMixer ) ;
	mhMixer = (HMIXER)-1;

	fdwOpen = devType ;
	if( hWnd != NULL ) fdwOpen |= CALLBACK_WINDOW ;
	mrResult = mixerOpen( &mhMixer, devHandle, (DWORD_PTR)hWnd,
		NULL, fdwOpen ) ;
	if( mrResult != MMSYSERR_NOERROR )
	{
		mhMixer = (HMIXER)-1 ;
		return -1 ;
	}

	nRet = GetLineDetails() ;
	if( nRet < 0 )
		return nRet ;

	return mnMixerCount ;
}

int CMixer::InitWithDevID(  unsigned int devID, DeviceTypes devType, HWND hWnd )
{
	return InitWithDevHandle( devID, (HandleTypes)devType, hWnd ) ;
}

int CMixer::GetMixerCaps( MIXERCAPS* pMixerCaps ) 
{
	if( mnMixerCount <= 0 )
		return -1 ;

	memcpy( (void*)pMixerCaps, (void*)&mMixerCaps, sizeof(MIXERCAPS) ); 

	return sizeof(MIXERCAPS) ;
}

int CMixer::GetLineDetails(void)
{
	int nRet = -1 ;
	unsigned int n1 ;

	nRet = GetMixerDetails() ;
	mnLineNums = mMixerCaps.cDestinations ;
	if( mnLineNums == 0 ) {	
		MessageBox( NULL, TEXT("Mixer has no Lines!"), TEXT("Error"), MB_OK ) ;
		return -1 ;
	}
	else
	{
		if( mpMixerLines ) delete [] mpMixerLines ;
		mpMixerLines = (CMixerLine*) new CMixerLine[mnLineNums] ;
		if( !mpMixerLines ) return -1 ;

		//初始化每条混音器线路，获取 MIXERLINE 和 MIXERLINECONTROLS 和  MIXERCONTROL data。
		for( n1=0 ; n1<mnLineNums ; n1++ )  
		{
			nRet = mpMixerLines[n1].Init( n1, mhMixer ) ;
		}
	}
	return mnLineNums ;
}

int CMixer::GetMixerDetails(void)
{
	MMRESULT mrResult ;

	//获取选定混音器的设备明细
	memset( (VOID*)&mMixerCaps, 0, sizeof(MIXERCAPS) ) ;
	mrResult = mixerGetDevCaps( (UINT_PTR)mhMixer, &mMixerCaps, sizeof(MIXERCAPS) );
	if( mrResult != MMSYSERR_NOERROR )
		return -1 ;

	return 0;
}

//获取线路个数
int CMixer::GetLineCount()
{
	return mnLineNums ;
}

//获取线路控制个数
int CMixer::GetLineControlCount( int lineIndex ) 
{
	int n1 = -1 ;
	if( lineIndex >= 0  &&  lineIndex < (int)mnLineNums )
		n1 = mpMixerLines[lineIndex].GetLineControlCount() ;
	return n1 ;
}

//获取连接个数
int CMixer::GetConnectionCount( int lineIndex ) 
{
	int n1 = -1 ;
	if( lineIndex >= 0  &&  lineIndex < (int)mnLineNums )
		n1 = mpMixerLines[lineIndex].GetConnectionCount() ;
	return n1 ; ;
}

//获取连接控制个数
int CMixer::GetConnectionControlCount( int lineIndex, int connectionIndex ) 
{
	int n1 = -1 ;
	if( connectionIndex >= 0  &&  connectionIndex < mpMixerLines[lineIndex].GetConnectionCount() )
		n1 = mpMixerLines[lineIndex].GetConnectionControlCount(connectionIndex) ;
	return n1 ; ;
}

int CMixer::GetLineInfo( int lineIndex, MIXERLINE* pMixerLine )
{
	int nRet = mpMixerLines[lineIndex].GetLineMixerLineInfo( pMixerLine ) ;
	return nRet ;
}

int CMixer::GetConnectionInfo( int lineIndex, int connectionIndex, MIXERLINE* pMixerLine )
{
	int nRet = mpMixerLines[lineIndex].GetConnectionMixerLineInfo( connectionIndex, pMixerLine ) ;
	return nRet ;
}

//获取线路控制ID
int CMixer::GetLineControlDwID( int lineIndex, int ctrlIndex ) 
{
	int n1 = -1 ;
	if( lineIndex >= 0  &&  lineIndex < (int)mnLineNums )
	{
		if( ctrlIndex >= 0  &&  ctrlIndex < mpMixerLines[lineIndex].GetLineControlCount() )
			n1 = mpMixerLines[lineIndex].GetLineControlDwID( ctrlIndex ) ;
	}
	return n1 ;
}

//获取连接控制ID
int CMixer::GetConnControlDwID( int lineIndex, int connIndex, int ctrlIndex ) 
{
	int n1 = -1 ;
	if( lineIndex >= 0  &&  lineIndex < (int)mnLineNums )
	{
		if( connIndex >= 0  &&  connIndex < mpMixerLines[lineIndex].GetConnectionCount() )
		{
			if( ctrlIndex >= 0  &&  ctrlIndex < mpMixerLines[lineIndex].GetConnectionControlCount(connIndex) )
				n1 = mpMixerLines[lineIndex].GetConnControlDwID( connIndex, ctrlIndex ) ;
		}
	}
	return n1 ;
}

//通过control ID获取MIXERCONTROL
int CMixer::GetControlMetrics( DWORD dwID, MIXERCONTROL* pMC ) 
{
	int nRet ;
	unsigned int lineIndex ; 

	for( lineIndex=0 ; lineIndex<mnLineNums ; lineIndex++ )
	{
		nRet = mpMixerLines[lineIndex].GetControlMetrics( dwID, pMC ) ; 
		if( nRet >= 0 )
			break ;
	}
	return nRet ;
}

//通过control ID获取控制状态数据
int CMixer::GetControlState( DWORD dwID, sChannelData* pStateData, unsigned int* nBuffSize ) 
{
	int nRet ;
	unsigned int lineIndex ; 

	for( lineIndex=0 ; lineIndex<mnLineNums ; lineIndex++ )
	{
		nRet = mpMixerLines[lineIndex].GetControlState( dwID, pStateData, nBuffSize ) ; 
		if( nRet >= 0 )
			break ;
	}
	return nRet ;
}

//通过control ID设置控制状态数据
int CMixer::SetControlState( DWORD dwID, sChannelData* pStateData, unsigned int* nBuffSize ) 
{
	int nRet ;
	unsigned int lineIndex ; 

	for( lineIndex=0 ; lineIndex<mnLineNums ; lineIndex++ )
	{
		nRet = mpMixerLines[lineIndex].SetControlState( dwID, pStateData, nBuffSize ) ; 
		if( nRet >= 0 )
			break ;
	}
	return nRet ;
}

//通过control ID设置控制是否为选中状态
int CMixer::SetControlSelState(DWORD dwID,DWORD dwLineId,unsigned int index,sChannelData* pStateData, unsigned int* nBuffSize )
{

	int nRet ;
	unsigned int lineIndex ; 

	for( lineIndex=0 ; lineIndex<mnLineNums ; lineIndex++ )
	{
		nRet = mpMixerLines[lineIndex].SetControlSelState( dwID,dwLineId,index, (pStateData->Left.dwValue==1) ) ; 
		if( nRet >= 0 )
			break ;
	}
	return nRet ;
}

//通过control ID LineId 获取是否为选中状态
int CMixer::GetControlSelState(DWORD dwID,DWORD dwLineId,unsigned int index,sChannelData* pStateData, unsigned int* nBuffSize )
{
	int nRet ;
	unsigned int lineIndex ; 

	for( lineIndex=0 ; lineIndex<mnLineNums ; lineIndex++ )
	{
		bool bEnable = false;
		nRet = mpMixerLines[lineIndex].GetControlSelState( dwID,dwLineId,index, bEnable ) ; 
		pStateData->Left.dwValue = pStateData->Left.dwValue = (bEnable?1:0);
		if( nRet >= 0 )
			break ;
	}
	return nRet ;

}

//获取控制声道个数
int CMixer::GetControlChannelCount( DWORD dwID ) 
{
	int nChannels ;
	MIXERCONTROL mc ;

	nChannels = GetControlMetrics( dwID, &mc ) ;

	return nChannels ;
}

//获取线路控制数据
int CMixer::GetLineControlMetrics( int lineIndex, int controlIndex, MIXERCONTROL* pMC ) 
{
	int nRet = -1 ;
	if( lineIndex >= 0  &&  lineIndex < (int)mnLineNums )
		nRet = mpMixerLines[lineIndex].GetLineControlMetrics( controlIndex, pMC ) ; 

	return nRet ;
}

//设置线路控制数据
int CMixer::GetConnectionControlMetrics( int lineIndex, int connectionIndex, int controlIndex, MIXERCONTROL* pMC ) 
{
	int nRet = -1 ;
	if( lineIndex>=0 && lineIndex<(int)mnLineNums )
	{
		nRet = mpMixerLines[lineIndex].GetConnectionControlMetrics( connectionIndex, 
			controlIndex, pMC ) ; 
	}
	return nRet ;
}

//获取线路控制状态
int CMixer::GetLineControlState( unsigned int lineIndex, unsigned int controlIndex,  sChannelData* pChannelData, unsigned int* nBuffSize ) 
{
	int nRet ;

	nRet = GetLineControlDwID( lineIndex, controlIndex ) ;
	if( nRet < 0 )
		return nRet ;

	nRet = mpMixerLines[lineIndex].GetControlState( nRet, pChannelData, nBuffSize ) ;

	return nRet ;
}

//设置线路控制状态
int CMixer::SetLineControlState( unsigned int lineIndex, unsigned int controlIndex,  sChannelData* pChannelData, unsigned int* nBuffSize ) 
{
	int nRet ;

	nRet = GetLineControlDwID( lineIndex, controlIndex ) ;
	if( nRet < 0 )
		return nRet ;

	nRet = mpMixerLines[lineIndex].SetControlState( nRet, pChannelData, nBuffSize ) ;

	return nRet ;
}

//获取连接控制状态
int CMixer::GetConnectionControlState( unsigned int lineIndex, unsigned int connectionIndex, 
							  unsigned int controlIndex, sChannelData* pChannelData, unsigned int* nBuffSize ) 
{
	int nRet ;

	nRet = GetConnControlDwID( lineIndex, connectionIndex, controlIndex ) ;
	if( nRet < 0 )
		return nRet ;

	nRet = mpMixerLines[lineIndex].GetControlState( nRet, pChannelData, nBuffSize ) ;

	return nRet ;
}

//设置连接控制状态
int CMixer::SetConnectionControlState( unsigned int lineIndex, unsigned int connectionIndex, 
							  unsigned int controlIndex, sChannelData* pChannelData, unsigned int* nBuffSize ) 
{
	int nRet ;

	nRet = GetConnControlDwID( lineIndex, connectionIndex, controlIndex ) ;
	if( nRet < 0 )
		return nRet ;

	nRet = mpMixerLines[lineIndex].SetControlState( nRet, pChannelData, nBuffSize ) ;

	return nRet ;
}

//获取线路信息
CMixerLine* CMixer::GetCLine( unsigned int lineIndex ) 
{
	if( lineIndex >= mnLineNums )
		return NULL ;
	return &mpMixerLines[lineIndex] ;
}