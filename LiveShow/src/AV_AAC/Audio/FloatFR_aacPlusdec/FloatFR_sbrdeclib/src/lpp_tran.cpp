/*
  Low Power Profile Transposer
*/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "lpp_tran.h"
#include "sbr_ram.h"
#include "sbr_rom.h"
#include "FloatFR.h"
#include <stdio.h>

#include "counters.h" /* the 3GPP instrumenting tool */


typedef struct {
  float  r11r;
  float  r01r;
  float  r01i;
  float  r02r;
  float  r02i;
  float  r12r;
  float  r12i;
  float  r22r;
  float  det;
} ACORR_COEFS;



/*
 *
 * \brief Calculate second order autocorrelation using 2 accumulators
 *
 */
static void
autoCorrelation2ndLP(ACORR_COEFS *ac,
                     float *realBuf,
                     int len
                     )
{
  int   j;
  float accu1, accu2;

  COUNT_sub_start("autoCorrelation2ndLP");


  MOVE(1);
  accu1 = 0.0;

  PTR_INIT(1); /* pointer for realBuf[]  */
  ADD(1); LOOP(1);
  for ( j = 0; j < len - 1; j++ ) {

    MAC(1);
    accu1 += realBuf[j-1] * realBuf[j-1];
  }

  MULT(1);
  accu2 = realBuf[-2] * realBuf[-2];

  ADD(1);
  accu2 += accu1;

  MAC(1);
  accu1 += realBuf[j-1] * realBuf[j-1];

  MOVE(2);
  ac->r11r = accu1;
  ac->r22r = accu2;


  MOVE(1);
  accu1 = 0.0;

  PTR_INIT(1); /* pointer for realBuf[] */
  LOOP(1);
  for ( j = 0; j < len - 1; j++ ) {

    MAC(1);
    accu1 += realBuf[j] * realBuf[j-1];
  }

  MULT(1);
  accu2 = realBuf[-1] * realBuf[-2];

  ADD(1);
  accu2 += accu1;

  MAC(1);
  accu1 += realBuf[j] * realBuf[j-1];

  MOVE(2);
  ac->r01r = accu1;
  ac->r12r = accu2;


  MOVE(1);
  accu1=0.0;

  PTR_INIT(1); /* pointer for realBuf[] */
  LOOP(1);
  for ( j = 0; j < len; j++ ) {

    MAC(1);
    accu1 += realBuf[j] * realBuf[j-2];
  }

  MOVE(1);
  ac->r02r = accu1;

  MULT(2); ADD(1); STORE(1);
  ac->det = ac->r11r * ac->r22r - ac->r12r * ac->r12r;

  MOVE(3);
  ac->r01i = ac->r02i = ac->r12i = 0.0f;

  INDIRECT(10); MOVE(10); /* move all register variables to the structure ac->... */

  COUNT_sub_end();
}

#ifndef LP_SBR_ONLY
static void
autoCorrelation2nd(ACORR_COEFS *ac,
                   float *realBuf,
                   float *imagBuf,
                   int len
                   )
{

  int   j;
  float accu1,accu2;

  COUNT_sub_start("autoCorrelation2nd");


  MOVE(1);
  accu1 = 0.0;

  PTR_INIT(2); /* pointer for realBuf[],
                              imagBuf[]  */
  ADD(1); LOOP(1);
  for ( j = 0; j < len - 1; j++ ) {

    MAC(2);
    accu1 += realBuf[j-1] * realBuf[j-1]
           + imagBuf[j-1] * imagBuf[j-1];
  }

  MULT(1); MAC(1);
  accu2 = realBuf[-2] * realBuf[-2]
        + imagBuf[-2] * imagBuf[-2];

  ADD(1);
  accu2 += accu1;

  MAC(2);
  accu1 += realBuf[j-1] * realBuf[j-1]
         + imagBuf[j-1] * imagBuf[j-1];

  MOVE(2);
  ac->r11r = accu1;
  ac->r22r = accu2;


  MOVE(1);
  accu1 = 0.0;

  PTR_INIT(2); /* pointer for realBuf[],
                              imagBuf[]  */
  LOOP(1);
  for ( j = 0; j < len - 1; j++ ) {

    MAC(2);
    accu1 += realBuf[j] * realBuf[j-1]
           + imagBuf[j] * imagBuf[j-1];
  }

  MAC(2);
  accu2 = realBuf[-1] * realBuf[-2]
        + imagBuf[-1] * imagBuf[-2];

  ADD(1);
  accu2 += accu1;

  MAC(2);
  accu1 += realBuf[j] * realBuf[j-1]
         + imagBuf[j] * imagBuf[j-1];

  MOVE(2);
  ac->r01r = accu1;
  ac->r12r = accu2;


  MOVE(1);
  accu1 = 0.0;

  PTR_INIT(2); /* pointer for realBuf[],
                              imagBuf[]  */
  LOOP(1);
  for ( j = 0; j < len - 1; j++ ) {

    MULT(2); MAC(1);
    accu1 += imagBuf[j] * realBuf[j-1]
           - realBuf[j] * imagBuf[j-1];
  }

  MULT(2); ADD(1);
  accu2 = imagBuf[-1] * realBuf[-2]
        - realBuf[-1] * imagBuf[-2];

  ADD(1);
  accu2 += accu1;

  MULT(2); MAC(1);
  accu1 += imagBuf[j] * realBuf[j-1]
         - realBuf[j] * imagBuf[j-1];

  MOVE(2);
  ac->r01i = accu1;
  ac->r12i = accu2;


  MOVE(2);
  accu1=accu2=0.0;

  PTR_INIT(2); /* pointer for realBuf[],
                              imagBuf[]  */
  LOOP(1);
  for ( j = 0; j < len; j++ ){

    MAC(2);
    accu1 += realBuf[j] * realBuf[j-2] + imagBuf[j] * imagBuf[j-2];

    MULT(1); MAC(2);
    accu2 += imagBuf[j] * realBuf[j-2] - realBuf[j] * imagBuf[j-2];
  }

  MOVE(2);
  ac->r02r = accu1;
  ac->r02i = accu2;

  MULT(2); MAC(2); STORE(1);
  ac->det = ac->r11r * ac->r22r - (ac->r12r * ac->r12r + ac->r12i * ac->r12i);

  INDIRECT(10); MOVE(10); /* move all register variables to the structure ac->... */

  COUNT_sub_end();
}
#endif

/*
 *
 * \brief Get bandwidth expansion factor from filtering level
 *
 */
static float
mapInvfMode (INVF_MODE mode,
             INVF_MODE prevMode)
{
  COUNT_sub_start("mapInvfMode");
  ADD(1); BRANCH(2); /* worst case */
  COUNT_sub_end();

  switch (mode) {
  case INVF_LOW_LEVEL:
    if(prevMode == INVF_OFF)
      return 0.6f;
    else
      return 0.75f;
    break;

  case INVF_MID_LEVEL:
    return 0.90f;
    break;

  case INVF_HIGH_LEVEL:
    return 0.98f;
    break;

  default:
    if(prevMode == INVF_LOW_LEVEL)
      return 0.6f;
    else
      return 0.0f;
    break;
  }
}


/*
 *
 * \brief Perform inverse filtering level emphasis
 *
 */
static void
inverseFilteringLevelEmphasis(HANDLE_SBR_LPP_TRANS hLppTrans,
                              unsigned char nInvfBands,
                              INVF_MODE *sbr_invf_mode,
                              INVF_MODE *sbr_invf_mode_prev,
                              float * bwVector
                              )
{
  int i;
  float accu;
  float  w1, w2;

  COUNT_sub_start("inverseFilteringLevelEmphasis");

  PTR_INIT(1); /* pointer for hLppTrans->bwVectorOld[] */
  LOOP(1);
  for(i = 0; i < nInvfBands; i++) {

    INDIRECT(1); FUNC(2); MOVE(1);
    bwVector[i] = mapInvfMode (sbr_invf_mode[i],
                               sbr_invf_mode_prev[i]);

    ADD(1); BRANCH(1);
    if(bwVector[i] < hLppTrans->bwVectorOld[i]) {

      MOVE(2);
      w1 = 0.75f;
      w2 = 0.25f;
    }
    else {

      MOVE(2);
      w1 = 0.90625f;
      w2 = 0.09375f;
    }

    MULT(1); MAC(1);
    accu = w1*bwVector[i] + w2*hLppTrans->bwVectorOld[i];

    ADD(1); BRANCH(1);
    if (accu < 0.015625f) {

      MOVE(1);
      accu=0;
    }

    ADD(1); BRANCH(1); MOVE(1);
    accu = min(accu,0.99609375f);

    MOVE(1);
    bwVector[i] = accu;
  }


  COUNT_sub_end();
}


/*
 * \brief Perform transposition by patching of subband samples.
 */
void lppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,
                    float **qmfBufferReal,
#ifndef LP_SBR_ONLY
                    float **qmfBufferImag,
#endif
                    float *degreeAlias,
                    int timeStep,
                    int firstSlotOffs,
                    int lastSlotOffs,
                    unsigned char nInvfBands,
                    INVF_MODE *sbr_invf_mode,
                    INVF_MODE *sbr_invf_mode_prev,
                    int   bUseLP
                    )
{
  int    bwIndex[MAX_NUM_PATCHES];
  float  bwVector[MAX_NUM_PATCHES];

  int    i,j;
  int    loBand, hiBand;

  PATCH_PARAM *patchParam;

  int    patch;

  float  alphar[LPC_ORDER], a0r, a1r;
  float  alphai[LPC_ORDER], a0i, a1i;

  float  bw;

  int    autoCorrLength;

  float k1, k1_below, k1_below2;

  ACORR_COEFS ac;
  int    startSample;
  int    stopSample;
  int    stopSampleClear;
  int    lb, hb;

  int targetStopBand;

  COUNT_sub_start("lppTransposer");

  INDIRECT(1); PTR_INIT(1);
  patchParam = hLppTrans->pSettings->patchParam;

  MOVE(1);
  bw = 0.0f;

  MOVE(2);
  k1_below=0, k1_below2=0;

  MULT(1);
  startSample = firstSlotOffs * timeStep;

  INDIRECT(1); MULT(1); ADD(1);
  stopSample  = hLppTrans->pSettings->nCols + lastSlotOffs * timeStep;

  FUNC(5);
  inverseFilteringLevelEmphasis(hLppTrans, nInvfBands, sbr_invf_mode, sbr_invf_mode_prev, bwVector);


  MOVE(1);
  stopSampleClear = stopSample;

  PTR_INIT(2); /* pointers for qmfBufferReal[],
                               qmfBufferImag[]  */
  INDIRECT(1); LOOP(1);
  for ( patch = 0; patch < hLppTrans->pSettings->noOfPatches; patch++ ) {
    LOOP(1);
    for (i = startSample; i < stopSampleClear; i++) {
      INDIRECT(1); ADD(1); LOOP(1);
      for(j=patchParam[patch].guardStartBand; j<patchParam[patch].guardStartBand+GUARDBANDS; j++){
        MOVE(1);
        qmfBufferReal[i][j] = 0.0;
#ifndef LP_SBR_ONLY
        BRANCH(1);
        if (!bUseLP) {
          MOVE(1);
          qmfBufferImag[i][j] = 0.0;
        }
#endif
      }
    }
  }

  INDIRECT(4); ADD(1);
  targetStopBand = patchParam[hLppTrans->pSettings->noOfPatches-1].targetStartBand +
    patchParam[hLppTrans->pSettings->noOfPatches-1].numBandsInPatch;

  PTR_INIT(2); /* pointers for qmfBufferReal[],
                               qmfBufferImag[]  */
  LOOP(1);
  for (i = startSample; i < stopSampleClear; i++) {
    LOOP(1);
    for (j=targetStopBand; j<NO_SYNTHESIS_CHANNELS; j++) {
      MOVE(1);
      qmfBufferReal[i][j] = 0.0;
#ifndef LP_SBR_ONLY
      BRANCH(1);
      if (!bUseLP) {
        MOVE(1);
        qmfBufferImag[i][j] = 0.0;
      }
#endif
    }
  }

  INDIRECT(1); ADD(1);
  autoCorrLength = hLppTrans->pSettings->nCols + 6;

  PTR_INIT(1); /* pointer for bwIndex[patch] */
  INDIRECT(1); LOOP(1);
  for ( patch=0; patch<hLppTrans->pSettings->noOfPatches; patch++ ) {
    MOVE(1);
    bwIndex[patch] = 0;
  }



  BRANCH(1);
  if (bUseLP) {

    INDIRECT(1); ADD(1); BRANCH(1); MOVE(1);
    lb = max(1, hLppTrans->pSettings->lbStartPatching - 2);

    INDIRECT(1);
    hb = patchParam[0].targetStartBand;
  }
#ifndef LP_SBR_ONLY
  else {

    INDIRECT(2); MOVE(2);
    lb = hLppTrans->pSettings->lbStartPatching;
    hb = hLppTrans->pSettings->lbStopPatching;
  }
#endif


  PTR_INIT(2); /* pointers for qmfBufferReal[],
                               qmfBufferImag[]  */
  INDIRECT(1); LOOP(1);
  for ( loBand = lb; loBand < hb; loBand++ ) {

    float  lowBandReal[MAX_ENV_COLS+LPC_ORDER];
#ifndef LP_SBR_ONLY
    float  lowBandImag[MAX_ENV_COLS+LPC_ORDER];
#endif
    int lowBandPtr =0;
    int resetLPCCoeffs=0;

    PTR_INIT(4); /* pointers for lowBandReal[],
                                 lowBandImag[],
                                 lpcFilterStatesReal,
                                 lpcFilterStatesImag  */
    LOOP(1);
    for(i=0;i<LPC_ORDER;i++){
      MOVE(1);
      lowBandReal[lowBandPtr] = hLppTrans->lpcFilterStatesReal[i][loBand];

#ifndef LP_SBR_ONLY
      if (!bUseLP) {
        MOVE(1);
        lowBandImag[lowBandPtr] = hLppTrans->lpcFilterStatesImag[i][loBand];
      }
#endif
      lowBandPtr++;
    }

    LOOP(1);
    for(i=0;i< 6;i++){

      MOVE(1);
      lowBandReal[lowBandPtr] = (float) qmfBufferReal[i][loBand];

#ifndef LP_SBR_ONLY
      BRANCH(1);
      if (!bUseLP) {
        MOVE(1);
        lowBandImag[lowBandPtr] = (float) qmfBufferImag[i][loBand];
      }
#endif
      lowBandPtr++;
    }


    INDIRECT(1); ADD(1); LOOP(1);
    for(i=6;i<hLppTrans->pSettings->nCols+6;i++){

        MOVE(1);
        lowBandReal[lowBandPtr] = (float) qmfBufferReal[i][loBand];

#ifndef LP_SBR_ONLY
        BRANCH(1);
        if (!bUseLP) {
          MOVE(1);
          lowBandImag[lowBandPtr] = (float) qmfBufferImag[i][loBand];
        }
#endif
        lowBandPtr++;
    }


    BRANCH(1);
    if (bUseLP) {
      PTR_INIT(1); ADD(1); FUNC(3);
      autoCorrelation2ndLP(&ac,
                           lowBandReal+LPC_ORDER,
                           autoCorrLength);
    }
#ifndef LP_SBR_ONLY
    else {
      PTR_INIT(1); ADD(2); FUNC(3);
      autoCorrelation2nd(&ac,
                         lowBandReal+LPC_ORDER,
                         lowBandImag+LPC_ORDER,
                         autoCorrLength);
    }
#endif

    MOVE(2);
    alphar[1] = 0;
    alphai[1] = 0;

    INDIRECT(1); BRANCH(1);
    if (ac.det != 0.0f) {
      float fac;

      DIV(1);
      fac = 1.0f / ac.det;

      MULT(4); ADD(2); STORE(1);
      alphar[1] = ( ac.r01r * ac.r12r - ac.r01i * ac.r12i - ac.r02r * ac.r11r ) * fac;

#ifndef LP_SBR_ONLY
      BRANCH(1);
      if (!bUseLP) {
        MULT(3); MAC(1); ADD(1); STORE(1);
        alphai[1] = ( ac.r01i * ac.r12r + ac.r01r * ac.r12i - ac.r02i * ac.r11r ) * fac;
      }
#endif
    }

    MOVE(2);
    alphar[0] = 0;
    alphai[0] = 0;


    INDIRECT(1); BRANCH(1);
    if ( ac.r11r != 0.0f ) {
      float fac;

      DIV(1);
      fac = 1.0f / ac.r11r;

      MULT(3); MAC(1); ADD(1); STORE(1);
      alphar[0] = - ( ac.r01r + alphar[1] * ac.r12r + alphai[1] * ac.r12i ) * fac;

#ifndef LP_SBR_ONLY
      BRANCH(1);
      if (!bUseLP) {
        MULT(4); ADD(2); STORE(1);
        alphai[0] = - ( ac.r01i + alphai[1] * ac.r12r - alphar[1] * ac.r12i ) * fac;
      }
#endif

    }
    
    MULT(1); MAC(1); ADD(1); BRANCH(1);
    if(alphar[0]*alphar[0] + alphai[0]*alphai[0] >= 16.0f) {
      MOVE(1);
      resetLPCCoeffs=1;
    }
    MULT(1); MAC(1); ADD(1); BRANCH(1);
    if(alphar[1]*alphar[1] + alphai[1]*alphai[1] >= 16.0f) {
      MOVE(1);
      resetLPCCoeffs=1;
    }

    BRANCH(1);
    if(resetLPCCoeffs){
      MOVE(4);
      alphar[0] = alphar[1] = 0;
      alphai[0] = alphai[1] = 0;
    }

    BRANCH(1);
    if (bUseLP) {

      INDIRECT(1); BRANCH(1);
      if(ac.r11r==0.0f) {
        MOVE(1);
        k1 = 0.0f;
      }
      else {
        INDIRECT(2); DIV(1); MULT(1);
        k1 = -(ac.r01r/ac.r11r);

        ADD(1); BRANCH(1); MOVE(1);
        k1 = min(k1, 1.0f);
        ADD(1); BRANCH(1); MOVE(1);
        k1 = max(k1,-1.0f);
      }

      ADD(1); BRANCH(1);
      if(loBand > 1){
        float deg;

        MULT(1); ADD(1);
        deg = 1.0f - (k1_below * k1_below);

        MOVE(1);
        degreeAlias[loBand] = 0;

        PTR_INIT(1); /* pointer for degreeAlias[] */

        LOGIC(2); BRANCH(1);
        if (((loBand & 1) == 0) && (k1 < 0)){

          BRANCH(1);
          if (k1_below < 0) {

            MOVE(1);
            degreeAlias[loBand] = 1.0f;

            BRANCH(1);
            if ( k1_below2 > 0 ) {

              MOVE(1);
              degreeAlias[loBand-1] = deg;
            }
          }
          else {
            BRANCH(1);
            if ( k1_below2 > 0 ) {
              MOVE(1);
              degreeAlias[loBand] = deg;
            }
          }
        }

        LOGIC(2); BRANCH(1);
        if (((loBand & 1) == 1) && (k1 > 0)){

          BRANCH(1);
          if (k1_below > 0) {

            MOVE(1);
            degreeAlias[loBand] = 1.0f;

            BRANCH(1);
            if ( k1_below2 < 0 ) {

              MOVE(1);
              degreeAlias[loBand-1] = deg;
            }
          }
          else {
            BRANCH(1);
            if ( k1_below2 < 0 ) {
              MOVE(1);
              degreeAlias[loBand] = deg;
            }
          }
        }
      }
      MOVE(2);
      k1_below2 = k1_below;
      k1_below = k1;
    }

    MOVE(1);
    patch = 0;

    PTR_INIT(1); /* pointer for patchParam[patch] */
    INDIRECT(1); LOOP(1);
    while ( patch < hLppTrans->pSettings->noOfPatches ) {

      ADD(1);
      hiBand = loBand + patchParam[patch].targetBandOffs;

      ADD(2); LOGIC(1); BRANCH(1);
      if ( loBand < patchParam[patch].sourceStartBand || loBand >= patchParam[patch].sourceStopBand ) {
        ADD(1);
        patch++;
        continue;
      }
      assert( hiBand < NO_SYNTHESIS_CHANNELS );

      LOOP(1);
      while (hiBand >= hLppTrans->pSettings->bwBorders[bwIndex[patch]]) {
        INDIRECT(1); /* while() condition */

        ADD(1); STORE(1);
        bwIndex[patch]++;
      }

      INDIRECT(1);
      bw = bwVector[bwIndex[patch]];

      INDIRECT(4); MULT(5);
      a0r = bw * alphar[0];
      a0i = bw * alphai[0];
      bw =  bw*bw;
      a1r = bw * alphar[1];
      a1i = bw * alphai[1];


      PTR_INIT(4); /* pointers for lowBandReal[],
                                   lowBandImag[],
                                   qmfBufferReal[],
                                   qmfBufferImag[]  */
      LOOP(1);
      for(i = startSample; i < stopSample; i++ ) {

        MOVE(1);
        qmfBufferReal[i][hiBand] = lowBandReal[LPC_ORDER+i];

        BRANCH(1);
        if (bUseLP) {

          BRANCH(1);
          if ( bw > 0 ) {

            MAC(2); STORE(1);
            qmfBufferReal[i][hiBand] = qmfBufferReal[i][hiBand] +
              a0r * lowBandReal[LPC_ORDER+i-1] +
              a1r * lowBandReal[LPC_ORDER+i-2];
          }
        }
#ifndef LP_SBR_ONLY
        else {

          MOVE(1);
          qmfBufferImag[i][hiBand] = lowBandImag[LPC_ORDER+i];

          BRANCH(1);
          if ( bw > 0 ) {
            float accu;

            MULT(4); ADD(3);
            accu = a0r * lowBandReal[LPC_ORDER+i-1] -  a0i * lowBandImag[LPC_ORDER+i-1]+
              a1r * lowBandReal[LPC_ORDER+i-2] -  a1i * lowBandImag[LPC_ORDER+i-2];

            ADD(1); STORE(1);
            qmfBufferReal[i][hiBand] = qmfBufferReal[i][hiBand] + accu;

            MAC(4);
            accu = a0i * lowBandReal[LPC_ORDER+i-1] +  a0r * lowBandImag[LPC_ORDER+i-1]+
              a1i * lowBandReal[LPC_ORDER+i-2] +  a1r * lowBandImag[LPC_ORDER+i-2];

            ADD(1); STORE(1);
            qmfBufferImag[i][hiBand] = qmfBufferImag[i][hiBand] + accu;
          }
        }
#endif

      }


      patch++;

    }  /* Patch */

  }  /* loBand (band) */


  PTR_INIT(4); /* pointers for lpcFilterStatesReal[][loBand],
                               lpcFilterStatesImag[][loBand],
                               qmfBufferReal[],
                               qmfBufferImag[]  */
  LOOP(1);
  for(i=0;i<LPC_ORDER;i++){
    INDIRECT(1); LOOP(1);
    for (loBand=0; loBand<patchParam[0].targetStartBand; loBand++) {
      MOVE(1);
      hLppTrans->lpcFilterStatesReal[i][loBand] = qmfBufferReal[hLppTrans->pSettings->nCols-LPC_ORDER+i][loBand];
#ifndef LP_SBR_ONLY
      BRANCH(1);
      if (!bUseLP) {
        MOVE(1);
        hLppTrans->lpcFilterStatesImag[i][loBand] = qmfBufferImag[hLppTrans->pSettings->nCols-LPC_ORDER+i][loBand];
      }
#endif
    }
  }

  BRANCH(1);
  if (bUseLP) {

    PTR_INIT(2); /* pointers for degreeAlias[loBand],
                                 degreeAlias[hiBand]  */
    INDIRECT(2); LOOP(1);
    for ( loBand = hLppTrans->pSettings->lbStartPatching; loBand <  hLppTrans->pSettings->lbStopPatching; loBand++ ) {

      MOVE(1);
      patch = 0;

      INDIRECT(1); LOOP(1);
      while ( patch < hLppTrans->pSettings->noOfPatches ) {

        INDIRECT(1); ADD(1);
        hiBand = loBand + patchParam[patch].targetBandOffs;

        LOGIC(2); ADD(3); BRANCH(1);
        if ( loBand < patchParam[patch].sourceStartBand
             || loBand >= patchParam[patch].sourceStopBand
             || hiBand >= NO_SYNTHESIS_CHANNELS
             ) {
          ADD(1);
          patch++;
          continue;
        }

        INDIRECT(1); ADD(1); BRANCH(1);
        if(hiBand != patchParam[patch].targetStartBand) {
          MOVE(1);
          degreeAlias[hiBand] = degreeAlias[loBand];
        }
        else {
          MOVE(1);
          degreeAlias[hiBand] = 0;
        }

        patch++;
      }
    }/* end  for loop */
  }

  PTR_INIT(2); /* pointers for bwVectorOld[],
                               bwVector[]     */
  LOOP(1);
  for (i = 0; i < nInvfBands; i++ ) {
    MOVE(1);
    hLppTrans->bwVectorOld[i] = bwVector[i];
  }

  COUNT_sub_end();
}

/*
 *
 * \brief Initialize one low power transposer instance
 *
 *
 */
int
createLppTransposer (unsigned int keyflag,
					 HANDLE_SBR_LPP_TRANS hLppTrans,
                     unsigned char highBandStartSb,
                     unsigned char *v_k_master,
                     unsigned char numMaster,
                     unsigned char usb,
                     unsigned char nCols,
                     unsigned char* noiseBandTable,
                     unsigned char noNoiseBands,
                     unsigned short fs,
                     unsigned char  chan
                     )
{
  HANDLE_SBR_LPP_TRANS hs;

  COUNT_sub_start("createLppTransposer");

  PTR_INIT(1);
  hs = hLppTrans;

  PTR_INIT(1);
  hs->pSettings = getSbrTransposerSettings(keyflag)/*&sbr_TransposerSettings*/;

  MOVE(1);
  hs->pSettings->nCols = nCols;

  PTR_INIT(2); /* pointer for sbr_LpcFilterStatesReal[chan][0],
                              sbr_LpcFilterStatesReal[chan][1] */
  MOVE(2);
  hs->lpcFilterStatesReal[0] = getSbrLpcFilterStatesReal(keyflag,chan,0)/*sbr_LpcFilterStatesReal[chan][0]*/;
  hs->lpcFilterStatesReal[1] = getSbrLpcFilterStatesReal(keyflag,chan,1)/*sbr_LpcFilterStatesReal[chan][1]*/;

#ifndef LP_SBR_ONLY
  PTR_INIT(2); /* pointer for sbr_LpcFilterStatesImag[chan][0],
                              sbr_LpcFilterStatesImag[chan][1] */
  MOVE(2);
  hs->lpcFilterStatesImag[0] = getSbrLpcFilterStatesImag(keyflag,chan,0)/*sbr_LpcFilterStatesImag[chan][0]*/;
  hs->lpcFilterStatesImag[1] = getSbrLpcFilterStatesImag(keyflag,chan,1)/*sbr_LpcFilterStatesImag[chan][1]*/;
#endif

  BRANCH(1);
  if (chan==0) {
    MOVE(1);
    hs->pSettings->nCols = nCols;

    FUNC(9);
    COUNT_sub_end();
    return resetLppTransposer (hs,
                               0,
                               highBandStartSb,
                               v_k_master,
                               numMaster,
                               noiseBandTable,
                               noNoiseBands,
                               usb,
                               fs);
  }
  COUNT_sub_end();
  return 0;
}


static int findClosestEntry(unsigned char goalSb, unsigned char *v_k_master, unsigned char numMaster, unsigned char direction)
{
  int index;

  COUNT_sub_start("findClosestEntry");

  INDIRECT(1); BRANCH(1);
  if( goalSb <= v_k_master[0] ) {
    COUNT_sub_end();
    return v_k_master[0];
  }

  INDIRECT(1); BRANCH(1);
  if( goalSb >= v_k_master[numMaster] ) {
    COUNT_sub_end();
    return v_k_master[numMaster];
  }

  BRANCH(1);
  if(direction) {

    MOVE(1);
    index = 0;

    PTR_INIT(1); /* pointer for v_k_master[index] */
    ADD(1); LOOP(1);
    while( v_k_master[index] < goalSb ) {
      ADD(1);
      index++;
    }
  } else {

    MOVE(1);
    index = numMaster;

    PTR_INIT(1); /* pointer for v_k_master[index] */
    ADD(1); LOOP(1);
    while( v_k_master[index] > goalSb ) {
      ADD(1);
      index--;
    }
  }

  COUNT_sub_end();
  return v_k_master[index];
}


/*
 *
 * \brief Reset memory for one lpp transposer instance
 *
 *
 */
int
resetLppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,
                    unsigned char xposctrl,
                    unsigned char highBandStartSb,
                    unsigned char *v_k_master,
                    unsigned char numMaster,
                    unsigned char* noiseBandTable,
                    unsigned char noNoiseBands,
                    unsigned char usb,
                    unsigned short fs
                    )
{
  int    i, patch;
  int    targetStopBand;

  TRANSPOSER_SETTINGS *pSettings;
  PATCH_PARAM  *patchParam;

  int sourceStartBand;
  int patchDistance;
  int numBandsInPatch;

  int lsb;

  int xoverOffset;

  int goalSb;

  COUNT_sub_start("resetLppTransposer");

  PTR_INIT(2);
  pSettings = hLppTrans->pSettings;
  patchParam = pSettings->patchParam;

  MOVE(1);
  lsb = v_k_master[0];

  ADD(1);
  xoverOffset = highBandStartSb - lsb;


  ADD(2); BRANCH(1);
  if ( lsb - SHIFT_START_SB < 4 ) {
    COUNT_sub_end();
    return (1);
  }



  ADD(1); BRANCH(1);
  if (xposctrl == 1) {

    ADD(1);
    lsb += xoverOffset;
    MOVE(1);
    xoverOffset = 0;
  }

  BRANCH(2); MOVE(1); /* worst case */
  switch(fs){
  case 16000:
  case 22050:
  case 24000:
  case 32000:
    goalSb=64;
    break;
  case 44100:
    goalSb=46;
    break;
  case 48000:
    goalSb=43;
    break;
  default:
    COUNT_sub_end();
    return(0);
  }

  FUNC(4);
  goalSb = findClosestEntry(goalSb, v_k_master, numMaster, 1);

  ADD(2); FUNC(1);
  if ( abs(goalSb - usb) < 4 ) {
    MOVE(1);
    goalSb = usb;
  }


  ADD(1);
  sourceStartBand = SHIFT_START_SB + xoverOffset;
  ADD(1);
  targetStopBand = lsb + xoverOffset;

  MOVE(1);
  patch = 0;

  PTR_INIT(1); /* pointer for patchParam[patch] */
  LOOP(1);
  while(targetStopBand < usb) {

    ADD(1); BRANCH(1);
    if (patch > MAX_NUM_PATCHES) {
      COUNT_sub_end();
      return -1;
    }

    MOVE(1);
    patchParam[patch].guardStartBand = targetStopBand;

    ADD(1);
    targetStopBand += GUARDBANDS;

    MOVE(1);
    patchParam[patch].targetStartBand = targetStopBand;

    ADD(1);
    numBandsInPatch = goalSb - targetStopBand;

    ADD(2); BRANCH(1);
    if ( numBandsInPatch >= lsb - sourceStartBand ) {
      ADD(1);
      patchDistance   = targetStopBand - sourceStartBand;

      LOGIC(1);
      patchDistance   = patchDistance & ~1;

      ADD(2);
      numBandsInPatch = lsb - (targetStopBand - patchDistance);

      ADD(2); FUNC(4);
      numBandsInPatch = findClosestEntry(targetStopBand + numBandsInPatch, v_k_master, numMaster, 0) -
                        targetStopBand;
    }


    ADD(2);
    patchDistance   = numBandsInPatch + targetStopBand - lsb;

    ADD(1); LOGIC(1);
    patchDistance   = (patchDistance + 1) & ~1;

    BRANCH(1);
    if (numBandsInPatch > 0) {

      ADD(1); STORE(1);
      patchParam[patch].sourceStartBand = targetStopBand - patchDistance;

      MOVE(2);
      patchParam[patch].targetBandOffs  = patchDistance;
      patchParam[patch].numBandsInPatch = numBandsInPatch;

      ADD(1); STORE(1);
      patchParam[patch].sourceStopBand  = patchParam[patch].sourceStartBand + numBandsInPatch;

      ADD(1);
      targetStopBand += patchParam[patch].numBandsInPatch;

      patch++;
    }

    MOVE(1);
    sourceStartBand = SHIFT_START_SB;


    FUNC(1); ADD(1); BRANCH(1);
    if( abs(targetStopBand - goalSb) < 3) {
      MOVE(1);
      goalSb = usb;
    }

  }

  ADD(1);
  patch--;

  ADD(1); LOGIC(1); BRANCH(1);
  if ( (patch>0) && (patchParam[patch].numBandsInPatch < 3) ) {
    ADD(1);
    patch--;

    ADD(1);
    targetStopBand = patchParam[patch].targetStartBand + patchParam[patch].numBandsInPatch;
  }

  ADD(1); BRANCH(1);
  if (patch >= MAX_NUM_PATCHES) {
    COUNT_sub_end();
    return -1;
  }

  ADD(1); STORE(1);
  pSettings->noOfPatches = patch + 1;

  MOVE(1);
  pSettings->lbStartPatching = targetStopBand;

  MOVE(1);
  pSettings->lbStopPatching  = 0;

  PTR_INIT(1); /* pointer for patchParam[patch] */
  LOOP(1);
  for ( patch = 0; patch < pSettings->noOfPatches; patch++ ) {

    INDIRECT(1); ADD(1); BRANCH(1); MOVE(1);
    pSettings->lbStartPatching = min( pSettings->lbStartPatching, patchParam[patch].sourceStartBand );

    INDIRECT(1); ADD(1); BRANCH(1); MOVE(1);
    pSettings->lbStopPatching  = max( pSettings->lbStopPatching, patchParam[patch].sourceStopBand );
  }

  PTR_INIT(2); /* pointer for bwBorders[i],
                              noiseBandTable[i+1]  */
  LOOP(1);
  for(i = 0 ; i < noNoiseBands; i++){
    MOVE(1);
    hLppTrans->pSettings->bwBorders[i] = noiseBandTable[i+1];
  }


  COUNT_sub_end();
  return 0;
}


