/*
  Parametric Stereo bitstream processing and decoding
*/
#ifndef __PS_BITDEC_H
#define __PS_BITDEC_H

#include "sbr_bitb.h"

struct PS_DEC;

#define EXTENSION_ID_PS_CODING   2

unsigned int
ReadPsData (struct PS_DEC  *h_ps_dec,
            HANDLE_BIT_BUFFER hBitBuf,
            int nBitsLeft);

void
DecodePs(struct PS_DEC *h_ps_dec);


#endif
