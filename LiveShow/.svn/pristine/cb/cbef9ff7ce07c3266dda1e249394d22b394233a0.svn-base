/*
  pulse data tool
*/

#include "block.h"
#include "FFR_bitbuffer.h"

#include "counters.h" /* the 3GPP instrumenting tool */

/*
  The function reads the elements for pulse data from
  the bitstream.
*/
void CPulseData_Read(HANDLE_BIT_BUF bs,     /*!< pointer to bitstream */
                     CPulseData *PulseData) /*!< pointer to pulse data side info */
{
  int i;

  COUNT_sub_start("CPulseData_Read");

  FUNC(2); INDIRECT(1); STORE(1); BRANCH(1);
  if ((PulseData->PulseDataPresent = (char) GetBits(bs,1)))
  {
    FUNC(2); INDIRECT(2); STORE(1);
    PulseData->NumberPulse = (char) GetBits(bs,2);

    FUNC(2); INDIRECT(2); STORE(1);
    PulseData->PulseStartBand = (char) GetBits(bs,6);

    PTR_INIT(2); /* PulseData->PulseOffset[i]
                    PulseData->PulseAmp[i]
                 */
    LOOP(1);
    for (i=0; i<=PulseData->NumberPulse; i++)
    {
      FUNC(2); STORE(1);
      PulseData->PulseOffset[i] = (char) GetBits(bs,5);

      FUNC(2); STORE(1);
      PulseData->PulseAmp[i] = (char) GetBits(bs,4);
    }
  }

  COUNT_sub_end();
}

/*
  The function applies the pulse data to the
  specified spectral lines.
*/
void CPulseData_Apply(CPulseData *PulseData,               /*!< pointer to pulse data side info */
                      const short *pScaleFactorBandOffsets, /*!< pointer to scalefactor band offsets */
                      int *coef)                           /*!< pointer to spectrum */
{
  int i,k;

  COUNT_sub_start("CPulseData_Apply");

  INDIRECT(1); BRANCH(1);
  if (PulseData->PulseDataPresent)
  {
    INDIRECT(1); MOVE(1);
    k = pScaleFactorBandOffsets[PulseData->PulseStartBand];
  
    PTR_INIT(2); /* PulseData->PulseOffset[i]
                    PulseData->PulseAmp[i]
                 */
    LOOP(1);
    for (i=0; i<=PulseData->NumberPulse; i++)
    {
      ADD(1);
      k += PulseData->PulseOffset[i];

      INDIRECT(1); BRANCH(1); ADD(1); STORE(1);
      if (coef [k] > 0) coef[k] += PulseData->PulseAmp[i];
      else              coef[k] -= PulseData->PulseAmp[i];
    }
  }

  COUNT_sub_end();
}
