/*
  independent channel concealment
*/

#ifndef CONCEAL_H
#define CONCEAL_H

#include "aac_ram.h"

#define MAXSFB  51            /* maximum number of scalefactor bands  */

typedef enum{
  CConcealment_Ok,
  CConcealment_FadeOut,
  CConcealment_Mute,
  CConcealment_FadeIn
} CConcealmentState;

typedef enum {
  CConcealment_NoExpand,
  CConcealment_Expand,
  CConcealment_Compress
} CConcealmentExpandType;


void CConcealment_Init (CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo);

void CConcealment_Apply (CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo,
                         CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                         char FrameOk);

#endif /* #ifndef CONCEAL_H */
