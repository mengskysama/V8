/*
  overlap & add
*/

#ifndef OVERLAPADD_H
#define OVERLAPADD_H

#include "aacdecoder.h"

enum
{
  OverlapBufferSize = FRAME_SIZE/2
};

typedef struct
{
  char WindowShape; 
  char WindowSequence; 
  float *pOverlapBuffer;
} COverlapAddData;

#endif /* #ifndef OVERLAPADD_H */
