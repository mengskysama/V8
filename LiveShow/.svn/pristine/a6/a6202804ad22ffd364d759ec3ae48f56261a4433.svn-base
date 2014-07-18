#ifndef _MD5_H
#define _MD5_H
#ifdef __cplusplus
extern "C"
{
#endif
#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
//#define uint32 unsigned long int
#define uint32 unsigned int
#endif

typedef struct
{
    uint32 total[2];
    uint32 state[4];
    uint8 buffer[64];
}
md5_context;

void md5_starts( md5_context *ctx );
void md5_update( md5_context *ctx, uint8 *input, uint32 length );
void md5_finish( md5_context *ctx, uint8 digest[16] );
char *md5_string(const unsigned char *in, char *out, uint32 in_len);
char *md5_convert(char* in, char out[16]);

#ifdef __cplusplus
}
#endif
#endif /* md5.h */

