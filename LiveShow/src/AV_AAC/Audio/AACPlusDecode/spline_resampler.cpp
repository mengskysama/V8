/*
  Spline-Interpolation-Resampler
*/
#include <stdlib.h>
#include <assert.h>
#include "spline_resampler.h"
#include "FloatFR.h"

#include "counters.h" /* the 3GPP instrumenting tool */

static const float __2_3 =  (2.0f/3.0f);
static const float __1_6 =  (1.0f/6.0f);


static const float a_22_16 = 0.3f;
static const float b_22_16 = 1.3f;

static const float a_24_16 = 0.24f;
static const float b_24_16 = 1.24f;


static const float a_22_8 = 0.06f; 
static const float b_22_8 = 1.06f; 

static const float a_24_8 = 0.05f; 
static const float b_24_8 = 1.05f; 


struct SPLINE_RESAMPLER { 
  
  int remainder;
  int quotient;

  int distance[2];

  int L;
  int M;

  float invL;

  float oldSamples[MAX_ORDER][2];
  int   position[2];


  float iirFilterCoeff_b;
  float iirFilterCoeff_a;

  float oldSampleFilter[2];

} splineResamplerInstance;

void
CreateSplineResampler(HANDLE_SPLINE_RESAMPLER* hr,
                      int Fin, int Fout)
{
  int i;

  COUNT_sub_start("CreateResampler");

  PTR_INIT(1); STORE(1);
  *hr = &splineResamplerInstance;

  INDIRECT(2); MOVE(2);
  (*hr)->L = Fout;
  (*hr)->M = Fin;

  INDIRECT(1); DIV(1); STORE(1);
  (*hr)->invL         = 1.0f/Fout;

  INDIRECT(2); MOVE(2);
  (*hr)->distance[0]  = 0;
  (*hr)->distance[1]  = 0;

  INDIRECT(2); DIV(2); STORE(2);
  (*hr)->remainder = Fin % Fout;
  (*hr)->quotient  = Fin / Fout;

  INDIRECT(2); MOVE(2);
  (*hr)->position[0]  = 0;
  (*hr)->position[1]  = 0;

  PTR_INIT(2); /* (*hr)->oldSamples[i][0]
                  (*hr)->oldSamples[i][1]
               */
  LOOP(1);
  for (i=0; i<MAX_ORDER; i++) {

    MOVE(2);
    (*hr)->oldSamples[i][0] = 0;
    (*hr)->oldSamples[i][1] = 0;
  }

  BRANCH(2);
  switch(Fin){

  case 22050:
    BRANCH(2);
    switch(Fout){
    case 16000:
      INDIRECT(2); PTR_INIT(2);
      (*hr)->iirFilterCoeff_a = a_22_16;
      (*hr)->iirFilterCoeff_b = b_22_16;
      break;
    case 8000:
      INDIRECT(2); PTR_INIT(2);
      (*hr)->iirFilterCoeff_a = a_22_8;
      (*hr)->iirFilterCoeff_b = b_22_8;
      break;
    };
    break;

  case 24000:
    BRANCH(2);
    switch(Fout){
    case 16000:
      INDIRECT(2); PTR_INIT(2);
      (*hr)->iirFilterCoeff_a = a_24_16;
      (*hr)->iirFilterCoeff_b = b_24_16;
      break;
    case 8000:
      INDIRECT(2); PTR_INIT(2);
      (*hr)->iirFilterCoeff_a = a_24_8;
      (*hr)->iirFilterCoeff_b = b_24_8;
      break;
    };
    break;
  default:
    INDIRECT(2); PTR_INIT(2);
    (*hr)->iirFilterCoeff_a = 0;
    (*hr)->iirFilterCoeff_b = 1;
    break;
  };

  INDIRECT(2); MOVE(2);
  (*hr)->iirFilterCoeff_a = 0;
  (*hr)->iirFilterCoeff_b = 1;

  COUNT_sub_end();
}
 
void CopyResamplerState(HANDLE_SPLINE_RESAMPLER hr)
{
  COUNT_sub_start("CopyResamplerState");

  INDIRECT(2); MOVE(2);
  hr->distance[1] = hr->distance[0];
  hr->position[1] = hr->position[0];

  COUNT_sub_end();
}

void SplineResample(HANDLE_SPLINE_RESAMPLER hr,
                    float* ioBuffer,
                    int inSamples, 
                    int* outSamples,
                    int ch)
{
  int i, k, samplesOut, outIndex = 0;
  float alpha, alpha_2, alpha_3, coeff, accu;
  float *pos;
  int inIndex=0;
  float tmpOutSamples[MAX_ORDER+1];
  int numTmpOutSamples = 0;
  
  COUNT_sub_start("Resample");

  MOVE(3); /* counting previous operations */

  PTR_INIT(10); /* hr->position[ch]
                   hr->distance[ch]
                   hr->quotient
                   hr->remainder
                   hr->L
                   hr->invL
                   hr->iirFilterCoeff_a
                   hr->iirFilterCoeff_b
                   tmpOutSamples[]
                   ioBuffer[]
                */

  /* special treatment for the first output samples: may need oldSamples */
  LOOP(1);
  while (hr->position[ch] < 3){ 

    MULT(3);
    alpha   = hr->invL * hr->distance[ch];
    alpha_2 = alpha * alpha;
    alpha_3 = alpha * alpha_2;
    
    
    ADD(2); MULT(1); MAC(1);
    coeff = (__1_6 * (1.0f - alpha_3) + 0.5f * (alpha_2 - alpha));

    ADD(1); BRANCH(1);
    if (hr->position[ch] >= 3) {

      INDIRECT(1); MULT(1);
      accu  = ioBuffer[hr->position[ch]-3] * coeff;
    }
    else {

      INDIRECT(1); MULT(1);
      accu  = hr->oldSamples[hr->position[ch]][ch] * coeff;
    }
    

    MULT(1); ADD(2);
    coeff = (0.5f * alpha_3 - alpha_2 + __2_3);

    ADD(1); BRANCH(1);
    if (hr->position[ch] >= 2) {

      INDIRECT(1); MAC(1);
      accu += ioBuffer[hr->position[ch]-2] * coeff;
    }
    else {

      INDIRECT(1); MAC(1);
      accu += hr->oldSamples[hr->position[ch]+1][ch] * coeff;
    }
    
    ADD(3); MULT(1);
    coeff = (0.5f * (-alpha_3 + alpha_2 + alpha) + __1_6);

    ADD(1); BRANCH(1);
    if (hr->position[ch] >= 1) {

      INDIRECT(1); MAC(1);
      accu += ioBuffer[hr->position[ch]-1] * coeff;
    }
    else {

      INDIRECT(1); MAC(1);
      accu += hr->oldSamples[hr->position[ch]+2][ch] * coeff;
    }
    
    MULT(1);
    coeff = (__1_6 * alpha_3);

    MAC(1);
    accu += ioBuffer[hr->position[ch]] * coeff;
    
    MOVE(1);
    tmpOutSamples[numTmpOutSamples++] = accu;

    /* Update variables */
    ADD(2);
    hr->position[ch] += hr->quotient;
    hr->distance[ch] += hr->remainder;

    ADD(1); BRANCH(1);
    if(hr->distance[ch] >= hr->L) {

      ADD(2);
      hr->distance[ch] -= hr->L;
      hr->position[ch] += 1;
    }

  }
  
  ADD(1);
  inIndex =  hr->position[ch] - 3;

  assert (inIndex >=0);

  
  /* now we have to work up input buffer samples such long that we can start writing the output samples ... */

  LOOP(1);
  while(numTmpOutSamples) 
  {
    MULT(3);
    alpha   = hr->invL * hr->distance[ch];
    alpha_2 = alpha * alpha;
    alpha_3 = alpha * alpha_2;

    PTR_INIT(1);
    pos = &ioBuffer[inIndex];
        
    ADD(2); MULT(1); MAC(1);
    coeff = (__1_6 * (1.0f - alpha_3) + 0.5f * (alpha_2 - alpha));

    MULT(1);
    accu  = *pos++ * coeff;
    
    ADD(2); MULT(1);
    coeff = (0.5f * alpha_3 - alpha_2 + __2_3);

    MAC(1);
    accu += *pos++ * coeff;
    
    ADD(3); MULT(1);
    coeff = (0.5f * (-alpha_3 + alpha_2 + alpha) + __1_6);

    MAC(1);
    accu += *pos++ * coeff;
    
    MULT(1);
    coeff = (__1_6 * alpha_3);

    MAC(1);
    accu += *pos++ * coeff;

    MOVE(1);
    tmpOutSamples[numTmpOutSamples++] =  accu;

    assert (numTmpOutSamples <= MAX_ORDER+1);


    /* Update variables */
    ADD(2);
    inIndex          += hr->quotient;
    hr->distance[ch] += hr->remainder;

    ADD(1); BRANCH(1);
    if(hr->distance[ch] >= hr->L) {

      ADD(2);
      hr->distance[ch] -= hr->L;
      inIndex          += 1;
    }

    /* write as many output samples as possible */
    ADD(2); BRANCH(1); MOVE(1);
    samplesOut = min( (inIndex-outIndex), numTmpOutSamples );

    PTR_INIT(1); /* tmpOutSamples[] */
    LOOP(1);
    for ( i=0; i<samplesOut; i++ ) {
      MOVE(1);
      ioBuffer[outIndex++] = tmpOutSamples[i];
    }

    PTR_INIT(1); /* tmpOutSamples[] */
    ADD(1); LOOP(1);
    for ( k=0; k<numTmpOutSamples-samplesOut; k++) {
      MOVE(1);
      tmpOutSamples[k] = tmpOutSamples[samplesOut+k];
    }

    ADD(1);
    numTmpOutSamples -= samplesOut;

  }


  /* now we can work directly on the i/o buffer */
  ADD(1); LOOP(1);
  while(inIndex < inSamples - 3) 
  {
    MULT(3);
    alpha   = hr->invL * hr->distance[ch];
    alpha_2 = alpha * alpha;
    alpha_3 = alpha * alpha_2;

    PTR_INIT(1);
    pos = &ioBuffer[inIndex];
        
    ADD(2); MULT(1); MAC(1);
    coeff = (__1_6 * (1.0f - alpha_3) + 0.5f * (alpha_2 - alpha));

    MULT(1);
    accu  = *pos++ * coeff;
    
    MULT(1); ADD(2);
    coeff = (0.5f * alpha_3 - alpha_2 + __2_3);

    MAC(1);
    accu += *pos++ * coeff;
    
    MULT(1); ADD(3);
    coeff = (0.5f * (-alpha_3 + alpha_2 + alpha) + __1_6);

    MAC(1);
    accu += *pos++ * coeff;
    
    MULT(1);
    coeff = (__1_6 * alpha_3);

    MAC(1);
    accu += *pos++ * coeff;


    /* Update variables */
    ADD(2);
    inIndex          += hr->quotient;
    hr->distance[ch] += hr->remainder;


    ADD(1); BRANCH(1);
    if(hr->distance[ch] >= hr->L) {

      ADD(2);
      hr->distance[ch] -= hr->L;
      inIndex      += 1;
    }

    assert(outIndex < inIndex); /* make sure that we can work inplace */

    MOVE(1);
    ioBuffer[outIndex++] =  accu;

    assert(outIndex <= inSamples);
  }

  MOVE(1);
  *outSamples = outIndex;

  ADD(2); STORE(1);
  hr->position[ch] = inIndex - (inSamples-3);

  if (hr->position[ch] < 0) {
    assert (hr->position[ch] >= 0);
  }

  
  /* update buffers */
  ADD(1); BRANCH(1);
  if (inSamples > 3) {

    PTR_INIT(2); /* hr->oldSamples[][]
                    ioBuffer[]
                 */
    LOOP(1);
    for (i=0; i<3; i++) {
      MOVE(1);
      hr->oldSamples[i][ch] = ioBuffer[inSamples-3+i];
    }
  }


  /* Do the IIR filtering*/

  PTR_INIT(1); /* ioBuffer[] */

  MULT(2); ADD(1); STORE(1);
  ioBuffer[0] = hr->iirFilterCoeff_b*ioBuffer[0] - hr->oldSampleFilter[ch]*hr->iirFilterCoeff_a;

  LOOP(1);
  for(i=1; i < outIndex; i++){
    MULT(2); ADD(1); STORE(1);
    ioBuffer[i] = hr->iirFilterCoeff_b*ioBuffer[i] - ioBuffer[i-1]*hr->iirFilterCoeff_a;
  }

  MOVE(1);
  hr->oldSampleFilter[ch] = ioBuffer[outIndex - 1];


  COUNT_sub_end();
}
