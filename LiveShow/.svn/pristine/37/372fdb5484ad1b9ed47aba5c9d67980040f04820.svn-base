/*
  decoding of long blocks
*/

#include <stdlib.h>
#include <math.h>
#include "aac_rom.h"
#include "imdct.h"
#include "FFR_bitbuffer.h"
#include "block.h"

#include "counters.h" /* the 3GPP instrumenting tool */


void CPns_Read (CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                HANDLE_BIT_BUF bs,
                const CodeBookDescription *hcb,
                unsigned char global_gain,
                int band,
                int group);

int CLongBlock_ReadSectionData(HANDLE_BIT_BUF bs,
                               CAacDecoderChannelInfo *pAacDecoderChannelInfo)
{
  int top;
  int band;
  char sect_cb;
  int sect_len;
  int sect_len_incr;
  int sect_esc_val = (1 << 5) - 1;
  char *pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  int ErrorStatus = AAC_DEC_OK;
  
  COUNT_sub_start("CLongBlock_ReadSectionData");

  INDIRECT(1); PTR_INIT(1); MOVE(2); /* counting previous operation */

  PTR_INIT(1); /* pCodeBook[] */
  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); )
  {
    MOVE(1);
    sect_len = 0;

    FUNC(2);
    sect_cb = (char) GetBits(bs,4);

    FUNC(2);
    sect_len_incr = GetBits(bs,5);

    LOOP(1);
    while (sect_len_incr == sect_esc_val)
    {
      ADD(1);
      sect_len += sect_esc_val;

      FUNC(2);
      sect_len_incr = GetBits(bs,5);
    }

    ADD(1);
    sect_len += sect_len_incr;


    ADD(1);
    top = band + sect_len;

    ADD(1); BRANCH(1);
    if (top > MAX_SFB_LONG) {
      COUNT_sub_end();

      return (AAC_DEC_DECODE_FRAME_ERROR);
    }

    LOOP(1);
    for (; band < top; band++)
    {
      MOVE(1);
      pCodeBook[band] = sect_cb;

      ADD(1); BRANCH(1);
      if (pCodeBook[band] == BOOKSCL)
      {
        COUNT_sub_end();
        return (AAC_DEC_INVALID_CODE_BOOK);
      }
    }
  }

  PTR_INIT(1); /* pCodeBook[] */
  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (; band < GetScaleFactorBandsTotal(&pAacDecoderChannelInfo->IcsInfo); band++)
  {
    MOVE(1);
    pCodeBook[band] = ZERO_HCB;
  }

  COUNT_sub_end();

  return (ErrorStatus);
}

void CLongBlock_ReadScaleFactorData(HANDLE_BIT_BUF bs,
                                    CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                    unsigned char global_gain)
{
  int temp;
  int band;
  int position = 0;
  int factor = global_gain;
  char *pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  short *pScaleFactor = pAacDecoderChannelInfo->pScaleFactor;
  const CodeBookDescription *hcb = &HuffmanCodeBooks[BOOKSCL];

  COUNT_sub_start("CLongBlock_ReadScaleFactorData");

  INDIRECT(3); PTR_INIT(3); MOVE(2); /* counting previous operations */

  PTR_INIT(2); /* pCodeBook[] 
                  pScaleFactor[]
               */
  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); band++)
  {

    BRANCH(2);
    switch (pCodeBook[band])
    {
      case ZERO_HCB: /* zero book */
        MOVE(1);
        pScaleFactor[band] = 0;
        break;

      default: /* regular scale factor */

        INDIRECT(1); FUNC(2);
        temp = CBlock_DecodeHuffmanWord(bs,hcb->CodeBook);

        ADD(2);
        factor += temp - 60; /* MIDFAC 1.5 dB */

        ADD(1); STORE(1);
        pScaleFactor[band] = factor - 100;
        break;

      case INTENSITY_HCB: /* intensity steering */
      case INTENSITY_HCB2:

        INDIRECT(1); FUNC(2);
        temp = CBlock_DecodeHuffmanWord(bs,hcb->CodeBook);

        ADD(2);
        position += temp - 60;

        ADD(1); STORE(1);
        pScaleFactor[band] = position - 100;
        break;

      case NOISE_HCB:

        FUNC(5);
        CPns_Read(pAacDecoderChannelInfo, bs, hcb, global_gain, band, 0);
        break;
    }
  }

  COUNT_sub_end();
}

int  CLongBlock_ReadSpectralData(HANDLE_BIT_BUF bs,
                                 CAacDecoderChannelInfo *pAacDecoderChannelInfo)
{
  int i,index,band,step;
  int scfExp,scfMod;
  int *QuantizedCoef;

  char *pCodeBook;
  short *pScaleFactor;
  float *pSpectralCoefficient;

  const short *BandOffsets = GetScaleFactorBandOffsets(&pAacDecoderChannelInfo->IcsInfo);
  const CodeBookDescription *hcb;

  COUNT_sub_start("CLongBlock_ReadSpectralData");

  INDIRECT(4); FUNC(1); PTR_INIT(5);
  pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  pScaleFactor = pAacDecoderChannelInfo->pScaleFactor;
  pSpectralCoefficient = pAacDecoderChannelInfo->pSpectralCoefficient;
  QuantizedCoef = (int*)pSpectralCoefficient;
  /* including initialization of BandOffsets */

  LOOP(1);
  for (index=0; index < MaximumBinsLong; index++)
  {
    MOVE(1);
    QuantizedCoef[index] = 0;
  }


  PTR_INIT(1); /* pointer for pCodeBook[] */
  INDIRECT(1); LOOP(1);
  for (band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); band++)
  {
    ADD(3); LOGIC(2); BRANCH(1);
    if ((pCodeBook[band] == ZERO_HCB) || (pCodeBook[band] == INTENSITY_HCB) || (pCodeBook[band] == INTENSITY_HCB2) || (pCodeBook[band] == NOISE_HCB))
      continue;

    PTR_INIT(1);
    hcb = &HuffmanCodeBooks[pCodeBook[band]] ;

    MOVE(1);
    step = 0;

    PTR_INIT(2); /* pointer for BandOffsets[],
                                QuantizedCoef[] */
    LOOP(1);
    for (index=BandOffsets[band]; index < BandOffsets[band+1]; index += step)
    {
      INDIRECT(1); FUNC(2); FUNC(3);
      step = CBlock_UnpackIndex(CBlock_DecodeHuffmanWord(bs,hcb->CodeBook),&QuantizedCoef[index],hcb);

      INDIRECT(1); BRANCH(1);
      if (hcb->Offset == 0)
      {
        PTR_INIT(1); /* pointer for QuantizedCoef[] */
        LOOP(1);
        for (i=0; i < step; i++)
        {
          BRANCH(1);
          if (QuantizedCoef[index+i])
          {
            FUNC(2);
            if (GetBits(bs,1)) /* sign bit */
            {
              MULT(1); STORE(1);
              QuantizedCoef[index+i] = -QuantizedCoef[index+i];
            }
          }
        }
      }

      ADD(1); BRANCH(1);
      if (pCodeBook[band] == ESCBOOK)
      {
        FUNC(2); STORE(1);
        QuantizedCoef[index] = CBlock_GetEscape(bs,QuantizedCoef[index]);

        FUNC(2); STORE(1);
        QuantizedCoef[index+1] = CBlock_GetEscape(bs,QuantizedCoef[index+1]);

        FUNC(1); FUNC(1); ADD(2); LOGIC(1); BRANCH(1);
        if (abs(QuantizedCoef[index]) > MAX_QUANTIZED_VALUE || abs(QuantizedCoef[index+1]) > MAX_QUANTIZED_VALUE) {
          COUNT_sub_end();
          return (AAC_DEC_DECODE_FRAME_ERROR);
        }
      }
    }
  }

  
  /* apply pulse data */
  INDIRECT(2); FUNC(3);
  CPulseData_Apply(&pAacDecoderChannelInfo->PulseData,
                   GetScaleFactorBandOffsets(&pAacDecoderChannelInfo->IcsInfo),
                   QuantizedCoef);


  PTR_INIT(2); /* pointer for BandOffsets[],
                              pScaleFactor     */
  INDIRECT(1); LOOP(1);
  for (index=0, band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); band++)
  {
    /* scalefactor exponents and scalefactor mantissa for current band */
    SHIFT(1);
    scfExp = pScaleFactor[band] >> 2;

    LOGIC(1);
    scfMod = pScaleFactor[band] & 3;

    PTR_INIT(2); /* pointer for QuantizedCoef[], 
                                pSpectralCoefficient */
    LOOP(1);
    for (index=BandOffsets[band]; index < BandOffsets[band+1]; index++)
    {
      ADD(1); FUNC(3); STORE(1);
      pSpectralCoefficient[index] = CBlock_Quantize(QuantizedCoef[index],scfMod,scfExp-9);
    }
  }

  /* zero out spectral data beyond max_sfb; index is now first bin of max_sfb+1 */
  PTR_INIT(1); /* pointer for pSpectralCoefficient */
  LOOP(1);
  for ( ; index < MaximumBinsLong; index++)
  {
    MOVE(1);
    pSpectralCoefficient[index] = 0;
  }

  COUNT_sub_end();
  return (AAC_DEC_OK);
}

static void 
NoLap1(float *coef,
       float *out,
       int stride)
{
  int i;

  COUNT_sub_start("NoLap1");

  PTR_INIT(2); /* out[stride*i]
                  coef[Size07-1-i]
               */
  LOOP(1);
  for (i=0; i<Size07; i++)
  {
    MULT(1); STORE(1);
    out[stride*i] = -coef[Size07-1-i];
  }

  COUNT_sub_end();
}


static void 
NoLap2(float *coef,
       float *out,
       int stride)
{
  int i;

  COUNT_sub_start("NoLap2");

  PTR_INIT(2); /* out[stride*i]
                  coef[Size07-1-i]
               */
  LOOP(1);
  for(i=0; i<Size07; i++)
  {
    MULT(1); STORE(1);
    out[stride*i] = -coef[Size07-1-i];
  }

  COUNT_sub_end();
}


static void 
StartLap(float *coef,
         float *out,
         int stride)
{
  int i;

  COUNT_sub_start("StartLap");

  PTR_INIT(2); /* out[stride*i]
                  coef[i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++)
  {
    MOVE(1);
    out[stride*i] = coef[i];
  }

  COUNT_sub_end();
}

static void 
StopLap(float *coef,
        float *prev,
        float *out,
        const float *window,
        int stride)
{
  int i;

  COUNT_sub_start("StopLap");

  PTR_INIT(5); /* out[stride*(Size07+i)]
                  coef[Size15+i]
                  window[i]
                  window[Size01*2-1-i]
                  prev[Size08-1-i])
               */
  LOOP(1);
  for (i=0; i<Size01; i++)
  {
    MULT(2); ADD(1); STORE(1);
    out[stride*(Size07+i)] = (coef[Size15+i]*window[i]) - (window[Size01*2-1-i]*prev[Size08-1-i]);
  }

  PTR_INIT(5); /* out[stride*(Size08+i)]
                  coef[Size08*2-1-i]
                  window[Size01+i]
                  window[Size01-1-i]
                  prev[Size07+i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++)
  {
    MULT(2); MAC(1); STORE(1);
    out[stride*(Size08+i)] = (-coef[Size08*2-1-i]*window[Size01+i]) - (window[Size01-1-i]*prev[Size07+i]);
  }

  COUNT_sub_end();
}

static void 
ShortLongLapIllegal(float *coef,
                    float *prev,
                    float *out,
                    const float *window_long,
                    const float *window_short,
                    int stride)
{
  int i;

  COUNT_sub_start("ShortLongLapIllegal");

  /* 0,...,Size07-1 */
  PTR_INIT(4); /* out[stride*i]
                  coef[Size08+i]
                  window_long[i]
                  prev[i]
               */
  LOOP(1);
  for (i=0; i<Size07; i++) {

    MULT(1); ADD(1); STORE(1);
    out[stride*i] = (coef[Size08+i]*window_long[i]) + prev[i];
  }

  /* Size07,...,Size08-1 */
  PTR_INIT(5); /* out[stride*(i+Size07)]
                  coef[Size15+i]
                  window_long[Size07+i]
                  window_short[Size02-1-i]
                  prev[Size08-1-i])
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {

    MULT(2); ADD(1); STORE(1);
    out[stride*(i+Size07)] = (coef[Size15+i]*window_long[Size07+i]) - (window_short[Size02-1-i]*prev[Size08-1-i]);
  }

  /* Size08,...,Size09-1 */
  PTR_INIT(5); /* out[stride*(i+Size08)]
                  coef[Size16-1-i]
                  window_long[Size08+i]
                  window_short[Size01-1-i]
                  prev[Size07+i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {

    MULT(2); MAC(1); STORE(1);
    out[stride*(i+Size08)] = (-coef[Size16-1-i]*window_long[Size08+i]) - (window_short[Size01-1-i]*prev[Size07+i]);
  }

  /* Size09,...,Size16-1 */
  PTR_INIT(3); /* out[stride*(i+Size09)]
                  coef[Size15-1-i]
                  window_long[Size09+i]
               */
  LOOP(1);
  for (i=0; i<Size07; i++) {

    MULT(2); STORE(1);
    out[stride*(i+Size09)] = -coef[Size15-1-i]*window_long[Size09+i];
  }

  COUNT_sub_end();
}

static void 
LongShortLapIllegal(float *coef,
                    float *prev,
                    float *out,
                    const float *window_long,
                    const float *window_short,
                    int stride)
{
  int i;

  COUNT_sub_start("LongShortLapIllegal");

  /* 0,...,Size07-1 */
  PTR_INIT(3); /* out[stride*i]
                  window_long[Size16-1-i]
                  prev[Size08-1-i]
               */
  LOOP(1);
  for (i=0; i<Size07; i++) {

    MULT(2); STORE(1);
    out[stride*i] = -window_long[Size16-1-i]*prev[Size08-1-i];
  }

  /* Size07,...,Size08-1 */
  PTR_INIT(5); /* out[stride*(Size07+i)]
                  coef[Size15+i]
                  window_short[i]
                  window_long[Size09-1-i]
                  prev[Size01-1-i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {

    MULT(2); ADD(1); STORE(1);
    out[stride*(Size07+i)] = (coef[Size15+i]*window_short[i]) - (window_long[Size09-1-i]*prev[Size01-1-i]);
  }

  /* Size08,...,Size09-1 */
  PTR_INIT(5); /* out[stride*(Size08+i)]
                  coef[Size16-1-i]
                  window_short[Size01+i]
                  window_long[Size08-1-i]
                  prev[i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {

    MULT(2); MAC(1); STORE(1);
    out[stride*(Size08+i)] = (-coef[Size16-1-i]*window_short[Size01+i]) - (window_long[Size08-1-i]*prev[i]);
  }

  /* Size09-Size16-1 */
  PTR_INIT(4); /* out[stride*(Size09+i)]
                  coef[Size15-1-i]
                  window_long[Size07-1-i]
                  prev[i+Size01]
               */
  LOOP(1);
  for (i=0; i<Size07; i++) {

    MULT(2); MAC(1); STORE(1);
    out[stride*(Size09+i)] = -coef[Size15-1-i] - (window_long[Size07-1-i]*prev[i+Size01]);
  }

  COUNT_sub_end();
}

void CLongBlock_FrequencyToTime(CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo,
                                CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                float outSamples[],
                                const int stride)
{
  int i;
  COverlapAddData *pOverlapAddData = &pAacDecoderStaticChannelInfo->OverlapAddData;
  const float *pLongWindow = pAacDecoderStaticChannelInfo->pLongWindow[pOverlapAddData->WindowShape];
  const float *pShortWindow = pAacDecoderStaticChannelInfo->pShortWindow[pOverlapAddData->WindowShape];
  float *pSpectralCoefficient = pAacDecoderChannelInfo->pSpectralCoefficient;

  COUNT_sub_start("CLongBlock_FrequencyToTime");

  INDIRECT(4); PTR_INIT(4); /* counting previous operations */

  /* Inverse IMDCT */
  FUNC(1);
  CLongBlock_InverseTransform(pSpectralCoefficient);

  /* Overlap&Add */
  INDIRECT(1); PTR_INIT(1); FUNC(1); BRANCH(2);
  switch(GetWindowSequence(&pAacDecoderChannelInfo->IcsInfo))
  {
    case OnlyLongSequence:

      INDIRECT(1); BRANCH(2);
      switch(pOverlapAddData->WindowSequence)
      {
        case OnlyLongSequence:
        case LongStopSequence:
          INDIRECT(1); FUNC(6);
          Lap1(pSpectralCoefficient,pOverlapAddData->pOverlapBuffer,outSamples,pLongWindow,Size08,stride);
        break;
        case LongStartSequence:
        case EightShortSequence:
          INDIRECT(1); FUNC(6);
          ShortLongLapIllegal(pSpectralCoefficient,pOverlapAddData->pOverlapBuffer,outSamples,pLongWindow,pShortWindow,stride);
        break;
      }
      PTR_INIT(2); /* pOverlapAddData->pOverlapBuffer[i]
                      pSpectralCoefficient[i]
                   */
      LOOP(1);
      for (i=0; i<Size08; i++)
      {
        MOVE(1);
        pOverlapAddData->pOverlapBuffer[i] = pSpectralCoefficient[i];
      }
    break;

    case LongStartSequence:

      INDIRECT(1); BRANCH(2);
      switch(pOverlapAddData->WindowSequence)
      {
        case OnlyLongSequence:
        case LongStopSequence:
          INDIRECT(1); FUNC(6);
          Lap1(pSpectralCoefficient,pOverlapAddData->pOverlapBuffer,outSamples,pLongWindow,Size08,stride);
        break;
        case LongStartSequence:
        case EightShortSequence:
          INDIRECT(1); FUNC(6);
          ShortLongLapIllegal(pSpectralCoefficient,pOverlapAddData->pOverlapBuffer,outSamples,pLongWindow,pShortWindow,stride);
        break;
      }
      INDIRECT(1); PTR_INIT(1); FUNC(3);
      NoLap1(&pSpectralCoefficient[Size01],pOverlapAddData->pOverlapBuffer,1);

      INDIRECT(1); PTR_INIT(1); FUNC(3);    
      StartLap(pSpectralCoefficient,&(pOverlapAddData->pOverlapBuffer[Size07]),1);
    break;

    case LongStopSequence:

      INDIRECT(1); BRANCH(2);
      switch(pOverlapAddData->WindowSequence)
      {
        case EightShortSequence:
        case LongStartSequence:

        PTR_INIT(2); /* outSamples[stride*i]
                        pOverlapAddData->pOverlapBuffer[i]
                     */
        LOOP(1);
        for (i=0; i<Size07; i++)
        {
          MOVE(1);
          outSamples[stride*i] = pOverlapAddData->pOverlapBuffer[i];
        }

        INDIRECT(1); FUNC(5);
        StopLap(pSpectralCoefficient, pOverlapAddData->pOverlapBuffer,outSamples,pShortWindow,stride);

        INDIRECT(2); PTR_INIT(2); FUNC(3);
        NoLap2(&pSpectralCoefficient[Size08],&outSamples[stride*Size09],stride);
        break;
        case OnlyLongSequence:
        case LongStopSequence:
        INDIRECT(1); FUNC(6);
        LongShortLapIllegal(pSpectralCoefficient,pOverlapAddData->pOverlapBuffer,outSamples,pLongWindow,pShortWindow,stride);
        break;
      }

      PTR_INIT(2); /* pOverlapAddData->pOverlapBuffer[i]
                      pSpectralCoefficient[i]
                   */
      LOOP(1);
      for (i=0; i<Size08; i++)
      {
        MOVE(1);
        pOverlapAddData->pOverlapBuffer[i] = pSpectralCoefficient[i];
      }
    break;
  }
  
  FUNC(1); FUNC(1); STORE(2);
  pOverlapAddData->WindowShape = GetWindowShape(&pAacDecoderChannelInfo->IcsInfo);
  pOverlapAddData->WindowSequence = GetWindowSequence(&pAacDecoderChannelInfo->IcsInfo);

  COUNT_sub_end();
}




