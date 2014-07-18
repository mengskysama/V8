/*
  Main for aacPlus decoding
*/

#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "FloatFR.h"
#include "FFR_bitbuffer.h"
#include "au_channel.h"
#include "fileifc.h"

#include "aacdecoder.h"
#include "sbrdecoder.h"
#include "spline_resampler.h"
#include "aac_ram.h"
#include "aac_rom.h"
#include <time.h>

#include "FFR_aacPLUScheck.h"

#include "counters.h" /* the 3GPP instrumenting tool */

#ifdef _DEBUG
extern "C" {FILE _iob[3]={__iob_func()[0],__iob_func()[1],__iob_func()[2]}; };
#endif

#define FILE_NAME_MAX     256
#define SAMPLES_PER_FRAME 1024

enum {
  MAIN_OK = 0x0,
  MAIN_OPEN_BITSTREAM_FILE_FAILED,
  MAIN_OPEN_16_BIT_PCM_FILE_FAILED,
  MAIN_OPEN_ERROR_PATTERN_FILE_FAILED,
  MAIN_FRAME_COUNTER_REACHED_STOP_FRAME
};  

int ErrorStatus = MAIN_OK;                             /*!< error status */

hAudioChannel audioOut = 0;                            /*!< handle to audio output */
AuChanInfo audioOutInfo;                               /*!< info structure for audio output file */
AuChanType audioOutType;                               /*!< type specifier for audio output (RT or File) */

FileWrapperPtr inputFile;                              /*!< file pointer to bitstream file (mp4) */
FILE *pOutputFile;                                     /*!< file pointer to 16 bit output file */
FILE *pErrorPatternFile = NULL;                        /*!< file pointer to error pattern file */

unsigned short frameCounter = 0;                       /*!< frame counter */

char inputBsFilename[FILENAME_MAX] = {0};              /*!< name of bitstream file */
char output16BitPcmFilename[FILENAME_MAX] = {0};       /*!< name of 16 bit output file */
char errorPatternFilename[FILENAME_MAX] = {0};         /*!< name of error pattern file */

/* 
   IO-Buffers 
*/
#define INPUT_BUF_SIZE (6144*2/8)                      /*!< Size of Input buffer in bytes*/

unsigned int inBuffer[INPUT_BUF_SIZE/(sizeof(int))];   /*!< Input buffer */
//float TimeDataFloat[4*SAMPLES_PER_FRAME];              /*!< Output buffer */
float * TimeDataFloat = NULL;

/*
  The function determines the last instructions if the exit process is called.
*/
static void aac_exit(void)
{
  if (audioOut)
    AuChannelClose(audioOut);
  FileWrapper_Close(inputFile);
  if (pOutputFile)
    fclose(pOutputFile);
  if (pErrorPatternFile)
    fclose(pErrorPatternFile);
}



/*
  The function reads the frameError flag from a file.
  
  return:  1 if the frame is ok, else 0
*/
static char ReadErrorPatternFile(FILE *epf)
{
  char tmp;
  int readOk;

  if (!epf)
    return 1;

  readOk = fscanf(epf, "%c\n", &tmp);
  if (readOk != 1) {
    rewind(epf);
    fscanf(epf, "%c\n", &tmp);
  }
  if (tmp == '0')
    return 1;
  else
    return 0;
}


/*
  The function reads the bitstream from a MPEG-4 file and stores the bitstream
  in the input buffer. The input buffer is filled with one AU.

  return:  1 if end of file has been reached
*/
static int CBitstreamFillBufferAU(HANDLE_BIT_BUF hBitBuf,     /*!< pointer to current data in bitstream */
                                  FileWrapperPtr transport
                                  )
{
  unsigned int cbValid;
  unsigned int i;
  int ErrorStatus = 0;
  unsigned char readbuf[4096];

  /* read bitstream */
  ErrorStatus = FileWrapper_Read(transport, readbuf, 4096,  &cbValid);

  for (i=0; i<cbValid; i++)
    WriteBits(hBitBuf,readbuf[i],8);

  return ErrorStatus;
}

/*
  Interleave output samples. In case of mono input, copy left channel to right channel
*/
static void 
interleaveSamples(float *pTimeCh0, 
                  float *pTimeCh1, 
                  short *pTimeOut, 
                  int frameSize, 
                  int *channels)
{
  int i;

  for (i=0; i<frameSize; i++)
  {
    *pTimeOut++ = (short) *pTimeCh0++;

    if(*channels == 2) {
      *pTimeOut++ = (short) *pTimeCh1++;
    }
    else {
      *pTimeOut = *(pTimeOut-1);
      *pTimeOut++;
    }
  }

  *channels = 2;
}

/*
  The function sets default audio parameters and opens the
  output file for 16 bit.

  return:  error status
*/
static int 
AuChannelInit16(char *outfile,     /*!< pointer to output file */
                int sampleRate,    /*!< sampling frequency */
                short max_chan,    /*!< channels */
                int read_args)     /*!< read arguments */
{
  AuChanError audioError = AU_CHAN_OK;
  
  /* determination of the audio output type */
  if (read_args == 2)
    audioOutType = TYPE_AUTODETECT;
  else
  {
    audioOutInfo.typeInfo.rt.level = 200;
    audioOutType = TYPE_RT;
  }

  /* set audio output default values */
  audioOutInfo.valid = 1;
  audioOutInfo.bitsPerSample = 16;
  audioOutInfo.fpScaleFactor = 32768.0;
  audioOutInfo.nChannels = max_chan;
  audioOutInfo.sampleRate = sampleRate;

  /* open output file */
  if ((audioError = AuChannelOpen(&audioOut,outfile,AU_CHAN_WRITE,&audioOutType,&audioOutInfo)))
  {
    fprintf(stderr,"\n    AuChannelOpen output file failed: error %d\n", audioError);
    return MAIN_OPEN_16_BIT_PCM_FILE_FAILED;
  }

  fprintf(stderr,"\nopened output ch=%d, fs=%d\n",audioOutInfo.nChannels,audioOutInfo.sampleRate);
  return MAIN_OK;
}


/*
  The function opens the error pattern file (if any).

  return:  error status
*/

static int 
OpenErrorPatternFile(char *file)    /*!< char[] containing the filename (if any) */
{
  /* open error pattern file */
  if (!file[0])
    return MAIN_OK;
  pErrorPatternFile = fopen(file, "r");
  if (pErrorPatternFile == NULL)
  {
    fprintf (stderr, "\n error pattern file not found\n\n");
    return MAIN_OPEN_ERROR_PATTERN_FILE_FAILED;
  }
  return MAIN_OK;
}

/*
  The function displays copyright message
*/
static void display_copyright_message(void)
{
  fprintf(stderr,"\n");
  fprintf(stderr,"*************************************************************\n");
  fprintf(stderr,"*Enhanced aacPlus 3GPP Floating Point Reference Decoder\n");
  fprintf(stderr,"* Build %s, %s\n", __DATE__, __TIME__);
  fprintf(stderr,"*\n");
  fprintf(stderr,"*************************************************************\n\n");
}

/*
  After an initialization phase, CAacDecoder_DecodeFrame() and applySBR()
  are called in an endless loop until the complete bitstream has been decoded.

  The main program provides the aac core and the sbr tool with the time data buffer of length
  4 * SAMPLES_PER_FRAME.

  return:  error status
*/
int main(int argc, char *argv[])
{
  int endOfFile = 0;
  char frameOk = 1;                                 /*!< frameOk flag */
  int i;
  int written16;
  char channelMode = 0;
  unsigned int keyflag = 0;
  struct BIT_BUF bitBuf, *hBitBuf;
  AACDECODER aacDecoderInfo = 0;                    /*!< pointer to aacdecoder structure */
  SBRBITSTREAM streamSBR;                           /*!< pointer to sbr bitstream buffer */
  SBRDECODER sbrDecoderInfo = 0;                    /*!< pointer to sbrdecoder structure */
  HANDLE_SPLINE_RESAMPLER splineResampler = 0;      /*!< pointer to spline resampler instance */
  short pTimeDataPcm[4*SAMPLES_PER_FRAME];          /*!< required only for interfacing with 
                                                         audio output library, thus not counted
                                                         for RAM usage */
  int frameSize;
  int sampleRate, outputSampleRate = 0;
  int numChannels;
  int numChannelsLast = 0;
  int numOutSamples;
  int bDownSample = 0;

  int fosr16 = 0;
  int fosr8  = 0;
  int bBitstreamDownMix = 0;
  int bValidMode = 0;

  /* initialize the 3GPP instrumenting tool */
  COUNT_init();

  /* print copyright message */
  display_copyright_message();

#ifdef MONO_ONLY
    bBitstreamDownMix = 1;
#else
    bBitstreamDownMix = 0;    
#endif  

	argv[1] = "C:\\save_test.3gp";
	argv[2] = "C:\\save_test_aac.wav";
	argv[3] = "def";

	argc = 4;


  if (argc != 4 && argc != 5) {
    fprintf(stderr, "\nUsage: %s <bitstream_file> <wav_file> <mode> [error_pattern_file]\n", argv[0]);
    fprintf(stderr, "\n       <mode> is one out of:");
    fprintf(stderr, "\n         (def) default decoding");
    fprintf(stderr, "\n         (mon) mono output decoding");
    fprintf(stderr, "\n         (f08) 8 kHz output decoding");
    fprintf(stderr, "\n         (f16) 16 kHz output decoding");
    fprintf(stderr, "\n         (m08) mono 8 kHz output decoding");
    fprintf(stderr, "\n         (m16) mono 16 kHz output decoding\n");
    return 0;
  }

  /*
    evaluate mode and set flags accordingly
   */
  if (strcmp(argv[3],"def")==0) {
    bValidMode = 1;
  }

  if (strcmp(argv[3],"mon")==0) {
    bValidMode = 1;
    bBitstreamDownMix = 1;
  }

  if (strcmp(argv[3],"f08")==0) {
    bValidMode = 1;
    fosr8 = 1;
  }

  if (strcmp(argv[3],"f16")==0) {
    bValidMode = 1;
    fosr16 = 1;
  }
  
  if (strcmp(argv[3],"m08")==0) {
    bValidMode = 1;
    bBitstreamDownMix = 1;
    fosr8 = 1;
  }
  
  if (strcmp(argv[3],"m16")==0) {
    bValidMode = 1;
    bBitstreamDownMix = 1;
    fosr16 = 1;
  }

  if (bValidMode == 0) {
    fprintf(stderr, "Invalid mode, call %s without arguments for help\n", argv[0]);
    return -1;
  }

  atexit(aac_exit);
  inputFile = FileWrapper_Open(argv[1]);
  if (inputFile == 0) {
    fprintf(stderr, "Failed to open bitstream file %s\n", argv[1]);
    return -1;
  } else if (!FileWrapper_IsMp4File(inputFile)) {
    fprintf(stderr, "Invalid input file %s\n", argv[1]);
    return -2;
  } else {
    fprintf(stderr, "Input bitstream file:\t%s\n", argv[1]);
  }

  strcpy(output16BitPcmFilename, argv[2]);

  if (argc == 5) {
    ErrorStatus = OpenErrorPatternFile(argv[4]);
    if (ErrorStatus == MAIN_OPEN_ERROR_PATTERN_FILE_FAILED) {
      fprintf(stderr, "Failed to open error pattern file\n");
      return -3;
    }
  }

  COUNT_sub_start("main_init");

  /* dual or dingle rate signalled in bitstream ? */
  INDIRECT(2); ADD(1); BRANCH(1);
  if (inputFile->samplingRate == inputFile->extensionSamplingRate) {
    MOVE(1);
    bDownSample = 1;
  }
  
  /* initialize bit buffer */
  PTR_INIT(1); FUNC(3);
  hBitBuf = CreateBitBuffer(&bitBuf,(unsigned char*) inBuffer,INPUT_BUF_SIZE);

  /* initialize AAC decoder */
  BRANCH(1);
  if (!ErrorStatus) {

    PTR_INIT(1); FUNC(3);
    aacDecoderInfo = CAacDecoderOpen(keyflag,
									 hBitBuf,
                                     &streamSBR,
                                     TimeDataFloat);

    BRANCH(1);
    if (!aacDecoderInfo) {
      MOVE(1);
      ErrorStatus = -1;
    }
  }

  /* initialize time data buffer */
  PTR_INIT(1); /* TimeDataFloat[] */
  LOOP(1);
  for (i=0; i < 4*SAMPLES_PER_FRAME; i++)
  {
	  MOVE(1);
	  TimeDataFloat[i] = 0.0;
  }

  BRANCH(1);
  if (!ErrorStatus) {

    INDIRECT(2); FUNC(3);
    ErrorStatus = CAacDecoderInit (aacDecoderInfo,
                                   inputFile->samplingRate,
                                   inputFile->bitRate);
  }
  
  BRANCH(1);
  if (ErrorStatus)
  {
    COUNT_end();
    COUNT_sub_end();
    return(ErrorStatus);
  }

  COUNT_sub_end();
   
  while (!ErrorStatus && !endOfFile)  {


	  double  start, finish;
	  start = clock();//取开始时间

    /* decode one frame of audio data */
    COUNT_sub_start("main_loop");
    
    MOVE(1);
    streamSBR.NrElements = 0;
    
    /* Reading bitstream data from file (not instrumented) */
    endOfFile = CBitstreamFillBufferAU(hBitBuf,inputFile);
    
    /* Reading error pattern from file (not instrumented) */
    frameOk = ReadErrorPatternFile(pErrorPatternFile); 
    
    /* AAC core decoder */
    PTR_INIT(6); FUNC(7);
    ErrorStatus = CAacDecoder_DecodeFrame(keyflag,
										  aacDecoderInfo,
                                          &frameSize,
                                          &sampleRate,
                                          &numChannels,
                                          &channelMode,
                                          frameOk);
    
    BRANCH(1);
    if (ErrorStatus) {
      /*
        Leave loop in case of errors, EOF is no error
      */
      BRANCH(1);
      if (endOfFile) {
        MOVE(1);
        ErrorStatus = 0;
      }
      break;
    }
    
    /* SBR decoder */
    /* open SBR-handle if SBR-Bitstream has been detected in core decoder */
    LOGIC(1); BRANCH(1); 
    if ((!sbrDecoderInfo) && streamSBR.NrElements) {
      int lpFilter = 0;      
      MOVE(1);

      BRANCH(1); MOVE(2);
      if(fosr16){
        lpFilter = 8;
        bDownSample = 1;
      }
      if(fosr8){
        lpFilter = 4;
        bDownSample = 1;
      }
      
      FUNC(4);
      sbrDecoderInfo = openSBR (keyflag,sampleRate,frameSize, bDownSample,lpFilter);
    }
    
    BRANCH(1);
    if (sbrDecoderInfo) {
      
      /* apply SBR processing */
      PTR_INIT(2); FUNC(7); ADD(1); BRANCH(1);
      if (applySBR(keyflag,
				   sbrDecoderInfo,
                   &streamSBR,
                   TimeDataFloat,
                   &numChannels,
                   frameOk, 
                   bDownSample, 
                   bBitstreamDownMix) != SBRDEC_OK){
        MOVE(1);
        sbrDecoderInfo = 0;
      }
      else {
        BRANCH(1);
        if(!bDownSample){
          
          MULT(2);
          frameSize = frameSize*2;
          sampleRate *= 2;
        }
      }
      
      BRANCH(1);
      if(bBitstreamDownMix) {
        MOVE(1);
        numChannels = 1;
      }
    }
    /* end sbr decoder */
    
    
    /* spline resampler*/
    /* init spline resampler if desired and/or needed */
    ADD(1); LOGIC(1); BRANCH(1);
    if (fosr16 && sampleRate != 16000) {
      
      /* initialize resampler if not done already */
      BRANCH(1);
      if (!splineResampler) {
        PTR_INIT(1); FUNC(3);
        CreateSplineResampler(&splineResampler, sampleRate, 16000);
      }

      ADD(2); LOGIC(1); BRANCH(1);
      if (numChannels == 2 && numChannelsLast == 1) {
        FUNC(1);
        CopyResamplerState(splineResampler);
      }

      /* do resampling, works inplace on one channel */
      PTR_INIT(1); FUNC(5);
      SplineResample (splineResampler, TimeDataFloat, frameSize, &numOutSamples, 0);
      
      ADD(1); BRANCH(1);
      if (numChannels == 2) {

        PTR_INIT(2); FUNC(5);
        SplineResample (splineResampler, &TimeDataFloat[frameSize], frameSize, &numOutSamples, 1);
      }
      
      MOVE(1);
      outputSampleRate = 16000;
    }
    else {
      ADD(1); LOGIC(1); BRANCH(1);
      if (fosr8 && sampleRate != 8000) {
        
        /* initialize resampler if not done already */
        BRANCH(1);
        if (!splineResampler) {
          PTR_INIT(1); FUNC(3);
          CreateSplineResampler(&splineResampler, sampleRate, 8000);
        }

        ADD(2); LOGIC(1); BRANCH(1);
        if (numChannels == 2 && numChannelsLast == 1) {
          FUNC(1);
          CopyResamplerState(splineResampler);
        }

        /* do resampling, works inplace on one channel */
        PTR_INIT(1); FUNC(5);
        SplineResample (splineResampler, TimeDataFloat, frameSize, &numOutSamples, 0);

        ADD(1); BRANCH(1);
        if (numChannels == 2) {

          PTR_INIT(2); FUNC(5);
          SplineResample (splineResampler, &TimeDataFloat[frameSize], frameSize, &numOutSamples, 1);
        }
        
        
        MOVE(1);
        outputSampleRate = 8000;
      }
      else {
        MOVE(2);
        outputSampleRate = sampleRate;
        numOutSamples = frameSize;
      }
    }

    numChannelsLast = numChannels;

    /* clip time samples */
    
    for (i = 0; i < frameSize * numChannels; i++) {
      if (TimeDataFloat[i] < -32768.0) {        
        TimeDataFloat[i] = -32768.0;
      }
      else {
        if (TimeDataFloat[i] > 32767.0) {          
          TimeDataFloat[i] = 32767.0;
        }
      }
    }
    
    /* interleave time samples */
    interleaveSamples(&TimeDataFloat[0],&TimeDataFloat[frameSize],pTimeDataPcm,frameSize,&numChannels);
    
    /* Audio output handling (not instrumented) */
    if (output16BitPcmFilename[0]) {
      if (frameCounter > 0) {

        /* open audio output if not done yet */
        if (!audioOut) {
          if ( (ErrorStatus = AuChannelInit16(output16BitPcmFilename,outputSampleRate,numChannels,2)) ) {
            COUNT_sub_end();
            fprintf(stderr, "Failed to open output file %s\n", output16BitPcmFilename);
            return -4;
          }
          else {
            fprintf(stderr, "Output file:\t\t%s\n", output16BitPcmFilename);
            /* doesn't really fit here, but we want to display argv in order */
            if (pErrorPatternFile != NULL) {
              fprintf(stderr, "Error pattern file:\t%s\n", argv[4]);
            }

            fprintf(stderr, "\n");
          }
        }

        /* write audio channels to pcm file */
        AuChannelWriteShort(audioOut,pTimeDataPcm,numOutSamples*numChannels,&written16);
      }
    }

	finish = clock();//取结束时间
	printf( "解码时间：：：：：：：：%f \n",(finish - start) );

    frameCounter++;

    if (frameCounter > 1) {
      fprintf(stderr,"\r[%5d]",frameCounter);      
    }
    
    COUNT_sub_end();
    
    /* 3GPP instrumenting tool: measure worst case work load at end of each decoding loop */
    COUNT_frame_update();
  }

  /* 3GPP instrumenting tool: print output data */
  COUNT_end();
  fprintf(stderr, "\ndecoding finished\n");

  return ErrorStatus;
}






