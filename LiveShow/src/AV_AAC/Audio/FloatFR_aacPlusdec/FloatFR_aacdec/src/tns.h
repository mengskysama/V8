/*
  temporal noise shaping tool
*/

#ifndef TNS_H
#define TNS_H

#include "channelinfo.h"



int 
TnsPresent(CTnsData *pTnsData);


void 
CTns_ReadDataPresentFlag(HANDLE_BIT_BUF bs,
                         CAacDecoderChannelInfo *pAacDecoderChannelInfo);

void 
CTns_Apply (CAacDecoderChannelInfo *pAacDecoderChannelInfo);

void CTns_Read(HANDLE_BIT_BUF bs,
               CAacDecoderChannelInfo *pAacDecoderChannelInfo);


#endif /* #ifndef TNS_H */
