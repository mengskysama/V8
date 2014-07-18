/*
  Parametric Stereo bitstream processing and decoding
*/
#include "huff_dec.h"
#include "ps_bitdec.h"
#include "ps_dec.h"
#include "sbr_rom.h"
#include "counters.h"


static const int aNoIidBins[3] = {NO_LOW_RES_IID_BINS, NO_IID_BINS, NO_HI_RES_BINS};
static const int aNoIccBins[3] = {NO_LOW_RES_ICC_BINS, NO_ICC_BINS, NO_HI_RES_BINS};

static const int aFixNoEnvDecode[4] = {0, 1, 2, 4};

static int
limitMinMax(int i,
            int min,
            int max)
{
  int result = i;

  COUNT_sub_start( "limitMinMax" );
                                                                                MOVE( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
  if (i<min){
    result = min;                                                               MOVE( 1 );
  }
  else {                                                                        ADD( 1 ); BRANCH( 1 );
    if (i>max) {
      result = max;                                                             MOVE( 1 );
    }
  }

  COUNT_sub_end();

  return result;
}

static void
deltaDecodeArray(int enable,
                 int *aIndex,
                 int *aPrevFrameIndex,
                 int DtDf,
                 int nrElements,
                 int stride,
                 int minIdx,
                 int maxIdx)
{
  int i;

  COUNT_sub_start( "deltaDecodeArray" );

                                                                                BRANCH( 1 );
  if ( enable==1 ) {                                                            BRANCH( 1 );
    if (DtDf == 0)  {
      aIndex[0] = 0 + aIndex[0];                                                PTR_INIT( 1 );
      aIndex[0] = limitMinMax(aIndex[0],minIdx,maxIdx);                         FUNC( 3 ); STORE( 1 );
                                                                                LOOP( 1 );
      for (i = 1; i < nrElements; i++) {
        aIndex[i] = aIndex[i-1] + aIndex[i];                                    ADD( 1 ); STORE( 1 );
        aIndex[i] = limitMinMax(aIndex[i],minIdx,maxIdx);                       FUNC( 3 ); STORE( 1 );
      }
    }
    else {                                                                      PTR_INIT( 2 ); LOOP( 1 );
      for (i = 0; i < nrElements; i++) {
        aIndex[i] = aPrevFrameIndex[i*stride] + aIndex[i];                      ADD( 1 ); STORE( 1 );
        aIndex[i] = limitMinMax(aIndex[i],minIdx,maxIdx);                       FUNC( 3 ); STORE( 1 );
      }
    }
  }
  else {                                                                        PTR_INIT( 1 ); LOOP( 1 );
    for (i = 0; i < nrElements; i++) {
      aIndex[i] = 0;                                                            MOVE( 1 );
    }
  }
                                                                                ADD( 1 ); BRANCH( 1 );
  if (stride==2) {                                                              MULT( 1 ); ADD( 1 ); PTR_INIT( 2 ); LOOP( 1 );
    for (i=nrElements*stride-1; i>0; i--) {
      aIndex[i] = aIndex[i/stride];                                             MOVE( 1 );
    }
  }
  COUNT_sub_end();
}

static void map34IndexTo20 (int *aIndex)
{
  PTR_INIT(1); /* aIndex[] */

  aIndex[0] =   (2*aIndex[0]+aIndex[1])/3;                                      MULT(2); ADD(1);
  aIndex[1] =	  (aIndex[1]+2*aIndex[2])/3;                                      MULT(2); ADD(1);
  aIndex[2] =	  (2*aIndex[3]+aIndex[4])/3;                                      MULT(2); ADD(1);
  aIndex[3] =	  (aIndex[4]+2*aIndex[5])/3;                                      MULT(2); ADD(1);
  aIndex[4] =	  (aIndex[6]+aIndex[7])/2;                                        MULT(1); ADD(1);
  aIndex[5] =	  (aIndex[8]+aIndex[9])/2;                                        MULT(1); ADD(1);
  aIndex[6] =	  aIndex[10];                                                     MOVE(1);
  aIndex[7] =	  aIndex[11];                                                     MOVE(1);
  aIndex[8] =	  (aIndex[12]+aIndex[13])/2;                                      MULT(1); ADD(1);
  aIndex[9] =	  (aIndex[14]+aIndex[15])/2;                                      MULT(1); ADD(1);
  aIndex[10] =	aIndex[16];                                                     MOVE(1);
  aIndex[11] =  aIndex[17];                                                     MOVE(1);
  aIndex[12] =	aIndex[18];                                                     MOVE(1);
  aIndex[13] =	aIndex[19];                                                     MOVE(1);
  aIndex[14] =	(aIndex[20]+aIndex[21])/2;                                      MULT(1); ADD(1);
  aIndex[15] =	(aIndex[22]+aIndex[23])/2;                                      MULT(1); ADD(1);
  aIndex[16] =	(aIndex[24]+aIndex[25])/2;                                      MULT(1); ADD(1);
  aIndex[17] =	(aIndex[26]+aIndex[27])/2;                                      MULT(1); ADD(1);
  aIndex[18] =	(aIndex[28]+aIndex[29]+aIndex[30]+aIndex[31])/4;                MULT(1); ADD(3);
  aIndex[19] =	(aIndex[32]+aIndex[33])/2;                                      MULT(1); ADD(1);
}

/***************************************************************************/
/*
  \brief  Decodes parametric stereo

  \return none

****************************************************************************/
void
DecodePs(struct PS_DEC *h_ps_dec)
{
  int gr, env;
  int noIidSteps;

  COUNT_sub_start( "DecodePs" );


                                                                               INDIRECT( 1 ); BRANCH( 1 );
  if (!h_ps_dec->bPsDataAvail) {
    h_ps_dec->noEnv = 0;                                                       INDIRECT( 1 ); MOVE( 1 );
  }

  noIidSteps = h_ps_dec->bFineIidQ?NO_IID_STEPS_FINE:NO_IID_STEPS;             INDIRECT( 1 ); BRANCH( 1 ); MOVE( 1 );
                                                                               PTR_INIT( 6 ); INDIRECT( 1 ); LOOP( 1 );
  for (env=0; env<h_ps_dec->noEnv; env++) {
    int *aPrevIidIndex;
    int *aPrevIccIndex;
                                                                               BRANCH( 1 );
    if (env==0) {
      aPrevIidIndex = h_ps_dec->aIidPrevFrameIndex;                            PTR_INIT( 1 );
      aPrevIccIndex = h_ps_dec->aIccPrevFrameIndex;                            PTR_INIT( 1 );
    }
    else {
      aPrevIidIndex = h_ps_dec->aaIidIndex[env-1];                             PTR_INIT( 1 );
      aPrevIccIndex = h_ps_dec->aaIccIndex[env-1];                             PTR_INIT( 1 );
    }

    deltaDecodeArray(h_ps_dec->bEnableIid,
                     h_ps_dec->aaIidIndex[env],
                     aPrevIidIndex,
                     h_ps_dec->abIidDtFlag[env],
                     aNoIidBins[h_ps_dec->freqResIid],
                     (h_ps_dec->freqResIid)?1:2,
                     -noIidSteps,
                     noIidSteps); 
                                                                               INDIRECT( 4 ); BRANCH( 1 ); MOVE( 2 ); FUNC( 8 );

    deltaDecodeArray(h_ps_dec->bEnableIcc,
                     h_ps_dec->aaIccIndex[env],
                     aPrevIccIndex,
                     h_ps_dec->abIccDtFlag[env],
                     aNoIccBins[h_ps_dec->freqResIcc],
                     (h_ps_dec->freqResIcc)?1:2,
                     0,
                     NO_ICC_STEPS-1);
                                                                               INDIRECT( 4 ); BRANCH( 1 ); MOVE( 2 ); FUNC( 8 );

  }   /* for (env=0; env<h_ps_dec->noEnv; env++) */

                                                                                INDIRECT( 1 ); BRANCH( 1 );
  if (h_ps_dec->noEnv==0) {
    h_ps_dec->noEnv = 1;                                                       INDIRECT( 1 ); MOVE( 1 );
                                                                               INDIRECT( 1 ); BRANCH( 1 );
    if (h_ps_dec->bEnableIid) {                                                PTR_INIT( 2 ); LOOP( 1 );
      for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
        h_ps_dec->aaIidIndex[h_ps_dec->noEnv-1][gr] =
          h_ps_dec->aIidPrevFrameIndex[gr];                                    MOVE( 1 );
      }
    }
    else {                                                                    PTR_INIT( 1 ); LOOP( 1 );
      for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
        h_ps_dec->aaIidIndex[h_ps_dec->noEnv-1][gr] = 0;                      MOVE( 1 );
      }
    }
                                                                               INDIRECT( 1 ); BRANCH( 1 );
    if (h_ps_dec->bEnableIcc) {                                                PTR_INIT( 2 ); LOOP( 1 );
      for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
        h_ps_dec->aaIccIndex[h_ps_dec->noEnv-1][gr] =
          h_ps_dec->aIccPrevFrameIndex[gr];                                    MOVE( 1 );
      }
    }
    else {                                                                    PTR_INIT( 1 ); LOOP( 1 );
      for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
        h_ps_dec->aaIccIndex[h_ps_dec->noEnv-1][gr] = 0;                      MOVE( 1 );
      }
    }
  }

                                                                              PTR_INIT( 2 ); LOOP( 1 );
  for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
    h_ps_dec->aIidPrevFrameIndex[gr] =
      h_ps_dec->aaIidIndex[h_ps_dec->noEnv-1][gr];                            MOVE( 1 );
  }
                                                                                PTR_INIT( 2 ); LOOP( 1 );
  for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
    h_ps_dec->aIccPrevFrameIndex[gr] =
      h_ps_dec->aaIccIndex[h_ps_dec->noEnv-1][gr];                            MOVE( 1 );
  }


  h_ps_dec->bPsDataAvail = 0;                                                  INDIRECT( 1 ); MOVE( 1 );

                                                                               INDIRECT( 1 ); BRANCH( 1 );
  if (h_ps_dec->bFrameClass == 0) {
    int shift;
                                                                                INDIRECT( 1 ); BRANCH( 1 );
    switch (h_ps_dec->noEnv){
    case 1:
      shift = 0;                                                                MOVE( 1 );
      break;
    case 2:
      shift = 1;                                                                MOVE( 1 );
      break;
    case 4:
      shift = 2;                                                                MOVE( 1 );
      break;
    }
    h_ps_dec->aEnvStartStop[0] = 0;                                            INDIRECT( 1 ); MOVE( 1 );
                                                                                PTR_INIT( 1 ); INDIRECT( 3 ); LOOP( 1 );
    for (env=1; env<h_ps_dec->noEnv; env++) {
      h_ps_dec->aEnvStartStop[env] =
        (env * h_ps_dec->noSubSamples) >> shift;                               SHIFT( 1 ); MULT( 1 ); STORE( 1 );
    }
    h_ps_dec->aEnvStartStop[h_ps_dec->noEnv] = h_ps_dec->noSubSamples;       INDIRECT( 3 ); MOVE( 1 );
  }
  else {   /* if (h_ps_dec->bFrameClass == 0) */
    h_ps_dec->aEnvStartStop[0] = 0;                                            INDIRECT( 1 ); MOVE( 1 );

                                                                                INDIRECT( 3 ); BRANCH( 1 );
    if (h_ps_dec->aEnvStartStop[h_ps_dec->noEnv] <
        (int)h_ps_dec->noSubSamples) {
      h_ps_dec->noEnv++;                                                       INDIRECT( 1 ); ADD( 1 ); STORE( 1 );
      h_ps_dec->aEnvStartStop[h_ps_dec->noEnv] = h_ps_dec->noSubSamples;     INDIRECT( 3 ); MOVE( 1 );
                                                                                PTR_INIT( 2 ); LOOP( 1 );
      for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
        h_ps_dec->aaIidIndex[h_ps_dec->noEnv-1][gr] =
          h_ps_dec->aaIidIndex[h_ps_dec->noEnv-2][gr];                        MOVE( 1 );
      }
                                                                                PTR_INIT( 2 ); LOOP( 1 );
      for (gr = 0; gr < NO_HI_RES_BINS; gr++) {
        h_ps_dec->aaIccIndex[h_ps_dec->noEnv-1][gr] =
          h_ps_dec->aaIccIndex[h_ps_dec->noEnv-2][gr];                        MOVE( 1 );
      }
    }

                                                                              INDIRECT( 2 ); ADD( 1 ); PTR_INIT( 1 ); LOOP( 1 );
    for (env=1; env<h_ps_dec->noEnv; env++) {
      int thr;
      thr = h_ps_dec->noSubSamples - h_ps_dec->noEnv + env;                   ADD( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
      if (h_ps_dec->aEnvStartStop[env] > thr) {
        h_ps_dec->aEnvStartStop[env] = thr;                                    MOVE( 1 );
      }
      else {
        thr = h_ps_dec->aEnvStartStop[env-1]+1;                                ADD( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
        if (h_ps_dec->aEnvStartStop[env] < thr) {
          h_ps_dec->aEnvStartStop[env] = thr;                                  MOVE( 1 );
        }
      }
    }
  }   /* if (h_ps_dec->bFrameClass == 0) ... else */

                                                                              INDIRECT( 1 ); ADD( 1 ); LOOP( 1 );
  for (env=0; env<h_ps_dec->noEnv; env++) {
                                                                              INDIRECT( 1 ); ADD( 1 ); BRANCH( 1 );
    if (h_ps_dec->freqResIid == 2)
    {
      map34IndexTo20 (h_ps_dec->aaIidIndex[env]);                             FUNC( 1 ); INDIRECT( 1 );
    }
                                                                              INDIRECT( 1 ); ADD( 1 ); BRANCH( 1 );
    if (h_ps_dec->freqResIcc == 2)
    {
      map34IndexTo20 (h_ps_dec->aaIccIndex[env]);                             FUNC( 1 ); INDIRECT( 1 );
    }
  }

  COUNT_sub_end();

}


/***************************************************************************/
/*

  \brief  Reads IID and ICC data from bitstream

****************************************************************************/
unsigned int
ReadPsData (HANDLE_PS_DEC h_ps_dec,
            HANDLE_BIT_BUFFER hBitBuf,
            int nBitsLeft
           )
{
  int     gr, env;
  int     dtFlag;
  int     startbits;
  Huffman CurrentTable;
  int     bEnableHeader;

  COUNT_sub_start( "ReadPsData" );
                                                                                BRANCH( 1 );
  if (!h_ps_dec) {
    COUNT_sub_end();
    return 0;
  }

  startbits = GetNrBitsAvailable(hBitBuf);                                     FUNC( 1 );

  bEnableHeader = (int) getbits (hBitBuf, 1);                                  FUNC( 2 );

                                                                               BRANCH( 1 );
  if (bEnableHeader) {
    h_ps_dec->bEnableIid = (int) getbits (hBitBuf, 1);                         FUNC( 2 ); INDIRECT( 1 ); STORE( 1 );

                                                                               BRANCH( 1 );
    if (h_ps_dec->bEnableIid) {
      h_ps_dec->freqResIid = (int) getbits (hBitBuf, 3);                       FUNC( 2 ); INDIRECT( 1 ); STORE( 1 );

                                                                               BRANCH( 1 ); ADD( 1 );
      if (h_ps_dec->freqResIid > 2){
        h_ps_dec->bFineIidQ = 1;                                               INDIRECT( 1 ); MOVE( 1 );
        h_ps_dec->freqResIid -=3;                                              INDIRECT( 1 ); ADD( 1 );
      }
      else{
        h_ps_dec->bFineIidQ = 0;                                               INDIRECT( 1 ); MOVE( 1 );
      }
    }

    h_ps_dec->bEnableIcc = (int) getbits (hBitBuf, 1);                         FUNC( 2 ); INDIRECT( 1 ); STORE( 1 );
                                                                                BRANCH( 1 );
    if (h_ps_dec->bEnableIcc) {
      h_ps_dec->freqResIcc = (int) getbits (hBitBuf, 3);                       FUNC( 2 ); INDIRECT( 1 ); STORE( 1 );

                                                                               BRANCH( 1 ); ADD( 1 );
      if (h_ps_dec->freqResIcc > 2){
        h_ps_dec->freqResIcc -=3;                                              INDIRECT( 1 ); ADD( 1 );
      }
    }
    h_ps_dec->bEnableExt = (int) getbits (hBitBuf, 1);                         FUNC( 2 ); INDIRECT( 1 ); STORE( 1 );
  }

  h_ps_dec->bFrameClass = (int) getbits (hBitBuf, 1);                          FUNC( 2 ); INDIRECT( 1 ); STORE( 1 );
                                                                                BRANCH( 1 );
  if (h_ps_dec->bFrameClass == 0) {
    h_ps_dec->noEnv = aFixNoEnvDecode[(int) getbits (hBitBuf, 2)];             FUNC( 2 ); INDIRECT( 2 ); MOVE( 1 );
  }
  else {
    h_ps_dec->noEnv = 1+(int) getbits (hBitBuf, 2);                            FUNC( 2 ); ADD( 1 ); INDIRECT( 1 ); STORE( 1 );
                                                                                PTR_INIT( 1 ); INDIRECT( 1 ); ADD( 1 ); LOOP( 1 );
    for (env=1; env<h_ps_dec->noEnv+1; env++) {
      h_ps_dec->aEnvStartStop[env] = ((int) getbits (hBitBuf, 5)) + 1;           FUNC( 2 ); ADD( 1 ); STORE( 1 );
    }
  }

                                                                                INDIRECT( 2 ); BRANCH( 1 );
  if ((h_ps_dec->freqResIid > 2) || (h_ps_dec->freqResIcc > 2)) {

    h_ps_dec->bPsDataAvail = 0;                                                STORE( 1 );

    nBitsLeft -= startbits - GetNrBitsAvailable(hBitBuf);                       FUNC( 1 ); ADD( 1 ); STORE( 1 );
                                                                                LOOP( 1 );
    while (nBitsLeft) {
      int i = nBitsLeft;                                                        STORE( 1 );
                                                                                ADD( 1 ); BRANCH( 1 );
      if (i>8) {
        i = 8;                                                                  STORE( 1 );
      }
      getbits (hBitBuf, i);                                                     FUNC( 2 );
      nBitsLeft -= i;                                                           ADD( 1 ); STORE( 1 );
    }
    COUNT_sub_end();
    return (startbits - GetNrBitsAvailable(hBitBuf));
  }

                                                                                INDIRECT( 1 ); BRANCH( 1 );
  if (h_ps_dec->bEnableIid) {
                                                                                PTR_INIT( 2 ); INDIRECT( 1 ); LOOP( 1 );
    for (env=0; env<h_ps_dec->noEnv; env++) {
      dtFlag = (int)getbits (hBitBuf, 1);                                       FUNC( 2 );
                                                                                BRANCH( 1 );
      if (!dtFlag) {
                                                                                BRANCH( 1 ); INDIRECT( 1 );
        if (h_ps_dec->bFineIidQ){
          CurrentTable = (Huffman)&aBookPsIidFineFreqDecode;                    PTR_INIT( 1 );
        }
        else {
          CurrentTable = (Huffman)&aBookPsIidFreqDecode;                        PTR_INIT( 1 );
        }
      }
      else {
                                                                                BRANCH( 1 ); INDIRECT( 1 );
        if (h_ps_dec->bFineIidQ){
          CurrentTable = (Huffman)&aBookPsIidFineTimeDecode;                    PTR_INIT( 1 );
        }
        else {
          CurrentTable = (Huffman)&aBookPsIidTimeDecode;                        PTR_INIT( 1 );
        }
      }

                                                                                PTR_INIT( 1 ); INDIRECT( 2 ); LOOP( 1 );
      for (gr = 0; gr < aNoIidBins[h_ps_dec->freqResIid]; gr++) {
                                                                                
        h_ps_dec->aaIidIndex[env][gr] = DecodeHuffmanCW(CurrentTable,hBitBuf); FUNC( 2 ); STORE( 1 );
      }
      h_ps_dec->abIidDtFlag[env] = dtFlag;                                     MOVE( 1 );
    }
  }

                                                                                INDIRECT( 1 ); BRANCH( 1 );
  if (h_ps_dec->bEnableIcc) {
                                                                                PTR_INIT( 2 ); INDIRECT( 1 ); LOOP( 1 );
    for (env=0; env<h_ps_dec->noEnv; env++) {
      dtFlag = (int)getbits (hBitBuf, 1);                                       FUNC( 2 );
                                                                                BRANCH( 1 );
      if (!dtFlag) {
        CurrentTable = (Huffman)&aBookPsIccFreqDecode;                          PTR_INIT( 1 );
      }
      else {
        CurrentTable = (Huffman)&aBookPsIccTimeDecode;                          PTR_INIT( 1 );
      }
                                                                                PTR_INIT( 1 ); INDIRECT( 2 ); LOOP( 1 );
      for (gr = 0; gr < aNoIccBins[h_ps_dec->freqResIcc]; gr++) {
        h_ps_dec->aaIccIndex[env][gr] = DecodeHuffmanCW(CurrentTable,hBitBuf); FUNC( 2 ); STORE( 1 );
      }
      h_ps_dec->abIccDtFlag[env] = dtFlag;                                     MOVE( 1 );
    }
  }

                                                                                INDIRECT( 1 ); BRANCH( 1 );
  if (h_ps_dec->bEnableExt) {

    int cnt, i;

    cnt = (int)getbits (hBitBuf, 4);                                            FUNC( 2 );
                                                                                ADD( 1 ); BRANCH( 1 );
    if (cnt==15)
    {
      cnt += (int)getbits (hBitBuf, 8);                                         FUNC( 2 ); ADD( 1 );
    }

    LOOP(1);
    for (i=0; i<cnt; i++)
    {
      getbits(hBitBuf, 8);                                                      FUNC(2);
    }
  }

  h_ps_dec->bPsDataAvail = 1;                                                  INDIRECT( 1 ); MOVE( 1 );
                                                                                FUNC( 1 ); ADD( 1 );
  COUNT_sub_end();

  return (startbits - GetNrBitsAvailable(hBitBuf));
}
