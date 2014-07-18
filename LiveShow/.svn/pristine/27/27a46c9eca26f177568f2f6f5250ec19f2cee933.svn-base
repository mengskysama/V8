/*
  Memory layout
*/

#ifndef AAC_RAM_H
#define AAC_RAM_H

#include "channel.h"


//extern CConcealmentInfo ConcealmentInfo[Channels];

//extern CStreamInfo StreamInfo;
//extern CAacDecoderStaticChannelInfo AacDecoderStaticChannelInfo[Channels];

//extern CPnsStaticInterChannelData PnsStaticInterChannelData;

//extern float OverlapBuffer[Channels*OverlapBufferSize];

//extern float WorkBufferCore[MaximumBinsLong*Channels];
//extern CAacDecoderChannelInfo *pAacDecoderChannelInfoInit[Channels];
// extern CAacDecoderDynamicData *pAacDecoderDynamicDataInit[Channels];
// 
// extern CAacDecoderDynamicCommonData *pAacDecoderDynamicCommonDataInit;


CStreamInfo* getCStreamInfo(unsigned int flag);
CAacDecoderStaticChannelInfo* getCAacDecoderStaticChannelInfo(unsigned int flag,int channel);
CPnsStaticInterChannelData* getCPnsStaticInterChannelData(unsigned int flag);
float* getOverlapBuffer(unsigned int flag,unsigned int pos);
CConcealmentInfo* getConcealmentInfo(unsigned int flag,unsigned int pos);
float* getWorkBufferCoreArray(unsigned int flag,int pos);
float* getInterimResult(unsigned int flag,int pos);
CAacDecoderChannelInfo *getAacDecoderChannelInfoInit(unsigned int flag,int channel);
float* getTimeDataFloat(unsigned int flag,int pos);
CAacDecoderDynamicData *getAacDecoderDynamicDataInit(unsigned int flag,int channel);
CAacDecoderDynamicCommonData *getAacDecoderDynamicCommonDataInit(unsigned int flag);
void ReleaseMapAacDecoderDataInfo(unsigned int keyflag);
#endif /* #ifndef AAC_RAM_H */
