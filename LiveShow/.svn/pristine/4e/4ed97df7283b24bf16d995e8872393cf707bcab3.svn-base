/*
  joint stereo processing
*/

#ifndef STEREO_H
#define STEREO_H

#include "channelinfo.h"


void CJointStereo_Read(HANDLE_BIT_BUF bs,
                       CJointStereoData *pJointStereoData,
                       int windowGroups,
                       int scaleFactorBandsTransmitted);

void CJointStereo_ApplyMS(CAacDecoderChannelInfo *pAacDecoderChannelInfo[],
                          const short *pScaleFactorBandOffsets,
                          char *pWindowGroupLength,
                          int windowGroups,
                          int scaleFactorBandsTransmitted);

void CJointStereo_ApplyIS(CAacDecoderChannelInfo *pAacDecoderChannelInfo[],
                          const short *pScaleFactorBandOffsets,
                          char *pWindowGroupLength,
                          int windowGroups,
                          int scaleFactorBandsTransmitted,
                          unsigned char CommonWindow);

#endif /* #ifndef STEREO_H */
