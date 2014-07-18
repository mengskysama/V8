/*
  Envelope calculation prototypes
*/
#ifndef __ENV_CALC_H
#define __ENV_CALC_H

#include "env_extr.h"  /* for HANDLE_SBR_HEADER_DATA */
#include "sbrdecsettings.h"

typedef struct
{
  float *filtBuffer;
  float *filtBufferNoise;

  int startUp;
  int phaseIndex;
  int prevTranEnv;

  int harmFlagsPrev[(MAX_FREQ_COEFFS+15)/16];

  char harmIndex;

}
SBR_CALCULATE_ENVELOPE;

typedef SBR_CALCULATE_ENVELOPE *HANDLE_SBR_CALCULATE_ENVELOPE;



void
calculateSbrEnvelope (HANDLE_SBR_CALCULATE_ENVELOPE h_sbr_cal_env,
                      HANDLE_SBR_HEADER_DATA hHeaderData,
                      HANDLE_SBR_FRAME_DATA hFrameData,
                      float **analysBufferReal_m,
#ifndef LP_SBR_ONLY
                      float **analysBufferImag_m,
#endif
                      float *degreeAlias,
                      int bUseLP
                      );

int
createSbrEnvelopeCalc (unsigned int keyflag,
					   HANDLE_SBR_CALCULATE_ENVELOPE hSbrCalculateEnvelope,
                       HANDLE_SBR_HEADER_DATA hHeaderData,
                       int chan);

void
resetSbrEnvelopeCalc (HANDLE_SBR_CALCULATE_ENVELOPE hCalEnv);

int
ResetLimiterBands ( unsigned char *limiterBandTable,
                    unsigned char *noLimiterBands,
                    unsigned char *freqBandTable,
                    int noFreqBands,
                    const PATCH_PARAM *patchParam,
                    int noPatches,
                    int limiterBands);


#endif
