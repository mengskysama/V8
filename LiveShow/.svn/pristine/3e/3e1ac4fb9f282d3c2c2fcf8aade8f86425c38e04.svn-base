#include "StdAfx.h"
#include "MixerControl.h"

CMixerControl::CMixerControl(void)
{
	mnControlCount = 0 ;
	mpControlData = NULL ;
	mpMixerControl = NULL ;
	mpMixerControlStateInfo = NULL ;
	mpbHaveStateInfo = NULL ;
}

CMixerControl::~CMixerControl(void)
{
	CleanupHeap() ;
}

void CMixerControl::CleanupHeap()
{
	if( mpMixerControlStateInfo )
	{
		for( int n1=0 ; n1<(int)mnControlCount ; n1++ )
		{
			if( mpMixerControlStateInfo[n1].paDetails )
			{
				delete [] mpMixerControlStateInfo[n1].paDetails ;
				mpMixerControlStateInfo[n1].paDetails = NULL;
			}
		}
		delete [] mpMixerControlStateInfo ;
		mpMixerControlStateInfo = NULL;
	}

 	if( mpMixerControl )
	{
		delete [] mpMixerControl ;
		mpMixerControl = NULL;
	}
 		
 	if( mpbHaveStateInfo )
	{
		delete [] mpbHaveStateInfo ;
		mpbHaveStateInfo = NULL;
	}
}

//获取控制器个数
int CMixerControl::GetControlCount(void) 
{
	return mnControlCount ;
}

//获取控制器洗洗
int CMixerControl::GetControlInfo(MIXERLINE* pMixerLine, HMIXER hMixer )
{
	if( pMixerLine->cControls < 1  ||	 mpMixerControl != NULL	)  return -1 ;

	int nRet = -1 ;
	MMRESULT mmr ;
	mhMixer = hMixer ;

	mnControlCount = pMixerLine->cControls ;
	mpMixerControl = new MIXERCONTROL[mnControlCount] ;
	if( !mpMixerControl )
		return -1 ;

	mMixerLineControls.cbStruct      = sizeof(MIXERLINECONTROLS);
	mMixerLineControls.dwLineID      = pMixerLine->dwLineID ;
	mMixerLineControls.dwControlType = 0 ;
	mMixerLineControls.cControls     = pMixerLine->cControls ;
	mMixerLineControls.cbmxctrl      = sizeof(MIXERCONTROL) ;
	mMixerLineControls.pamxctrl      = mpMixerControl ;

	// 获取线路控制器信息
	mmr = mixerGetLineControls( (HMIXEROBJ)hMixer, &mMixerLineControls,MIXER_GETLINECONTROLSF_ALL ) ;
	if( mmr != MMSYSERR_NOERROR )	
	{
		delete [] mpMixerControl ;
		mpMixerControl = NULL ;
		return -1 ;					
	}

	memcpy( (void*)&mpMixerLine, (void*)pMixerLine, sizeof(MIXERLINE) ) ;

	mpbHaveStateInfo = (bool*)new bool[mnControlCount] ;
	mpMixerControlStateInfo = new MIXERCONTROLDETAILS[mnControlCount] ;

	// 获取控制器信息
	if( mpbHaveStateInfo && mpMixerControlStateInfo )
		nRet = InitMixerControlDetails() ;

	if( nRet<0 )			
	{
		CleanupHeap() ;
		return -1 ;			
	}

	return mnControlCount ;
}

//获取控制器信息
int CMixerControl::GetControlInfoByIndex( int nIndex, MIXERCONTROL* pMC ) 
{
	if( nIndex < 0  ||  nIndex > (int)mnControlCount )
		return -1 ;		

	memcpy( (void*)pMC, (void*)&mMixerLineControls.pamxctrl[nIndex], sizeof(MIXERCONTROL) ) ;

	return mpMixerLine.cChannels ;	
}

//获取控制状态
int CMixerControl::GetControStateData(DWORD dwControlID, void* pDetails, unsigned int* nSizeOfDetails)
{
	unsigned int n1 ;
	unsigned int nSize ;
	MMRESULT mmr ;

	// 获取控制器关联的id
	for( n1=0 ; n1<mnControlCount ; n1++ )
	{
		if( mpMixerControl[n1].dwControlID == dwControlID )
			break ;
	}

	if( n1 >= mnControlCount )	
	{
		*nSizeOfDetails = 0 ;
		return -3 ;	
	}

	//获取控制器详细信息
	nSize = mpMixerControlStateInfo[n1].cbDetails * 
		mpMixerControlStateInfo[n1].cChannels ;
	if( nSize > *nSizeOfDetails )	 
	{
		*nSizeOfDetails = nSize ;
		return -2 ;
	}

	memset( (void*)pDetails, 0, *nSizeOfDetails ) ;

	mpMixerControlStateInfo[n1].cbStruct = sizeof(MIXERCONTROLDETAILS) ;
	mpMixerControlStateInfo[n1].dwControlID = dwControlID ;

	mpMixerControlStateInfo[n1].hwndOwner = NULL ;	
	mpMixerControlStateInfo[n1].cMultipleItems = 0 ; 

	if( mpMixerControl[n1].dwControlType & MIXERCONTROL_CONTROLTYPE_CUSTOM )
		mpMixerControlStateInfo[n1].cChannels = 1 ;
	else
		mpMixerControlStateInfo[n1].cChannels = mpMixerLine.cChannels ;

	mmr = mixerGetControlDetails( (HMIXEROBJ)mhMixer, &mpMixerControlStateInfo[n1], 
		MIXER_GETCONTROLDETAILSF_VALUE ) ;	
	if( mmr != MMSYSERR_NOERROR )
		return -1 ;

	mpbHaveStateInfo[n1] = TRUE ;
	memcpy( (void*)pDetails, (void*)mpMixerControlStateInfo[n1].paDetails, 
		mpMixerControlStateInfo[n1].cChannels * mpMixerControlStateInfo[n1].cbDetails ) ;
	return 0 ;
}

//设置控制状态
int CMixerControl::SetControStateData(DWORD dwControlID, void* pDetails, unsigned int* nSizeOfDetails)
{
	unsigned int n1 ;
	unsigned int nSize ;
	MMRESULT mmr ;

	// 获取控制器关联的id
	for( n1=0 ; n1<mnControlCount ; n1++ )
	{
		if( mpMixerControl[n1].dwControlID == dwControlID )
			break ;
	}

	if( n1 >= mnControlCount )	
	{
		*nSizeOfDetails = 0 ;
		return -3 ;	
	}

	if(	!mpbHaveStateInfo[n1] )	
		return -1 ;				

	//获取对应的控制明细信息
	nSize = mpMixerControlStateInfo[n1].cbDetails * 
		mpMixerControlStateInfo[n1].cChannels ;

	if( nSize > *nSizeOfDetails )	
	{
		*nSizeOfDetails = nSize ;	
		return -2 ;	
	}

	mpMixerControlStateInfo[n1].cbStruct = sizeof(MIXERCONTROLDETAILS) ;
	mpMixerControlStateInfo[n1].dwControlID = dwControlID ;

	mpMixerControlStateInfo[n1].hwndOwner = NULL ;	
	mpMixerControlStateInfo[n1].cMultipleItems = 0 ; 

	if( mpMixerControl[n1].dwControlType & MIXERCONTROL_CONTROLTYPE_CUSTOM )
		mpMixerControlStateInfo[n1].cChannels = 1 ;
	else
		mpMixerControlStateInfo[n1].cChannels = mpMixerLine.cChannels ;

	memcpy( (void*)mpMixerControlStateInfo[n1].paDetails, pDetails, 
		mpMixerLine.cChannels * mpMixerControlStateInfo[n1].cbDetails ) ;

	mmr = mixerSetControlDetails( (HMIXEROBJ)mhMixer, &mpMixerControlStateInfo[n1], 
		MIXER_SETCONTROLDETAILSF_VALUE ) ;
	if( mmr != MMSYSERR_NOERROR )
		return -1 ;

	return 0 ;
}

int CMixerControl::SetControSelStateData(DWORD dwID,DWORD dwLineId, unsigned int index,bool bEnable)
{

	unsigned int n1 ;
	unsigned int nSize ;
	MMRESULT mmr ;

	// 获取控制器关联的id
	for( n1=0 ; n1<mnControlCount ; n1++ )
	{
		if( mpMixerControl[n1].dwControlID == dwID )
			break ;
	}

	if( n1 >= mnControlCount )	
	{
		return -3 ;	
	}

	if(	!mpbHaveStateInfo[n1] )	
		return -1 ;	

	MIXERCONTROL   mxc; 
	MIXERLINECONTROLS   mxlc; 
	mxlc.cbStruct   =   sizeof(MIXERLINECONTROLS); 
	mxlc.dwLineID   =   dwLineId; 
	mxlc.dwControlType   =   MIXERCONTROL_CONTROLTYPE_MUX; 
	mxlc.cControls   =   1; 
	mxlc.cbmxctrl   =   sizeof(MIXERCONTROL); 
	mxlc.pamxctrl   =   &mxc;   

	if   (::mixerGetLineControls((HMIXEROBJ)mhMixer, 
		&mxlc, 
		MIXER_OBJECTF_HMIXER   | 
		MIXER_GETLINECONTROLSF_ONEBYTYPE) 
		!=   MMSYSERR_NOERROR)   return   -1;   

	MIXERCONTROLDETAILS_BOOLEAN   mxcdMute[8]; 
	MIXERCONTROLDETAILS   mxcd; 
	mxcd.cbStruct   =   sizeof(MIXERCONTROLDETAILS); 
	mxcd.dwControlID   =   mxc.dwControlID;
	mxcd.cChannels   =   1; 
	mxcd.cMultipleItems   =mxc.cMultipleItems;
	mxcd.cbDetails   =   sizeof(*mxcdMute); 
	mxcd.paDetails   =&mxcdMute; 
	if   (::mixerGetControlDetails((HMIXEROBJ)mhMixer, 
		&mxcd, 
		MIXER_OBJECTF_HMIXER   | 
		MIXER_GETCONTROLDETAILSF_VALUE) 
		!=   MMSYSERR_NOERROR)   return   -1; 

	int   j; 
	for(j=0;j <(int)mxc.cMultipleItems;j++) 
		mxcdMute[j].fValue=false; 
	if (bEnable)
	{
		mxcdMute[index].fValue=true;
	}
	else
	{
		index = (++index>7?0:index);
		mxcdMute[index].fValue=true;
	}

	if   (::mixerSetControlDetails((HMIXEROBJ)mhMixer, 
		&mxcd, 
		MIXER_OBJECTF_HMIXER   | 
		MIXER_GETCONTROLDETAILSF_VALUE) 
		!=   MMSYSERR_NOERROR)   return   -1;  
	return 0;
}

//获取选择状态
int CMixerControl::GetControSelStateData(DWORD dwID,DWORD dwLineId, unsigned int index,bool& bEnable)
{
	unsigned int n1 ;
	unsigned int nSize ;
	MMRESULT mmr ;

	// 获取控制器关联的id
	for( n1=0 ; n1<mnControlCount ; n1++ )
	{
		if( mpMixerControl[n1].dwControlID == dwID )
			break ;
	}

	if( n1 >= mnControlCount )	
	{
		return -3 ;	
	}

	if(	!mpbHaveStateInfo[n1] )	
		return -1 ;	

	MIXERCONTROL   mxc; 
	MIXERLINECONTROLS   mxlc; 
	mxlc.cbStruct   =   sizeof(MIXERLINECONTROLS); 
	mxlc.dwLineID   =   dwLineId; 
	mxlc.dwControlType   =   MIXERCONTROL_CONTROLTYPE_MUX; 
	mxlc.cControls   =   1; 
	mxlc.cbmxctrl   =   sizeof(MIXERCONTROL); 
	mxlc.pamxctrl   =   &mxc;   

	if   (::mixerGetLineControls((HMIXEROBJ)mhMixer, 
		&mxlc, 
		MIXER_OBJECTF_HMIXER   | 
		MIXER_GETLINECONTROLSF_ONEBYTYPE) 
		!=   MMSYSERR_NOERROR)   return   -1;   

	MIXERCONTROLDETAILS_BOOLEAN   mxcdMute[8]; 
	MIXERCONTROLDETAILS   mxcd; 
	mxcd.cbStruct   =   sizeof(MIXERCONTROLDETAILS); 
	mxcd.dwControlID   =   mxc.dwControlID;
	mxcd.cChannels   =   1; 
	mxcd.cMultipleItems   =mxc.cMultipleItems;
	mxcd.cbDetails   =   sizeof(*mxcdMute); 
	mxcd.paDetails   =&mxcdMute; 
	if   (::mixerGetControlDetails((HMIXEROBJ)mhMixer, 
		&mxcd, 
		MIXER_OBJECTF_HMIXER   | 
		MIXER_GETCONTROLDETAILSF_VALUE) 
		!=   MMSYSERR_NOERROR)   return   -1; 

	bEnable = (mxcdMute[index].fValue==1);
	return 0;

}

//初始化控制明细
int CMixerControl::InitMixerControlDetails()
{
	unsigned int n1 ;
	for( n1=0 ; n1<mnControlCount ; n1++ )
	{
		mpbHaveStateInfo[n1] = FALSE ;

		mpMixerControlStateInfo[n1].cbStruct = sizeof(MIXERCONTROLDETAILS) ;
		mpMixerControlStateInfo[n1].dwControlID = 0 ;
		mpMixerControlStateInfo[n1].hwndOwner = NULL ;	

		if( mpMixerControl[n1].dwControlType & MIXERCONTROL_CONTROLTYPE_CUSTOM )
			mpMixerControlStateInfo[n1].cChannels = 1 ;
		else
			mpMixerControlStateInfo[n1].cChannels = mpMixerLine.cChannels ;

		if( mpMixerControl[n1].fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
			mpMixerControlStateInfo[n1].cMultipleItems = mpMixerControl[n1].cMultipleItems ;
		else if( mpMixerControl[n1].dwControlType & MIXERCONTROL_CONTROLTYPE_CUSTOM  && 
			mpMixerControl[n1].fdwControl & MIXER_SETCONTROLDETAILSF_CUSTOM )
			mpMixerControlStateInfo[n1].cMultipleItems = (DWORD)mpMixerControlStateInfo[n1].hwndOwner ;
		else
			mpMixerControlStateInfo[n1].cMultipleItems = 0 ;

		DWORD dwClass = mpMixerControl[n1].dwControlType & MIXERCONTROL_CT_CLASS_MASK ;
		switch( dwClass ) 
		{
		case MIXERCONTROL_CT_CLASS_FADER:	//MIXERCONTROLDETAILS_UNSIGNED
			mpMixerControlStateInfo[n1].cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED) ;
			mpControlData = (void*)new MIXERCONTROLDETAILS_UNSIGNED[mpMixerControlStateInfo[n1].cChannels] ;
			mpMixerControlStateInfo[n1].paDetails = mpControlData ; 
			break ;
		case MIXERCONTROL_CT_CLASS_SLIDER:	//MIXERCONTROLDETAILS_SIGNED
			mpMixerControlStateInfo[n1].cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED) ;
			mpControlData = (void*)new MIXERCONTROLDETAILS_SIGNED[mpMixerControlStateInfo[n1].cChannels] ;
			mpMixerControlStateInfo[n1].paDetails = mpControlData ;
			break ;
		case MIXERCONTROL_CT_CLASS_SWITCH:	//MIXERCONTROLDETAILS_BOOLEAN
			mpMixerControlStateInfo[n1].cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN) ;
			mpControlData = (void*)new MIXERCONTROLDETAILS_BOOLEAN[mpMixerControlStateInfo[n1].cChannels] ;
			mpMixerControlStateInfo[n1].paDetails = mpControlData ;
			break ;
		case MIXERCONTROL_CT_CLASS_CUSTOM:	
			mpMixerControlStateInfo[n1].cbDetails = mpMixerControl[n1].Metrics.cbCustomData ;
			mpControlData = (void*)new unsigned char[mpMixerControl[n1].Metrics.cbCustomData] ;
			mpMixerControlStateInfo[n1].paDetails = mpControlData ;
			break ;
		case MIXERCONTROL_CT_CLASS_METER:	// MIXERCONTROLDETAILS_BOOLEAN, MIXERCONTROLDETAILS_SIGNED, MIXERCONTROLDETAILS_UNSIGNED
		case MIXERCONTROL_CT_CLASS_NUMBER:	// MIXERCONTROLDETAILS_SIGNED and MIXERCONTROLDETAILS_UNSIGNED 
		case MIXERCONTROL_CT_CLASS_LIST:	// MIXERCONTROLDETAILS_BOOLEAN, MIXERCONTROLDETAILS_LISTTEXT
		case MIXERCONTROL_CT_CLASS_TIME:	// 不支持
			mpMixerControlStateInfo[n1].paDetails = NULL;
			break;
		}
	}
	return 0 ;
}