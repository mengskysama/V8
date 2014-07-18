/*
  AAC decoder main
*/

#ifndef AACDECODER_H
#define AACDECODER_H

#include <stdio.h>
#include "sbrdecoder.h"
#include "FFR_bitbuffer.h"


enum {
  AAC_DEC_OK = 0x0,
  AAC_DEC_UNSUPPORTED_FORMAT,
  AAC_DEC_DECODE_FRAME_ERROR,
  AAC_DEC_INVALID_CODE_BOOK,
  AAC_DEC_UNSUPPORTED_WINDOW_SHAPE,
  AAC_DEC_PREDICTION_NOT_SUPPORTED_IN_LC_AAC,
  AAC_DEC_UNIMPLEMENTED_PCE,
  AAC_DEC_UNIMPLEMENTED_DSE,
  AAC_DEC_UNIMPLEMENTED_LFE,
  AAC_DEC_UNIMPLEMENTED_CCE,
  AAC_DEC_UNIMPLEMENTED_GAIN_CONTROL_DATA,
  AAC_DEC_UNIMPLEMENTED_EP_SPECIFIC_CONFIG_PARSE,
  AAC_DEC_UNIMPLEMENTED_CELP_SPECIFIC_CONFIG_PARSE,
  AAC_DEC_UNIMPLEMENTED_HVXC_SPECIFIC_CONFIG_PARSE,
  AAC_DEC_OVERWRITE_BITS_IN_INPUT_BUFFER,
  AAC_DEC_CANNOT_REACH_BUFFER_FULLNESS
};


typedef struct AAC_DECODER_INSTANCE *AACDECODER;

#define FRAME_SIZE  1024

/* initialization of aac decoder */
AACDECODER CAacDecoderOpen(unsigned int& keyflag,
						   HANDLE_BIT_BUF pBs,
                           SBRBITSTREAM *streamSBR,
                           float*&pTimeData);

int CAacDecoderInit(AACDECODER self,
                    int samplingRate,
                    int bitrate);

/* aac decoder */
int CAacDecoder_DecodeFrame(unsigned int keyflag,
							AACDECODER aacDecoderInstance,
                            int *frameSize,
                            int *sampleRate,
                            int *numChannels,
                            char *channelMode,
                            char errorStatus
                            );

int CAacDecoderClose(unsigned int keyflag);

float* getTimeDataFloat(unsigned int flag,int pos);
#endif /* #ifndef AACDECODER_H */
