/*
  CRC check coutines
*/
#include "sbr_crc.h"
#include "sbr_bitb.h"
#include "sbr_const.h"
#include "FloatFR.h"

#include "counters.h" /* the 3GPP instrumenting tool */

const unsigned short MAXCRCSTEP = 16;


/*
  \brief     crc calculation
*/
static unsigned long
calcCRC (HANDLE_CRC hCrcBuf, unsigned long bValue, int nBits)
{
  int i;
  unsigned long bMask = (1UL << (nBits - 1));

  COUNT_sub_start("calcCRC");

  ADD(1); SHIFT(1); /* counting previous operations */

  LOOP(1);
  for (i = 0; i < nBits; i++, bMask >>= 1) {
    unsigned short flag = (hCrcBuf->crcState & hCrcBuf->crcMask) ? 1 : 0;
    unsigned short flag1 = (bMask & bValue) ? 1 : 0;

    INDIRECT(2); ADD(2); LOGIC(2); BRANCH(2); MOVE(2); /* counting previous operations */

    LOGIC(1);
    flag ^= flag1;

    SHIFT(1); STORE(1);
    hCrcBuf->crcState <<= 1;

    BRANCH(1);
    if (flag)
    {
      INDIRECT(1); LOGIC(1); STORE(1);
      hCrcBuf->crcState ^= hCrcBuf->crcPoly;
    }
  }

  COUNT_sub_end();

  return (hCrcBuf->crcState);
}


/*
  \brief     crc
*/
static int
getCrc (HANDLE_BIT_BUFFER hBitBuf, unsigned long NrBits)
{

  int i;
  int CrcStep = NrBits / MAXCRCSTEP;
  int CrcNrBitsRest = (NrBits - CrcStep * MAXCRCSTEP);
  unsigned long bValue;

  CRC_BUFFER CrcBuf;

  COUNT_sub_start("getCrc");

  DIV(1); MULT(1); ADD(1); /* counting previous operations */

  MOVE(3);
  CrcBuf.crcState = SBR_CRC_START;
  CrcBuf.crcPoly  = SBR_CRC_POLY;
  CrcBuf.crcMask  = SBR_CRC_MASK;

  LOOP(1);
  for (i = 0; i < CrcStep; i++) {

    FUNC(2);
    bValue = getbits (hBitBuf, MAXCRCSTEP);

    PTR_INIT(1); FUNC(3);
    calcCRC (&CrcBuf, bValue, MAXCRCSTEP);
  }

  FUNC(2);
  bValue = getbits (hBitBuf, CrcNrBitsRest);

  PTR_INIT(1); FUNC(3);
  calcCRC (&CrcBuf, bValue, CrcNrBitsRest);

  LOGIC(1); /* counting post operation */

  COUNT_sub_end();

  return (CrcBuf.crcState & SBR_CRC_RANGE);

}


/*
  \brief   crc interface
  \return  1: CRC OK, 0: CRC check failure
*/
int
SbrCrcCheck (HANDLE_BIT_BUFFER hBitBuf,
             long NrBits)
{
  int crcResult = 1;
  BIT_BUFFER BitBufferCRC;
  unsigned long NrCrcBits;
  unsigned long crcCheckResult;
  long NrBitsAvailable;
  unsigned long crcCheckSum;

  COUNT_sub_start("SbrCrcCheck");

  MOVE(1); /* counting previous operation */

  FUNC(2);
  crcCheckSum = getbits (hBitBuf, SI_SBR_CRC_BITS);

  PTR_INIT(1); FUNC(2);
  CopyBitbufferState (hBitBuf, &BitBufferCRC);


  PTR_INIT(1); FUNC(1);
  NrBitsAvailable = GetNrBitsAvailable(&BitBufferCRC);

  BRANCH(1);
  if (NrBitsAvailable <= 0){
    COUNT_sub_end();
    return 0;
  }

  FUNC(2);
  NrCrcBits = min (NrBits, NrBitsAvailable);

  PTR_INIT(1); FUNC(2);
  crcCheckResult = getCrc (&BitBufferCRC, NrCrcBits);


  ADD(1); BRANCH(1);
  if (crcCheckResult != crcCheckSum) {
    MOVE(1);
    crcResult = 0;
  }

  COUNT_sub_end();

  return crcResult;
}
