/*
  perceptual noise substitution tool
*/

#ifndef PNS_H
#define PNS_H

#include "channelinfo.h"

void CPns_InitPns(CAacDecoderChannelInfo *pAacDecoderChannelInfo);

void CPns_InitInterChannelData(CAacDecoderChannelInfo *pAacDecoderChannelInfo);

int CPns_IsPnsUsed(CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                   const int group,
                   const int band);

void CPns_SetCorrelation(CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                         const int group,
                         const int band);

int CPns_IsCorrelated(CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                      const int group,
                      const int band);

void CPns_Read (CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                HANDLE_BIT_BUF bs,
                const CodeBookDescription *hcb,
                unsigned char global_gain,
                int band,
                int group);

void CPns_Apply (CAacDecoderChannelInfo *pAacDecoderChannelInfo[],
                 int channel);

#endif /* #ifndef PNS_H */
