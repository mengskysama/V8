/*
  Spline resampler structs and prototypes
*/
#ifndef __SPLINE_RESAMPLER_H_
#define __SPLINE_RESAMPLER_H_

#define MAX_ORDER 3

typedef struct SPLINE_RESAMPLER *HANDLE_SPLINE_RESAMPLER;

void CreateSplineResampler(HANDLE_SPLINE_RESAMPLER* hr, int Fin, int Fout);
void SplineResample(HANDLE_SPLINE_RESAMPLER hr, float* ioBuffer, int inSamples, int* outSamples, int channel);
void CopyResamplerState(HANDLE_SPLINE_RESAMPLER hr);
#endif
