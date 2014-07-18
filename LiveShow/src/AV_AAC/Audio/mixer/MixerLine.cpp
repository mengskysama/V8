#include "StdAfx.h"
#include "MixerLine.h"
#include "MixerControl.h"

CMixerLine::CMixerLine(void)
{
	mhMixer = (HMIXER)-1 ;
	mnLineIndex = -1 ;
	mnLineControlNum = 0 ;
	mpLineControls = NULL ;
 	mnConnections = 0 ;
 	
 	mpControls = NULL ;
 	mpConnectionsMixerLineInfo = NULL ;
 
}

CMixerLine::~CMixerLine(void)
{
	if( mpLineControls ) 
		delete mpLineControls ;

	if( mpControls )
		delete [] mpControls ;
	if( mpConnectionsMixerLineInfo )
		delete [] mpConnectionsMixerLineInfo ;
}

int CMixerLine::Init(unsigned int nLineIndex, HMIXER hMixer)
{
	int nRet = -1 ;
	MMRESULT mmr ;

	mhMixer = hMixer ;
	mnLineIndex = nLineIndex ;

	mMixerLineInfo.cbStruct = sizeof(MIXERLINE) ;
	mMixerLineInfo.dwDestination = (DWORD)nLineIndex ;
	mMixerLineInfo.dwSource = 0 ;// 标志，表示是一个混音器线路
	//获取制定混音器线路nLineIndex信息
	mmr = mixerGetLineInfo( (HMIXEROBJ)mhMixer,&mMixerLineInfo, 
		MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_DESTINATION) ;

	//获取混音器线路控制器数据信息
	mnLineControlNum = mMixerLineInfo.cControls ;
	if( mmr == MMSYSERR_NOERROR )
		nRet = GetConnectionsControlData() ;
	else
	{
		MessageBox( NULL, TEXT("mixerGetLineInfo call failed!"), TEXT("Error"), MB_OK ) ;
		mnLineControlNum = 0 ;
		return -1 ;
	}

	//线路控制器个数大于0 
	if( mnLineControlNum )
	{
		mpLineControls = new CMixerControl ;//一条混音线路只有一个混音控制器
		if( mpLineControls )
			nRet = mpLineControls->GetControlInfo(  &mMixerLineInfo, mhMixer ) ;
	}

	return nRet ;
}

int CMixerLine::GetConnectionsControlData(void)
{
	unsigned int n1 ;
	MMRESULT mmr ;

	if( mhMixer == (HMIXER)-1 )  return -1 ;

	mnConnections = mMixerLineInfo.cConnections ;//当前线路连接个数
	if( !mnConnections ) {
		return -2 ;
	}

	//一个控制器 对应一个混音器线路信息数组
	mpConnectionsMixerLineInfo = (MIXERLINE*)new MIXERLINE[mnConnections] ;

	//一个控制器 对应一个控制器信息 数组
	mpControls = new CMixerControl[mnConnections] ;

	// 获取每个控制器信息
	for( n1=0 ; n1<mnConnections ; n1++ )
	{
		memcpy( (void*)&mpConnectionsMixerLineInfo[n1] , (void*)&mMixerLineInfo, sizeof(MIXERLINE) ) ;
		mpConnectionsMixerLineInfo[n1].dwSource = n1 ;
		mmr = mixerGetLineInfo( (HMIXEROBJ)mhMixer, &mpConnectionsMixerLineInfo[n1], 
			MIXER_GETLINEINFOF_SOURCE ) ;
		mpControls[n1].GetControlInfo( &mpConnectionsMixerLineInfo[n1], mhMixer ) ;
	}

	return 0;
}

int CMixerLine::GetLineControlCount()
{
	return mnLineControlNum ;
}

int CMixerLine::GetConnectionCount()
{
	return mnConnections ;
}

int CMixerLine::GetConnectionControlCount( int connectionIndex )
{
	int n1 = -1 ;
	if( connectionIndex >= 0  &&  (unsigned int)connectionIndex < mnConnections )
		n1 = mpControls[connectionIndex].GetControlCount() ;
	return n1 ;
}

int CMixerLine::GetControlMetrics( DWORD dwID, MIXERCONTROL* pMC )
{
	unsigned int controlIndex ;
	unsigned int connectionIndex ; 
	unsigned int nControls ;
	int nChannels ;

	// 检查线路控制
	for( controlIndex=0 ; controlIndex<mnLineControlNum ; controlIndex++ )
	{
		nChannels = GetLineControlMetrics( (int)controlIndex, pMC ) ; 
		if( pMC->dwControlID == dwID )
			return( nChannels ) ;
	}

	//检查连接控制
	for( connectionIndex=0 ; connectionIndex<mnConnections ; connectionIndex++ )
	{
		nControls = mpControls[connectionIndex].GetControlCount() ;
		for( controlIndex=0 ; controlIndex<nControls ; controlIndex++ )
		{
			nChannels = mpControls[connectionIndex].GetControlInfoByIndex( controlIndex, pMC ) ;
			if( pMC->dwControlID == dwID )
				return( nChannels ) ;
		}
	}
	return -1 ;
}

int CMixerLine::GetControlState( DWORD dwID, void* pStateData, unsigned int* nBuffSize )
{
	int nRet = -1 ;
	unsigned int nControlCount ;
	unsigned int controlIndex ;
	unsigned int connectionIndex ; 
	unsigned int n1 = *nBuffSize ;

	// 搜索线路控制
	for( controlIndex=0 ; controlIndex<mnLineControlNum ; controlIndex++ )
	{
		n1 = *nBuffSize ;
		nRet = mpLineControls->GetControStateData( dwID, pStateData, &n1 ) ;
		if( nRet < 0 )				
		{
			if( nRet == -2 )		
			{
				*nBuffSize = n1 ;	
				return -1  ;
			}
		}
		else
			return nRet ;
	}

	//搜索连接控制
	for( connectionIndex=0 ; connectionIndex<mnConnections ; connectionIndex++ )
	{
		nControlCount = mpControls[connectionIndex].GetControlCount() ;
		for( controlIndex=0 ; controlIndex<nControlCount ; controlIndex++ )
		{
			n1 = *nBuffSize ;
			nRet = mpControls[connectionIndex].GetControStateData( dwID, pStateData, &n1 ) ;
			if( nRet < 0  )		
			{
				if( nRet == -2 )	
				{
					*nBuffSize = n1 ;	
					return nRet  ;
				}
			}
			else	
				return nRet ;
		}
	}

	return nRet ;
}

int CMixerLine::SetControlState( DWORD dwID, void* pStateData, unsigned int* nBuffSize )
{
	int nRet ;
	unsigned int nControlCount ;
	unsigned int controlIndex ;
	unsigned int connectionIndex ; 
	unsigned int n1 ;

	// 搜索线路控制
	for( controlIndex=0 ; controlIndex<mnLineControlNum ; controlIndex++ )
	{
		n1 = *nBuffSize ;
		nRet = mpLineControls->SetControStateData( dwID, pStateData, &n1 ) ;
		if( nRet < 0 )				 
		{
			if( nRet == -2 )		 
			{
				*nBuffSize = n1 ;	 
				return -1  ;
			}
		}
		else	 
			return nRet ;
	}

	// 搜索连接控制
	for( connectionIndex=0 ; connectionIndex<mnConnections ; connectionIndex++ )
	{
		nControlCount = mpControls[connectionIndex].GetControlCount() ;
		for( controlIndex=0 ; controlIndex<nControlCount ; controlIndex++ )
		{
			n1 = *nBuffSize ;
			nRet = mpControls[connectionIndex].SetControStateData( dwID, pStateData, &n1 ) ;
			if( nRet < 0 )				 
			{
				if( nRet == -2 )		 
				{
					*nBuffSize = n1 ;	 
					return -1  ;
				}
			}
			else	 
				return nRet ;
		}
	}

	return -1 ;
}

int CMixerLine::SetControlSelState( DWORD dwID,DWORD dwLineId, unsigned int index,bool bEnable  ) 
{
	int nRet ;
	unsigned int nControlCount ;
	unsigned int controlIndex ;
	unsigned int connectionIndex ; 

	// 搜索线路控制
	for( controlIndex=0 ; controlIndex<mnLineControlNum ; controlIndex++ )
	{
		nRet = mpLineControls->SetControSelStateData( dwID, dwLineId,index,bEnable) ;
		if( nRet < 0 )				 
		{
			if( nRet == -2 )		 
			{	 
				return -1  ;
			}
		}
		else	 
			return nRet ;
	}

	// 搜索连接控制
	for( connectionIndex=0 ; connectionIndex<mnConnections ; connectionIndex++ )
	{
		nControlCount = mpControls[connectionIndex].GetControlCount() ;
		for( controlIndex=0 ; controlIndex<nControlCount ; controlIndex++ )
		{
			nRet = mpControls[connectionIndex].SetControSelStateData( dwID, dwLineId,index,bEnable) ;
			if( nRet < 0 )				 
			{
				if( nRet == -2 )		 
				{ 
					return -1  ;
				}
			}
			else	 
				return nRet ;
		}
	}

	return -1 ;
}

//获取控制器选中状态
int CMixerLine::GetControlSelState( DWORD dwID,DWORD dwLineId,unsigned int index, bool& bEnable ) 
{
	int nRet ;
	unsigned int nControlCount ;
	unsigned int controlIndex ;
	unsigned int connectionIndex ; 

	// 搜索线路控制
	for( controlIndex=0 ; controlIndex<mnLineControlNum ; controlIndex++ )
	{
		nRet = mpLineControls->GetControSelStateData( dwID, dwLineId,index,bEnable) ;
		if( nRet < 0 )				 
		{
			if( nRet == -2 )		 
			{	 
				return -1  ;
			}
		}
		else	 
			return nRet ;
	}

	// 搜索连接控制
	for( connectionIndex=0 ; connectionIndex<mnConnections ; connectionIndex++ )
	{
		nControlCount = mpControls[connectionIndex].GetControlCount() ;
		for( controlIndex=0 ; controlIndex<nControlCount ; controlIndex++ )
		{
			nRet = mpControls[connectionIndex].GetControSelStateData( dwID, dwLineId,index,bEnable) ;
			if( nRet < 0 )				 
			{
				if( nRet == -2 )		 
				{ 
					return -1  ;
				}
			}
			else	 
				return nRet ;
		}
	}

	return -1 ;
}

//获取线路控制信息
int CMixerLine::GetLineControlMetrics( int controlIndex, MIXERCONTROL* pMC ) 
{
	int nChannels ;

	if( controlIndex < 0  ||  (unsigned int)controlIndex >= mnLineControlNum )
		return -1 ;

	nChannels = mpLineControls->GetControlInfoByIndex( controlIndex, pMC ) ;
	return nChannels ;
}

//获取线路控制状态
int CMixerLine::GetLineControlState( int controlIndex, void* pStateData, unsigned int* nBuffSize ) 
{
	int nRet ;

	nRet = GetLineControlDwID( controlIndex ) ;
	if( nRet < 0 )
		return -1 ;

	nRet = GetControlState( nRet, pStateData, nBuffSize ) ;

	return nRet ;
}

//设置线路控制状态
int CMixerLine::SetLineControlState( int controlIndex, void* pStateData, unsigned int* nBuffSize ) 
{
	int nRet ;

	nRet = GetLineControlDwID( controlIndex ) ;
	if( nRet < 0 )
		return -1 ;

	nRet = SetControlState( nRet, pStateData, nBuffSize ) ;

	return nRet ;
}

//获取连接控制信息
int CMixerLine::GetConnectionControlMetrics( int connectionIndex, int controlIndex, MIXERCONTROL* pMC ) 
{
	int nChannels ;

	if(  connectionIndex < 0  ||  (unsigned int)connectionIndex >= mnConnections )
		return -1 ;

	if(  controlIndex < 0  ||  controlIndex >= mpControls[connectionIndex].GetControlCount() )
		return -1 ;

	nChannels = mpControls[connectionIndex].GetControlInfoByIndex( controlIndex, pMC ) ;

	return nChannels ;
}

//获取连接控制状态
int CMixerLine::GetConnectionControlState( unsigned int connectionIndex, unsigned int controlIndex, void* pStateData, unsigned int* nBuffSize ) 
{
	int nRet = -1 ;
	DWORD dwID ;

	nRet = GetConnControlDwID( connectionIndex, controlIndex ) ;
	if( nRet < 0 )
		return nRet ;

	dwID = (DWORD)nRet ;
	nRet = mpControls[connectionIndex].GetControStateData( dwID, pStateData, nBuffSize ) ;

	return nRet ;
}

//设置连接控制状态
int CMixerLine::SetConnectionControlState( unsigned int connectionIndex, unsigned int controlIndex, void* pStateData, unsigned int* nBuffSize ) 
{
	int nRet = -1 ;
	DWORD dwID ;

	nRet = GetConnControlDwID( connectionIndex, controlIndex ) ;
	if( nRet < 0 )
		return nRet ;

	dwID = (DWORD)nRet ;
	nRet = mpControls[connectionIndex].SetControStateData( dwID, pStateData, nBuffSize ) ;

	return nRet ;
}

//获取连接、线路控制index
int CMixerLine::GetConnControlDwID( int connectionIndex, int controlIndex )
{
	MIXERCONTROL MC ;
	int nRet ;

	if( connectionIndex < 0  ||  (unsigned int)connectionIndex >= mnConnections )
		return -1 ;

	if( controlIndex < 0  ||  controlIndex >= mpControls[connectionIndex].GetControlCount() ) 
		return -1 ;

	nRet = mpControls[connectionIndex].GetControlInfoByIndex( controlIndex, &MC ) ;
	if( nRet < 0 )
		return -1 ;

	return MC.dwControlID ;
}

//获取线路控制index
int CMixerLine::GetLineControlDwID( int controlIndex ) 
{
	MIXERCONTROL MC ;

	if( controlIndex < 0  ||  (unsigned int)controlIndex >= mnLineControlNum )
		return -1 ;
	mpLineControls->GetControlInfoByIndex( controlIndex, &MC ) ;

	return MC.dwControlID ;
}

//获取本线路 混音线路信息
int CMixerLine::GetLineMixerLineInfo( MIXERLINE* pMixerLineInfo ) 
{
	if( mnLineIndex < 0 )
		return -1 ;

	memcpy( (void*)pMixerLineInfo, (void*)&mMixerLineInfo, sizeof(MIXERLINE) ) ;

	return 0 ;
}

//获取指定连接index 混音线路信息
int CMixerLine::GetConnectionMixerLineInfo( unsigned int connectionIndex, MIXERLINE* pMixerLineInfo )
{
	if( mnLineIndex < 0  ||  connectionIndex >= mnConnections )
		return -1 ;

	memcpy( (void*)pMixerLineInfo, (void*)&mpConnectionsMixerLineInfo[connectionIndex], 
		sizeof(MIXERLINE) ) ;

	return 0 ;
}