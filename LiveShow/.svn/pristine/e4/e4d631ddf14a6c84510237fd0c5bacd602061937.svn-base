/*
  Bitbuffer management
*/
#ifndef __SBR_BITBUF_H
#define __SBR_BITBUF_H


typedef struct
{
  unsigned char *char_ptr;
  unsigned char buffered_bits;
  unsigned short buffer_word;
  unsigned long nrBitsRead;
  unsigned long bufferLen;
}
BIT_BUFFER;

typedef BIT_BUFFER *HANDLE_BIT_BUFFER;

void initBitBuffer (HANDLE_BIT_BUFFER hBitBuf,
		    unsigned char *start_ptr, unsigned long bufferLen);

unsigned long getbits (HANDLE_BIT_BUFFER hBitBuf, int n);
unsigned long GetNrBitsRead (HANDLE_BIT_BUFFER hBitBuf);
long GetNrBitsAvailable (HANDLE_BIT_BUFFER hBitBuf);

void CopyBitbufferState (HANDLE_BIT_BUFFER hBitBuf,
			 HANDLE_BIT_BUFFER hBitBufDest);

#endif
