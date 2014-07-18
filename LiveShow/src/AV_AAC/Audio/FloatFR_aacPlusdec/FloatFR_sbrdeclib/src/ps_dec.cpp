/*
  Parametric stereo decoding
*/
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sbr_rom.h"
#include "sbr_ram.h"
#include "ps_dec.h"
#include "counters.h"

#include <assert.h>
#include "FloatFR.h"
#include "aac_ram.h"

static void deCorrelate( HANDLE_PS_DEC h_ps_dec,
                         float *rIntBufferLeft, float *iIntBufferLeft,
                         float *rIntBufferRight, float *iIntBufferRight);

static void applyRotation( HANDLE_PS_DEC pms,
                           float *qmfLeftReal , float *qmfLeftImag,
                           float *qmfRightReal, float *qmfRightImag );

/***************************************************************************/
/*
  \brief  Creates one instance of the PS_DEC struct

  \return Error info

****************************************************************************/
int
CreatePsDec(unsigned int keyflag,
			HANDLE_PS_DEC h_ps_dec,
            unsigned int noSubSamples)
{
  int i, j;
  int *pErr;
  float *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7;

  const int pHybridResolution[] = { HYBRID_8_CPLX,
                                    HYBRID_2_REAL,
                                    HYBRID_2_REAL };

  COUNT_sub_start("CreatePsDec");

  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(sizeof(struct PS_DEC));
  memset(h_ps_dec,0,sizeof(struct PS_DEC));

   /* initialisation */
  INDIRECT(1); MOVE(1);
  h_ps_dec->noSubSamples = noSubSamples;

  INDIRECT(1); DIV(1); STORE(1);
  h_ps_dec->invNoSubSamples = 1.0f / noSubSamples;

  INDIRECT(7); MOVE(7);
  h_ps_dec->bPsDataAvail = 0;
  h_ps_dec->bEnableIid = 0;
  h_ps_dec->bEnableIcc = 0;
  INDIRECT(1); MOVE(1);
  h_ps_dec->bEnableExt = 0;
  h_ps_dec->bFineIidQ  = 0;
  h_ps_dec->freqResIid = 0;
  h_ps_dec->freqResIcc = 0;
  h_ps_dec->lastUsb = 0;

  PTR_INIT(7);
  ptr1 = getSbrOverlapBuffer(keyflag,MAXNRSBRCHANNELS-1,0)/*&sbr_OverlapBuffer[MAXNRSBRCHANNELS-1][0]*/;
  ptr2 = getOverlapBuffer(keyflag,OverlapBufferSize)/*&OverlapBuffer[OverlapBufferSize]*/;
  ptr3 = getSbrCodecQmfStatesAnalysis(keyflag,QMF_FILTER_STATE_ANA_SIZE);/*&sbr_CodecQmfStatesAnalysis[QMF_FILTER_STATE_ANA_SIZE];*/
  ptr4 = getSbrGainSmooth(keyflag,MAXNRSBRCHANNELS-1,0);/*&sbr_GainSmooth[MAXNRSBRCHANNELS-1][0]*/
  ptr5 = getSbrNoiseSmooth(keyflag,MAXNRSBRCHANNELS-1,0)/*&sbr_NoiseSmooth[MAXNRSBRCHANNELS-1][0]*/;
  ptr6 = getSbrLpcFilterStatesReal(keyflag,MAXNRSBRCHANNELS-1,0,0)/*&sbr_LpcFilterStatesReal[MAXNRSBRCHANNELS-1][0][0]*/;
  ptr7 = getSbrLpcFilterStatesImag(keyflag,MAXNRSBRCHANNELS-1,0,0)/*&sbr_LpcFilterStatesImag[MAXNRSBRCHANNELS-1][0][0]*/;

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->aPeakDecayFast = ptr1;ptr1+=NO_BINS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(NO_BINS);
  memset(h_ps_dec->aPeakDecayFast,0,sizeof(float)*NO_BINS);

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->aPrevNrg = ptr1;ptr1+=NO_BINS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(NO_BINS);
  memset(h_ps_dec->aPrevNrg,0,sizeof(float)*NO_BINS);

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->aPrevPeakDiff = ptr1;ptr1+=NO_BINS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(NO_BINS);
  memset(h_ps_dec->aPrevPeakDiff,0,sizeof(float)*NO_BINS);


  INDIRECT(1); PTR_INIT(2); FUNC(4);
  pErr = (int*)CreateHybridFilterBank ( &h_ps_dec->hHybrid,
                                        NO_QMF_CHANNELS_IN_HYBRID,
                                        pHybridResolution,
                                        &ptr1);

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->mHybridRealLeft = ptr1;ptr1+=SUBQMF_GROUPS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(SUBQMF_GROUPS);
  memset(h_ps_dec->mHybridRealLeft,0,sizeof(float)*SUBQMF_GROUPS);

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->mHybridImagLeft = ptr1;ptr1+=SUBQMF_GROUPS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(SUBQMF_GROUPS);
  memset(h_ps_dec->mHybridImagLeft,0,sizeof(float)*SUBQMF_GROUPS);

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->mHybridRealRight = ptr1;ptr1+=SUBQMF_GROUPS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(SUBQMF_GROUPS);
  memset(h_ps_dec->mHybridRealRight,0,sizeof(float)*SUBQMF_GROUPS);

  INDIRECT(1); PTR_INIT(1); ADD(1);
  h_ps_dec->mHybridImagRight = ptr1;ptr1+=SUBQMF_GROUPS;
  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(SUBQMF_GROUPS);
  memset(h_ps_dec->mHybridImagRight,0,sizeof(float)*SUBQMF_GROUPS);

  INDIRECT(1); MOVE(1);
  h_ps_dec->delayBufIndex   = 0;

  PTR_INIT(2); /* h_ps_dec->aDelayBufIndex[]
                  h_ps_dec->aNoSampleDelay[]
               */
  LOOP(1);
  for (i=0 ; i < NO_DELAY_CHANNELS ; i++) {
    MOVE(1);
    h_ps_dec->aDelayBufIndex[i] = 0;

    ADD(1); BRANCH(1);
    if (i<SHORT_DELAY_START)
    {
      MOVE(1);
      h_ps_dec->aNoSampleDelay[i] = LONG_DELAY;
    }
    else
    {
      MOVE(1);
      h_ps_dec->aNoSampleDelay[i] = SHORT_DELAY;
    }
  }

  INDIRECT(4); PTR_INIT(4); ADD(4);
  h_ps_dec->aaRealDelayBufferQmf = (float **)ptr6;ptr6+= NO_QMF_ICC_CHANNELS * sizeof (float *)/sizeof(float);
  h_ps_dec->aaImagDelayBufferQmf = (float **)ptr7;ptr7+= NO_QMF_ICC_CHANNELS * sizeof (float *)/sizeof(float);
  h_ps_dec->aaRealDelayBufferSubQmf = (float **)ptr1;ptr1+= SUBQMF_GROUPS * sizeof (float *)/sizeof(float);
  h_ps_dec->aaImagDelayBufferSubQmf = (float **)ptr1;ptr1+= SUBQMF_GROUPS * sizeof (float *)/sizeof(float);

  PTR_INIT(3); /* h_ps_dec->aaRealDelayBufferQmf[]
                  h_ps_dec->aaImagDelayBufferQmf[]
                  h_ps_dec->aNoSampleDelay[]
               */
  LOOP(1);
  for (i = 0; i < NO_QMF_ICC_CHANNELS; i++) {
    int delay;

    ADD(1); BRANCH(1);
    if (i < NO_QMF_ALLPASS_CHANNELS){

      MOVE(1);
      delay = 2;

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaRealDelayBufferQmf[i] = ptr4;ptr4+= delay;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(delay);
      memset(h_ps_dec->aaRealDelayBufferQmf[i],0,delay*sizeof(float));

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaImagDelayBufferQmf[i] = ptr5;ptr5+= delay;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(delay);
      memset(h_ps_dec->aaImagDelayBufferQmf[i],0,delay*sizeof(float));
    }
    else{

      MOVE(1);
      delay = h_ps_dec->aNoSampleDelay[i-NO_QMF_ALLPASS_CHANNELS];

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaRealDelayBufferQmf[i] = ptr1;ptr1+= delay;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(delay);
      memset(h_ps_dec->aaRealDelayBufferQmf[i],0,delay*sizeof(float));

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaImagDelayBufferQmf[i] = ptr1;ptr1+= delay;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(delay);
      memset(h_ps_dec->aaImagDelayBufferQmf[i],0,delay*sizeof(float));
    }
  }

  PTR_INIT(2); /* h_ps_dec->aaRealDelayBufferSubQmf[]
                  h_ps_dec->aaImagDelayBufferSubQmf[]
               */
  INDIRECT(1);
  LOOP(1);
  for (i = 0; i < SUBQMF_GROUPS; i++) {

    PTR_INIT(1); ADD(1);
    h_ps_dec->aaRealDelayBufferSubQmf[i] = ptr1;ptr1+= DELAY_ALLPASS;

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(DELAY_ALLPASS);
    memset(h_ps_dec->aaRealDelayBufferSubQmf[i],0,DELAY_ALLPASS*sizeof(float));

    PTR_INIT(1); ADD(1);
    h_ps_dec->aaImagDelayBufferSubQmf[i] = ptr1;ptr1+= DELAY_ALLPASS;

    FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(DELAY_ALLPASS);
    memset(h_ps_dec->aaImagDelayBufferSubQmf[i],0,DELAY_ALLPASS*sizeof(float));
  }

  PTR_INIT(7); /* h_ps_dec->aDelayRBufIndexSer[]
                  h_ps_dec->aNoSampleDelayRSer[]
                  h_ps_dec->aaaRealDelayRBufferSerQmf[]
                  h_ps_dec->aaaImagDelayRBufferSerQmf[]
                  h_ps_dec->aaaRealDelayRBufferSerSubQmf[]
                  h_ps_dec->aaaImagDelayRBufferSerSubQmf[]
                  aRevLinkDelaySer[]
               */
  LOOP(1);
  for (i=0 ; i < NO_SERIAL_ALLPASS_LINKS ; i++) {

    MOVE(2);
    h_ps_dec->aDelayRBufIndexSer[i] = 0;
    h_ps_dec->aNoSampleDelayRSer[i] = aRevLinkDelaySer[i];

    PTR_INIT(4); MULT(4); ADD(4);
    h_ps_dec->aaaRealDelayRBufferSerQmf[i] = (float **)ptr2;ptr2+= h_ps_dec->aNoSampleDelayRSer[i] * sizeof (float *)/sizeof(float);
    h_ps_dec->aaaImagDelayRBufferSerQmf[i] = (float **)ptr2;ptr2+= h_ps_dec->aNoSampleDelayRSer[i] * sizeof (float *)/sizeof(float);
    h_ps_dec->aaaRealDelayRBufferSerSubQmf[i] = (float **)ptr3;ptr3+= h_ps_dec->aNoSampleDelayRSer[i] * sizeof (float *)/sizeof(float);
    h_ps_dec->aaaImagDelayRBufferSerSubQmf[i] = (float **)ptr3;ptr3+= h_ps_dec->aNoSampleDelayRSer[i] * sizeof (float *)/sizeof(float);

    PTR_INIT(4); /* h_ps_dec->aaaRealDelayRBufferSerQmf[][]
                    h_ps_dec->aaaImagDelayRBufferSerQmf[][]
                    h_ps_dec->aaaRealDelayRBufferSerSubQmf[][]
                    h_ps_dec->aaaImagDelayRBufferSerSubQmf[][]
                 */
    LOOP(1);
    for (j = 0; j < h_ps_dec->aNoSampleDelayRSer[i]; j++) {

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaaRealDelayRBufferSerQmf[i][j] = ptr2;ptr2+= NO_QMF_ALLPASS_CHANNELS;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(NO_QMF_ALLPASS_CHANNELS);
      memset(h_ps_dec->aaaRealDelayRBufferSerQmf[i][j],0,NO_QMF_ALLPASS_CHANNELS*sizeof(float));

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaaImagDelayRBufferSerQmf[i][j] = ptr2;ptr2+= NO_QMF_ALLPASS_CHANNELS;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(NO_QMF_ALLPASS_CHANNELS);
      memset(h_ps_dec->aaaImagDelayRBufferSerQmf[i][j],0,NO_QMF_ALLPASS_CHANNELS*sizeof(float));

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaaRealDelayRBufferSerSubQmf[i][j] = ptr3;ptr3+= SUBQMF_GROUPS;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(SUBQMF_GROUPS);
      memset(h_ps_dec->aaaRealDelayRBufferSerSubQmf[i][j],0,SUBQMF_GROUPS*sizeof(float));

      PTR_INIT(1); ADD(1);
      h_ps_dec->aaaImagDelayRBufferSerSubQmf[i][j] = ptr3;ptr3+= SUBQMF_GROUPS;

      FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(SUBQMF_GROUPS);
      memset(h_ps_dec->aaaImagDelayRBufferSerSubQmf[i][j],0,SUBQMF_GROUPS*sizeof(float));
    }
  }

  PTR_INIT(2); /* pms->h11Prev[]
                  pms->h12Prev[]
               */
  LOOP(1);
  for ( i = 0; i < NO_IID_GROUPS; i++ )
  {
    MOVE(2);
    h_ps_dec->h11Prev[i] = 1.0f;
    h_ps_dec->h12Prev[i] = 1.0f;
  }

  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(sizeof(h_ps_dec->h21Prev));
  memset( h_ps_dec->h21Prev, 0, sizeof( h_ps_dec->h21Prev ) );

  FUNC(3); LOOP(1); INDIRECT(2); PTR_INIT(1); MOVE(1); STORE(sizeof(h_ps_dec->h22Prev));
  memset( h_ps_dec->h22Prev, 0, sizeof( h_ps_dec->h22Prev ) );

  COUNT_sub_end();

  return 0;
} /*END CreatePsDec*/


/***************************************************************************/
/*
  \brief  Applies IID, ICC,
****************************************************************************/
void
ApplyPsSlot(HANDLE_PS_DEC h_ps_dec,
            float **rIntBufferLeft,
            float **iIntBufferLeft,
            float *rIntBufferRight,
            float *iIntBufferRight
            )
{
  COUNT_sub_start( "ApplyPsSlot" );

  HybridAnalysis ( (const float**)rIntBufferLeft,
                   (const float**)iIntBufferLeft,
                   &h_ps_dec->mHybridRealLeft,
                   &h_ps_dec->mHybridImagLeft,
                   h_ps_dec->hHybrid );                                        FUNC( 5 );


  deCorrelate(h_ps_dec,
              *rIntBufferLeft,
              *iIntBufferLeft,
              rIntBufferRight,
              iIntBufferRight);                                                 FUNC( 5 );

  applyRotation(h_ps_dec,
                *rIntBufferLeft,
                *iIntBufferLeft,
                rIntBufferRight,
                iIntBufferRight);                                               FUNC( 5 );

  HybridSynthesis ( (const float**)&h_ps_dec->mHybridRealLeft,
                    (const float**)&h_ps_dec->mHybridImagLeft,
                    rIntBufferLeft,
                    iIntBufferLeft,
                    h_ps_dec->hHybrid );                                       INDIRECT( 3 ); FUNC( 5 );

  HybridSynthesis ( (const float**)&h_ps_dec->mHybridRealRight,
                    (const float**)&h_ps_dec->mHybridImagRight,
                    &rIntBufferRight,
                    &iIntBufferRight,
                    h_ps_dec->hHybrid );                                       INDIRECT( 2 ); FUNC( 5 );

  COUNT_sub_end();

}/* END ApplyPsSlot */



/***************************************************************************/
/*
  \brief  deCorrelate

****************************************************************************/
static void
deCorrelate(HANDLE_PS_DEC h_ps_dec,
            float *rIntBufferLeft,
            float *iIntBufferLeft,
            float *rIntBufferRight,
            float *iIntBufferRight
            )
{
  int sb, maxsb, gr, sb_delay, sbOffset, bin;
  int m;
  float iInputLeft;
  float rInputLeft;

  float peakDiff, nrg;
  float aPower[NO_BINS];
  float aTransRatio[NO_BINS];

  float *aLeftReal;
  float *aLeftImag;
  float *aRightReal;
  float *aRightImag;

  float ***pppRealDelayRBufferSer;
  float ***pppImagDelayRBufferSer;
  float **ppRealDelayBuffer;
  float **ppImagDelayBuffer;

  float *pFractDelayPhaseFactorRe;
  float *pFractDelayPhaseFactorIm;
  float **ppFractDelayPhaseFactorSerRe;
  float **ppFractDelayPhaseFactorSerIm;
  int   usb;

  COUNT_sub_start( "deCorrelate" );

  usb = h_ps_dec->usb;                                                         INDIRECT ( 1 );

  aLeftReal = h_ps_dec->mHybridRealLeft;                                       PTR_INIT( 1 );
  aLeftImag = h_ps_dec->mHybridImagLeft;                                       PTR_INIT( 1 );
  aRightReal = h_ps_dec->mHybridRealRight;                                     PTR_INIT( 1 );
  aRightImag = h_ps_dec->mHybridImagRight;                                     PTR_INIT( 1 );

  pppRealDelayRBufferSer = h_ps_dec->aaaRealDelayRBufferSerSubQmf;             PTR_INIT( 1 );
  pppImagDelayRBufferSer = h_ps_dec->aaaImagDelayRBufferSerSubQmf;             PTR_INIT( 1 );

  ppRealDelayBuffer = h_ps_dec->aaRealDelayBufferSubQmf;                       PTR_INIT( 1 );
  ppImagDelayBuffer = h_ps_dec->aaImagDelayBufferSubQmf;                       PTR_INIT( 1 );

  pFractDelayPhaseFactorRe = (float*)aFractDelayPhaseFactorReSubQmf;            PTR_INIT( 1 );
  pFractDelayPhaseFactorIm = (float*)aFractDelayPhaseFactorImSubQmf;            PTR_INIT( 1 );

  ppFractDelayPhaseFactorSerRe = (float**)aaFractDelayPhaseFactorSerReSubQmf;   PTR_INIT( 1 );
  ppFractDelayPhaseFactorSerIm = (float**)aaFractDelayPhaseFactorSerImSubQmf;   PTR_INIT( 1 );

  sbOffset = 0;                                                                 MOVE( 1 );
                                                                                PTR_INIT( 1 ); LOOP( 1 );
  for (bin=0; bin < NO_BINS; bin++) {
    aPower[bin] = 0;                                                            MOVE( 1 );
  }
                                                                                LOOP( 1 );
  for (gr=0; gr < NO_IID_GROUPS; gr++) {
    bin = ( ~NEGATE_IPD_MASK ) & bins2groupMap[gr];                             LOGIC( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
    if (gr == SUBQMF_GROUPS) {
      aLeftReal = rIntBufferLeft;                                               PTR_INIT( 1 );
      aLeftImag = iIntBufferLeft;                                               PTR_INIT( 1 );
    }
                                                                                ADD( 1 ); BRANCH( 1 );
    if (gr < SUBQMF_GROUPS) {
      maxsb = groupBorders[gr]+1;                                               ADD( 1 );
    }
    else {
      maxsb = min(usb, groupBorders[gr+1]);                                     ADD(1); BRANCH(1); MOVE(1);
    }

                                                                                PTR_INIT( 5 ); LOOP( 1 );
    for (sb = groupBorders[gr]; sb < maxsb; sb++) {
      aPower[bin] += aLeftReal[sb]*aLeftReal[sb] +
                     aLeftImag[sb]*aLeftImag[sb];                               MAC( 2 ); STORE( 2 );
    } /* sb */
  } /* gr */

  aLeftReal = h_ps_dec->mHybridRealLeft;                                       PTR_INIT( 1 );
  aLeftImag = h_ps_dec->mHybridImagLeft;                                       PTR_INIT( 1 );

  COUNT_sub_start( "deCorrelate_transDet" );
                                                                                PTR_INIT( 4 ); INDIRECT( 1 ); MOVE( 1 ); LOOP( 1 );
  for (bin=0; bin < NO_BINS; bin++) {
    h_ps_dec->aPeakDecayFast[bin] *= PEAK_DECAY_FACTOR;                        MULT( 1 ); STORE( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
    if (h_ps_dec->aPeakDecayFast[bin] < aPower[bin]) {
      h_ps_dec->aPeakDecayFast[bin] = aPower[bin];                             MOVE( 1 );
    }

    peakDiff = h_ps_dec->aPrevPeakDiff[bin] +
               (h_ps_dec->aPeakDecayFast[bin] - aPower[bin] - h_ps_dec->aPrevPeakDiff[bin]) *
               NRG_INT_COEFF;                                                   ADD( 2 ); MAC( 1 );
    h_ps_dec->aPrevPeakDiff[bin] = peakDiff;                                   MOVE( 1 );

                                                                                ADD( 1 ); MAC( 1 );
    nrg = h_ps_dec->aPrevNrg[bin] + (aPower[bin] - h_ps_dec->aPrevNrg[bin])*NRG_INT_COEFF;
    h_ps_dec->aPrevNrg[bin] = nrg;                                             MOVE( 1 );

    peakDiff *= 1.5f;                                                           MULT( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
    if (peakDiff <= nrg) {
      aTransRatio[bin] = 1.0f;                                                  MOVE( 1 );
    }
    else {
      aTransRatio[bin] = nrg / peakDiff;                                        DIV( 1 ); STORE( 1 );
    }
  } /* bin */
  COUNT_sub_end();

                                                                                LOOP( 1 );
  for (gr=0; gr < NO_IID_GROUPS; gr++) {
    bin = ( ~NEGATE_IPD_MASK ) & bins2groupMap[gr];                             LOGIC( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
    if (gr == SUBQMF_GROUPS) {
      aLeftReal = rIntBufferLeft;                                               PTR_INIT( 1 );
      aLeftImag = iIntBufferLeft;                                               PTR_INIT( 1 );
      aRightReal = rIntBufferRight;                                             PTR_INIT( 1 );
      aRightImag = iIntBufferRight;                                             PTR_INIT( 1 );

      pppRealDelayRBufferSer = h_ps_dec->aaaRealDelayRBufferSerQmf;            PTR_INIT( 1 );
      pppImagDelayRBufferSer = h_ps_dec->aaaImagDelayRBufferSerQmf;            PTR_INIT( 1 );

      ppRealDelayBuffer = h_ps_dec->aaRealDelayBufferQmf;                      PTR_INIT( 1 );
      ppImagDelayBuffer = h_ps_dec->aaImagDelayBufferQmf;                      PTR_INIT( 1 );

      pFractDelayPhaseFactorRe = (float*)aFractDelayPhaseFactorReQmf;           PTR_INIT( 1 );
      pFractDelayPhaseFactorIm = (float*)aFractDelayPhaseFactorImQmf;           PTR_INIT( 1 );

      ppFractDelayPhaseFactorSerRe = (float**)aaFractDelayPhaseFactorSerReQmf;  PTR_INIT( 1 );
      ppFractDelayPhaseFactorSerIm = (float**)aaFractDelayPhaseFactorSerImQmf;  PTR_INIT( 1 );

      sbOffset = NO_QMF_CHANNELS_IN_HYBRID;                                     MOVE( 1 );
    }
                                                                                ADD( 1 ); BRANCH( 1 );
    if (gr < SUBQMF_GROUPS) {
      maxsb = groupBorders[gr]+1;                                               ADD( 1 );
    }
    else {
      maxsb = min(usb, groupBorders[gr+1]);                                     ADD(1); BRANCH(1); MOVE(1);
    }
                                                                                PTR_INIT( 2 ); LOOP( 1 );
    for (sb = groupBorders[gr]; sb < maxsb; sb++) {
      float decayScaleFactor;
      float rTmp, iTmp, rTmp0, iTmp0, rIn, iIn;

      sb_delay = sb - sbOffset;                                                 ADD( 1 );

      rInputLeft = aLeftReal[sb];                                               MOVE( 1 );
      iInputLeft = aLeftImag[sb];                                               MOVE( 1 );
                                                                                ADD( 2 ); LOGIC( 1 ); BRANCH( 1 );
      if (gr >= SUBQMF_GROUPS && sb >= NO_ALLPASS_CHANNELS) {
        COUNT_sub_start( "deCorrelate_delay" );

                                                                                INDIRECT( 2 ); MULT( 1 ); ADD( 1 ); MOVE( 1 );
        rIn = ppRealDelayBuffer[sb_delay][h_ps_dec->aDelayBufIndex[sb - NO_ALLPASS_CHANNELS]];
                                                                                INDIRECT( 1 ); MOVE( 1 );
        iIn = ppImagDelayBuffer[sb_delay][h_ps_dec->aDelayBufIndex[sb - NO_ALLPASS_CHANNELS]];
                                                                                INDIRECT( 1 ); MOVE( 1 );
        ppRealDelayBuffer[sb_delay][h_ps_dec->aDelayBufIndex[sb - NO_ALLPASS_CHANNELS]] = rInputLeft;
                                                                                INDIRECT( 1 ); MOVE( 1 );
        ppImagDelayBuffer[sb_delay][h_ps_dec->aDelayBufIndex[sb - NO_ALLPASS_CHANNELS]] = iInputLeft;
                                                                                INDIRECT( 1 ); ADD( 2 ); BRANCH( 1 );
        if (++h_ps_dec->aDelayBufIndex[sb - NO_ALLPASS_CHANNELS] >=
            h_ps_dec->aNoSampleDelay[sb - NO_ALLPASS_CHANNELS]) {
          h_ps_dec->aDelayBufIndex[sb - NO_ALLPASS_CHANNELS] = 0;          MOVE( 1 );
        }
        COUNT_sub_end();
      }
      else {
        COUNT_sub_start( "deCorrelate_allpass" );
                                                                                  ADD( 2 ); LOGIC( 1 ); BRANCH( 1 );
        if (gr < SUBQMF_GROUPS || sb <= DECAY_CUTOFF) {
          decayScaleFactor = 1.0f;                                                MOVE( 1 );
        }
        else {
          decayScaleFactor = ( 1.0f + DECAY_CUTOFF * DECAY_SLOPE ) - DECAY_SLOPE * sb;         MAC( 1 );
        }

        rIn = ppRealDelayBuffer[sb_delay][h_ps_dec->delayBufIndex];            INDIRECT( 2 ); MULT( 1 ); MOVE( 1 );
        iIn = ppImagDelayBuffer[sb_delay][h_ps_dec->delayBufIndex];            INDIRECT( 1 ); MOVE( 1 );

                                                                                MULT( 2 ); ADD( 1 );
        rTmp = rIn*pFractDelayPhaseFactorRe[sb_delay] - iIn*pFractDelayPhaseFactorIm[sb_delay];
                                                                                MULT( 1 ); MAC( 1 );
        iTmp = rIn*pFractDelayPhaseFactorIm[sb_delay] + iIn*pFractDelayPhaseFactorRe[sb_delay];
        rIn = rTmp;                                                             MOVE( 1 );
        iIn = iTmp;                                                             MOVE( 1 );

        ppRealDelayBuffer[sb_delay][h_ps_dec->delayBufIndex] = rInputLeft;     INDIRECT( 1 ); MOVE( 1 );
        ppImagDelayBuffer[sb_delay][h_ps_dec->delayBufIndex] = iInputLeft;     INDIRECT( 1 ); MOVE( 1 );
                                                                                PTR_INIT( 6 ); LOOP( 1 );
        for (m=0; m<NO_SERIAL_ALLPASS_LINKS ; m++) {
                                                                                PTR_INIT( 1 ); MOVE( 1 );
          rTmp0 = pppRealDelayRBufferSer[m][h_ps_dec->aDelayRBufIndexSer[m]][sb_delay];
                                                                                PTR_INIT( 1 ); MOVE( 1 );
          iTmp0 = pppImagDelayRBufferSer[m][h_ps_dec->aDelayRBufIndexSer[m]][sb_delay];

                                                                                MULT( 2 ); ADD( 1 );
          rTmp = rTmp0*ppFractDelayPhaseFactorSerRe[m][sb_delay] - iTmp0*ppFractDelayPhaseFactorSerIm[m][sb_delay];
                                                                                MULT( 1 ); MAC( 1 );
          iTmp = rTmp0*ppFractDelayPhaseFactorSerIm[m][sb_delay] + iTmp0*ppFractDelayPhaseFactorSerRe[m][sb_delay];

          rTmp += -decayScaleFactor * aRevLinkDecaySer[m] * rIn;        MULT( 1 ); MAC( 1 );
          iTmp += -decayScaleFactor * aRevLinkDecaySer[m] * iIn;        MULT( 1 ); MAC( 1 );
                                                                                ADD( 1 ); MULT( 1 ); MAC( 1 ); STORE( 1 );
          pppRealDelayRBufferSer[m][h_ps_dec->aDelayRBufIndexSer[m]][sb_delay] = rIn + decayScaleFactor * aRevLinkDecaySer[m] * rTmp;
                                                                                ADD( 1 ); MULT( 1 ); MAC( 1 ); STORE( 1 );
          pppImagDelayRBufferSer[m][h_ps_dec->aDelayRBufIndexSer[m]][sb_delay] = iIn + decayScaleFactor * aRevLinkDecaySer[m] * iTmp;

          rIn = rTmp;                                                           MOVE( 1 );
          iIn = iTmp;                                                           MOVE( 1 );
        }
        COUNT_sub_end();
      }

      aRightReal[sb] = aTransRatio[bin] * rIn;                                  MULT( 1 ); STORE( 1 );
      aRightImag[sb] = aTransRatio[bin] * iIn;                                  MULT( 1 ); STORE( 1 );
    } /* sb */
  } /* gr */

                                                                                INDIRECT( 1 ); ADD( 2 );
  if (++h_ps_dec->delayBufIndex >= DELAY_ALLPASS) {
    h_ps_dec->delayBufIndex = 0;                                               MOVE( 1 );
  }
                                                                                PTR_INIT( 2 ); LOOP( 1 );
  for (m=0; m<NO_SERIAL_ALLPASS_LINKS ; m++) {
                                                                                ADD( 2 );
    if (++h_ps_dec->aDelayRBufIndexSer[m] >= h_ps_dec->aNoSampleDelayRSer[m]) {
      h_ps_dec->aDelayRBufIndexSer[m] = 0;                                     MOVE( 1 );
    }
  }

  COUNT_sub_end();

} /* END deCorrelate */


/***************************************************************************/
/*
  \brief  Initialise rotation

****************************************************************************/
void
InitRotationEnvelope(HANDLE_PS_DEC pms,
                     int env,
                     int usb)
{
  int     group;
  int     bin;
  float   scaleL, scaleR;
  float   alpha, beta;
  float   h11, h12, h21, h22;
  float   invEnvLength;
  const float  *pScaleFactors;
  int     noIidSteps;

  COUNT_sub_start( "InitRotationEnvelope" );

                                                                                BRANCH(1); INDIRECT(1);
  if (pms->bFineIidQ)
  {
    noIidSteps = NO_IID_STEPS_FINE;                                             MOVE(1);
    pScaleFactors = scaleFactorsFine;                                           PTR_INIT(1);
  }
  else{
    noIidSteps = NO_IID_STEPS;                                                  MOVE(1);
    pScaleFactors = scaleFactors;                                               PTR_INIT(1);
  }

                                                                                BRANCH(1);
  if (env == 0)
  {
    PTR_INIT( 2 ); /* pms->usb
                      pms->lastUsb
                   */
    pms->lastUsb = pms->usb;                                                    MOVE(1);
    pms->usb = usb;                                                             MOVE(1);
                                                                                BRANCH(1); ADD(1); LOGIC(2);
    if (usb > pms->lastUsb && pms->lastUsb!=0)
    {
      int sb, i, k, kmax;
      kmax = 2;                                                                 MOVE(1);
                                                                                LOOP( 1 );
      for (sb = pms->lastUsb-NO_QMF_CHANNELS_IN_HYBRID; sb < usb-NO_QMF_CHANNELS_IN_HYBRID; sb++){
                                                                                BRANCH(1); ADD(1); LOGIC(1);
        if (sb<NO_QMF_ALLPASS_CHANNELS) {
                                                                                PTR_INIT( 3 ); LOOP( 2 );
          for (i=0 ; i<NO_SERIAL_ALLPASS_LINKS ; i++) {
            for (k=0 ; k < pms->aNoSampleDelayRSer[i]; k++) {
              pms->aaaRealDelayRBufferSerQmf[i][k][sb] = 0;                     MOVE(1);
              pms->aaaImagDelayRBufferSerQmf[i][k][sb] = 0;                     MOVE(1);
            }
          }
        }
        else {
          kmax = pms->aNoSampleDelay[sb-NO_QMF_ALLPASS_CHANNELS];               INDIRECT( 1 );
        }
                                                                                PTR_INIT( 2 ); LOOP( 1 );
        for (k=0 ; k < kmax; k++) {
          pms->aaRealDelayBufferQmf[sb][k] = 0;                                 MOVE(1);
          pms->aaImagDelayBufferQmf[sb][k] = 0;                                 MOVE(1);
        }
      }
    }
  }
                                                                                PTR_INIT( 1 ); ADD( 1 );
  invEnvLength = ( float )( pms->aEnvStartStop[env + 1] - pms->aEnvStartStop[env] );

                                                                                INDIRECT( 1 ); ADD( 1 ); BRANCH( 1 );
  if (invEnvLength == pms->noSubSamples){
    invEnvLength = pms->invNoSubSamples;                                        INDIRECT( 1 );
  }
  else{
    invEnvLength = 1.0f/invEnvLength;                                           DIV( 1 );
  }

                                                                                PTR_INIT( 10 ); LOOP( 1 );
  for ( group = 0; group < NO_IID_GROUPS; group++ )
  {
    bin = ( ~NEGATE_IPD_MASK ) & bins2groupMap[group];                          LOGIC( 1 );

    scaleR = pScaleFactors[noIidSteps + pms->aaIidIndex[env][bin]];             INDIRECT( 1 ); ADD( 1 ); MOVE( 1 );
    scaleL = pScaleFactors[noIidSteps - pms->aaIidIndex[env][bin]];             INDIRECT( 1 ); ADD( 1 ); MOVE( 1 );
    alpha  = alphas[pms->aaIccIndex[env][bin]];                                 INDIRECT( 1 ); MOVE( 1 );
    beta   = alpha * ( scaleR - scaleL ) / PSC_SQRT2F;                          ADD( 1 ); MULT( 2 );

    h11 = ( float )( scaleL * cos( beta + alpha ) );                            ADD( 1 ); TRANS( 1 ); MULT( 1 );
    h12 = ( float )( scaleR * cos( beta - alpha ) );                            ADD( 1 ); TRANS( 1 ); MULT( 1 );
    h21 = ( float )( scaleL * sin( beta + alpha ) );                            TRANS( 1 ); MULT( 1 );
    h22 = ( float )( scaleR * sin( beta - alpha ) );                            TRANS( 1 ); MULT( 1 );

    pms->deltaH11[group]  = ( h11 - pms->h11Prev[group] ) * invEnvLength;       INDIRECT( 1 ); ADD( 1 ); MULT( 1 ); STORE( 1 );
    pms->deltaH12[group]  = ( h12 - pms->h12Prev[group] ) * invEnvLength;       INDIRECT( 1 ); ADD( 1 ); MULT( 1 ); STORE( 1 );
    pms->deltaH21[group]  = ( h21 - pms->h21Prev[group] ) * invEnvLength;       INDIRECT( 1 ); ADD( 1 ); MULT( 1 ); STORE( 1 );
    pms->deltaH22[group]  = ( h22 - pms->h22Prev[group] ) * invEnvLength;       INDIRECT( 1 ); ADD( 1 ); MULT( 1 ); STORE( 1 );

    pms->H11[group]  = pms->h11Prev[group];                                     INDIRECT( 1 ); MOVE( 1 );
    pms->H12[group]  = pms->h12Prev[group];                                     INDIRECT( 1 ); MOVE( 1 );
    pms->H21[group]  = pms->h21Prev[group];                                     INDIRECT( 1 ); MOVE( 1 );
    pms->H22[group]  = pms->h22Prev[group];                                     INDIRECT( 1 ); MOVE( 1 );

    pms->h11Prev[group] = h11;                                                  MOVE( 1 );
    pms->h12Prev[group] = h12;                                                  MOVE( 1 );
    pms->h21Prev[group] = h21;                                                  MOVE( 1 );
    pms->h22Prev[group] = h22;                                                  MOVE( 1 );
  } /* groups loop */

  COUNT_sub_end();

} /* END InitRotationEnvelope */


/***************************************************************************/
/*
  \brief  Rotation

****************************************************************************/
static void
applyRotation( HANDLE_PS_DEC  pms,
               float            *qmfLeftReal,
               float            *qmfLeftImag,
               float            *qmfRightReal,
               float            *qmfRightImag
             )
{
  int     group;
  int     bin;
  int     subband, maxSubband;
  float  *hybrLeftReal;
  float  *hybrLeftImag;
  float  *hybrRightReal;
  float  *hybrRightImag;
  float   tempLeftReal, tempLeftImag;
  float   tempRightReal, tempRightImag;
  int     usb;

  COUNT_sub_start( "applyRotation" );

  usb = pms->usb;                                                               INDIRECT ( 1 );

  hybrLeftReal  = pms->mHybridRealLeft;                                         PTR_INIT( 1 );
  hybrLeftImag  = pms->mHybridImagLeft;                                         PTR_INIT( 1 );
  hybrRightReal = pms->mHybridRealRight;                                        PTR_INIT( 1 );
  hybrRightImag = pms->mHybridImagRight;                                        PTR_INIT( 1 );
                                                                                PTR_INIT( 12 ); LOOP( 1 );
  for ( group = 0; group < NO_IID_GROUPS; group++ )
  {
    bin = ( ~NEGATE_IPD_MASK ) & bins2groupMap[group];                          LOGIC( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
    if ( group == SUBQMF_GROUPS )
    {
      hybrLeftReal  = qmfLeftReal;                                              PTR_INIT( 1 );
      hybrLeftImag  = qmfLeftImag;                                              PTR_INIT( 1 );
      hybrRightReal = qmfRightReal;                                             PTR_INIT( 1 );
      hybrRightImag = qmfRightImag;                                             PTR_INIT( 1 );
    }
                                                                                ADD( 1 ); BRANCH( 1 );
    if ( group < SUBQMF_GROUPS )
    {
        maxSubband = groupBorders[group] + 1;                                   ADD( 1 );
    }
    else
    {
        maxSubband = min(usb, groupBorders[group + 1]);                         ADD(1); BRANCH(1); MOVE(1);
    }

    pms->H11[group]    += pms->deltaH11[group];                                 ADD( 1 ); STORE( 1 );
    pms->H12[group]    += pms->deltaH12[group];                                 ADD( 1 ); STORE( 1 );
    pms->H21[group]    += pms->deltaH21[group];                                 ADD( 1 ); STORE( 1 );
    pms->H22[group]    += pms->deltaH22[group];                                 ADD( 1 ); STORE( 1 );

                                                                                PTR_INIT( 4 ); LOOP( 1 );
    for ( subband = groupBorders[group]; subband < maxSubband; subband++ )
    {
                                                                                MULT( 1 ); MAC( 1 );
      tempLeftReal  = pms->H11[group] * hybrLeftReal[subband] + pms->H21[group] * hybrRightReal[subband];
                                                                                MULT( 1 ); MAC( 1 );
      tempLeftImag  = pms->H11[group] * hybrLeftImag[subband] + pms->H21[group] * hybrRightImag[subband];
                                                                                MULT( 1 ); MAC( 1 );
      tempRightReal = pms->H12[group] * hybrLeftReal[subband] + pms->H22[group] * hybrRightReal[subband];
                                                                                MULT( 1 ); MAC( 1 );
      tempRightImag = pms->H12[group] * hybrLeftImag[subband] + pms->H22[group] * hybrRightImag[subband];

      hybrLeftReal [subband] = tempLeftReal;                                    MOVE( 1 );
      hybrLeftImag [subband] = tempLeftImag;                                    MOVE( 1 );
      hybrRightReal[subband] = tempRightReal;                                   MOVE( 1 );
      hybrRightImag[subband] = tempRightImag;                                   MOVE( 1 );
    } /* subband loop */
  } /* groups loop */

  COUNT_sub_end();

} /* END applyRotation */
