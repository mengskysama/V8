/*
  Bitstream routines
*/

#include "aac_ram.h"
#include "FloatFR.h"
#include "FFR_bitbuffer.h"

#include "counters.h" /* the 3GPP instrumenting tool */

/*
  The function loads bits from bitstream buffer.
  
  return:  valid bits
*/
unsigned long GetBits(HANDLE_BIT_BUF hBitBuf,  /*!< pointer to current data in bitstream */
                      int nBits)               /*!< number of bits */
{
  unsigned short tmp;

  COUNT_sub_start("GetBits");

  FUNC(2);
  tmp = (unsigned short) ReadBits(hBitBuf,nBits);

  COUNT_sub_end();

  return (tmp);
}


/*
  The function rewinds the bitstream pointeers
*/
void PushBack(HANDLE_BIT_BUF hBitBuf, /*!< pointer to current data in bitstream */
              int nBits)              /*!< number of bits */
{
  COUNT_sub_start("PushBack");

  MULT(1); FUNC(2);
  WindBitBufferBidirectional(hBitBuf,-nBits);

  COUNT_sub_end();
}


/*
  The function applies byte alignement
*/
void ByteAlign(HANDLE_BIT_BUF hBitBuf, /*!< pointer to current data in bitstream */
               long *pByteAlignBits)   /*!< pointer to last state of cntBits */
{
  int alignment;

  COUNT_sub_start("ByteAlign");

  INDIRECT(1); ADD(1); LOGIC(1);
  alignment = (*pByteAlignBits - hBitBuf->cntBits) % 8;

  BRANCH(1);
  if (alignment)
  {
    ADD(1); FUNC(2);
    GetBits(hBitBuf,8 - alignment);
  }

  MOVE(1);
  *pByteAlignBits = hBitBuf->cntBits;

  COUNT_sub_end();
}
