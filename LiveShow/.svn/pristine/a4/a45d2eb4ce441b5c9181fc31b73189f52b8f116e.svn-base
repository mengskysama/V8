/*
  Frequency scale calculation
*/
#include <math.h>
#include <string.h>
#include <assert.h>
#include "sbr_rom.h"
#include "sbr_const.h"
#include "env_extr.h"
#include "freq_sca.h"
#include "FloatFR.h"

#include "counters.h" /* the 3GPP instrumenting tool */

#define MAX_OCTAVE         29
#define MAX_SECOND_REGION  50


static int  numberOfBands(float bpo, int start, int stop, int warpFlag);
static void CalcBands(unsigned char * diff, unsigned char start, unsigned char stop, unsigned char num_bands);
static int  modifyBands(unsigned char max_band, unsigned char * diff, unsigned char length);
static void cumSum(unsigned char start_value, unsigned char* diff, unsigned char length, unsigned char *start_adress);


/*!
  \brief     Retrieve QMF-band where the SBR range starts

  \return  Number of start band
*/
static unsigned char
getStartBand(unsigned short fs,
             unsigned char  startFreq)
{
  int band;

  COUNT_sub_start("getStartBand");

    switch(fs) {
    case 48000:
      INDIRECT(1); MOVE(1);
      band = sbr_start_freq_48[startFreq];
      break;
    case 44100:
      INDIRECT(1); MOVE(1);
      band = sbr_start_freq_44[startFreq];
      break;
    case 32000:
      INDIRECT(1); MOVE(1);
      band = sbr_start_freq_32[startFreq];
      break;
    case 24000:
      INDIRECT(1); MOVE(1);
      band = sbr_start_freq_24[startFreq];
      break;
    case 22050:
      INDIRECT(1); MOVE(1);
      band = sbr_start_freq_22[startFreq];
      break;
    case 16000:
      INDIRECT(1); MOVE(1);
      band = sbr_start_freq_16[startFreq];
      break;
    default:
      MOVE(1);
      band = -1;
    }

  COUNT_sub_end();

  return band;
}


/*!
  \brief     Generates master frequency tables

  \return  errorCode, 0 if successful
*/
int
sbrdecUpdateFreqScale(unsigned char * v_k_master,
                      unsigned char *numMaster,
                      SBR_HEADER_DATA * hHeaderData)
{
  int      err=0;
  unsigned short fs = hHeaderData->outSampleRate;
  float    bpo;
  int      dk=0;

  unsigned char     k0, k2, k1=0, i;
  unsigned char     num_bands0;
  unsigned char     num_bands1;
  unsigned char     diff_tot[MAX_OCTAVE + MAX_SECOND_REGION];
  unsigned char     *diff0 = diff_tot;
  unsigned char     *diff1 = diff_tot+MAX_OCTAVE;
  int     k2_achived;
  int     k2_diff;
  int     incr=0;

  COUNT_sub_start("sbrdecUpdateFreqScale");

  INDIRECT(1); ADD(1); MOVE(6); /* counting previous operations */

  INDIRECT(1); FUNC(2);
  k0 = getStartBand(fs, hHeaderData->startFreq);


  INDIRECT(1); ADD(1); BRANCH(1);
  if(hHeaderData->stopFreq<14) {

    switch(fs) {
    case 48000:
      MOVE(1);
      k1 = 21;
      break;
    case 44100:
      MOVE(1);
      k1 = 23;
      break;
    case 32000:
    case 24000:
      MOVE(1);
      k1 = 32;
      break;
    case 22050:
      MOVE(1);
      k1 = 35;
      break;
    case 16000:
      MOVE(1);
      k1 = 48;
      break;
    default:
      COUNT_sub_end();
      return -1;
    }

    FUNC(4);
    CalcBands( diff0, k1, 64, 13 );

    FUNC(2);
    shellsort( diff0, 13 );

    FUNC(4);
    cumSum(k1, diff0, 13, diff1);

    INDIRECT(1); MOVE(1);
    k2 = diff1[hHeaderData->stopFreq];
  }
  else {
    INDIRECT(1); ADD(1); BRANCH(1);
    if(hHeaderData->stopFreq==14) {
    MULT(1);
    k2=2*k0;
    }
  else
  {
    MULT(1);
    k2=3*k0;
  }
  }

  ADD(1); BRANCH(1);
  if (k2 > NO_SYNTHESIS_CHANNELS)
  {
    MOVE(1);
    k2 = NO_SYNTHESIS_CHANNELS;
  }


  ADD(3); LOGIC(1); BRANCH(1);
  if ( ((k2 - k0) > MAX_FREQ_COEFFS) || (k2 <= k0) ) {
    COUNT_sub_end();
    return -1;
  }

  ADD(3); LOGIC(1); BRANCH(1);
  if ( (fs == 44100) && ( (k2 - k0) > MAX_FREQ_COEFFS_FS44100 ) ) {
    COUNT_sub_end();
    return -1;
  }

  ADD(3); LOGIC(1); BRANCH(1);
  if ( (fs >= 48000) && ( (k2 - k0) > MAX_FREQ_COEFFS_FS48000 ) ) {
    COUNT_sub_end();
    return -1;
  }


  INDIRECT(1); ADD(1); BRANCH(1);
  if(hHeaderData->freqScale>0) {

    INDIRECT(1); ADD(1); BRANCH(1);
    if(hHeaderData->freqScale==1) {

      MOVE(1);
      bpo = 12.0f;
    }
    else {
      INDIRECT(1); ADD(1); BRANCH(1);
      if(hHeaderData->freqScale==2) {
      MOVE(1);
      bpo = 10.0f;
    }
    else {
      MOVE(1);
      bpo =  8.0f;
    }
    }


    MULT(2); ADD(1); BRANCH(1);
    if( 1000 * k2 > 2245 * k0 ) {
      MULT(1);
      k1 = 2*k0;

      FUNC(4);
      num_bands0 = numberOfBands(bpo, k0, k1, 0);

      INDIRECT(1); FUNC(4);
      num_bands1 = numberOfBands(bpo, k1, k2, hHeaderData->alterScale );

      ADD(1); BRANCH(1);
      if ( num_bands0 < 1) {
        COUNT_sub_end();
        return -1;
      }

      ADD(1); BRANCH(1);
      if ( num_bands1 < 1 ) {
        COUNT_sub_end();
        return -1;
      }

      FUNC(4);
      CalcBands(diff0, k0, k1, num_bands0);

      FUNC(2);
      shellsort( diff0, num_bands0);

      BRANCH(1);
      if (diff0[0] == 0) {
        COUNT_sub_end();
        return -1;
      }

      FUNC(4);
      cumSum(k0, diff0, num_bands0, v_k_master);

      FUNC(4);
      CalcBands(diff1, k1, k2, num_bands1);

      FUNC(2);
      shellsort( diff1, num_bands1);

      INDIRECT(1); ADD(1); BRANCH(1);
      if(diff0[num_bands0-1] > diff1[0]) {

        FUNC(3);
        err = modifyBands(diff0[num_bands0-1],diff1, num_bands1);

        BRANCH(1);
        if (err)
        {
          COUNT_sub_end();
          return -1;
        }
      }

      INDIRECT(1); PTR_INIT(1); FUNC(4);
      cumSum(k1, diff1, num_bands1, &v_k_master[num_bands0]);

      ADD(1); STORE(1);
      *numMaster = num_bands0 + num_bands1;

    }
    else {
      MOVE(1);
      k1=k2;

      FUNC(4);
      num_bands0 = numberOfBands(bpo, k0, k1, 0);

      ADD(1); BRANCH(1);
      if ( num_bands0 < 1) {
        COUNT_sub_end();
        return -1;
      }

      FUNC(4);
      CalcBands(diff0, k0, k1, num_bands0);

      FUNC(2);
      shellsort(diff0, num_bands0);

      BRANCH(1);
      if (diff0[0] == 0) {
        COUNT_sub_end();
        return -1;
      }

      FUNC(4);
      cumSum(k0, diff0, num_bands0, v_k_master);

      MOVE(1);
      *numMaster = num_bands0;

    }
  }
  else {

     INDIRECT(1); BRANCH(1);
     if (hHeaderData->alterScale==0) {

        MOVE(1);
        dk = 1;
        ADD(1); LOGIC(1);
        num_bands0 = (k2 - k0) & 254;
      } else {
        MOVE(1);
        dk = 2;

        ADD(2); SHIFT(1); LOGIC(1);
        num_bands0 = ( ((k2 - k0) >> 1) + 1 ) & 254;
      }

      ADD(1); BRANCH(1);
      if (num_bands0 < 1) {
        COUNT_sub_end();
        return -1;
      }

      MULT(1); ADD(1);
      k2_achived = k0 + num_bands0*dk;

      ADD(1);
      k2_diff = k2 - k2_achived;

      PTR_INIT(1); /* diff_tot[i] */
      LOOP(1);
      for(i=0;i<num_bands0;i++)
      {
        MOVE(1);
        diff_tot[i] = dk;
      }

      BRANCH(1);
      if (k2_diff < 0) {

          MOVE(2);
          incr = 1;
          i = 0;
      }

      BRANCH(1);
      if (k2_diff > 0) {

          MOVE(1);
          incr = -1;

          ADD(1);
          i = num_bands0-1;
      }

      PTR_INIT(1); /* diff_tot[i] */
      LOOP(1);
      while (k2_diff != 0) {

        ADD(1); STORE(1);
        diff_tot[i] = diff_tot[i] - incr;

        ADD(1);
        i = i + incr;

        ADD(1);
        k2_diff = k2_diff + incr;
      }

      FUNC(4);
      cumSum(k0, diff_tot, num_bands0, v_k_master);/* cumsum */

    MOVE(1);
    *numMaster = num_bands0;
  }

  ADD(1); BRANCH(1);
  if (*numMaster < 1) {
    COUNT_sub_end();
    return -1;
  }

  COUNT_sub_end();

  return 0;
}


/*!
  \brief     Calculate number of SBR bands between start and stop band

  \return    number of bands
*/
static int
numberOfBands(float  bpo,
              int    start,
              int    stop,
              int    warpFlag)
{
  float num_bands_div2;
  int   num_bands;

  COUNT_sub_start("numberOfBands");

  FUNC(2); MULT(2);
  num_bands_div2 = 0.5f * FloatFR_getNumOctaves(start,stop) * bpo;

  BRANCH(1);
  if (warpFlag) {
    MULT(1);
    num_bands_div2 *= (25200.0f/32768.0f);
  }

  ADD(1);
  num_bands_div2 += 0.5f;
  MULT(1);
  num_bands = 2 * (int)num_bands_div2;

  COUNT_sub_end();

  return(num_bands);
}


/*!
  \brief     Calculate width of SBR bands

*/
static void
CalcBands(unsigned char * diff,
          unsigned char start,
          unsigned char stop,
          unsigned char num_bands)
{
  int i;
  int previous;
  int current;
  float exact = (float)start;
  float bandfactor = (float) pow( (float)stop * sbr_invIntTable[start], sbr_invIntTable[num_bands] );

  COUNT_sub_start("CalcBands");

  INDIRECT(2); MULT(1); TRANS(1); MOVE(1); /* counting previous operations */

  MOVE(1);
  previous=start;

  PTR_INIT(1); /* diff[] */
  LOOP(1);
  for(i=1; i<=num_bands; i++)  {

    MULT(1);
    exact *= bandfactor;

    ADD(1);
    current = (int)(exact + 0.5f);

    ADD(1); STORE(1);
    diff[i-1]=current-previous;

    MOVE(1);
    previous=current;
  }

  COUNT_sub_end();
}


/*!
  \brief     Calculate cumulated sum vector from delta vector
*/
static void
cumSum(unsigned char start_value, unsigned char* diff, unsigned char length, unsigned char *start_adress)
{
  int i;

  COUNT_sub_start("cumSum");

  MOVE(1);
  start_adress[0]=start_value;

  PTR_INIT(2); /* start_adress[]
                  diff[]
               */
  LOOP(1);
  for(i=1; i<=length; i++)
  {
    ADD(1); STORE(1);
    start_adress[i] = start_adress[i-1] + diff[i-1];
  }

  COUNT_sub_end();
}


/*!
  \brief     Adapt width of frequency bands in the second region

*/
static int
modifyBands(unsigned char max_band_previous, unsigned char * diff, unsigned char length)
{
  int change = max_band_previous - diff[0];

  COUNT_sub_start("modifyBands");

  ADD(1); /* counting previous operations */

  INDIRECT(1); ADD(2); DIV(1); BRANCH(1);
  if ( change > (diff[length-1]-diff[0])/2 )
  {
    MOVE(1);
    change = (diff[length-1]-diff[0])/2;
  }

  ADD(2); STORE(2);
  diff[0] += change;
  diff[length-1] -= change;

  FUNC(2);
  shellsort(diff, length);

  COUNT_sub_end();

  return 0;
}


/*!
  \brief   Update high resolution frequency band table
*/
void
sbrdecUpdateHiRes(unsigned char * h_hires,
                  unsigned char * num_hires,
                  unsigned char * v_k_master,
                  unsigned char num_bands,
                  unsigned char xover_band)
{
  unsigned char i;

  COUNT_sub_start("sbrdecUpdateHiRes");

  ADD(1); STORE(1);
  *num_hires = num_bands-xover_band;

  PTR_INIT(2); /* h_hires[]
                  v_k_master[]
               */
  LOOP(1);
  for(i=xover_band; i<=num_bands; i++) {

    MOVE(1);
    h_hires[i-xover_band] = v_k_master[i];
  }

  COUNT_sub_end();
}


/*!
  \brief  Build low resolution table out of high resolution table
*/
void
sbrdecUpdateLoRes(unsigned char * h_lores,
                  unsigned char * num_lores,
                  unsigned char * h_hires,
                  unsigned char num_hires)
{
  unsigned char i;

  COUNT_sub_start("sbrdecUpdateLoRes");

  LOGIC(1); BRANCH(1);
  if( (num_hires & 1) == 0) {
    SHIFT(1); STORE(1);
    *num_lores = num_hires >> 1;

    PTR_INIT(2); /* h_lores[]
                    h_hires[]
                 */
    LOOP(1);
    for(i=0; i<=*num_lores; i++)
    {
      MOVE(1);
      h_lores[i] = h_hires[i*2];
    }
  }
  else {
    ADD(1); SHIFT(1); STORE(1);
    *num_lores = (num_hires+1) >> 1;

    MOVE(1);
    h_lores[0] = h_hires[0];

    PTR_INIT(2); /* h_lores[]
                    h_hires[]
                 */
    LOOP(1);
    for(i=1; i<=*num_lores; i++) {

      MOVE(1);
      h_lores[i] = h_hires[i*2-1];
    }
  }

  COUNT_sub_end();
}


/*!
  \brief   Derive a low-resolution frequency-table from the master frequency table
*/
void
sbrdecDownSampleLoRes(unsigned char *v_result,
                      unsigned char num_result,
                      unsigned char *freqBandTableRef,
                      unsigned char num_Ref)
{
  int step;
  int i,j;
  int org_length,result_length;
  int v_index[MAX_FREQ_COEFFS/2];

  COUNT_sub_start("sbrdecDownSampleLoRes");

  /* init */
  MOVE(2);
  org_length = num_Ref;
  result_length = num_result;

  MOVE(2);
  v_index[0] = 0;
  i=0;

  PTR_INIT(1); /* v_index[] */
  LOOP(1);
  while(org_length > 0) {
    ADD(1);
    i++;

    DIV(1);
    step = org_length / result_length;

    ADD(1);
    org_length = org_length - step;

    ADD(1);
    result_length--;

    ADD(1); STORE(1);
    v_index[i] = v_index[i-1] + step;
  }

  PTR_INIT(2); /* v_result[]
                  v_index[]
               */
  LOOP(1);
  for(j=0;j<=i;j++) {
    INDIRECT(1); MOVE(1);
    v_result[j]=freqBandTableRef[v_index[j]];
  }

  COUNT_sub_end();
}


/*!
  \brief   Sorting routine
*/
void shellsort(unsigned char *in, unsigned char n)
{

  int i, j, v, w;
  int inc = 1;

  COUNT_sub_start("shellsort");

  MOVE(1); /* counting previous operations */

  LOOP(1);
  do
  {
    MULT(1); ADD(1);
    inc = 3 * inc + 1;
  }
  while (inc <= n);

  LOOP(1);
  do {

    DIV(1);
    inc = inc / 3;

    PTR_INIT(1); /* in[i] */
    LOOP(1);
    for (i = inc; i < n; i++) {

      MOVE(2);
      v = in[i];
      j = i;

      PTR_INIT(2); /* in[j-inc]
                      in[j]
                   */
      LOOP(1);
      while ((w=in[j-inc]) > v) {

        MOVE(1);
        in[j] = w;

        ADD(1);
        j -= inc;

        ADD(1); BRANCH(1);
        if (j < inc)
          break;
      }

      MOVE(1);
      in[j] = v;
    }
  } while (inc > 1);

  COUNT_sub_end();
}



/*!
  \brief   Reset frequency band tables
  \return  errorCode, 0 if successful
*/
int
resetFreqBandTables(HANDLE_SBR_HEADER_DATA hHeaderData)
{
  int err;
  int k2,kx, lsb, usb;
  int     intTemp;
  unsigned char    nBandsLo, nBandsHi;
  HANDLE_FREQ_BAND_DATA hFreq = hHeaderData->hFreqBandData;

  COUNT_sub_start("resetFreqBandTables");

  INDIRECT(1); PTR_INIT(1); /* counting previous operations */

  INDIRECT(2); PTR_INIT(1); FUNC(3);
  err = sbrdecUpdateFreqScale(hFreq->v_k_master,
                              &hFreq->numMaster,
                              hHeaderData);

  INDIRECT(2); ADD(1); LOGIC(1); BRANCH(1);
  if ( err || (hHeaderData->xover_band > hFreq->numMaster) ) {
    COUNT_sub_end();
    return -1;
  }

  INDIRECT(4); PTR_INIT(1); FUNC(5);
  sbrdecUpdateHiRes(hFreq->freqBandTable[HI], &nBandsHi, hFreq->v_k_master, hFreq->numMaster, hHeaderData->xover_band );

  INDIRECT(2); PTR_INIT(1); FUNC(4);
  sbrdecUpdateLoRes(hFreq->freqBandTable[LO], &nBandsLo, hFreq->freqBandTable[HI], nBandsHi);


  INDIRECT(2); MOVE(2);
  hFreq->nSfb[LO] = nBandsLo;
  hFreq->nSfb[HI] = nBandsHi;

  ADD(2); LOGIC(1); BRANCH(1);
  if ( (nBandsLo <= 0) || (nBandsLo > MAX_FREQ_COEFFS / 2) ) {
    COUNT_sub_end();
    return -1;
  }

  INDIRECT(2); MOVE(2);
  lsb = hFreq->freqBandTable[LOW_RES][0];
  usb = hFreq->freqBandTable[LOW_RES][nBandsLo];

  ADD(2); LOGIC(1);
  if ( (lsb > NO_ANALYSIS_CHANNELS) || (lsb >= usb) ) {
    COUNT_sub_end();
    return -1;
  }


  INDIRECT(2); MOVE(2);
  k2 = hFreq->freqBandTable[HI][nBandsHi];
  kx = hFreq->freqBandTable[HI][0];

  INDIRECT(1); BRANCH(1);
  if (hHeaderData->noise_bands == 0)
  {
    INDIRECT(1); MOVE(1);
    hFreq->nNfb = 1;
  }
  else
  {
    float temp;

    FUNC(2);
    temp = FloatFR_getNumOctaves(kx,k2);

    INDIRECT(1); MULT(1);
    temp = temp * (float)hHeaderData->noise_bands;

    ADD(1);
    intTemp = (int)(temp + 0.5f);

    assert( intTemp ==  (int)((hHeaderData->noise_bands * log( (float)k2/kx) / (float)(log(2.0)))+0.5) );

    BRANCH(1);
    if( intTemp==0)
    {
      MOVE(1);
      intTemp=1;
    }

    INDIRECT(1); MOVE(1);
    hFreq->nNfb = intTemp;
  }

  INDIRECT(2); MOVE(1);
  hFreq->nInvfBands = hFreq->nNfb;

  INDIRECT(1); ADD(1); BRANCH(1);
  if( hFreq->nNfb > MAX_NOISE_COEFFS ) {
    COUNT_sub_end();
    return -1;
  }

  INDIRECT(3); FUNC(4);
  sbrdecDownSampleLoRes(hFreq->freqBandTableNoise,
                        hFreq->nNfb,
                        hFreq->freqBandTable[LO],
                        nBandsLo);

  INDIRECT(2); MOVE(2);
  hFreq->lowSubband  = lsb;
  hFreq->highSubband = usb;

  COUNT_sub_end();

  return 0;
}
