/*
  CFFTN header file
*/

#ifndef __cfftn_h
#define __cfftn_h

void forwardFFT( float *data, int length );

int cfftn(float Re[],
          float Im[],
          int  nTotal,
          int  nPass,
          int  nSpan,
          int  iSign);

int CFFTN(float *afftData,int len, int isign);
int CFFTNRI(float *afftDataReal,float *afftDataImag,int len, int isign);

int CFFTN_NI(float *InRealData,
              float *InImagData,
              float *OutRealData,
              float *OurImagData,
              int len, int isign);

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


#endif
