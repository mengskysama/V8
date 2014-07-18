/*
  Declaration of constant tables
*/
#ifndef __rom_H
#define __rom_H

#include "sbrdecsettings.h" /* for MAXNRSBRCHANNELS */
#include "sbrdecoder.h"
#include "env_extr.h"
#include "qmf_dec.h"
#include "ps_dec.h"


#define INV_INT_TABLE_SIZE     55
#define SBR_NF_NO_RANDOM_VAL  512

extern const unsigned char sbr_start_freq_16[16];
extern const unsigned char sbr_start_freq_22[16];
extern const unsigned char sbr_start_freq_24[16];
extern const unsigned char sbr_start_freq_32[16];
extern const unsigned char sbr_start_freq_44[16];
extern const unsigned char sbr_start_freq_48[16];


extern const float sbr_limGains[4];
extern const float sbr_limiterBandsPerOctave[4];
extern const float sbr_smoothFilter[4];
extern const float sbr_invIntTable[INV_INT_TABLE_SIZE];
extern const float sbr_randomPhase[SBR_NF_NO_RANDOM_VAL][2];


extern const float sbr_qmf_64_640[QMF_FILTER_STATE_SYN_SIZE/2 + 5];


extern struct dct4Twiddle dct4TwiddleTable[];

#ifndef LP_SBR_ONLY
extern const float sbr_cos_twiddle_L64[];
extern const float sbr_sin_twiddle_L64[];
extern const float sbr_alt_sin_twiddle_L64[];
extern const float sbr_cos_twiddle_L32[];
extern const float sbr_sin_twiddle_L32[];
extern const float sbr_alt_sin_twiddle_L32[];
extern const float sbr_t_cos_L32[];
extern const float sbr_t_sin_L32[];
extern const float sbr_cos_twiddle_ds_L32[];
extern const float sbr_sin_twiddle_ds_L32[];
#endif

extern const FRAME_INFO sbr_frame_info1_16;
extern const FRAME_INFO sbr_frame_info2_16;
extern const FRAME_INFO sbr_frame_info4_16;
extern const FRAME_INFO sbr_frame_info8_16;

extern const SBR_HEADER_DATA sbr_defaultHeader;

extern const char sbr_huffBook_EnvLevel10T[120][2];
extern const char sbr_huffBook_EnvLevel10F[120][2];
extern const char sbr_huffBook_EnvBalance10T[48][2];
extern const char sbr_huffBook_EnvBalance10F[48][2];
extern const char sbr_huffBook_EnvLevel11T[62][2];
extern const char sbr_huffBook_EnvLevel11F[62][2];
extern const char sbr_huffBook_EnvBalance11T[24][2];
extern const char sbr_huffBook_EnvBalance11F[24][2];
extern const char sbr_huffBook_NoiseLevel11T[62][2];
extern const char sbr_huffBook_NoiseBalance11T[24][2];


extern const float aRevLinkDecaySer[NO_SERIAL_ALLPASS_LINKS];
extern const char  aRevLinkDelaySer[];

extern const char  groupBorders[NO_IID_GROUPS + 1];
extern const short bins2groupMap[NO_IID_GROUPS];

extern const float aFractDelayPhaseFactorReQmf[NO_QMF_ALLPASS_CHANNELS];
extern const float aFractDelayPhaseFactorImQmf[NO_QMF_ALLPASS_CHANNELS];
extern const float aFractDelayPhaseFactorReSubQmf[SUBQMF_GROUPS];
extern const float aFractDelayPhaseFactorImSubQmf[SUBQMF_GROUPS];

extern const float *aaFractDelayPhaseFactorSerReQmf[3];
extern const float *aaFractDelayPhaseFactorSerImQmf[3];
extern const float *aaFractDelayPhaseFactorSerReSubQmf[3];
extern const float *aaFractDelayPhaseFactorSerImSubQmf[3];

extern const float scaleFactors[NO_IID_LEVELS];
extern const float scaleFactorsFine[NO_IID_LEVELS_FINE];
extern const float alphas[NO_ICC_LEVELS];

extern const char  aBookPsIidTimeDecode[28][2];
extern const char  aBookPsIidFreqDecode[28][2];
extern const char  aBookPsIccTimeDecode[14][2];
extern const char  aBookPsIccFreqDecode[14][2];
extern const char  aBookPsIidFineTimeDecode[60][2];
extern const char  aBookPsIidFineFreqDecode[60][2];

extern const float p2_6[];
extern const float p8_13[];

#endif

