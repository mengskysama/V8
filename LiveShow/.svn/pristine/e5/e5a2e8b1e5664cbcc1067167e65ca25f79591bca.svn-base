/*
  decoding of short blocks
*/

#include <stdlib.h>
#include <math.h>
#include "block.h"
#include "aac_rom.h"
#include "imdct.h"
#include "FFR_bitbuffer.h"

#include "counters.h" /* the 3GPP instrumenting tool */


void CPns_Read (CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                HANDLE_BIT_BUF bs,
                const CodeBookDescription *hcb,
                unsigned char global_gain,
                int band,
                int group);

void CShortBlock_Init(CAacDecoderChannelInfo *pAacDecoderChannelInfo)
{
  int group,band;
  char *pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  short *pScaleFactor = pAacDecoderChannelInfo->pScaleFactor;

  COUNT_sub_start("CShortBlock_Init");

  INDIRECT(2); PTR_INIT(2); /* counting previous operations */

  PTR_INIT(2); /* pCodeBook[]
                  pScaleFactor[]
               */
  LOOP(1);
  for (group=0; group<MaximumGroups; group++)
  {
    LOOP(1);
    for (band=0; band<MaximumScaleFactorBandsShort; band++)
    {
      MOVE(2);
      pCodeBook[group*MaximumScaleFactorBandsShort+band] = 0;
      pScaleFactor[group*MaximumScaleFactorBandsShort+band] = 0;
    }
  }

  COUNT_sub_end();
}

int CShortBlock_ReadSectionData(HANDLE_BIT_BUF bs,
                                CAacDecoderChannelInfo *pAacDecoderChannelInfo)
{
  int top;
  int band;
  int group;
  char sect_cb;
  int sect_len;
  int sect_len_incr;
  int sect_esc_val = (1 << 3) - 1 ;
  char *pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  int ErrorStatus = AAC_DEC_OK;

  COUNT_sub_start("CShortBlock_ReadSectionData");
  
  INDIRECT(1); PTR_INIT(1); MOVE(2); /* counting previous operation */

  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (group=0; group<GetWindowGroups(&pAacDecoderChannelInfo->IcsInfo); group++)
  {
    PTR_INIT(1); /* pCodeBook[] */
    FUNC(1); LOOP(1);
    for (band=0; band<GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); )
    {
      MOVE(1);
      sect_len = 0 ;

      FUNC(2);
      sect_cb = (char) GetBits(bs,4) ;

      FUNC(2);
      sect_len_incr = GetBits(bs,3);

      LOOP(1);
      while (sect_len_incr == sect_esc_val)
      {
        ADD(1);
        sect_len += sect_esc_val;

        FUNC(2);
        sect_len_incr = GetBits(bs,3);
      }

      ADD(1);
      sect_len += sect_len_incr;


      ADD(1);
      top = band + sect_len;

      MULT(1); ADD(2); BRANCH(1);
      if (top + group*MaximumScaleFactorBandsShort > (MAX_WINDOWS * MAX_SFB_SHORT)) {
        COUNT_sub_end();
        return (AAC_DEC_DECODE_FRAME_ERROR);
      }

      LOOP(1);
      for (; band < top; band++)
      {
        MOVE(1);
        pCodeBook[group*MaximumScaleFactorBandsShort+band] = sect_cb;

        ADD(1); BRANCH(1);
        if (pCodeBook[group*MaximumScaleFactorBandsShort+band] == BOOKSCL)
        {
          COUNT_sub_end();
          return (AAC_DEC_INVALID_CODE_BOOK);
        }
      }
    }

    PTR_INIT(1); /* pCodeBook[] */
    FUNC(1); LOOP(1);
    for ( ; band < GetScaleFactorBandsTotal(&pAacDecoderChannelInfo->IcsInfo); band++)
    {
      MOVE(1);
      pCodeBook[group*MaximumScaleFactorBandsShort+band] = ZERO_HCB;
    }
  }


  COUNT_sub_end();

  return (ErrorStatus);
}

void CShortBlock_ReadScaleFactorData(HANDLE_BIT_BUF bs,
                                     CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                     unsigned char global_gain)
{
  int temp;
  int band;
  int group;
  int position = 0;
  int factor = global_gain;
  char *pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  short *pScaleFactor = pAacDecoderChannelInfo->pScaleFactor;
  const CodeBookDescription *hcb = &HuffmanCodeBooks[BOOKSCL];

  COUNT_sub_start("CShortBlock_ReadScaleFactorData");

  INDIRECT(3); PTR_INIT(3); MOVE(2); /* counting previous operations */

  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (group=0; group < GetWindowGroups(&pAacDecoderChannelInfo->IcsInfo); group++)
  {
    PTR_INIT(2); /* pCodeBook[] 
                    pScaleFactor[]
                 */
    for (band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); band++)
    {
      BRANCH(2);
      switch (pCodeBook[group*MaximumScaleFactorBandsShort+band])
      {
        case ZERO_HCB: /* zero book */
          MOVE(1);
          pScaleFactor[group*MaximumScaleFactorBandsShort+band] = 0;
          break;

        default: /* decode scale factor */

          INDIRECT(1); FUNC(2);
          temp = CBlock_DecodeHuffmanWord(bs,hcb->CodeBook);

          ADD(2);
          factor += temp - 60; /* MIDFAC 1.5 dB */

          ADD(1); STORE(1);
          pScaleFactor[group*MaximumScaleFactorBandsShort+band] = factor - 100;
          break;

        case INTENSITY_HCB: /* intensity steering */
        case INTENSITY_HCB2:

          INDIRECT(1); FUNC(2);
          temp = CBlock_DecodeHuffmanWord(bs,hcb->CodeBook);

          ADD(2);
          position += temp - 60;

          ADD(1); STORE(1);
          pScaleFactor[group*MaximumScaleFactorBandsShort+band] = position - 100;
          break;

        case NOISE_HCB: /* PNS */

          FUNC(5);
          CPns_Read(pAacDecoderChannelInfo, bs, hcb, global_gain, band, group);
          break;
      }
    }
  }


  COUNT_sub_end();
}

int  CShortBlock_ReadSpectralData(HANDLE_BIT_BUF bs,
                                  CAacDecoderChannelInfo *pAacDecoderChannelInfo)
{
  int i,index,step;
  int window,group,groupwin,groupoffset,band;
  int scfExp,scfMod;
  int *QuantizedCoef;
  char *pCodeBook = pAacDecoderChannelInfo->pCodeBook;
  short *pScaleFactor = pAacDecoderChannelInfo->pScaleFactor;
  float *pSpectralCoefficient = pAacDecoderChannelInfo->pSpectralCoefficient;
  const short *BandOffsets = GetScaleFactorBandOffsets(&pAacDecoderChannelInfo->IcsInfo);
  const CodeBookDescription *hcb;

  COUNT_sub_start("CShortBlock_ReadSpectralData");

  QuantizedCoef = (int*)pSpectralCoefficient;
  PTR_INIT(5); INDIRECT(4); FUNC(1); /* counting previous operations */

  LOOP(1);
  for (window=0; window < MaximumWindows; window++)
  {
    PTR_INIT(1); /* pointer for QuantizedCoef[] */
    LOOP(1);
    for (index=0; index < MaximumBinsShort; index++) {
      MOVE(1);
      QuantizedCoef[window*MaximumBinsShort+index] = 0;
    }
  }


  MOVE(1);
  groupoffset = 0;

  INDIRECT(1); FUNC(1); LOOP(1);
  for (group=0; group < GetWindowGroups(&pAacDecoderChannelInfo->IcsInfo); group++)
  {
    PTR_INIT(1); /* pointer for pCodeBook[] */
    INDIRECT(1); FUNC(1); LOOP(1);
    for (band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); band++)
    {
      PTR_INIT(1);
      hcb = &HuffmanCodeBooks[pCodeBook[group*MaximumScaleFactorBandsShort+band]];

      INDIRECT(1); FUNC(1); LOOP(1);
      for (groupwin=0; groupwin < GetWindowGroupLength(&pAacDecoderChannelInfo->IcsInfo,group); groupwin++)
      {
        ADD(1);
        window = groupoffset + groupwin;

        ADD(4); LOGIC(3); BRANCH(1);
        if (  (pCodeBook[group*MaximumScaleFactorBandsShort+band] == ZERO_HCB)
            ||(pCodeBook[group*MaximumScaleFactorBandsShort+band] == INTENSITY_HCB)
            ||(pCodeBook[group*MaximumScaleFactorBandsShort+band] == INTENSITY_HCB2)
            ||(pCodeBook[group*MaximumScaleFactorBandsShort+band] == NOISE_HCB))
          continue;

        MOVE(1);
        step = 0 ;

        PTR_INIT(2); /* pointer for BandOffsets[],
                                    QuantizedCoef[] */
        LOOP(1);
        for (index=BandOffsets[band]; index < BandOffsets[band+1]; index+=step)
        {
          INDIRECT(1); FUNC(2); PTR_INIT(1); FUNC(3);
          step = CBlock_UnpackIndex(CBlock_DecodeHuffmanWord(bs,hcb->CodeBook),&QuantizedCoef[window*MaximumBinsShort+index],hcb);

          INDIRECT(1); BRANCH(1);
          if (hcb->Offset == 0)
          {
            PTR_INIT(1); /* pointer for QuantizedCoef[] */
            LOOP(1);
            for (i=0; i < step; i++)
            {
              BRANCH(1);
              if (QuantizedCoef[window*MaximumBinsShort+index+i])
              {
                FUNC(2);
                if (GetBits(bs,1)) /* sign bit */
                {
                  MULT(1); STORE(1);
                  QuantizedCoef [window*MaximumBinsShort+index+i] = -QuantizedCoef [window*MaximumBinsShort+index+i];
                }
              }
            }
          }

          ADD(1); BRANCH(1);
          if (pCodeBook[group*MaximumScaleFactorBandsShort+band] == ESCBOOK)
          {
            FUNC(2); STORE(1);
            QuantizedCoef[window*MaximumBinsShort+index] = CBlock_GetEscape(bs,QuantizedCoef[window*MaximumBinsShort+index]);

            FUNC(2); STORE(1);
            QuantizedCoef[window*MaximumBinsShort+index+1] = CBlock_GetEscape(bs,QuantizedCoef[window*MaximumBinsShort+index+1]);
  
            FUNC(1); FUNC(1); ADD(2); LOGIC(1); BRANCH(1);
            if (abs(QuantizedCoef[window*MaximumBinsShort+index]) > MAX_QUANTIZED_VALUE || abs(QuantizedCoef[window*MaximumBinsShort+index+1]) > MAX_QUANTIZED_VALUE) {
              COUNT_sub_end();
              return (AAC_DEC_DECODE_FRAME_ERROR);
            }
          }
        }
      }
    }

    INDIRECT(1); ADD(1);
    groupoffset += GetWindowGroupLength(&pAacDecoderChannelInfo->IcsInfo,group);
  }



  INDIRECT(1); FUNC(1); LOOP(1);
  for (window=0, group=0; group < GetWindowGroups(&pAacDecoderChannelInfo->IcsInfo); group++)
  {
    INDIRECT(1); FUNC(1); LOOP(1);
    for (groupwin=0; groupwin < GetWindowGroupLength(&pAacDecoderChannelInfo->IcsInfo,group); groupwin++, window++)
    {
      MOVE(1);
      index = 0;

      PTR_INIT(2); /* pointer for BandOffsets[],
                                  pScaleFactor     */
      INDIRECT(1); FUNC(1); LOOP(1) ;
      /* quantize & apply scalefactors */
      for (band=0; band < GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo); band++)
      {
        /* scalefactor exponents and scalefactor mantissa for current band */
        SHIFT(1);
        scfExp = pScaleFactor[group*MaximumScaleFactorBandsShort+band] >> 2;

        LOGIC(1);
        scfMod = pScaleFactor[group*MaximumScaleFactorBandsShort+band] & 3;

        PTR_INIT(2); /* pointer for QuantizedCoef[], 
                                    pSpectralCoefficient */
        LOOP(1);
        for (index=BandOffsets[band]; index < BandOffsets[band+1] ;index++)
        {
          ADD(1); FUNC(3); STORE(1);
          pSpectralCoefficient[window*MaximumBinsShort+index] = CBlock_Quantize(QuantizedCoef[window*MaximumBinsShort+index],scfMod,scfExp-6);
        }
      }

      PTR_INIT(1); /* pointer for pSpectralCoefficient */
      LOOP(1);
      for (; index < MaximumBinsShort; index++) {
        MOVE(1);
        pSpectralCoefficient[window*MaximumBinsShort+index] = 0.0;
      }
    }
  }


  COUNT_sub_end();
  return (AAC_DEC_OK);
}

static void 
LongShortLapIllegal(float *current,
                    float *prev,
                    float *out,
                    const float *shortWindow,
                    const float *shortWindowPrev,
                    const float *longWindowPrev,
                    int stride)
{
  int i;

  COUNT_sub_start("CShortBlock_LongShortLapIllegal");

  /* 0,...,Size07-1 */
  PTR_INIT(3); /* out[stride*i]
                  longWindowPrev[Size16-1-i]
                  prev[Size08-1-i]
               */
  LOOP(1);
  for (i=0; i<Size07; i++) {
    MULT(2); STORE(1);
    out[stride*i] = -longWindowPrev[Size16-1-i]*prev[Size08-1-i];
  }

  /* Size07,...,Size08-1 */
  PTR_INIT(5); /* out[stride*(Size07+i)]
                  current[Size01+i]
                  shortWindowPrev[i]
                  longWindowPrev[Size09-1-i]
                  prev[Size01-1-i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); ADD(1); STORE(1);
    out[stride*(Size07+i)] = (current[Size01+i]*shortWindowPrev[i]) - (longWindowPrev[Size09-1-i]*prev[Size01-1-i]);
  }

  /* Size08,...,Size09-1 */
  PTR_INIT(5); /* out[stride*(Size08+i)]
                  current[Size02-1-i]
                  shortWindowPrev[Size01+i]
                  longWindowPrev[Size08-1-i]
                  prev[i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(1); STORE(1);
    out[stride*(Size08+i)] = (-current[Size02-1-i]*shortWindowPrev[Size01+i]) - (longWindowPrev[Size08-1-i]*prev[i]);
  }

  /* Size09,...,Size10-1 */
  PTR_INIT(7); /* out[stride*(Size09+i)]
                  current[Size03+i]
                  current[Size01-1-i]
                  shortWindow[i]
                  shortWindow[Size02-1-i]
                  longWindowPrev[Size07-1-i]
                  prev[i+Size01]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1);
    out[stride*(Size09+i)] = (current[Size03+i]*shortWindow[i]) - (shortWindow[Size02-1-i]*current[Size01-1-i]) - (longWindowPrev[Size07-1-i]*prev[i+Size01]);
  }

  /* Size10,...,Size11-1 */
  PTR_INIT(7); /* out[stride*(Size10+i)]
                  current[Size04-1-i]
                  current[i]
                  shortWindow[Size01+i]
                  shortWindow[Size01-1-i]
                  longWindowPrev[Size06-1-i]
                  prev[i+Size02]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1);
    out[stride*(Size10+i)] = (-current[Size04-1-i]*shortWindow[Size01+i]) - (shortWindow[Size01-1-i]*current[i]) - (longWindowPrev[Size06-1-i]*prev[i+Size02]);
  }

  /* Size11,...,Size12-1 */
  PTR_INIT(7); /* out[stride*(Size11+i)]
                  current[Size05+i]
                  current[Size03-1-i]
                  shortWindow[i]
                  shortWindow[Size02-1-i]
                  longWindowPrev[Size05-1-i]
                  prev[i+Size03]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1); /* calculating: (-1 * (current[Size05+i]*shortWindow[i])) + (shortWindow[Size02-1-i]*current[Size03-1-i]) + (longWindowPrev[Size05-1-i]*prev[i+Size03]) */
    out[stride*(Size11+i)] = (current[Size05+i]*shortWindow[i]) - (shortWindow[Size02-1-i]*current[Size03-1-i]) - (longWindowPrev[Size05-1-i]*prev[i+Size03]);
  }

  /* Size12,...,Size13-1 */
  PTR_INIT(7); /* out[stride*(Size12+i)]
                  current[Size06-1-i]
                  current[Size02+i]
                  shortWindow[Size01+i]
                  shortWindow[Size01-1-i]
                  longWindowPrev[Size04-1-i]
                  prev[i+Size04]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1);
    out[stride*(Size12+i)] = (-current[Size06-1-i]*shortWindow[Size01+i]) - (shortWindow[Size01-1-i]*current[Size02+i]) - (longWindowPrev[Size04-1-i]*prev[i+Size04]);
  }

  /* Size13,...,Size14-1 */
  PTR_INIT(7); /* out[stride*(Size13+i)]
                  current[Size07+i]
                  current[Size05-1-i]
                  shortWindow[i]
                  shortWindow[Size02-1-i]
                  longWindowPrev[Size03-1-i]
                  prev[i+Size05]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1);
    out[stride*(Size13+i)] = (current[Size07+i]*shortWindow[i]) - (shortWindow[Size02-1-i]*current[Size05-1-i]) - (longWindowPrev[Size03-1-i]*prev[i+Size05]);
  }

  /* Size14,...,Size15-1 */
  PTR_INIT(7); /* out[stride*(Size14+i)]
                  current[Size08-1-i]
                  shortWindow[Size01+i]
                  shortWindow[Size01-1-i]
                  current[Size04+i]
                  longWindowPrev[Size02-1-i]
                  prev[i+Size06]
               */
  LOOP(1);
  for(i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1);
    out[stride*(Size14+i)] = (-current[Size08-1-i]*shortWindow[Size01+i]) - (shortWindow[Size01-1-i]*current[Size04+i]) - (longWindowPrev[Size02-1-i]*prev[i+Size06]);
  }

  /* Size15,...,Size16-1 */
  PTR_INIT(7); /* out[stride*(Size15+i)]
                  current[Size09+i]
                  current[Size07-1-i]
                  shortWindow[i]
                  shortWindow[Size02-1-i]
                  longWindowPrev[Size01-1-i]
                  prev[i+Size07]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(2); STORE(1);
    out[stride*(Size15+i)] = (current[Size09+i]*shortWindow[i]) - (shortWindow[Size02-1-i]*current[Size07-1-i]) - (longWindowPrev[Size01-1-i]*prev[i+Size07]);
  }

  PTR_INIT(5); /* prev[i]
                  current[Size10-1-i]
                  current[Size06+i]
                  shortWindow[Size01+i]
                  shortWindow[Size01-1-i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++) {
    MULT(2); MAC(1); STORE(1);
    prev[i] = -current[Size10-1-i]*shortWindow[Size01+i] - shortWindow[Size01-1-i]*current[Size06+i];
  }

  COUNT_sub_end();
}

void CShortBlock_FrequencyToTime(CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo,
                                 CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                 float outSamples[],
                                 const int stride)
{
  int i;
  COverlapAddData *pOverlapAddData = &pAacDecoderStaticChannelInfo->OverlapAddData;
  const float *shortWindow = pAacDecoderStaticChannelInfo->pShortWindow[GetWindowShape(&pAacDecoderChannelInfo->IcsInfo)];
  const float *shortWindowPrev = pAacDecoderStaticChannelInfo->pShortWindow[pOverlapAddData->WindowShape];
  const float *longWindowPrev = pAacDecoderStaticChannelInfo->pLongWindow[pOverlapAddData->WindowShape];
  float *pSpectralCoefficient = pAacDecoderChannelInfo->pSpectralCoefficient;

  COUNT_sub_start("CShortBlock_FrequencyToTime");

  INDIRECT(5); PTR_INIT(5); /* counting previous operations */

  /* Inverse MDCT */
  PTR_INIT(1); /* pSpectralCoefficient[i*Size02] */
  LOOP(1);
  for (i=0; i<MaximumWindows; i++) {
    FUNC(1);
    CShortBlock_InverseTransform(&pSpectralCoefficient[i*Size02]);
  }

  /* Overlap & Add */
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

      INDIRECT(2); PTR_INIT(3); FUNC(6);
      Lap1(&pSpectralCoefficient[0],&pOverlapAddData->pOverlapBuffer[Size07],
           &outSamples[stride*Size07],shortWindowPrev,Size01,stride);

      INDIRECT(1); PTR_INIT(3); FUNC(6);
      Lap1(&pSpectralCoefficient[Size02],&pSpectralCoefficient[0],
           &outSamples[stride*Size09],shortWindow,Size01,stride);

      INDIRECT(1); PTR_INIT(3); FUNC(6);
      Lap1(&pSpectralCoefficient[Size04],&pSpectralCoefficient[Size02],
           &outSamples[stride*Size11],shortWindow,Size01,stride);

      INDIRECT(1); PTR_INIT(3); FUNC(6);
      Lap1(&pSpectralCoefficient[Size06],&pSpectralCoefficient[Size04],
           &outSamples[stride*Size13],shortWindow,Size01,stride);

      PTR_INIT(3); FUNC(6);
      Lap2(&pSpectralCoefficient[Size08],&pSpectralCoefficient[Size06],
           pOverlapAddData->pOverlapBuffer,shortWindow,Size01,1);

      PTR_INIT(3); /* outSamples[stride*(Size15+i)]
                      pOverlapAddData->pOverlapBuffer[i]
                      pOverlapAddData->pOverlapBuffer[i+Size01]
                   */
      LOOP(1);
      for (i=0; i<Size01; i++)
      {
        MOVE(2);
        outSamples[stride*(Size15+i)] = pOverlapAddData->pOverlapBuffer[i];
        pOverlapAddData->pOverlapBuffer[i] = pOverlapAddData->pOverlapBuffer[i+Size01];
      }
    break;

    case OnlyLongSequence:
    case LongStopSequence:
      INDIRECT(1); FUNC(6);
      LongShortLapIllegal(pSpectralCoefficient,pOverlapAddData->pOverlapBuffer,outSamples,
                          shortWindow,shortWindowPrev,longWindowPrev,stride);
    break;
  }

  PTR_INIT(3); FUNC(6);
  Lap2(&pSpectralCoefficient[Size10],&pSpectralCoefficient[Size08],
       &pOverlapAddData->pOverlapBuffer[Size01],shortWindow,Size01,1);

  PTR_INIT(3); FUNC(6);
  Lap2(&pSpectralCoefficient[Size12],&pSpectralCoefficient[Size10],
       &pOverlapAddData->pOverlapBuffer[Size03],shortWindow,Size01,1);

  PTR_INIT(3); FUNC(6);
  Lap2(&pSpectralCoefficient[Size14],&pSpectralCoefficient[Size12],
       &pOverlapAddData->pOverlapBuffer[Size05],shortWindow,Size01,1);

  PTR_INIT(2); /* pOverlapAddData->pOverlapBuffer[i+Size07]
                  pSpectralCoefficient[Size14+i]
               */
  LOOP(1);
  for (i=0; i<Size01; i++)
  {
    MOVE(1);
    pOverlapAddData->pOverlapBuffer[i+Size07] = pSpectralCoefficient[Size14+i];
  }

  INDIRECT(1); PTR_INIT(1); FUNC(1); FUNC(1); STORE(2);
  pOverlapAddData->WindowShape = GetWindowShape(&pAacDecoderChannelInfo->IcsInfo);
  pOverlapAddData->WindowSequence = GetWindowSequence(&pAacDecoderChannelInfo->IcsInfo);


  COUNT_sub_end();
}
