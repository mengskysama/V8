/*
  Hybrid Filter Bank
*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "hybrid.h"
#include "cfftn.h"
#include "sbr_rom.h"

#include "counters.h" /* the 3GPP instrumenting tool */


static void twoChannelFiltering( const float *pQmf,
                                 float *mHybrid )
{
  int n;
  float cum0, cum1;

  COUNT_sub_start("twoChannelFiltering");

  LOOP(1); PTR_INIT(2);
  MULT(1);
  cum0 = 0.5f * pQmf[HYBRID_FILTER_DELAY];

  MOVE(1);
  cum1 = 0;

  LOOP(1); PTR_INIT(2);
  for(n = 0; n < 6; n++) {

    MAC(1); MULT(1);
    cum1 += p2_6[n] * pQmf[2*n+1];
  }

  ADD(1); STORE(1);
  mHybrid[0] = cum0 + cum1;
  ADD(1); STORE(1);
  mHybrid[1] = cum0 - cum1;

  COUNT_sub_end();
}


static void eightChannelFiltering( const float *pQmfReal,
                                   const float *pQmfImag,
                                   float *mHybridReal,
                                   float *mHybridImag )
{
  int n;
  float real, imag;
  float cum[16];

  COUNT_sub_start("eightChannelFiltering");


  PTR_INIT(3);

  MULT(1); MAC(1);
  real = p8_13[4]  * pQmfReal[4] +
         p8_13[12] * pQmfReal[12];

  MULT(1); MAC(1);
  imag = p8_13[4]  * pQmfImag[4] +
         p8_13[12] * pQmfImag[12];

  ADD(1); MULT(1); STORE(1);
  cum[4] =  (imag - real) * 0.70710678118655f;

  ADD(1); MULT(1); STORE(1);
  cum[5] = -(imag + real) * 0.70710678118655f;

  MULT(1); MAC(1);
  real = p8_13[3]  * pQmfReal[3] +
         p8_13[11] * pQmfReal[11];

  MULT(1); MAC(1);
  imag = p8_13[3]  * pQmfImag[3] +
         p8_13[11] * pQmfImag[11];

  MULT(1); MAC(1); STORE(1);
  cum[6] =   imag * 0.92387953251129f - real * 0.38268343236509f;

  MULT(1); MAC(1); STORE(1);
  cum[7] = -(imag * 0.38268343236509f + real * 0.92387953251129f);

  MULT(1); MAC(1); ADD(1); STORE(1);
  cum[9] = -( p8_13[2]  * pQmfReal[2] +
              p8_13[10] * pQmfReal[10] );

  MULT(1); MAC(1); STORE(1);
  cum[8] =    p8_13[2]  * pQmfImag[2] +
              p8_13[10] * pQmfImag[10];

  MULT(1); MAC(1);
  real = p8_13[1]  * pQmfReal[1] +
         p8_13[9] * pQmfReal[9];

  MULT(1); MAC(1);
  imag = p8_13[1]  * pQmfImag[1] +
         p8_13[9] * pQmfImag[9];

  MULT(1); MAC(1); STORE(1);
  cum[10] = imag * 0.92387953251129f + real * 0.38268343236509f;

  MULT(1); MAC(1); STORE(1);
  cum[11] = imag * 0.38268343236509f - real * 0.92387953251129f;

  MULT(1); MAC(1);
  real = p8_13[0]  * pQmfReal[0] +
         p8_13[8] * pQmfReal[8];

  MULT(1); MAC(1);
  imag = p8_13[0]  * pQmfImag[0] +
         p8_13[8] * pQmfImag[8];

  ADD(1); MULT(1); STORE(1);
  cum[12] = (imag + real) * 0.70710678118655f;

  ADD(1); MULT(1); STORE(1);
  cum[13] = (imag - real) * 0.70710678118655f;

  MULT(2);
  real = p8_13[7]  * pQmfReal[7];
  imag = p8_13[7]  * pQmfImag[7];

  MULT(1); MAC(1); STORE(1);
  cum[14] = imag * 0.38268343236509f + real * 0.92387953251129f;

  MULT(1); MAC(1); STORE(1);
  cum[15] = imag * 0.92387953251129f - real * 0.38268343236509f;

  MULT(2); STORE(2);
  cum[0] = p8_13[HYBRID_FILTER_DELAY]  * pQmfReal[HYBRID_FILTER_DELAY];
  cum[1] = p8_13[HYBRID_FILTER_DELAY]  * pQmfImag[HYBRID_FILTER_DELAY];

  MULT(2);
  real = p8_13[5]  * pQmfReal[5];
  imag = p8_13[5]  * pQmfImag[5];

  MULT(1); MAC(1); STORE(1);
  cum[2] = real * 0.92387953251129f - imag * 0.38268343236509f;

  MULT(1); MAC(1); STORE(1);
  cum[3] = real * 0.38268343236509f + imag * 0.92387953251129f;

  FUNC(3);
  CFFTN(cum, 8, 1);

  LOOP(1); PTR_INIT(3);
  for(n = 0; n < 8; n++) {

    MOVE(2);
    mHybridReal[n] = cum[2*n];
    mHybridImag[n] = cum[2*n+1];
  }

  COUNT_sub_end();
}

/**************************************************************************/
/*
  \brief HybridAnalysis

  \return none.

*/
/**************************************************************************/
void
HybridAnalysis ( const float **mQmfReal,
                 const float **mQmfImag,
                 float **mHybridReal,
                 float **mHybridImag,
                 HANDLE_HYBRID hHybrid )

{
  int band, oddQmf;
  HYBRID_RES hybridRes;
  int  chOffset = 0;

  COUNT_sub_start("HybridAnalysis");

  INDIRECT(1); MOVE(1);
  MOVE(1);

  INDIRECT(1); LOOP(1);
  for(band = 0; band < hHybrid->nQmfBands; band++) {

    LOGIC(1);
    oddQmf = (band & 1);

    INDIRECT(1); MOVE(1);
    hybridRes = (HYBRID_RES)hHybrid->pResolution[band];

    INDIRECT(1); MULT(1);
    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(2); MOVE(1); STORE(hHybrid->qmfBufferMove - 1);
    memcpy(hHybrid->pWorkReal, hHybrid->mQmfBufferReal[band],
           hHybrid->qmfBufferMove * sizeof(float));

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(2); MOVE(1); STORE(hHybrid->qmfBufferMove - 1);
    memcpy(hHybrid->pWorkImag, hHybrid->mQmfBufferImag[band],
           hHybrid->qmfBufferMove * sizeof(float));

    PTR_INIT(4);MOVE(2);
    hHybrid->pWorkReal [hHybrid->qmfBufferMove] = mQmfReal [HYBRID_FILTER_DELAY] [band];
    hHybrid->pWorkImag [hHybrid->qmfBufferMove] = mQmfImag [HYBRID_FILTER_DELAY] [band];

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(2); MOVE(1); STORE(hHybrid->qmfBufferMove - 1);
    memcpy(hHybrid->mQmfBufferReal[band], hHybrid->pWorkReal + 1,
           hHybrid->qmfBufferMove * sizeof(float));

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(2); MOVE(1); STORE(hHybrid->qmfBufferMove - 1);
    memcpy(hHybrid->mQmfBufferImag[band], hHybrid->pWorkImag + 1,
           hHybrid->qmfBufferMove * sizeof(float));

    switch(hybridRes) {
    case HYBRID_2_REAL:
      ADD(1); BRANCH(1);

      FUNC(2);
      twoChannelFiltering( hHybrid->pWorkReal,
                           hHybrid->mTempReal );

      FUNC(2);
      twoChannelFiltering( hHybrid->pWorkImag,
                           hHybrid->mTempImag );

      PTR_INIT(4);
      MOVE(4);
      mHybridReal [0] [chOffset] = hHybrid->mTempReal [0];
      mHybridImag [0] [chOffset] = hHybrid->mTempImag [0];
      mHybridReal [0] [chOffset + 1] = hHybrid->mTempReal [1];
      mHybridImag [0] [chOffset + 1] = hHybrid->mTempImag [1];

      ADD(1);
      chOffset += 2;

      break;
    case HYBRID_8_CPLX:
      ADD(1); BRANCH(1);

      FUNC(4);
      eightChannelFiltering( hHybrid->pWorkReal,
                             hHybrid->pWorkImag,
                             hHybrid->mTempReal,
                             hHybrid->mTempImag );

      PTR_INIT(4);
      MOVE(12); ADD(4);
      mHybridReal [0] [chOffset + 0] = hHybrid->mTempReal [0];
      mHybridImag [0] [chOffset + 0] = hHybrid->mTempImag [0];
      mHybridReal [0] [chOffset + 1] = hHybrid->mTempReal [1];
      mHybridImag [0] [chOffset + 1] = hHybrid->mTempImag [1];
      mHybridReal [0] [chOffset + 2] = hHybrid->mTempReal [2] + hHybrid->mTempReal [5];
      mHybridImag [0] [chOffset + 2] = hHybrid->mTempImag [2] + hHybrid->mTempImag [5];
      mHybridReal [0] [chOffset + 3] = hHybrid->mTempReal [3] + hHybrid->mTempReal [4];
      mHybridImag [0] [chOffset + 3] = hHybrid->mTempImag [3] + hHybrid->mTempImag [4];
      mHybridReal [0] [chOffset + 4] = hHybrid->mTempReal [6];
      mHybridImag [0] [chOffset + 4] = hHybrid->mTempImag [6];
      mHybridReal [0] [chOffset + 5] = hHybrid->mTempReal [7];
      mHybridImag [0] [chOffset + 5] = hHybrid->mTempImag [7];

      ADD(1);
      chOffset += 6;

      break;
    default:
      assert(0);
    }
  }

  COUNT_sub_end();
}

/**************************************************************************/
/*
  \brief

  \return none.

*/
/**************************************************************************/
void
HybridSynthesis ( const float **mHybridReal,
                  const float **mHybridImag,
                  float **mQmfReal,
                  float **mQmfImag,
                  HANDLE_HYBRID hHybrid )
{
  int  k, band;
  HYBRID_RES hybridRes;
  int  chOffset = 0;

  COUNT_sub_start("HybridSynthesis");
  INDIRECT(1); MOVE(1);
  MOVE(1);

  INDIRECT(1); LOOP(1);
  for(band = 0; band < hHybrid->nQmfBands; band++) {

    INDIRECT(1); MOVE(1);
    hybridRes = (HYBRID_RES)min(hHybrid->pResolution[band],6);

    MOVE(2);
    mQmfReal [0] [band] = mQmfImag [0] [band] = 0;

    PTR_INIT(2); LOOP(1);
    for(k = 0; k < hybridRes; k++) {

      ADD(2);
      mQmfReal [0] [band] += mHybridReal [0] [chOffset + k];
      mQmfImag [0] [band] += mHybridImag [0] [chOffset + k];
    }
    MOVE(2);

    ADD(1);
    chOffset += hybridRes;

  }

  COUNT_sub_end();
}
/**************************************************************************/
/*
  \brief    CreateHybridFilterBank


  \return   errorCode, noError if successful.

*/
/**************************************************************************/
int
CreateHybridFilterBank ( HANDLE_HYBRID *phHybrid,
                         int noBands,
                         const int *pResolution,
                         float **pPtr)
{
  int i;
  int maxNoChannels = 0;
  HANDLE_HYBRID hs;
  float *ptr = *pPtr;

  COUNT_sub_start("CreateHybridFilterBank");

  MOVE(1); PTR_INIT(1); /* counting previous operations */

  PTR_INIT(1);
  *phHybrid = NULL;

  PTR_INIT(1); ADD(1);
  hs = (HANDLE_HYBRID)ptr;ptr+=sizeof(HYBRID)/sizeof(float);

  INDIRECT(1); PTR_INIT(1); MULT(1); ADD(1);
  hs->pResolution = (int*)ptr;ptr+=noBands * sizeof (int)/sizeof(float);

  PTR_INIT(2); /* hs->pResolution[i]
                  pResolution[i]
               */
  LOOP(1);
  for (i = 0; i < noBands; i++) {

    MOVE(1);
    hs->pResolution[i] = pResolution[i];

    ADD(3); LOGIC(2); BRANCH(1);
    if( pResolution[i] != HYBRID_8_CPLX &&
        pResolution[i] != HYBRID_2_REAL &&
        pResolution[i] != HYBRID_4_CPLX )
    {
      COUNT_sub_end();
      return 1;
    }

    ADD(1); BRANCH(1);
    if(pResolution[i] > maxNoChannels)
    {
      MOVE(1);
      maxNoChannels = pResolution[i];
    }
  }

  INDIRECT(2); MOVE(2);
  hs->nQmfBands     = noBands;
  hs->qmfBufferMove = HYBRID_FILTER_LENGTH - 1;

  INDIRECT(3); PTR_INIT(2); ADD(4);
  hs->pWorkReal = ptr;ptr+= 1 + hs->qmfBufferMove;
  hs->pWorkImag = ptr;ptr+= 1 + hs->qmfBufferMove;

  INDIRECT(2); PTR_INIT(2); MULT(2); ADD(2);
  hs->mQmfBufferReal = (float **)ptr;ptr+= noBands * sizeof (float *)/sizeof(float);
  hs->mQmfBufferImag = (float **)ptr;ptr+= noBands * sizeof (float *)/sizeof(float);

  PTR_INIT(2); /* hs->mQmfBufferReal[i]
                  hs->mQmfBufferImag[i]
               */
  INDIRECT(1);
  LOOP(1);
  for (i = 0; i < noBands; i++) {

    PTR_INIT(1); ADD(1);
    hs->mQmfBufferReal[i] = ptr;ptr+= hs->qmfBufferMove;

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(hs->qmfBufferMove);
    memset(hs->mQmfBufferReal[i],0,hs->qmfBufferMove*sizeof(float));

    PTR_INIT(1); ADD(1);
    hs->mQmfBufferImag[i] = ptr;ptr+= hs->qmfBufferMove;

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(hs->qmfBufferMove);
    memset(hs->mQmfBufferImag[i],0,hs->qmfBufferMove*sizeof(float));
  }

  PTR_INIT(1); ADD(1);
  hs->mTempReal = ptr;ptr+= maxNoChannels;

  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(maxNoChannels);
  memset(hs->mTempReal,0,maxNoChannels*sizeof(float));

  PTR_INIT(1); ADD(1);
  hs->mTempImag = ptr;ptr+= maxNoChannels;

  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(maxNoChannels);
  memset(hs->mTempImag,0,maxNoChannels*sizeof(float));

  PTR_INIT(2);
  *phHybrid = hs;
  *pPtr = ptr;

  COUNT_sub_end();

  return 0;
}

#if 0

/**************************************************************************/
/*
  \brief

  \return   none

*/
/**************************************************************************/
void
DeleteHybridFilterBank ( HANDLE_HYBRID *phHybrid ) /* < Pointer to handle to HYBRID struct. */
{
  COUNT_sub_start("DeleteHybridFilterBank");
  COUNT_sub_end();

  return;
}
#endif
