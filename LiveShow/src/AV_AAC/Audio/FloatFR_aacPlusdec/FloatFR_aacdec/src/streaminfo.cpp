/*
  current bitstream parameters
*/

#include "aac_ram.h"
#include "streaminfo.h"

#include "counters.h" /* the 3GPP instrumenting tool */

/*  Stream Configuration and Information.

    This class holds configuration and information data for a stream to be decoded. It
    provides the calling application as well as the decoder with substantial information,
    e.g. profile, sampling rate, number of channels found in the bitstream etc.
*/
void CStreamInfoOpen(unsigned int keyflag,CStreamInfo **pStreamInfo)
{
  COUNT_sub_start("CStreamInfoOpen");

  /* initialize CStreamInfo */
  PTR_INIT(1); MOVE(1);
  pStreamInfo[0] = getCStreamInfo(keyflag)/*&StreamInfo*/;

  MOVE(7);
  pStreamInfo[0]->SamplingRateIndex = 0;
  pStreamInfo[0]->SamplingRate = 0;
  pStreamInfo[0]->Profile = 0;
  pStreamInfo[0]->ChannelConfig = 0;
  pStreamInfo[0]->Channels = 0;
  pStreamInfo[0]->BitRate = 0;
  pStreamInfo[0]->SamplesPerFrame = FRAME_SIZE;

  COUNT_sub_end();
}


