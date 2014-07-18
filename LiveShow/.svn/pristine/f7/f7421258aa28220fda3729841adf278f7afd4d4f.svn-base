/*
  Complex qmf analysis/synthesis
*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "sbr_fft.h"
#include "qmf_dec.h"
#include "string.h"
#include "sbr_ram.h"
#include "sbr_rom.h"
#include "counters.h"

#define QMF_ANA_FILTER_STRIDE  2

/*
    Perform dct type 4
 */
static void
dct4 (float *data,
      int L,
      HANDLE_SBR_QMF_FILTER_BANK qmfBank
      )
{
  int i, M, ld;
  float wim, wre;
  float re1, im1, re2, im2;
  struct dct4Twiddle *pTwiddle;

  const float cosPiBy8  = 0.92387953251129f;
  const float cosPi3By8 = 0.38268343236509f;
  MOVE(2);

  MOVE(1);
  i = 1;

  MULT(1);
  M = L / 2;

  MOVE(1);
  ld = -2;


  ADD(1); BRANCH(1);
  if (L > 2) {

    LOOP(1);
    while(i < L) {
      SHIFT(1);
      i <<= 1;

      ADD(1);
      ld++;
    }

    INDIRECT(1); PTR_INIT(1);
    pTwiddle = &qmfBank->pDct4Twiddle[ld];

    MULT(1); /* M / 2 */
    LOOP(1); PTR_INIT(6); /* pointers for data[2 * i],
                                          data[2 * M - 2 * i],
                                          pTwiddle->sin_twiddle[],
                                          pTwiddle->cos_twiddle[],
                                          pTwiddle->sin_twiddle[M - 1 - i],
                                          pTwiddle->cos_twiddle[M - 1 - i] */
    for (i = 0; i < M / 2; i++) {

      MOVE(4);
      re1 = data[2 * i];
      im2 = data[2 * i + 1];
      re2 = data[2 * M - 2 - 2 * i];
      im1 = data[2 * M - 1 - 2 * i];

      MOVE(2);
      wim = pTwiddle->sin_twiddle[i];
      wre = pTwiddle->cos_twiddle[i];

      MULT(1); MAC(1); STORE(1);
      data[2 * i]     = im1 * wim + re1 * wre;

      MULT(2); ADD(1); STORE(1);
      data[2 * i + 1] = im1 * wre - re1 * wim;

      MOVE(2);
      wim = pTwiddle->sin_twiddle[M - 1 - i];
      wre = pTwiddle->cos_twiddle[M - 1 - i];

      MULT(1); MAC(1); STORE(1);
      data[2 * M - 2 - 2 * i] = im2 * wim + re2 * wre;

      MULT(2); ADD(1); STORE(1);
      data[2 * M - 1 - 2 * i] = im2 * wre - re2 * wim;
    }

    ADD(1); BRANCH(1);
    if (M == 2) {

      MOVE(2);
      re1 = data[0];
      im1 = data[1];

      ADD(4); STORE(4);
      data[0] = re1 + data[2];
      data[1] = im1 + data[3];
      data[2] = re1 - data[2];
      data[3] = im1 - data[3];
    }
    else {
      FUNC(2);
      sbrfft(data,M);
    }

    INDIRECT(2); MOVE(2);
    wim = pTwiddle->alt_sin_twiddle[0];
    wre = pTwiddle->alt_sin_twiddle[M];

    PTR_INIT(4);  /* pointers for data[2 * i],
                                  data[2 * M - 2 * i],
                                  pTwiddle->alt_sin_twiddle[i],
                                  pTwiddle->alt_sin_twiddle[M - 1 - i] */
    LOOP(1);
    for (i = 0; i < M / 2; i++) {

      MOVE(4);
      re1 = data[2 * i];
      im1 = data[2 * i + 1];
      re2 = data[2 * M - 2 - 2 * i];
      im2 = data[2 * M - 1 - 2 * i];

      MULT(1); MAC(1); STORE(1);
      data[2 * i]             = re1 * wre + im1 * wim;

      MULT(2); ADD(1); STORE(1);
      data[2 * M - 1 - 2 * i] = re1 * wim - im1 * wre;

      MOVE(2);
      wim = pTwiddle->alt_sin_twiddle[i + 1];
      wre = pTwiddle->alt_sin_twiddle[M - 1 - i];

      MULT(1); MAC(1); STORE(1);
      data[2 * M - 2 - 2 * i] = re2 * wim + im2 * wre;
      MULT(2); ADD(1); STORE(1);
      data[2 * i + 1]         = re2 * wre - im2 * wim;
    }
  }
  else { /* 2-point transform */

    MOVE(2);
    re1 = data[0];
    re2 = data[1];

    MULT(1); MAC(1); STORE(1);
    data[0] = re1 * cosPiBy8  + re2 * cosPi3By8;

    MULT(2); ADD(1); STORE(1);
    data[1] = re1 * cosPi3By8 - re2 * cosPiBy8;
  }

}


/*
  Perform dct type 3
 */
static void
dct3 (float *data,
      int L,
      HANDLE_SBR_QMF_FILTER_BANK qmfBank
      )
{
  int i, M, N;
  float s1, s2, s3, s4;
  float temp[16];

  const float sqrtHalf  = 0.70710678118655f;
  MOVE(1);

  MULT(1);
  M = L / 2;

  MULT(1);
  N = L / 4;


  ADD(1); BRANCH(1);
  if (L > 2) {

    PTR_INIT(2);  /* pointers for data[2 * i + 1],
                                  temp[i] */
    LOOP(1);
    for(i = 0; i < N; i++) {
      MOVE(1);
      temp[i] = data[2*i+1];
    }

    PTR_INIT(2);  /* pointers for data[2 * i],
                                  data[i] */
    LOOP(1);
    for(i = 1; i < M; i++) {
      MOVE(1);
      data[i] = data[2*i];
    }

    PTR_INIT(2);  /* pointers for data[L-1-i],
                                  data[L-1 - 2*i] */
    LOOP(1);
    for(i = 1; i < N; i++) {
      MOVE(1);
      data[L-1 - i] = data[L-1 - 2*i];
    }

    PTR_INIT(2);  /* pointers for data[i+M],
                                  temp[i] */
    LOOP(1);
    for(i = 0; i < N; i++) {
      MOVE(1);
      data[i + M] = temp[i];
    }

    FUNC(3);
    dct3(data,     M, qmfBank);

    FUNC(3);
    dct4(data + M, M, qmfBank);

    PTR_INIT(4);  /* pointers for data[i],
                                  data[i + M],
                                  data[M-1 - i],
                                  data[L-1 - i] */
    LOOP(1);
    for(i = 0; i < N; i++) {

      MOVE(4);
      s1 = data[i];
      s2 = data[i + M];
      s3 = data[M-1 - i];
      s4 = data[L-1 - i];

      ADD(4); STORE(4);
      data[i]       = (s1 + s2);
      data[L-1 - i] = (s1 - s2);
      data[M-1 - i] = (s3 + s4);
      data[i + M]   = (s3 - s4);
    }

  }
  else {
    MULT(1);
    s1 = data[1] * sqrtHalf;

    ADD(2); STORE(2);
    data[1] = (data[0] - s1);
    data[0] = (data[0] + s1);
  }
}

/*
   Perform dct type 2
 */
static void
dct2 (float *data,
      int L,
      HANDLE_SBR_QMF_FILTER_BANK qmfBank
      )
{
  int i, M, N;
  float s1, s2, s3, s4;
  float temp[16];

  const float sqrtHalf  = 0.70710678118655f;
  MOVE(1);

  MULT(1);
  M = L / 2;

  MULT(1);
  N = L / 4;

  ADD(1); BRANCH(1);
  if (L > 2) {

    PTR_INIT(4);  /* pointers for data[i],
                                  data[i + M],
                                  data[M-1 - i],
                                  data[L-1 - i] */
    LOOP(1);
    for(i = 0; i < N; i++) {

      MOVE(4);
      s1 = data[i];
      s2 = data[i + M];
      s3 = data[M-1 - i];
      s4 = data[L-1 - i];

      ADD(4); STORE(4);
      data[i]       = (s1 + s4);
      data[i + M]   = (s1 - s4);
      data[M-1 - i] = (s3 + s2);
      data[L-1 - i] = (s3 - s2);
    }

    FUNC(3);
    dct2(data,     M, qmfBank);

    FUNC(3);
    dct4(data + M, M, qmfBank);

    PTR_INIT(2);  /* pointers for data[2*i + M],
                                  temp[i]        */
    LOOP(1);
    for(i = 0; i < N; i++) {
      MOVE(1);
      temp[i] = data[2*i + M];
    }

    PTR_INIT(2);  /* pointers for data[L - 2*i],
                                  data[M - i]    */
    LOOP(1);
    for(i = 1; i < M; i++) {
      MOVE(1);
      data[L - 2*i] = data[M - i];
    }

    PTR_INIT(2);  /* pointers for data[4*i - 1],
                                  data[M-1 + 2*i] */
    LOOP(1);
    for(i = 1; i < N; i++) {
      MOVE(1);
      data[4*i - 1] = data[M-1 + 2*i];
    }

    PTR_INIT(2);  /* pointers for data[4*i + 1],
                                  temp[i]        */
    LOOP(1);
    for(i = 0; i < N; i++) {
      MOVE(1);
      data[4*i + 1] = temp[i];
    }
  }
  else { /* 2-point transform */
    MOVE(1);
    s1 = data[0];

    ADD(1); STORE(1);
    data[0] =   (s1 + data[1]);

    ADD(1); MAC(1); STORE(1);
    data[1] =  ((s1 - data[1]) * sqrtHalf);
  }
}

/*
 *
 * \brief Perform real-valued forward modulation of the time domain
 *        data of timeIn and stores the real part of the subband
 *        samples in rSubband
 *
 */
static void
sbrForwardModulationLP (const float *timeIn,
                        float *rSubband,
                        HANDLE_SBR_QMF_FILTER_BANK qmfBank
                        )
{
  int i, L, M;

  COUNT_sub_start("sbrForwardModulationLP");

  MOVE(1);
  L = NO_ANALYSIS_CHANNELS;

  MULT(1);
  M = L/2;

  PTR_INIT(1);  /* pointers for rSubband[] */
  MULT(1); MOVE(1);
  rSubband[0] = timeIn[3 * M];

  PTR_INIT(2);  /* pointers for timeIn[3 * M - i],
                                timeIn[3 * M + i]  */
  LOOP(1);
  for (i = 1; i < M; i++) {
    ADD(1); STORE(1);
    rSubband[i] = timeIn[3 * M - i] + timeIn[3 * M + i];
  }

  LOOP(1);
  for (i = M; i < L; i++) {
    ADD(1); STORE(1);
    rSubband[i] = timeIn[3 * M - i] - timeIn[i - M];
  }

  FUNC(3);
  dct3 (rSubband, L, qmfBank);

  COUNT_sub_end();
}


#ifndef LP_SBR_ONLY

/*
 *
 * \brief Cosine modulation of the time domain data of a subband. Performed in-place
 *
 */
static void
cosMod (float *subband,
        HANDLE_SBR_QMF_FILTER_BANK qmfBank
        )
{
  int i, M;
  float wim, wre;
  float re1, im1, re2, im2;
  float accu1,accu2;

  COUNT_sub_start("cosMod");

  INDIRECT(1); SHIFT(1);
  M = qmfBank->no_channels >> 1;

  PTR_INIT(6);  /* pointers for subband[2 * i],
                                subband[2 * M - 2 * i],
                                qmfBank->sin_twiddle[i],
                                qmfBank->cos_twiddle[i],
                                qmfBank->sin_twiddle[M - 1 - i],
                                qmfBank->cos_twiddle[M - 1 - i]  */

  MULT(1); /* M/2 */ LOOP(1);
  for (i = 0; i < M /2; i++) {

    MOVE(4);
    re1 = subband[2 * i];
    im2 = subband[2 * i + 1];
    re2 = subband[2 * M - 2 - 2 * i];
    im1 = subband[2 * M - 1 - 2 * i];

    MOVE(2);
    wim = qmfBank->sin_twiddle[i];
    wre = qmfBank->cos_twiddle[i];

    MULT(1); MAC(1);
    accu1 =  im1 * wim + re1 * wre;
    MULT(2); ADD(1);
    accu2 =  im1 * wre - re1 * wim;

    MOVE(2);
    subband[2 * i] = accu1;
    subband[2 * i + 1] = accu2;

    MOVE(2);
    wim = qmfBank->sin_twiddle[M - 1 - i];
    wre = qmfBank->cos_twiddle[M - 1 - i];

    MULT(1); MAC(1);
    accu1 = im2 * wim + re2 * wre;
    MULT(2); ADD(1);
    accu2 = im2 * wre - re2 * wim;

    MOVE(2);
    subband[2 * M - 2 - 2 * i] = accu1;
    subband[2 * M - 1 - 2 * i] = accu2;
  }

  FUNC(2);
  sbrfft(subband, M);

  PTR_INIT(4);  /* pointers for  subband[2 * i],
                                 subband[2 * M - 2 * i],
                                 qmfBank->alt_sin_twiddle[i],
                                 qmfBank->sin_twiddle[M - 1 - i] */

  MOVE(2);
  wim = qmfBank->alt_sin_twiddle[0];
  wre = qmfBank->alt_sin_twiddle[M];

  LOOP(1);
  for (i = 0; i < M / 2; i++) {

    MOVE(4);
    re1 = subband[2 * i];
    im1 = subband[2 * i + 1];
    re2 = subband[2 * M - 2 - 2 * i];
    im2 = subband[2 * M - 1 - 2 * i];


    MULT(1); MAC(1);
    accu1=re1 * wre + im1 * wim;
    MULT(2); ADD(1);
    accu2=re1 * wim - im1 * wre;

    MOVE(2);
    subband[2 * i] = accu1;
    subband[2 * M - 1 - 2 * i]= accu2;

    MOVE(2);
    wim = qmfBank->alt_sin_twiddle[i + 1];
    wre = qmfBank->alt_sin_twiddle[M - 1 - i];

    MULT(1); MAC(1);
    accu1=re2 * wim + im2 * wre;
    MULT(2); ADD(1);
    accu2=re2 * wre - im2 * wim;

    MOVE(2);
    subband[2 * M - 2 - 2 * i] = accu1;
    subband[2 * i + 1] = accu2;
  }

  COUNT_sub_end();
}


/*
 *
 * \brief Sine modulation of the time domain data of a subband. Performed in-place
 *
 */
static void
sinMod (float *subband,
        HANDLE_SBR_QMF_FILTER_BANK qmfBank
        )
{
  int i, M;
  float wre, wim;
  float re1, im1, re2, im2;
  float accu1,accu2;

  COUNT_sub_start("sinMod");

  INDIRECT(1); SHIFT(1);
  M = qmfBank->no_channels >> 1;

  PTR_INIT(6);  /* pointers for subband[2 * i],
                                subband[2 * M - 2 * i],
                                qmfBank->sin_twiddle[i],
                                qmfBank->cos_twiddle[i],
                                qmfBank->sin_twiddle[M - 1 - i],
                                qmfBank->cos_twiddle[M - 1 - i]  */

  MULT(1); /* M/2 */ LOOP(1);
  for (i = 0; i < M / 2; i++) {

    MOVE(4);
    re1 = subband[2 * i];
    im2 = subband[2 * i + 1];
    re2 = subband[2 * M - 2 - 2 * i];
    im1 = subband[2 * M - 1 - 2 * i];

    MOVE(2);
    wre = qmfBank->sin_twiddle[i];
    wim = qmfBank->cos_twiddle[i];

    MULT(1); MAC(1);
    accu1 = im1 * wim + re1 * wre;
    MULT(2); ADD(1);
    accu2 = im1 * wre - re1 * wim;

    MOVE(2);
    subband[2 * i + 1] = accu1;
    subband[2 * i] = accu2;

    MOVE(2);
    wre = qmfBank->sin_twiddle[M - 1 - i];
    wim = qmfBank->cos_twiddle[M - 1 - i];

    MULT(1); MAC(1);
    accu1 = im2 * wim + re2 * wre;
    MULT(2); ADD(1);
    accu2 = im2 * wre - re2 * wim;

    MOVE(2);
    subband[2 * M - 1 - 2 * i] = accu1;
    subband[2 * M - 2 - 2 * i] = accu2;
  }

  FUNC(2);
  sbrfft(subband, M);

  PTR_INIT(4);  /* pointers for  subband[2 * i],
                                 subband[2 * M - 2 * i],
                                 qmfBank->alt_sin_twiddle[i],
                                 qmfBank->sin_twiddle[M - 1 - i] */
  MOVE(2);
  wim = qmfBank->alt_sin_twiddle[0];
  wre = qmfBank->alt_sin_twiddle[M];

  for (i = 0; i < M / 2; i++) {

    MOVE(4);
    re1 = subband[2 * i];
    im1 = subband[2 * i + 1];
    re2 = subband[2 * M - 2 - 2 * i];
    im2 = subband[2 * M - 1 - 2 * i];

    MULT(2); MAC(1);
    accu1 = -(re1 * wre + im1 * wim);
    MULT(2); ADD(1);
    accu2 = -(re1 * wim - im1 * wre);

    MOVE(2);
    subband[2 * M - 1 - 2 * i] = accu1;
    subband[2 * i] = accu2;

    MOVE(2);
    wim = qmfBank->alt_sin_twiddle[i + 1];
    wre = qmfBank->alt_sin_twiddle[M - 1 - i];

    MULT(2); MAC(1);
    accu1 = -(re2 * wim + im2 * wre);
    MULT(2); ADD(1);
    accu2 = -(re2 * wre - im2 * wim);

    MOVE(2);
    subband[2 * i + 1] = accu1;
    subband[2 * M - 2 - 2 * i] = accu2;

  }

  COUNT_sub_end();
}

/*
 *
 * \brief Perform complex-valued forward modulation of the time domain
 *        data of timeIn and stores the real part of the subband
 *        samples in rSubband, and the imaginary part in iSubband
 *
 */
static void
sbrForwardModulation (const float *timeIn,
                      float *rSubband,
                      float *iSubband,
                      HANDLE_SBR_QMF_FILTER_BANK anaQmf
                      )
{
  int i, offset;

  float real, imag;

  COUNT_sub_start("sbrForwardModulation");

  MOVE(1);
  offset = 2 * NO_ANALYSIS_CHANNELS;

  PTR_INIT(1);  /* pointer for timeIn[offset - 1 - i] */
  LOOP(1);
  for (i = 0; i < NO_ANALYSIS_CHANNELS; i++) {
    ADD(2); STORE(2);
    rSubband[i] = timeIn[i] - timeIn[offset - 1 - i];
    iSubband[i] = timeIn[i] + timeIn[offset - 1 - i];
  }

  FUNC(2);
  cosMod (rSubband, anaQmf);
  FUNC(2);
  sinMod (iSubband, anaQmf);

  PTR_INIT(4);  /* pointers for rSubband[i],
                                iSubband[i],
                                anaQmf->t_cos[i],
                                anaQmf->t_sin[i]  */
  INDIRECT(1); LOOP(1);
  for (i = 0; i < anaQmf->lsb; i++) {

    MOVE(2);
    real = rSubband[i];
    imag = iSubband[i];

    MULT(1); MAC(1); STORE(1);
    rSubband[i] = real * anaQmf->t_cos[i] + imag * anaQmf->t_sin[i];
    MULT(2); ADD(1); STORE(1);
    iSubband[i] = imag * anaQmf->t_cos[i] - real * anaQmf->t_sin[i];
  }

  COUNT_sub_end();
}
#endif /* #ifndef LP_SBR_ONLY */


/*
 *
 * \brief Perform complex-valued subband filtering of the time domain
 *        data of timeIn and stores the real part of the subband
 *        samples in rAnalysis, and the imaginary part in iAnalysis
 *
 */
void
cplxAnalysisQmfFiltering (
                          const float *timeIn,
                          float **qmfReal,
#ifndef LP_SBR_ONLY
                          float **qmfImag,
#endif
                          HANDLE_SBR_QMF_FILTER_BANK anaQmf,
                          int   bUseLP
                          )
{
  int i, k;
  float analysisBuffer[NO_ANALYSIS_CHANNELS*2];
  const float *ptr_pf;
  const float *ptr_states;
  const float *ptr_timeIn;
  int p;
  float accu;

  COUNT_sub_start("cplxAnalysisQmfFiltering");

  PTR_INIT(1);
  ptr_timeIn = timeIn;

  INDIRECT(1); LOOP(1);
  for (i = 0; i < anaQmf->no_col; i++) {

    FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(2*NO_ANALYSIS_CHANNELS);
    memset (analysisBuffer, 0, 2*NO_ANALYSIS_CHANNELS * sizeof (float));

    INDIRECT(2); PTR_INIT(2);
    ptr_pf     = anaQmf->p_filter;
    ptr_states = anaQmf->FilterStatesAna;


    FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(NO_ANALYSIS_CHANNELS);
    memcpy(anaQmf->FilterStatesAna+(QMF_FILTER_STATE_ANA_SIZE-NO_ANALYSIS_CHANNELS),ptr_timeIn,NO_ANALYSIS_CHANNELS*sizeof(float));

                                                                       COUNT_sub_start("AnalysisPolyphaseFiltering");

                                                                       LOOP(1); PTR_INIT(1);
    for (k = 0; k < NO_ANALYSIS_CHANNELS; k++) {
      ptr_pf += NO_POLY * (QMF_ANA_FILTER_STRIDE - 1);                 MAC(1);
      accu=0;                                                          MOVE(1);

      for (p = 0; p < NO_POLY; p++) {
        accu +=  *ptr_pf++ * ptr_states[2*NO_ANALYSIS_CHANNELS * p];   MAC(1);
      }
      analysisBuffer[2*NO_ANALYSIS_CHANNELS - 1 - k] = accu;           STORE(1);
      ptr_states++;
    }

    accu=0;                                                            MOVE(1); LOOP(1); PTR_INIT(1);
    for (p = 0; p < NO_POLY; p++) {
      accu +=  *ptr_pf++ * anaQmf->FilterStatesAna[2*NO_ANALYSIS_CHANNELS * p + 2*NO_ANALYSIS_CHANNELS-1]; MAC(1);
    }
    analysisBuffer[0] = accu;                                          MOVE(1);
    ptr_pf -= NO_POLY * 2;                                             PTR_INIT(1);

                                                                       LOOP(1); PTR_INIT(1);
    for (k = 0; k < NO_ANALYSIS_CHANNELS-1; k++){
      ptr_pf -= NO_POLY * (QMF_ANA_FILTER_STRIDE - 1);                 PTR_INIT(1);
      accu=0;                                                          MOVE(1);

      for (p = 0; p < NO_POLY; p++) {
        accu +=  *--ptr_pf * ptr_states[2*NO_ANALYSIS_CHANNELS * p];   MAC(1);
      }
      analysisBuffer[NO_ANALYSIS_CHANNELS - 1 - k] = accu;             STORE(1);
      ptr_states++;
    }

                                                                       COUNT_sub_end();



    BRANCH(1);
    if (bUseLP) {
      FUNC(3);
      sbrForwardModulationLP (analysisBuffer,
                              qmfReal[i],
                              anaQmf);
    }
#ifndef LP_SBR_ONLY
    else {
      FUNC(4);
      sbrForwardModulation (analysisBuffer,
                            qmfReal[i],
                            qmfImag[i],
                            anaQmf);
    }
#endif

    /*
      Shift filter states
      Should be realized with modulo adressing on a DSP instead of a true buffer shift
    */
    /* FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(NO_ANALYSIS_CHANNELS); */
    memmove(anaQmf->FilterStatesAna,anaQmf->FilterStatesAna+NO_ANALYSIS_CHANNELS,(QMF_FILTER_STATE_ANA_SIZE-NO_ANALYSIS_CHANNELS)*sizeof(float));

    ptr_timeIn += NO_ANALYSIS_CHANNELS;
  }

  COUNT_sub_end();
}


static void
inverseModulationLP (float *qmfReal,
                     float *qmfReal2,
                     HANDLE_SBR_QMF_FILTER_BANK synQmf
                     )
{
  int i, L, M;
  float timeOut[2*NO_ACTUAL_SYNTHESIS_CHANNELS];

  COUNT_sub_start("inverseModulationLP");

  INDIRECT(1);
  L = synQmf->no_channels;

  MULT(1); STORE(1);
  M = L / 2;

  PTR_INIT(2);  /* pointers for timeOut[],
                                qmfReal[]  */
  INDIRECT(1); LOOP(1);
  for (i = 0; i < synQmf->usb; i++) {

    MOVE(1);
    timeOut[i + M] = qmfReal[i];
  }

  INDIRECT(1); LOOP(1);
  for (i = synQmf->usb; i < L; i++) {

    MOVE(1);
    timeOut[i + M] = 0;
  }

  FUNC(3);
  dct2(timeOut+M, L, synQmf);

  MOVE(1);
  timeOut[3 * M] = 0;

  PTR_INIT(2);  /* pointers for timeOut[i]
                                timeOut[-i] */
  LOOP(1);
  for (i = 1; i < M; i++) {

    MULT(1); STORE(1);
    timeOut[i + 3 * M] = - timeOut[3 * M - i];
  }

  PTR_INIT(2);  /* pointer for timeOut[i],
                               timeOut[L-i] */
  LOOP(1);
  for (i = 0; i < M; i++) {

    MOVE(1);
    timeOut[i] = timeOut[L - i];
  }

  PTR_INIT(2);  /* pointer for timeOut[],
                               qmfReal[]  */
  LOOP(1);
  for (i = 0; i < L; i++) {

    MOVE(1);
    qmfReal[i] = timeOut[i];
  }

  PTR_INIT(2);  /* pointer for timeOut[],
                               qmfReal2[]  */
  LOOP(1);
  for (i = 0; i < L; i++) {

    MOVE(1);
    qmfReal2[i] = timeOut[L+i];
  }

  COUNT_sub_end();
}

#ifndef LP_SBR_ONLY
/*
 *
 * \brief Perform complex-valued inverse modulation of the subband
 *        samples stored in rSubband (real part) and iSubband (imaginary
 *        part) and stores the result in timeOut
 *
 */
static void
inverseModulation (float *qmfReal,
                   float *qmfImag,
                   HANDLE_SBR_QMF_FILTER_BANK synQmf
                   )
{
  int i, no_synthesis_channels, M;

  float r1, i1, r2, i2;

  COUNT_sub_start("inverseModulation");

  INDIRECT(1); MOVE(1);
  no_synthesis_channels = synQmf->no_channels;

  MULT(1);
  M = no_synthesis_channels / 2;
  PTR_INIT(2);  /* pointer for qmfReal[],
                               qmfImag[] */
  INDIRECT(1); LOOP(1);
  for (i = synQmf->usb; i < no_synthesis_channels; i++) {
    MOVE(2);

    qmfReal[i]=qmfImag[i]=0;
  }

  FUNC(2);
  cosMod (qmfReal, synQmf);
  FUNC(2);
  sinMod (qmfImag, synQmf);

  PTR_INIT(4);  /* pointer for qmfReal[],
                               qmfImag[],
                               qmfImag[no_synthesis_channels - 1 - i],
                               qmfReal[no_synthesis_channels - i - 1]   */
  LOOP(1);
  for (i = 0; i < M; i++) {

    MOVE(4);
    r1 = qmfReal[i];
    i2 = qmfImag[no_synthesis_channels - 1 - i];
    r2 = qmfReal[no_synthesis_channels - i - 1];
    i1 = qmfImag[i];

    ADD(4); STORE(4);
    qmfReal[i] = (r1 - i1);
    qmfImag[no_synthesis_channels - 1 - i] = -(r1 + i1);
    qmfReal[no_synthesis_channels - i - 1] = (r2 - i2);
    qmfImag[i] = -(r2 + i2);
  }

  COUNT_sub_end();
}
#endif /* #ifndef LP_SBR_ONLY */


/*
 *
 *
 * \brief Perform complex-valued subband synthesis of the
 *        low band and the high band and store the
 *        time domain data in timeOut
 *
*/
void
cplxSynthesisQmfFiltering( float **qmfReal,
#ifndef LP_SBR_ONLY
                           float **qmfImag,
#endif
                           float *timeOut,
                           HANDLE_SBR_QMF_FILTER_BANK synQmf,
                           int   bUseLP,
                           HANDLE_PS_DEC h_ps_dec,
                           int   active
                           )
{
  int i, j;
  float *ptr_time_out;

  float *filterStates;

  float accu;
  int p;
  float qmfReal2[NO_ACTUAL_SYNTHESIS_CHANNELS];

  float *imagSlot;

  int no_synthesis_channels;
  int qmf_filter_state_syn_size;

  float qmfRealTmp[NO_ACTUAL_SYNTHESIS_CHANNELS];
  float qmfImagTmp[NO_ACTUAL_SYNTHESIS_CHANNELS];

  int env;

  COUNT_sub_start("cplxSynthesisQmfFiltering");

  MOVE(1);
  env = 0;

  FUNC(3); LOOP(1); PTR_INIT(1); MOVE(1); STORE(NO_ACTUAL_SYNTHESIS_CHANNELS);
  memset(qmfRealTmp,0,NO_ACTUAL_SYNTHESIS_CHANNELS*sizeof(float));

  FUNC(3); LOOP(1); PTR_INIT(1); MOVE(1); STORE(NO_ACTUAL_SYNTHESIS_CHANNELS);
  memset(qmfImagTmp,0,NO_ACTUAL_SYNTHESIS_CHANNELS*sizeof(float));

  INDIRECT(1); MOVE(1);
  no_synthesis_channels = synQmf->no_channels;

  INDIRECT(1); MOVE(1);
  qmf_filter_state_syn_size = synQmf->qmf_filter_state_size;

  INDIRECT(1); PTR_INIT(1);
  filterStates = synQmf->FilterStatesSyn;

  PTR_INIT(1);
  ptr_time_out = timeOut;

  INDIRECT(1); LOOP(1);
  for (i = 0; i < synQmf->no_col; i++) {

    const float *p_filter = synQmf->p_filter;
    INDIRECT(1); PTR_INIT(1);

    BRANCH(1);
    if (bUseLP) {

      PTR_INIT(1);
      imagSlot = qmfReal2;
    }
#ifndef LP_SBR_ONLY
    else {

      PTR_INIT(1);
      imagSlot = *(qmfImag + i);
    }
#endif


#ifndef MONO_ONLY
    BRANCH(1);
    if(active){

      ADD(1); INDIRECT(1); BRANCH(1);
      if(i == h_ps_dec-> aEnvStartStop[env]){

        FUNC(3); INDIRECT(1);
        InitRotationEnvelope(h_ps_dec,env,synQmf->usb);
        env++;
      }

      FUNC(5);
      ApplyPsSlot(h_ps_dec,
                  &qmfReal[i],
                  &qmfImag[i],
                  qmfRealTmp,
                  qmfImagTmp);
    }
#endif



#ifndef  LP_SBR_ONLY
    BRANCH(1);
    if(!bUseLP) {

      BRANCH(1);
      if(no_synthesis_channels == NO_SYNTHESIS_CHANNELS_DOWN_SAMPLED){

        PTR_INIT(4);  /* pointers for qmfReal[i][j],
                                      imagSlot[j],
                                      synQmf->cos_twiddle_ds[j],
                                      synQmf->sin_twiddle_ds[j]  */
        LOOP(1);
        for (j = 0; j < no_synthesis_channels; j++){
          float temp;

          MOVE(1);
          temp = qmfReal[i][j];

          MULT(1); MAC(1); STORE(1);
          qmfReal[i][j] = synQmf->t_cos[j] * qmfReal[i][j] + synQmf->t_sin[j] * imagSlot[j];

          MULT(2); ADD(1); STORE(1);
          imagSlot[j]   = synQmf->t_cos[j] * imagSlot[j]   - synQmf->t_sin[j] * temp;
        }
      }

      PTR_INIT(2);  /* pointers for qmfReal[i][j],
                                    imagSlot[j],   */
      INDIRECT(1); LOOP(1);
      for (j = 0; j < synQmf->usb; j++) {

        MULT(2); STORE(2);
        qmfReal[i][j] *= -1.0;
        imagSlot[j]   *= -1.0;
      }
    }
#endif

    BRANCH(1);
    if (bUseLP) {

      FUNC(3);
      inverseModulationLP (qmfReal[i], imagSlot, synQmf);
    }
#ifndef  LP_SBR_ONLY
    else {

      FUNC(3);
      inverseModulation (qmfReal[i], imagSlot, synQmf);
    }
#endif

    BRANCH(1);
    if (bUseLP) {

      PTR_INIT(2);  /* pointers for qmfReal[i][j],
                                    imagSlot[j],   */
      LOOP(1);
      for (j = 0; j < no_synthesis_channels; j++) {

        MULT(2); STORE(2);
        qmfReal[i][j] =  qmfReal[i][j] * 0.0625f;
        imagSlot[j]   =  imagSlot[j]   * 0.0625f;
      }
    }
#ifndef  LP_SBR_ONLY
    else {

      PTR_INIT(2);  /* pointers for qmfReal[i][j],
                                    imagSlot[j],   */
      LOOP(1);
      for (j = 0; j < no_synthesis_channels; j++) {

        MULT(2); STORE(2);
        qmfReal[i][j] =  qmfReal[i][j] * 0.03125f;
        imagSlot[j]   =  imagSlot[j]   * 0.03125f;
      }
    }
#endif

    COUNT_sub_start("SynthesisPolyphaseFiltering");

    MULT(1); PTR_INIT(2);  /* pointers for filterStates[p * 2*no_synthesis_channels + j],
                                           imagSlot[no_synthesis_channels -1 - j]         */
    LOOP(1);
    for (j = 0; j < no_synthesis_channels; j++){
      float newSample;
      MOVE(1);
      newSample = imagSlot[no_synthesis_channels -1 - j];

      if(no_synthesis_channels == 32){
        p_filter += NO_POLY;
      }

      LOOP(1);
      for (p = 0; p < NO_POLY; p++) {

        MAC(1); STORE(1);
        accu = filterStates[p * 2*no_synthesis_channels + j] + (*p_filter++) * newSample;
        filterStates[p * 2*no_synthesis_channels + j] = accu;

      }

    }


    MULT(2); PTR_INIT(1);  /* pointer for filterStates[p * 2*no_synthesis_channels + no_synthesis_channels + (no_synthesis_channels-1)] */
    LOOP(1);
    for (p = 0; p < NO_POLY; p++) {

      MAC(1); STORE(1);
      accu =  filterStates[p * 2*no_synthesis_channels + no_synthesis_channels + (no_synthesis_channels-1) ] +  (*p_filter++) * qmfReal[i][0];
      filterStates[p * 2*no_synthesis_channels + no_synthesis_channels + (no_synthesis_channels-1)] = accu;

    }
    MOVE(1);
    ptr_time_out[0] = accu;

    p_filter -= NO_POLY*2;

    MULT(2); PTR_INIT(2);  /* pointers for filterStates[p * 2*no_synthesis_channels + no_synthesis_channels + j],
                                           qmfReal[no_synthesis_channels -1 - j]                                  */
    ADD(1); LOOP(1);
    for (j = 0; j < no_synthesis_channels-1; j++){
      float newSample;

      MOVE(1);
      newSample = qmfReal[i][no_synthesis_channels -1 - j];

      if(no_synthesis_channels == 32){

        p_filter -= NO_POLY;
      }

      LOOP(1);
      for (p = 0; p < NO_POLY; p++) {

        MAC(1); STORE(1);
        accu =  filterStates[p * 2*no_synthesis_channels + no_synthesis_channels + j] + (*--p_filter) * newSample;
        filterStates[p * 2*no_synthesis_channels + no_synthesis_channels + j] = accu;

      }

      MOVE(1);
      ptr_time_out[no_synthesis_channels - 1 - j] = accu;
    }


    ptr_time_out += no_synthesis_channels;

    /*
      Shift filter states
      Should be replaces by modulo operation if available
    */
    /* FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE((qmf_filter_state_syn_size - no_synthesis_channels)); */
    memmove (filterStates + no_synthesis_channels, filterStates,
             (qmf_filter_state_syn_size - no_synthesis_channels) * sizeof (float));

    /* FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(no_synthesis_channels); */
    memset (filterStates, 0, no_synthesis_channels * sizeof (float));

    BRANCH(1);
    if(active){
      FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(no_synthesis_channels);
      memcpy(qmfReal[i],qmfRealTmp,sizeof(float)*no_synthesis_channels);

#ifndef  LP_SBR_ONLY
      FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(no_synthesis_channels);
      memcpy(qmfImag[i],qmfImagTmp,sizeof(float)*no_synthesis_channels);
#endif
    }

    COUNT_sub_end();

  }

  COUNT_sub_end();
}



/*
 *
 * \brief Create QMF filter bank instance
 *
 * \return 0 if succesful
 *
 */
int
createCplxAnalysisQmfBank (unsigned int keyflag,
						   HANDLE_SBR_QMF_FILTER_BANK h_sbrQmf,
                           int noCols,
                           int lsb,
                           int usb,
                           int chan)
{
  int L;

  COUNT_sub_start("createCplxAnalysisQmfBank");


  FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(sizeof(SBR_QMF_FILTER_BANK));
  memset(h_sbrQmf,0,sizeof(SBR_QMF_FILTER_BANK));


  MOVE(1);
  L = NO_ANALYSIS_CHANNELS;

  PTR_INIT(2);
  h_sbrQmf->p_filter = sbr_qmf_64_640;
  h_sbrQmf->pDct4Twiddle = dct4TwiddleTable;

#ifndef LP_SBR_ONLY
  PTR_INIT(5);
  h_sbrQmf->cos_twiddle = sbr_cos_twiddle_L32;
  h_sbrQmf->sin_twiddle = sbr_sin_twiddle_L32;
  h_sbrQmf->alt_sin_twiddle = sbr_alt_sin_twiddle_L32;
  h_sbrQmf->t_cos = sbr_t_cos_L32;
  h_sbrQmf->t_sin = sbr_t_sin_L32;
#endif

  MOVE(4);
  h_sbrQmf->no_channels = L;
  h_sbrQmf->no_col = noCols;
  h_sbrQmf->lsb = lsb;
  h_sbrQmf->usb = usb;

  PTR_INIT(1);
  h_sbrQmf->FilterStatesAna   = getSbrCodecQmfStatesAnalysis(keyflag,chan*QMF_FILTER_STATE_ANA_SIZE);//&(sbr_CodecQmfStatesAnalysis[chan*QMF_FILTER_STATE_ANA_SIZE]);

  FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(QMF_FILTER_STATE_ANA_SIZE);
  memset(h_sbrQmf->FilterStatesAna,0,QMF_FILTER_STATE_ANA_SIZE*sizeof(float));

  COUNT_sub_end();

  return (0);

}


/*
 *
 * \brief Create QMF filter bank instance
 *
 * \return 0 if successful
 *
 */
int
createCplxSynthesisQmfBank (unsigned int keyflag,
							HANDLE_SBR_QMF_FILTER_BANK h_sbrQmf,
                            int noCols,
                            int lsb,
                            int usb,
                            int chan,
                            int bDownSample)
{
  int L, qmfFilterStateSize;

  COUNT_sub_start("createCplxSynthesisQmfBank");

  FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(sizeof(SBR_QMF_FILTER_BANK));
  memset(h_sbrQmf,0,sizeof(SBR_QMF_FILTER_BANK));

  BRANCH(1);
  if(bDownSample){

    MOVE(2);
    L = NO_SYNTHESIS_CHANNELS_DOWN_SAMPLED;
    qmfFilterStateSize = QMF_FILTER_STATE_SYN_SIZE_DOWN_SAMPLED;
  }
  else{

    MOVE(2);
    L = NO_SYNTHESIS_CHANNELS;
    qmfFilterStateSize = QMF_FILTER_STATE_SYN_SIZE;
  }

  PTR_INIT(1);
  h_sbrQmf->p_filter = sbr_qmf_64_640;

  MOVE(3);
  h_sbrQmf->no_channels = L;
  h_sbrQmf->qmf_filter_state_size = qmfFilterStateSize;
  h_sbrQmf->no_col = noCols;

  MOVE(1);
  h_sbrQmf->lsb = lsb;

  BRANCH(1);
  if(bDownSample){
    MOVE(1);
    h_sbrQmf->usb = 32;
  }
  else{
    MOVE(1);
    h_sbrQmf->usb = usb;
  }

  PTR_INIT(1);
  h_sbrQmf->pDct4Twiddle = dct4TwiddleTable;

#ifndef LP_SBR_ONLY

  ADD(1); BRANCH(1);
  if(L == 32){

    PTR_INIT(5);
    h_sbrQmf->cos_twiddle = sbr_cos_twiddle_L32;
    h_sbrQmf->sin_twiddle = sbr_sin_twiddle_L32;
    h_sbrQmf->alt_sin_twiddle = sbr_alt_sin_twiddle_L32;
    h_sbrQmf->t_cos = sbr_cos_twiddle_ds_L32;
    h_sbrQmf->t_sin = sbr_sin_twiddle_ds_L32;
  }
  else{

    PTR_INIT(3);
    h_sbrQmf->cos_twiddle = sbr_cos_twiddle_L64;
    h_sbrQmf->sin_twiddle = sbr_sin_twiddle_L64;
    h_sbrQmf->alt_sin_twiddle = sbr_alt_sin_twiddle_L64;
  }
#endif

  PTR_INIT(1);
  h_sbrQmf->FilterStatesSyn = getSbrQmfStatesSynthesis(keyflag,chan*qmfFilterStateSize)/*&sbr_QmfStatesSynthesis[chan*qmfFilterStateSize]*/;

  FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(qmfFilterStateSize);
  memset(h_sbrQmf->FilterStatesSyn,0,qmfFilterStateSize*sizeof(float));

  COUNT_sub_end();

  return 0;

}


