/*
  SBR specific Payload Extraction
*/
#ifndef AACPLUSCHECK_H
#define AACPLUSCHECK_H

#include "FFR_bitbuffer.h"

void FFRaacplus_checkForPayload(HANDLE_BIT_BUF bs,
                                SBRBITSTREAM *streamSBR,
                                int previous_element);

#endif
