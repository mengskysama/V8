/*
  current bitstream parameters
*/

#ifndef STREAMINFO_H
#define STREAMINFO_H

typedef struct
{
  char SamplingRateIndex;
  unsigned int SamplingRate;
  unsigned int Profile;
  unsigned int ChannelConfig;
  unsigned int Channels;
  unsigned int BitRate;
  unsigned int SamplesPerFrame;
} CStreamInfo;

/* Initialize stream info */
void CStreamInfoOpen(unsigned int keyflag,CStreamInfo **pStreamInfo);

#endif /* #ifndef STREAMINFO_H */
