/*
  CRC checking routines
*/
#ifndef __SBR_CRC_H
#define __SBR_CRC_H

#include "sbr_bitb.h"

#define SBR_CRC_POLY                  0x0233
#define SBR_CRC_MASK                  0x0200
#define SBR_CRC_START                 0x0000
#define SBR_CRC_RANGE                 0x03FF

typedef struct
{
  unsigned short crcState;
  unsigned short crcMask;
  unsigned short crcPoly;
}
CRC_BUFFER;

typedef CRC_BUFFER *HANDLE_CRC;

int SbrCrcCheck (HANDLE_BIT_BUFFER hBitBuf,
                 long NrCrcBits);


#endif
