/*
  temporal noise shaping tool
*/
#include <math.h>
#include "aac_rom.h"
#include "FFR_bitbuffer.h"
#include "tns.h"

#include "counters.h" /* the 3GPP instrumenting tool */


/*
  The function returns the minimum of 3 values.

  return:  minimum value
*/
static int Minimum(int a, /*!< value 1 */
                   int b, /*!< value 2 */
                   int c) /*!< value 3 */
{
  int t;

  COUNT_sub_start("Minimum");
  ADD(2); BRANCH(2); MOVE(1);
  COUNT_sub_end();

  t = (a < b ) ? a : b;
  return (t < c) ? t : c;
}

/*
  The function returns the tns data present bit
*/
int TnsPresent(CTnsData *pTnsData) /*!< pointer to tns side info */
{
  COUNT_sub_start("TnsPresent");
  INDIRECT(1);
  COUNT_sub_end();

  return (pTnsData->TnsDataPresent);
}

/*
  The function converts the decoded index values into
  parcor coefficients, which are used in the lattice
  filter.
*/
static void TnsDecodeCoefficients(CFilter *filter, /*!< pointer to filter side info */
                                  float *a)        /*!< pointer to parcor coefficients */
{
  int i;

  COUNT_sub_start("TnsDecodeCoefficients");

  PTR_INIT(2); /* filter->Coeff[]
                  a[]
               */
  LOOP(1);
  for (i=0; i < filter->Order; i++)
  {
    ADD(1); BRANCH(1); INDIRECT(1); MOVE(1);
    if (filter->Resolution == 3)
      a[i+1] = tnsCoeff3[filter->Coeff[i]+4];
    else
      a[i+1] = tnsCoeff4[filter->Coeff[i]+8];
  }

  COUNT_sub_end();
}

/*
  The function applies the conversion of the parcor coefficients
  to the lpc coefficients.
*/
static void 
TnsParcor2Lpc(float *parcor, /*!< pointer to parcor coefficients */
              float *lpc,    /*!< pointer to lpc coefficients */
              int order)     /*!< filter order */

{
  int i,j;
  float z1;
  float z[MaximumOrder+1];
  float w[MaximumOrder+1];
  float accu;

  COUNT_sub_start("TnsParcor2Lpc");

  {  
    PTR_INIT(2); /* pointers for z[], w[] */
    LOOP(1);
    for (i=0; i<MaximumOrder+1; i++)
    {
      MOVE(2);
      z[i] = 0.0;
      w[i] = 0.0;
    }

    PTR_INIT(1); /* pointers for lpc[] */
    LOOP(1);
    for (i=0; i<=order; i++)
    {
      BRANCH(1); MOVE(1);
      if (i == 0)
        accu = 1.0;
      else
        accu = 0.0;
      
      MOVE(1);
      z1 = accu;
      
      PTR_INIT(3); /* pointers for parcor[], z[], w[] */
      LOOP(1);
      for (j=0; j<order; j++)
      {
        MOVE(1);
        w[j] = accu;

        MAC(1);
        accu += parcor[j] * z[j];
      }

      PTR_INIT(3); /* pointers for parcor[], z[], w[] */
      LOOP(1);
      for (j=order-1; j>=0; j--)
      {
        MULT(1); ADD(1); STORE(1);
        z[j+1] = parcor[j] * w[j] + z[j];
      }

      MOVE(2);
      z[0] = z1;
      lpc[i] = accu;
    }
  }

  COUNT_sub_end();
}

/*
  The function applies the tns filtering to the
  spectrum.
*/
static void 
TnsFilterIIR(float *spec,   /*!< pointer to spectrum */
             float *lpc,    /*!< pointer to lpc coefficients */
             float *state,  /*!< pointer to states */
             int size,      /*!< nunber of filtered spectral lines */
             char inc,      /*!< increment or decrement */
             int order)     /*!< filter order */

{
  int i,j;
  float accu;

  COUNT_sub_start("TnsFilterIIR");

  LOOP(1);
  for (i=0; i<order; i++)
  {
    MOVE(1);
    state[i] = 0.0F;
  }

  ADD(1); BRANCH(1);
  if (inc == -1)
  {
    ADD(2);
    spec += size-1;
  }

  LOOP(1);
  for (i=0; i<size; i++)
  {
    MULT(1);
    accu = *spec * lpc[0];

    PTR_INIT(2); /* pointer for lpc[],
                                state[]
                 */
    LOOP(1);
    for (j=0; j<order; j++)
    {
      MULT(1); ADD(1);
      accu -= lpc[j+1] * state[j];
    }

    PTR_INIT(1); /* pointer for state[] */
    LOOP(1);
    for (j=order-1; j>0; j--)
    {
      MOVE(1);
      state[j] = state[j-1];
    }

    MOVE(2);
    state[0] = accu;
    *spec = accu;

    ADD(1);
    spec += inc;
  }

  COUNT_sub_end();
}

/*
  The function reads the data-present flag for tns from
  the bitstream.
*/
void CTns_ReadDataPresentFlag(HANDLE_BIT_BUF bs,                              /*!< pointer to bitstream */
                              CAacDecoderChannelInfo *pAacDecoderChannelInfo) /*!< pointer to aac decoder channel info */
{
  CTnsData *pTnsData = &pAacDecoderChannelInfo->TnsData;

  COUNT_sub_start("CTns_ReadDataPresentFlag");

  INDIRECT(1); PTR_INIT(1); /* counting previous operation */

  FUNC(2); INDIRECT(1); STORE(1);
  pTnsData->TnsDataPresent = (char) GetBits(bs,1);

  COUNT_sub_end();
}

/*
  The function reads the elements for tns from
  the bitstream.
*/
void CTns_Read(HANDLE_BIT_BUF bs,                              /*!< pointer to bitstream */
               CAacDecoderChannelInfo *pAacDecoderChannelInfo) /*!< pointer to aac decoder channel info */
{
  char window,n_filt,order;
  char length,coef_res,coef_compress;  
  CTnsData *pTnsData = &pAacDecoderChannelInfo->TnsData;

  COUNT_sub_start("CTns_Read");

  INDIRECT(1); PTR_INIT(1); /* counting previous operation */

  INDIRECT(1); BRANCH(1);
  if (!pTnsData->TnsDataPresent) {

    COUNT_sub_end();
    return;
  }

  PTR_INIT(1); /* pTnsData->NumberOfFilters[window] */
  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (window = 0; window < GetWindowsPerFrame(&pAacDecoderChannelInfo->IcsInfo); window++)
  {
    FUNC(1); FUNC(2); BRANCH(1); MOVE(1);
    pTnsData->NumberOfFilters[window] = n_filt = (char) GetBits(bs,IsLongBlock(&pAacDecoderChannelInfo->IcsInfo) ? 2 : 1);

    BRANCH(1);
    if (n_filt)
    {
      char index;
      char nextstopband;

      FUNC(2);
      coef_res = (char) GetBits(bs,1);

      FUNC(1);
      nextstopband = GetScaleFactorBandsTotal(&pAacDecoderChannelInfo->IcsInfo);

      PTR_INIT(1); /* pTnsData->Filter[window][index] */
      LOOP(1);
      for (index=0; index < n_filt; index++)
      {
        CFilter *filter = &(pTnsData->Filter[window][index]);

        FUNC(1); BRANCH(1); FUNC(2);
        length = (char) GetBits(bs,IsLongBlock(&pAacDecoderChannelInfo->IcsInfo) ? 6 : 4);

        ADD(1);
        filter->StartBand = nextstopband - length;

        MOVE(1);
        filter->StopBand  = nextstopband;

        MOVE(1);
        nextstopband = filter->StartBand;

        FUNC(1); FUNC(2); BRANCH(1); MOVE(1);
        filter->Order = order = (char) GetBits(bs,IsLongBlock(&pAacDecoderChannelInfo->IcsInfo) ? 5 : 3);

        BRANCH(1);
        if (order)
        {
          char i,coef,s_mask,n_mask;
          static const char sgn_mask[] = {  0x2,  0x4,  0x8 };
          static const char neg_mask[] = { ~0x3, ~0x7, ~0xF };

          FUNC(2); BRANCH(1); MOVE(1);
          filter->Direction = GetBits(bs,1) ? -1 : 1;

          FUNC(2);
          coef_compress = (char) GetBits(bs,1);

          ADD(1); STORE(1);
          filter->Resolution = coef_res + 3;

          ADD(2); INDIRECT(2); MOVE(2);
          s_mask = sgn_mask[coef_res + 1 - coef_compress];
          n_mask = neg_mask[coef_res + 1 - coef_compress];

          PTR_INIT(1); /* filter->Coeff[] */
          LOOP(1);
          for (i=0; i < order; i++)
          {
            ADD(1); FUNC(2);
            coef = (char) GetBits(bs,filter->Resolution - coef_compress);

            LOGIC(1); BRANCH(1);
            if (coef & s_mask) {
              LOGIC(1); STORE(1);
              filter->Coeff[i] =  (coef | n_mask);
            } else {
              MOVE(1);
              filter->Coeff[i] = coef;
            }
          }
        }
      }
    }
  }


  COUNT_sub_end();
}

/*
  The function applies the tns to the spectrum,
*/
void CTns_Apply (CAacDecoderChannelInfo *pAacDecoderChannelInfo) /*!< pointer to aac decoder info */
{
  float tnsState[MaximumOrder];
  CTnsData *pTnsData = &pAacDecoderChannelInfo->TnsData;
  float *pSpectrum = pAacDecoderChannelInfo->pSpectralCoefficient;

  int window,index,start,stop,size;

  float lpc[MaximumOrder+1];
  float CoeffParc[MaximumOrder+1];

  COUNT_sub_start("CTns_Apply");

  INDIRECT(2); PTR_INIT(2); /* counting previous operations */

  INDIRECT(1); BRANCH(1);
  if (!pTnsData->TnsDataPresent) {
    COUNT_sub_end();
    return;
  }

  PTR_INIT(1); /* pTnsData->NumberOfFilters[window] */
  INDIRECT(1); PTR_INIT(1); FUNC(1); LOOP(1);
  for (window=0; window < GetWindowsPerFrame(&pAacDecoderChannelInfo->IcsInfo); window++)
  {
    PTR_INIT(1); /* pTnsData->Filter[window][index] */
    LOOP(1);
    for (index=0; index < pTnsData->NumberOfFilters[window]; index++)
    {
      CFilter *filter = &(pTnsData->Filter[window][index]);

      FUNC(2);
      TnsDecodeCoefficients(filter,CoeffParc);

      FUNC(1); FUNC(1); FUNC(3);
      start = Minimum(filter->StartBand,GetMaximumTnsBands(&pAacDecoderChannelInfo->IcsInfo),
                      GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo));

      FUNC(1); INDIRECT(1); MOVE(1);
      start = GetScaleFactorBandOffsets(&pAacDecoderChannelInfo->IcsInfo)[start];

      FUNC(3);
      stop = Minimum(filter->StopBand, GetMaximumTnsBands(&pAacDecoderChannelInfo->IcsInfo),
                     GetScaleFactorBandsTransmitted(&pAacDecoderChannelInfo->IcsInfo));

      INDIRECT(1); MOVE(1);
      stop = GetScaleFactorBandOffsets(&pAacDecoderChannelInfo->IcsInfo)[stop];

      ADD(1);
      size = stop - start;

      BRANCH(1);
      if (size <= 0) continue;

      BRANCH(1);
      if (filter->Order <= 0) continue;

      PTR_INIT(1); FUNC(3);
      TnsParcor2Lpc(&CoeffParc[1],lpc,filter->Order);

      INDIRECT(1); PTR_INIT(1); FUNC(6);
      TnsFilterIIR(&pSpectrum[window*FRAME_SIZE/8+start],lpc,tnsState,size,
                   filter->Direction,filter->Order);
    }
  }

  COUNT_sub_end();
}


