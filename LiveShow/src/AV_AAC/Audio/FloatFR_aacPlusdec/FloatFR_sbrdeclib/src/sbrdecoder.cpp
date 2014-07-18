/*
  SBR decoder frontend
*/
#include "LibMutex.h"
#include <string.h>
#include <assert.h>
#include "sbr_const.h"
#include "sbr_bitb.h"
#include "sbrdecoder.h"
#include "freq_sca.h"
#include "env_extr.h"
#include "sbr_dec.h"
#include "env_dec.h"
#include "sbr_crc.h"
#include "sbr_ram.h"
#include "sbr_rom.h"
#include "FloatFR.h"
#include "ps_bitdec.h"
#include "ps_dec.h"

#include "counters.h" /* the 3GPP instrumenting tool */
#include <map>
using namespace std;


#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

struct SBR_DECODER_INSTANCE
{
  SBR_CHANNEL      SbrChannel[MAXNRSBRCHANNELS];
  SBR_HEADER_DATA  sbr_header;
  struct PS_DEC ParametricStereoDec;
  SBR_CONCEAL_DATA SbrConcealData;
}/*sbrDecoderInstance*/;


class CSBRDecoderObject
{
public:
	CSBRDecoderObject(SBR_DECODER_INSTANCE*p)
	{
		pSbrInstance = p;
	}
	~CSBRDecoderObject()
	{
		if (pSbrInstance)
			delete pSbrInstance;
		pSbrInstance = NULL;
	}

	SBR_DECODER_INSTANCE* getSbrDecoder()
	{
		return pSbrInstance;
	}
private:
	SBR_DECODER_INSTANCE* pSbrInstance;
};

static map<unsigned int,CSBRDecoderObject*> g_sbrDecoderInstance;
typedef map<unsigned int,CSBRDecoderObject*>::iterator sbrDecoderInstanceIterator;
LibMutex g_sbrDecoderInstanceMutex;

CSBRDecoderObject* getSbrDecoderInstance(unsigned int keyflag)
{
	sbrDecoderInstanceIterator iter = g_sbrDecoderInstance.find(keyflag);
	if (iter!=g_sbrDecoderInstance.end())
	{
		return iter->second;
	}
	LibMutexLocker locker(&g_sbrDecoderInstanceMutex);
	CSBRDecoderObject* pInstance = new CSBRDecoderObject(new SBR_DECODER_INSTANCE());
	g_sbrDecoderInstance.insert(pair<unsigned int,CSBRDecoderObject*>(keyflag,pInstance));
	return pInstance;
}

void ReleaseSbrDecoder(unsigned int keyflag)
{
	LibMutexLocker locker(&g_sbrDecoderInstanceMutex);
	sbrDecoderInstanceIterator iter = g_sbrDecoderInstance.find(keyflag);
	if (iter!=g_sbrDecoderInstance.end())
	{
		delete iter->second;
		g_sbrDecoderInstance.erase(iter);
	}
}

SBR_CHANNEL* getSbrDecoderInstanceChannel(unsigned int keyflag,int index)
{
	CSBRDecoderObject* pInstance = getSbrDecoderInstance(keyflag);
	return &pInstance->getSbrDecoder()->SbrChannel[index];
}

SBR_HEADER_DATA* getSbrDecoderInstanceHead(unsigned int keyflag)
{
	CSBRDecoderObject* pInstance = getSbrDecoderInstance(keyflag);
	return &pInstance->getSbrDecoder()->sbr_header;
}

PS_DEC* getSbrDecoderInstanceDEC(unsigned int keyflag)
{
	CSBRDecoderObject* pInstance = getSbrDecoderInstance(keyflag);
	return &pInstance->getSbrDecoder()->ParametricStereoDec;
}

SBR_CONCEAL_DATA* getSbrConcealData(unsigned int keyflag)
{
	CSBRDecoderObject* pInstance = getSbrDecoderInstance(keyflag);
	return &pInstance->getSbrDecoder()->SbrConcealData;
}

static void
map2channelSbrData2one(HANDLE_SBR_FRAME_DATA hFrameDataLeft,
                       HANDLE_SBR_FRAME_DATA hFrameDataRight,
                       HANDLE_SBR_HEADER_DATA hHeaderData);


void closeSBR(unsigned int flag)
{
	ReleaseMapSbrDataInfoStore(flag);
	ReleaseSbrDecoder(flag);
}

/*!
  \brief     Set up SBR decoder phase 1

  \return    Handle
*/
SBRDECODER openSBR (unsigned int keyflag,int sampleRate, int samplesPerFrame, int bDownSample, int bApplyQmfLp)
{
  int i, err;
  SBR_CHANNEL *SbrChannel = getSbrDecoderInstanceChannel(keyflag,0)/*&sbrDecoderInstance.SbrChannel[0]*/;
  HANDLE_SBR_HEADER_DATA hHeaderData = getSbrDecoderInstanceHead(keyflag)/*&sbrDecoderInstance.sbr_header*/;
  HANDLE_SBR_CONCEAL_DATA hSbrConcealData = getSbrConcealData(keyflag)/*&sbrDecoderInstance.SbrConcealData*/;

  COUNT_sub_start("openSBR");

  FloatFR_Init();  /* Not needed for a DSP implementation */

  FUNC(3);
  initHeaderData( keyflag,hHeaderData,
                  sampleRate,
                  samplesPerFrame);

  LOOP(1);
  for (i = 0; i < MAXNRSBRCHANNELS; i++) {

    FUNC(5);
    PTR_INIT(1);
    err = createSbrDec (keyflag,
						&(SbrChannel[i]),
                        hHeaderData,
                        i,
                        bApplyQmfLp,
                        sampleRate);

    BRANCH(1);
    if (err) {
      COUNT_sub_end();
      return NULL;
    }
  } LOOP(1);

  for (i = 0; i < MAXNRQMFCHANNELS; i++) {

    FUNC(4);
    PTR_INIT(1);
    err = createSbrQMF (keyflag,
						&(SbrChannel[i]),
                        hHeaderData,
                        i,
                        bDownSample);

    BRANCH(1);
    if (err) {
      COUNT_sub_end();
      return NULL;
    }
  }

#ifndef MONO_ONLY
  FUNC(5);
  PTR_INIT(1); INDIRECT(3);
  err = CreatePsDec(keyflag,getSbrDecoderInstanceDEC(keyflag)/*&sbrDecoderInstance.ParametricStereoDec*/,
                    SbrChannel[0].SbrDec.SynthesisQmfBank.no_col);

  BRANCH(1);
  if ( err) {
    COUNT_sub_end();
    return NULL;
  }
 #endif /* #ifndef MONO_ONLY */

  PTR_INIT(1);
  hSbrConcealData->Bitstream = getSbrPrevBitstream(keyflag)/*&sbr_PrevBitstream*/;

  MOVE(3);
  hSbrConcealData->FrameOk = 1;
  hSbrConcealData->Bitstream->NrElements     = 1;
  hSbrConcealData->Bitstream->NrElementsCore = 1;

  LOOP(1);
  for (i = 0; i < MAXNRELEMENTS; i++) {

    MOVE(3);
    hSbrConcealData->Bitstream->sbrElement[i].ElementID     = 0;
    hSbrConcealData->Bitstream->sbrElement[i].ExtensionType = SBR_EXTENSION;
    hSbrConcealData->Bitstream->sbrElement[i].Payload       = 0;
  }

  FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(/*sizeof(sbr_OverlapBuffer)/2*/getSbrOverlapBufferLength(keyflag));
  //memset(sbr_OverlapBuffer, 0, sizeof(sbr_OverlapBuffer)/2);
  ResetSbrOverlapBuffer(keyflag);

  PTR_INIT(1);
  COUNT_sub_end();

  return getSbrDecoderInstance(keyflag)->getSbrDecoder()/*&sbrDecoderInstance*/;
}

/*!
  \brief     Delay SBR bitstream

  \return none
*/
static void
DelaySbrBitstr(HANDLE_SBR_CONCEAL_DATA hSbrConcealData,
               SBRBITSTREAM *hSbrBS,
               int *SbrFrameOK)
{
  int tmp;
  SBRBITSTREAM bs_tmp;

  COUNT_sub_start("DelaySbrBitstr");

  INDIRECT(1); MOVE(1);
  tmp       = hSbrConcealData->Bitstream->sbrElement[0].ElementID;

  INDIRECT(1); MOVE(3);
  bs_tmp    = hSbrBS[0];
  hSbrBS[0] = *(hSbrConcealData->Bitstream);
  *(hSbrConcealData->Bitstream) = bs_tmp;

  BRANCH(1);
  if(!(*SbrFrameOK))
  {
    MOVE(1);
    hSbrConcealData->Bitstream->sbrElement[0].ElementID = tmp;
  }

  INDIRECT(1); MOVE(2);
  tmp = *SbrFrameOK;
  *SbrFrameOK = hSbrConcealData->FrameOk;
  MOVE(1);
  hSbrConcealData->FrameOk = tmp;

  COUNT_sub_end();
}


/*!
  \brief     SBR decoder processing

 \return SBRDEC_OK if successfull, else error code
*/
SBR_ERROR
applySBR (unsigned int keyflag,
		  SBRDECODER self,
          SBRBITSTREAM * Bitstr,
          float *timeData,
          int *numChannels,
          int SbrFrameOK,
          int bDownSample,
          int bBitstreamDownMix
          )
{
  unsigned char i;
  unsigned char dualMono = 0;
  int stereo = 0;
  int CRCLen = 0;
  int crcEnable = 0;
  int readHeader = 0;
  int err = 0;


  SBR_CHANNEL *SbrChannel = &self->SbrChannel[0];
  BIT_BUFFER bitBuf;
  HANDLE_SBR_HEADER_DATA hHeaderData = &self->sbr_header;
  SBR_HEADER_STATUS headerStatus = HEADER_NOT_INITIALIZED;
  int codecFrameSize = hHeaderData->codecFrameSize;
  SBR_SYNC_STATE initialSyncState = hHeaderData->syncState;
  HANDLE_SBR_CONCEAL_DATA hConcealData = &self->SbrConcealData;

  float * pWorkBuffer1 = &timeData[2*codecFrameSize];


  SBR_FRAME_DATA *hFrameDataLeft  = (SBR_FRAME_DATA*) pWorkBuffer1;
  SBR_FRAME_DATA *hFrameDataRight = (SBR_FRAME_DATA*) getInterimResultSbr(keyflag,0)/*InterimResult*/;
  assert( sizeof(SBR_FRAME_DATA) <= MAX_FRAME_SIZE*sizeof(float));

  COUNT_sub_start("applySBR");

  INDIRECT(5); MOVE(9); PTR_INIT(6); /* counting previous operations */


  PTR_INIT(1); FUNC(3);
  DelaySbrBitstr(hConcealData, Bitstr, &SbrFrameOK);


  INDIRECT(1); BRANCH(1);
  if (Bitstr->NrElements) {

    PTR_INIT(6); /* Bitstr->sbrElement[]
                    SbrChannel[]
                    hHeaderData
                    hFrameDataLeft
                    hFrameDataRight
                    bitBuf
                 */
    INDIRECT(1); LOOP(1);
    for (i=0; i<Bitstr->NrElements; i++) {

      /* Save last error flag */
      MOVE(1);
      hHeaderData->prevFrameErrorFlag = hHeaderData->frameErrorFlag;

      ADD(1); BRANCH(1);
      if (Bitstr->NrElements == 2) {
        MOVE(1);
        dualMono = 1;
      }
      else {
        BRANCH(2);
        switch (Bitstr->sbrElement[i].ElementID) {
          case SBR_ID_SCE:
            MOVE(1);
            stereo = 0;
            break;

          case SBR_ID_CPE:
            MOVE(1);
            stereo = 1;
            break;

          default:
            MOVE(1);
            SbrFrameOK = 0;
        }
      }

      MULT(1); FUNC(3);
      initBitBuffer (&bitBuf,
                     Bitstr->sbrElement[i].Data,
                     Bitstr->sbrElement[i].Payload * 8) ;

      FUNC(2);
      getbits (&bitBuf, LEN_NIBBLE);


      BRANCH(1);
      if (SbrFrameOK) {

        ADD(1); BRANCH(1);
        if (Bitstr->sbrElement[i].ExtensionType == SBR_EXTENSION_CRC) {

          MOVE(1);
          crcEnable = 1;

          ADD(2); MULT(1);
          CRCLen = 8*(Bitstr->sbrElement[i].Payload-1)+4 - SI_SBR_CRC_BITS;

          BRANCH(1);
          if (CRCLen < 0) {

            MOVE(2);
            crcEnable = 0;
            SbrFrameOK = 0;
          }
        }


        BRANCH(1);
        if (crcEnable)
        {
          FUNC(2);
          SbrFrameOK = SbrCrcCheck (&bitBuf,
                                    CRCLen);
        }

        FUNC(2);
        readHeader = getbits (&bitBuf, 1);

        BRANCH(1);
        if (SbrFrameOK){
          int lr;

          FUNC(1);
          if (readHeader) {

            FUNC(3);
            headerStatus = sbrGetHeaderData (hHeaderData,
                                             &bitBuf,
                                             (SBR_ELEMENT_ID)Bitstr->sbrElement[i].ElementID);

            ADD(1); BRANCH(1);
            if (headerStatus == HEADER_NOT_INITIALIZED) {
              COUNT_sub_end();
              return SBRDEC_NOT_INITIALIZED;
            }

            ADD(1); BRANCH(1);
            if (headerStatus == HEADER_RESET) {
              FUNC(1);
              err = resetFreqBandTables(hHeaderData);

              PTR_INIT(1); /* SbrChannel[] */
              LOOP(1);
              for (lr = 0 ; lr < MAXNRSBRCHANNELS; lr++) {
                INDIRECT(1); PTR_INIT(1); FUNC(1);
                resetSbrEnvelopeCalc (&(SbrChannel[lr].SbrDec.SbrCalculateEnvelope));
              }

              PTR_INIT(1); /* SbrChannel[] */
              LOOP(1);
              for (lr = 0 ; lr < MAXNRQMFCHANNELS; lr++) {
                PTR_INIT(1); FUNC(5);
                err |= resetSbrQMF (&(SbrChannel[lr].SbrDec),
                                    hHeaderData,
                                    lr,
                                    *numChannels,
                                    SbrChannel[lr].hPrevFrameData);
              }

              BRANCH(1);
              if (err==0) {

                MOVE(1);
                hHeaderData->syncState = SBR_ACTIVE;
              }
            }
          } // if (readHeader)

          ADD(1); LOGIC(1); BRANCH(1);
          if (err || hHeaderData->syncState == SBR_NOT_INITIALIZED) {

            ADD(1); LOGIC(1); BRANCH(1);
            if (err && hHeaderData->syncState == SBR_NOT_INITIALIZED) {
              COUNT_sub_end();
              return SBRDEC_NOT_INITIALIZED;
            }

            SHIFT(1); FUNC(3);
            initHeaderData( keyflag,
							hHeaderData,
                            hHeaderData->outSampleRate >> 1,
                            codecFrameSize);

            FUNC(1);
            err = resetFreqBandTables(hHeaderData);

            MOVE(2);
            hHeaderData->hFreqBandData->lowSubband = NO_ANALYSIS_CHANNELS;
            hHeaderData->hFreqBandData->highSubband = NO_ANALYSIS_CHANNELS;

            PTR_INIT(1); /* SbrChannel[lr] */
            LOOP(1);
            for (lr = 0 ; lr < MAXNRSBRCHANNELS; lr++) {
              INDIRECT(1); PTR_INIT(1); FUNC(1);
              resetSbrEnvelopeCalc (&(SbrChannel[lr].SbrDec.SbrCalculateEnvelope));
            }

            PTR_INIT(1); /* SbrChannel[lr] */
            LOOP(1);
            for (lr = 0 ; lr < MAXNRQMFCHANNELS; lr++) {
              PTR_INIT(1); FUNC(5);
              err |= resetSbrQMF (&(SbrChannel[lr].SbrDec),
                                  hHeaderData,
                                  lr,
                                  *numChannels,
                                  SbrChannel[lr].hPrevFrameData);
            }

            MOVE(1);
            hHeaderData->syncState = UPSAMPLING;
          }


          ADD(1); BRANCH(1);
          if (hHeaderData->syncState == SBR_ACTIVE) {


            BRANCH(1);
            if (dualMono) {

              BRANCH(1);
              if (i == 0) {

                BRANCH(1); MOVE(1);
                hFrameDataLeft->xposCtrl = max(0, SbrChannel[i].hPrevFrameData->xposCtrl);

                FUNC(4);
                SbrFrameOK = sbrGetSingleChannelElement(hHeaderData,
                                                        hFrameDataLeft,
                                                        NULL,
                                                        &bitBuf);
              }
              else {

                BRANCH(1); MOVE(1);
                hFrameDataRight->xposCtrl = max(0, SbrChannel[i].hPrevFrameData->xposCtrl);

                FUNC(4);
                SbrFrameOK = sbrGetSingleChannelElement(hHeaderData,
                                                        hFrameDataRight,
                                                        NULL,
                                                        &bitBuf);
              }
            }
            else {
              BRANCH(1); MOVE(1);
              hFrameDataLeft->xposCtrl = max(0, SbrChannel[i].hPrevFrameData->xposCtrl);

              BRANCH(1);
              if (stereo) {

                BRANCH(1); MOVE(1);
                hFrameDataRight->xposCtrl = max(0, SbrChannel[i+1].hPrevFrameData->xposCtrl);

                FUNC(4);
                SbrFrameOK = sbrGetChannelPairElement(hHeaderData,
                                                      hFrameDataLeft,
                                                      hFrameDataRight,
                                                      &bitBuf);
              }
              else
              {


                BRANCH(1);
                if (bBitstreamDownMix)
                {
                  MOVE(1);
                  //sbrDecoderInstance.ParametricStereoDec.bForceMono = 1;
				  getSbrDecoderInstanceDEC(keyflag)->bForceMono = 1;
                }
                else
                {
                  MOVE(1);
                  //sbrDecoderInstance.ParametricStereoDec.bForceMono = 0;
				   getSbrDecoderInstanceDEC(keyflag)->bForceMono = 0;
                }

                FUNC(4);
                SbrFrameOK = sbrGetSingleChannelElement(hHeaderData,
                                                        hFrameDataLeft,
                                                        getSbrDecoderInstanceDEC(keyflag)/*&sbrDecoderInstance.ParametricStereoDec*/,
                                                        &bitBuf);
              }
            }
            {
              int payloadbits = GetNrBitsRead (&bitBuf);
              int fillbits = (8 - (payloadbits & 7)) & 7;

              FUNC(1); ADD(1); LOGIC(2); /* counting previous operations */

              ADD(2); MULT(1); BRANCH(1);
              if ((payloadbits + fillbits) != 8 * Bitstr->sbrElement[i].Payload)
              {
                MOVE(1);
                SbrFrameOK = 0;
              }
            }
           }
        }
      }

      ADD(1); LOGIC(1); BRANCH(1);
      if (!SbrFrameOK || headerStatus == CONCEALMENT) {

        MOVE(1);
        hHeaderData->frameErrorFlag = 1;
      }
    }
  }
  else {
    ADD(1); BRANCH(1);
    if (hHeaderData->syncState != SBR_NOT_INITIALIZED)
    {
      MOVE(1);
      hHeaderData->syncState = UPSAMPLING;
    }

    ADD(1); BRANCH(1); MOVE(1);
    stereo = ( *numChannels == 2) ? 1 : 0 ;
  }

  ADD(2); LOGIC(2); BRANCH(1);
  if ((hHeaderData->syncState == SBR_NOT_INITIALIZED) ||
      (initialSyncState == SBR_NOT_INITIALIZED && SbrFrameOK==0) ) {

    MOVE(1);
    hHeaderData->syncState = SBR_NOT_INITIALIZED;

    COUNT_sub_end();
    return SBRDEC_NOT_INITIALIZED;
  }


  ADD(1); BRANCH(1);
  if (hHeaderData->syncState == SBR_ACTIVE) {

    MOVE(1);
    err = 0;

    INDIRECT(2); ADD(2); LOGIC(1); BRANCH(1);
    if ( (headerStatus == HEADER_RESET) ||
         (hFrameDataLeft->xposCtrl != SbrChannel[0].hPrevFrameData->xposCtrl) ) {

      FUNC(9); LOGIC(1);
      err |= resetLppTransposer (&SbrChannel[0].SbrDec.LppTrans,
                                 hFrameDataLeft->xposCtrl,
                                 hHeaderData->hFreqBandData->lowSubband,
                                 hHeaderData->hFreqBandData->v_k_master,
                                 hHeaderData->hFreqBandData->numMaster,
                                 hHeaderData->hFreqBandData->freqBandTableNoise,
                                 hHeaderData->hFreqBandData->nNfb,
                                 hHeaderData->hFreqBandData->highSubband,
                                 hHeaderData->outSampleRate);

      INDIRECT(2); PTR_INIT(1); FUNC(7); LOGIC(1);
      err |= ResetLimiterBands ( hHeaderData->hFreqBandData->limiterBandTable,
                                 &hHeaderData->hFreqBandData->noLimiterBands,
                                 hHeaderData->hFreqBandData->freqBandTable[LO],
                                 hHeaderData->hFreqBandData->nSfb[LO],
                                 SbrChannel[0].SbrDec.LppTrans.pSettings->patchParam,
                                 SbrChannel[0].SbrDec.LppTrans.pSettings->noOfPatches,
                                 hHeaderData->limiterBands);
    }

    ADD(3); LOGIC(4); BRANCH(1);
    if ( (MAXNRQMFCHANNELS>1) && 
         (stereo || dualMono) &&
         ((headerStatus == HEADER_RESET) ||
          (hFrameDataRight->xposCtrl != SbrChannel[1].hPrevFrameData->xposCtrl)) ) {

      FUNC(9); LOGIC(1);
      err |= resetLppTransposer (&SbrChannel[1].SbrDec.LppTrans,
                                 hFrameDataRight->xposCtrl,
                                 hHeaderData->hFreqBandData->lowSubband,
                                 hHeaderData->hFreqBandData->v_k_master,
                                 hHeaderData->hFreqBandData->numMaster,
                                 hHeaderData->hFreqBandData->freqBandTableNoise,
                                 hHeaderData->hFreqBandData->nNfb,
                                 hHeaderData->hFreqBandData->highSubband,
                                 hHeaderData->outSampleRate);

      INDIRECT(1); PTR_INIT(1); FUNC(7); LOGIC(1);
      err |= ResetLimiterBands ( hHeaderData->hFreqBandData->limiterBandTable,
                                 &hHeaderData->hFreqBandData->noLimiterBands,
                                 hHeaderData->hFreqBandData->freqBandTable[LO],
                                 hHeaderData->hFreqBandData->nSfb[LO],
                                 SbrChannel[1].SbrDec.LppTrans.pSettings->patchParam,
                                 SbrChannel[1].SbrDec.LppTrans.pSettings->noOfPatches,
                                 hHeaderData->limiterBands);
    }

    BRANCH(1);
    if (err) {
      MOVE(1);
      hHeaderData->syncState = UPSAMPLING;
    }
    else {
      BRANCH(1);
      if (dualMono)  {

        FUNC(5);
        decodeSbrData (hHeaderData,
                       hFrameDataLeft,
                       SbrChannel[0].hPrevFrameData,
                       NULL,
                       NULL);

        FUNC(5);
        decodeSbrData (hHeaderData,
                       hFrameDataRight,
                       SbrChannel[1].hPrevFrameData,
                       NULL,
                       NULL);
      }
      else {

        BRANCH(1); FUNC(5);
        decodeSbrData (hHeaderData,
                       hFrameDataLeft,
                       SbrChannel[0].hPrevFrameData,
                       (stereo) ? hFrameDataRight : NULL,
                       (stereo) ? SbrChannel[1].hPrevFrameData : NULL);

        
#ifndef MONO_ONLY
        ADD(1); BRANCH(1);
        if(hHeaderData->channelMode == PS_STEREO ) {
          PTR_INIT(1); FUNC(1);
          DecodePs(getSbrDecoderInstanceDEC(keyflag)/*&sbrDecoderInstance.ParametricStereoDec*/);
        }
#endif /* #ifndef MONO_ONLY */
        
      }
    }

  }


  ADD(1); LOGIC(1); BRANCH(1);
  if ( initialSyncState == SBR_NOT_INITIALIZED && hHeaderData->frameErrorFlag ) {

    MOVE(1);
    hHeaderData->syncState = SBR_NOT_INITIALIZED;

    COUNT_sub_end();
    return SBRDEC_NOT_INITIALIZED;
  }


  ADD(1); BRANCH(1);
  if(hHeaderData->syncState == SBR_ACTIVE){

    INDIRECT(1); MOVE(1);
    SbrChannel[0].hPrevFrameData->stopPos = hFrameDataLeft->frameInfo.borders[hFrameDataLeft->frameInfo.nEnvelopes];

    LOGIC(1); BRANCH(1);
    if(stereo || dualMono)
    {
      INDIRECT(1); MOVE(1);
      SbrChannel[1].hPrevFrameData->stopPos = hFrameDataRight->frameInfo.borders[hFrameDataRight->frameInfo.nEnvelopes];
    }
  }


  LOGIC(1); BRANCH(1);
  if(stereo && bBitstreamDownMix){
    int kk;

    ADD(1); BRANCH(1);
    if(hHeaderData->syncState == SBR_ACTIVE){
      FUNC(3);
      map2channelSbrData2one(hFrameDataLeft,hFrameDataRight,hHeaderData);
    }

    PTR_INIT(1); /* timeData[kk] */
    LOOP(1);
    for(kk = 0; kk < codecFrameSize; kk++){
      ADD(2); DIV(1); STORE(1);
      timeData[kk] = (timeData[kk] + timeData[kk + codecFrameSize])/2.0f;

      MOVE(1);
      timeData[kk + codecFrameSize] = timeData[kk];
    }
  }


  PTR_INIT(3); ADD(1); FUNC(11);
  sbr_dec (keyflag,
		   &SbrChannel[0].SbrDec,
           timeData,
           pWorkBuffer1,
           /*InterimResult*/getInterimResultSbr(keyflag,0),
           hHeaderData,
           hFrameDataLeft,
           SbrChannel[0].hPrevFrameData,
           (hHeaderData->syncState == SBR_ACTIVE),
           getSbrDecoderInstanceDEC(keyflag)/*&sbrDecoderInstance.ParametricStereoDec*/,
           &SbrChannel[1].SbrDec.SynthesisQmfBank, 
           *numChannels);



  LOGIC(2); BRANCH(1);
  if (!bBitstreamDownMix && (stereo || dualMono)) {
    FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(codecFrameSize);
    memcpy( timeData, pWorkBuffer1, codecFrameSize*sizeof(float) );

    FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(SBR_FRAME_DATA));
    memcpy( hFrameDataLeft, hFrameDataRight, sizeof(SBR_FRAME_DATA) );

    MOVE(1);
    hFrameDataRight = hFrameDataLeft;

    FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(codecFrameSize);
    memcpy( getInterimResultSbr(keyflag,0)/*InterimResult*/, pWorkBuffer1+codecFrameSize, codecFrameSize*sizeof(float) );

    PTR_INIT(3); ADD(1); FUNC(11);
    sbr_dec (keyflag,
			 &SbrChannel[1].SbrDec,
             timeData + codecFrameSize,
             pWorkBuffer1,
             getInterimResultSbr(keyflag,0)/*InterimResult*/,
             hHeaderData,
             hFrameDataRight,
             SbrChannel[1].hPrevFrameData,
             (hHeaderData->syncState == SBR_ACTIVE),
             NULL,
             NULL,
             *numChannels);

    BRANCH(1);
    if(bDownSample){
      FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(codecFrameSize);
      memcpy( timeData+codecFrameSize, pWorkBuffer1, codecFrameSize*sizeof(float) );
    }
    else{
      FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(codecFrameSize);
      memcpy( &(timeData[codecFrameSize]), getInterimResultSbr(keyflag,0)/*InterimResult*/, codecFrameSize*sizeof(float) );
    }
  }
  else {

    ADD(1); BRANCH(1);
    if(hHeaderData->channelMode == PS_STEREO) {
      MOVE(1);
      *numChannels = 2;
    }
    else  {

      BRANCH(1);
      if(bDownSample){
        FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(codecFrameSize);
        memcpy( timeData, pWorkBuffer1, codecFrameSize*sizeof(float) );
        
        FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(codecFrameSize);
        memcpy( timeData+codecFrameSize, pWorkBuffer1, codecFrameSize*sizeof(float) );
      }
      else{
        FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(2*codecFrameSize);
        memcpy( timeData, pWorkBuffer1, 2*codecFrameSize*sizeof(float) );
      }
    }
  }
  
  COUNT_sub_end();

  return SBRDEC_OK;
}




/*!
  \brief     SBR bitstream data conversion from stereo to mono.

 \return none.
*/
static void
map2channelSbrData2one(HANDLE_SBR_FRAME_DATA hFrameDataLeft,
                       HANDLE_SBR_FRAME_DATA hFrameDataRight,
                       HANDLE_SBR_HEADER_DATA hHeaderData){
  int band, env, envLeft, envRight, offset, offsetLeft,offsetRight, nScaleFactors;

  HANDLE_FREQ_BAND_DATA hFreq  = hHeaderData->hFreqBandData;
  FRAME_INFO *frameInfoLeft    = &hFrameDataLeft->frameInfo;
  FRAME_INFO *frameInfoRight   = &hFrameDataRight->frameInfo;

  FRAME_INFO frameInfoMerge;
  char addHarmonicsMerge[MAX_FREQ_COEFFS];

  unsigned char bordersTemp[MAX_ENVELOPES*2 +2];
  int nEnvelopes = 1;

  INVF_MODE sbr_invf_modeMerge[MAX_INVF_BANDS];

  float  iEnvelopeMerge[MAX_NUM_ENVELOPE_VALUES];
  float  sbrNoiseFloorLevelMerge[MAX_NUM_NOISE_VALUES];

  float*  iEnvelopeLeft           = hFrameDataLeft->iEnvelope;
  float*  iEnvelopeRight          = hFrameDataRight->iEnvelope;
  float*  sbrNoiseFloorLevelLeft  = hFrameDataLeft->sbrNoiseFloorLevel;
  float*  sbrNoiseFloorLevelRight = hFrameDataRight->sbrNoiseFloorLevel;

  unsigned char noSubbands   = hFreq->highSubband - hFreq->lowSubband;
  unsigned char noNoiseBands = hFreq->nNfb;
  unsigned char * nSfb       = hFreq->nSfb;

  COUNT_sub_start("map2channelSbrData2one");

  INDIRECT(10); MOVE(3); PTR_INIT(8); /* counting previous operations */

  PTR_INIT(3); /* sbr_invf_modeMerge[]
                  hFrameDataLeft->sbr_invf_mode[]
                  hFrameDataRight->sbr_invf_mode[]
               */
  LOOP(1);
  for(band = 0; band < noNoiseBands; band++){
    ADD(1); BRANCH(1); MOVE(1);
    sbr_invf_modeMerge[band] = max(hFrameDataLeft->sbr_invf_mode[band],
                                   hFrameDataRight->sbr_invf_mode[band]);
  }

  PTR_INIT(3); /* addHarmonicsMerge[]
                  hFrameDataLeft->addHarmonics[]
                  hFrameDataRight->addHarmonics[]
               */
  LOOP(1);
  for(band = 0; band < noSubbands; band++){
    LOGIC(1); STORE(1);
    addHarmonicsMerge[band] = (hFrameDataLeft->addHarmonics[band] ||
                               hFrameDataRight->addHarmonics[band]);
  }


  INDIRECT(2); ADD(1); BRANCH(1); MOVE(1);
  bordersTemp[0] = max(frameInfoLeft->borders[0], frameInfoRight->borders[0]);

  INDIRECT(3); ADD(1); BRANCH(1); MOVE(1);
  bordersTemp[nEnvelopes] = max(frameInfoLeft->borders[frameInfoLeft->nEnvelopes],
                                frameInfoRight->borders[frameInfoRight->nEnvelopes]);


  MOVE(1);
  envLeft = 0;

  PTR_INIT(1); /* frameInfoLeft->borders[] */
  LOOP(1);
  while(frameInfoLeft->borders[envLeft] <= bordersTemp[0]) {
    ADD(1); /* while() condition */

    envLeft++;
  }

  MOVE(1);
  envRight = 0;

  PTR_INIT(1); /* frameInfoRight->borders[] */
  LOOP(1);
  while(frameInfoRight->borders[envRight] <= bordersTemp[0]) {
    ADD(1); /* while() condition */

    envRight++;
  }

  PTR_INIT(1); /* bordersTemp[] */
  INDIRECT(2); LOOP(1);
  while(envLeft <= frameInfoLeft->nEnvelopes && envRight <= frameInfoRight->nEnvelopes){
    unsigned char borderLeftChannel  = frameInfoLeft->borders[envLeft];
    unsigned char borderRightChannel = frameInfoRight->borders[envRight];

    MOVE(2); /* counting previous operations */
    ADD(2); LOGIC(1); /* while() condition */

    MOVE(1);
    bordersTemp[nEnvelopes + 1] = bordersTemp[nEnvelopes];

    ADD(1); BRANCH(1);
    if (borderLeftChannel < borderRightChannel){
      MOVE(1);
      bordersTemp[nEnvelopes] = frameInfoLeft->borders[envLeft];

      envLeft++;
    }
    else{
      MOVE(1);
      bordersTemp[nEnvelopes] = frameInfoRight->borders[envRight];

      envRight++;
    }

    ADD(1);
    nEnvelopes++;
  }


  PTR_INIT(1); /* bordersTemp[] */
  LOOP(1);
  for(env = 0; env < nEnvelopes - 1; env++){

    LOOP(1);
    while(bordersTemp[env + 1] - bordersTemp[env] < 2 && env < nEnvelopes - 1){
      ADD(4); LOGIC(1); /* while() condition */

      FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(nEnvelopes - env - 1);
      memcpy(bordersTemp + env + 1, bordersTemp + env  + 2, (nEnvelopes - env - 1)*sizeof(unsigned char));

      ADD(1);
      nEnvelopes--;
    }
  }


  INDIRECT(1); ADD(2); BRANCH(1);
  if(bordersTemp[nEnvelopes] - bordersTemp[nEnvelopes - 1] < 2){

    MOVE(1);
    bordersTemp[nEnvelopes - 1] = bordersTemp[nEnvelopes];

    ADD(1);
    nEnvelopes--;
  }


  LOOP(1);
  while(nEnvelopes > 5){

    PTR_INIT(1); /* bordersTemp[] */
    LOOP(1);
    for(env = nEnvelopes; env > 0; env--){

      ADD(2); BRANCH(1);
      if(bordersTemp[env] - bordersTemp[env - 1] < 4)
        break;
    }

    FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(nEnvelopes - env - 1);
    memcpy(bordersTemp + env - 1, bordersTemp + env, (nEnvelopes - env + 1 )*sizeof(unsigned char));

    ADD(1);
    nEnvelopes--;
  }


  MOVE(1);
  frameInfoMerge.nEnvelopes = nEnvelopes;

  PTR_INIT(2); /* frameInfoMerge.borders[]
                  bordersTemp[]
               */
  LOOP(1);
  for(env = 0; env <= nEnvelopes; env++){

    MOVE(1);
    frameInfoMerge.borders[env] = bordersTemp[env];
  }


  INDIRECT(1); ADD(1); BRANCH(1);
  if(frameInfoLeft->tranEnv == -1){

    MOVE(1);
    frameInfoMerge.tranEnv = frameInfoRight->tranEnv;
  }
  else{
    INDIRECT(1); ADD(1); BRANCH(1);
    if(frameInfoRight->tranEnv == -1){

      MOVE(1);
      frameInfoMerge.tranEnv = frameInfoLeft->tranEnv;
    }
    else{

      ADD(1); BRANCH(1); MOVE(1);
      frameInfoMerge.tranEnv = min(frameInfoLeft->tranEnv, frameInfoRight->tranEnv);
    }
  }


  INDIRECT(2); ADD(1); BRANCH(1); MOVE(1);
  frameInfoMerge.nNoiseEnvelopes = max(frameInfoLeft->nNoiseEnvelopes,frameInfoRight->nNoiseEnvelopes);

  ADD(1); BRANCH(1);
  if(frameInfoMerge.nNoiseEnvelopes > 1){

    ADD(1); BRANCH(1);
    if(frameInfoLeft->nNoiseEnvelopes < frameInfoRight->nNoiseEnvelopes){

      MOVE(1);
      frameInfoMerge.bordersNoise[0] = frameInfoMerge.borders[0];

      MOVE(1);
      frameInfoMerge.bordersNoise[1] = frameInfoRight->bordersNoise[1];

      MOVE(1); INDIRECT(1);
      frameInfoMerge.bordersNoise[2] = frameInfoMerge.borders[frameInfoMerge.nEnvelopes];
    }
    else{

      ADD(1); BRANCH(1);
      if(frameInfoRight->nNoiseEnvelopes < frameInfoLeft->nNoiseEnvelopes){

        MOVE(1);
        frameInfoMerge.bordersNoise[0] = frameInfoMerge.borders[0];

        MOVE(1);
        frameInfoMerge.bordersNoise[1] = frameInfoLeft->bordersNoise[1];

        MOVE(1); INDIRECT(1);
        frameInfoMerge.bordersNoise[2] = frameInfoMerge.borders[frameInfoMerge.nEnvelopes];
      }
      else{

        MOVE(1);
        frameInfoMerge.bordersNoise[0] = frameInfoMerge.borders[0];

        INDIRECT(2); ADD(1); BRANCH(1); MOVE(1);
        frameInfoMerge.bordersNoise[1] = min(frameInfoLeft->bordersNoise[1],frameInfoRight->bordersNoise[1]);

        MOVE(1); INDIRECT(1);
        frameInfoMerge.bordersNoise[2] = frameInfoMerge.borders[frameInfoMerge.nEnvelopes];
      }
    }
  }
  else{

    MOVE(1);
    frameInfoMerge.bordersNoise[0] = frameInfoMerge.borders[0];

    MOVE(1); INDIRECT(1);
    frameInfoMerge.bordersNoise[1] = frameInfoMerge.borders[frameInfoMerge.nEnvelopes];
  }


  ADD(1); BRANCH(1);
  if(frameInfoMerge.nNoiseEnvelopes > 1){

    PTR_INIT(1); /* frameInfoMerge.borders[] */
    LOOP(1);
    for(env = 1; env < frameInfoMerge.nEnvelopes; env++){

      ADD(1); BRANCH(1);
      if(frameInfoMerge.borders[env] >= frameInfoMerge.bordersNoise[1]){
        break;
      }
    }

    MOVE(1);
    frameInfoMerge.bordersNoise[1] = frameInfoMerge.borders[env];
  }


  MOVE(5);
  envLeft     = 0;
  envRight    = 0;
  offset      = 0;
  offsetRight = 0;
  offsetLeft  = 0;

  PTR_INIT(4); /* frameInfoLeft->freqRes[]
                  frameInfoRight->freqRes]
                  frameInfoMerge.borders[]
                  frameInfoMerge.freqRes[]
               */
  LOOP(1);
  for(env = 0; env < frameInfoMerge.nEnvelopes;env++){
    unsigned char freqResLeft  = frameInfoLeft->freqRes[envLeft];
    unsigned char freqResRight = frameInfoRight->freqRes[envRight];
    unsigned char freqRes;
    int bandLeft  = 0;
    int bandRight = 0;
    int band;

    MOVE(4); /* counting previous operation */

    ADD(2); BRANCH(1);
    if(frameInfoMerge.borders[env + 1] > frameInfoLeft->borders[envLeft+1]){

      ADD(2); BRANCH(1);
      if(envLeft < frameInfoLeft->nEnvelopes - 1){

        INDIRECT(1); ADD(1);
        offsetLeft += nSfb[freqResLeft];

        envLeft++;
      }
    }

    ADD(2); BRANCH(1);
    if(frameInfoMerge.borders[env + 1] > frameInfoRight->borders[envRight+1]){

      ADD(2); BRANCH(1);
      if(envRight < frameInfoRight->nEnvelopes - 1){

        INDIRECT(1); ADD(1);
        offsetRight += nSfb[freqResRight];

        envRight++;
      }
    }

    MOVE(2);
    freqResLeft  = frameInfoLeft->freqRes[envLeft];
    freqResRight = frameInfoRight->freqRes[envRight];

    LOGIC(1);
    freqRes      = freqResLeft || freqResRight;

    MOVE(1);
    frameInfoMerge.freqRes[env] = freqRes;

    PTR_INIT(3); /* iEnvelopeMerge[]
                    iEnvelopeLeft[]
                    iEnvelopeRight[]
                 */
    INDIRECT(1); LOOP(1);
    for(band = 0; band < nSfb[freqRes]; band++){
      unsigned char ui      = hFreq->freqBandTable[freqRes     ][band+1];
      unsigned char uiLeft  = hFreq->freqBandTable[freqResLeft ][bandLeft+1];
      unsigned char uiRight = hFreq->freqBandTable[freqResRight][bandRight+1];

      INDIRECT(3); MOVE(3); /* counting previous operations */

      ADD(1); BRANCH(1);
      if(ui > uiLeft) {

        ADD(1);
        bandLeft++;
      }

      ADD(1); BRANCH(1);
      if(ui > uiRight) {

        ADD(1);
        bandRight++;
      }

      ADD(1); DIV(1); STORE(1);
      iEnvelopeMerge[band + offset] = (iEnvelopeLeft[bandLeft + offsetLeft] +
                                       iEnvelopeRight[bandRight + offsetRight])/2.0f;
    }
    INDIRECT(1); ADD(1);
    offset      +=  nSfb[freqRes];
  }

  MOVE(1);
  nScaleFactors = offset;



  MOVE(5);
  envLeft     = 0;
  envRight    = 0;
  offset      = 0;
  offsetRight = 0;
  offsetLeft  = 0;

  PTR_INIT(3); /* frameInfoMerge.bordersNoise[]
                  frameInfoLeft->bordersNoise[]
                  frameInfoRight->bordersNoise[]
               */
  LOOP(1);
  for(env = 0; env < frameInfoMerge.nNoiseEnvelopes; env++){
    int band;

    ADD(3); LOGIC(1); BRANCH(1);
    if(frameInfoMerge.bordersNoise[env] > frameInfoLeft->bordersNoise[envLeft] &&
       envLeft < frameInfoLeft->nNoiseEnvelopes - 1){

      ADD(2);
      envLeft++;
      offsetLeft += noNoiseBands;
    }

    ADD(3); LOGIC(1); BRANCH(1);
    if(frameInfoMerge.bordersNoise[env] > frameInfoRight->bordersNoise[envRight] &&
      envRight < frameInfoRight->nNoiseEnvelopes - 1){

      ADD(2);
      envRight++;
      offsetRight += noNoiseBands;
    }

    PTR_INIT(3); /* sbrNoiseFloorLevelMerge[]
                    sbrNoiseFloorLevelLeft[]
                    sbrNoiseFloorLevelRight[]
                 */
    LOOP(1);
    for(band = 0; band < noNoiseBands; band++){

      ADD(1); DIV(1); STORE(1);
      sbrNoiseFloorLevelMerge[band + offset] = (sbrNoiseFloorLevelLeft[band + offsetLeft] +
                                                sbrNoiseFloorLevelRight[band + offsetRight])/2.0f;
    }

    ADD(1);
    offset += noNoiseBands;
  }



  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(sizeof(FRAME_INFO));
  memcpy(frameInfoLeft,&frameInfoMerge,sizeof(FRAME_INFO));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_ENVELOPES+1);
  memcpy(frameInfoLeft->borders, frameInfoMerge.borders, (MAX_ENVELOPES+1)*sizeof(unsigned char));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_ENVELOPES);
  memcpy(frameInfoLeft->freqRes, frameInfoMerge.freqRes, (MAX_ENVELOPES)*sizeof(unsigned char));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_NOISE_ENVELOPES+1);
  memcpy(frameInfoLeft->bordersNoise, frameInfoMerge.bordersNoise, (MAX_NOISE_ENVELOPES+1)*sizeof(unsigned char));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_NUM_ENVELOPE_VALUES);
  memcpy(iEnvelopeLeft, iEnvelopeMerge, MAX_NUM_ENVELOPE_VALUES*sizeof(float));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_NUM_NOISE_VALUES);
  memcpy(sbrNoiseFloorLevelLeft, sbrNoiseFloorLevelMerge, MAX_NUM_NOISE_VALUES*sizeof(float));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_FREQ_COEFFS);
  memcpy(hFrameDataLeft->addHarmonics, addHarmonicsMerge,MAX_FREQ_COEFFS*sizeof(char));

  FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(MAX_INVF_BANDS);
  memcpy(hFrameDataLeft->sbr_invf_mode, sbr_invf_modeMerge, MAX_INVF_BANDS*sizeof(INVF_MODE));

  INDIRECT(1); MOVE(1);
  hFrameDataLeft->nScaleFactors = nScaleFactors;

  COUNT_sub_end();
}


