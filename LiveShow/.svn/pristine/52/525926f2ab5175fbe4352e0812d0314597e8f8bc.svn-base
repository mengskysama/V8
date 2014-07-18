#include "StdAfx.h"
#include "MixerChannel.h"
#include "Mixer.h"
#include "MixerControl.h"
#include "MixerLine.h"
#include <string>

using namespace std;

#define RESOURCE_ID_START 100

CMixerChannel::CMixerChannel(void)
{
	mhMixer = NULL ;
	mpCMixer = NULL;
	bMixerInit = false;
    mpCtrlUT = NULL;
	nCtrlUTIndex = 0;
	nCtrlUTCount = 0;
	nextResourceID = RESOURCE_ID_START;
}

CMixerChannel::~CMixerChannel(void)
{
	if( mpCMixer ) delete [] mpCMixer ;
	mpCMixer = NULL;

	if (mpCtrlUT) delete [] mpCtrlUT;
	mpCtrlUT = NULL;

	if (mhMixer) mixerClose(mhMixer);
	mhMixer = (HMIXER)-1;
}

void CMixerChannel::Init(HWND hWnd)
{
	if (bMixerInit) return;
	bMixerInit = true;
	unsigned int mixerIndex = 0;
	unsigned int groupIndex = 0;
	unsigned int nsize = 0;
	MIXERLINE mixerLine ;
	MIXERCONTROL mixerControl ;
	strucChannelData channelData ;
	mMixerCount = mixerGetNumDevs();
	mpCMixer = new CMixer[mMixerCount] ;
	this->hMixerWnd = hWnd;
	nCtrlUTCount = 0;

	if( mpCMixer )
	{
		for( mixerIndex=0 ; mixerIndex<mMixerCount  ; mixerIndex++ )
		{
			mpCMixer[mixerIndex].Init( mixerIndex, hWnd ) ;
			unsigned lineIndex;
			for (lineIndex=0; lineIndex < mpCMixer[mixerIndex].GetLineCount(); lineIndex++)
			{
				nCtrlUTCount += mpCMixer[mixerIndex].GetConnectionCount(lineIndex);
				nCtrlUTCount ++;
			}
		}
	}
	nCtrlUTCount *= 3;
	mpCtrlUT = new strucCtrlUT[nCtrlUTCount];
	memset(mpCtrlUT,0,sizeof(strucCtrlUT)*nCtrlUTCount);

	for( mixerIndex=0 ; mixerIndex<mMixerCount  ; mixerIndex++ )
	{
		unsigned int lineIndex;
		unsigned int lineCount =  mpCMixer[mixerIndex].GetLineCount();
		for (lineIndex=0; lineIndex < lineCount; lineIndex++)
		{
			mpCMixer[mixerIndex].GetLineInfo( lineIndex, &mixerLine ) ;
			unsigned int connCount = mpCMixer[mixerIndex].GetConnectionCount(lineIndex);
			unsigned int ncontrolIndex = mpCMixer[mixerIndex].GetLineControlCount(lineIndex);
			bool bSelect = false;
			DWORD dwlineId = mixerLine.dwLineID;
			if (mixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_DST_SPEAKERS)
			{
				OutputDebugStr(L"MIXERLINE_COMPONENTTYPE_DST_SPEAKERS\n");
				bSelect = false;
			}
			else if (mixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_DST_WAVEIN)
			{
				OutputDebugStr(L"MIXERLINE_COMPONENTTYPE_DST_WAVEIN\n");
				if (connCount > 1)
				{
					bSelect = true;
				}
			}

			if (ncontrolIndex!=0)
			{
				int n1 = 0;
				do 
				{
					mpCMixer[mixerIndex].GetLineControlMetrics(lineIndex, n1, &mixerControl);
					nsize = sizeof(channelData.stateData);
					mpCMixer[mixerIndex].GetLineControlState(lineIndex,n1,&channelData.stateData, &nsize);
					if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_VOLUME) )
					{
						//记录左右声道平衡调节
						memcpy_s(&mpCtrlUT[groupIndex].szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR),&mixerLine.szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR));
						mpCtrlUT[groupIndex].ctrlType = Balance;
						mpCtrlUT[groupIndex].groupIndex = groupIndex;
						mpCtrlUT[groupIndex].mixerIndex = mixerIndex;
						mpCtrlUT[groupIndex].lineIndex = lineIndex;
						mpCtrlUT[groupIndex].connIndex = -1;
						mpCtrlUT[groupIndex].controlIndex = n1;
						mpCtrlUT[groupIndex].ascIndex = 0;
					    mpCtrlUT[groupIndex].desIndex = connCount;
						mpCtrlUT[groupIndex].muteType =(bSelect?kSelec:kMute);
						mpCtrlUT[groupIndex].dwLineId = dwlineId;
						mpCtrlUT[groupIndex].mcomponentType = mixerLine.dwComponentType;
						mpCtrlUT[groupIndex].sChannelData.nChannels = mixerLine.cChannels;
						mpCtrlUT[groupIndex].sChannelData.stateData.Left = channelData.stateData.Left;
						mpCtrlUT[groupIndex].sChannelData.stateData.Right = channelData.stateData.Right;
						mpCtrlUT[groupIndex].dwMixerCtrlID = mixerControl.dwControlID; //
						mpCtrlUT[groupIndex].dwType = mixerLine.Target.dwType;
						mpCtrlUT[groupIndex].dwDeviceID = mixerLine.Target.dwDeviceID;
						mpCtrlUT[groupIndex].cMultipleItems = mixerControl.cMultipleItems;
						groupIndex++;
					}

					if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_VOLUME) )
					{
						//声音大小
						memcpy_s(&mpCtrlUT[groupIndex].szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR),&mixerLine.szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR));
						mpCtrlUT[groupIndex].ctrlType = Volume;
						mpCtrlUT[groupIndex].groupIndex = groupIndex;
						mpCtrlUT[groupIndex].mixerIndex = mixerIndex;
						mpCtrlUT[groupIndex].lineIndex = lineIndex;
						mpCtrlUT[groupIndex].connIndex = -1;
						mpCtrlUT[groupIndex].controlIndex = n1;
						mpCtrlUT[groupIndex].ascIndex = 0;
						mpCtrlUT[groupIndex].desIndex = connCount;
						mpCtrlUT[groupIndex].muteType =(bSelect?kSelec:kMute);
						mpCtrlUT[groupIndex].dwLineId = dwlineId;
						mpCtrlUT[groupIndex].mcomponentType = mixerLine.dwComponentType;
						mpCtrlUT[groupIndex].sChannelData.nChannels = mixerLine.cChannels;
						mpCtrlUT[groupIndex].sChannelData.stateData.Left = channelData.stateData.Left;
						mpCtrlUT[groupIndex].sChannelData.stateData.Right = channelData.stateData.Right;
						mpCtrlUT[groupIndex].dwMixerCtrlID = mixerControl.dwControlID;//
						mpCtrlUT[groupIndex].dwType = mixerLine.Target.dwType;
						mpCtrlUT[groupIndex].dwDeviceID = mixerLine.Target.dwDeviceID;
						mpCtrlUT[groupIndex].cMultipleItems = mixerControl.cMultipleItems;
						groupIndex++;
					}

					if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MUTE))
					{
						//记录CheckBox调节
						memcpy_s(&mpCtrlUT[groupIndex].szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR),&mixerLine.szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR));
						mpCtrlUT[groupIndex].ctrlType = Mute;
						mpCtrlUT[groupIndex].groupIndex = groupIndex;
						mpCtrlUT[groupIndex].mixerIndex = mixerIndex;
						mpCtrlUT[groupIndex].lineIndex = lineIndex;
						mpCtrlUT[groupIndex].connIndex = -1;
						mpCtrlUT[groupIndex].controlIndex = n1;
						mpCtrlUT[groupIndex].ascIndex = 0;
						mpCtrlUT[groupIndex].desIndex = connCount;
						mpCtrlUT[groupIndex].muteType =(bSelect?kSelec:kMute);
						mpCtrlUT[groupIndex].dwLineId = dwlineId;
						mpCtrlUT[groupIndex].mcomponentType = mixerLine.dwComponentType;
						mpCtrlUT[groupIndex].sChannelData.nChannels = mixerLine.cChannels;
						mpCtrlUT[groupIndex].sChannelData.stateData.Left = channelData.stateData.Left;
						mpCtrlUT[groupIndex].sChannelData.stateData.Right = channelData.stateData.Right;
						mpCtrlUT[groupIndex].dwMixerCtrlID = mixerControl.dwControlID;//
						mpCtrlUT[groupIndex].dwType = mixerLine.Target.dwType;
						mpCtrlUT[groupIndex].dwDeviceID = mixerLine.Target.dwDeviceID;
						mpCtrlUT[groupIndex].cMultipleItems = mixerControl.cMultipleItems;
						groupIndex++;
					}
				} while (++n1 < ncontrolIndex);
			}
			unsigned int connIndex;
			for (connIndex=0; connIndex<connCount; connIndex++ )
			{
				mpCMixer[mixerIndex].GetConnectionInfo(lineIndex,connIndex,&mixerLine);
				unsigned int ncontrolIndex = mpCMixer[mixerIndex].GetConnectionControlCount(lineIndex,connIndex);
				if (ncontrolIndex!=0)
				{
					int n1 = 0;
					do 
					{
						mpCMixer[mixerIndex].GetConnectionControlMetrics(lineIndex, connIndex,n1, &mixerControl);
						nsize = sizeof(channelData.stateData);
						mpCMixer[mixerIndex].GetConnectionControlState(lineIndex,connIndex,n1,&channelData.stateData, &nsize);
// 
// 						if (MIXERLINE_COMPONENTTYPE_SRC_LINE == mixerLine.dwComponentType)
// 						{
// 							TRACE(L"11111111111111111111111111111111111\n");
// 						}


						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_VOLUME))
						{
							//记录左右声道平衡调节
							memcpy_s(&mpCtrlUT[groupIndex].szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR),&mixerLine.szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR));
							mpCtrlUT[groupIndex].ctrlType = Balance;
							mpCtrlUT[groupIndex].groupIndex = groupIndex;
							mpCtrlUT[groupIndex].mixerIndex = mixerIndex;
							mpCtrlUT[groupIndex].lineIndex = lineIndex;
							mpCtrlUT[groupIndex].connIndex = connIndex;
							mpCtrlUT[groupIndex].controlIndex = n1;
							mpCtrlUT[groupIndex].ascIndex = connIndex+1;
							mpCtrlUT[groupIndex].desIndex = connCount-1-connIndex;
							mpCtrlUT[groupIndex].muteType =(bSelect?kSelec:kMute);
							mpCtrlUT[groupIndex].dwLineId = dwlineId;
							mpCtrlUT[groupIndex].mcomponentType = mixerLine.dwComponentType;
							mpCtrlUT[groupIndex].sChannelData.nChannels = mixerLine.cChannels;
							mpCtrlUT[groupIndex].sChannelData.stateData.Left = channelData.stateData.Left;
							mpCtrlUT[groupIndex].sChannelData.stateData.Right = channelData.stateData.Right;
							mpCtrlUT[groupIndex].dwMixerCtrlID = mixerControl.dwControlID; //
							mpCtrlUT[groupIndex].dwType = mixerLine.Target.dwType;
							mpCtrlUT[groupIndex].dwDeviceID = mixerLine.Target.dwDeviceID;
							mpCtrlUT[groupIndex].cMultipleItems = mixerControl.cMultipleItems;
							groupIndex++;
						}

						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_VOLUME))
						{
							//声音大小 Volume
							memcpy_s(&mpCtrlUT[groupIndex].szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR),&mixerLine.szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR));
							mpCtrlUT[groupIndex].ctrlType = Volume;
							mpCtrlUT[groupIndex].groupIndex = groupIndex;
							mpCtrlUT[groupIndex].mixerIndex = mixerIndex;
							mpCtrlUT[groupIndex].lineIndex = lineIndex;
							mpCtrlUT[groupIndex].connIndex = connIndex;
							mpCtrlUT[groupIndex].controlIndex = n1;
							mpCtrlUT[groupIndex].ascIndex = connIndex+1;
							mpCtrlUT[groupIndex].desIndex = connCount-1-connIndex;
							mpCtrlUT[groupIndex].muteType =(bSelect?kSelec:kMute);
							mpCtrlUT[groupIndex].dwLineId = dwlineId;
							mpCtrlUT[groupIndex].mcomponentType = mixerLine.dwComponentType;
							mpCtrlUT[groupIndex].sChannelData.nChannels = mixerLine.cChannels;
							mpCtrlUT[groupIndex].sChannelData.stateData.Left = channelData.stateData.Left;
							mpCtrlUT[groupIndex].sChannelData.stateData.Right = channelData.stateData.Right;
							mpCtrlUT[groupIndex].dwMixerCtrlID = mixerControl.dwControlID;//
							mpCtrlUT[groupIndex].dwType = mixerLine.Target.dwType;
							mpCtrlUT[groupIndex].dwDeviceID = mixerLine.Target.dwDeviceID;
							mpCtrlUT[groupIndex].cMultipleItems = mixerControl.cMultipleItems;
							groupIndex++;
						}

						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MUTE))
						{
							//Mute CheckBox调节
							memcpy_s(&mpCtrlUT[groupIndex].szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR),&mixerLine.szName[0],MIXER_LONG_NAME_CHARS*sizeof(WCHAR));
							mpCtrlUT[groupIndex].ctrlType = Mute;
							mpCtrlUT[groupIndex].groupIndex = groupIndex;
							mpCtrlUT[groupIndex].mixerIndex = mixerIndex;
							mpCtrlUT[groupIndex].lineIndex = lineIndex;
							mpCtrlUT[groupIndex].connIndex = connIndex;
							mpCtrlUT[groupIndex].controlIndex = n1;
							mpCtrlUT[groupIndex].ascIndex = connIndex+1;
							mpCtrlUT[groupIndex].desIndex = connCount-1-connIndex;
							mpCtrlUT[groupIndex].muteType =(bSelect?kSelec:kMute);
							mpCtrlUT[groupIndex].dwLineId = dwlineId;
							mpCtrlUT[groupIndex].mcomponentType = mixerLine.dwComponentType;
							mpCtrlUT[groupIndex].sChannelData.nChannels = mixerLine.cChannels;
							mpCtrlUT[groupIndex].sChannelData.stateData.Left = channelData.stateData.Left;
							mpCtrlUT[groupIndex].sChannelData.stateData.Right = channelData.stateData.Right;
							mpCtrlUT[groupIndex].dwMixerCtrlID = mixerControl.dwControlID;//
							mpCtrlUT[groupIndex].dwType = mixerLine.Target.dwType;
							mpCtrlUT[groupIndex].dwDeviceID = mixerLine.Target.dwDeviceID;
							mpCtrlUT[groupIndex].cMultipleItems = mixerControl.cMultipleItems;
							groupIndex++;
						}
/*
#define MIXERCONTROL_CONTROLTYPE_CUSTOM         (MIXERCONTROL_CT_CLASS_CUSTOM | MIXERCONTROL_CT_UNITS_CUSTOM)
#define MIXERCONTROL_CONTROLTYPE_BOOLEANMETER   (MIXERCONTROL_CT_CLASS_METER | MIXERCONTROL_CT_SC_METER_POLLED | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_SIGNEDMETER    (MIXERCONTROL_CT_CLASS_METER | MIXERCONTROL_CT_SC_METER_POLLED | MIXERCONTROL_CT_UNITS_SIGNED)
#define MIXERCONTROL_CONTROLTYPE_PEAKMETER      (MIXERCONTROL_CONTROLTYPE_SIGNEDMETER + 1)
#define MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER  (MIXERCONTROL_CT_CLASS_METER | MIXERCONTROL_CT_SC_METER_POLLED | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_BOOLEAN        (MIXERCONTROL_CT_CLASS_SWITCH | MIXERCONTROL_CT_SC_SWITCH_BOOLEAN | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_ONOFF          (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 1)
#define MIXERCONTROL_CONTROLTYPE_MUTE           (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 2)
#define MIXERCONTROL_CONTROLTYPE_MONO           (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 3)
#define MIXERCONTROL_CONTROLTYPE_LOUDNESS       (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 4)
#define MIXERCONTROL_CONTROLTYPE_STEREOENH      (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 5)
#define MIXERCONTROL_CONTROLTYPE_BASS_BOOST     (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 0x00002277)
#define MIXERCONTROL_CONTROLTYPE_BUTTON         (MIXERCONTROL_CT_CLASS_SWITCH | MIXERCONTROL_CT_SC_SWITCH_BUTTON | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_DECIBELS       (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_DECIBELS)
#define MIXERCONTROL_CONTROLTYPE_SIGNED         (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_SIGNED)
#define MIXERCONTROL_CONTROLTYPE_UNSIGNED       (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_PERCENT        (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_PERCENT)
#define MIXERCONTROL_CONTROLTYPE_SLIDER         (MIXERCONTROL_CT_CLASS_SLIDER | MIXERCONTROL_CT_UNITS_SIGNED)
#define MIXERCONTROL_CONTROLTYPE_PAN            (MIXERCONTROL_CONTROLTYPE_SLIDER + 1)
#define MIXERCONTROL_CONTROLTYPE_QSOUNDPAN      (MIXERCONTROL_CONTROLTYPE_SLIDER + 2)
#define MIXERCONTROL_CONTROLTYPE_FADER          (MIXERCONTROL_CT_CLASS_FADER | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_VOLUME         (MIXERCONTROL_CONTROLTYPE_FADER + 1)
#define MIXERCONTROL_CONTROLTYPE_BASS           (MIXERCONTROL_CONTROLTYPE_FADER + 2)
#define MIXERCONTROL_CONTROLTYPE_TREBLE         (MIXERCONTROL_CONTROLTYPE_FADER + 3)
#define MIXERCONTROL_CONTROLTYPE_EQUALIZER      (MIXERCONTROL_CONTROLTYPE_FADER + 4)
#define MIXERCONTROL_CONTROLTYPE_SINGLESELECT   (MIXERCONTROL_CT_CLASS_LIST | MIXERCONTROL_CT_SC_LIST_SINGLE | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_MUX            (MIXERCONTROL_CONTROLTYPE_SINGLESELECT + 1)
#define MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT (MIXERCONTROL_CT_CLASS_LIST | MIXERCONTROL_CT_SC_LIST_MULTIPLE | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_MIXER          (MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT + 1)
#define MIXERCONTROL_CONTROLTYPE_MICROTIME      (MIXERCONTROL_CT_CLASS_TIME | MIXERCONTROL_CT_SC_TIME_MICROSECS | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_MILLITIME      (MIXERCONTROL_CT_CLASS_TIME | MIXERCONTROL_CT_SC_TIME_MILLISECS | MIXERCONTROL_CT_UNITS_UNSIGNED)
*/
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_CUSTOM))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_CUSTOM\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_BOOLEANMETER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_BOOLEANMETER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_SIGNEDMETER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_SIGNEDMETER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_PEAKMETER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_PEAKMETER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_BOOLEAN))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_BOOLEAN\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_ONOFF))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_ONOFF\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MUTE))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MUTE\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MONO))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MONO\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_LOUDNESS))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_LOUDNESS\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_STEREOENH))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_STEREOENH\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_BASS_BOOST))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_BASS_BOOST\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}

						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_BUTTON))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_BUTTON\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_DECIBELS))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_DECIBELS\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_SIGNED))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_SIGNED\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_UNSIGNED))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_UNSIGNED\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_PERCENT))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_PERCENT\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_SLIDER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_SLIDER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_PAN))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_PAN\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_QSOUNDPAN))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_QSOUNDPAN\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_FADER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_FADER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_VOLUME))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_VOLUME\n",mixerLine.szName);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_BASS))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_BASS\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_TREBLE))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_TREBLE\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_EQUALIZER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_EQUALIZER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_SINGLESELECT))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_SINGLESELECT\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MUX))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MUX\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MIXER))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MIXER\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MICROTIME))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MICROTIME\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}
						if( !(mixerControl.dwControlType ^ MIXERCONTROL_CONTROLTYPE_MILLITIME))
						{
							WCHAR buf[512] = {0};
							wsprintf(buf,L"%s = MIXERCONTROL_CONTROLTYPE_MILLITIME\n",&mixerLine.szName[0]);
							OutputDebugStr(buf);
						}

					} while (++n1 < ncontrolIndex);
				}
			}
		}
	}
}

// void CMixerChannel::ShowMixerCaps(CComboBox* pMixerCombo)
// {
// 	MIXERCAPS mixerCaps ;
// 	unsigned int n1 ;
// 	
// 	if (!pMixerCombo) return;
// 	pMixerCombo->ResetContent();
// 
// 	for( n1=0 ; n1<mMixerCount  ; n1++ )
// 	{
// 		mpCMixer[n1].GetMixerCaps( &mixerCaps )  ;
// 		pMixerCombo->InsertString( n1, mixerCaps.szPname ) ;
// 	}
// 	pMixerCombo->SetCurSel( 0 ) ;
// }
// 
// void CMixerChannel::ShowMixerLineControl(unsigned int nMixerLine,CComboBox* pMixerCombo)
// {
// 	if (!pMixerCombo) return;
// 	pMixerCombo->ResetContent();
// 	
// 	if (nMixerLine<0 || nMixerLine>mMixerCount) return;
// 
// 	MIXERCAPS mixerCaps ;
// 	CString str ;
// 	unsigned int n1  = 0;
// 	mpCMixer[nMixerLine].GetMixerCaps( &mixerCaps )  ;
// 
// 	do {
// 		str.Format( _T("%d"), n1 ) ;
// 		pMixerCombo->InsertString( n1++, str ) ;
// 	} while( n1<mpCMixer[nMixerLine].GetLineCount() ) ;
// 	pMixerCombo->SetCurSel( 0 ) ;
// }

void CMixerChannel::GetLineTargetTypeStr( DWORD dwType, std::string& strType ) 
{
	switch( dwType ) 
	{			
	case MIXERLINE_TARGETTYPE_AUX		: strType = "the auxiliary device"				; break ;
	case MIXERLINE_TARGETTYPE_MIDIIN	: strType = "the MIDI input device"				; break ;
	case MIXERLINE_TARGETTYPE_MIDIOUT	: strType = "the MIDI output device"			; break ;
	case MIXERLINE_TARGETTYPE_WAVEIN	: strType = "the waveform-audio input device"	; break ;
	case MIXERLINE_TARGETTYPE_WAVEOUT	: strType = "the waveform-audio output device"	; break ;
	case MIXERLINE_TARGETTYPE_UNDEFINED	: strType = "not bound to a defined media type"	; break ;
	default:
		strType = "Error - value is undefined"	;
	}
}

int CMixerChannel::UpdateMixerControlState( unsigned int groupId,const strucChannelSet& sSet) 
{
	unsigned int n1 ;
	unsigned int nSize ;
	sChannelData state ;
	int nRet = 0 ;
	for( n1=0 ; n1<nCtrlUTCount  ; n1++ )
	{
		if( mpCtrlUT[n1].groupIndex == groupId )
			break ;
	}

	if( n1 >= nCtrlUTCount ) return -1 ;
	nSize = sizeof(sChannelData) ;
	state.Right.dwValue = state.Left.dwValue = 0 ;
	switch( mpCtrlUT[n1].ctrlType )
	{
	case Volume:
		state.Left.dwValue = sSet.stateData.Left.dwValue;
		state.Right.dwValue = sSet.stateData.Right.dwValue;
		mpCMixer[mpCtrlUT[n1].mixerIndex].SetControlState(mpCtrlUT[groupId].dwMixerCtrlID,&state, &nSize);
		break;
	case Balance:
		state.Left.dwValue = sSet.stateData.Left.dwValue;
		state.Right.dwValue = sSet.stateData.Right.dwValue;
		mpCMixer[mpCtrlUT[n1].mixerIndex].SetControlState(mpCtrlUT[groupId].dwMixerCtrlID,&state, &nSize);
		break;
	case Mute:
		state.Right.dwValue = state.Left.dwValue = sSet.bChecked?1:0 ;
		mpCMixer[mpCtrlUT[n1].mixerIndex].SetControlState(mpCtrlUT[groupId].dwMixerCtrlID,&state, &nSize);
		break;
	default:
		nRet = -1 ;
	}
	return nRet;
}

int CMixerChannel::SetMixerControlSelState(unsigned int groupId,const strucChannelSet& sSet)
{
	unsigned int n1 ;
	unsigned int nSize ;
	sChannelData state ;
	int nRet = 0 ;
	for( n1=0 ; n1<nCtrlUTCount  ; n1++ )
	{
		if( mpCtrlUT[n1].groupIndex == groupId )
			break ;
	}

	if( n1 >= nCtrlUTCount ) return -1 ;
	nSize = sizeof(sChannelData) ;
	state.Right.dwValue = state.Left.dwValue = 0 ;
	switch( mpCtrlUT[n1].ctrlType )
	{
	case Volume: //对Volume设置选中或非选中状态 
		state.Right.dwValue = state.Left.dwValue = sSet.bChecked?1:0 ;
		mpCMixer[mpCtrlUT[n1].mixerIndex].SetControlSelState(mpCtrlUT[groupId].dwMixerCtrlID,mpCtrlUT[groupId].dwLineId,mpCtrlUT[groupId].desIndex,&state, &nSize);
		break;
	default:
		nRet = -1 ;
	}
	return nRet;
}

int CMixerChannel::GetMixerControlSelState(unsigned int groupId,strucChannelSet& sSet)
{
	unsigned int n1 ;
	unsigned int nSize ;
	sChannelData state ;
	int nRet = 0 ;
	for( n1=0 ; n1<nCtrlUTCount  ; n1++ )
	{
		if( mpCtrlUT[n1].groupIndex == groupId )
			break ;
	}

	if( n1 >= nCtrlUTCount ) return -1 ;
	nSize = sizeof(sChannelData) ;
	state.Right.dwValue = state.Left.dwValue = 0 ;
	switch( mpCtrlUT[n1].ctrlType )
	{
	case Volume: //对Volume设置选中或非选中状态 
		mpCMixer[mpCtrlUT[n1].mixerIndex].GetControlSelState(mpCtrlUT[groupId].dwMixerCtrlID,mpCtrlUT[groupId].dwLineId,mpCtrlUT[groupId].desIndex,&state, &nSize);
		sSet.bChecked = (state.Left.dwValue==1);
		break;
	default:
		nRet = -1 ;
	}
	return nRet;	
}

bool CMixerChannel::IsMixerControlSelState(unsigned int groupId)
{
	unsigned int n1 ;
	for( n1=0 ; n1<nCtrlUTCount  ; n1++ )
	{
		if( mpCtrlUT[n1].groupIndex == groupId )
			break ;
	}
	if( n1 >= nCtrlUTCount ) return false ;
	return (mpCtrlUT[n1].muteType == kSelec);

}

int CMixerChannel::GetMixerControlState( unsigned int groupId,strucChannelSet& sSet )
{
	unsigned int n1 ;
	unsigned int nSize ;
	sChannelData state ;
	int nRet = 0 ;
	for( n1=0 ; n1<nCtrlUTCount  ; n1++ )
	{
		if( mpCtrlUT[n1].groupIndex == groupId )
			break ;
	}
	if( n1 >= nCtrlUTCount ) return -1 ;
	nSize = sizeof(sChannelData) ;
	state.Right.dwValue = state.Left.dwValue = 0 ;
	switch( mpCtrlUT[n1].ctrlType )
	{
	case Volume:
		mpCMixer[mpCtrlUT[n1].mixerIndex].GetControlState(mpCtrlUT[groupId].dwMixerCtrlID,&state, &nSize);
		sSet.stateData.Left.dwValue = state.Left.dwValue;
		sSet.stateData.Right.dwValue = state.Right.dwValue;
		break;
	case Balance:
		mpCMixer[mpCtrlUT[n1].mixerIndex].GetControlState(mpCtrlUT[groupId].dwMixerCtrlID,&state, &nSize);
		sSet.stateData.Left.dwValue = state.Left.dwValue;
		sSet.stateData.Right.dwValue = state.Right.dwValue;
		break;
	case Mute:
		mpCMixer[mpCtrlUT[n1].mixerIndex].GetControlState(mpCtrlUT[groupId].dwMixerCtrlID,&state, &nSize);
		sSet.bChecked=(state.Left.dwValue==0 && state.Right.dwValue==0)?FALSE:TRUE;
		break;
	default:
		nRet = -1 ;
	}
	return nRet;
}

int CMixerChannel::GetMixerGroupId(unsigned int& groupId, const StrucMixerParam& sMixerParam)
{
	unsigned int nIndex = 0;
	int nSelGroupId = -1;
	for (nIndex = 0; nIndex<nCtrlUTCount; nIndex++)
	{
		WCHAR buf[512]={0};
		wsprintf(buf,L"name=%s index=%d mixerIndex=%d lineIndex=%d \n ",mpCtrlUT[nIndex].szName,nIndex,mpCtrlUT[nIndex].mixerIndex,mpCtrlUT[nIndex].lineIndex);
		OutputDebugStr(buf);
		if (mpCtrlUT[nIndex].ctrlType != sMixerParam.enctrlType ) continue;
		if (mpCtrlUT[nIndex].mcomponentType!=sMixerParam.nControlType) continue;
		if (mpCtrlUT[nIndex].dwType != sMixerParam.dwLineType && sMixerParam.dwLineType!=0) continue;
		if (sMixerParam.enType==MIXER_OUT)
		{
			if (mpCtrlUT[nIndex].mixerIndex + mpCtrlUT[nIndex].lineIndex == 0) 
				nSelGroupId = mpCtrlUT[nIndex].groupIndex;
		}
		else if (sMixerParam.enType==MIXER_IN)
		{
			if (mpCtrlUT[nIndex].mixerIndex + mpCtrlUT[nIndex].lineIndex == 1) 
				nSelGroupId = mpCtrlUT[nIndex].groupIndex;
		}
		else if (sMixerParam.enType==MIXER_OTHER)
		{
			if (mpCtrlUT[nIndex].mixerIndex + mpCtrlUT[nIndex].lineIndex > 1) 
				nSelGroupId = mpCtrlUT[nIndex].groupIndex;
		}
		if (nSelGroupId > 0) break;
	}
	groupId = (unsigned int)nSelGroupId;
	return nSelGroupId;
}	

int CMixerChannel::GetMixerGroupId(unsigned int& groupId,const std::wstring& str,const StrucMixerParam& sMixerParam)
{
	unsigned int nIndex = 0;
	int nSelGroupId = -1;
	for (nIndex = 0; nIndex<nCtrlUTCount; nIndex++)
	{
		if (sMixerParam.enType==MIXER_OUT)
		{
			std::wstring strName = mpCtrlUT[nIndex].szName;
			if(mpCtrlUT[nIndex].mixerIndex + mpCtrlUT[nIndex].lineIndex != 0) continue;
			if (strName.find(str) == wstring::npos || sMixerParam.enctrlType != mpCtrlUT[nIndex].ctrlType)
			{
				continue;
			}
			nSelGroupId = mpCtrlUT[nIndex].groupIndex;
		}
		else if (sMixerParam.enType==MIXER_IN)
		{
			std::wstring strName = mpCtrlUT[nIndex].szName;
			if (mpCtrlUT[nIndex].mixerIndex + mpCtrlUT[nIndex].lineIndex != 1) continue;
			if (strName.find(str) == wstring::npos || sMixerParam.enctrlType != mpCtrlUT[nIndex].ctrlType)
			{
				continue;
			}
			nSelGroupId = mpCtrlUT[nIndex].groupIndex;
		}
		else if (sMixerParam.enType==MIXER_OTHER)
		{
			std::wstring strName = mpCtrlUT[nIndex].szName;
			if (mpCtrlUT[nIndex].mixerIndex + mpCtrlUT[nIndex].lineIndex <1) continue;
			if (strName.find(str) == wstring::npos || sMixerParam.enctrlType != mpCtrlUT[nIndex].ctrlType)
			{
				continue;
			}
			nSelGroupId = mpCtrlUT[nIndex].groupIndex;
		}
		if (nSelGroupId > 0) break;
	}
	groupId = (unsigned int)nSelGroupId;
	return nSelGroupId;
}

//获取音频输入设备中选中的设备
int CMixerChannel::GetMixerControlSelGroupId(unsigned int& groupId)
{
	unsigned int nIndex = 0;
	int nSelGroupId = -1;
	for (nIndex = 0; nIndex<nCtrlUTCount; nIndex++)
	{
		if (mpCtrlUT[nIndex].ctrlType != Volume) continue;
		if (mpCtrlUT[nIndex].muteType != kSelec) continue;
		
		strucChannelSet sSet;
		if (GetMixerControlSelState(mpCtrlUT[nIndex].groupIndex,sSet)!=-1 && sSet.bChecked)
		{
			nSelGroupId = mpCtrlUT[nIndex].groupIndex;
			break;
		}
	}
	groupId = nSelGroupId;
	return nSelGroupId;
}
