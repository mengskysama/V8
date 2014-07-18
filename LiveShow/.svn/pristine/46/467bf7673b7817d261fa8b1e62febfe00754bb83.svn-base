/*
  joint stereo processing
*/

#include <math.h>

#include "stereo.h"
#include "aac_rom.h"
#include "FFR_bitbuffer.h"

#include "counters.h" /* the 3GPP instrumenting tool */

/*
  The function reads joint stereo data from bitstream.
*/
void CJointStereo_Read(HANDLE_BIT_BUF bs,                   /*!< pointer to bitstream */
                       CJointStereoData *pJointStereoData,  /*!< pointer to joint stereo side info */
                       int windowGroups,                    /*!< number of window groups */
                       int scaleFactorBandsTransmitted)     /*!< number of transmitted scalefactor bands */
{
  int group,band;

  COUNT_sub_start("CJointStereo_Read");

  INDIRECT(1); FUNC(2); STORE(1);
  pJointStereoData->MsMaskPresent = (char) GetBits(bs,2);

  /* Clear MS mask */
  PTR_INIT(1); /* pJointStereoData->MsUsed[] */
  LOOP(1);
  for (band=0; band<scaleFactorBandsTransmitted; band++)
  {
    MOVE(1);
    pJointStereoData->MsUsed[band] = 0;
  }

  INDIRECT(1); BRANCH(1);
  switch (pJointStereoData->MsMaskPresent)
  {
    case 0 : /* no M/S */
      /* all flags are already cleared */
      break ;

    case 1 : /* read ms_used */

      LOOP(1);
      for (group=0; group<windowGroups; group++)
      {
        PTR_INIT(1); /* pJointStereoData->MsUsed[] */
        LOOP(1);
        for (band=0; band<scaleFactorBandsTransmitted; band++)
        {
          FUNC(2); SHIFT(1); LOGIC(1); STORE(1);
          pJointStereoData->MsUsed[band] |= (GetBits(bs,1) << group);
        }
      }
      break ;

    case 2 : /* full spectrum M/S */

      PTR_INIT(1); /* pJointStereoData->MsUsed[] */
      LOOP(1);
      for (band=0; band<scaleFactorBandsTransmitted; band++)
      {
        MOVE(1);
        pJointStereoData->MsUsed[band] = 255 ;  /* set all flags to 1 */
      }
      break ;
  }

  COUNT_sub_end();
}

/*
  The function applies MS stereo.
*/
void CJointStereo_ApplyMS(CAacDecoderChannelInfo *pAacDecoderChannelInfo[2],   /*!< aac channel info */
                          const short *pScaleFactorBandOffsets,                /*!< pointer to scalefactor band offsets */
                          char *pWindowGroupLength,                            /*!< pointer to window group length array */
                          int windowGroups,                                    /*!< number of window groups */
                          int scaleFactorBandsTransmitted)                     /*!< number of transmitted scalefactor bands */
{
  CJointStereoData *pJointStereoData = pAacDecoderChannelInfo[0]->pJointStereoData;
  int window, group, groupwin, band, index;

  COUNT_sub_start("CJointStereo_ApplyMS");

  INDIRECT(1); PTR_INIT(1); /* counting previous operation */

  LOOP(1);
  for (window=0,group=0; group<windowGroups; group++)
  {
    PTR_INIT(2); /* pAacDecoderChannelInfo[0]->pSpectralCoefficient[]
                    pAacDecoderChannelInfo[1]->pSpectralCoefficient[]
                 */
    INDIRECT(1); LOOP(1);
    for (groupwin=0; groupwin<pWindowGroupLength[group]; groupwin++, window++)
    {
      float *LeftSpectrum = &pAacDecoderChannelInfo[0]->pSpectralCoefficient[window*FRAME_SIZE/8];
      float *RightSpectrum = &pAacDecoderChannelInfo[1]->pSpectralCoefficient[window*FRAME_SIZE/8];

      PTR_INIT(2); /* pJointStereoData->MsUsed[]
                      pScaleFactorBandOffsets[]
                   */
      LOOP(1);
      for (band=0; band<scaleFactorBandsTransmitted; band++)
      {
        SHIFT(1); LOGIC(1); BRANCH(1);
        if (pJointStereoData->MsUsed[band] & (1 << group))
        {
          PTR_INIT(2); /* LeftSpectrum[index]
                          RightSpectrum[index]
                       */
          LOOP(1);
          for (index=pScaleFactorBandOffsets[band]; index<pScaleFactorBandOffsets[band+1]; index++)
          {
            float LeftCoefficient  = LeftSpectrum[index];
            float RightCoefficient = RightSpectrum[index];

            ADD(2); STORE(2);
            LeftSpectrum[index]  = LeftCoefficient+RightCoefficient;
            RightSpectrum[index] = LeftCoefficient-RightCoefficient;
          }
        }
      }
    }
  }

  COUNT_sub_end();
}

/*
  The function applies intensity stereo.
*/
void CJointStereo_ApplyIS(CAacDecoderChannelInfo *pAacDecoderChannelInfo[2], /*!< aac channel info */
                          const short *pScaleFactorBandOffsets,              /*!< pointer to scalefactor band offsets */
                          char *pWindowGroupLength,                          /*!< pointer to window group length array */
                          int windowGroups,                                  /*!< number of window groups */
                          int scaleFactorBandsTransmitted,                   /*!< number of transmitted scalefactor bands */
                          unsigned char CommonWindow)                        /*!< common window bit */
{
  CJointStereoData *pJointStereoData = pAacDecoderChannelInfo[0]->pJointStereoData;
  int window, group, groupwin, band, index;

  COUNT_sub_start("CJointStereo_ApplyIS");

  INDIRECT(1); PTR_INIT(1); /* counting previous operation */

  PTR_INIT(2); /* pAacDecoderChannelInfo[1]->pCodeBook[group*MaximumScaleFactorBandsShort]
                  pAacDecoderChannelInfo[1]->pScaleFactor[group*MaximumScaleFactorBandsShort]
               */
  LOOP(1);
  for (window=0,group=0; group<windowGroups; group++)
  {
    char *CodeBook = &pAacDecoderChannelInfo[1]->pCodeBook[group*MaximumScaleFactorBandsShort];
    short *ScaleFactor = &pAacDecoderChannelInfo[1]->pScaleFactor[group*MaximumScaleFactorBandsShort];

    PTR_INIT(2); /* pAacDecoderChannelInfo[0]->pSpectralCoefficient[window*FRAME_SIZE/8]
                    pAacDecoderChannelInfo[1]->pSpectralCoefficient[window*FRAME_SIZE/8]
                 */
    LOOP(1);
    for (groupwin=0; groupwin<pWindowGroupLength[group]; groupwin++, window++)
    {
      float *LeftSpectrum = &pAacDecoderChannelInfo[0]->pSpectralCoefficient[window*FRAME_SIZE/8];
      float *RightSpectrum = &pAacDecoderChannelInfo[1]->pSpectralCoefficient[window*FRAME_SIZE/8];

      PTR_INIT(4); /* CodeBook[]
                      pJointStereoData->MsUsed[]
                      ScaleFactor[band]
                      pScaleFactorBandOffsets[band]
                   */
      LOOP(1);
      for (band=0; band<scaleFactorBandsTransmitted; band++)
      {
        ADD(2); LOGIC(1); BRANCH(1);
        if ((CodeBook [band] == INTENSITY_HCB) || (CodeBook [band] == INTENSITY_HCB2))
        {
          float scale = (float) pow (0.5f, 0.25f * (ScaleFactor[band] + 100));

          ADD(1); MULT(1); TRANS(1); /* counting previous operations */
          
          SHIFT(1); LOGIC(2); BRANCH(1);
          if (CommonWindow && (pJointStereoData->MsUsed[band] & (1 << group)))
          {
            ADD(1); BRANCH(1);
            if (CodeBook[band] == INTENSITY_HCB) /* _NOT_ in-phase */
            {
              MULT(1);
              scale = -scale ;
            }
          }
          else
          {
            ADD(1); BRANCH(1);
            if (CodeBook[band] == INTENSITY_HCB2) /* out-of-phase */
            {
              MULT(1);
              scale = -scale ;
            }
          }
          
          PTR_INIT(2); /* RightSpectrum[index]
                          LeftSpectrum[index]
                       */
          LOOP(1);
          for (index=pScaleFactorBandOffsets[band]; index<pScaleFactorBandOffsets[band+1]; index++)
          {
            MULT(1); STORE(1);
            RightSpectrum[index] = LeftSpectrum[index] * scale;
          }
        }
      }
    }
  }

  COUNT_sub_end();
}
