/*
  SBR Payload Extraction
*/

#include "FloatFR.h"
#include "sbrdecoder.h"
#include "FFR_bitbuffer.h"
#include "FFR_aacPLUScheck.h"

#include "counters.h" /* the 3GPP instrumenting tool */


/*
  \brief Extraction of aacPlus - specific payload from fill element

  \return void
*/

void FFRaacplus_checkForPayload(HANDLE_BIT_BUF bs,
                                SBRBITSTREAM *streamSBR,
                                int prev_element)
{
  int i;
  int count=0;
  int esc_count=0;

  COUNT_sub_start("FFRaacplus_checkForPayload");

  MOVE(2); /* counting previous operations */


  FUNC(2);
  count = ReadBits(bs,4);

  ADD(1); BRANCH(1);
  if (count == 15)
  {
    FUNC(2);
    esc_count = ReadBits(bs,8);

    ADD(1);
    count =  esc_count + 14;
  }

  BRANCH(1);
  if (count)
  {
     unsigned char extension_type;

    FUNC(2);
    extension_type = (unsigned char) ReadBits(bs,4);


    ADD(5); LOGIC(4); BRANCH(1);
    if (   (prev_element == SBR_ID_SCE || prev_element == SBR_ID_CPE)
        && ((extension_type == SBR_EXTENSION) || (extension_type == SBR_EXTENSION_CRC))
        && (streamSBR->NrElements < MAXNRELEMENTS)    )
    {
      FUNC(2); INDIRECT(1); STORE(1);
      streamSBR->sbrElement [streamSBR->NrElements].Data[0] = (unsigned char) ReadBits(bs,4);

      PTR_INIT(1); /* streamSBR->sbrElement [streamSBR->NrElements].Data[i] */
      LOOP(1);
      for (i=1; i<count; i++)
      {
        FUNC(2); STORE(1);
        streamSBR->sbrElement [streamSBR->NrElements].Data[i] = (unsigned char) ReadBits(bs,8);
      }

      MOVE(2);
      streamSBR->sbrElement[streamSBR->NrElements].ExtensionType = extension_type;
      streamSBR->sbrElement[streamSBR->NrElements].Payload = count;

      ADD(1); INDIRECT(1); STORE(1);
      streamSBR->NrElements += 1;
    }
    else
    {
      FUNC(2);
      ReadBits(bs,4);

      LOOP(1);
      for (i=1; i<count; i++)
      {
        FUNC(2);
        ReadBits(bs,8);
      }
    }
  }

  COUNT_sub_end();
}


