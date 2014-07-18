/*
  decoder main
*/

#include <assert.h>
#include "aac_rom.h"
#include "aac_ram.h"
#include "aacdecoder.h"
#include "FFR_bitbuffer.h"

#include "FFR_aacPLUScheck.h"
#include "dse.h"
#include <map>
using namespace std;
#include "LibMutex.h"



struct AAC_DECODER_INSTANCE {
  unsigned char frameOK;   /*!< Will be unset if the CRC, a consistency check etc. fails */
  unsigned long bitCount;
  long byteAlignBits;
  HANDLE_BIT_BUF pBs;
  float *pTimeData;
  SBRBITSTREAM *pStreamSbr;
  CStreamInfo *pStreamInfo;
  CAacDecoderChannelInfo *pAacDecoderChannelInfo[Channels];
  CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo[Channels];
  int BlockNumber;
};

class AAC_DECODER_INSTANCE_AUTO_DELTE
{
public:
	AAC_DECODER_INSTANCE_AUTO_DELTE(AACDECODER pAac){
		m_pAacDecoder = pAac;
	}

	~AAC_DECODER_INSTANCE_AUTO_DELTE(){
		if (m_pAacDecoder)
			delete m_pAacDecoder;
		m_pAacDecoder = NULL;
	}

	AACDECODER getAacDecoder(){ return m_pAacDecoder;}
private:
	AACDECODER m_pAacDecoder;
};

static unsigned int g_keyFlag = 0;
static map<unsigned int,AAC_DECODER_INSTANCE_AUTO_DELTE*> g_mapAacDecoderInstance;
LibMutex g_mapAacDecoderInstanceMutex;

#include "conceal.h"
#include "counters.h" /* the 3GPP instrumenting tool */

void CPns_InitPns(CAacDecoderChannelInfo *pAacDecoderChannelInfo);
void CPns_InitInterChannelData(CAacDecoderChannelInfo *pAacDecoderChannelInfo);


/*
  The function initializes the pointers to AacDecoderChannelInfo for each channel,
  set the start values for window shape and window sequence of overlap&add to zero,
  set the overlap buffer to zero and initializes the pointers to the window coefficients.
*/
AACDECODER CAacDecoderOpen(unsigned int& keyflag,
						   HANDLE_BIT_BUF pBs,       /*!< pointer to bitbuffer structure */
                           SBRBITSTREAM *pStreamSbr, /*!< pointer to sbr bitstream structure */
                           float *&pTimeData)         /*!< pointer to time data */
{
  int i,ch;

  LibMutexLocker locker(&g_mapAacDecoderInstanceMutex);
  AACDECODER pAacDecoder = new struct AAC_DECODER_INSTANCE();
  AAC_DECODER_INSTANCE_AUTO_DELTE* AacDecoderInstances = new AAC_DECODER_INSTANCE_AUTO_DELTE(pAacDecoder);

  keyflag = ++g_keyFlag;

  while (g_mapAacDecoderInstance.find(keyflag)!=g_mapAacDecoderInstance.end())
  {
	  keyflag = ++g_keyFlag;
  }
  
  pAacDecoder->BlockNumber = 0;
  g_mapAacDecoderInstance.insert(pair<unsigned int,AAC_DECODER_INSTANCE_AUTO_DELTE*>(keyflag,AacDecoderInstances));


  pTimeData = getTimeDataFloat(keyflag,0);

  COUNT_sub_start("CAacDecoderOpen");

  /* initialize bit counter for syncroutine */
  MOVE(1);
  pAacDecoder->bitCount = 0;

  /* initialize pointer to bit buffer structure */
  MOVE(1);
  pAacDecoder->pBs = pBs;

  /* initialize pointer to time data */
  MOVE(1);
  pAacDecoder->pTimeData = pTimeData;

  /* initialize pointer to sbr bitstream structure */
  MOVE(1);
  pAacDecoder->pStreamSbr = pStreamSbr;

  /* initialize stream info */
  PTR_INIT(1); FUNC(1);
  CStreamInfoOpen(keyflag,&pAacDecoder->pStreamInfo);

  /* initialize CAacDecoderChannelInfo */
  PTR_INIT(2); /* AacDecoderInstance.pAacDecoderStaticChannelInfo[ch] 
                  AacDecoderStaticChannelInfo[ch]
               */
  LOOP(1);
  for (ch=0; ch<Channels; ch++)
  {
    /* initialize pointer to CAacDecoderStaticChannelInfo for each channel */
    MOVE(1);
    pAacDecoder->pAacDecoderStaticChannelInfo[ch] = getCAacDecoderStaticChannelInfo(keyflag,ch);/*&AacDecoderStaticChannelInfo[ch]*/

    /* initialize overlap & add for each channel */
    MOVE(2);
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->OverlapAddData.WindowShape = 0;
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->OverlapAddData.WindowSequence = 0;

    PTR_INIT(1); MOVE(1);
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->OverlapAddData.pOverlapBuffer = getOverlapBuffer(keyflag,ch*OverlapBufferSize)/*&OverlapBuffer[ch*OverlapBufferSize]*/;

    PTR_INIT(1); /* AacDecoderInstance.pAacDecoderStaticChannelInfo[ch]->OverlapAddData.pOverlapBuffer[i] */
    LOOP(1);
    for (i=0; i<FRAME_SIZE/2; i++)
    {
      MOVE(1);
      pAacDecoder->pAacDecoderStaticChannelInfo[ch]->OverlapAddData.pOverlapBuffer[i] = 0.0;
    }

    /* initialize window shapes for each channel */
    MOVE(4);
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->pLongWindow[0] = OnlyLongWindowSine;
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->pShortWindow[0] = OnlyShortWindowSine;
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->pLongWindow[1] = OnlyLongWindowKBD;
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->pShortWindow[1] = OnlyShortWindowKBD;

    PTR_INIT(1); MOVE(1);
    pAacDecoder->pAacDecoderStaticChannelInfo[ch]->hConcealmentInfo = getConcealmentInfo(keyflag,ch)/*&ConcealmentInfo[ch]*/;

    FUNC(1);
    CConcealment_Init (pAacDecoder->pAacDecoderStaticChannelInfo[ch]);
  }

  /* these are static, but we access them via pointers inside the dynamic data */
  getCPnsStaticInterChannelData(keyflag)->current_seed = 0;
  getCPnsStaticInterChannelData(keyflag)->pns_frame_number = 0;

  PTR_INIT(1); /* counting post-operation */

  COUNT_sub_end();

  return pAacDecoder;
}

/*!
\brief Initialization of AacDecoderChannelInfo

\return none
*/
int CAacDecoderClose(unsigned int keyflag)
{
	LibMutexLocker locker(&g_mapAacDecoderInstanceMutex);
	map<unsigned int,AAC_DECODER_INSTANCE_AUTO_DELTE*>::iterator iter = g_mapAacDecoderInstance.find(keyflag);
	if (iter!=g_mapAacDecoderInstance.end())
	{
		delete iter->second;
		g_mapAacDecoderInstance.erase(iter);
	}
	ReleaseMapAacDecoderDataInfo(keyflag);
	return 0;
}

int CAacDecoderInit(AACDECODER self,
                    int samplingRate,
                    int bitrate)
{
  int i;
  int numEntries = sizeof(SamplingRateInfoTable)/sizeof(SamplingRateInfo);

  COUNT_sub_start("CAacDecoderInit");

  MOVE(1); /* counting previous operation */

  BRANCH(1);
  if (!self)
  {
    COUNT_sub_end();
    return -1;
  }

  INDIRECT(1); MOVE(1);
  self->pStreamInfo->SamplingRate  = samplingRate;
  
  PTR_INIT(1); /* SamplingRateInfoTable[i] */
  LOOP(1);
  for (i=0; i<numEntries; i++)
  {
    ADD(1); BRANCH(1);
    if (samplingRate == SamplingRateInfoTable[i].SamplingFrequency)
      break;
  }

  ADD(1); BRANCH(1);
  if (i == numEntries)
  {
    COUNT_sub_end();
    return -1;
  }

  MOVE(2);
  self->pStreamInfo->SamplingRateIndex = i;
  self->pStreamInfo->BitRate = bitrate;

  COUNT_sub_end();
  
  return 0;
}

/*!
  The function decodes one aac frame. The decoding of coupling channel
  elements are not supported. The transport layer might signal, that the
  data of the current frame is invalid, e.g. as a result of a packet
  loss in streaming mode.
*/
int CAacDecoder_DecodeFrame(unsigned int keyflag,
							AACDECODER self,            /*!< pointer to aacdecoder structure */
                            int *frameSize,             /*!< pointer to frame size */
                            int *sampleRate,            /*!< pointer to sample rate */
                            int *numChannels,           /*!< pointer to number of channels */
                            char *channelMode,          /*!< mode: 0=mono, 1=stereo */
                            char frameOK                /*!< indicates if current frame data is valid */
                            )
{
  unsigned char aacChannels=0;
  long tmp;
  unsigned char ch;
  int type = 0;
  int ErrorStatus = frameOK;
  HANDLE_BIT_BUF bs = self->pBs;
  int previous_element;
  int channel;

  COUNT_sub_start("CAacDecoder_DecodeFrame");

  MOVE(7); INDIRECT(1); PTR_INIT(1); /* counting previous operations */

  /* initialize pointer to CAacDecoderChannelInfo for each channel */
  PTR_INIT(4); /* self->pAacDecoderChannelInfo[ch]
                  pAacDecoderChannelInfoInit[ch];
                  pAacDecoderDynamicDataInit[ch]
                  WorkBufferCore[ch*MaximumBinsLong];
               */
  LOOP(1);
  for (ch=0; ch<Channels; ch++)
  {
    INDIRECT(1); PTR_INIT(5);
    self->pAacDecoderChannelInfo[ch] = getAacDecoderChannelInfoInit(keyflag,ch)/*pAacDecoderChannelInfoInit[ch]*/;
    self->pAacDecoderChannelInfo[ch]->pCodeBook = getAacDecoderDynamicDataInit(keyflag,ch)->aCodeBook/*pAacDecoderDynamicDataInit[ch]->aCodeBook*/;
    self->pAacDecoderChannelInfo[ch]->pScaleFactor = getAacDecoderDynamicDataInit(keyflag,ch)->aScaleFactor/*pAacDecoderDynamicDataInit[ch]->aScaleFactor*/;
    self->pAacDecoderChannelInfo[ch]->pSpectralCoefficient = getWorkBufferCoreArray(keyflag,ch*MaximumBinsLong)/*&WorkBufferCore[ch*MaximumBinsLong]*/;
    self->pAacDecoderChannelInfo[ch]->pJointStereoData = &(getAacDecoderDynamicCommonDataInit(keyflag)->JointStereoData)/*&pAacDecoderDynamicCommonDataInit->JointStereoData*/;
    self->pAacDecoderChannelInfo[ch]->pPnsInterChannelData = &(getAacDecoderDynamicCommonDataInit(keyflag)->PnsInterChannelData)/*&pAacDecoderDynamicCommonDataInit->PnsInterChannelData*/;
    self->pAacDecoderChannelInfo[ch]->pPnsStaticInterChannelData = getCPnsStaticInterChannelData(keyflag)/*&PnsStaticInterChannelData*/;
  }

  INDIRECT(1); MOVE(1);
  self->frameOK = 1;

  PTR_INIT(1); /* self->pAacDecoderChannelInfo[channel] */
  LOOP(1);
  for (channel=0; channel < Channels; channel++)
  {
    FUNC(1);
    CPns_InitPns(self->pAacDecoderChannelInfo[channel]);
  }

  INDIRECT(1); FUNC(1);
  CPns_InitInterChannelData(self->pAacDecoderChannelInfo[0]);

  MOVE(1);
  aacChannels = 0;

  INDIRECT(1); PTR_INIT(1); FUNC(2);
  ByteAlign(bs,&self->byteAlignBits);

  MOVE(1);
  previous_element = ID_END;

  LOOP(1);
  while ( (type != ID_END) && self->frameOK )
  {
    ADD(1); LOGIC(1); /* while() condition */

    FUNC(2);
    type = GetBits(bs,3);

    INDIRECT(1); BRANCH(1);
    if (bs->cntBits < 0)
    {
      INDIRECT(1); MOVE(1);
      self->frameOK = 0;
    }

    BRANCH(1);
    switch (type)
    {
      case ID_SCE:
      
        /*
          Consistency check
        */

        ADD(1); BRANCH(1);
        if(aacChannels >= Channels){

          INDIRECT(1); MOVE(1);
          self->frameOK = 0;
          break;
        }
          
        INDIRECT(1); MOVE(1);
        self->pStreamSbr->sbrElement[self->pStreamSbr->NrElements].ElementID = SBR_ID_SCE;

        INDIRECT(1); BRANCH(1); 
        if(self->frameOK) {

          INDIRECT(3); PTR_INIT(1); FUNC(3);
          ErrorStatus = CSingleChannelElement_Read(bs,self->pAacDecoderChannelInfo,self->pStreamInfo);

          BRANCH(1);
          if (ErrorStatus) {

            INDIRECT(1); MOVE(1);
            self->frameOK = 0;
          }
        }

        INDIRECT(1); BRANCH(1);
        if ( self->frameOK )
        {
          INDIRECT(1); FUNC(1);
          CSingleChannelElement_Decode(self->pAacDecoderChannelInfo);

          ADD(1);
          aacChannels += 1;
        }

        break;
      
      case ID_CPE:
       
        /*
          Consistency check
        */
        ADD(1); BRANCH(1);
        if(aacChannels >= Channels){

          INDIRECT(1); MOVE(1);
          self->frameOK = 0;
          break;
        }
          
        INDIRECT(1); MOVE(1);
        self->pStreamSbr->sbrElement[self->pStreamSbr->NrElements].ElementID = SBR_ID_CPE;


        INDIRECT(1); BRANCH(1);
        if (self->frameOK) {

          INDIRECT(3); PTR_INIT(1); FUNC(3);
          ErrorStatus = CChannelPairElement_Read(bs,self->pAacDecoderChannelInfo,self->pStreamInfo);

          BRANCH(1);
          if (ErrorStatus) {

            INDIRECT(1); MOVE(1);
            self->frameOK = 0;
          }
        }

        INDIRECT(1); BRANCH(1);
        if ( self->frameOK ){

          INDIRECT(1); FUNC(1);
          CChannelPairElement_Decode(self->pAacDecoderChannelInfo);

          ADD(1);
          aacChannels += 2;
        }

        break;
      
      case ID_CCE:
      
        INDIRECT(1); MOVE(2);
        ErrorStatus = AAC_DEC_UNIMPLEMENTED_CCE;
        self->frameOK = 0;
        break;

      case ID_LFE:

        INDIRECT(1); MOVE(2);
        ErrorStatus = AAC_DEC_UNIMPLEMENTED_LFE;
        self->frameOK = 0;
        break;
      
      case ID_DSE:

        INDIRECT(1); PTR_INIT(1); FUNC(2);
        CDse_Read(bs, &self->byteAlignBits);
        break;

      case ID_PCE:

        INDIRECT(1); MOVE(2);
        ErrorStatus = AAC_DEC_UNIMPLEMENTED_PCE;
        self->frameOK = 0;

        break;
      
      case ID_FIL:

        INDIRECT(1); MOVE(1);
        tmp = bs->cntBits;

        FFRaacplus_checkForPayload(bs,self->pStreamSbr,previous_element);
        break;
      
      case ID_END:
        break;
    }

    MOVE(1);
    previous_element = type;
  }


  /* Update number of channels (if valid) */
  INDIRECT(1); BRANCH(1);
  if (self->frameOK) {

    INDIRECT(1); MOVE(1);
    self->pStreamInfo->Channels = aacChannels;
  }

  INDIRECT(1); LOGIC(1); STORE(1);
  self->frameOK = self->frameOK && frameOK;

  /*
    Inverse transform
  */
  PTR_INIT(3); /* self->pAacDecoderStaticChannelInfo[ch]
                  self->pAacDecoderChannelInfo[ch]
                  self->pTimeData[ch*MaximumBinsLong]
               */
  INDIRECT(1); LOOP(1);
  for (ch=0; ch<self->pStreamInfo->Channels; ch++) {

    /*
      Conceal erred spectral data 
    */
    FUNC(3);
    CConcealment_Apply(self->pAacDecoderStaticChannelInfo[ch],
                       self->pAacDecoderChannelInfo[ch],
                       self->frameOK);
  
  
    PTR_INIT(1); FUNC(1); BRANCH(1);
    if (IsLongBlock(&self->pAacDecoderChannelInfo[ch]->IcsInfo))
    {
      PTR_INIT(1); FUNC(4);
      CLongBlock_FrequencyToTime(self->pAacDecoderStaticChannelInfo[ch],
                                 self->pAacDecoderChannelInfo[ch],
                                 &self->pTimeData[ch*MaximumBinsLong],
                                 1);
    }
    else
    {
      PTR_INIT(1); FUNC(4);
      CShortBlock_FrequencyToTime(self->pAacDecoderStaticChannelInfo[ch],
                                  self->pAacDecoderChannelInfo[ch],
                                  &self->pTimeData[ch*MaximumBinsLong],
                                  1);
    }
  }


  INDIRECT(1); PTR_INIT(1); FUNC(2);
  ByteAlign(bs,&self->byteAlignBits);

  INDIRECT(3); MOVE(3);
  *frameSize = self->pStreamInfo->SamplesPerFrame;
  *sampleRate = self->pStreamInfo->SamplingRate;
  *numChannels = self->pStreamInfo->Channels;

  ADD(1); BRANCH(1);
  if (*numChannels == 1)
  {
    MOVE(1);
    *channelMode = 0;
  }
  else {
    BRANCH(1); MOVE(1);
    if (*numChannels == 2)
      *channelMode = 1;
    else
      *channelMode = 2;
  }

  ADD(1);
  self->BlockNumber++;

  COUNT_sub_end();

  return ErrorStatus;
}



