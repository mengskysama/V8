/*
  perceptual noise substitution tool
*/

#include <math.h>
#include <assert.h>
#include "aac_rom.h"
#include "channelinfo.h"
#include "block.h"
#include "pns.h"
#include "FFR_bitbuffer.h"

#include "counters.h" /* the 3GPP instrumenting tool */

#define PNS_BAND_FLAGS_MASK              ((1 << PNS_BAND_FLAGS_SHIFT) - 1)
#define PNS_BAND_FLAGS_SHIFT             3

#define NOISE_OFFSET 90           /* cf. ISO 14496-3 p. 175 */

int noise_index;
int noise_left_index;
int noise_left_index_start;

void GenerateRandomVector (float scale,
                           float spec[],
                           int bandOffsetStart,
                           int bandOffsetEnd,
                           short *randomState);

/*
  The function initializes the InterChannel data
*/
void CPns_InitInterChannelData(CAacDecoderChannelInfo *pAacDecoderChannelInfo) 
{
  unsigned int i;
  CPnsInterChannelData *pInterChannelData = pAacDecoderChannelInfo->pPnsInterChannelData;

  COUNT_sub_start("CPns_InitInterChannelData");

  PTR_INIT(1); MOVE(1); /* counting previous operations */

  PTR_INIT(1); /* pInterChannelData->correlated[i] */
  LOOP(1);
  for (i = 0; i < PNS_BAND_FLAGS_SIZE; i++)
  {
    MOVE(1);
    pInterChannelData->correlated[i] = 0;
  }

  COUNT_sub_end();
}


/*
  The function initializes the PNS data
*/
void CPns_InitPns(CAacDecoderChannelInfo *pAacDecoderChannelInfo) 
{
  unsigned int i;
  CPnsData *pPnsData = &pAacDecoderChannelInfo->PnsData;

  COUNT_sub_start("CPns_InitPns");

  PTR_INIT(1); MOVE(1); /* counting previous operations */

  PTR_INIT(1); /* pPnsData->pnsUsed[i] */
  LOOP(1);
  for (i = 0; i < PNS_BAND_FLAGS_SIZE; i++)
  {
    MOVE(1);
    pPnsData->pnsUsed[i] = 0;
  }

  MOVE(2);
  pPnsData->PnsActive = 0;
  pPnsData->CurrentEnergy = 0;

  COUNT_sub_end();
}


/*
  The function returns a value indicating whether PNS is used or not 
  acordding to the noise energy

  return:  PNS used
*/
int CPns_IsPnsUsed (CAacDecoderChannelInfo *pAacDecoderChannelInfo,
               const int group,
               const int band)
{
  CPnsData *pPnsData = &pAacDecoderChannelInfo->PnsData;
  unsigned pns_band = group*MaximumScaleFactorBandsShort+band;

  COUNT_sub_start("CPns_IsPnsUsed");
  INDIRECT(1); PTR_INIT(1); MULT(1); ADD(1); /* counting previous operations */
  SHIFT(2); LOGIC(2); INDIRECT(1); /* counting post-operation */
  COUNT_sub_end();

  return (pPnsData->pnsUsed[pns_band >> PNS_BAND_FLAGS_SHIFT] >> (pns_band & PNS_BAND_FLAGS_MASK)) & (unsigned char)1;
}

/*
  The function activates the noise correlation between the channel pair
*/
void CPns_SetCorrelation(CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                         const int group,
                         const int band)
{
  CPnsInterChannelData *pInterChannelData = pAacDecoderChannelInfo->pPnsInterChannelData;
  unsigned pns_band = group*MaximumScaleFactorBandsShort+band;

  COUNT_sub_start("CPns_SetCorrelation");

  INDIRECT(1); PTR_INIT(1); MULT(1); ADD(1); /* counting previous operations */

  SHIFT(2); LOGIC(2); INDIRECT(1); STORE(1);
  pInterChannelData->correlated[pns_band >> PNS_BAND_FLAGS_SHIFT] |= (unsigned char)1 << (pns_band & PNS_BAND_FLAGS_MASK);

  COUNT_sub_end();
}

/*
  The function indicates if the noise correlation between the channel pair
  is activated

  return:  PNS used
*/
int CPns_IsCorrelated(CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                      const int group,
                      const int band)
{
  CPnsInterChannelData *pInterChannelData = pAacDecoderChannelInfo->pPnsInterChannelData;
  unsigned pns_band = group*MaximumScaleFactorBandsShort+band;

  COUNT_sub_start("CPns_IsCorrelated");
  INDIRECT(1); PTR_INIT(1); MULT(1); ADD(1); /* counting previous operations */
  SHIFT(2); LOGIC(2); INDIRECT(1); /* counting post-operation */
  COUNT_sub_end();

  return (pInterChannelData->correlated[pns_band >> PNS_BAND_FLAGS_SHIFT] >> (pns_band & PNS_BAND_FLAGS_MASK)) & (unsigned char)1;
}

/*
  The function reads the PNS information from the bitstream
*/
void CPns_Read (CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                HANDLE_BIT_BUF bs,
                const CodeBookDescription *hcb,
                unsigned char global_gain,
                int band,
                int group /* = 0 */)
{
  int delta ;
  unsigned pns_band = group*MaximumScaleFactorBandsShort+band;
  CPnsData *pPnsData = &pAacDecoderChannelInfo->PnsData;

  COUNT_sub_start("CPns_Read");

  INDIRECT(1); PTR_INIT(1); MULT(1); ADD(1); /* counting previous operations */

  INDIRECT(1); BRANCH(1);
  if (pPnsData->PnsActive) {

    INDIRECT(1); FUNC(2); ADD(1);
    delta = CBlock_DecodeHuffmanWord (bs, hcb->CodeBook) - 60;
  } else {
    int noiseStartValue = GetBits(bs,9);

    FUNC(2); /* counting previous operation */

    ADD(1);
    delta = noiseStartValue - 256 ;

    MOVE(1);
    pPnsData->PnsActive = 1;

    INDIRECT(1); ADD(1); STORE(1);
    pPnsData->CurrentEnergy = global_gain - NOISE_OFFSET;
  }

  INDIRECT(1); ADD(1); STORE(1);
  pPnsData->CurrentEnergy += delta ;

  INDIRECT(1); MOVE(1);
  pAacDecoderChannelInfo->pScaleFactor[pns_band] = pPnsData->CurrentEnergy;

  SHIFT(2); LOGIC(2); INDIRECT(1); STORE(1);
  pPnsData->pnsUsed[pns_band >> PNS_BAND_FLAGS_SHIFT] |= (unsigned char)1 << (pns_band & PNS_BAND_FLAGS_MASK);

  COUNT_sub_end();
}

/*
  The function applies PNS (i.e. it generates noise) on the bands
  flagged as noisy bands
*/
void CPns_Apply (CAacDecoderChannelInfo *pAacDecoderChannelInfo[],
                 int channel)
{
  int window, group, groupwin, band;
  CPnsData *pPnsData = &pAacDecoderChannelInfo[channel]->PnsData;
  CIcsInfo *pIcsInfo = &pAacDecoderChannelInfo[channel]->IcsInfo;

  COUNT_sub_start("CPns_Apply");

  INDIRECT(2); PTR_INIT(2); /* counting previous operation */

  INDIRECT(1); BRANCH(1);
  if (pPnsData->PnsActive) {
    const short *BandOffsets = GetScaleFactorBandOffsets(pIcsInfo);
    int fft_exp = IsLongBlock(&pAacDecoderChannelInfo[channel]->IcsInfo)? 9 : 6; /* This coefficient is related to the spectral exponent used in the
                                                                                    requantization (see functions CLongBlock_ReadSpectralData() and 
                                                                                    CShortBlock_ReadSpectralData() ).
                                                                                 */

    FUNC(1); FUNC(1); PTR_INIT(1); MOVE(1); /* counting previous operations */

    FUNC(1); LOOP(1);
    for (window = 0, group = 0; group < GetWindowGroups(pIcsInfo); group++) {

      FUNC(1); LOOP(1);
      for (groupwin = 0; groupwin < GetWindowGroupLength(pIcsInfo, group); groupwin++, window++) {
        float *spectrum = &pAacDecoderChannelInfo[channel]->pSpectralCoefficient[FRAME_SIZE/8*window];

        INDIRECT(1); PTR_INIT(1); /* counting previous operation */

        PTR_INIT(1); /* BandOffsets[band] */
        FUNC(1); LOOP(1);
        for (band = 0 ; band < GetScaleFactorBandsTransmitted(pIcsInfo); band++) {

          FUNC(3); BRANCH(1);
          if (CPns_IsPnsUsed (pAacDecoderChannelInfo[channel], group, band)) {
            unsigned pns_band = group*MaximumScaleFactorBandsShort+band;
            float scale = (float) pow(2.0, 0.25 * pAacDecoderChannelInfo[channel]->pScaleFactor[pns_band] - fft_exp);

            MULT(2); ADD(2); TRANS(1); /* counting previous operation */

            if (CPns_IsCorrelated(pAacDecoderChannelInfo[0], group, band)) {
              if (channel == 0) {
                /* store random state for right channel */
                pAacDecoderChannelInfo[0]->pPnsInterChannelData->randomState[pns_band] = pAacDecoderChannelInfo[0]->pPnsStaticInterChannelData->current_seed;
                GenerateRandomVector(scale,
                                     spectrum,
                                     BandOffsets[band],
                                     BandOffsets[band + 1],
                                     &(pAacDecoderChannelInfo[0]->pPnsStaticInterChannelData->current_seed));
              } else {
                /* use same random state as was used for left channel */
                GenerateRandomVector(scale,
                                     spectrum,
                                     BandOffsets[band],
                                     BandOffsets[band + 1],
                                     &(pAacDecoderChannelInfo[0]->pPnsInterChannelData->randomState[pns_band]));
              }
            }
            else {
              GenerateRandomVector(scale,
                                   spectrum,
                                   BandOffsets[band],
                                   BandOffsets[band + 1],
                                   &(pAacDecoderChannelInfo[0]->pPnsStaticInterChannelData->current_seed));
            }
          }
        }
      }
    }
    pAacDecoderChannelInfo[0]->pPnsStaticInterChannelData->current_seed += pAacDecoderChannelInfo[0]->pPnsStaticInterChannelData->pns_frame_number;
  } 
  if (channel == 0) {
    pAacDecoderChannelInfo[0]->pPnsStaticInterChannelData->pns_frame_number++;
  }

  COUNT_sub_end();
}

void GenerateRandomVector (float scale,
                           float spec[],
                           int bandOffsetStart,
                           int bandOffsetEnd,
                           short *randomState)
{
  int i;
  float nrg = 0.0f;

  COUNT_sub_start("GenerateRandomVector");

  MOVE(1); /* counting previous operation */

  PTR_INIT(2); /* spec[i]
                  sbr_randomPhase[noise_index][0]
               */
  LOOP(1);
  for (i = bandOffsetStart; i < bandOffsetEnd; i++) {

    MOVE(2); MULT(1); ADD(1);
    spec[i] = *randomState = (0x529L * *randomState) + 0x3a7fL; /* snd */

    MAC(1);
    nrg += spec[i] * spec[i];
  }

  TRANS(1); DIV(1);
  scale /= (float) sqrt(nrg);

  PTR_INIT(1); /* spec[i] */
  LOOP(1);
  for (i = bandOffsetStart; i < bandOffsetEnd; i++) {

    MULT(1); STORE(1);
    spec[i] *= scale;
  }

  COUNT_sub_end();
}
