/*
  Envelope extraction prototypes
*/

#ifndef __ENVELOPE_EXTRACTION_H
#define __ENVELOPE_EXTRACTION_H

#include "sbr_const.h"
#include "sbr_bitb.h"
#include "sbrdecoder.h"
#include "sbrdecsettings.h"
#include "lpp_tran.h"

typedef enum
{
  HEADER_OK,
  HEADER_RESET,
  CONCEALMENT,
  HEADER_NOT_INITIALIZED
}
SBR_HEADER_STATUS;

typedef enum
{
  SBR_NOT_INITIALIZED,
  UPSAMPLING,
  SBR_ACTIVE
}
SBR_SYNC_STATE;


typedef enum
{
  COUPLING_OFF,
  COUPLING_LEVEL,
  COUPLING_BAL
}
COUPLING_MODE;

typedef enum
{
  UNDEFINED_CHANNEL_MODE,
  MONO,
  STEREO,
  PS_STEREO
}
CHANNEL_MODE;

struct PS_DEC;

typedef struct
{
  unsigned char nSfb[2];
  unsigned char nNfb;
  unsigned char numMaster;
  unsigned char lowSubband;
  unsigned char highSubband;
  unsigned char limiterBandTable[MAX_NUM_LIMITERS+1];
  unsigned char noLimiterBands;
  unsigned char nInvfBands;
  unsigned char *freqBandTable[2];
  unsigned char freqBandTableLo[MAX_FREQ_COEFFS/2+1];

  unsigned char freqBandTableHi[MAX_FREQ_COEFFS+1];

  unsigned char freqBandTableNoise[MAX_NOISE_COEFFS+1];

  unsigned char v_k_master[MAX_FREQ_COEFFS+1];

}
FREQ_BAND_DATA;

typedef FREQ_BAND_DATA *HANDLE_FREQ_BAND_DATA;

typedef struct
{
  SBR_SYNC_STATE syncState;
  unsigned char frameErrorFlag;
  unsigned char prevFrameErrorFlag;
  unsigned char numberTimeSlots;
  unsigned char timeStep;
  unsigned short codecFrameSize;
  unsigned short outSampleRate;


  CHANNEL_MODE  channelMode;
  unsigned char ampResolution;


  unsigned char startFreq;
  unsigned char stopFreq;
  unsigned char xover_band;
  unsigned char freqScale;
  unsigned char alterScale;
  unsigned char noise_bands;


  unsigned char limiterBands;
  unsigned char limiterGains;
  unsigned char interpolFreq;
  unsigned char smoothingLength;

  HANDLE_FREQ_BAND_DATA hFreqBandData;  /* < Pointer to struct #FREQ_BAND_DATA */
}
SBR_HEADER_DATA;

typedef SBR_HEADER_DATA *HANDLE_SBR_HEADER_DATA;


typedef struct
{
  unsigned char frameClass;
  unsigned char nEnvelopes;
  unsigned char borders[MAX_ENVELOPES+1];
  unsigned char freqRes[MAX_ENVELOPES];
  char          tranEnv;
  unsigned char nNoiseEnvelopes;
  unsigned char bordersNoise[MAX_NOISE_ENVELOPES+1];
}
FRAME_INFO;


typedef struct
{
  float sfb_nrg_prev[MAX_FREQ_COEFFS];
  float prevNoiseLevel[MAX_NOISE_COEFFS];
  unsigned char ampRes;
  unsigned char stopPos;
  char xposCtrl;
  COUPLING_MODE coupling;
  INVF_MODE sbr_invf_mode[MAX_INVF_BANDS];
}
SBR_PREV_FRAME_DATA;

typedef SBR_PREV_FRAME_DATA *HANDLE_SBR_PREV_FRAME_DATA;


typedef struct
{
  int nScaleFactors;

  FRAME_INFO frameInfo;
  unsigned char domain_vec[MAX_ENVELOPES];
  unsigned char domain_vec_noise[MAX_NOISE_ENVELOPES];

  INVF_MODE sbr_invf_mode[MAX_INVF_BANDS];
  COUPLING_MODE coupling;
  int ampResolutionCurrentFrame;

  char addHarmonics[MAX_FREQ_COEFFS];
  char xposCtrl;

  float  iEnvelope[MAX_NUM_ENVELOPE_VALUES];
  float  sbrNoiseFloorLevel[MAX_NUM_NOISE_VALUES];
}
SBR_FRAME_DATA;

typedef SBR_FRAME_DATA *HANDLE_SBR_FRAME_DATA;


void initSbrPrevFrameData (HANDLE_SBR_PREV_FRAME_DATA h_prev_data,
                           int timeSlots);

int sbrGetSingleChannelElement (HANDLE_SBR_HEADER_DATA hHeaderData,
                                HANDLE_SBR_FRAME_DATA  hFrameData,
                                struct PS_DEC      *hPs,
                                HANDLE_BIT_BUFFER hBitBuf);

int sbrGetChannelPairElement (HANDLE_SBR_HEADER_DATA hHeaderData,
                              HANDLE_SBR_FRAME_DATA hFrameDataLeft,
                              HANDLE_SBR_FRAME_DATA hFrameDataRight,
                              HANDLE_BIT_BUFFER hBitBuf);

SBR_HEADER_STATUS
sbrGetHeaderData (SBR_HEADER_DATA *h_sbr_header,
                  HANDLE_BIT_BUFFER hBitBuf,
                  SBR_ELEMENT_ID id_sbr);

void
initHeaderData  (unsigned int keyflag,
				 SBR_HEADER_DATA *headerData,
                 int sampleRate,
                 int samplesPerFrame);
#endif
