/*
  wrapper around libisomedia
*/


#ifndef __MP4FF_H
#define __MP4FF_H

typedef struct MP4_FILE* HANDLE_MP4_FILE;


int
GetMPEG4ASConfig(unsigned int sampleRate,
                 unsigned int noChannels,
                 unsigned char* buf,
                 unsigned int*  numStreamMuxBits,
                 int bSbrPresent,
                 int bDingleRate);

int
OpenMP4File(HANDLE_MP4_FILE* phMp4File,
            unsigned char*  audioSpecConfigData,
            int     audioSpecConfigByte,
            char*   filename,
            int     sampleRate,
            int     bitRate,
            int     nChannels,
            int     bSbrPresent,
            int     b3GppFile);

int
MP4FileAddFrame(HANDLE_MP4_FILE mp4File,
                unsigned int*   payload,
                unsigned int    numBytes);

int
WriteMP4File(HANDLE_MP4_FILE mp4File);

void
CloseMP4File(HANDLE_MP4_FILE mp4File);


#endif

