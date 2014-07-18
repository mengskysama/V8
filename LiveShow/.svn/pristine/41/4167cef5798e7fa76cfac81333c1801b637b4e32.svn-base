/*
  Envelope calculation
*/

#include <assert.h>
#include "env_calc.h"
#include "sbr_const.h"
#include "freq_sca.h"
#include "env_extr.h"
#include "sbr_ram.h"
#include "sbr_rom.h"
#include "FloatFR.h"
#include <math.h>

#include "counters.h" /* the 3GPP instrumenting tool */

#define EPS           1e-12

static void updateFiltBuffer(float *filtBuffer,
                             float *filtBufferNoise,
                             float *nrgGain,
                             float *noiseLevel,
                             int    noSubbands);

static void calcNrgPerSubband(float  **analysBufferReal,
#ifndef LP_SBR_ONLY
                              float  **analysBufferImag,
#endif
                              int    lowSubband, int highSubband,
                              int    start_pos,  int next_pos,
                              float  *nrgEst,
                              int    bUseLP);

static void calcNrgPerSfb(float  **analysBufferReal,
#ifndef LP_SBR_ONLY
                          float  **analysBufferImag,
#endif
                          int    nSfb,
                          unsigned char *freqBandTable,
                          int    start_pos,  int next_pos,
                          float  *nrg_est,
                          int    bUseLP);

static void calcSubbandGain(float nrgRef,
                            float nrgEst,
                            float tmpNoise,
                            float *ptrNrgGain,
                            float *ptrNoiseLevel,
                            float *ptrNrgSine,
                            char   sinePresentFlag,
                            char   sineMapped,
                            int    noNoiseFlag);

static void calcAvgGain(float  *nrgRef,
                        float  *nrgEst,
                        int     lowSubband,
                        int     highSubband,
                        float  *sumRef,
                        float  *ptrAvgGain);

static void adjustTimeSlotLP(float  *ptrReal,
                             float  *gain,
                             float  *noiseLevel,
                             float  *sineLevel,
                             char   *ptrHarmIndex,
                             int    lowSubbands,
                             int    noSubbands,
                             int    noNoiseFlag,
                             int    *ptrPhaseIndex);
#ifndef LP_SBR_ONLY
static void adjustTimeSlot(float  *ptrReal,
                           float  *ptrImag,
                           float  *filtBuffer,
                           float  *filtBufferNoise,
                           float  *gain,
                           float  *noiseLevel,
                           float  *sineLevel,
                           char   *ptrHarmIndex,
                           int    lowSubbands,
                           int    noSubbands,
                           float  smooth_ratio,
                           int    noNoiseFlag,
                           int    *ptrPhaseIndex);
#endif


static void mapSineFlags(unsigned char *freqBandTable,
                         int nSfb,
                         char *addHarmonics,
                         int *harmFlagsPrev,
                         int tranEnv,
                         char *sineMapped)

{
  unsigned int mask;
  int qmfband2, li, ui, i;
  int lowSubband2 = 2 * freqBandTable[0];
  int bitcount = 0;
  int oldflags = 0;
  int newflags = 0;

  COUNT_sub_start("mapSineFlags");

  MULT(1); MOVE(3); /* counting previous operations */


  PTR_INIT(1); /* pointers for sineMapped[]
                */
  LOOP(1);
  for (i=0; i<MAX_FREQ_COEFFS; i++) {

    MOVE(1);
    sineMapped[i] = MAX_ENVELOPES;
  }

  PTR_INIT(1); /* pointers for freqBandTable[]
                */
  LOOP(1);
  for (i=nSfb-1; i>=0; i--) {

    MOVE(2);
    li = freqBandTable[i];
    ui = freqBandTable[i+1];

    SHIFT(1);
    mask = 1 << bitcount;

    BRANCH(1);
    if (bitcount == 0) {

      MOVE(2);
      oldflags = *harmFlagsPrev;
      newflags = 0;
    }

    BRANCH(1);
    if ( addHarmonics[i] ) {

      LOGIC(1);
      newflags |= mask;

      ADD(2);
      qmfband2 = ui+li-lowSubband2;


      INDIRECT(1); LOGIC(1); BRANCH(1); MOVE(1);
      sineMapped[qmfband2 >> 1] = ( oldflags & mask ) ? 0 : tranEnv;
    }

    ADD(2); LOGIC(1); BRANCH(1);
    if ((++bitcount == 16) || i==0) {

      MOVE(1);
      bitcount = 0;

      MOVE(1);
      *harmFlagsPrev++ = newflags;
    }
  }

  COUNT_sub_end();
}


/*!
  \brief     Reduce gain-adjustment induced aliasing for real valued filterbank.
*/
static void
aliasingReduction(float*  degreeAlias,
                  float*  nrgGain,
                  float*  nrgEst,
                  int*    useAliasReduction,
                  int noSubbands)
{
  int group, grouping = 0, index = 0, noGroups, k;
  int groupVector[MAX_FREQ_COEFFS];

  COUNT_sub_start("aliasingReduction");

  MOVE(2); /* counting previous operations */


  PTR_INIT(3); /* pointers for degreeAlias[],
                               useAliasReduction[],
                               groupVector[]
                */
  LOOP(1);
  for (k = 0; k < noSubbands-1; k++ ){

    LOGIC(1); BRANCH(1);
    if ( (degreeAlias[k + 1] != 0) && useAliasReduction[k] ) {

      BRANCH(1);
      if(grouping==0){

        MOVE(2);
        groupVector[index] = k;
        grouping = 1;

        index++;
      }
      else{
        ADD(2); BRANCH(1);
        if(groupVector[index-1] + 3 == k){

          ADD(1); STORE(1);
          groupVector[index] = k + 1;

          MOVE(1);
          grouping = 0;

          index++;
        }
      }
    }
    else{
      BRANCH(1);
      if(grouping){

        BRANCH(1);
        if(useAliasReduction[k]) {

          ADD(1); STORE(1);
          groupVector[index] = k + 1;
        }
        else {

          MOVE(1);
          groupVector[index] = k;
        }

        MOVE(1);
        grouping = 0;

        index++;
      }
    }
  }

  BRANCH(1);
  if(grouping){

    MOVE(1);
    groupVector[index] = noSubbands;

    index++;
  }

  SHIFT(1);
  noGroups = index >> 1;


  PTR_INIT(1); /* pointers for groupVector[]
                */
  LOOP(1);
  for (group = 0; group < noGroups; group ++) {
    float  nrgOrig = 0.0f;
    float  nrgAmp = 0.0f;
    float  nrgMod = 0.0f;
    float  groupGain;
    float  compensation;

    int startGroup = groupVector[2*group];
    int stopGroup  = groupVector[2*group+1];

    MOVE(5); /* counting previous operations */

    PTR_INIT(2); /* pointers for nrgEst[],
                                 nrgGain[]
                 */
    LOOP(1);
    for(k = startGroup; k < stopGroup; k++){
      float tmp;

      MOVE(1);
      tmp = nrgEst[k];

      ADD(1);
      nrgOrig += tmp;


      MAC(1);
      nrgAmp += tmp * nrgGain[k];
    }


    ADD(1); DIV(1);
    groupGain = nrgAmp / (nrgOrig + (float)EPS);

    PTR_INIT(3); /* pointers for degreeAlias[],
                                 nrgGain[],
                                 nrgEst[]
                 */
    LOOP(1);
    for(k = startGroup; k < stopGroup; k++){
      float alpha;

      MOVE(1);
      alpha = degreeAlias[k];

      ADD(2); BRANCH(1);
      if (k < noSubbands - 1) {

        ADD(1); BRANCH(1);
        if (degreeAlias[k + 1] > alpha) {

          MOVE(1);
          alpha = degreeAlias[k + 1];
        }
      }


      ADD(1); MULT(1); MAC(1); STORE(1);
      nrgGain[k] = alpha*groupGain + (1.0f-alpha)*nrgGain[k];


      MAC(1);
      nrgMod += nrgGain[k] * nrgEst[k];
    }

     ADD(1); DIV(1);
    compensation = nrgAmp / (nrgMod + (float)EPS);

    PTR_INIT(1); /* pointers for nrgGain[k]
                  */
    LOOP(1);
    for(k = startGroup; k < stopGroup; k++){

      MULT(1); STORE(1);
      nrgGain[k] = nrgGain[k] * compensation;
    }
  }

  COUNT_sub_end();
}


/*!
  \brief  Apply spectral envelope to subband samples
*/
void
calculateSbrEnvelope (HANDLE_SBR_CALCULATE_ENVELOPE h_sbr_cal_env,
                      HANDLE_SBR_HEADER_DATA hHeaderData,
                      HANDLE_SBR_FRAME_DATA  hFrameData,
                      float **analysBufferReal_m,
#ifndef LP_SBR_ONLY
                      float **analysBufferImag_m,
#endif
                      float *degreeAlias,
                      int   bUseLP
                      )
{
  int c, cc, li, ui, i, j, k, l, m, envNoise;
  unsigned char start_pos, stop_pos, freq_res;

  unsigned char nEnvelopes;
  unsigned char*   borders;

  float *noiseLevels;
  HANDLE_FREQ_BAND_DATA hFreq;

  int noNoiseFlag;
  int smooth_length;

  const unsigned char *nSfb;

  int timeStep;
  int lowSubband;
  int highSubband;

  int noSubbands;

  int    noNoiseBands;

  float  nrgRef[MAX_FREQ_COEFFS];
  float  nrgEst[MAX_FREQ_COEFFS];
  float  noiseLevel[MAX_FREQ_COEFFS];
  float  nrgGain[MAX_FREQ_COEFFS];
  float  nrgSine[MAX_FREQ_COEFFS];
  char   sineMapped[MAX_FREQ_COEFFS];

  int useAliasReduction[64];

  float  limiterGain;

  COUNT_sub_start("calculateSbrEnvelope");

  MOVE(2);
  m = 0, envNoise = 0;

  INDIRECT(2); MOVE(2);
  nEnvelopes = hFrameData->frameInfo.nEnvelopes;
  borders = hFrameData->frameInfo.borders;

  INDIRECT(2); PTR_INIT(2);
  noiseLevels = hFrameData->sbrNoiseFloorLevel;
  hFreq = hHeaderData->hFreqBandData;

  INDIRECT(1); PTR_INIT(1);
  nSfb = hFreq->nSfb;

  INDIRECT(3); MOVE(3);
  timeStep = hHeaderData->timeStep;
  lowSubband = hFreq->lowSubband;
  highSubband = hFreq->highSubband;

  ADD(1);
  noSubbands = highSubband - lowSubband;

  INDIRECT(1); MOVE(1);
  noNoiseBands = hFreq->nNfb;

  INDIRECT(2); MOVE(2);
  limiterGain = sbr_limGains[hHeaderData->limiterGains];


  INDIRECT(5); FUNC(6);
  mapSineFlags(hFreq->freqBandTable[HI],
               hFreq->nSfb[HI],
               hFrameData->addHarmonics,
               h_sbr_cal_env->harmFlagsPrev,
               hFrameData->frameInfo.tranEnv,
               sineMapped);



  PTR_INIT(12); /* pointers for hFrameData->frameInfo.freqRes[],
                                hFrameData->frameInfo.bordersNoise[],
                                hFrameData->frameInfo.tranEnv,
                                h_sbr_cal_env->prevTranEnv,
                                hHeaderData->smoothingLength,
                                hHeaderData->interpolFreq,
                                hFreq->freqBandTableNoise[1],
                                useAliasReduction[],
                                hFreq->noLimiterBands,
                                h_sbr_cal_env->startUp,
                                h_sbr_cal_env->filtBuffer,
                                h_sbr_cal_env->filtBufferNoise
                */
  LOOP(1);
  for (i = 0; i < nEnvelopes; i++) {




    MULT(2);
    start_pos = timeStep * borders[i];
    stop_pos = timeStep * borders[i+1];

    MOVE(1);
    freq_res = hFrameData->frameInfo.freqRes[i];

    PTR_INIT(2); /* pointers for nSfb[freq_res],
                                 hFreq->freqBandTable[freq_res]
                 */


    ADD(1); BRANCH(1);
    if (borders[i] == hFrameData->frameInfo.bordersNoise[envNoise+1]){

      ADD(2);
      noiseLevels += noNoiseBands;
      envNoise++;
    }

    ADD(2); LOGIC(1); BRANCH(1);
    if(i==hFrameData->frameInfo.tranEnv || i==h_sbr_cal_env->prevTranEnv) {

      MOVE(2);
      noNoiseFlag = 1;
      smooth_length = 0;
    }
    else {

      MOVE(1);
      noNoiseFlag = 0;

      ADD(1); SHIFT(1);
      smooth_length = (1 - hHeaderData->smoothingLength) << 2;
    }


    BRANCH(1);
    if (hHeaderData->interpolFreq) {

      FUNC(8);
      calcNrgPerSubband(analysBufferReal_m,
#ifndef LP_SBR_ONLY
                        analysBufferImag_m,
#endif
                        lowSubband, highSubband,
                        start_pos, stop_pos,
                        nrgEst,
                        bUseLP);
    }
    else {

      FUNC(8);
      calcNrgPerSfb(analysBufferReal_m,
#ifndef LP_SBR_ONLY
                    analysBufferImag_m,
#endif
                    nSfb[freq_res],
                    hFreq->freqBandTable[freq_res],
                    start_pos, stop_pos,
                    nrgEst,
                    bUseLP);
    }


    {
      unsigned char * table;

      unsigned char uiNoise;
      unsigned char noiseBandIndex;

      PTR_INIT(1);
      table = hFreq->freqBandTable[freq_res];

      MOVE(2);
      uiNoise = hFreq->freqBandTableNoise[1];
      noiseBandIndex = 0;

      MOVE(2);
      c = 0, cc = 0;

      PTR_INIT(1); /* pointer for table[] */
      LOOP(1);
      for (j = 0; j < nSfb[freq_res]; j++) {
        char sinePresentFlag;

        MOVE(2);
        li = table[j];
        ui = table[j+1];

        MOVE(1);
        sinePresentFlag = 0;

        PTR_INIT(1); /* pointer for sineMapped[] */
        LOOP(1);
        for (k=li; k<ui; k++) {

          ADD(1); BRANCH(1);
          if(i >= sineMapped[cc]) {

            MOVE(1);
            sinePresentFlag = 1;
          }
          ADD(1);
          cc++;
        }


        PTR_INIT(8); /* pointers for hFreq->freqBandTableNoise[],
                                     useAliasReduction[],
                                     hFrameData->iEnvelope[],
                                     nrgRef[],
                                     nrgSine[],
                                     nrgEst[],
                                     nrgGain[],
                                     noiseLevel[]
                     */
        LOOP(1);
        for (k=li; k<ui; k++) {

          ADD(1); BRANCH(1);
          if(k >= uiNoise){

            ADD(1);
            noiseBandIndex++;

            MOVE(1);
            uiNoise = hFreq->freqBandTableNoise[noiseBandIndex+1];
          }

          BRANCH(1);
          if (bUseLP) {

            BRANCH(1); MOVE(1);
            if (sinePresentFlag)
              useAliasReduction[k-lowSubband] = 0;
            else
              useAliasReduction[k-lowSubband] = 1;
          }


          MOVE(1);
          nrgRef[c] = hFrameData->iEnvelope[m];

          MOVE(1);
          nrgSine[c] = 0;

          {
            ADD(1); FUNC(9);
            calcSubbandGain(nrgRef[c],
                            nrgEst[c],
                            noiseLevels[noiseBandIndex],
                            &nrgGain[c],
                            &noiseLevel[c],
                            &nrgSine[c],
                            sinePresentFlag, i >= sineMapped[c],
                            noNoiseFlag);
          }
          ADD(1);
          c++;
        }
        ADD(1);
        m++;
      }
    }



    PTR_INIT(1); /* pointer for hFreq->limiterBandTable[] */
    LOOP(1);
    for (c = 0; c < hFreq->noLimiterBands; c++) {
      float maxGain, sumRef;

      const float SBR_MAX_GAIN = 1.0e10;
      MOVE(1); /* previous initialization */

      PTR_INIT(2); FUNC(5);
      calcAvgGain(nrgRef,
                  nrgEst,
                  hFreq->limiterBandTable[c], hFreq->limiterBandTable[c+1],
                  &sumRef,
                  &maxGain);

      /* Multiply maxGain with limiterGain: */
      MULT(1);
      maxGain *= limiterGain;

      ADD(1); BRANCH(1);
      if (maxGain > SBR_MAX_GAIN) {
        MOVE(1);
        maxGain = SBR_MAX_GAIN;
      }

      PTR_INIT(4); /* pointer for nrgGain[],
                                  noiseLevel[],
                                  nrgEst[],
                                  nrgSine[]
                   */
      LOOP(1);
      for (k = hFreq->limiterBandTable[c]; k < hFreq->limiterBandTable[c+1]; k++) {

        ADD(1); BRANCH(1);
        if (nrgGain[k] > maxGain) {

          DIV(1); MULT(1); STORE(1);
          noiseLevel[k] *= (maxGain / nrgGain[k]);

          MOVE(1);
          nrgGain[k] = maxGain;
        }
      }


      {
        float  boostGain, accu;
        MOVE(1);
        accu = 0.0f;
        PTR_INIT(4); /* pointer for nrgGain[],
                                    noiseLevel[],
                                    nrgEst[],
                                    nrgSine[]
                     */
        LOOP(1);
        for (k = hFreq->limiterBandTable[c]; k < hFreq->limiterBandTable[c + 1]; k++) {

          MAC(1);
          accu += nrgGain[k] * nrgEst[k];

          BRANCH(1);
          if(nrgSine[k] != 0.0f) {

            ADD(1);
            accu += nrgSine[k];
          }
          else {
            BRANCH(1);
            if(noNoiseFlag == 0) {

              ADD(1);
              accu += noiseLevel[k];
            }
          }
        }

        BRANCH(1);
        if (accu == 0) {

          MOVE(1);
          accu = (float)EPS;
        }

        DIV(1);
        boostGain = sumRef / accu;

        ADD(1); BRANCH(1);
        if(boostGain > 2.51188643f) {

          MOVE(1);
          boostGain = 2.51188643f;
        }

        PTR_INIT(4); /* pointer for nrgGain[],
                                    noiseLevel[],
                                    nrgEst[],
                                    nrgSine[]
                     */
        LOOP(1);
        for (k = hFreq->limiterBandTable[c]; k < hFreq->limiterBandTable[c + 1]; k++) {

          MULT(3); STORE(3);
          nrgGain[k] = nrgGain[k] * boostGain;
          nrgSine[k] = nrgSine[k] * boostGain;
          noiseLevel[k] = noiseLevel[k] * boostGain;
        }
      }
    }


    BRANCH(1);
    if (bUseLP) {

      ADD(1); FUNC(5);
      aliasingReduction(degreeAlias+lowSubband,
                        nrgGain,
                        nrgEst,
                        useAliasReduction,
                        noSubbands);
    }


    PTR_INIT(4); /* pointer for nrgGain[],
                                noiseLevel[],
                                nrgEst[],
                                nrgSine[]
                 */
    LOOP(1);
    for (k=0; k<noSubbands; k++) {

      TRANS(3); STORE(3);
      nrgSine[k] = (float) sqrt(nrgSine[k]);
      nrgGain[k] = (float) sqrt(nrgGain[k]);
      noiseLevel[k] = (float) sqrt(noiseLevel[k]);
    }


    BRANCH(1);
    if (h_sbr_cal_env->startUp) {
      
      PTR_INIT(4); /* pointer for h_sbr_cal_env->filtBufferNoise[],
                      h_sbr_cal_env->filtBuffer[],
                      nrgGain[],
                      noiseLevel[]
                   */
      LOOP(1);
      for (k = 0; k < noSubbands; k++) {
        
        MOVE(2);
        h_sbr_cal_env->filtBufferNoise[k] = noiseLevel[k];
        h_sbr_cal_env->filtBuffer[k] = nrgGain[k];
      }
      
      MOVE(1);
      h_sbr_cal_env->startUp = 0;
    }
    
    
    PTR_INIT(1); /* pointer for sbr_smoothFilter[] */
    LOOP(1);
    for (l = start_pos; l < stop_pos; l++) {
      
      BRANCH(1);
      if (bUseLP) {
        
        PTR_INIT(3); FUNC(9);
        adjustTimeSlotLP( *(analysBufferReal_m + l) + lowSubband,
                          nrgGain,
                          noiseLevel,
                          nrgSine,
                          &h_sbr_cal_env->harmIndex,
                          lowSubband,
                          noSubbands,
                          noNoiseFlag,
                          &h_sbr_cal_env->phaseIndex);
      }
#ifndef LP_SBR_ONLY
      else {
        
        float smooth_ratio;
        
        ADD(2); BRANCH(1);
        if (l-start_pos < smooth_length) {
          
          MOVE(1);
          smooth_ratio = sbr_smoothFilter[l-start_pos];
        }
        else {
          
          MOVE(1);
          smooth_ratio = 0;
        }
        
        PTR_INIT(4); FUNC(9);
        adjustTimeSlot( *(analysBufferReal_m + l) + lowSubband,
                        *(analysBufferImag_m + l) + lowSubband,
                        h_sbr_cal_env->filtBuffer,
                        h_sbr_cal_env->filtBufferNoise,
                        nrgGain,
                        noiseLevel,
                        nrgSine,
                        &h_sbr_cal_env->harmIndex,
                        lowSubband,
                        noSubbands,
                        smooth_ratio,
                        noNoiseFlag,
                        &h_sbr_cal_env->phaseIndex);
      }
#endif
    }

    FUNC(5);
    updateFiltBuffer(h_sbr_cal_env->filtBuffer,
                     h_sbr_cal_env->filtBufferNoise,
                     nrgGain,
                     noiseLevel,
                     noSubbands);
  }


  ADD(1); BRANCH(1);
  if(hFrameData->frameInfo.tranEnv == nEnvelopes) {

    MOVE(1);
    h_sbr_cal_env->prevTranEnv = 0;
  }
  else {

    MOVE(1);
    h_sbr_cal_env->prevTranEnv = -1;
  }

  COUNT_sub_end();

}


/*!
  \brief   Create envelope instance

  \return  errorCode, 0 if successful
*/
int
createSbrEnvelopeCalc (unsigned int keyflag,
					   HANDLE_SBR_CALCULATE_ENVELOPE hs,
                       HANDLE_SBR_HEADER_DATA hHeaderData,
                       int chan)
{
  int err = 0;
  int i;

  COUNT_sub_start("createSbrEnvelopeCalc");

  MOVE(1); /* counting previous operations */

  PTR_INIT(1); /* pointer for hs->harmFlagsPrev[]
                */
  LOOP(1);
  for (i=0; i<(MAX_FREQ_COEFFS+15)/16; i++) {

    MOVE(1);
    hs->harmFlagsPrev[i] = 0;
  }

  MOVE(1);
  hs->harmIndex = 0;

  PTR_INIT(2);
  hs->filtBuffer      = getSbrGainSmooth(keyflag,chan)/*sbr_GainSmooth[chan]*/;
  hs->filtBufferNoise = getSbrNoiseSmooth(keyflag,chan)/*sbr_NoiseSmooth[chan]*/;

  MOVE(1);
  hs->prevTranEnv = -1;

  FUNC(1);
  resetSbrEnvelopeCalc(hs);

  BRANCH(1);
  if (chan==0) {

    FUNC(1);
    err = resetFreqBandTables(hHeaderData);
  }

  COUNT_sub_end();

  return err;
}


/*!
  \brief   Reset envelope instance

  \return  errorCode, 0 if successful
*/
void
resetSbrEnvelopeCalc (HANDLE_SBR_CALCULATE_ENVELOPE hCalEnv)
{
  COUNT_sub_start("resetSbrEnvelopeCalc");

  MOVE(2);
  hCalEnv->phaseIndex = 0;
  hCalEnv->startUp = 1;

  COUNT_sub_end();
}


/*!
  \brief  Update buffers for gains and noise levels
*/
static void updateFiltBuffer(float *filtBuffer,
                             float *filtBufferNoise,
                             float *nrgGain,
                             float *noiseLevel,
                             int     noSubbands)
{
  int k;

  COUNT_sub_start("updateFiltBuffer");

  LOOP(1);
  for (k=0; k<noSubbands; k++) {

    MOVE(2);
    *filtBuffer++ = *nrgGain++;
    *filtBufferNoise++ = *noiseLevel++;
  }

  COUNT_sub_end();
}


/*!
  \brief  Estimates the mean energy per subband
*/
static void calcNrgPerSubband(float  **analysBufferReal,
#ifndef LP_SBR_ONLY
                              float  **analysBufferImag,
#endif
                              int    lowSubband,
                              int    highSubband,
                              int    start_pos,
                              int    next_pos,
                              float  *nrgEst,
                              int    bUseLP
                              )
{
  float  temp, accu, invWidth;
  int    k,l;

  COUNT_sub_start("calcNrgPerSubband");

  ADD(1); INDIRECT(1); MOVE(1);
  invWidth = sbr_invIntTable[next_pos - start_pos];

  PTR_INIT(2); /* pointer for analysBufferReal[l][k],
                              analysBufferImag[l][k]
               */
  LOOP(1);
  for (k=lowSubband; k<highSubband; k++) {

    MOVE(1);
    accu = 0.0f;

    LOOP(1);
    for (l=start_pos; l<next_pos; l++) {

      MOVE(1);
      temp = analysBufferReal[l][k];

      MAC(1);
      accu += temp * temp;

#ifndef LP_SBR_ONLY
      BRANCH(1);
      if (!bUseLP) {

        MOVE(1);
        temp = analysBufferImag[l][k];

        MAC(1);
        accu += temp * temp;
      }
#endif
    }

    BRANCH(1);
    if (bUseLP) {

      MULT(1);
      accu *= 2.0;
    }

    MULT(1);
    *nrgEst++ = accu * invWidth;
  }

  COUNT_sub_end();
}


/*!
  \brief   Estimates the mean energy of each Scale factor band.
*/
static void calcNrgPerSfb(float  **analysBufferReal,
#ifndef LP_SBR_ONLY
                          float  **analysBufferImag,
#endif
                          int    nSfb,
                          unsigned char *freqBandTable,
                          int    start_pos,
                          int    next_pos,
                          float  *nrgEst,
                          int   bUseLP
                          )
{
  float  temp, accu, invWidth;
  int    j,k,l,li,ui;

  COUNT_sub_start("calcNrgPerSfb");

  ADD(1); INDIRECT(1); MOVE(1);
  invWidth = sbr_invIntTable[next_pos - start_pos];

  PTR_INIT(3); /* pointer for freqBandTable[],
                              analysBufferReal[][],
                              analysBufferImag[][]
               */
  LOOP(1);
  for(j=0; j<nSfb; j++) {

    MOVE(2);
    li = freqBandTable[j];
    ui = freqBandTable[j+1];

    MOVE(1);
    accu = 0.0f;

    LOOP(1);
    for (k=li; k<ui; k++) {

      LOOP(1);
      for (l=start_pos; l<next_pos; l++) {

        MOVE(1);
        temp   = analysBufferReal[l][k];

        MAC(1);
        accu += temp * temp;
#ifndef LP_SBR_ONLY

        BRANCH(1);
        if (!bUseLP) {

          MOVE(1);
          temp  = analysBufferImag[l][k];

          MAC(1);
          accu += temp * temp;
        }
#endif
      }
    }

    BRANCH(1);
    if (bUseLP) {

      MULT(1);
      accu *= 2.0;
    }

    MULT(1);
    accu *= invWidth;

    INDIRECT(1); MULT(1);
    accu *= sbr_invIntTable[ui-li];

    LOOP(1);
    for (k=li; k<ui; k++)
    {
      MOVE(1);
      *nrgEst++ = accu;
    }
  }

  COUNT_sub_end();
}


/*!
  \brief  Calculate gain, noise, and additional sine level for one subband.
*/
static void calcSubbandGain(float  nrgRef,
                            float  nrgEst,
                            float  tmpNoise,
                            float  *ptrNrgGain,
                            float  *ptrNoiseLevel,
                            float  *ptrNrgSine,
                            char   sinePresentFlag,
                            char   sineMapped,
                            int    noNoiseFlag)
{
  float a,b;
  float c;

  COUNT_sub_start("calcSubbandGain");

  ADD(1);
  nrgEst += 1.0f;

  MULT(1);
  a = nrgRef * tmpNoise;

  ADD(1);
  b = 1.0f + tmpNoise;

  DIV(1); STORE(1);
  *ptrNoiseLevel = a / b;

  BRANCH(1);
  if (sinePresentFlag) {

    MULT(1);
    c = b * nrgEst;

    DIV(1); STORE(1);
    *ptrNrgGain = a / c;

    BRANCH(1);
    if (sineMapped) {
      DIV(1); STORE(1);
      *ptrNrgSine = nrgRef / b;
    }
  }
  else {
    BRANCH(1);
    if (noNoiseFlag) {
      MOVE(1);
      b = nrgEst;
    }
    else {
      MULT(1);
      b = b * nrgEst;
    }

    DIV(1); STORE(1);
    *ptrNrgGain = nrgRef / b;
  }

  COUNT_sub_end();
}


/*!
  \brief  Calculate "average gain" for the specified subband range.
*/
static void calcAvgGain(float  *nrgRef,
                        float  *nrgEst,
                        int    lowSubband,
                        int    highSubband,
                        float  *ptrSumRef,
                        float  *ptrAvgGain)
{
  float sumRef = 1.0;
  float sumEst = 1.0;
  int    k;

  COUNT_sub_start("calcAvgGain");

  MOVE(2); /* counting previous operations */

  PTR_INIT(2); /* pointer for nrgRef[],
                              nrgEst[]
               */
  LOOP(1);
  for (k=lowSubband; k<highSubband; k++){

    ADD(1);
    sumRef += nrgRef[k];

    ADD(1);
    sumEst += nrgEst[k];
  }

  DIV(1); STORE(1);
  *ptrAvgGain = sumRef / sumEst;

  MOVE(1);
  *ptrSumRef = sumRef;

  COUNT_sub_end();
}


/*!
  \brief   Amplify one timeslot of the signal with the calculated gains
           and add the noisefloor.
*/
static void adjustTimeSlotLP(float  *ptrReal,
                             float  *gain,
                             float  *noiseLevel,
                             float  *nrgSine,
                             char   *ptrHarmIndex,
                             int    lowSubband,
                             int    noSubbands,
                             int    noNoiseFlag,
                             int    *ptrPhaseIndex)
{
  int    k;
  float  signalReal;
  float  noiseReal;
  int    index = *ptrPhaseIndex;
  char   harmIndex = *ptrHarmIndex;
  char   freqInvFlag = (lowSubband & 1);
  float  sineLevel;
  float  sineLevelNext;
  float  sineLevelPrev;
  int    tone_count= 0;


  COUNT_sub_start("adjustTimeSlotLP");

  PTR_INIT(2); LOGIC(1); MOVE(1); /* counting previous operations */

  ADD(1); LOGIC(1);
  index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

  INDIRECT(1); MULT(1);
  signalReal = *ptrReal * gain[0];

  INDIRECT(1); MOVE(1);
  sineLevel = nrgSine[0];
  ADD(1); BRANCH(1);
  if (noSubbands == 1) {
    MOVE(1);
    sineLevelNext = 0;
  }
  else {
    INDIRECT(1); MOVE(1);
    sineLevelNext = nrgSine[1];
  }

  BRANCH(1);
  if (sineLevel != 0.0f) {
    ADD(1);
    tone_count++;
  }
  else {
    BRANCH(1); LOGIC(1);
    if (!noNoiseFlag) {
      INDIRECT(2); MULT(1); STORE(1);
      noiseReal = sbr_randomPhase[index][0] * noiseLevel[0];

      ADD(1); MOVE(1);
      signalReal  = signalReal + noiseReal;
    }
  }

  BRANCH(2);
  switch(harmIndex) {
  case 0:
    ADD(1); MOVE(1);
    signalReal = signalReal + sineLevel;
    break;
  case 2:
    ADD(1); MOVE(1);
    signalReal = signalReal - sineLevel;
    break;
  case 1:
    BRANCH(1);
    if (freqInvFlag) {
      MAC(1); STORE(1);
      *(ptrReal-1) = *(ptrReal-1) + 0.00815f * sineLevel;

      MULT(1); ADD(1); MOVE(1);
      signalReal   = signalReal - 0.00815f * sineLevelNext;
    }
    else {
      MAC(1); STORE(1);
      *(ptrReal-1) = *(ptrReal-1) - 0.00815f * sineLevel;

      MULT(1); ADD(1); MOVE(1);
      signalReal   = signalReal + 0.00815f * sineLevelNext;
    }
    break;
  case 3:
    BRANCH(1);
    if (freqInvFlag) {
      MAC(1); STORE(1);
      *(ptrReal-1) = *(ptrReal-1) - 0.00815f * sineLevel;

      MULT(1); ADD(1); MOVE(1);
      signalReal   = signalReal + 0.00815f * sineLevelNext;
    }
    else {
      MAC(1); STORE(1);
      *(ptrReal-1) = *(ptrReal-1) + 0.00815f * sineLevel;

      MULT(1); ADD(1); MOVE(1);
      signalReal   = signalReal - 0.00815f * sineLevelNext;
    }
    break;
  }

  ADD(1); STORE(1);
  *ptrReal++ = signalReal;

  LOGIC(1);
  freqInvFlag = !freqInvFlag;


  PTR_INIT(4); /* pointer for gain[],
                              nrgSine[],
                              sbr_randomPhase[][0],
                              noiseLevel[]          */

  BRANCH(2);
  switch(harmIndex) {
  case 0:

    ADD(1); LOOP(1);
    for (k=1; k<noSubbands-1; k++) {

      ADD(1); LOGIC(1);
      index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

      MULT(1);
      signalReal = *ptrReal * gain[k];

      MOVE(3);
      sineLevelPrev = sineLevel;
      sineLevel =  sineLevelNext;
      sineLevelNext = nrgSine[k+1];

      BRANCH(1); LOGIC(2);
      if (sineLevel == 0.0f && !noNoiseFlag) {
        INDIRECT(2); MULT(1); STORE(1);
        noiseReal = sbr_randomPhase[index][0] * noiseLevel[k];

        ADD(1); MOVE(1);
        signalReal  = signalReal + noiseReal;
      }

      ADD(2); STORE(1);
      *ptrReal++ = signalReal + sineLevel;

      LOGIC(1);
      freqInvFlag = !freqInvFlag;
    }
    break;
  case 1:
    ADD(1); LOOP(1);
    for (k=1; k<noSubbands-1; k++) {

      ADD(1); LOGIC(1);
      index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

      MULT(1);
      signalReal = *ptrReal * gain[k];

      MOVE(3);
      sineLevelPrev = sineLevel;
      sineLevel =  sineLevelNext;
      sineLevelNext = nrgSine[k+1];

      BRANCH(1);
      if (sineLevel != 0.0f) {
        ADD(1);
        tone_count++;
      }
      else {
        BRANCH(1); LOGIC(1);
        if (!noNoiseFlag) {
          INDIRECT(2); MULT(1); STORE(1);
          noiseReal = sbr_randomPhase[index][0] * noiseLevel[k];

          ADD(1); MOVE(1);
          signalReal  = signalReal + noiseReal;
        }
      }

      ADD(1); BRANCH(1);
      if (tone_count <= 16) {
        float addSine;

        ADD(1); MULT(1);
        addSine = (sineLevelPrev - sineLevelNext) * 0.00815f;

        BRANCH(1);
        if (freqInvFlag) {
          ADD(1); MOVE(1);
          signalReal = signalReal + addSine;
        }
        else {
          ADD(1); MOVE(1);
          signalReal = signalReal - addSine;
        }
      }

      ADD(1); STORE(1);
      *ptrReal++ = signalReal;

      LOGIC(1);
      freqInvFlag = !freqInvFlag;
    }
    break;
  case 2:

    ADD(1); LOOP(1);
    for (k=1; k<noSubbands-1; k++) {

      ADD(1); LOGIC(1);
      index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

      MULT(1);
      signalReal = *ptrReal * gain[k];

      MOVE(3);
      sineLevelPrev = sineLevel;
      sineLevel =  sineLevelNext;
      sineLevelNext = nrgSine[k+1];

      BRANCH(1); LOGIC(2);
      if (sineLevel == 0.0f && !noNoiseFlag) {
        INDIRECT(2); MULT(1); STORE(1);
        noiseReal = sbr_randomPhase[index][0] * noiseLevel[k];

        ADD(1); MOVE(1);
        signalReal  = signalReal + noiseReal;
      }

      ADD(2); STORE(1);
      *ptrReal++ = signalReal - sineLevel;

      LOGIC(1);
      freqInvFlag = !freqInvFlag;
    }
    break;
  case 3:
    ADD(1); LOOP(1);
    for (k=1; k<noSubbands-1; k++) {

      ADD(1); LOGIC(1);
      index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

      MULT(1);
      signalReal = *ptrReal * gain[k];

      MOVE(3);
      sineLevelPrev = sineLevel;
      sineLevel =  sineLevelNext;
      sineLevelNext = nrgSine[k+1];

      BRANCH(1);
      if (sineLevel != 0.0f) {
        ADD(1);
        tone_count++;
      }
      else {
        BRANCH(1); LOGIC(1);
        if (!noNoiseFlag) {
          INDIRECT(2); MULT(1); STORE(1);
          noiseReal = sbr_randomPhase[index][0] * noiseLevel[k];

          ADD(1); MOVE(1);
          signalReal  = signalReal + noiseReal;
        }
      }

      ADD(1); BRANCH(1);
      if (tone_count <= 16) {
        float addSine;

        ADD(1); MULT(1);
        addSine = (sineLevelPrev - sineLevelNext) * 0.00815f;

        BRANCH(1);
        if (freqInvFlag) {
          ADD(1); MOVE(1);
          signalReal = signalReal - addSine;
        }
        else {
          ADD(1); MOVE(1);
          signalReal = signalReal + addSine;
        }
      }

      ADD(1); STORE(1);
      *ptrReal++ = signalReal;

      LOGIC(1);
      freqInvFlag = !freqInvFlag;
    }
    break;
  }

  ADD(1); BRANCH(1);
  if (noSubbands > 1) {
    ADD(1); LOGIC(1);
    index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

    MULT(1);
    signalReal = *ptrReal * gain[k];

    MULT(1);
    sineLevelPrev = sineLevel * 0.00815f;

    MOVE(1);
    sineLevel =  sineLevelNext;

    BRANCH(1);
    if (sineLevel != 0.0f) {
      ADD(1);
      tone_count++;
    }
    else {
      BRANCH(1); LOGIC(1);
      if (!noNoiseFlag) {
        INDIRECT(2); MULT(1); STORE(1);
        noiseReal = sbr_randomPhase[index][0] * noiseLevel[k];

        ADD(1); MOVE(1);
        signalReal  = signalReal + noiseReal;
      }
    }

    BRANCH(2);
    switch(harmIndex) {
    case 0:
      ADD(1); STORE(1);
      *ptrReal = signalReal + sineLevel;
      break;
    case 2:
      ADD(1); STORE(1);
      *ptrReal = signalReal - sineLevel;
      break;
    case 1:

      ADD(1); BRANCH(1);
      if(tone_count <= 16) {
        BRANCH(1);
        if (freqInvFlag) {

          ADD(1); STORE(1);
          *ptrReal++   = signalReal + sineLevelPrev;

          ADD(2); BRANCH(1);
          if (k + lowSubband < 62) {
            MAC(1); STORE(1);
            *ptrReal     = *ptrReal   - 0.00815f*sineLevel;
          }
        }
        else {
          ADD(1); STORE(1);
          *ptrReal++   = signalReal - sineLevelPrev;

          ADD(2); BRANCH(1);
          if (k + lowSubband < 62) {
            MAC(1); STORE(1);
            *ptrReal     = *ptrReal   + 0.00815f*sineLevel;
          }
        }
      }
      else {
        MOVE(1);
        *ptrReal = signalReal;
      }
      break;
    case 3:
      ADD(1); BRANCH(1);
      if(tone_count <= 16) {
        BRANCH(1);
        if (freqInvFlag) {

          ADD(1); STORE(1);
          *ptrReal++   = signalReal - sineLevelPrev;

          ADD(2); BRANCH(1);
          if (k + lowSubband < 62) {
            MAC(1); STORE(1);
            *ptrReal     = *ptrReal   + 0.00815f*sineLevel;
          }
        }
        else {

          ADD(1); STORE(1);
          *ptrReal++   = signalReal + sineLevelPrev;

          ADD(2); BRANCH(1);
          if (k + lowSubband < 62) {
            MAC(1); STORE(1);
            *ptrReal     = *ptrReal - 0.00815f*sineLevel;
          }
        }
      }
      else {
        MOVE(1);
        *ptrReal = signalReal;
      }
      break;

    }

  }

  ADD(1); LOGIC(1); STORE(1);
  *ptrHarmIndex  = (harmIndex + 1) & 3;

  MOVE(1);
  *ptrPhaseIndex = index;

  COUNT_sub_end();
}

#ifndef LP_SBR_ONLY
static void adjustTimeSlot(float  *ptrReal,
                           float  *ptrImag,
                           float  *filtBuffer,
                           float  *filtBufferNoise,
                           float  *gain,
                           float  *noiseLevel,
                           float  *nrgSine,
                           char   *ptrHarmIndex,
                           int    lowSubband,
                           int    noSubbands,
                           float  smooth_ratio,
                           int    noNoiseFlag,
                           int    *ptrPhaseIndex)
{
  int    k;
  float  signalReal, signalImag;
  float  noiseReal,  noiseImag;
  float  smoothedGain, smoothedNoise;
  float  direct_ratio = 1.0f - smooth_ratio;
  int    index = *ptrPhaseIndex;
  char   harmIndex = *ptrHarmIndex;
  char   freqInvFlag = (lowSubband & 1);
  float  sineLevel;

  COUNT_sub_start("adjustTimeSlot");

  PTR_INIT(2); ADD(1); LOGIC(1); /* counting previous operations */

  PTR_INIT(5); /* pointers for gain[],
                               nrgSine[],
                               sbr_randomPhase[][0],
                               noiseLevel[],
                               filtBuffer[]          */
  LOOP(1);
  for (k=0; k<noSubbands; k++) {

    BRANCH(1);
    if (smooth_ratio > 0) {

      MULT(1); MAC(1);
      smoothedGain =
        smooth_ratio * filtBuffer[k] +
        direct_ratio * gain[k];

      MULT(1); MAC(1);
      smoothedNoise =
        smooth_ratio * filtBufferNoise[k] +
        direct_ratio * noiseLevel[k];
    }
    else {
      MOVE(2);
      smoothedGain  = gain[k];
      smoothedNoise = noiseLevel[k];
    }

    MULT(2);
    signalReal = *ptrReal * smoothedGain;
    signalImag = *ptrImag * smoothedGain;

    ADD(1); LOGIC(1);
    index = (index + 1) & (SBR_NF_NO_RANDOM_VAL - 1);

    BRANCH(1);
    if (nrgSine[k] != 0.0f) {

      MOVE(1);
      sineLevel = nrgSine[k];

      BRANCH(2);
      switch(harmIndex) {
      case 0:
        ADD(1); STORE(1);
        *ptrReal = signalReal + sineLevel;

        MOVE(1);
        *ptrImag = signalImag;
        break;
      case 2:

        ADD(1); STORE(1);
        *ptrReal = signalReal - sineLevel;

        MOVE(1);
        *ptrImag = signalImag;
        break;
      case 1:

        MOVE(1);
        *ptrReal = signalReal;

        BRANCH(1);
        if (freqInvFlag) {
          ADD(1); STORE(1);
          *ptrImag = signalImag - sineLevel;
        }
        else {
          ADD(1); STORE(1);
          *ptrImag = signalImag + sineLevel;
        }
        break;
      case 3:
        MOVE(1);
        *ptrReal = signalReal;

        BRANCH(1);
        if (freqInvFlag) {
          ADD(1); STORE(1);
          *ptrImag = signalImag + sineLevel;
        }
        else {
          ADD(1); STORE(1);
          *ptrImag = signalImag - sineLevel;
        }
        break;
      }
    }
    else {

      BRANCH(1);
      if (noNoiseFlag) {
        MOVE(2);
        *ptrReal = signalReal;
        *ptrImag = signalImag;
      }
      else {
        MULT(2);
        noiseReal = sbr_randomPhase[index][0] * smoothedNoise;
        noiseImag = sbr_randomPhase[index][1] * smoothedNoise;

        ADD(2); STORE(2);
        *ptrReal  = signalReal + noiseReal;
        *ptrImag  = signalImag + noiseImag;
      }
    }
    LOGIC(1);
    freqInvFlag = !freqInvFlag;

    ADD(2);
    ptrReal++;
    ptrImag++;
  }
  ADD(1); LOGIC(1); STORE(1);
  *ptrHarmIndex = (harmIndex + 1) & 3;

  MOVE(1);
  *ptrPhaseIndex = index;

  COUNT_sub_end();
}
#endif


/*!
  \brief   Reset limiter bands.
*/
int
ResetLimiterBands ( unsigned char *limiterBandTable,
                    unsigned char *noLimiterBands,
                    unsigned char *freqBandTable,
                    int noFreqBands,
                    const PATCH_PARAM *patchParam,
                    int noPatches,
                    int limiterBands)
{
  int i, k, isPatchBorder[2], loLimIndex, hiLimIndex, tempNoLim, nBands;
  unsigned char workLimiterBandTable[MAX_FREQ_COEFFS / 2 + MAX_NUM_PATCHES + 1];
  int patchBorders[MAX_NUM_PATCHES + 1];
  int kx, k2;
  float temp;

  int lowSubband = freqBandTable[0];
  int highSubband = freqBandTable[noFreqBands];

  COUNT_sub_start("ResetLimiterBands");

  INDIRECT(1); MOVE(2); /* counting previous operations */

  BRANCH(1);
  if(limiterBands == 0) {

    MOVE(1);
    limiterBandTable[0] = 0;

    ADD(1); STORE(1);
    limiterBandTable[1] = highSubband - lowSubband;

    MOVE(1);
    nBands = 1;

  } else {

    PTR_INIT(2); /* patchBorders[i]
                    patchParam[i]
                 */
    LOOP(1);
    for (i = 0; i < noPatches; i++) {

      ADD(1); STORE(1);
      patchBorders[i] = patchParam[i].guardStartBand - lowSubband;
    }

    ADD(1); STORE(1);
    patchBorders[i] = highSubband - lowSubband;

    PTR_INIT(2); /* workLimiterBandTable[k]
                    freqBandTable[k]
                 */
    LOOP(1);
    for (k = 0; k <= noFreqBands; k++) {

      ADD(1); STORE(1);
      workLimiterBandTable[k] = freqBandTable[k] - lowSubband;
    }

    PTR_INIT(2); /* workLimiterBandTable[noFreqBands + k]
                    patchBorders[k]
                 */
    LOOP(1);
    for (k = 1; k < noPatches; k++) {

      MOVE(1);
      workLimiterBandTable[noFreqBands + k] = patchBorders[k];
    }

    ADD(2); MOVE(1);
    tempNoLim = nBands = noFreqBands + noPatches - 1;

    ADD(1); FUNC(2);
    shellsort(workLimiterBandTable, tempNoLim + 1);

    MOVE(2);
    loLimIndex = 0;
    hiLimIndex = 1;


    LOOP(1);
    while (hiLimIndex <= tempNoLim) {

      INDIRECT(2); ADD(2);
      k2 = workLimiterBandTable[hiLimIndex] + lowSubband;
      kx = workLimiterBandTable[loLimIndex] + lowSubband;

      FUNC(2);
      temp = FloatFR_getNumOctaves(kx,k2);

      INDIRECT(1); MULT(1);
      temp = temp * sbr_limiterBandsPerOctave[limiterBands];


      ADD(1); BRANCH(1);
      if (temp < 0.49f) {

        ADD(1); BRANCH(1);
        if (workLimiterBandTable[hiLimIndex] == workLimiterBandTable[loLimIndex]) {

          MOVE(1);
          workLimiterBandTable[hiLimIndex] = highSubband;

          ADD(2);
          nBands--;
          hiLimIndex++;

          continue;
        }

        MOVE(2);
        isPatchBorder[0] = isPatchBorder[1] = 0;

        PTR_INIT(1); /* patchBorders[k] */
        INDIRECT(1); /* workLimiterBandTable[hiLimIndex] */
        LOOP(1);
        for (k = 0; k <= noPatches; k++) {

          ADD(1); BRANCH(1);
          if (workLimiterBandTable[hiLimIndex] == patchBorders[k]) {

            MOVE(1);
            isPatchBorder[1] = 1;
            break;
          }
        }

        BRANCH(1);
        if (!isPatchBorder[1]) {

          MOVE(1);
          workLimiterBandTable[hiLimIndex] = highSubband;

          ADD(2);
          nBands--;
          hiLimIndex++;

          continue;
        }

        PTR_INIT(1); /* patchBorders[k] */
        INDIRECT(1); /* workLimiterBandTable[loLimIndex] */
        LOOP(1);
        for (k = 0; k <= noPatches; k++) {

          ADD(1); BRANCH(1);
          if (workLimiterBandTable[loLimIndex] == patchBorders[k]) {

            MOVE(1);
            isPatchBorder[0] = 1;

            break;
          }
        }

        BRANCH(1);
        if (!isPatchBorder[0]) {

          MOVE(1);
          workLimiterBandTable[loLimIndex] = highSubband;

          ADD(1);
          nBands--;
        }
      }

      MOVE(1);
      loLimIndex = hiLimIndex;

      ADD(1);
      hiLimIndex++;

    }

    ADD(1); FUNC(2);
    shellsort(workLimiterBandTable, tempNoLim + 1);

    ADD(1); LOGIC(1); BRANCH(1);
    if( nBands > MAX_NUM_LIMITERS || nBands <= 0) {

      COUNT_sub_end();
      return -1;
    }

    PTR_INIT(2); /* limiterBandTable[k]
                    workLimiterBandTable[k]
                 */
    LOOP(1);
    for (k = 0; k <= nBands; k++) {

      MOVE(1);
      limiterBandTable[k] = workLimiterBandTable[k];
    }
  }

  MOVE(1);
  *noLimiterBands = nBands;

  COUNT_sub_end();

  return 0;
}

