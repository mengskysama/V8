/*
  Memory layout
*/
#include <stdio.h>
#include "aacdecoder.h"
#include "aac_ram.h"
#include "FloatFR.h"
#include "LibMutex.h"
#include <map>
using namespace std;

/*
  Static memory areas, must not be overwritten in other sections of the decoder
*/

struct CAacDecoderDataInfo
{
public:
	CAacDecoderDataInfo():bChannelInfoInit(false){}
	bool bChannelInfoInit;

	/* The structure CAacDecoderStaticChannelInfo contains the static sideinfo which is needed
	for the decoding of one aac channel. Dimension: #AacDecoderChannels */
	CAacDecoderStaticChannelInfo AacDecoderStaticChannelInfo[Channels];

	/* The structure CStreamInfo contains the streaming information. */
	CStreamInfo StreamInfo;

	/* The structure CPnsStaticInterChannelData contains the random number generator state and the pns frame counter */
	CPnsStaticInterChannelData PnsStaticInterChannelData;

	/* Overlap buffer */
	float OverlapBuffer[Channels*OverlapBufferSize];

	CConcealmentInfo ConcealmentInfo[Channels];
	/*!
	Dynamic memory areas, might be reused in other algorithm sections,
	e.g. the sbr decoder
	*/

	/* AAC Decoder Workbuffer */
	float WorkBufferCore[Channels*MaximumBinsLong];


#ifdef LP_SBR_ONLY
	CAacDecoderChannelInfo AacDecoderChannelInfo[Channels];

	CAacDecoderChannelInfo *pAacDecoderChannelInfoInit[Channels] =
	{
		&AacDecoderChannelInfo[0],
		&AacDecoderChannelInfo[1]
	};
#else
	float InterimResult[MaximumBinsLong];

	CAacDecoderChannelInfo *pAacDecoderChannelInfoInit[Channels];
#endif

	float TimeDataFloat[4*MaximumBinsLong];
	CAacDecoderDynamicData *pAacDecoderDynamicDataInit[Channels];
	CAacDecoderDynamicCommonData *pAacDecoderDynamicCommonDataInit;
};

static map<unsigned int,CAacDecoderDataInfo*> g_mapAacDecoderDataInfo;
typedef map<unsigned int,CAacDecoderDataInfo*>::iterator CAacDecoderDataInfoIterator;
LibMutex g_mapAacDecoderDataInfoMutex;

void ReleaseMapAacDecoderDataInfo(unsigned int keyflag)
{
	LibMutexLocker locker(&g_mapAacDecoderDataInfoMutex);
	CAacDecoderDataInfoIterator iter = g_mapAacDecoderDataInfo.find(keyflag);
	if (iter!=g_mapAacDecoderDataInfo.end())
	{
		delete iter->second;
		g_mapAacDecoderDataInfo.erase(iter);
	}
}

CAacDecoderDataInfo* getAacDecoderDataInfo(unsigned int flag)
{
	CAacDecoderDataInfoIterator iter = g_mapAacDecoderDataInfo.find(flag);
	if (iter!=g_mapAacDecoderDataInfo.end())
	{
		return iter->second;
	}
	else
	{
		LibMutexLocker locker(&g_mapAacDecoderDataInfoMutex);
		CAacDecoderDataInfo* pAacDecoderDataInfo = new CAacDecoderDataInfo();
		g_mapAacDecoderDataInfo.insert(pair<unsigned int,CAacDecoderDataInfo*>(flag,pAacDecoderDataInfo));
		return pAacDecoderDataInfo;
	}
}

CAacDecoderStaticChannelInfo* getCAacDecoderStaticChannelInfo(unsigned int flag,int channel)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->AacDecoderStaticChannelInfo[channel];
}

CStreamInfo* getCStreamInfo(unsigned int flag)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->StreamInfo;
}

CPnsStaticInterChannelData* getCPnsStaticInterChannelData(unsigned int flag)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->PnsStaticInterChannelData;
}

float* getOverlapBuffer(unsigned int flag,unsigned int pos)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->OverlapBuffer[pos];
}

CConcealmentInfo* getConcealmentInfo(unsigned int flag,unsigned int pos)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->ConcealmentInfo[pos];
}

float* getWorkBufferCoreArray(unsigned int flag,int pos)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->WorkBufferCore[pos];
}

float* getInterimResult(unsigned int flag,int pos)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->InterimResult[pos];
}

void DecoderChannelInfoInit(unsigned int flag)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	if (!pAacDataInfo->bChannelInfoInit)
	{
		//pAacDecoderChannelInfoInit init
		for (int i=0;i<min(2,Channels);i++)
		{
			pAacDataInfo->pAacDecoderChannelInfoInit[i] = (CAacDecoderChannelInfo *) &pAacDataInfo->InterimResult[i*sizeof(CAacDecoderChannelInfo)/sizeof(float)];
		}
		pAacDataInfo->pAacDecoderDynamicDataInit[0] = (CAacDecoderDynamicData *) &pAacDataInfo->TimeDataFloat[3*MaximumBinsLong];
		pAacDataInfo->pAacDecoderDynamicDataInit[1] = (CAacDecoderDynamicData *) &pAacDataInfo->TimeDataFloat[3*MaximumBinsLong+sizeof(CAacDecoderDynamicData)/sizeof(float)];

		pAacDataInfo->pAacDecoderDynamicCommonDataInit = (CAacDecoderDynamicCommonData *) &pAacDataInfo->TimeDataFloat[3*MaximumBinsLong+2*sizeof(CAacDecoderDynamicData)/sizeof(float)];
		pAacDataInfo->bChannelInfoInit = true;
	}
}

CAacDecoderChannelInfo *getAacDecoderChannelInfoInit(unsigned int flag,int channel)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	DecoderChannelInfoInit(flag);
	return pAacDataInfo->pAacDecoderChannelInfoInit[channel];
}

float* getTimeDataFloat(unsigned int flag,int pos)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	return &pAacDataInfo->TimeDataFloat[pos];
}

CAacDecoderDynamicData *getAacDecoderDynamicDataInit(unsigned int flag,int channel)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	DecoderChannelInfoInit(flag);
	return pAacDataInfo->pAacDecoderDynamicDataInit[channel];
}

CAacDecoderDynamicCommonData *getAacDecoderDynamicCommonDataInit(unsigned int flag)
{
	CAacDecoderDataInfo* pAacDataInfo = getAacDecoderDataInfo(flag);
	DecoderChannelInfoInit(flag);
	return pAacDataInfo->pAacDecoderDynamicCommonDataInit;
}


