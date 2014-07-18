/*
  long/short decoding
*/

#include <assert.h>
#include <math.h>
#include "aac_rom.h"
#include "FFR_bitbuffer.h"

#include "FloatFR.h"

#include "counters.h" /* the 3GPP instrumenting tool */

void CTns_ReadDataPresentFlag(HANDLE_BIT_BUF bs, CAacDecoderChannelInfo *pAacDecoderChannelInfo);
void CTns_Read(HANDLE_BIT_BUF bs,CAacDecoderChannelInfo *pAacDecoderChannelInfo);
void CTns_Apply(CAacDecoderChannelInfo *pAacDecoderChannelInfo);
void CPns_Apply(CAacDecoderChannelInfo *pAacDecoderChannelInfo[], int channel);

float CBlock_Quantize(int value,
                      int scfMod,
                      int scale)
{
  COUNT_sub_start("CBlock_Quantize");

  BRANCH(1);
  if (value != 0)
  {
    float scalefactor = (float) pow (2.0F, 0.25F * scfMod + scale);

    MULT(1); ADD(1); TRANS(1); /* counting previous operation */

    BRANCH(1);
    if (value >= 0.0) {
      TRANS(1); MULT(1);
      COUNT_sub_end();
      return (float) (pow (value, 4.0 / 3.0) * scalefactor) ;
    }
    else {
      TRANS(1); MULT(3);
      COUNT_sub_end();
      return (float) (-pow (-value, 4.0 / 3.0) * scalefactor) ;
    }
  }
  else {
    COUNT_sub_end();
    return (0.0) ;
  }

}

/*
  The function reads the huffman codeword from the bitstream and
  returns the index value.

  return:  index value
*/
int CBlock_DecodeHuffmanWord(HANDLE_BIT_BUF bs,                                 /*!< pointer to bitstream */
                             const unsigned short (*CodeBook) [HuffmanEntries]) /*!< pointer to sideinfo of the current codebook */
{
  unsigned int val;
  unsigned int index;

  COUNT_sub_start("CBlock_DecodeHuffmanWord");

  MOVE(1);
  index = 0;

  PTR_INIT(1); /* pointer for CodeBook[] */
  LOOP(1);
  while (1)
  {
    FUNC(2); INDIRECT(1);
    val = CodeBook[index][GetBits(bs,HuffmanBits)];
 
    LOGIC(1); BRANCH(1);
    if ((val & 1) == 0)
    {
      SHIFT(1);
      index = val>>2;
      continue;
    }
    else
    {
      LOGIC(1); BRANCH(1);
      if (val & 2)
      {
        FUNC(2);
        PushBack(bs,1);
      }

      SHIFT(1);
      val >>= 2;
      break;
    }
  }
  COUNT_sub_end();
  return val;
}

/*
  The function reads the escape sequence from the bitstream,
  if the absolute value of the quantized coefficient has the
  value 16.

  return:  quantized coefficient
*/
int CBlock_GetEscape(HANDLE_BIT_BUF bs,  /*!< pointer to bitstream */
                     const int q)        /*!< quantized coefficient */
{
  int i, off, neg ;

  COUNT_sub_start("CBlock_GetEscape");

  BRANCH(1);
  if (q < 0)
  {
    ADD(1); BRANCH(1);
    if (q != -16) {
      COUNT_sub_end();
      return q;
    }

    MOVE(1);
    neg = 1;
  }
  else
  {
    ADD(1); BRANCH(1);
    if (q != +16) {

      COUNT_sub_end();
      return q;
    }

    MOVE(1);
    neg = 0;
  }

  LOOP(1);
  for (i=4; ; i++)
  {
    FUNC(2); BRANCH(1);
    if (GetBits(bs,1) == 0)
      break;
  }

  ADD(1); BRANCH(1);
  if (i > 16)
  {
    ADD(2); BRANCH(1);
    if (i - 16 > (int) LongSize) { /* cannot read more than "LongSize" bits at once in the function ReadBits() */
      COUNT_sub_end();
      return (MAX_QUANTIZED_VALUE + 1); /* returning invalid value that will be captured later */
    }

    ADD(1); FUNC(2); SHIFT(1);
    off = GetBits(bs,i-16) << 16;

    FUNC(2); LOGIC(1);
    off |= GetBits(bs,16);
  }
  else
  {
    FUNC(2);
    off = GetBits(bs,i);
  }

  ADD(1); SHIFT(1);
  i = off + (1 << i);

  BRANCH(1);
  if (neg) {
    MULT(1);
    i = -i;
  }

  COUNT_sub_end();

  return i;
}

/*
  The function converts the index values to quantized coefficients.

  return:  dimension
*/
int CBlock_UnpackIndex(int idx,                        /*!< pointer to index */
                       int *qp,                        /*!< pointer to quantized coefficients */
                       const CodeBookDescription *hcb) /*!< pointer to sideinfo of the current codebook */
{
  int offset, bits, mask;

  COUNT_sub_start("CBlock_UnpackIndex");

  INDIRECT(2); MOVE(2);
  offset = hcb->Offset;
  bits = hcb->numBits;

  SHIFT(1); ADD(1);
  mask = (1<<bits)-1;

  ADD(1); INDIRECT(1); BRANCH(1);
  if (hcb->Dimension == 4)
  {
    LOGIC(1); ADD(1); STORE(1);
    qp[0] = (idx & mask)-offset;

    SHIFT(1);
    idx >>= bits;

    LOGIC(1); ADD(1); STORE(1);
    qp[1] = (idx & mask)-offset;

    SHIFT(1);
    idx >>= bits;

    LOGIC(1); ADD(1); STORE(1);
    qp[2] = (idx & mask)-offset;

    SHIFT(1);
    idx >>= bits;

    LOGIC(1); ADD(1); STORE(1);
    qp[3] = (idx & mask)-offset;;
  }
  else
  {
    LOGIC(1); ADD(1); STORE(1);
    qp[0] = (idx & mask)-offset;

    SHIFT(1);
    idx >>= bits;

    LOGIC(1); ADD(1); STORE(1);
    qp[1] = (idx & mask)-offset;
  }

  INDIRECT(1); /* for hcb->Dimension */
  COUNT_sub_end();

  return hcb->Dimension;
}

/*
  The function reads the element of the individual channel stream for
  long blocks.

  return:  none
*/
int CLongBlock_Read(HANDLE_BIT_BUF bs,                              /*!< pointer to bitstream */
                    CAacDecoderChannelInfo *pAacDecoderChannelInfo, /*!< pointer to aac decoder channel info */
                    unsigned char global_gain)                      /*!< global gain */
{
  int GainControlDataPresent;
  int ErrorStatus = AAC_DEC_OK;

  COUNT_sub_start("CLongBlock_Read");

  MOVE(1); /* counting previous operation */
  
  FUNC(2); ADD(1); BRANCH(1);
  if ((ErrorStatus = CLongBlock_ReadSectionData(bs,pAacDecoderChannelInfo)))
  {
    COUNT_sub_end();
    return (ErrorStatus);
  }

  FUNC(3);
  CLongBlock_ReadScaleFactorData(bs,pAacDecoderChannelInfo,global_gain);

  INDIRECT(1); PTR_INIT(1); FUNC(2);
  CPulseData_Read(bs,&pAacDecoderChannelInfo->PulseData);

  FUNC(2);
  CTns_ReadDataPresentFlag(bs,pAacDecoderChannelInfo);

  FUNC(2);
  CTns_Read(bs,pAacDecoderChannelInfo);

  FUNC(2);
  GainControlDataPresent = GetBits(bs,1);

  BRANCH(1);
  if (GainControlDataPresent)
  {
    COUNT_sub_end();
    return (AAC_DEC_UNIMPLEMENTED_GAIN_CONTROL_DATA);
  }

  FUNC(2); ADD(1); BRANCH(1);
  if ((ErrorStatus = CLongBlock_ReadSpectralData(bs, pAacDecoderChannelInfo)))
  {
    COUNT_sub_end();
    return (ErrorStatus);
  }

  COUNT_sub_end();

  return (ErrorStatus);
}

/*
  The function reads the element of the individual channel stream for
  short blocks. Gain control data is not supported.
*/
int CShortBlock_Read(HANDLE_BIT_BUF bs,                              /*!< pointer to bitstream */
                     CAacDecoderChannelInfo *pAacDecoderChannelInfo, /*!< pointer to aac decoder channel info */
                     unsigned char global_gain)                      /*!< global gain */
{
  int GainControlDataPresent;
  int ErrorStatus = AAC_DEC_OK;
  
  COUNT_sub_start("CShortBlock_Read");

  MOVE(1); /* counting previous operation */

  FUNC(2); ADD(1); BRANCH(1);
  if ((ErrorStatus = CShortBlock_ReadSectionData(bs,pAacDecoderChannelInfo)))
  {
    COUNT_sub_end();
    return (ErrorStatus);
  }

  FUNC(3);
  CShortBlock_ReadScaleFactorData(bs,pAacDecoderChannelInfo,global_gain);

  INDIRECT(1); PTR_INIT(1); FUNC(3);
  CPulseData_Read(bs,&pAacDecoderChannelInfo->PulseData);

  FUNC(2);
  CTns_ReadDataPresentFlag(bs,pAacDecoderChannelInfo);

  FUNC(2);
  CTns_Read(bs,pAacDecoderChannelInfo);

  FUNC(2);
  GainControlDataPresent = GetBits(bs,1);

  BRANCH(1);
  if (GainControlDataPresent)
  {
    COUNT_sub_end();
    return (AAC_DEC_UNIMPLEMENTED_GAIN_CONTROL_DATA);
  }

  FUNC(2); ADD(1); BRANCH(1);
  if ((ErrorStatus = CShortBlock_ReadSpectralData(bs,pAacDecoderChannelInfo)))
  {
    COUNT_sub_end();

    return (ErrorStatus);   
  }


  COUNT_sub_end();
  return (ErrorStatus);
}

void ApplyTools(CAacDecoderChannelInfo *pAacDecoderChannelInfo[],
                int channel)
{
  COUNT_sub_start("ApplyTools");

  FUNC(2);
  CPns_Apply(pAacDecoderChannelInfo, channel);

  INDIRECT(1); FUNC(1);
  CTns_Apply(pAacDecoderChannelInfo[channel]);

  COUNT_sub_end();
}

void 
Lap1(float *coef,         /*!< pointer to current spectrum */
     float *prev,         /*!< pointer to previous spectrum */
     float *out,          /*!< pointer to output time samples */
     const float *window, /*!< pointer to window coefficients */
     int size,            /*!< number of spectral lines */
     int stride)          /*!< stride */
{
  int i;

  COUNT_sub_start("Lap1");

  PTR_INIT(5); /* pointers for out[],
                               coef[],
                               window[i],
                               window[size*2-1-i], 
                               prev[]              */
  LOOP(1);
  for (i=0; i<size; i++) {

    MULT(2); ADD(1); STORE(1);
    out[stride*i] = (coef[size+i]*window[i]) - (window[size*2-1-i]*prev[size-1-i]);
  }

  PTR_INIT(5); /* pointers for out[],
                               coef[],
                               window[i],
                               window[size*2-1-i], 
                               prev[]              */
  LOOP(1);
  for (i=0; i<size; i++) {

    MULT(2); MAC(1); STORE(1);
    out[stride*(i+size)] = (-coef[size*2-1-i]*window[size+i]) - (window[size-1-i]*prev[i]);
  }

  COUNT_sub_end();
}

void 
Lap2(float *coef,         /*!< pointer to current spectrum */
     float *prev,         /*!< pointer to previous spectrum */
     float *out,          /*!< pointer to output time samples */
     const float *window, /*!< pointer to window coefficients */
     int size,            /*!< number of spectral lines */
     int stride)          /*!< stride */
{
  int i;

  COUNT_sub_start("Lap2");

  PTR_INIT(5); /* pointers for out[],
                               coef[],
                               window[i],
                               window[size*2-1-i], 
                               prev[]              */
  LOOP(1);
  for (i=0; i<size; i++) {

    MULT(2); ADD(1); STORE(1);
    out[stride*i] = coef[size+i]*window[i] - window[size*2-1-i]*prev[size-1-i];
  }

  PTR_INIT(5); /* pointers for out[],
                               coef[],
                               window[i],
                               window[size*2-1-i], 
                               prev[]              */
  LOOP(1);
  for (i=0; i<size; i++) {

    MULT(2); MAC(1); STORE(1);
    out[stride*(i+size)] = -coef[size*2-1-i]*window[size+i] - window[size-1-i]*prev[i];
  }

  COUNT_sub_end();
}

