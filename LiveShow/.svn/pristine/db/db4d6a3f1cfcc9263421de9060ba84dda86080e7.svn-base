/*
  individual channel stream info
*/

#include "aac_rom.h"
#include "streaminfo.h"
#include "FFR_bitbuffer.h"

#include "counters.h" /* the 3GPP instrumenting tool */

unsigned char IsValid(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("IsValid");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->Valid;
}

unsigned char IsLongBlock(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("IsLongBlock");
  INDIRECT(1); ADD(1); BRANCH(1);
  COUNT_sub_end();

  return (pIcsInfo->WindowSequence != EightShortSequence);
}

unsigned char IsShortBlock(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("IsShortBlock");
  INDIRECT(1); ADD(1); BRANCH(1);
  COUNT_sub_end();

  return (pIcsInfo->WindowSequence == EightShortSequence);
}

unsigned char IsMainProfile(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("IsMainProfile");
  INDIRECT(1); ADD(1); BRANCH(1);
  COUNT_sub_end();

  return (pIcsInfo->Profile == ProfileMain);
}

int GetProfile(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetProfile");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->Profile;
}

char GetWindowShape(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetWindowShape");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->WindowShape;
}

char GetWindowSequence(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetWindowSequence");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->WindowSequence;
}

int GetWindowsPerFrame(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetWindowsPerFrame");
  INDIRECT(1); ADD(1); BRANCH(1);
  COUNT_sub_end();

  return (pIcsInfo->WindowSequence == EightShortSequence) ? 8 : 1;
}

char GetWindowGroups(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetWindowGroups");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->WindowGroups;
}

char GetWindowGroupLength(CIcsInfo *pIcsInfo, int index)
{
  COUNT_sub_start("GetWindowGroupLength");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->WindowGroupLength[index];
}

char *GetWindowGroupLengthTable(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetWindowGroupLengthTable");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->WindowGroupLength;
}

char GetScaleFactorBandsTransmitted(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetScaleFactorBandsTransmitted");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->MaxSfBands;
}

char GetScaleFactorBandsTotal(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetScaleFactorBandsTotal");
  INDIRECT(1);
  COUNT_sub_end();

  return pIcsInfo->TotalSfBands;
}

int SamplingRateFromIndex(int index)
{
  COUNT_sub_start("SamplingRateFromIndex");
  INDIRECT(1);
  COUNT_sub_end();

  return SamplingRateInfoTable[index].SamplingFrequency;
}

int GetSamplingFrequency(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetSamplingFrequency");
  INDIRECT(1); FUNC(1);
  COUNT_sub_end();

  return SamplingRateFromIndex(pIcsInfo->SamplingRateIndex);
}

char GetMaximumTnsBands(CIcsInfo *pIcsInfo)
{
  int idx = IsLongBlock(pIcsInfo) ? 0 : 1;

  COUNT_sub_start("GetMaximumTnsBands");
  FUNC(1); MOVE(1); /* counting previous operation */

  INDIRECT(1); /* counting post operation */
  COUNT_sub_end();

  return tns_max_bands_tbl[pIcsInfo->SamplingRateIndex][idx];
}

const short *GetScaleFactorBandOffsets(CIcsInfo *pIcsInfo)
{
  COUNT_sub_start("GetScaleFactorBandOffsets");

  FUNC(1); BRANCH(1);
  if (IsLongBlock(pIcsInfo))
  {
    INDIRECT(1);
    COUNT_sub_end();
    return SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].ScaleFactorBands_Long;
  }
  else
  {
    INDIRECT(1);
    COUNT_sub_end();
    return SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].ScaleFactorBands_Short;
  }
}

void IcsReset(CIcsInfo *pIcsInfo, CStreamInfo *pStreamInfo)
{
  COUNT_sub_start("IcsReset");

  INDIRECT(2); MOVE(1);
  pIcsInfo->Valid = 0;
  pIcsInfo->TotalSfBands = 0;

  INDIRECT(4); MOVE(2);
  pIcsInfo->SamplingRateIndex = pStreamInfo->SamplingRateIndex;
  pIcsInfo->Profile = pStreamInfo->Profile;

  COUNT_sub_end();
}

int IcsRead(HANDLE_BIT_BUF bs,
             CIcsInfo *pIcsInfo)
{
  int i;
  char mask;
  char PredictorDataPresent;
  int ErrorStatus = AAC_DEC_OK;
  
  COUNT_sub_start("IcsRead");

  MOVE(1); /* counting previous operation */

  INDIRECT(1); FUNC(2); STORE(1);
  pIcsInfo->IcsReservedBit = (char) GetBits(bs,1);

  INDIRECT(1); FUNC(2); STORE(1);
  pIcsInfo->WindowSequence = (char) GetBits(bs,2);

  INDIRECT(1); FUNC(2); STORE(1);
  pIcsInfo->WindowShape = (char) GetBits(bs,1);

  FUNC(1); BRANCH(1);
  if (IsLongBlock(pIcsInfo))
  {
    INDIRECT(2); MOVE(1);
    pIcsInfo->TotalSfBands = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].NumberOfScaleFactorBands_Long;

    INDIRECT(1); FUNC(2); STORE(1);
    pIcsInfo->MaxSfBands = (char) GetBits(bs,6);
    
    FUNC(2); BRANCH(1);
    if ((PredictorDataPresent = (char) GetBits(bs,1)))
    {
      COUNT_sub_end();
      return (AAC_DEC_PREDICTION_NOT_SUPPORTED_IN_LC_AAC);
    }

    INDIRECT(2); MOVE(2);
    pIcsInfo->WindowGroups = 1;
    pIcsInfo->WindowGroupLength[0] = 1;
  }
  else
  {
    INDIRECT(2); MOVE(1);
    pIcsInfo->TotalSfBands = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].NumberOfScaleFactorBands_Short;

    INDIRECT(1); FUNC(2); STORE(1);
    pIcsInfo->MaxSfBands = (char) GetBits(bs,4);

    INDIRECT(1); FUNC(2); STORE(1);
    pIcsInfo->ScaleFactorGrouping = (char) GetBits(bs,7);

    INDIRECT(1); MOVE(1);
    pIcsInfo->WindowGroups = 0 ;

    PTR_INIT(1); /* pIcsInfo->WindowGroupLength[] */
    LOOP(1);
    for (i=0; i < 7; i++)
    {
      ADD(1); SHIFT(1);
      mask = 1 << (6 - i);

      MOVE(1);
      pIcsInfo->WindowGroupLength[i] = 1;

      INDIRECT(1); LOGIC(1); BRANCH(1);
      if (pIcsInfo->ScaleFactorGrouping & mask)
      {
        INDIRECT(1); ADD(1); STORE(1);
        pIcsInfo->WindowGroupLength[pIcsInfo->WindowGroups]++;
      }
      else
      {
        INDIRECT(1); ADD(1); STORE(1);
        pIcsInfo->WindowGroups++;
      }
    }

    /* loop runs to i < 7 only */
    INDIRECT(1); MOVE(1);
    pIcsInfo->WindowGroupLength[7] = 1;

    INDIRECT(1); ADD(1); STORE(1);
    pIcsInfo->WindowGroups++;
  }

  INDIRECT(1); MOVE(1);
  pIcsInfo->Valid = 1;

  COUNT_sub_end();

  return ErrorStatus;
}

