#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "FloatFR.h"
#include "au_channel.h"
#include "aacenc.h"
#include "iir32resample.h"
#include "resampler.h"

/* for MPEG-4 file format support */
#include "mp4file.h"
#include "ISOMovies.h"

#include "counters.h" /* the 3GPP instrumenting tool */

#include "sbr_main.h"
#include "aac_ram.h"
#include "aac_rom.h"

/* dynamic buffer of SBR that can be reused for resampling */
extern float sbr_envRBuffer[];

#define CORE_DELAY   (1600)
#define INPUT_DELAY  ((CORE_DELAY)*2 +6*64-2048+1)     /* ((1600 (core codec)*2 (multi rate) + 6*64 (sbr dec delay) - 2048 (sbr enc delay) + magic*/
#define MAX_DS_FILTER_DELAY 16                         /* the additional max resampler filter delay (source fs)*/

/*

      input buffer (1ch)

      |------------ 1537   -------------|-----|---------- 2048 -------------|
           (core2sbr delay     )          ds     (read, core and ds area)
*/
static float inputBuffer[(AACENC_BLOCKSIZE*2 + MAX_DS_FILTER_DELAY + INPUT_DELAY)*MAX_CHANNELS];
unsigned int outputBuffer[(6144/8)*MAX_CHANNELS/(sizeof(int))];


static IIR21_RESAMPLER IIR21_reSampler[MAX_CHANNELS];



/* the following variables are used during initialization */
static unsigned int sampleRateAAC;
static unsigned int sampleRateIn;
static int coreReadOffset = 0;
static int envWriteOffset = 0;
static int envReadOffset = 0;
static int useParametricStereo = 0;
static int writeOffset=INPUT_DELAY*MAX_CHANNELS;

static int bDoIIR32Resample = 0;
static int bDoIIR2Downsample = 0;

static int bDoUpsample = 0;
static int upsampleReadOffset = 0;

static  struct AAC_ENCODER *aacEnc = 0;
static  HANDLE_SBR_ENCODER hEnvEnc=NULL;

static int
configureEncoder( unsigned int bitrate, AACENC_CONFIG* hConfig )
{
  COUNT_sub_start("main_configure");
  writeOffset=INPUT_DELAY*MAX_CHANNELS;
  /*
    set up SBR configuration
  */
  PTR_INIT(1); FUNC(4); BRANCH(1);
  if(!IsSbrSettingAvail (bitrate, hConfig->nChannelsOut, sampleRateIn, &sampleRateAAC)) {
    COUNT_sub_end();
    fprintf(stderr,"\nNo valid SBR configuration found\n");
    return 1;
  }

  {
    sbrConfiguration sbrConfig;

    MOVE(1);
    envReadOffset = 0;

    BRANCH(1);
    if(useParametricStereo)
    {
      MOVE(3);
      envReadOffset = (MAX_DS_FILTER_DELAY + INPUT_DELAY)*MAX_CHANNELS;
      writeOffset = envReadOffset;
    }

    PTR_INIT(1); FUNC(1);
    InitializeSbrDefaults (&sbrConfig);


    MOVE(1);
    sbrConfig.usePs = useParametricStereo;

    PTR_INIT(1); FUNC(6);
    AdjustSbrSettings(&sbrConfig,
                      bitrate,
                      hConfig->nChannelsOut,
                      sampleRateAAC,
                      AACENC_TRANS_FAC,
                      24000);

    PTR_INIT(3); ADD(1); FUNC(4);
    EnvOpen  (&hEnvEnc,
              inputBuffer,
              &sbrConfig,
              &hConfig->bandWidth);

    /* set IIR 2:1 downsampling */
    BRANCH(1); MOVE(1);
    bDoIIR2Downsample = (bDoUpsample) ? 0 : 1;

    BRANCH(1);
    if (useParametricStereo)
    {
      MOVE(1);
      bDoIIR2Downsample = 0;
    }
  }


  /*
    set up mode-dependant 1:2 upsampling
  */

  BRANCH(1);
  if (bDoUpsample) {
    int transitionFac;

    ADD(1); BRANCH(1);
    if (hConfig->nChannelsIn>1) {
      COUNT_sub_end();
      fprintf( stderr, "\n Stereo @ 16kHz input sample rate is not supported\n");
      return 1;
    }

    PTR_INIT(1); FUNC(1);
    InitIIR21_Resampler(&(IIR21_reSampler[0]));

#if (MAX_CHANNELS==2)
    PTR_INIT(1); FUNC(1);
    InitIIR21_Resampler(&(IIR21_reSampler[1]));
#endif

    assert(IIR21_reSampler[0].delay <=MAX_DS_FILTER_DELAY);

    BRANCH(1);
    if (useParametricStereo) {

      ADD(1);
      writeOffset   += AACENC_BLOCKSIZE * MAX_CHANNELS;

      MOVE(2);
      upsampleReadOffset  = writeOffset;
      envWriteOffset  = envReadOffset;
    }
    else
    {
      ADD(1);
      writeOffset        += AACENC_BLOCKSIZE * MAX_CHANNELS;

      MOVE(1);
      coreReadOffset      = writeOffset;

      ADD(2); SHIFT(1); MULT(1);
      upsampleReadOffset  = writeOffset - (((INPUT_DELAY-IIR21_reSampler[0].delay) >> 1) * MAX_CHANNELS);

      ADD(1); LOGIC(1); MULT(1);
      envWriteOffset      = ((INPUT_DELAY-IIR21_reSampler[0].delay) &  0x1) * MAX_CHANNELS;

      MOVE(1);
      envReadOffset       = 0;
    }
  }
  else {

    /*
      set up 2:1 downsampling
    */

    BRANCH(1);
    if (bDoIIR2Downsample){

      PTR_INIT(1); FUNC(1);
      InitIIR21_Resampler(&(IIR21_reSampler[0]));

#if (MAX_CHANNELS==2)
      PTR_INIT(1); FUNC(1);
      InitIIR21_Resampler(&(IIR21_reSampler[1]));
#endif

      assert(IIR21_reSampler[0].delay <=MAX_DS_FILTER_DELAY);

      MAC(1);
      writeOffset += IIR21_reSampler[0].delay*MAX_CHANNELS;
    }
  }

  BRANCH(1);
  if (bDoIIR32Resample)
  {
    FUNC(1);
    IIR32Init();
  }

  /*
     set up AAC encoder, now that samling rate is known
  */
  MOVE(1);
  hConfig->sampleRate = sampleRateAAC;

  PTR_INIT(1); FUNC(2); BRANCH(1);
  if ( AacEncOpen( &aacEnc, *hConfig) ) {
    COUNT_sub_end();
    fprintf(stderr,"\n Initialisation of AAC failed !\n");
    return 1;
  }

  COUNT_sub_end();
  return 0;
}


static void
getNextConfig(FILE *fc, int *nextFrame, int *nextBitrate)
{
  if (fc==NULL)
    return;

  if (fscanf(fc, "%d %d", nextFrame, nextBitrate) != 2) {
    *nextFrame = 0;
  }
}


int main(int argc, char *argv[])
{
  AuChanType auType = TYPE_AUTODETECT ; /* must be set */
  AuChanMode auFlags = AU_CHAN_READ;
  AuChanError auError;
  AuChanInfo inputInfo;
  hAudioChannel inputFile = NULL;

  FILE *fOut=NULL;
  FILE *fConfig=NULL;
  HANDLE_MP4_FILE hMp4File=NULL;

  AACENC_CONFIG     config;

  int nChannelsAAC, nChannelsSBR;
  int nextConfigFrame = 0, nextConfigBitrate = 0;

  int  bEncodeMono = 0;

  unsigned int bitrate;
  unsigned int frmCnt;
  int bandwidth = 0;

  unsigned int numAncDataBytes=0;
  unsigned char ancDataBytes[MAX_PAYLOAD_SIZE];
  unsigned int ancDataLength = 0;

  /*!< required only for interfacing with audio output library, thus not counted into RAM requirements */
  short TimeDataPcm[AACENC_BLOCKSIZE*2*MAX_CHANNELS];

  int numSamplesRead;
  int bDingleRate = 0;

  int inSamples;
  int nSamplesPerChannel;
  const int nRuns = 4;
  float *resamplerScratch = sbr_envRBuffer;


  /* initialize the 3GPP instrumenting tool */
  COUNT_init();

  fprintf(stderr,"\n");
  fprintf(stderr,"*************************************************************\n");
  fprintf(stderr,"* Enhanced aacPlus 3GPP Floating Point Reference Encoder\n");
  fprintf(stderr,"* Build %s, %s\n", __DATE__, __TIME__);
  fprintf(stderr,"*\n");
  fprintf(stderr,"*************************************************************\n\n");


  /*
   * parse command line arguments
   */
  if (argc != 5 && argc != 6) {
    fprintf(stderr, "\nUsage:   %s <wav_file> <bitstream_file> <bitrate> <(m)ono/(s)tereo> (<config file>)\n", argv[0]);
    fprintf(stderr, "\nExample: %s input.wav out.3gp 24000 s\n", argv[0]); 
    fprintf(stderr, "\nOptionally an ASCII formatted config file may be given to simulate bitrate switching");
    fprintf(stderr, "\nEach line of the config file shall hold: <numFrame> <new bitrate> <CR>");
    fprintf(stderr, "\nThe config file may hold an arbitraty number of lines");
    fprintf(stderr, "\nExample: %s input.wav out.3gp 24000 s config.txt\n", argv[0]); 
    return 0;
  }

  bitrate = atoi(argv[3]);

  if ( strcmp (argv[4],"m") == 0 ) {
    bEncodeMono = 1;
  }
  else {
    if ( strcmp (argv[4],"s") != 0 ) {
      fprintf(stderr, "\nWrong mode %c, use either (m)ono or (s)tereo\n", argv[4]);
      return 0;
    }
  }
  fflush(stdout);

  /*
    open audio input file
  */

  inputInfo.bitsPerSample  = 16 ;       /* only relevant if valid == 1 */
  inputInfo.sampleRate     = 44100 ;    /* only relevant if valid == 1 */
  inputInfo.nChannels      = 2 ;        /* only relevant if valid == 1 */
  inputInfo.nSamples       = 0 ;        /* only relevant if valid == 1 */
  inputInfo.isLittleEndian = 1;

  inputInfo.fpScaleFactor  = AACENC_PCM_LEVEL ; /* must be set */
  inputInfo.valid          = 1 ;        /* must be set */
  inputInfo.useWaveExt     = 0;


  auError = AuChannelOpen (&inputFile, argv[1], auFlags, &auType, &inputInfo);

  if(auError != AU_CHAN_OK){
    fprintf(stderr,"could not open %s\n",argv[1]);
    exit(10);
  }

  if ( (!bEncodeMono) && (inputInfo.nChannels!=2) ) {
    fprintf(stderr,"Need stereo input for stereo coding mode !\n");
    goto close_encoder;
  }

#if (MAX_CHANNELS==1)
  if (!bEncodeMono) {
    fprintf(stderr,"Mono encoder cannot encode stereo coding mode !\n");
    goto close_encoder;
  }
#endif

  if (argc == 6) {
    fConfig = fopen(argv[5], "rt");
    if (fConfig == NULL) {
      fprintf(stderr,"Could not open config input file !\n");
      goto close_encoder;
    }
  }

  COUNT_sub_start("main_init");


  /*
    set up basic parameters for aacPlus codec
  */
  PTR_INIT(1); FUNC(1);
  AacInitDefaultConfig(&config);

  BRANCH(1); MOVE(1);
  nChannelsAAC = nChannelsSBR = bEncodeMono ? 1:inputInfo.nChannels;

  ADD(3); LOGIC(3); BRANCH(1);
  if ( (inputInfo.nChannels == 2) && (!bEncodeMono) && (bitrate >= 16000) && (bitrate < 36000) )
  {

    MOVE(1);
    useParametricStereo = 1;
  }

  BRANCH(1);
  if (useParametricStereo)
  {
    MOVE(2);
    nChannelsAAC = 1;
    nChannelsSBR = 2;
  }

  ADD(3); LOGIC(2); BRANCH(1);
  if ( (inputInfo.sampleRate == 48000) && (nChannelsAAC == 2) && (bitrate < 24000)
#if 1
 ||
       (inputInfo.sampleRate == 48000) && (nChannelsAAC == 1) && (bitrate < 12000)
#endif

 ) {

    MOVE(1);
    bDoIIR32Resample  = 1;
  }

  ADD(1); BRANCH(1);
  if (inputInfo.sampleRate == 16000) {

    MOVE(3);
    bDoUpsample = 1;
    inputInfo.sampleRate = 32000;
    bDingleRate = 1;
  }

  MOVE(1);
  sampleRateIn = inputInfo.sampleRate;

  BRANCH(1);
  if (bDoIIR32Resample)
  {
    MOVE(1);
    sampleRateIn = 32000;
  }

  MOVE(3);
  config.bitRate = bitrate;
  config.nChannelsIn=inputInfo.nChannels;
  config.nChannelsOut=nChannelsAAC;

  MOVE(1);
  config.bandWidth=bandwidth;


  BRANCH(1); FUNC(1);
  if ( configureEncoder( bitrate, &config ) ) {
    goto close_encoder;
  }

  if (fConfig) {
    getNextConfig(fConfig, &nextConfigFrame, &nextConfigBitrate);
  }


  /*
    set up MPEG-4/3GPP file format library (not instrumented nor accounted for RAM requirements)
   */

  {
    unsigned char ASConfigBuffer[80];
    unsigned int  nConfigBits;
    unsigned int  nConfigBytes;

    memset (ASConfigBuffer, 0, 80);

    if ( GetMPEG4ASConfig( sampleRateAAC,
                           nChannelsAAC,
                           ASConfigBuffer,
                           &nConfigBits,
                           1,
                           bDingleRate) ) {
      fprintf(stderr, "\nCould not initialize Audio Specific Config\n");
      exit(10);
    }


    nConfigBytes = (nConfigBits+7)>>3;

    if (OpenMP4File(&hMp4File,
                    ASConfigBuffer,
                    nConfigBytes,
                    argv[2],
                    (!bDingleRate) ? sampleRateAAC*2 : sampleRateAAC, /* output sampleRate */
                    config.bitRate,
                    nChannelsAAC,
                    1,
                    1) ) {
      fprintf(stderr, "\nFailed to create 3GPP file\n") ;
      exit(10);
    }
  }

  /*
    Be verbose
   */


  fprintf(stdout,"input file %s: \nsr = %ld, nc = %ld\n\n",
          argv[1], (bDoUpsample) ? inputInfo.sampleRate/2 : inputInfo.sampleRate, inputInfo.nChannels);
  fprintf(stdout,"output file %s: \nbr = %ld sr-OUT = %d  nc-OUT = %ld\n\n",
          argv[2], bitrate, (!bDingleRate) ? sampleRateAAC*2 : sampleRateAAC, nChannelsSBR);
  fflush(stdout);


  MOVE(1);
  frmCnt = 0;

  FUNC(2); LOOP(1); PTR_INIT(1); MOVE(1); STORE(sizeof(TimeDataPcm));
  memset(TimeDataPcm,0,sizeof(TimeDataPcm));


  /*
    set up input samples block size feed
   */

  BRANCH(1);
  if (bDoIIR32Resample) {

    FUNC(1); DIV(1);
    inSamples = IIR32GetResamplerFeed(AACENC_BLOCKSIZE * inputInfo.nChannels * 2) / nRuns;

    assert(inSamples <= AACENC_BLOCKSIZE * 2);
  }
  else {

    MOVE(1);
    inSamples = AACENC_BLOCKSIZE * inputInfo.nChannels * 2;

    BRANCH(1);
    if (bDoUpsample) {
      SHIFT(1);
      inSamples =  inSamples>>1;
    }
  }

  COUNT_sub_end();



  /*
    The frame loop
   */

  while (1)
  {
    int i, ch, outSamples, numOutBytes;

    COUNT_sub_start("main_loop");


    /*
      File input read, resample and downmix
     */

    BRANCH(1);
    if (bDoIIR32Resample) {

      /* resampling from 48 kHz to 32 kHz prior to encoding */

      int stopLoop = 0;
      int nSamplesProcessed = 0;
      int r;

      const int nDSOutBlockSize = AACENC_BLOCKSIZE * 2 / nRuns;
      int stride = inputInfo.nChannels;
#if (MAX_CHANNELS==2)
      if (inputInfo.nChannels==1)
        stride = 2;
#endif

      MOVE(2); SHIFT(1); MULT(1); DIV(1); /* counting previous operations */

      LOOP(1);
      for (r=0; r<nRuns; r++) {

        PTR_INIT(1); FUNC(4); ADD(1);
        if ( AuChannelReadShort(inputFile, TimeDataPcm,
                                inSamples, &numSamplesRead) != AU_CHAN_OK) {
          MOVE(1);
          stopLoop = 1;
          break;
        }

        /* copy from short to float input buffer */
        PTR_INIT(2); /* resamplerScratch[]
                        TimeDataPcm[]
                     */
        LOOP(1);
        for (i=0; i<numSamplesRead; i++) {

          MOVE(1);
          resamplerScratch[i] = (float) TimeDataPcm[i];
        }

        BRANCH(2);
        switch (inputInfo.nChannels) {
        case 1:
          MOVE(1);
          nSamplesPerChannel = numSamplesRead;
          break;
        case 2:
          SHIFT(1);
          nSamplesPerChannel = numSamplesRead >> 1;
          break;
        default:
          DIV(1);
          nSamplesPerChannel = numSamplesRead / inputInfo.nChannels;
        }

        /* supposed to return exact number samples to encode one block of audio */
        MULT(2); ADD(1); PTR_INIT(1); FUNC(5); ADD(1);
        nSamplesProcessed += IIR32Resample( resamplerScratch,
                                            &inputBuffer[writeOffset + r*nDSOutBlockSize*stride],
                                            nSamplesPerChannel,
                                            nDSOutBlockSize,
                                            inputInfo.nChannels);

        if (inputInfo.nChannels==1 && stride==2) {
          /* reordering necessary since the encoder takes interleaved data */
          for (i=nDSOutBlockSize-1; i>=0; i--) {
            inputBuffer[writeOffset+r*nDSOutBlockSize*2+2*i] = 
              inputBuffer[writeOffset+r*nDSOutBlockSize*2+i];
          }
        }
      }

      BRANCH(1);
      if (stopLoop)
        break;

      MOVE(1);
      numSamplesRead = nSamplesProcessed;

    }
    else {

      /* no resampling prior to encoding required */


      /* read from file */
      PTR_INIT(1); FUNC(4);
      if ( AuChannelReadShort(inputFile, TimeDataPcm,
                              inSamples, &numSamplesRead) != AU_CHAN_OK) {
        break;
      }


      /* copy from short to float input buffer */
      ADD(1); BRANCH(1);
      if ( inputInfo.nChannels == nChannelsSBR ) {
        PTR_INIT(2); /* inputBuffer[i+writeOffset]
                        TimeDataPcm[i]
                     */
        LOOP(1);
        for (i=0; i<numSamplesRead; i++) {

          MOVE(1);
          inputBuffer[i+writeOffset] = (float) TimeDataPcm[i];
        }
      }


#if (MAX_CHANNELS==2)

      /* copy from short to float input buffer, reordering necessary since the encoder takes interleaved data */
      ADD(1); BRANCH(1);
      if(inputInfo.nChannels == 1) {
        int i;

        PTR_INIT(2); /* inputBuffer[writeOffset+MAX_CHANNELS*i]
                        inputBuffer[writeOffset+i]
                     */
        LOOP(1);
        for (i=0; i<numSamplesRead; i++) {

          MOVE(1);
          inputBuffer[writeOffset+2*i] = (float) TimeDataPcm[i];
        }
      }

      /* copy from short to float input buffer, downmix stereo input signal to mono, reordering necessary since the encoder takes interleaved data */
      ADD(1); LOGIC(1); BRANCH(1);
      if( (inputInfo.nChannels == 2) && bEncodeMono) {
        int i;

        PTR_INIT(2); /*  inputBuffer[writeOffset+2*i]
                         inputBuffer[writeOffset+2*i+1]
                     */
        SHIFT(1); LOOP(1);
        for(i=0;i<numSamplesRead/2;i++){

          ADD(1); MULT(1); STORE(1);
          inputBuffer[writeOffset+2*i] = ((float) TimeDataPcm[2*i] + (float) TimeDataPcm[2*i+1])*0.5f;
        }
      }

#endif

    } /*end if (bDoIIR32Resample) */


    BRANCH(1);
    if (bDoUpsample) {

      LOOP(1);
      for(ch=0;ch < inputInfo.nChannels;ch++){

        PTR_INIT(2); ADD(4); DIV(1); FUNC(7);
        IIR21_Upsample( &(IIR21_reSampler[ch]),
                        inputBuffer+upsampleReadOffset+ch,
                        numSamplesRead/inputInfo.nChannels,
                        MAX_CHANNELS,
                        inputBuffer+envWriteOffset+ch,
                        &outSamples,
                        MAX_CHANNELS);

      }
    }


    /*
      encode one SBR frame
     */

    PTR_INIT(1); ADD(2); FUNC(6);
    EnvEncodeFrame (hEnvEnc,
                    inputBuffer + envReadOffset,
                    inputBuffer,
                    MAX_CHANNELS,
                    &numAncDataBytes,
                    ancDataBytes);


    /*
      2:1 downsampling for AAC core
     */

    BRANCH(1);
    if (bDoIIR2Downsample) {

      LOOP(1);
      for( ch=0; ch<nChannelsAAC; ch++ ) {

        PTR_INIT(2); ADD(3); DIV(1); FUNC(7);
        IIR21_Downsample(&(IIR21_reSampler[ch]),
                         inputBuffer+writeOffset+ch,
                         numSamplesRead/inputInfo.nChannels,
                         MAX_CHANNELS,
                         inputBuffer+ch,
                         &outSamples,
                         MAX_CHANNELS);

      }
    }


    /* SBR side info data is passed as ancillary data */
    BRANCH(1);
    if(numAncDataBytes == 0)
    {
      MOVE(1);
      numAncDataBytes=ancDataLength;
    }


    /*
      encode one AAC frame
     */


    LOGIC(1); BRANCH(1);
    if (hEnvEnc && useParametricStereo) {

      PTR_INIT(2); FUNC(7);
      AacEncEncode(aacEnc,
                   inputBuffer,
                   1, /* stride */
                   ancDataBytes,
                   &numAncDataBytes,
                   outputBuffer,
                   &numOutBytes);

    }
    else
    {
      PTR_INIT(2); ADD(1); FUNC(7);
      AacEncEncode(aacEnc,
                   inputBuffer+coreReadOffset,
                   MAX_CHANNELS,
                   ancDataBytes,
                   &numAncDataBytes,
                   outputBuffer,
                   &numOutBytes);


      BRANCH(1);
      if(hEnvEnc) {

        BRANCH(1);
        if (bDoUpsample) {
          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(envWriteOffset-envReadOffset);
          memmove( &inputBuffer[envReadOffset],
                   &inputBuffer[envReadOffset+AACENC_BLOCKSIZE*MAX_CHANNELS*2],
                   (envWriteOffset-envReadOffset)*sizeof(float));

          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(writeOffset-upsampleReadOffset);
          memmove( &inputBuffer[upsampleReadOffset],
                   &inputBuffer[upsampleReadOffset+AACENC_BLOCKSIZE*MAX_CHANNELS],
                   (writeOffset-upsampleReadOffset)*sizeof(float));
        }
        else {
          FUNC(2); LOOP(1); PTR_INIT(2); MOVE(1); STORE(writeOffset);
          memmove( inputBuffer,inputBuffer+AACENC_BLOCKSIZE*2*MAX_CHANNELS,writeOffset*sizeof(float));
        }
      }
    }


    /*
      Write one frame of encoded audio to file
     */

    BRANCH(1);
    if (numOutBytes) {
      FUNC(3);
      MP4FileAddFrame( hMp4File,
                       outputBuffer,
                       numOutBytes );
    }



    ADD(1);
    frmCnt++;

    if (frmCnt == nextConfigFrame) {
      if ( configureEncoder (nextConfigBitrate, &config) ) {
        goto close_encoder;
      }

      getNextConfig (fConfig, &nextConfigFrame, &nextConfigBitrate);
    }
    COUNT_sub_end();

    /* 3GPP instrumenting tool: measure worst case work load at end of each decoding loop */
    COUNT_frame_update();

    fprintf(stderr,"[%d]\r",frmCnt); fflush(stderr);
  }
  fprintf(stderr,"\n"); fflush(stderr);


  /*
    Close encoder
   */

 close_encoder:

  AacEncClose(aacEnc);

  AuChannelClose (inputFile);

  if(fConfig)
    fclose(fConfig);

  if (hMp4File) {
    if (WriteMP4File( hMp4File)) {
      
      fprintf(stderr, "Writing of 3GPP file failed.");
    }
    CloseMP4File( hMp4File);
  }

  if(hEnvEnc)
  {
    EnvClose(hEnvEnc);
  }

  /* 3GPP instrumenting tool: print output data */
  COUNT_end();
  printf("\nencoding finished\n");

  return 0;
}
