/*
  Huffman Decoder
*/
#ifndef __HUFF_DEC_H
#define __HUFF_DEC_H

#include "sbr_bitb.h"

typedef const char (*Huffman)[2];

int
DecodeHuffmanCW (Huffman h,
                 HANDLE_BIT_BUFFER hBitBuf);

#endif
