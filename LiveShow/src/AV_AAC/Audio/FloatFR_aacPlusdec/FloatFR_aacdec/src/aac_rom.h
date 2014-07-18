/*
  Definition of constant tables
*/

#ifndef AAC_ROM_H
#define AAC_ROM_H

#include "block.h"

#define AAC_NF_NO_RANDOM_VAL  512   /*!< Size of random number array for noise floor */

extern const float OnlyLongWindowSine[FRAME_SIZE];
extern const float OnlyShortWindowSine[FRAME_SIZE/8];
extern const float OnlyLongWindowKBD[FRAME_SIZE];
extern const float OnlyShortWindowKBD[FRAME_SIZE/8];


extern const char specExpTableComb[4][14];

extern const float trigData[513];

extern const SamplingRateInfo SamplingRateInfoTable[9];

extern const CodeBookDescription HuffmanCodeBooks[13];

extern const char tns_max_bands_tbl[9][2];

extern const float tnsCoeff3[8];

extern const float tnsCoeff4[16];

#endif /* #ifndef AAC_ROM_H */
