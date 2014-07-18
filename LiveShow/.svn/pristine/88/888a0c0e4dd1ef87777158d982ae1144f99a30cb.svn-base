/*
  Huffman Decoder
*/

#include "huff_dec.h"
#include "FloatFR.h"

#include "counters.h" /* the 3GPP instrumenting tool */

/***************************************************************************/
/*
  \brief     Decodes one huffman code word

  \return    decoded value
****************************************************************************/
int
DecodeHuffmanCW (Huffman h,
                 HANDLE_BIT_BUFFER hBitBuf)
{
  char index = 0;
  int value, bit;

  COUNT_sub_start("DecodeHuffmanCW");

  MOVE(1); /* counting previous operation */

  LOOP(1);
  while (index >= 0) {

    FUNC(2);
    bit = getbits (hBitBuf, 1);

    INDIRECT(1); MOVE(1);
    index = h[index][bit];
  }

  ADD(1);
  value = index+64;

  COUNT_sub_end();

  return value;
}
