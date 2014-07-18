/*
  channel info
*/

#ifndef CHANNEL_H
#define CHANNEL_H

#include "block.h"
#include "FFR_bitbuffer.h"

enum
{
  ID_SCE = 0,
  ID_CPE,
  ID_CCE,
  ID_LFE,
  ID_DSE,
  ID_PCE,
  ID_FIL,
  ID_END,
};

enum
{
  L = 0,
  R = 1,
  Channels = 2
};

int ReadICS(HANDLE_BIT_BUF bs,
            CAacDecoderChannelInfo *pAacDecoderChannelInfo);

int CSingleChannelElement_Read(HANDLE_BIT_BUF bs,
                              CAacDecoderChannelInfo *pAacDecoderChannelInfo[Channels],
                              CStreamInfo *pStreamInfo);

int CChannelPairElement_Read(HANDLE_BIT_BUF bs,
                             CAacDecoderChannelInfo *pAacDecoderChannelInfo[Channels],
                             CStreamInfo *pStreamInfo);

void CSingleChannelElement_Decode(CAacDecoderChannelInfo *pAacDecoderChannelInfo[Channels]);

void CChannelPairElement_Decode(CAacDecoderChannelInfo *pAacDecoderChannelInfo[Channels]);

void MapMidSideMaskToPnsCorrelation (CAacDecoderChannelInfo *pAacDecoderChannelInfo[Channels]);

#endif /* #ifndef CHANNEL_H */
