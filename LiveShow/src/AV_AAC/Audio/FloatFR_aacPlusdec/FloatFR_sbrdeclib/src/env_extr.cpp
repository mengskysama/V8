/*
  Envelope extraction
*/
#include <string.h>
#include <assert.h>
#include "sbr_ram.h"
#include "sbr_rom.h"
#include "env_extr.h"
#include "huff_dec.h"
#include "sbr_const.h"       /* Various defines */
#include "FloatFR.h"
#include "ps_bitdec.h"
#include "ps_dec.h"

#include "counters.h" /* the 3GPP instrumenting tool */

static int extractFrameInfo (HANDLE_BIT_BUFFER hBitBuf,
                             HANDLE_SBR_HEADER_DATA hHeaderData,
                             HANDLE_SBR_FRAME_DATA h_frame_data);
static int sbrGetEnvelope (HANDLE_SBR_HEADER_DATA hHeaderData,
                           HANDLE_SBR_FRAME_DATA h_frame_data,
                           HANDLE_BIT_BUFFER hBitBuf);
static void sbrGetDirectionControlData (HANDLE_SBR_FRAME_DATA hFrameData,
                                        HANDLE_BIT_BUFFER hBitBuf);
static void sbrGetNoiseFloorData (HANDLE_SBR_HEADER_DATA hHeaderData,
                                  HANDLE_SBR_FRAME_DATA h_frame_data,
                                  HANDLE_BIT_BUFFER hBitBuf);
static int checkFrameInfo (FRAME_INFO *pFrameInfo, int numberOfTimeSlots);



/*
  \brief     Initialize SBR header data
*/
void
initHeaderData (unsigned int keyflag,
				SBR_HEADER_DATA *hHeaderData,
                int sampleRate,
                int samplesPerFrame)
{
  HANDLE_FREQ_BAND_DATA hFreq = getFreqBandData(keyflag)/*&FreqBandData*/;

  COUNT_sub_start("initHeaderData");

  PTR_INIT(1); MOVE(1); /* counting previous operation */

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(SBR_HEADER_DATA));
  memcpy(hHeaderData, &sbr_defaultHeader, sizeof(SBR_HEADER_DATA));

  INDIRECT(4); MOVE(2);
  hFreq->freqBandTable[LOW_RES]  = hFreq->freqBandTableLo;
  hFreq->freqBandTable[HIGH_RES] = hFreq->freqBandTableHi;

  INDIRECT(1); MOVE(1);
  hHeaderData->hFreqBandData = hFreq;

  INDIRECT(1); MOVE(1);
  hHeaderData->codecFrameSize = samplesPerFrame;

  INDIRECT(1); MULT(1); STORE(1);
  hHeaderData->outSampleRate = SBR_UPSAMPLE_FAC * sampleRate;

  INDIRECT(2); ADD(1); SHIFT(1); STORE(1);
  hHeaderData->numberTimeSlots = samplesPerFrame >> (4 + hHeaderData->timeStep);

  COUNT_sub_end();
}


/*
  \brief   Initialize the SBR_PREV_FRAME_DATA struct
*/
void
initSbrPrevFrameData (HANDLE_SBR_PREV_FRAME_DATA h_prev_data,
                      int timeSlots)
{
  int i;

  COUNT_sub_start("initSbrPrevFrameData");

  PTR_INIT(1); /* h_prev_data->sfb_nrg_prev[i] */
  LOOP(1);
  for (i=0; i < MAX_FREQ_COEFFS; i++)
  {
    MOVE(1);
    h_prev_data->sfb_nrg_prev[i] = 0.0f;
  }

  PTR_INIT(1); /* h_prev_data->prevNoiseLevel[i] */
  LOOP(1);
  for (i=0; i < MAX_NOISE_COEFFS; i++)
  {
    MOVE(1);
    h_prev_data->prevNoiseLevel[i] = 0.0f;
  }

  PTR_INIT(1); /* h_prev_data->sbr_invf_mode[i] */
  LOOP(1);
  for (i=0; i < MAX_INVF_BANDS; i++)
  {
    MOVE(1);
    h_prev_data->sbr_invf_mode[i] = INVF_OFF;
  }

  INDIRECT(4); MOVE(4);
  h_prev_data->stopPos = timeSlots;
  h_prev_data->coupling = COUPLING_OFF;
  h_prev_data->ampRes = 0;
  h_prev_data->xposCtrl = -1;

  COUNT_sub_end();
}


/*
  \brief   Read header data from bitstream

  \return  error status - 0 if ok
*/
SBR_HEADER_STATUS
sbrGetHeaderData (SBR_HEADER_DATA *h_sbr_header,
                  HANDLE_BIT_BUFFER hBitBuf,
                  SBR_ELEMENT_ID id_sbr)
{
  SBR_HEADER_DATA lastHeader;
  int dummy;
  int headerExtra1, headerExtra2;

  COUNT_sub_start("sbrGetHeaderData");


  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(SBR_HEADER_DATA));
  memcpy (&lastHeader, h_sbr_header, sizeof(SBR_HEADER_DATA));


  INDIRECT(1); FUNC(2); STORE(1);
  h_sbr_header->ampResolution = (unsigned char)getbits (hBitBuf, SI_SBR_AMP_RES_BITS);

  INDIRECT(1); FUNC(2); STORE(1);
  h_sbr_header->startFreq = (unsigned char)getbits (hBitBuf, SI_SBR_START_FREQ_BITS);

  INDIRECT(1); FUNC(2); STORE(1);
  h_sbr_header->stopFreq = (unsigned char)getbits (hBitBuf, SI_SBR_STOP_FREQ_BITS);

  INDIRECT(1); FUNC(2); STORE(1);
  h_sbr_header->xover_band = (unsigned char)getbits (hBitBuf, SI_SBR_XOVER_BAND_BITS);

  INDIRECT(1); FUNC(2);
  dummy = (unsigned char)getbits (hBitBuf, SI_SBR_RESERVED_BITS_HDR);

  INDIRECT(1); FUNC(2);
  headerExtra1 = (unsigned char)getbits (hBitBuf, SI_SBR_HEADER_EXTRA_1_BITS);

  INDIRECT(1); FUNC(2);
  headerExtra2 = (unsigned char)getbits (hBitBuf, SI_SBR_HEADER_EXTRA_2_BITS);

  ADD(1); BRANCH(1);
  if (id_sbr == SBR_ID_SCE) {
    INDIRECT(1); MOVE(1);
    h_sbr_header->channelMode = MONO;
  }
  else {
    INDIRECT(1); MOVE(1);
    h_sbr_header->channelMode = STEREO;
  }


  BRANCH(1);
  if(headerExtra1) {

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->freqScale = (unsigned char)getbits (hBitBuf, SI_SBR_FREQ_SCALE_BITS);

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->alterScale = (unsigned char)getbits (hBitBuf, SI_SBR_ALTER_SCALE_BITS);

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->noise_bands = (unsigned char)getbits (hBitBuf, SI_SBR_NOISE_BANDS_BITS);
  }
  else {
    INDIRECT(3); MOVE(3);
    h_sbr_header->freqScale   = SBR_FREQ_SCALE_DEFAULT;
    h_sbr_header->alterScale  = SBR_ALTER_SCALE_DEFAULT;
    h_sbr_header->noise_bands = SBR_NOISE_BANDS_DEFAULT;
  }

  BRANCH(1);
  if(headerExtra2) {

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->limiterBands = (unsigned char)getbits (hBitBuf, SI_SBR_LIMITER_BANDS_BITS);

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->limiterGains = (unsigned char)getbits (hBitBuf, SI_SBR_LIMITER_GAINS_BITS);

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->interpolFreq = (unsigned char)getbits (hBitBuf, SI_SBR_INTERPOL_FREQ_BITS);

    INDIRECT(1); FUNC(2); STORE(1);
    h_sbr_header->smoothingLength = (unsigned char)getbits (hBitBuf, SI_SBR_SMOOTHING_LENGTH_BITS);
  }
  else {
    INDIRECT(4); MOVE(4);
    h_sbr_header->limiterBands    = SBR_LIMITER_BANDS_DEFAULT;
    h_sbr_header->limiterGains    = SBR_LIMITER_GAINS_DEFAULT;
    h_sbr_header->interpolFreq    = SBR_INTERPOL_FREQ_DEFAULT;
    h_sbr_header->smoothingLength = SBR_SMOOTHING_LENGTH_DEFAULT;
  }


  ADD(7); LOGIC(6); BRANCH(1);
  if(h_sbr_header->syncState != SBR_ACTIVE ||
     lastHeader.startFreq   != h_sbr_header->startFreq   ||
     lastHeader.stopFreq    != h_sbr_header->stopFreq    ||
     lastHeader.xover_band  != h_sbr_header->xover_band  ||
     lastHeader.freqScale   != h_sbr_header->freqScale   ||
     lastHeader.alterScale  != h_sbr_header->alterScale  ||
     lastHeader.noise_bands != h_sbr_header->noise_bands) {

    COUNT_sub_end();
    return HEADER_RESET; /* New settings */
  }

  COUNT_sub_end();

  return HEADER_OK; /* Everything ok */
}

/*
  \brief   Get missing harmonics parameters

  \return  error status - 0 if ok
*/
static int
sbrGetSyntheticCodedData(HANDLE_SBR_HEADER_DATA hHeaderData,
                         HANDLE_SBR_FRAME_DATA hFrameData,
                         HANDLE_BIT_BUFFER     hBitBuf)
{
  int i, bitsRead = 0;

  int flag = getbits(hBitBuf,1);

  COUNT_sub_start("sbrGetSyntheticCodedData");

  MOVE(1); FUNC(2); /* counting previous operations */

  ADD(1);
  bitsRead++;

  BRANCH(1);
  if(flag){

    PTR_INIT(1); /* hFrameData->addHarmonics[] */
    INDIRECT(1); LOOP(1);
    for(i=0;i<hHeaderData->hFreqBandData->nSfb[HI];i++){

      FUNC(2); STORE(1);
      hFrameData->addHarmonics[i]  = (unsigned char)getbits (hBitBuf, SI_SBR_ADDITIONAL_HARMONICS_BITS );

      ADD(1);
      bitsRead++;
    }
  }
  else {
    PTR_INIT(1); /* hFrameData->addHarmonics[] */
    LOOP(1);
    for(i=0; i<MAX_FREQ_COEFFS; i++)
    {
      MOVE(1);
      hFrameData->addHarmonics[i]  = 0;
    }
  }

  COUNT_sub_end();

  return(bitsRead);
}

/*
  \brief      Reads extension data from the bitstream

*/
static int extractExtendedData(HANDLE_BIT_BUFFER     hBitBuf,
                               HANDLE_SBR_HEADER_DATA hHeaderData,
                               HANDLE_PS_DEC       hPs
                               )
{
  int extended_data;
  int i,nBitsLeft;

  COUNT_sub_start("extractExtendedData");

  FUNC(2);
  extended_data = getbits(hBitBuf, SI_SBR_EXTENDED_DATA_BITS);

  BRANCH(1);
  if (extended_data) {
    int cnt, bPsRead;

    MOVE(1);
    bPsRead = 0;

    FUNC(2);
    cnt = getbits(hBitBuf, SI_SBR_EXTENSION_SIZE_BITS);

    ADD(1); BRANCH(1);
    if (cnt == (1<<SI_SBR_EXTENSION_SIZE_BITS)-1)
    {
      FUNC(2); ADD(1);
      cnt += getbits(hBitBuf, SI_SBR_EXTENSION_ESC_COUNT_BITS);
    }

    SHIFT(1);
    nBitsLeft = 8 * cnt;

    LOOP(1);
    while (nBitsLeft > 7) {
      int extension_id = getbits(hBitBuf, SI_SBR_EXTENSION_ID_BITS);

      FUNC(2); /* counting previous operation */

      ADD(1);
      nBitsLeft -= SI_SBR_EXTENSION_ID_BITS;

      BRANCH(2);
      switch(extension_id) {

#ifndef MONO_ONLY

      case EXTENSION_ID_PS_CODING:

        BRANCH(1); 
        if (!hPs) {

          COUNT_sub_end();
          return 0;
        }

        INDIRECT(1); BRANCH(1); LOGIC(1);
        if (!hPs->bForceMono && !bPsRead) {
          FUNC(3); ADD(1);
          nBitsLeft -= ReadPsData(hPs, hBitBuf, nBitsLeft);

          MOVE(1);
          bPsRead = 1;

          INDIRECT(1); MOVE(1);
          hHeaderData->channelMode = PS_STEREO;
          break;
        }

#endif /* #ifndef MONO_ONLY */

      default:
        SHIFT(1);
        cnt = nBitsLeft >> 3;

        LOOP(1);
        for (i=0; i<cnt; i++)
        {
          FUNC(2);
          getbits(hBitBuf, 8);
        }

        SHIFT(1); ADD(1);
        nBitsLeft -= cnt * 8;
        break;
      }
    }

    FUNC(2);
    getbits(hBitBuf, nBitsLeft);
  }

  COUNT_sub_end();
  return 1;
}




/*
  \brief   Read bitstream elements of one channel

  \return  SbrFrameOK:  1=ok, 0=error
*/
int
sbrGetSingleChannelElement (HANDLE_SBR_HEADER_DATA hHeaderData,
                            HANDLE_SBR_FRAME_DATA  hFrameData,
                            HANDLE_PS_DEC          hPs,
                            HANDLE_BIT_BUFFER      hBitBuf)
{
  int i, bit;

  COUNT_sub_start("sbrGetSingleChannelElement");

  INDIRECT(1); MOVE(1);
  hFrameData->coupling = COUPLING_OFF;

  FUNC(2);
  bit = getbits(hBitBuf, 1);

  BRANCH(1);
  if (bit)
  {
    FUNC(2);
    getbits(hBitBuf, SI_SBR_RESERVED_BITS_DATA);
  }


  FUNC(3); BRANCH(1);
  if ( !extractFrameInfo (hBitBuf, hHeaderData, hFrameData) )
  {
    COUNT_sub_end();
    return 0;
  }

  INDIRECT(2); PTR_INIT(1); FUNC(2); BRANCH(1);
  if ( !checkFrameInfo (&hFrameData->frameInfo, hHeaderData->numberTimeSlots) )
  {
    COUNT_sub_end();
    return 0;
  }


  FUNC(2);
  sbrGetDirectionControlData (hFrameData, hBitBuf);


  INDIRECT(1); ADD(1); BRANCH(1);
  if (hFrameData->domain_vec[0]==FREQ)
  {
    INDIRECT(1); MOVE(1);
    hHeaderData->frameErrorFlag = 0;
  }

  PTR_INIT(1); /* hFrameData->sbr_invf_mode[] */
  INDIRECT(1); LOOP(1);
  for (i=0; i<hHeaderData->hFreqBandData->nInvfBands; i++) {

    FUNC(2); STORE(1);
    hFrameData->sbr_invf_mode[i] =
      (INVF_MODE) getbits (hBitBuf, SI_SBR_INVF_MODE_BITS);
  }

  FUNC(3); BRANCH(1);
  if ( !sbrGetEnvelope (hHeaderData, hFrameData, hBitBuf) )
  {
    COUNT_sub_end();
    return 0;
  }

  FUNC(3);
  sbrGetNoiseFloorData (hHeaderData, hFrameData, hBitBuf);

  FUNC(3);
  sbrGetSyntheticCodedData(hHeaderData, hFrameData, hBitBuf);

  FUNC(3); BRANCH(1);
  if ( !extractExtendedData(hBitBuf, hHeaderData, hPs) )
  {
    COUNT_sub_end();
    return 0;
  }

  COUNT_sub_end();

  return 1;
}



/*
  \brief      Read bitstream elements of a channel pair
  \return     SbrFrameOK
*/
int
sbrGetChannelPairElement (HANDLE_SBR_HEADER_DATA hHeaderData,
                          HANDLE_SBR_FRAME_DATA hFrameDataLeft,
                          HANDLE_SBR_FRAME_DATA hFrameDataRight,
                          HANDLE_BIT_BUFFER hBitBuf)
{
  int i, bit;

  COUNT_sub_start("sbrGetChannelPairElement");

  FUNC(2);
  bit = getbits(hBitBuf, 1);

  BRANCH(1);
  if (bit) {

    FUNC(2);
    getbits(hBitBuf, SI_SBR_RESERVED_BITS_DATA);

    FUNC(2);
    getbits(hBitBuf, SI_SBR_RESERVED_BITS_DATA);
  }

  INDIRECT(1); ADD(1); BRANCH(1);
  if (hHeaderData->channelMode != STEREO) {

    INDIRECT(1); MOVE(1);
    hHeaderData->syncState = UPSAMPLING;

    COUNT_sub_end();
    return 0;
  }

  FUNC(2);
  bit = getbits (hBitBuf, SI_SBR_COUPLING_BITS);

  BRANCH(1);
  if (bit) {

    INDIRECT(2); MOVE(2);
    hFrameDataLeft->coupling = COUPLING_LEVEL;
    hFrameDataRight->coupling = COUPLING_BAL;
  }
  else {

    INDIRECT(2); MOVE(2);
    hFrameDataLeft->coupling = COUPLING_OFF;
    hFrameDataRight->coupling = COUPLING_OFF;
  }


  FUNC(3); BRANCH(1);
  if ( !extractFrameInfo (hBitBuf, hHeaderData, hFrameDataLeft) )
  {
    COUNT_sub_end();
    return 0;
  }

  INDIRECT(2); PTR_INIT(1); FUNC(2); BRANCH(1);
  if ( !checkFrameInfo (&hFrameDataLeft->frameInfo, hHeaderData->numberTimeSlots) )
  {
    COUNT_sub_end();
    return 0;
  }

  INDIRECT(1); BRANCH(1);
  if (hFrameDataLeft->coupling) {
    FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(FRAME_INFO));
    memcpy (&hFrameDataRight->frameInfo, &hFrameDataLeft->frameInfo, sizeof(FRAME_INFO));
  }
  else {
    FUNC(3); BRANCH(1);
    if ( !extractFrameInfo (hBitBuf, hHeaderData, hFrameDataRight) )
    {
      COUNT_sub_end();
      return 0;
    }

    INDIRECT(2); PTR_INIT(1); FUNC(2); BRANCH(1);
    if ( !checkFrameInfo (&hFrameDataRight->frameInfo, hHeaderData->numberTimeSlots) )
    {
      COUNT_sub_end();
      return 0;
    }
  }

  FUNC(2);
  sbrGetDirectionControlData (hFrameDataLeft, hBitBuf);

  FUNC(2);
  sbrGetDirectionControlData (hFrameDataRight, hBitBuf);

  INDIRECT(2); ADD(2); LOGIC(1); BRANCH(1);
  if ((hFrameDataLeft->domain_vec[0]==FREQ) && (hFrameDataRight->domain_vec[0]==FREQ))
  {
    INDIRECT(1); MOVE(1);
    hHeaderData->frameErrorFlag = 0;
  }


  PTR_INIT(1); /* hFrameDataLeft->sbr_invf_mode[] */
  INDIRECT(1); LOOP(1);
  for (i=0; i<hHeaderData->hFreqBandData->nInvfBands; i++) {

    FUNC(2); STORE(1);
    hFrameDataLeft->sbr_invf_mode[i] = (INVF_MODE) getbits (hBitBuf, SI_SBR_INVF_MODE_BITS);
  }

  INDIRECT(1); BRANCH(1);
  if (hFrameDataLeft->coupling) {

    PTR_INIT(2); /* hFrameDataRight->sbr_invf_mode[]
                    hFrameDataLeft->sbr_invf_mode[]
                 */
    INDIRECT(1); LOOP(1);
    for (i=0; i<hHeaderData->hFreqBandData->nInvfBands; i++) {
      MOVE(1);
      hFrameDataRight->sbr_invf_mode[i] = hFrameDataLeft->sbr_invf_mode[i];
    }

    FUNC(3); BRANCH(1);
    if ( !sbrGetEnvelope (hHeaderData, hFrameDataLeft, hBitBuf) ) {
      COUNT_sub_end();
      return 0;
    }

    FUNC(3);
    sbrGetNoiseFloorData (hHeaderData, hFrameDataLeft, hBitBuf);

    FUNC(3); BRANCH(1);
    if ( !sbrGetEnvelope (hHeaderData, hFrameDataRight, hBitBuf) ) {
      COUNT_sub_end();
      return 0;
    }
  }
  else {

    PTR_INIT(1); /* hFrameDataRight->sbr_invf_mode[] */
    INDIRECT(1); LOOP(1);
    for (i=0; i<hHeaderData->hFreqBandData->nInvfBands; i++) {

      FUNC(2); STORE(1);
      hFrameDataRight->sbr_invf_mode[i] = (INVF_MODE) getbits (hBitBuf, SI_SBR_INVF_MODE_BITS);
    }


    FUNC(3); BRANCH(1);
    if ( !sbrGetEnvelope (hHeaderData, hFrameDataLeft, hBitBuf ) )
    {
      COUNT_sub_end();
      return 0;
    }

    FUNC(3); BRANCH(1);
    if ( !sbrGetEnvelope (hHeaderData, hFrameDataRight, hBitBuf) )
    {
      COUNT_sub_end();
      return 0;
    }

    FUNC(3);
    sbrGetNoiseFloorData (hHeaderData, hFrameDataLeft, hBitBuf);

  }

  FUNC(3);
  sbrGetNoiseFloorData (hHeaderData, hFrameDataRight, hBitBuf);

  FUNC(3);
  sbrGetSyntheticCodedData(hHeaderData, hFrameDataLeft, hBitBuf);

  FUNC(3);
  sbrGetSyntheticCodedData(hHeaderData, hFrameDataRight, hBitBuf);

  FUNC(3); BRANCH(1);
  if ( !extractExtendedData(hBitBuf, hHeaderData, NULL) )
  {
    COUNT_sub_end();
    return 0;
  }

  COUNT_sub_end();

  return 1;
}




/*
  \brief   Read direction control data from bitstream
*/
void
sbrGetDirectionControlData (HANDLE_SBR_FRAME_DATA h_frame_data,
                            HANDLE_BIT_BUFFER hBitBuf)
{
  int i;

  PTR_INIT(2); LOOP(1);
  for (i = 0; i < h_frame_data->frameInfo.nEnvelopes; i++) {
    FUNC(2); STORE(1);
    h_frame_data->domain_vec[i] = (unsigned char)getbits (hBitBuf, SI_SBR_DOMAIN_BITS);
  }

  PTR_INIT(2); LOOP(1);
  for (i = 0; i < h_frame_data->frameInfo.nNoiseEnvelopes; i++) {
    FUNC(2); STORE(1);
    h_frame_data->domain_vec_noise[i] = (unsigned char)getbits (hBitBuf, SI_SBR_DOMAIN_BITS);
  }
}



/*
  \brief   Read noise-floor-level data from bitstream
*/
void
sbrGetNoiseFloorData (HANDLE_SBR_HEADER_DATA hHeaderData,
                      HANDLE_SBR_FRAME_DATA h_frame_data,
                      HANDLE_BIT_BUFFER hBitBuf)
{
  int i,j;
  int delta;
  COUPLING_MODE coupling;
  int noNoiseBands = hHeaderData->hFreqBandData->nNfb;

  Huffman hcb_noiseF;
  Huffman hcb_noise;
  int envDataTableCompFactor;

  int start_bits = SI_SBR_START_NOISE_BITS_AMP_RES_3_0;
  int start_bits_balance = SI_SBR_START_NOISE_BITS_BALANCE_AMP_RES_3_0;

  COUNT_sub_start("sbrGetNoiseFloorData");

  INDIRECT(1); MOVE(2); /* counting previous operations */

  INDIRECT(1); MOVE(1);
  coupling = h_frame_data->coupling;


  ADD(1); BRANCH(1);
  if (coupling == COUPLING_BAL) {

    PTR_INIT(2); MOVE(1);
    hcb_noise = (Huffman)&sbr_huffBook_NoiseBalance11T;
    hcb_noiseF = (Huffman)&sbr_huffBook_EnvBalance11F;
    envDataTableCompFactor = 1;
  }
  else {

    PTR_INIT(2); MOVE(1);
    hcb_noise = (Huffman)&sbr_huffBook_NoiseLevel11T;
    hcb_noiseF = (Huffman)&sbr_huffBook_EnvLevel11F;
    envDataTableCompFactor = 0;
  }

  PTR_INIT(2); /* h_frame_data->domain_vec_noise[i]
                  h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands]
               */
  LOOP(1);
  for (i=0; i<h_frame_data->frameInfo.nNoiseEnvelopes; i++) {

    ADD(1); BRANCH(1);
    if (h_frame_data->domain_vec_noise[i] == FREQ) {

      ADD(1); BRANCH(1);
      if (coupling == COUPLING_BAL) {

        FUNC(2); SHIFT(1); STORE(1);
        h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands] =
          (float) (((int)getbits (hBitBuf,start_bits_balance)) << envDataTableCompFactor);
      }
      else {

        FUNC(2); STORE(1);
        h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands] =
          (float) getbits (hBitBuf, start_bits);
      }

      PTR_INIT(1); /* h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands+j] */
      LOOP(1);
      for (j = 1; j < noNoiseBands; j++) {

        FUNC(2);
        delta = DecodeHuffmanCW(hcb_noiseF, hBitBuf);

        SHIFT(1); STORE(1);
        h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands+j] = (float) (delta << envDataTableCompFactor);
      }
    }
    else {
      PTR_INIT(1); /* h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands+j] */
      LOOP(1);
      for (j = 0; j < noNoiseBands; j++) {

        FUNC(2);
        delta = DecodeHuffmanCW(hcb_noise, hBitBuf);

        SHIFT(1); STORE(1);
        h_frame_data->sbrNoiseFloorLevel[i*noNoiseBands+j] = (float) (delta << envDataTableCompFactor);
      }
    }
  }

  COUNT_sub_end();
}


/*
  \brief   Read envelope data from bitstream
*/
int
sbrGetEnvelope (HANDLE_SBR_HEADER_DATA hHeaderData,
                HANDLE_SBR_FRAME_DATA h_frame_data,
                HANDLE_BIT_BUFFER hBitBuf)
{
  int i, j;
  unsigned char no_band[MAX_ENVELOPES];
  int delta = 0;
  int offset = 0;
  COUPLING_MODE coupling = h_frame_data->coupling;
  int ampRes = hHeaderData->ampResolution;
  int nEnvelopes = h_frame_data->frameInfo.nEnvelopes;
  int envDataTableCompFactor;
  int start_bits, start_bits_balance;
  Huffman hcb_t, hcb_f;

  COUNT_sub_start("sbrGetEnvelope");

  INDIRECT(3); MOVE(5); /* counting previous operations */

  INDIRECT(1); MOVE(1);
  h_frame_data->nScaleFactors = 0;

    INDIRECT(1); ADD(2); LOGIC(1); BRANCH(1);
    if ( (h_frame_data->frameInfo.frameClass == FIXFIX) && (nEnvelopes == 1) ) {
      MOVE(1);
      ampRes = SBR_AMP_RES_1_5;
    }

  INDIRECT(1); MOVE(1);
  h_frame_data->ampResolutionCurrentFrame = ampRes;

  ADD(1); BRANCH(1);
  if(ampRes == SBR_AMP_RES_3_0)
  {
    MOVE(2);
    start_bits = SI_SBR_START_ENV_BITS_AMP_RES_3_0;
    start_bits_balance = SI_SBR_START_ENV_BITS_BALANCE_AMP_RES_3_0;
  }
  else
  {
    MOVE(2);
    start_bits = SI_SBR_START_ENV_BITS_AMP_RES_1_5;
    start_bits_balance = SI_SBR_START_ENV_BITS_BALANCE_AMP_RES_1_5;
  }

  PTR_INIT(2); /* no_band[]
                  h_frame_data->frameInfo.freqRes[]
               */
  LOOP(1);
  for (i = 0; i < nEnvelopes; i++) {

    INDIRECT(1); MOVE(1);
    no_band[i] = hHeaderData->hFreqBandData->nSfb[h_frame_data->frameInfo.freqRes[i]];

    ADD(1);
    h_frame_data->nScaleFactors += no_band[i];
  }
  INDIRECT(1); STORE(1); /* h_frame_data->nScaleFactors */

  ADD(1); BRANCH(1);
  if (h_frame_data->nScaleFactors > MAX_NUM_ENVELOPE_VALUES)
  {
    COUNT_sub_end();
    return 0;
  }

  ADD(1); BRANCH(1);
  if (coupling == COUPLING_BAL) {

    MOVE(1);
    envDataTableCompFactor = 1;

    ADD(1); BRANCH(1);
    if (ampRes == SBR_AMP_RES_1_5) {

      PTR_INIT(2);
      hcb_t = (Huffman)&sbr_huffBook_EnvBalance10T;
      hcb_f = (Huffman)&sbr_huffBook_EnvBalance10F;
    }
    else {

      PTR_INIT(2);
      hcb_t = (Huffman)&sbr_huffBook_EnvBalance11T;
      hcb_f = (Huffman)&sbr_huffBook_EnvBalance11F;
    }
  }
  else {

    MOVE(1);
    envDataTableCompFactor = 0;

    ADD(1); BRANCH(1);
    if (ampRes == SBR_AMP_RES_1_5) {

      PTR_INIT(2);
      hcb_t = (Huffman)&sbr_huffBook_EnvLevel10T;
      hcb_f = (Huffman)&sbr_huffBook_EnvLevel10F;
    }
    else {

      PTR_INIT(2);
      hcb_t = (Huffman)&sbr_huffBook_EnvLevel11T;
      hcb_f = (Huffman)&sbr_huffBook_EnvLevel11F;
    }
  }

  PTR_INIT(3); /* h_frame_data->domain_vec[]
                  no_band[]
                  h_frame_data->iEnvelope[]
               */
  LOOP(1);
  for (j = 0, offset = 0; j < nEnvelopes; j++) {

    ADD(1); BRANCH(1);
    if (h_frame_data->domain_vec[j] == FREQ) {

      ADD(1); BRANCH(1);
      if (coupling == COUPLING_BAL) {

        FUNC(2); SHIFT(1); STORE(1);
        h_frame_data->iEnvelope[offset] =
          (float) (( (int)getbits(hBitBuf, start_bits_balance)) << envDataTableCompFactor);
      }
      else {

        FUNC(2); STORE(1);
        h_frame_data->iEnvelope[offset] =
          (float) getbits(hBitBuf, start_bits);
      }
    }

    PTR_INIT(1); /* h_frame_data->iEnvelope[] */
    LOOP(1);
    for (i = (1 - h_frame_data->domain_vec[j]); i < no_band[j]; i++) {

      ADD(1); BRANCH(1);
      if (h_frame_data->domain_vec[j] == FREQ) {

        FUNC(2);
        delta = DecodeHuffmanCW(hcb_f, hBitBuf);
      }
      else {

        FUNC(2);
        delta = DecodeHuffmanCW(hcb_t, hBitBuf);
      }

      SHIFT(1); STORE(1);
      h_frame_data->iEnvelope[offset + i] = (float) (delta << envDataTableCompFactor);
    }

    ADD(1);
    offset += no_band[j];
  }

  COUNT_sub_end();

  return 1;
}


/*
  \brief   Extract the frame information (structure FRAME_INFO) from the bitstream
  \return  Zero for bitstream error, one for correct.
*/
int
extractFrameInfo (HANDLE_BIT_BUFFER hBitBuf,
                  HANDLE_SBR_HEADER_DATA hHeaderData,
                  HANDLE_SBR_FRAME_DATA h_frame_data)
{


  int pointer_bits = 0, nEnv = 0, b = 0, border, i, frameClass, n = 0,
    k, p, aL, aR, nL, nR,
    temp = 0, staticFreqRes;


  FRAME_INFO * pFrameInfo = &h_frame_data->frameInfo;
  int numberTimeSlots = hHeaderData->numberTimeSlots;

  COUNT_sub_start("extractFrameInfo");

  INDIRECT(2); MOVE(6); PTR_INIT(1);

  FUNC(2);
  frameClass = getbits (hBitBuf, SBR_CLA_BITS);

  INDIRECT(1); MOVE(1);
  pFrameInfo->frameClass = frameClass;

  BRANCH(2);
  switch (frameClass) {
  case FIXFIX:
    FUNC(2);
    temp = getbits (hBitBuf, SBR_ENV_BITS);

    FUNC(2);
    staticFreqRes = getbits (hBitBuf, SBR_RES_BITS);

    SHIFT(1);
    nEnv = (int) (1 << temp);

    ADD(1); BRANCH(1);
    if (nEnv > MAX_ENVELOPES)
    {
      COUNT_sub_end();
      return 0;
    }

    ADD(1);
    b = nEnv + 1;

    BRANCH(2);
    switch (nEnv) {
    case 1:
      BRANCH(2);
      switch (numberTimeSlots) {
        case 16:
          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(FRAME_INFO));
          memcpy (pFrameInfo, &sbr_frame_info1_16, sizeof(FRAME_INFO));
          break;
      }
      break;
    case 2:
      switch (numberTimeSlots) {
        case 16:
          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(FRAME_INFO));
          memcpy (pFrameInfo, &sbr_frame_info2_16, sizeof(FRAME_INFO));
          break;
      }
      break;
    case 4:
      switch (numberTimeSlots) {
        case 16:
          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(FRAME_INFO));
          memcpy (pFrameInfo, &sbr_frame_info4_16, sizeof(FRAME_INFO));
          break;
      }
      break;
    case 8:
#if (MAX_ENVELOPES >= 8)
      BRANCH(2);
      switch (numberTimeSlots) {
        case 16:
          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(FRAME_INFO));
          memcpy (pFrameInfo, &sbr_frame_info8_16, sizeof(FRAME_INFO));
          break;
      }
      break;
#else
      COUNT_sub_end();
      return 0;
#endif
    }

    BRANCH(1);
    if (!staticFreqRes) {

      PTR_INIT(1); /* pFrameInfo->freqRes[] */
      LOOP(1);
      for (i = 0; i < nEnv ; i++)
      {
        MOVE(1);
        pFrameInfo->freqRes[i] = 0;
      }
    }

    break;
  case FIXVAR:
  case VARFIX:
    FUNC(2);
    temp = getbits (hBitBuf, SBR_ABS_BITS);

    FUNC(2);
    n    = getbits (hBitBuf, SBR_NUM_BITS);

    ADD(2);
    nEnv = n + 1;
    b = nEnv + 1;

    break;
  }


  BRANCH(2);
  switch (frameClass) {
  case FIXVAR:

    INDIRECT(1); MOVE(1);
    pFrameInfo->borders[0] = 0;

    ADD(2);
    border = temp + numberTimeSlots;
    i = b-1;

    PTR_INIT(1); /* pFrameInfo->borders[] */

    MOVE(1);
    pFrameInfo->borders[i] = border;

    LOOP(1);
    for (k = 0; k < n; k++) {
      FUNC(2);
      temp = getbits (hBitBuf, SBR_REL_BITS);

      MULT(2); ADD(2);
      border -= (2 * temp + 2);

      MOVE(1);
      pFrameInfo->borders[--i] = border;
    }


    FUNC(1); ADD(2);
    pointer_bits = (int)(FloatFR_logDualis(n+2) + 0.992188);

    FUNC(2);
    p = getbits (hBitBuf, pointer_bits);

    ADD(2); BRANCH(1);
    if (p > n+1)
    {
      COUNT_sub_end();
      return 0;
    }

    BRANCH(1);
    if (p) {
      INDIRECT(1); ADD(2); STORE(1);
      pFrameInfo->tranEnv = n + 2 - p;
    } else {
      INDIRECT(1); MOVE(1);
      pFrameInfo->tranEnv = -1;
    }

    PTR_INIT(1); /* pFrameInfo->freqRes[] */
    LOOP(1);
    for (k = n; k >= 0; k--) {
      FUNC(2); STORE(1);
      pFrameInfo->freqRes[k] = (unsigned char)getbits (hBitBuf, SBR_RES_BITS); /* f = F [SBR_RES_BITS bits] */
    }


    ADD(1); LOGIC(1); BRANCH(1);
    if (p == 0 || p == 1)
    {
      INDIRECT(2); MOVE(1);
      pFrameInfo->bordersNoise[1] = pFrameInfo->borders[n];
    }
    else
    {
      INDIRECT(2); MOVE(1);
      pFrameInfo->bordersNoise[1] = pFrameInfo->borders[pFrameInfo->tranEnv];
    }

    break;

  case VARFIX:

    INDIRECT(1); MOVE(2);
    border = temp;
    pFrameInfo->borders[0] = border;

    PTR_INIT(1); /* pFrameInfo->borders[] */
    LOOP(1);
    for (k = 1; k <= n; k++) {
      FUNC(2);
      temp = getbits (hBitBuf, SBR_REL_BITS);

      MULT(1); ADD(2);
      border += (2 * temp + 2);

      MOVE(1);
      pFrameInfo->borders[k] = border;
    }
    MOVE(1);
    pFrameInfo->borders[k] = numberTimeSlots;


    FUNC(1); ADD(2);
    pointer_bits = (int)(FloatFR_logDualis(n+2) + 0.992188);

    FUNC(2);
    p = getbits (hBitBuf, pointer_bits);

    ADD(2); BRANCH(1);
    if (p > n+1)
    {
      COUNT_sub_end();
      return 0;
    }

    ADD(1); LOGIC(1); BRANCH(1);
    if (p == 0 || p == 1)
    {
      INDIRECT(1); MOVE(1);
      pFrameInfo->tranEnv = -1;
    }
    else
    {
      INDIRECT(1); ADD(1); STORE(1);
      pFrameInfo->tranEnv = p - 1;
    }


    PTR_INIT(1); /* pFrameInfo->freqRes[k] */
    LOOP(1);
    for (k = 0; k <= n; k++) {
      FUNC(2); STORE(1);
      pFrameInfo->freqRes[k] = (unsigned char)getbits(hBitBuf, SBR_RES_BITS);
    }


    BRANCH(1);
    switch (p) {
    case 0:
      INDIRECT(2); STORE(1);
      pFrameInfo->bordersNoise[1] = pFrameInfo->borders[1];
      break;
    case 1:
      INDIRECT(2); STORE(1);
      pFrameInfo->bordersNoise[1] = pFrameInfo->borders[n];
      break;
    default:
      INDIRECT(2); STORE(1);
      pFrameInfo->bordersNoise[1] = pFrameInfo->borders[pFrameInfo->tranEnv];
      break;
    }

    break;

  case VARVAR:

    FUNC(2);
    aL = getbits (hBitBuf, SBR_ABS_BITS);

    FUNC(2);
    aR = getbits (hBitBuf, SBR_ABS_BITS) + numberTimeSlots;

    FUNC(2);
    nL = getbits (hBitBuf, SBR_NUM_BITS);

    FUNC(2);
    nR = getbits (hBitBuf, SBR_NUM_BITS);


    ADD(2);
    nEnv = nL + nR + 1;

    ADD(2); BRANCH(1);
    if (nEnv > MAX_ENVELOPES)
    {
      COUNT_sub_end();
      return 0;
    }

    ADD(1);
    b = nEnv + 1;


    INDIRECT(1); MOVE(2);
    border            = aL;
    pFrameInfo->borders[0] = border;

    PTR_INIT(1); /* pFrameInfo->borders[] */
    LOOP(1);
    for (k = 1; k <= nL; k++) {
      FUNC(2);
      temp = getbits (hBitBuf, SBR_REL_BITS);

      MULT(1); ADD(2);
      border += (2 * temp + 2);

      MOVE(1);
      pFrameInfo->borders[k] = border;
    }


    MOVE(2);
    border = aR;
    i      = nEnv;

    PTR_INIT(1); /* pFrameInfo->borders[] */

    MOVE(1);
    pFrameInfo->borders[i] = border;

    LOOP(1);
    for (k = 0; k < nR; k++) {

      FUNC(2);
      temp = getbits (hBitBuf, SBR_REL_BITS);

      MULT(1); ADD(2);
      border -= (2 * temp + 2);

      MOVE(1);
      pFrameInfo->borders[--i] = border;
    }


    FUNC(1); ADD(3);
    pointer_bits = (int)(FloatFR_logDualis(nL+nR+2) + 0.992188);

    FUNC(2);
    p = getbits (hBitBuf, pointer_bits);

    ADD(3); BRANCH(1);
    if (p > nL+nR+1)
    {
      COUNT_sub_end();
      return 0;
    }

    BRANCH(1);
    if (p) {
      INDIRECT(1); ADD(1); STORE(1);
      pFrameInfo->tranEnv = b - p;
    } else {
      INDIRECT(1); MOVE(1);
      pFrameInfo->tranEnv = -1;
    }


    PTR_INIT(1); /* pFrameInfo->freqRes[] */
    LOOP(1);
    for (k = 0; k < nEnv; k++) {
      FUNC(2);
      pFrameInfo->freqRes[k] = (unsigned char)getbits(hBitBuf, SBR_RES_BITS);
    }


    INDIRECT(1); MOVE(1);
    pFrameInfo->bordersNoise[0] = aL;

    ADD(1); BRANCH(1);
    if (nEnv == 1) {
      INDIRECT(1); MOVE(1);
      pFrameInfo->bordersNoise[1] = aR;
    }
    else {
       ADD(1); LOGIC(1); BRANCH(1); INDIRECT(2); MOVE(1);
      if (p == 0 || p == 1)
        pFrameInfo->bordersNoise[1] = pFrameInfo->borders[nEnv - 1];
      else
        pFrameInfo->bordersNoise[1] = pFrameInfo->borders[pFrameInfo->tranEnv];

      INDIRECT(1); MOVE(1);
      pFrameInfo->bordersNoise[2] = aR;
    }
    break;
  }


  INDIRECT(1); MOVE(1);
  pFrameInfo->nEnvelopes = nEnv;

  ADD(1); BRANCH(1); INDIRECT(1); MOVE(1);
  if (nEnv == 1)
    pFrameInfo->nNoiseEnvelopes = 1;
  else
    pFrameInfo->nNoiseEnvelopes = 2;


  ADD(2); LOGIC(1); BRANCH(1);
  if (frameClass == VARFIX || frameClass == FIXVAR) {
    INDIRECT(2); MOVE(1);
    pFrameInfo->bordersNoise[0] = pFrameInfo->borders[0];

    INDIRECT(2); MOVE(1);
    pFrameInfo->bordersNoise[pFrameInfo->nNoiseEnvelopes] = pFrameInfo->borders[nEnv];
  }

  COUNT_sub_end();

  return 1;
}


/*
  \brief   Check if the frameInfo vector has reasonable values.
  \return  Zero for error, one for correct
*/
int
checkFrameInfo (FRAME_INFO * pFrameInfo,
                int numberOfTimeSlots)
{
  int maxPos,i,j;
  int startPos;
  int stopPos;
  int tranEnv;
  int startPosNoise;
  int stopPosNoise;
  int nEnvelopes = pFrameInfo->nEnvelopes;
  int nNoiseEnvelopes = pFrameInfo->nNoiseEnvelopes;

  COUNT_sub_start("checkFrameInfo");

  INDIRECT(2); MOVE(2); /* counting previous operations */

  ADD(2); LOGIC(1); BRANCH(1);
  if(nEnvelopes < 1 || nEnvelopes > MAX_ENVELOPES)
  {
    COUNT_sub_end();
    return 0;
  }

  ADD(1); BRANCH(1);
  if(nNoiseEnvelopes > MAX_NOISE_ENVELOPES)
  {
    COUNT_sub_end();
    return 0;
  }

  INDIRECT(5); MOVE(5);
  startPos        = pFrameInfo->borders[0];
  stopPos         = pFrameInfo->borders[nEnvelopes];
  tranEnv         = pFrameInfo->tranEnv;
  startPosNoise   = pFrameInfo->bordersNoise[0];
  stopPosNoise    = pFrameInfo->bordersNoise[nNoiseEnvelopes];

  BRANCH(2);
  switch(numberOfTimeSlots) {
  case 16:
    MOVE(1);
    maxPos = 19;
    break;
  default:
    COUNT_sub_end();
    return 0;
  }


  ADD(1); LOGIC(1); BRANCH(1);
  if( (startPos < 0) || (startPos >= stopPos) )
  {
    COUNT_sub_end();
    return 0;
  }

  ADD(2); BRANCH(1);
  if( startPos > maxPos-numberOfTimeSlots )
  {
    COUNT_sub_end();
    return 0;
  }

  ADD(1); BRANCH(1);
  if( stopPos < numberOfTimeSlots )
  {
    COUNT_sub_end();
    return 0;
  }

  ADD(1); BRANCH(1);
  if(stopPos > maxPos)
  {
    COUNT_sub_end();
    return 0;
  }

  PTR_INIT(1); /* pFrameInfo->borders[] */
  LOOP(1);
  for(i=0;i<nEnvelopes;i++) {

    ADD(1); BRANCH(1);
    if(pFrameInfo->borders[i] >= pFrameInfo->borders[i+1])
    {
      COUNT_sub_end();
      return 0;
    }
  }

  ADD(1); BRANCH(1);
  if(tranEnv>nEnvelopes)
  {
    COUNT_sub_end();
    return 0;
  }


  ADD(2); LOGIC(1); BRANCH(1);
  if(nEnvelopes==1 && nNoiseEnvelopes>1)
  {
    COUNT_sub_end();
    return 0;
  }

  ADD(2); LOGIC(1); BRANCH(1);
  if(startPos != startPosNoise || stopPos != stopPosNoise)
  {
    COUNT_sub_end();
    return 0;
  }


  PTR_INIT(1); /* pFrameInfo->bordersNoise[] */
  LOOP(1);
  for(i=0; i<nNoiseEnvelopes; i++) {

    ADD(2); BRANCH(1);
    if(pFrameInfo->bordersNoise[i] >= pFrameInfo->bordersNoise[i+1])
    {
      COUNT_sub_end();
      return 0;
    }
  }

  PTR_INIT(1); /* pFrameInfo->bordersNoise[] */
  LOOP(1);
  for(i=0; i<nNoiseEnvelopes; i++) {

    MOVE(1);
    startPosNoise = pFrameInfo->bordersNoise[i];

    PTR_INIT(1); /* pFrameInfo->borders[] */
    LOOP(1);
    for(j=0; j<nEnvelopes; j++) {

      ADD(1); BRANCH(1);
      if(pFrameInfo->borders[j] == startPosNoise)
        break;
    }

    ADD(1); BRANCH(1);
    if(j==nEnvelopes)
    {
      COUNT_sub_end();
      return 0;
    }
  }

  COUNT_sub_end();

  return 1;
}
