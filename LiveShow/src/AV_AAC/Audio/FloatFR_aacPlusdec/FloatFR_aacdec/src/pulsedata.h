/*
  pulse data tool
*/

#ifndef PULSEDATA_H
#define PULSEDATA_H

#include "bitstream.h"
#include "FFR_bitbuffer.h"

enum
{
  MaximumLines = 4
};

typedef struct
{
  char PulseDataPresent;
  char NumberPulse;
  char PulseStartBand;
  char PulseOffset[MaximumLines];
  char PulseAmp[MaximumLines];
} CPulseData;

void CPulseData_Read(HANDLE_BIT_BUF bs,
                     CPulseData *PulseData);

void CPulseData_Apply(CPulseData *PulseData,
                      const short *pScaleFactorBandOffsets,
                      int *coef);

#endif /* #ifndef PULSEDATA_H */
