/*
  individual channel stream info
*/

#ifndef CHANNELINFO_H
#define CHANNELINFO_H

#include "aacdecoder.h"
#include "overlapadd.h"
#include "pulsedata.h"
#include "streaminfo.h"

#include "FFR_bitbuffer.h"

#define MAX_WINDOWS   8
#define MAX_SFB_LONG  64
#define MAX_SFB_SHORT 16
#define MAX_QUANTIZED_VALUE 8191

enum
{
  OnlyLongSequence = 0,
  LongStartSequence,
  EightShortSequence,
  LongStopSequence,

  ProfileMain = 0,
  ProfileLowComplexity,
  ProfileSSR,
  ProfileReserved
};

typedef struct
{
  int SamplingFrequency;
  char NumberOfScaleFactorBands_Long;
  const short *ScaleFactorBands_Long;
  char NumberOfScaleFactorBands_Short;
  const short *ScaleFactorBands_Short;
} SamplingRateInfo;

typedef struct
{
  char CommonWindow;
  char ElementInstanceTag;
  unsigned char GlobalGain;
} CRawDataInfo;

typedef struct
{
  unsigned char Valid;

  char IcsReservedBit;
  char WindowShape;
  char WindowSequence;
  char MaxSfBands;
  char ScaleFactorGrouping;

  char TotalSfBands;
  char SamplingRateIndex;
  char Profile;

  char WindowGroups;
  char WindowGroupLength[MAX_WINDOWS];

} CIcsInfo;


typedef struct
{
  char  WindowShape;
  char  WindowSequence;
  float SpectralCoefficient[FRAME_SIZE];

  int   iRandomPhase;
  int   prevFrameOk[2];
  int   cntConcealFrame;
  int   ConcealState;
  int   nFadeInFrames;
  int   nValidFrames;
  int   nFadeOutFrames;

} CConcealmentInfo;

typedef struct
{
  const float *pLongWindow[2];
  const float *pShortWindow[2];

  COverlapAddData OverlapAddData;

  CConcealmentInfo *hConcealmentInfo;

} CAacDecoderStaticChannelInfo;

typedef struct
{
  short aScaleFactor[MAX_WINDOWS * MAX_SFB_SHORT];
  char  aCodeBook[MAX_WINDOWS * MAX_SFB_SHORT];

} CAacDecoderDynamicData;

enum
{
  JointStereoMaximumGroups = 8,
  JointStereoMaximumBands = 64
};

#define PNS_BAND_FLAGS_SIZE              16

typedef struct {
  unsigned char correlated[PNS_BAND_FLAGS_SIZE];
  short randomState[PNS_BAND_FLAGS_SIZE * 8];
} CPnsInterChannelData;

typedef struct {
  short current_seed;
  short pns_frame_number;
} CPnsStaticInterChannelData;

typedef struct {
  unsigned char pnsUsed[PNS_BAND_FLAGS_SIZE];
  int CurrentEnergy;
  unsigned char PnsActive;
} CPnsData;

typedef struct
{
  char MsMaskPresent; 
  unsigned char MsUsed[JointStereoMaximumBands]; /*!< every byte contains flags for up to 8 groups */
} CJointStereoData;

typedef struct
{
  CJointStereoData JointStereoData; /*! Common MS-mask for a channel-pair */
  CPnsInterChannelData PnsInterChannelData;
} CAacDecoderDynamicCommonData;

enum
{
  MaximumWindows = 8,
  MaximumBands = 49,
  MaximumOrder = 31,
  MaximumFilters = 3
};

typedef struct
{
  char StartBand;
  char StopBand;

  char Direction;
  char Resolution;

  char Order;
  char Coeff[MaximumOrder];
} CFilter;

typedef struct
{
  char TnsDataPresent;
  char NumberOfFilters[MaximumWindows];
  CFilter Filter[MaximumWindows][MaximumFilters];
} CTnsData;

typedef struct
{
  short *pScaleFactor;
  char  *pCodeBook;

  float *pSpectralCoefficient;

  CIcsInfo IcsInfo;
  CTnsData TnsData;
  CPulseData PulseData;
  CRawDataInfo RawDataInfo;
  CJointStereoData *pJointStereoData;
  CPnsInterChannelData *pPnsInterChannelData;

  CPnsData PnsData;
  CPnsStaticInterChannelData *pPnsStaticInterChannelData;

} CAacDecoderChannelInfo;

int IcsRead(HANDLE_BIT_BUF bs,CIcsInfo *pIcsInfo);
void IcsReset(CIcsInfo *pIcsInfo, CStreamInfo *pStreamInfo);

unsigned char IsValid(CIcsInfo *pIcsInfo);
unsigned char IsLongBlock(CIcsInfo *pIcsInfo);
unsigned char IsShortBlock(CIcsInfo *pIcsInfo);
unsigned char IsMainProfile(CIcsInfo *pIcsInfo);
int  GetProfile(CIcsInfo *pIcsInfo);
char GetWindowShape(CIcsInfo *pIcsInfo);
char GetWindowSequence(CIcsInfo *pIcsInfo);
int  GetWindowsPerFrame(CIcsInfo *pIcsInfo);
char GetWindowGroups(CIcsInfo *pIcsInfo);
char GetWindowGroupLength(CIcsInfo *pIcsInfo, int index);
char *GetWindowGroupLengthTable(CIcsInfo *pIcsInfo);
char GetScaleFactorBandsTransmitted(CIcsInfo *pIcsInfo);
char GetScaleFactorBandsTotal(CIcsInfo *pIcsInfo);
const short *GetScaleFactorBandOffsets(CIcsInfo *pIcsInfo);
int SamplingRateFromIndex(int index);
int GetSamplingFrequency(CIcsInfo *pIcsInfo);
char GetMaximumTnsBands(CIcsInfo *pIcsInfo);
#endif /* #ifndef CHANNELINFO_H */
