/*
  Memory layout
*/
#ifndef __SBR_RAM_H
#define __SBR_RAM_H

#include "sbrdecsettings.h" /* for MAXNRSBRCHANNELS */
#include "sbrdecoder.h"
#include "env_extr.h"

//extern  float  sbr_CodecQmfStatesAnalysis[];
//extern  float  sbr_QmfStatesSynthesis[];

//extern  float sbr_GainSmooth[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];
//extern  float sbr_NoiseSmooth[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];
//extern  char   sbr_GainSmooth_e[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];

//extern FREQ_BAND_DATA FreqBandData;

// #ifdef LP_SBR_ONLY
// extern float   sbr_OverlapBuffer[MAXNRQMFCHANNELS][MAX_OV_COLS * NO_SYNTHESIS_CHANNELS];
// #else
// extern float   sbr_OverlapBuffer[MAXNRQMFCHANNELS][2 * MAX_OV_COLS * NO_SYNTHESIS_CHANNELS];
// #endif

// extern float   sbr_LpcFilterStatesReal[MAXNRQMFCHANNELS][LPC_ORDER][NO_ANALYSIS_CHANNELS];
// #ifndef LP_SBR_ONLY
// extern float   sbr_LpcFilterStatesImag[MAXNRQMFCHANNELS][LPC_ORDER][NO_ANALYSIS_CHANNELS];
// #endif

//extern TRANSPOSER_SETTINGS sbr_TransposerSettings;

//extern SBR_PREV_FRAME_DATA PrevFrameData[MAXNRSBRCHANNELS];

//extern SBRBITSTREAM sbr_PrevBitstream;


#ifdef LP_SBR_ONLY
extern float   *InterimResult;
#else
extern float   *WorkBuffer2;
//extern float   InterimResult[MAX_FRAME_SIZE];
//float* getInterimResultSbr(unsigned int flag,int pos);
#endif

float* getSbrCodecQmfStatesAnalysis(unsigned int flag,int index);
float* getSbrQmfStatesSynthesis(unsigned int flag,int index);
float* getSbrGainSmooth(unsigned int flag,int channel);
float* getSbrGainSmooth(unsigned int flag,int channel,int coeffs);
float* getSbrNoiseSmooth(unsigned int flag,int channel);
float* getSbrNoiseSmooth(unsigned int flag,int channel,int coeffs);
FREQ_BAND_DATA* getFreqBandData(unsigned int flag);
float* getSbrOverlapBuffer(unsigned int flag,int one);
float* getSbrOverlapBuffer(unsigned int flag,int one,int two);
void ResetSbrOverlapBuffer(unsigned int flag);
int getSbrOverlapBufferLength(unsigned int flag);
float* getSbrLpcFilterStatesReal(unsigned int flag,int one,int two);
float* getSbrLpcFilterStatesReal(unsigned int flag,int one,int two,int three);
float* getSbrLpcFilterStatesImag(unsigned int flag,int one,int two);
float* getSbrLpcFilterStatesImag(unsigned int flag,int one,int two,int three);
TRANSPOSER_SETTINGS* getSbrTransposerSettings(unsigned int flag);
SBR_PREV_FRAME_DATA* getPrevFrameData(unsigned int flag,int channel);
SBRBITSTREAM* getSbrPrevBitstream(unsigned int flag);
void ReleaseMapSbrDataInfoStore(unsigned int flag);
float* getInterimResultSbr(unsigned int flag,int pos);
#endif

