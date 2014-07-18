/*
  envelope decoding
  This module provides envelope decoding and error concealment algorithms. The main
  entry point is decodeSbrData().
*/
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "env_dec.h"
#include "sbr_const.h"
#include "env_extr.h"

#include "counters.h" /* the 3GPP instrumenting tool */

static void decodeEnvelope (HANDLE_SBR_HEADER_DATA hHeaderData,
                            HANDLE_SBR_FRAME_DATA h_sbr_data,
                            HANDLE_SBR_PREV_FRAME_DATA h_prev_data,
                            HANDLE_SBR_PREV_FRAME_DATA h_prev_data_otherChannel);
static void sbr_envelope_unmapping (HANDLE_SBR_HEADER_DATA hHeaderData,
                                    HANDLE_SBR_FRAME_DATA h_data_left,
                                    HANDLE_SBR_FRAME_DATA h_data_right);
static void requantizeEnvelopeData (HANDLE_SBR_FRAME_DATA h_sbr_data,
                                    int ampResolution);
static void deltaToLinearPcmEnvelopeDecoding (HANDLE_SBR_HEADER_DATA hHeaderData,
                                              HANDLE_SBR_FRAME_DATA h_sbr_data,
                                              HANDLE_SBR_PREV_FRAME_DATA h_prev_data);
static void decodeNoiseFloorlevels (HANDLE_SBR_HEADER_DATA hHeaderData,
                                    HANDLE_SBR_FRAME_DATA h_sbr_data,
                                    HANDLE_SBR_PREV_FRAME_DATA h_prev_data);
static void timeCompensateFirstEnvelope (HANDLE_SBR_HEADER_DATA hHeaderData,
                                         HANDLE_SBR_FRAME_DATA h_sbr_data,
                                         HANDLE_SBR_PREV_FRAME_DATA h_prev_data);
static int checkEnvelopeData (HANDLE_SBR_HEADER_DATA hHeaderData,
                              HANDLE_SBR_FRAME_DATA h_sbr_data,
                              HANDLE_SBR_PREV_FRAME_DATA h_prev_data);



#define SBR_ENERGY_PAN_OFFSET   12.0f
#define SBR_MAX_ENERGY          35.0f

#define DECAY                    1.0f

#define DECAY_COUPLING           1.0f




/*
  \brief  Convert table index
*/
static int indexLow2High(int offset,
                         int index,
                         int res)
{
  COUNT_sub_start("indexLow2High");

  ADD(1); BRANCH(1);
  if(res == LO)
  {
    BRANCH(1);
    if (offset >= 0)
    {
        ADD(1); BRANCH(1);
        if (index < offset)
        {
          COUNT_sub_end();
          return(index);
        }
        else
        {
          SHIFT(1); ADD(1); /* counting post-operation */
          COUNT_sub_end();
          return(2*index - offset);
        }
    }
    else
    {
        MULT(1);
        offset = -offset;

        ADD(1); BRANCH(1);
        if (index < offset)
        {
          SHIFT(1); ADD(1); /* counting post-operation */
          COUNT_sub_end();
          return(2*index+index);
        }
        else
        {
          SHIFT(1); ADD(1); /* counting post-operation */
          COUNT_sub_end();
          return(2*index + offset);
        }
    }
  }
  else
  {
    COUNT_sub_end();
    return(index);
  }
}


/*
  \brief  Update previous envelope value for delta-coding

*/
static void mapLowResEnergyVal(float  currVal,
                               float  *prevData,
                               int offset,
                               int index,
                               int res)
{
  COUNT_sub_start("mapLowResEnergyVal");

  ADD(1); BRANCH(1);
  if(res == LO)
  {
    BRANCH(1);
    if (offset >= 0)
    {
      ADD(1); BRANCH(1);
        if(index < offset)
        {
            INDIRECT(1); MOVE(1);
            prevData[index] = currVal;
        }
        else
        {
            INDIRECT(1); MOVE(2);
            prevData[2*index - offset] = currVal;
            prevData[2*index+1 - offset] = currVal;
        }
    }
    else
    {
        MULT(1);
        offset = -offset;

        ADD(1); BRANCH(1);
        if (index < offset)
        {
            INDIRECT(1); MOVE(3);
            prevData[3*index] = currVal;
            prevData[3*index+1] = currVal;
            prevData[3*index+2] = currVal;
        }
        else
        {
            INDIRECT(1); MOVE(2);
            prevData[2*index + offset] = currVal;
            prevData[2*index + 1 + offset] = currVal;
        }
    }
  }
  else
  {
    INDIRECT(1); MOVE(1);
    prevData[index] = currVal;
  }

  COUNT_sub_end();
}



/*
  \brief    Convert raw envelope and noisefloor data to energy levels

*/
void
decodeSbrData (HANDLE_SBR_HEADER_DATA hHeaderData,
               HANDLE_SBR_FRAME_DATA h_data_left,
               HANDLE_SBR_PREV_FRAME_DATA h_prev_data_left,
               HANDLE_SBR_FRAME_DATA h_data_right,
               HANDLE_SBR_PREV_FRAME_DATA h_prev_data_right)
{
  int errLeft;

  COUNT_sub_start("decodeSbrData");

  FUNC(4);
  decodeEnvelope (hHeaderData, h_data_left, h_prev_data_left, h_prev_data_right);

  FUNC(3);
  decodeNoiseFloorlevels (hHeaderData, h_data_left, h_prev_data_left);

  BRANCH(1);
  if(h_data_right != NULL) {

    MOVE(1);
    errLeft = hHeaderData->frameErrorFlag;

    FUNC(4);
    decodeEnvelope (hHeaderData, h_data_right, h_prev_data_right, h_prev_data_left);

    FUNC(3);
    decodeNoiseFloorlevels (hHeaderData, h_data_right, h_prev_data_right);

    INDIRECT(1); LOGIC(1); BRANCH(1);
    if (!errLeft && hHeaderData->frameErrorFlag) {
      FUNC(4);
      decodeEnvelope (hHeaderData, h_data_left, h_prev_data_left, h_prev_data_right);
    }

    INDIRECT(1); BRANCH(1);
    if (h_data_left->coupling) {

      FUNC(3);
      sbr_envelope_unmapping (hHeaderData, h_data_left, h_data_right);
    }
  }

  COUNT_sub_end();
}


/*
  \brief   Convert from coupled channels to independent L/R data
*/
static void
sbr_envelope_unmapping (HANDLE_SBR_HEADER_DATA hHeaderData,
                        HANDLE_SBR_FRAME_DATA h_data_left,
                        HANDLE_SBR_FRAME_DATA h_data_right)
{
  int i;
  float tempR, tempL, newL, newR;
  const float unmapScale = (float) pow(2.0, -16.0);

  COUNT_sub_start("sbr_envelope_unmapping");

  MOVE(1); /* counting previous operations */

  PTR_INIT(2); /* pointers for h_data_left->iEnvelope[],
                               h_data_right->iEnvelope[]
               */
  INDIRECT(1); LOOP(1);
  for (i = 0; i < h_data_left->nScaleFactors; i++) {

    MOVE(1);
    tempR = h_data_right->iEnvelope[i];

    MULT(1);
    tempR = tempR * unmapScale;

    MOVE(1);
    tempL = h_data_left->iEnvelope[i];

    MULT(1); ADD(1); DIV(1);
    newR = 2.0f * tempL / (tempR + 1.0f);

    MULT(1);
    newL = tempR * newR;

    MOVE(2);
    h_data_left->iEnvelope[i] = newL;
    h_data_right->iEnvelope[i] = newR;
  }

  PTR_INIT(2); /* pointers for h_data_left->sbrNoiseFloorLevel[i],
                               h_data_right->sbrNoiseFloorLevel[i]
               */
  INDIRECT(2); MULT(1); LOOP(1);
  for (i = 0; i < hHeaderData->hFreqBandData->nNfb * h_data_left->frameInfo.nNoiseEnvelopes; i++) {

    ADD(2);
    tempL = NOISE_FLOOR_OFFSET - h_data_left->sbrNoiseFloorLevel[i];
    tempR = h_data_right->sbrNoiseFloorLevel[i] - 12.0f /*SBR_ENERGY_PAN_OFFSET*/;

    TRANS(2);
    tempL = (float) pow((double)2.0, (double)tempL);
    tempR = (float) pow((double)2.0, (double)tempR);

    MULT(1); ADD(1); DIV(1);
    newR = 2.0f * tempL / (1.0f + tempR);

    MULT(1);
    newL = tempR * newR;


    MOVE(2);
    h_data_left->sbrNoiseFloorLevel[i] = newL;
    h_data_right->sbrNoiseFloorLevel[i] = newR;
  }

  COUNT_sub_end();
}


/*
  \brief    Simple alternative to the real SBR concealment

*/
static void
leanSbrConcealment(HANDLE_SBR_HEADER_DATA hHeaderData,
                   HANDLE_SBR_FRAME_DATA  h_sbr_data,
                   HANDLE_SBR_PREV_FRAME_DATA h_prev_data
                   )
{
  float  target;
  float  step;
  int i;

  int currentStartPos = h_prev_data->stopPos - hHeaderData->numberTimeSlots;
  int currentStopPos = hHeaderData->numberTimeSlots;

  COUNT_sub_start("leanSbrConcealment");

  INDIRECT(2); ADD(1); MOVE(1); /* counting previous operations */


  INDIRECT(2); MOVE(1);
  h_sbr_data->ampResolutionCurrentFrame = h_prev_data->ampRes;

  INDIRECT(2); MOVE(1);
  h_sbr_data->coupling = h_prev_data->coupling;

  PTR_INIT(2); /* h_sbr_data->sbr_invf_mode[i]
                  h_prev_data->sbr_invf_mode[i]
               */
  LOOP(1);
  for(i=0;i<MAX_INVF_BANDS;i++)
  {
    MOVE(1);
    h_sbr_data->sbr_invf_mode[i] = h_prev_data->sbr_invf_mode[i];
  }

  INDIRECT(8); MOVE(8);
  h_sbr_data->frameInfo.nEnvelopes = 1;
  h_sbr_data->frameInfo.borders[0] = currentStartPos;
  h_sbr_data->frameInfo.borders[1] = currentStopPos;
  h_sbr_data->frameInfo.freqRes[0] = 1;
  h_sbr_data->frameInfo.tranEnv = -1;
  h_sbr_data->frameInfo.nNoiseEnvelopes = 1;
  h_sbr_data->frameInfo.bordersNoise[0] = currentStartPos;
  h_sbr_data->frameInfo.bordersNoise[1] = currentStopPos;

  INDIRECT(2); MOVE(1);
  h_sbr_data->nScaleFactors = hHeaderData->hFreqBandData->nSfb[1];


  INDIRECT(1); MOVE(1);
  h_sbr_data->domain_vec[0] = TIME;

  INDIRECT(1); ADD(1);
  if (h_sbr_data->coupling == COUPLING_BAL) {

    MOVE(2);
    target = SBR_ENERGY_PAN_OFFSET;
    step = DECAY_COUPLING;
  }
  else {

    MOVE(2);
    target = 0.0;
    step = DECAY;
  }

  INDIRECT(1); ADD(1); BRANCH(1);
  if (h_sbr_data->ampResolutionCurrentFrame == SBR_AMP_RES_1_5) {

    MULT(2);
    target *= 2;
    step *= 2;
  }

  PTR_INIT(2); /* h_prev_data->sfb_nrg_prev[i]
                  h_sbr_data->iEnvelope[i]
               */
  INDIRECT(1); LOOP(1);
  for (i=0; i < h_sbr_data->nScaleFactors; i++) {

    ADD(1); BRANCH(1);
    if (h_prev_data->sfb_nrg_prev[i] > target)
    {
      MULT(1); STORE(1);
      h_sbr_data->iEnvelope[i] = -step;
    }
    else
    {
      MOVE(1);
      h_sbr_data->iEnvelope[i] = step;
    }
  }

  INDIRECT(1); MOVE(1);
  h_sbr_data->domain_vec_noise[0] = TIME;

  PTR_INIT(1); /* h_sbr_data->sbrNoiseFloorLevel[i] */
  INDIRECT(1); LOOP(1);
  for (i=0; i < hHeaderData->hFreqBandData->nNfb; i++)
  {
    MOVE(1);
    h_sbr_data->sbrNoiseFloorLevel[i] = 0.0f;
  }

  PTR_INIT(1); LOOP(1);
  for (i=0; i< MAX_FREQ_COEFFS; i++) {
    MOVE(1);
    h_sbr_data->addHarmonics[i] = 0;
  }

  COUNT_sub_end();
}


/*
  \brief   Build reference energies and noise levels from bitstream elements
*/
static void
decodeEnvelope (HANDLE_SBR_HEADER_DATA hHeaderData,
                HANDLE_SBR_FRAME_DATA  h_sbr_data,
                HANDLE_SBR_PREV_FRAME_DATA h_prev_data,
                HANDLE_SBR_PREV_FRAME_DATA otherChannel
                )
{
  int i;
  int Error_flag;
  float tempSfbNrgPrev[MAX_FREQ_COEFFS];

  COUNT_sub_start("decodeEnvelope");


  INDIRECT(5); LOGIC(2); ADD(3); BRANCH(1);
  if ( (!hHeaderData->prevFrameErrorFlag) && (!hHeaderData->frameErrorFlag) &&
       (h_sbr_data->frameInfo.borders[0] != h_prev_data->stopPos - hHeaderData->numberTimeSlots) ) {
    INDIRECT(1); ADD(1); BRANCH(1);
    if (h_sbr_data->domain_vec[0] == TIME) {
      INDIRECT(1); MOVE(1);
      hHeaderData->frameErrorFlag = 1;
    }
    else {
      INDIRECT(1); MOVE(1);
      hHeaderData->prevFrameErrorFlag = 1;
    }
  }


  INDIRECT(1); BRANCH(1);
  if (hHeaderData->frameErrorFlag)
    {
      FUNC(3);
      leanSbrConcealment(hHeaderData,
                         h_sbr_data,
                         h_prev_data);

      FUNC(3);
      deltaToLinearPcmEnvelopeDecoding (hHeaderData, h_sbr_data, h_prev_data);
    }
  else
    {
      INDIRECT(1); BRANCH(1);
      if (hHeaderData->prevFrameErrorFlag) {
        FUNC(3);
        timeCompensateFirstEnvelope (hHeaderData, h_sbr_data, h_prev_data);

        INDIRECT(2); ADD(1); BRANCH(1);
        if (h_sbr_data->coupling != h_prev_data->coupling) {
          PTR_INIT(2); /* pointers for h_prev_data->sfb_nrg_prev[i],
                                       otherChannel->sfb_nrg_prev[i]
                       */
          LOOP(1);
          for (i = 0; i < hHeaderData->hFreqBandData->nSfb[HI]; i++) {

            INDIRECT(1); ADD(1); BRANCH(1);
            if (h_prev_data->coupling == COUPLING_BAL) {

              MOVE(1);
              h_prev_data->sfb_nrg_prev[i] = otherChannel->sfb_nrg_prev[i];
            }

            else {
              INDIRECT(1); ADD(1); BRANCH(1);
              if (h_sbr_data->coupling == COUPLING_LEVEL) {
                ADD(1); MULT(1); STORE(1);
                h_prev_data->sfb_nrg_prev[i] = 0.5f * (h_prev_data->sfb_nrg_prev[i] + otherChannel->sfb_nrg_prev[i]);
              }
              else {
                INDIRECT(1); ADD(1); BRANCH(1);
                if (h_sbr_data->coupling == COUPLING_BAL) {
                  MOVE(1);
                  h_prev_data->sfb_nrg_prev[i] = SBR_ENERGY_PAN_OFFSET;
                }
              }
            }
          }
        }
      }
      FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_FREQ_COEFFS);
      memcpy (tempSfbNrgPrev, h_prev_data->sfb_nrg_prev,
              MAX_FREQ_COEFFS * sizeof (float));

      FUNC(3);
      deltaToLinearPcmEnvelopeDecoding (hHeaderData, h_sbr_data, h_prev_data);

      FUNC(3);
      Error_flag = checkEnvelopeData (hHeaderData, h_sbr_data, h_prev_data);

      BRANCH(1);
      if (Error_flag)
        {
          INDIRECT(1); MOVE(1);
          hHeaderData->frameErrorFlag = 1;

          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_FREQ_COEFFS);
          memcpy (h_prev_data->sfb_nrg_prev, tempSfbNrgPrev,
                  MAX_FREQ_COEFFS * sizeof (float));

          FUNC(4);
          decodeEnvelope (hHeaderData, h_sbr_data, h_prev_data, otherChannel);
          return;
        }
    }

  INDIRECT(1); FUNC(2);
  requantizeEnvelopeData (h_sbr_data, h_sbr_data->ampResolutionCurrentFrame);

  COUNT_sub_end();
}


/*
  \brief   Verify that envelope energies are within the allowed range
  \return  0 if all is fine, 1 if an envelope value was too high
*/
static int
checkEnvelopeData (HANDLE_SBR_HEADER_DATA hHeaderData,
                   HANDLE_SBR_FRAME_DATA h_sbr_data,
                   HANDLE_SBR_PREV_FRAME_DATA h_prev_data
                   )
{
  float *iEnvelope = h_sbr_data->iEnvelope;
  float *sfb_nrg_prev = h_prev_data->sfb_nrg_prev;
  int    i = 0, errorFlag = 0;
  float  sbr_max_energy =
    (h_sbr_data->ampResolutionCurrentFrame == SBR_AMP_RES_3_0) ? SBR_MAX_ENERGY : (2.0f * SBR_MAX_ENERGY);

  COUNT_sub_start("checkEnvelopeData");

  INDIRECT(3); PTR_INIT(2); MOVE(3); ADD(1); BRANCH(1); /* counting previous operation */


  INDIRECT(1); LOOP(1);
  for (i = 0; i < h_sbr_data->nScaleFactors; i++) {

    ADD(1); BRANCH(1);
    if (iEnvelope[i] > sbr_max_energy) {
      MOVE(1);
      errorFlag = 1;
    }

    BRANCH(1);
    if (iEnvelope[i] < 0)
    {
      MOVE(1);
      iEnvelope[i] = 0;
    }
  }


  INDIRECT(1); LOOP(1);
  for (i = 0; i < hHeaderData->hFreqBandData->nSfb[HI]; i++) {

    BRANCH(1);
    if (sfb_nrg_prev[i] < 0)
    {
      MOVE(1);
      sfb_nrg_prev[i] = 0;
    }
    else {
    ADD(1); BRANCH(1);
    if (sfb_nrg_prev[i] > sbr_max_energy) {
      MOVE(1);
      sfb_nrg_prev[i] = sbr_max_energy;
    }
    }
  }

  COUNT_sub_end();

  return (errorFlag);
}


/*
  \brief   Verify that the noise levels are within the allowed range

*/
static void
limitNoiseLevels(HANDLE_SBR_HEADER_DATA hHeaderData,
                 HANDLE_SBR_FRAME_DATA h_sbr_data)
{
  int i;
  int nNfb = hHeaderData->hFreqBandData->nNfb;
  float value;

  float lowerLimit = 0.0;
  float upperLimit = 35.0;

  COUNT_sub_start("limitNoiseLevels");

  INDIRECT(1); MOVE(3); /* counting previous operations */

  PTR_INIT(1); /* h_sbr_data->sbrNoiseFloorLevel[i] */
  INDIRECT(1); MULT(1); LOOP(1);
  for (i = 0; i < h_sbr_data->frameInfo.nNoiseEnvelopes * nNfb; i++) {

    MOVE(1);
    value = h_sbr_data->sbrNoiseFloorLevel[i];

    ADD(1); BRANCH(1);
    if (value > upperLimit)
    {
      MOVE(1);
      h_sbr_data->sbrNoiseFloorLevel[i] = upperLimit;
    }
    else {
    ADD(1); BRANCH(1);
    if (value < lowerLimit)
    {
      MOVE(1);
      h_sbr_data->sbrNoiseFloorLevel[i] = lowerLimit;
    }
    }
  }

  COUNT_sub_end();
}


/*
  \brief   Compensate for the wrong timing that might occur after a frame error.
*/
static void
timeCompensateFirstEnvelope (HANDLE_SBR_HEADER_DATA hHeaderData,
                             HANDLE_SBR_FRAME_DATA h_sbr_data,
                             HANDLE_SBR_PREV_FRAME_DATA h_prev_data)
{
  int i, nScalefactors;
  FRAME_INFO *pFrameInfo = &h_sbr_data->frameInfo;
  unsigned char *nSfb = hHeaderData->hFreqBandData->nSfb;
  int estimatedStartPos = h_prev_data->stopPos - hHeaderData->numberTimeSlots;
  int refLen, newLen;
  float deltaExp;

  COUNT_sub_start("timeCompensateFirstEnvelope");

  INDIRECT(4); PTR_INIT(2); MOVE(1); /* counting previous operations */

  INDIRECT(2); ADD(1);
  refLen = pFrameInfo->borders[1] - pFrameInfo->borders[0];

  ADD(1);
  newLen = pFrameInfo->borders[1] - estimatedStartPos;

  BRANCH(1);
  if (newLen <= 0) {

    MOVE(2);
    newLen = refLen;
    estimatedStartPos = pFrameInfo->borders[0];
  }

  DIV(1);
  deltaExp = (float)refLen / (float)newLen;

  TRANS(1); DIV(1);
  deltaExp = (float)( log(deltaExp)/log(2.0) );

  INDIRECT(1); ADD(1); BRANCH(1);
  if (h_sbr_data->ampResolutionCurrentFrame == SBR_AMP_RES_1_5) /* fine resolution */
  {
    MULT(1);
    deltaExp *= 2.0f;
  }

  INDIRECT(1); MOVE(2);
  pFrameInfo->borders[0] = estimatedStartPos;
  pFrameInfo->bordersNoise[0] = estimatedStartPos;

  INDIRECT(1); ADD(1); BRANCH(1);
  if (h_sbr_data->coupling != COUPLING_BAL) {

    INDIRECT(1); BRANCH(1); MOVE(1);
    nScalefactors = (pFrameInfo->freqRes[0]) ? nSfb[HI] : nSfb[LO];

    PTR_INIT(1); /* h_sbr_data->iEnvelope[i] */
    LOOP(1);
    for (i = 0; i < nScalefactors; i++)
    {
      ADD(1); STORE(1);
      h_sbr_data->iEnvelope[i] = h_sbr_data->iEnvelope[i] + deltaExp;
    }
  }

  COUNT_sub_end();
}



/*
  \brief   Convert each envelope value from logarithmic to linear domain

*/
static void
requantizeEnvelopeData (HANDLE_SBR_FRAME_DATA h_sbr_data, int ampResolution)
{
  int i;
  float mantissa;
  int ampShift = 1 - ampResolution;
  int exponent;

  COUNT_sub_start("requantizeEnvelopeData");

  ADD(1); /* counting previous operation */

  PTR_INIT(1); /* h_sbr_data->iEnvelope[] */
  INDIRECT(1); LOOP(1);
  for (i = 0; i < h_sbr_data->nScaleFactors; i++) {

    MOVE(1);
    exponent = (int)h_sbr_data->iEnvelope[i];

    LOGIC(1); BRANCH(1); MOVE(1);
    mantissa = (exponent & ampShift) ? 0.707106781186548f : 0.5f;

    SHIFT(1);
    exponent = exponent >> ampShift;

    ADD(1);
    exponent += 5;

    TRANS(1); MULT(1); STORE(1);
    h_sbr_data->iEnvelope[i] = (float) (mantissa * pow(2.0, (double)exponent));
  }

  COUNT_sub_end();
}


/*
  \brief   Build new reference energies from old ones and delta coded data
*/
static void
deltaToLinearPcmEnvelopeDecoding (HANDLE_SBR_HEADER_DATA hHeaderData,
                                  HANDLE_SBR_FRAME_DATA h_sbr_data,
                                  HANDLE_SBR_PREV_FRAME_DATA h_prev_data)
{
  int i, domain, no_of_bands, band, freqRes, offset;
  float *sfb_nrg_prev;
  float *ptr_nrg;

  COUNT_sub_start("deltaToLinearPcmEnvelopeDecoding");

  PTR_INIT(2);
  sfb_nrg_prev = h_prev_data->sfb_nrg_prev;
  ptr_nrg = h_sbr_data->iEnvelope;

  INDIRECT(2); MULT(1); ADD(1);
  offset = 2 * hHeaderData->hFreqBandData->nSfb[LO] - hHeaderData->hFreqBandData->nSfb[HI];

  PTR_INIT(2); /* pointers for h_sbr_data->domain_vec[i],
                               h_sbr_data->frameInfo.freqRes[i]
               */
  LOOP(1);
  for (i = 0; i < h_sbr_data->frameInfo.nEnvelopes; i++) {

    MOVE(2);
    domain = h_sbr_data->domain_vec[i];
    freqRes = h_sbr_data->frameInfo.freqRes[i];

    INDIRECT(1); MOVE(1);
    no_of_bands = hHeaderData->hFreqBandData->nSfb[freqRes];

    ADD(1); BRANCH(1);
    if (domain == FREQ)
    {
      FUNC(5);
      mapLowResEnergyVal(*ptr_nrg, sfb_nrg_prev, offset, 0, freqRes);

      ptr_nrg++;

      LOOP(1);
      for (band = 1; band < no_of_bands; band++)
      {
        *ptr_nrg = *ptr_nrg + *(ptr_nrg-1);

        FUNC(5);
        mapLowResEnergyVal(*ptr_nrg, sfb_nrg_prev, offset, band, freqRes);

        ptr_nrg++;
      }
    }
    else
    {
      LOOP(1);
      for (band = 0; band < no_of_bands; band++)
      {

        FUNC(3); INDIRECT(1); MOVE(1);
        *ptr_nrg = *ptr_nrg + sfb_nrg_prev[indexLow2High(offset, band, freqRes)];

        FUNC(5);
        mapLowResEnergyVal(*ptr_nrg, sfb_nrg_prev, offset, band, freqRes);

        ptr_nrg++;
      }
    }
  }

  COUNT_sub_end();
}


/*
  \brief   Build new noise levels from old ones and delta coded data
*/
static void
decodeNoiseFloorlevels (HANDLE_SBR_HEADER_DATA hHeaderData,
                        HANDLE_SBR_FRAME_DATA h_sbr_data,
                        HANDLE_SBR_PREV_FRAME_DATA h_prev_data)
{
  int i;
  int nNfb = hHeaderData->hFreqBandData->nNfb;
  int nNoiseFloorEnvelopes = h_sbr_data->frameInfo.nNoiseEnvelopes;

  COUNT_sub_start("decodeNoiseFloorlevels");

  MOVE(2); /* counting previous operations */

  ADD(1); BRANCH(1);
  if (h_sbr_data->domain_vec_noise[0] == FREQ) {

    PTR_INIT(1); /* pointers for h_sbr_data->sbrNoiseFloorLevel[]
                  */
    LOOP(1);
    for (i = 1; i < nNfb; i++) {

      ADD(1); STORE(1);
      h_sbr_data->sbrNoiseFloorLevel[i] =
        h_sbr_data->sbrNoiseFloorLevel[i] + h_sbr_data->sbrNoiseFloorLevel[i - 1];
    }
  }
  else {
    PTR_INIT(1); /* pointers for h_sbr_data->sbrNoiseFloorLevel[]
                  */
    LOOP(1);
    for (i = 0; i < nNfb; i++) {

      ADD(1); STORE(1);
      h_sbr_data->sbrNoiseFloorLevel[i] =
        h_sbr_data->sbrNoiseFloorLevel[i] + h_prev_data->prevNoiseLevel[i];
    }
  }

  ADD(1); BRANCH(1);
  if (nNoiseFloorEnvelopes > 1) {

    ADD(1); BRANCH(1);
    if (h_sbr_data->domain_vec_noise[1] == FREQ) {

      PTR_INIT(1); /* pointers for h_sbr_data->sbrNoiseFloorLevel[]
                    */
      MULT(1); LOOP(1);
      for (i = nNfb + 1; i < 2*nNfb; i++) {

        ADD(1); STORE(1);
        h_sbr_data->sbrNoiseFloorLevel[i] =
          h_sbr_data->sbrNoiseFloorLevel[i] + h_sbr_data->sbrNoiseFloorLevel[i - 1];
      }
    }
    else {

      PTR_INIT(1); /* pointers for h_sbr_data->sbrNoiseFloorLevel[]
                    */
      LOOP(1);
      for (i = 0; i < nNfb; i++) {

        ADD(1); STORE(1);
        h_sbr_data->sbrNoiseFloorLevel[i + nNfb] =
          h_sbr_data->sbrNoiseFloorLevel[i + nNfb] + h_sbr_data->sbrNoiseFloorLevel[i];
      }
    }
  }

  FUNC(2);
  limitNoiseLevels(hHeaderData, h_sbr_data);

  MULT(1); ADD(1); PTR_INIT(2); /* pointers for h_sbr_data->sbrNoiseFloorLevel[],
                                                h_prev_data->prevNoiseLevel[]
                                */
  LOOP(1);
  for (i = 0; i < nNfb; i++) {

    MOVE(1);
    h_prev_data->prevNoiseLevel[i] = h_sbr_data->sbrNoiseFloorLevel[i + nNfb*(nNoiseFloorEnvelopes-1)];
  }


  BRANCH(1);
  if (!h_sbr_data->coupling) {
    int nf_e;

    PTR_INIT(1); /* pointers for h_sbr_data->sbrNoiseFloorLevel[]
                  */
    MULT(1); LOOP(1);
    for (i = 0; i < nNoiseFloorEnvelopes*nNfb; i++) {

      ADD(1);
      nf_e = NOISE_FLOOR_OFFSET - (long)h_sbr_data->sbrNoiseFloorLevel[i];

      TRANS(1); STORE(1);
      h_sbr_data->sbrNoiseFloorLevel[i] = (float) pow(2.0, (double)nf_e);
    }
  }

  COUNT_sub_end();
}
