/*
  independent channel concealment
*/

/*
  The concealment routine swaps the spectral data from the previous and the current frame just before 
  the final frequency to time conversion. In case a single frame is corrupted, concealmant interpolates 
  between the last good and the first good frame to create the spectral data for the missing frame.

  If multiple frames are corrupted, concealment implements first a fade out based on slightly modified
  spectral values from the last good frame. As soon as good frames are available, concealmant fades
  in the new spectral data. 
*/

#include <math.h>

#include "conceal.h"
#include "channelinfo.h"
#include "aac_rom.h"
#include "FloatFR.h"

#include "counters.h" /* the 3GPP instrumenting tool */

#define MINUS_3dB 0.70710678f

extern const float sbr_randomPhase[AAC_NF_NO_RANDOM_VAL][2];

static const float fadeFacTable[] = {
  MINUS_3dB,
  MINUS_3dB * MINUS_3dB,
  MINUS_3dB * MINUS_3dB * MINUS_3dB,
  MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB,
  MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB,
  MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB,
  MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB,
  MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB * MINUS_3dB
};

void CConcealment_UpdateState (CConcealmentInfo *pConcealmentInfo,
                               int FrameOk);

void CConcealment_ApplyRandomSign (CConcealmentInfo *pConcealmentInfo,
                                   float *spec);

void CConcealment_CalcBandEnergy (float                  *spectrum,
                                  int                     blockType,
                                  int                     samplingRateIndex,
                                  CConcealmentExpandType  ex,
                                  float                  *sfbEnergy);

void CConcealment_InterpolateBuffer (float       *spectrum,
                                     float       *enPrev,
                                     float       *enAct,
                                     int          sfbCnt,
                                     const short *pSfbOffset);




/*
  The function initializes the concealment information.
*/
void CConcealment_Init (CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo)
{
  int i;
  CConcealmentInfo *pConcealmentInfo = pAacDecoderStaticChannelInfo->hConcealmentInfo;

  COUNT_sub_start("CConcealment_Init");

  PTR_INIT(1); MOVE(1); /* counting previous operations */

  PTR_INIT(1); /* pConcealmentInfo->SpectralCoefficient[i] */
  LOOP(1);
  for (i = 0; i < FRAME_SIZE; i++) {
    MOVE(1);
    pConcealmentInfo->SpectralCoefficient[i] = 0.0;
  }

  MOVE(1);
  pConcealmentInfo->iRandomPhase = 0;

  MOVE(2);
  pConcealmentInfo->WindowSequence = 0;
  pConcealmentInfo->WindowShape = 0;

  MOVE(2);
  pConcealmentInfo->prevFrameOk[0] = pConcealmentInfo->prevFrameOk[1] = 1;

  MOVE(5);
  pConcealmentInfo->cntConcealFrame = 0;
  pConcealmentInfo->nFadeInFrames   = 5;
  pConcealmentInfo->nValidFrames    = 5;
  pConcealmentInfo->nFadeOutFrames  = 5;
  pConcealmentInfo->ConcealState    = CConcealment_Ok;

  COUNT_sub_end();
}

/*
  The function swaps the data from the current and the previous frame. If an
  error has occured, frame interpolation is performed to restore the missing 
  frame. In case of multiple faulty frames, fade-in and fade-out is applied.
*/
void CConcealment_Apply (CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo,
                         CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                         char FrameOk)
{
  float sfbEnergyPrev[MAXSFB];
  float sfbEnergyAct[MAXSFB];


  char  tmp1;
  float tmp2;
  int   i;

  CConcealmentInfo *pConcealmentInfo     =  pAacDecoderStaticChannelInfo->hConcealmentInfo;
  float            *pSpectralCoefficient =  pAacDecoderChannelInfo->pSpectralCoefficient;
  CIcsInfo         *pIcsInfo             = &pAacDecoderChannelInfo->IcsInfo;

  COUNT_sub_start("CConcealment_Apply");

  INDIRECT(3); PTR_INIT(3); /* counting previous operations */

  /* if the current frame is ok, save it (and swap out stored frame, i.e. the stored
     frame will be current output frame. For correct frames, this means that the
     previous frame will be played now. */

  BRANCH(1);
  if (FrameOk) {

    INDIRECT(2); MOVE(3);
    tmp1 = pIcsInfo->WindowShape;
    pIcsInfo->WindowShape = pConcealmentInfo->WindowShape;
    pConcealmentInfo->WindowShape = tmp1;

    INDIRECT(2); MOVE(3);
    tmp1 = pIcsInfo->WindowSequence;
    pIcsInfo->WindowSequence = pConcealmentInfo->WindowSequence;
    pConcealmentInfo->WindowSequence = tmp1;

    PTR_INIT(2); /* pSpectralCoefficient[]
                    pConcealmentInfo->SpectralCoefficient[]
                 */
    LOOP(1);
    for (i = 0; i < FRAME_SIZE; i++) {

      MOVE(3);
      tmp2 = pSpectralCoefficient[i];
      pSpectralCoefficient[i] = pConcealmentInfo->SpectralCoefficient[i];
      pConcealmentInfo->SpectralCoefficient[i] = tmp2;
    }
  } else {

    INDIRECT(2); MOVE(2);
    pIcsInfo->WindowShape = pConcealmentInfo->WindowShape;
    pIcsInfo->WindowSequence = pConcealmentInfo->WindowSequence;

    PTR_INIT(2); /* pSpectralCoefficient[]
                    pConcealmentInfo->SpectralCoefficient[]
                 */
    LOOP(1);
    for (i = 0; i < FRAME_SIZE; i++) {

      MOVE(1);
      pSpectralCoefficient[i] = pConcealmentInfo->SpectralCoefficient[i];
    }
  }

  FUNC(2);
  CConcealment_UpdateState(pConcealmentInfo, FrameOk);

  /* if previous frame was not ok */
  INDIRECT(1); BRANCH(1);
  if (!pConcealmentInfo->prevFrameOk[1]) {

    INDIRECT(1); LOGIC(1); BRANCH(1);
    if (FrameOk && pConcealmentInfo->prevFrameOk[0]) {

      INDIRECT(1); ADD(1); BRANCH(1);
      if (pIcsInfo->WindowSequence == EightShortSequence) { /* f_(n-2) == EightShortSequence */
        int wnd;

        INDIRECT(1); ADD(1); BRANCH(1);
        if (pConcealmentInfo->WindowSequence == EightShortSequence) { /* f_n == EightShortSequence */

          int scaleFactorBandsTotal = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].NumberOfScaleFactorBands_Short;
          const short *pSfbOffset   = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].ScaleFactorBands_Short;

          INDIRECT(2); MOVE(1); PTR_INIT(1); /* counting previous operations */

          INDIRECT(2); MOVE(2);
          pIcsInfo->WindowShape = 1;
          pIcsInfo->WindowSequence = EightShortSequence;

          PTR_INIT(2); /* pConcealmentInfo->SpectralCoefficient[wnd * (FRAME_SIZE / MAX_WINDOWS)]
                          pSpectralCoefficient[wnd * (FRAME_SIZE / MAX_WINDOWS)]
                       */
          LOOP(1);
          for (wnd = 0; wnd < MAX_WINDOWS; wnd++) {

            INDIRECT(1); FUNC(5);
            CConcealment_CalcBandEnergy(&pSpectralCoefficient[wnd * (FRAME_SIZE / MAX_WINDOWS)], /* spec_(n-2) */
                                        EightShortSequence,
                                        pIcsInfo->SamplingRateIndex,
                                        CConcealment_NoExpand,
                                        sfbEnergyPrev);

            FUNC(5);
            CConcealment_CalcBandEnergy(&pConcealmentInfo->SpectralCoefficient[wnd * (FRAME_SIZE / MAX_WINDOWS)], /* spec_n */
                                        EightShortSequence,
                                        pIcsInfo->SamplingRateIndex,
                                        CConcealment_NoExpand,
                                        sfbEnergyAct);

            FUNC(5);
            CConcealment_InterpolateBuffer(&pSpectralCoefficient[wnd * (FRAME_SIZE / MAX_WINDOWS)], /* spec_(n-1) */
                                           sfbEnergyPrev,
                                           sfbEnergyAct,
                                           scaleFactorBandsTotal,
                                           pSfbOffset);
          }

        } else { /* f_n != EightShortSequence */

          int scaleFactorBandsTotal = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].NumberOfScaleFactorBands_Long;
          const short *pSfbOffset   = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].ScaleFactorBands_Long;

          INDIRECT(2); MOVE(1); PTR_INIT(1); /* counting previous operations */

          PTR_INIT(1); INDIRECT(1); FUNC(5);
          CConcealment_CalcBandEnergy(&pSpectralCoefficient[FRAME_SIZE - (FRAME_SIZE / MAX_WINDOWS)], /* spec_(n-2) last window */
                                      EightShortSequence,
                                      pIcsInfo->SamplingRateIndex,
                                      CConcealment_Expand,
                                      sfbEnergyAct);

          INDIRECT(1); FUNC(5);
          CConcealment_CalcBandEnergy(pConcealmentInfo->SpectralCoefficient, /* spec_n */
                                      OnlyLongSequence,
                                      pIcsInfo->SamplingRateIndex,
                                      CConcealment_NoExpand,
                                      sfbEnergyPrev);

          INDIRECT(2); MOVE(2);
          pIcsInfo->WindowShape = 0;
          pIcsInfo->WindowSequence = LongStopSequence;

          PTR_INIT(2); /* pSpectralCoefficient[]
                          pConcealmentInfo->SpectralCoefficient[]
                       */
          LOOP(1);
          for (i = 0; i < FRAME_SIZE; i++) {

            MOVE(1);
            pSpectralCoefficient[i] = pConcealmentInfo->SpectralCoefficient[i]; /* spec_n */
          }

          FUNC(5);
          CConcealment_InterpolateBuffer(pSpectralCoefficient, /* spec_(n-1) */
                                         sfbEnergyPrev,
                                         sfbEnergyAct,
                                         scaleFactorBandsTotal,
                                         pSfbOffset);
        }
      }
      else {
        int scaleFactorBandsTotal = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].NumberOfScaleFactorBands_Long;
        const short *pSfbOffset   = SamplingRateInfoTable[pIcsInfo->SamplingRateIndex].ScaleFactorBands_Long;

        INDIRECT(2); MOVE(1); PTR_INIT(1); /* counting previous operations */

        INDIRECT(1); FUNC(5);
        CConcealment_CalcBandEnergy(pSpectralCoefficient,  /* spec_(n-2) */
                                    OnlyLongSequence,
                                    pIcsInfo->SamplingRateIndex,
                                    CConcealment_NoExpand,
                                    sfbEnergyPrev);

        INDIRECT(1); ADD(1); BRANCH(1);
        if(pConcealmentInfo->WindowSequence == EightShortSequence) {  /* f_n == EightShortSequence */


          INDIRECT(2); MOVE(2);
          pIcsInfo->WindowShape = 1;
          pIcsInfo->WindowSequence = LongStartSequence;

          /* Expand first short spectrum */
          INDIRECT(1); FUNC(5);
          CConcealment_CalcBandEnergy(pConcealmentInfo->SpectralCoefficient,  /* spec_n */
                                      EightShortSequence,
                                      pIcsInfo->SamplingRateIndex,
                                      CConcealment_Expand,
                                      sfbEnergyAct);
        }
        else {

          INDIRECT(2); MOVE(2);
          pIcsInfo->WindowShape = 0;
          pIcsInfo->WindowSequence = OnlyLongSequence;

          INDIRECT(2); FUNC(5);
          CConcealment_CalcBandEnergy(pConcealmentInfo->SpectralCoefficient,  /* spec_n */
                                      OnlyLongSequence,
                                      pIcsInfo->SamplingRateIndex,
                                      CConcealment_NoExpand,
                                      sfbEnergyAct);
        }

        FUNC(5);
        CConcealment_InterpolateBuffer(pSpectralCoefficient,  /* spec_(n-1) */
                                       sfbEnergyPrev,
                                       sfbEnergyAct,
                                       scaleFactorBandsTotal,
                                       pSfbOffset);
      }
    }

    /* Noise substitution of sign of the output spectral coefficients */
    FUNC(2);
    CConcealment_ApplyRandomSign (pConcealmentInfo,
                                  pSpectralCoefficient);
  }

  /* update FrameOk */
  INDIRECT(2); MOVE(2);
  pConcealmentInfo->prevFrameOk[0]  = pConcealmentInfo->prevFrameOk[1];
  pConcealmentInfo->prevFrameOk[1]  = FrameOk;

  /* scale spectrum according to concealment state */
  INDIRECT(1); BRANCH(2);
  switch (pConcealmentInfo->ConcealState) {
  case CConcealment_FadeOut:

    PTR_INIT(1); /* pSpectralCoefficient[] */
    INDIRECT(1); LOOP(1);
    for (i = 0; i < FRAME_SIZE; i++) {

      MULT(1); STORE(1);
      pSpectralCoefficient[i] = pSpectralCoefficient[i] * fadeFacTable[pConcealmentInfo->cntConcealFrame];
    }
    break;

  case CConcealment_FadeIn:

    PTR_INIT(1); /* pSpectralCoefficient[] */
    INDIRECT(1); LOOP(1);
    for (i = 0; i < FRAME_SIZE; i++) {

      MULT(1); STORE(1);
      pSpectralCoefficient[i] = pSpectralCoefficient[i] * fadeFacTable[pConcealmentInfo->nFadeInFrames - pConcealmentInfo->cntConcealFrame - 1];
    }
    break;

  case CConcealment_Mute:

    PTR_INIT(1); /* pSpectralCoefficient[] */
    INDIRECT(1); LOOP(1);
    for (i = 0; i < FRAME_SIZE; i++) {

      MOVE(1);
      pSpectralCoefficient[i] = 0.0;
    }
    break;
  }

  COUNT_sub_end();
}

/*
  The function toggles the sign of the spectral data randomly. This is 
  useful to ensure the quality of the concealed frames.

  return:  none
*/
void CConcealment_ApplyRandomSign (CConcealmentInfo *pConcealmentInfo,
                                   float *spec)
{
  int i;

  COUNT_sub_start("CConcealment_ApplyRandomSign");

  PTR_INIT(2); /* spec[]
                  sbr_randomPhase[pConcealmentInfo->iRandomPhase][0]
               */
  LOOP(1);
  for(i = 0; i < FRAME_SIZE; i++) {

    BRANCH(1);
    if( sbr_randomPhase[pConcealmentInfo->iRandomPhase][0] < 0.0 ) {

      MULT(1); STORE(1);
      spec[i] = -spec[i];
    }

    pConcealmentInfo->iRandomPhase = (pConcealmentInfo->iRandomPhase + 1) & (AAC_NF_NO_RANDOM_VAL - 1);
  }

  COUNT_sub_end();
}

/*
  The function calculates band-wise the spectral energy. This is used for 
  frame interpolation.
  The band-wise energy is not stored logarithmized anymore.

  return:  none
*/
void CConcealment_CalcBandEnergy (float                  *spectrum,
                                  int                     blockType,
                                  int                     samplingRateIndex,
                                  CConcealmentExpandType  expandType,
                                  float                  *sfbEnergy)
{
  int line, sfb;
  const short *pSfbOffset;
  int scaleFactorBandsTotal;
  float enAccu;

  COUNT_sub_start("CConcealment_CalcBandEnergy");

  /* In the following calculations, enAccu is initialized with LSB-value in order to avoid zero energy-level */

  MOVE(1);
  line = 0;

  BRANCH(2);
  switch(blockType) {

  case OnlyLongSequence:
  case LongStartSequence:
  case LongStopSequence:

    ADD(1); BRANCH(1);
    if(expandType == CConcealment_NoExpand) {

      /* standard long calculation */
      INDIRECT(1); MOVE(2);
      scaleFactorBandsTotal = SamplingRateInfoTable[samplingRateIndex].NumberOfScaleFactorBands_Long;
      pSfbOffset = SamplingRateInfoTable[samplingRateIndex].ScaleFactorBands_Long;

      PTR_INIT(1); /* sfbEnergy[] */
      LOOP(1);
      for (sfb = 0; sfb < scaleFactorBandsTotal; sfb++) {

        MOVE(1);
        enAccu = 1;

        PTR_INIT(1); /* spectrum[] */
        LOOP(1);
        for( ; line < pSfbOffset[sfb+1]; line++) {

          MAC(1);
          enAccu += spectrum[line] * spectrum[line];
        }

        MOVE(1);
        sfbEnergy[sfb] = enAccu;
      }
    }
    else {

      /* compress long to short */
      INDIRECT(1); MOVE(2);
      scaleFactorBandsTotal = SamplingRateInfoTable[samplingRateIndex].NumberOfScaleFactorBands_Short;
      pSfbOffset = SamplingRateInfoTable[samplingRateIndex].ScaleFactorBands_Short;

      PTR_INIT(1); /* sfbEnergy[] */
      LOOP(1);
      for (sfb = 0; sfb < scaleFactorBandsTotal; sfb++) {

        MOVE(1);
        enAccu = 1;

        PTR_INIT(1); /* spectrum[] */
        LOOP(1);
        for(; line < pSfbOffset[sfb+1] << 3; line++) {

          MAC(1);
          enAccu += spectrum[line] * spectrum[line];
        }

        MULT(1); STORE(1);
        sfbEnergy[sfb] = enAccu * 0.125f;
      }
    }
    break;
  case EightShortSequence:

    ADD(1); BRANCH(1);
    if(expandType == CConcealment_NoExpand) {   

      /* standard short calculation */
      INDIRECT(1); MOVE(2);
      scaleFactorBandsTotal = SamplingRateInfoTable[samplingRateIndex].NumberOfScaleFactorBands_Short;
      pSfbOffset = SamplingRateInfoTable[samplingRateIndex].ScaleFactorBands_Short;

      PTR_INIT(1); /* sfbEnergy[] */
      LOOP(1);
      for(sfb = 0; sfb < scaleFactorBandsTotal; sfb++) {

        MOVE(1);
        enAccu = 1;

        PTR_INIT(1); /* spectrum[] */
        LOOP(1);
        for( ; line < pSfbOffset[sfb+1]; line++) {

          MAC(1);
          enAccu += spectrum[line] * spectrum[line];
        }

        MOVE(1);
        sfbEnergy[sfb] = enAccu;
      }
    }
    else {

      /* expand short to long */
      INDIRECT(1); MOVE(2);
      scaleFactorBandsTotal = SamplingRateInfoTable[samplingRateIndex].NumberOfScaleFactorBands_Long;
      pSfbOffset = SamplingRateInfoTable[samplingRateIndex].ScaleFactorBands_Long;

      PTR_INIT(1); /* sfbEnergy[] */
      LOOP(1);
      for (sfb = 0; sfb < scaleFactorBandsTotal; sfb++) {

        MOVE(1);
        enAccu = 1;

        PTR_INIT(1); /* spectrum[] */
        LOOP(1);
        for ( ; line < pSfbOffset[sfb+1]; line++) {

          MAC(1);
          enAccu += spectrum[line >> 3] * spectrum[line >> 3];
        }

        MOVE(1);
        sfbEnergy[sfb] = enAccu;
      }
    }
    break;
  }

  COUNT_sub_end();
}

/*
  The function creates the interpolated spectral data according to the 
  energy of the last good frame and the current (good) frame.
*/
void CConcealment_InterpolateBuffer (float       *spectrum,
                                     float       *enPrev,
                                     float       *enAct,
                                     int          sfbCnt,
                                     const short *pSfbOffset)
{
  int    sfb, line = 0;
  float  multiplier;

  COUNT_sub_start("CConcealment_InterpolateBuffer");

  MOVE(1); /* counting previous operation */

  PTR_INIT(3); /* enAct[]
                  enPrev[]
                  spectrum[]
               */
  LOOP(1);
  for(sfb = 0; sfb < sfbCnt; sfb++) {

    DIV(1); TRANS(1);
    multiplier = (float) pow((double)enAct[sfb] / enPrev[sfb], (double)0.25);

    LOOP(1);
    for(; line < pSfbOffset[sfb+1]; line++) {

      MULT(1); STORE(1);
      spectrum [line] = spectrum[line] * multiplier;
    }
  }

  COUNT_sub_end();
}

/*
  The function updates the state of the concealment state-machine. The 
  states are: mute, fade-in, fade-out, and frame-ok.
*/
void CConcealment_UpdateState(CConcealmentInfo *pConcealmentInfo,
                              int FrameOk)
{
  COUNT_sub_start("CConcealment_UpdateState");

  BRANCH(2);
  switch(pConcealmentInfo->ConcealState){
  case CConcealment_Mute:

    INDIRECT(2); LOGIC(3); BRANCH(1);
    if(pConcealmentInfo->prevFrameOk[1] ||
       (pConcealmentInfo->prevFrameOk[0] &&
        !pConcealmentInfo->prevFrameOk[1] &&
        FrameOk)) {

      INDIRECT(1); ADD(1); STORE(1);
      pConcealmentInfo->cntConcealFrame++;

      INDIRECT(1); ADD(2); BRANCH(1);
      if (pConcealmentInfo->cntConcealFrame >= pConcealmentInfo->nValidFrames){

        INDIRECT(2); MOVE(2);
        pConcealmentInfo->cntConcealFrame = 0;
        pConcealmentInfo->ConcealState    = CConcealment_FadeIn;
      }
    }
    break;

  case CConcealment_FadeIn:

    INDIRECT(2); LOGIC(3); BRANCH(1);
    if(pConcealmentInfo->prevFrameOk[1] ||
       (pConcealmentInfo->prevFrameOk[0] &&
        !pConcealmentInfo->prevFrameOk[1] &&
        FrameOk)) {

      INDIRECT(1); ADD(1); STORE(1);
      pConcealmentInfo->cntConcealFrame++;

      INDIRECT(1); ADD(2); BRANCH(1);
      if(pConcealmentInfo->cntConcealFrame == pConcealmentInfo->nFadeInFrames) {

        INDIRECT(1); MOVE(2);
        pConcealmentInfo->cntConcealFrame = 0;
        pConcealmentInfo->ConcealState    = CConcealment_Ok;
      }
    }
    else{

      INDIRECT(2); MOVE(2);
      pConcealmentInfo->cntConcealFrame  = 0;
      pConcealmentInfo->ConcealState     = CConcealment_FadeOut;
    }
    break;

  case CConcealment_Ok:

    INDIRECT(2); LOGIC(3); BRANCH(1);
    if(!(pConcealmentInfo->prevFrameOk[1] ||
         (pConcealmentInfo->prevFrameOk[0] &&
          !pConcealmentInfo->prevFrameOk[1] &&
          FrameOk))) {

      INDIRECT(1); MOVE(1);
      pConcealmentInfo->ConcealState = CConcealment_FadeOut;
    }
    break;

  case CConcealment_FadeOut:

    INDIRECT(2); LOGIC(3); BRANCH(1);
    if(pConcealmentInfo->prevFrameOk[1] ||
       (pConcealmentInfo->prevFrameOk[0] &&
        !pConcealmentInfo->prevFrameOk[1] &&
        FrameOk)) {

      INDIRECT(2); MOVE(2);
      pConcealmentInfo->cntConcealFrame = 0;
      pConcealmentInfo->ConcealState    = CConcealment_Ok;
    }
    else{

      INDIRECT(1); ADD(1); STORE(1);
      pConcealmentInfo->cntConcealFrame++;

      INDIRECT(1); ADD(1); BRANCH(1);
      if(pConcealmentInfo->cntConcealFrame == pConcealmentInfo->nFadeOutFrames) {

        INDIRECT(1); MOVE(2);
        pConcealmentInfo->cntConcealFrame = 0;
        pConcealmentInfo->ConcealState    = CConcealment_Mute;
      }
    }
    break;
  }
  
  COUNT_sub_end();
}
