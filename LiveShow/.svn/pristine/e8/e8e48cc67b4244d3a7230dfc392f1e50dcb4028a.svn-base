/*
  SBR decoder frontend prototypes and definitions
*/

#ifndef __SBRDECODER_H
#define __SBRDECODER_H

#define SBR_EXTENSION          13  /* 1101 */
#define SBR_EXTENSION_CRC      14  /* 1110 */

#define MAXNRELEMENTS 2
#define MAXNRSBRCHANNELS MAXNRELEMENTS

#ifdef MONO_ONLY
#define MAXNRQMFCHANNELS 1
#else
#define MAXNRQMFCHANNELS MAXNRSBRCHANNELS
#endif

#define MAXSBRBYTES 269


typedef enum
{
  SBRDEC_OK = 0,
  SBRDEC_CONCEAL,
  SBRDEC_NOSYNCH,
  SBRDEC_ILLEGAL_PROGRAM,
  SBRDEC_ILLEGAL_TAG,
  SBRDEC_ILLEGAL_CHN_CONFIG,
  SBRDEC_ILLEGAL_SECTION,
  SBRDEC_ILLEGAL_SCFACTORS,
  SBRDEC_ILLEGAL_PULSE_DATA,
  SBRDEC_MAIN_PROFILE_NOT_IMPLEMENTED,
  SBRDEC_GC_NOT_IMPLEMENTED,
  SBRDEC_ILLEGAL_PLUS_ELE_ID,
  SBRDEC_CREATE_ERROR,
  SBRDEC_NOT_INITIALIZED
}
SBR_ERROR;

typedef enum
{
  SBR_ID_SCE = 0,
  SBR_ID_CPE,
  SBR_ID_CCE,
  SBR_ID_LFE,
  SBR_ID_DSE,
  SBR_ID_PCE,
  SBR_ID_FIL,
  SBR_ID_END
}
SBR_ELEMENT_ID;

typedef struct
{
  int ElementID;
  int ExtensionType;
  int Payload;
  unsigned char Data[MAXSBRBYTES];
}
SBR_ELEMENT_STREAM;

typedef struct
{
  int NrElements;
  int NrElementsCore;
  SBR_ELEMENT_STREAM sbrElement[MAXNRELEMENTS]; /* for the delayed frame */
}
SBRBITSTREAM;


typedef struct SBR_DECODER_INSTANCE *SBRDECODER;


SBRDECODER openSBR (unsigned int keyflag,int sampleRate, int samplesPerFrame, int bDownSample, int bApplyQmfLp) ;

SBR_ERROR applySBR (unsigned int keyflag,
					SBRDECODER self,
                    SBRBITSTREAM * Bitstr,
                    float *TimeData,
                    int *numChannels,
                    int frameOK,
                    int bDownSample,
                    int bBitstreamDownMix);

void closeSBR(unsigned int flag);
#endif
