/*
  Fast FFT routines prototypes
*/
#include <string.h>
#include <assert.h>
#include "sbr_fft.h"

#include "counters.h" /* the 3GPP instrumenting tool */

#define INV_SQRT2    7.071067811865475e-1f
#define COS_PI_DIV8  9.238795325112867e-1f
#define COS_3PI_DIV8 3.826834323650898e-1f
#define SQRT2PLUS1   2.414213562373095f
#define SQRT2MINUS1  4.142135623730952e-1f

static const float c[4] = { 9.807852804032304e-1f,
                            8.314696123025452e-1f,
                            5.555702330196023e-1f,
                            1.950903220161283e-1f };

/*
  Computes a 4-point FFT. Complex-valued input takes 16 real additions.
*/
static void fft4(float *vec)
{
  float temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;

  COUNT_sub_start("fft4");

  ADD(8);
  temp0 = vec[0] + vec[4];
  temp2 = vec[0] - vec[4];
  temp1 = vec[1] + vec[5];
  temp3 = vec[1] - vec[5];
  temp4 = vec[2] + vec[6];
  temp7 = vec[2] - vec[6];
  temp5 = vec[7] + vec[3];
  temp6 = vec[7] - vec[3];

  ADD(8); STORE(8);
  vec[0] = temp0 + temp4;
  vec[1] = temp1 + temp5;
  vec[2] = temp2 - temp6;
  vec[3] = temp3 - temp7;
  vec[4] = temp0 - temp4;
  vec[5] = temp1 - temp5;
  vec[6] = temp2 + temp6;
  vec[7] = temp3 + temp7;

  COUNT_sub_end();
}


/*
  Computes an 8-point FFT. Complex-valued input takes 52 real additions
  and 4 real multiplications.
*/
static void fft8(float *  vec)
{
  float temp1[16];
  float temp2[16];

  COUNT_sub_start("fft8");

  ADD(16);
  temp1[0]  = vec[0] + vec[8];
  temp1[2]  = vec[0] - vec[8];
  temp1[1]  = vec[1] + vec[9];
  temp1[3]  = vec[1] - vec[9];
  temp1[4]  = vec[2] + vec[10];
  temp1[6]  = vec[2] - vec[10];
  temp1[5]  = vec[3] + vec[11];
  temp1[7]  = vec[3] - vec[11];
  temp1[8]  = vec[4] + vec[12];
  temp1[10] = vec[4] - vec[12];
  temp1[9]  = vec[5] + vec[13];
  temp1[11] = vec[5] - vec[13];
  temp1[12] = vec[6] + vec[14];
  temp1[14] = vec[6] - vec[14];
  temp1[13] = vec[7] + vec[15];
  temp1[15] = vec[7] - vec[15];

  ADD(12);
  temp2[0]  =  temp1[0] + temp1[8];
  temp2[4]  =  temp1[0] - temp1[8];
  temp2[1]  =  temp1[1] + temp1[9];
  temp2[5]  =  temp1[1] - temp1[9];
  temp2[8]  =  temp1[2] - temp1[11];
  temp2[10] =  temp1[2] + temp1[11];
  temp2[9]  =  temp1[3] + temp1[10];
  temp2[11] =  temp1[3] - temp1[10];
  temp2[2]  =  temp1[4] + temp1[12];
  temp2[7]  =  temp1[4] - temp1[12];
  temp2[3]  =  temp1[5] + temp1[13];
  temp2[6]  =  temp1[13]- temp1[5];

  ADD(4);
  temp1[1]  =  temp1[6] + temp1[14];
  temp1[2]  =  temp1[6] - temp1[14];
  temp1[0]  =  temp1[7] + temp1[15];
  temp1[3]  =  temp1[7] - temp1[15];

  ADD(4); MULT(4);
  temp2[12] = (temp1[0] + temp1[2]) *  INV_SQRT2;
  temp2[14] = (temp1[0] - temp1[2]) *  INV_SQRT2;
  temp2[13] = (temp1[3] - temp1[1]) *  INV_SQRT2;
  temp2[15] = (temp1[1] + temp1[3]) * -INV_SQRT2;

  ADD(16); STORE(16);
  vec[0]  = temp2[0] + temp2[2];
  vec[8]  = temp2[0] - temp2[2];
  vec[1]  = temp2[1] + temp2[3];
  vec[9]  = temp2[1] - temp2[3];
  vec[4]  = temp2[4] - temp2[6];
  vec[12] = temp2[4] + temp2[6];
  vec[5]  = temp2[5] - temp2[7];
  vec[13] = temp2[5] + temp2[7];
  vec[6]  = temp2[8] + temp2[14];
  vec[14] = temp2[8] - temp2[14];
  vec[7]  = temp2[9] + temp2[15];
  vec[15] = temp2[9] - temp2[15];
  vec[2]  = temp2[10]+ temp2[12];
  vec[10] = temp2[10]- temp2[12];
  vec[3]  = temp2[11]+ temp2[13];
  vec[11] = temp2[11]- temp2[13];

  COUNT_sub_end();
}


/*
  Computes a 16-point FFT. Complex-valued input takes 144 real additions and
  24 real multiplications.
*/
static void fft16(float *  vec)
{
  float temp10, temp11, temp12, temp13, temp14, temp15, temp16, temp17,
    temp18, temp19, temp110, temp111, temp112, temp113, temp114, temp115;
  float temp20, temp21, temp22, temp23, temp24, temp25, temp26, temp27,
    temp28, temp29, temp210, temp211, temp212, temp213, temp214, temp215;
  float vec0, vec1, vec2, vec3, vec4, vec5, vec6, vec7,
    vec8, vec9, vec10, vec11, vec12, vec13, vec14, vec15;

  COUNT_sub_start("fft16");

  ADD(16);
  vec0 = vec[0] + vec[16];
  vec1 = vec[1] + vec[17];
  vec2 = vec[2] + vec[18];
  vec3 = vec[3] + vec[19];
  vec4 = vec[4] + vec[20];
  vec5 = vec[5] + vec[21];
  vec6 = vec[6] + vec[22];
  vec7 = vec[7] + vec[23];
  vec8 = vec[8] + vec[24];
  vec9 = vec[9] + vec[25];
  vec10 = vec[10] + vec[26];
  vec11 = vec[11] + vec[27];
  vec12 = vec[12] + vec[28];
  vec13 = vec[13] + vec[29];
  vec14 = vec[14] + vec[30];
  vec15 = vec[15] + vec[31];

  ADD(16);
  temp10  = vec0 + vec8;
  temp12  = vec0 - vec8;
  temp11  = vec1 + vec9;
  temp13  = vec1 - vec9;
  temp14  = vec2 + vec10;
  temp16  = vec2 - vec10;
  temp15  = vec3 + vec11;
  temp17  = vec3 - vec11;
  temp18  = vec4 + vec12;
  temp110 = vec4 - vec12;
  temp19  = vec5 + vec13;
  temp111 = vec5 - vec13;
  temp112 = vec6 + vec14;
  temp114 = vec6 - vec14;
  temp113 = vec7 + vec15;
  temp115 = vec7 - vec15;

  ADD(12);
  temp20  =  temp10 + temp18;
  temp24  =  temp10 - temp18;
  temp21  =  temp11 + temp19;
  temp25  =  temp11 - temp19;
  temp28  =  temp12 - temp111;
  temp210 =  temp12 + temp111;
  temp29  =  temp13 + temp110;
  temp211 =  temp13 - temp110;
  temp22  =  temp14 + temp112;
  temp27  =  temp14 - temp112;
  temp23  =  temp15 + temp113;
  temp26  =  temp113- temp15;

  ADD(4);
  temp11  =  temp16 + temp114;
  temp12  =  temp16 - temp114;
  temp10  =  temp17 + temp115;
  temp13  =  temp17 - temp115;

  ADD(4); MULT(4);
  temp212 = (temp10 + temp12) *  INV_SQRT2;
  temp214 = (temp10 - temp12) *  INV_SQRT2;
  temp213 = (temp13 - temp11) *  INV_SQRT2;
  temp215 = (temp11 + temp13) * -INV_SQRT2;

  ADD(16);
  vec0 = vec[0] - vec[16];
  vec1 = vec[1] - vec[17];
  vec2 = vec[2] - vec[18];
  vec3 = vec[3] - vec[19];
  vec4 = vec[4] - vec[20];
  vec5 = vec[5] - vec[21];
  vec6 = vec[6] - vec[22];
  vec7 = vec[7] - vec[23];
  vec8 = vec[8] - vec[24];
  vec9 = vec[9] - vec[25];
  vec10 = vec[10] - vec[26];
  vec11 = vec[11] - vec[27];
  vec12 = vec[12] - vec[28];
  vec13 = vec[13] - vec[29];
  vec14 = vec[14] - vec[30];
  vec15 = vec[15] - vec[31];

  ADD(12); MULT(12);
  temp19  = (vec2 + vec14) * -COS_3PI_DIV8;
  temp110 = (vec2 - vec14) * COS_PI_DIV8;
  temp18  = (vec3 + vec15) * COS_3PI_DIV8;
  temp111 = (vec3 - vec15) *  COS_PI_DIV8;
  temp15  = (vec4 + vec12) * -INV_SQRT2;
  temp16  = (vec4 - vec12) * INV_SQRT2;
  temp14  = (vec5 + vec13) * INV_SQRT2;
  temp17  = (vec5 - vec13) *  INV_SQRT2;
  temp113 = (vec6 + vec10) * -COS_PI_DIV8;
  temp114 = (vec6 - vec10) * COS_3PI_DIV8;
  temp112 = (vec7 + vec11) * COS_PI_DIV8;
  temp115 = (vec7 - vec11) *  COS_3PI_DIV8;

  MULT(4); MAC(4);
  vec2 = temp18  * SQRT2PLUS1  - temp112 * SQRT2MINUS1;
  vec3 = temp19  * SQRT2PLUS1  - temp113 * SQRT2MINUS1;
  vec4 = temp110 * SQRT2MINUS1 - temp114 * SQRT2PLUS1;
  vec5 = temp111 * SQRT2MINUS1 - temp115 * SQRT2PLUS1;

  ADD(4);
  temp18 += temp112;
  temp19 += temp113;
  temp110+= temp114;
  temp111+= temp115;

  ADD(4);
  vec6   = vec0  + temp14;
  vec10  = vec0  - temp14;
  vec7   = vec1  + temp15;
  vec11  = vec1  - temp15;

  ADD(4);
  vec12  = temp16 - vec9;
  vec14  = temp16 + vec9;
  vec13  = vec8  + temp17;
  vec15  = vec8  - temp17;

  ADD(8);
  temp10  = vec6  - vec14;
  temp12  = vec6  + vec14;
  temp11  = vec7  + vec15;
  temp13  = vec7  - vec15;
  temp14  = vec10 + vec12;
  temp16  = vec10 - vec12;
  temp15  = vec11 + vec13;
  temp17  = vec11 - vec13;

  ADD(4);
  vec10  = temp18 + temp110;
  temp110 = temp18 - temp110;
  vec11  = temp19 + temp111;
  temp111 = temp19 - temp111;

  ADD(4);
  temp112 = vec2  + vec4;
  temp114 = vec2  - vec4;
  temp113 = vec3  + vec5;
  temp115 = vec3  - vec5;

  ADD(32); STORE(32);
  *vec++ = temp20 + temp22;
  *vec++ = temp21 + temp23;
  *vec++ = temp12 + vec10;
  *vec++ = temp13 + vec11;
  *vec++ = temp210+ temp212;
  *vec++ = temp211+ temp213;
  *vec++ = temp10 + temp112;
  *vec++ = temp11 + temp113;
  *vec++ = temp24 - temp26;
  *vec++ = temp25 - temp27;
  *vec++ = temp16 + temp114;
  *vec++ = temp17 + temp115;
  *vec++ = temp28 + temp214;
  *vec++ = temp29 + temp215;
  *vec++ = temp14 + temp110;
  *vec++ = temp15 + temp111;
  *vec++ = temp20 - temp22;
  *vec++ = temp21 - temp23;
  *vec++ = temp12 - vec10;
  *vec++ = temp13 - vec11;
  *vec++ = temp210- temp212;
  *vec++ = temp211- temp213;
  *vec++ = temp10 - temp112;
  *vec++ = temp11 - temp113;
  *vec++ = temp24 + temp26;
  *vec++ = temp25 + temp27;
  *vec++ = temp16 - temp114;
  *vec++ = temp17 - temp115;
  *vec++ = temp28 - temp214;
  *vec++ = temp29 - temp215;
  *vec++ = temp14 - temp110;
  *vec++ = temp15 - temp111;


  COUNT_sub_end();
}


/*
  Computes a 32-point FFT. Complex-valued input takes 380 real additions and
  108 real multiplications.
*/
static void fft32(float *  vec)
{
  float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7,
    tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15;
  float temp10, temp11, temp12, temp13, temp14, temp15, temp16, temp17,
    temp18, temp19, temp110, temp111, temp112, temp113, temp114, temp115;
  float temp20, temp21, temp22, temp23, temp24, temp25, temp26, temp27,
    temp28, temp29, temp210, temp211, temp212, temp213, temp214, temp215;
  float temp30, temp31, temp32, temp33, temp34, temp35, temp36, temp37,
    temp38, temp39, temp310, temp311, temp312, temp313, temp314, temp315;
  float temp316, temp317, temp318, temp319, temp320, temp321, temp322, temp323,
    temp324, temp325, temp326, temp327, temp328, temp329, temp330, temp331;
  float temp40, temp41, temp42, temp43, temp44, temp45, temp46, temp47,
    temp48, temp49, temp410, temp411, temp412, temp413, temp414, temp415;

  COUNT_sub_start("fft32");

  ADD(6);
  temp20   = vec[2]  - vec[34];
  temp21   = vec[3]  - vec[35];
  temp30   = vec[0]  + vec[32];
  temp31   = vec[1]  + vec[33];
  temp32   = vec[2]  + vec[34];
  temp33   = vec[3]  + vec[35];

  ADD(6);
  temp22   = vec[6]  - vec[38];
  temp23   = vec[7]  - vec[39];
  temp34   = vec[4]  + vec[36];
  temp35   = vec[5]  + vec[37];
  temp36   = vec[6]  + vec[38];
  temp37   = vec[7]  + vec[39];

  ADD(6);
  temp24   = vec[10] - vec[42];
  temp25   = vec[11] - vec[43];
  temp38   = vec[8]  + vec[40];
  temp39   = vec[9]  + vec[41];
  temp310  = vec[10] + vec[42];
  temp311  = vec[11] + vec[43];

  ADD(6);
  temp26   = vec[14] - vec[46];
  temp27   = vec[15] - vec[47];
  temp312  = vec[12] + vec[44];
  temp313  = vec[13] + vec[45];
  temp314  = vec[14] + vec[46];
  temp315  = vec[15] + vec[47];

  ADD(6);
  temp28   = vec[18] - vec[50];
  temp29   = vec[19] - vec[51];
  temp316  = vec[16] + vec[48];
  temp317  = vec[17] + vec[49];
  temp318  = vec[18] + vec[50];
  temp319  = vec[19] + vec[51];

  ADD(6);
  temp210  = vec[22] - vec[54];
  temp211  = vec[23] - vec[55];
  temp320  = vec[20] + vec[52];
  temp321  = vec[21] + vec[53];
  temp322  = vec[22] + vec[54];
  temp323  = vec[23] + vec[55];

  ADD(6);
  temp212  = vec[26] - vec[58];
  temp213  = vec[27] - vec[59];
  temp324  = vec[24] + vec[56];
  temp325  = vec[25] + vec[57];
  temp326  = vec[26] + vec[58];
  temp327  = vec[27] + vec[59];

  ADD(6);
  temp214  = vec[30] - vec[62];
  temp215  = vec[31] - vec[63];
  temp328  = vec[28] + vec[60];
  temp329  = vec[29] + vec[61];
  temp330  = vec[30] + vec[62];
  temp331  = vec[31] + vec[63];

  ADD(16);
  temp41  = -(temp20 + temp214);
  temp42  =   temp20 - temp214;
  temp40  =   temp21 + temp215;
  temp43  =   temp21 - temp215;
  temp45  = -(temp22 + temp212);
  temp46  =   temp22 - temp212;
  temp44  =   temp23 + temp213;
  temp47  =   temp23 - temp213;
  temp49  = -(temp24 + temp210);
  temp410 =   temp24 - temp210;
  temp48  =   temp25 + temp211;
  temp411 =   temp25 - temp211;
  temp413 = -(temp26 + temp28);
  temp414 =   temp26 - temp28;
  temp412 =   temp27 + temp29;
  temp415 =   temp27 - temp29;

  MULT(1); MAC(3);
  temp20  = temp40 * c[3] + temp44 * c[2] + temp48  * c[1] + temp412 * c[0];

  MULT(4); MAC(4); ADD(2);
  temp24  = temp40 * c[2] + temp44 * c[0] + temp48  * c[3] - temp412 * c[1];
  temp28  = temp40 * c[1] + temp44 * c[3] - temp48  * c[0] + temp412 * c[2];

  MULT(3); MAC(1); ADD(2);
  temp212 = temp40 * c[0] - temp44 * c[1] + temp48  * c[2] - temp412 * c[3];

  MULT(1); MAC(3);
  temp21  = temp41 * c[3] + temp45 * c[2] + temp49  * c[1] + temp413 * c[0];

  MULT(4); MAC(4); ADD(2);
  temp25  = temp41 * c[2] + temp45 * c[0] + temp49  * c[3] - temp413 * c[1];
  temp29  = temp41 * c[1] + temp45 * c[3] - temp49  * c[0] + temp413 * c[2];

  MULT(3); MAC(1); ADD(2);
  temp213 = temp41 * c[0] - temp45 * c[1] + temp49  * c[2] - temp413 * c[3];

  MULT(1); MAC(3);
  temp22  = temp42 * c[0] + temp46 * c[1] + temp410 * c[2] + temp414 * c[3];

  MULT(4); ADD(3);
  temp26  = temp42 * c[1] - temp46 * c[3] - temp410 * c[0] - temp414 * c[2];

  MULT(2); MAC(2); ADD(1);
  temp210 = temp42 * c[2] - temp46 * c[0] + temp410 * c[3] + temp414 * c[1];

  MULT(3); MAC(1); ADD(2);
  temp214 = temp42 * c[3] - temp46 * c[2] + temp410 * c[1] - temp414 * c[0];

  MULT(1); MAC(3);
  temp23  = temp43 * c[0] + temp47 * c[1] + temp411 * c[2] + temp415 * c[3];

  MULT(4); ADD(3);
  temp27  = temp43 * c[1] - temp47 * c[3] - temp411 * c[0] - temp415 * c[2];

  MULT(2); MAC(2); ADD(1);
  temp211 = temp43 * c[2] - temp47 * c[0] + temp411 * c[3] + temp415 * c[1];

  MULT(3); MAC(1); ADD(2);
  temp215 = temp43 * c[3] - temp47 * c[2] + temp411 * c[1] - temp415 * c[0];

  ADD(16);
  temp40  = temp20  + temp22;
  temp414 = temp20  - temp22;
  temp41  = temp21  + temp23;
  temp415 = temp21  - temp23;
  temp42  = temp24  + temp26;
  temp412 = temp24  - temp26;
  temp43  = temp25  + temp27;
  temp413 = temp25  - temp27;
  temp44  = temp28  + temp210;
  temp410 = temp28  - temp210;
  temp45  = temp29  + temp211;
  temp411 = temp29  - temp211;
  temp46  = temp212 + temp214;
  temp48  = temp212 - temp214;
  temp47  = temp213 + temp215;
  temp49  = temp213 - temp215;

  ADD(16);
  temp10 = temp30 + temp316;
  temp11 = temp31 + temp317;
  temp12 = temp32 + temp318;
  temp13 = temp33 + temp319;
  temp14 = temp34 + temp320;
  temp15 = temp35 + temp321;
  temp16 = temp36 + temp322;
  temp17 = temp37 + temp323;
  temp18 = temp38 + temp324;
  temp19 = temp39 + temp325;
  temp110 = temp310 + temp326;
  temp111 = temp311 + temp327;
  temp112 = temp312 + temp328;
  temp113 = temp313 + temp329;
  temp114 = temp314 + temp330;
  temp115 = temp315 + temp331;

  ADD(16);
  tmp0  = temp10 + temp18;
  tmp2  = temp10 - temp18;
  tmp1  = temp11 + temp19;
  tmp3  = temp11 - temp19;
  tmp4  = temp12 + temp110;
  tmp6  = temp12 - temp110;
  tmp5  = temp13 + temp111;
  tmp7  = temp13 - temp111;
  tmp8  = temp14 + temp112;
  tmp10 = temp14 - temp112;
  tmp9  = temp15 + temp113;
  tmp11 = temp15 - temp113;
  tmp12 = temp16 + temp114;
  tmp14 = temp16 - temp114;
  tmp13 = temp17 + temp115;
  tmp15 = temp17 - temp115;

  ADD(12);
  temp20  =  tmp0 + tmp8;
  temp24  =  tmp0 - tmp8;
  temp21  =  tmp1 + tmp9;
  temp25  =  tmp1 - tmp9;
  temp28  =  tmp2 - tmp11;
  temp210 =  tmp2 + tmp11;
  temp29  =  tmp3 + tmp10;
  temp211 =  tmp3 - tmp10;
  temp22  =  tmp4 + tmp12;
  temp27  =  tmp4 - tmp12;
  temp23  =  tmp5 + tmp13;
  temp26  =  tmp13- tmp5;

  ADD(4);
  tmp1  =  tmp6 + tmp14;
  tmp2  =  tmp6 - tmp14;
  tmp0  =  tmp7 + tmp15;
  tmp3  =  tmp7 - tmp15;

  ADD(4); MULT(4);
  temp212 = (tmp0 + tmp2) *  INV_SQRT2;
  temp214 = (tmp0 - tmp2) *  INV_SQRT2;
  temp213 = (tmp3 - tmp1) *  INV_SQRT2;
  temp215 = (tmp1 + tmp3) * -INV_SQRT2;

  ADD(16);
  temp10 = temp30 - temp316;
  temp11 = temp31 - temp317;
  temp12 = temp32 - temp318;
  temp13 = temp33 - temp319;
  temp14 = temp34 - temp320;
  temp15 = temp35 - temp321;
  temp16 = temp36 - temp322;
  temp17 = temp37 - temp323;
  temp18 = temp38 - temp324;
  temp19 = temp39 - temp325;
  temp110 = temp310 - temp326;
  temp111 = temp311 - temp327;
  temp112 = temp312 - temp328;
  temp113 = temp313 - temp329;
  temp114 = temp314 - temp330;
  temp115 = temp315 - temp331;

  ADD(16);
  temp30  = temp20 + temp22;
  temp316 = temp20 - temp22;
  temp31  = temp21 + temp23;
  temp317 = temp21 - temp23;
  temp38  = temp24 - temp26;
  temp324 = temp24 + temp26;
  temp39  = temp25 - temp27;
  temp325 = temp25 + temp27;
  temp312 = temp28 + temp214;
  temp328 = temp28 - temp214;
  temp313 = temp29 + temp215;
  temp329 = temp29 - temp215;
  temp34  = temp210+ temp212;
  temp320 = temp210- temp212;
  temp35  = temp211+ temp213;
  temp321 = temp211- temp213;

  ADD(12); MULT(12);
  tmp9  = (temp12 + temp114) * -COS_3PI_DIV8;
  tmp10 = (temp12 - temp114) * COS_PI_DIV8;
  tmp8  = (temp13 + temp115) * COS_3PI_DIV8;
  tmp11 = (temp13 - temp115) *  COS_PI_DIV8;
  tmp5  = (temp14 + temp112) * -INV_SQRT2;
  tmp6  = (temp14 - temp112) * INV_SQRT2;
  tmp4  = (temp15 + temp113) * INV_SQRT2;
  tmp7  = (temp15 - temp113) *  INV_SQRT2;
  tmp13 = (temp16 + temp110) * -COS_PI_DIV8;
  tmp14 = (temp16 - temp110) * COS_3PI_DIV8;
  tmp12 = (temp17 + temp111) * COS_PI_DIV8;
  tmp15 = (temp17 - temp111) *  COS_3PI_DIV8;

  MULT(4); MAC(4);
  temp12 = tmp8  * SQRT2PLUS1  - tmp12 * SQRT2MINUS1;
  temp13 = tmp9  * SQRT2PLUS1  - tmp13 * SQRT2MINUS1;
  temp14 = tmp10 * SQRT2MINUS1 - tmp14 * SQRT2PLUS1;
  temp15 = tmp11 * SQRT2MINUS1 - tmp15 * SQRT2PLUS1;

  ADD(8);
  tmp8 += tmp12;
  tmp9 += tmp13;
  tmp10+= tmp14;
  tmp11+= tmp15;
  temp16   = temp10  + tmp4;
  temp110  = temp10  - tmp4;
  temp17   = temp11  + tmp5;
  temp111  = temp11  - tmp5;

  ADD(4);
  temp112  = tmp6 - temp19;
  temp114  = tmp6 + temp19;
  temp113  = temp18  + tmp7;
  temp115  = temp18  - tmp7;

  ADD(8);
  tmp0  = temp16  - temp114;
  tmp2  = temp16  + temp114;
  tmp1  = temp17  + temp115;
  tmp3  = temp17  - temp115;
  tmp4  = temp110 + temp112;
  tmp6  = temp110 - temp112;
  tmp5  = temp111 + temp113;
  tmp7  = temp111 - temp113;

  ADD(4);
  temp110  = tmp8 + tmp10;
  tmp10 = tmp8 - tmp10;
  temp111  = tmp9 + tmp11;
  tmp11 = tmp9 - tmp11;

  ADD(4);
  tmp12 = temp12  + temp14;
  tmp14 = temp12  - temp14;
  tmp13 = temp13  + temp15;
  tmp15 = temp13  - temp15;


  ADD(16);
  temp32  = tmp2 + temp110;
  temp318 = tmp2 - temp110;
  temp33  = tmp3 + temp111;
  temp319 = tmp3 - temp111;
  temp36  = tmp0 + tmp12;
  temp322 = tmp0 - tmp12;
  temp37  = tmp1 + tmp13;
  temp323 = tmp1 - tmp13;
  temp314 = tmp4 + tmp10;
  temp330 = tmp4 - tmp10;
  temp315 = tmp5 + tmp11;
  temp331 = tmp5 - tmp11;
  temp310 = tmp6 + tmp14;
  temp326 = tmp6 - tmp14;
  temp311 = tmp7 + tmp15;
  temp327 = tmp7 - tmp15;

  ADD(16);
  temp10   = vec[0]  - vec[32];
  temp11   = vec[1]  - vec[33];
  temp12   = vec[4]  - vec[36];
  temp13   = vec[5]  - vec[37];
  temp14   = vec[8]  - vec[40];
  temp15   = vec[9]  - vec[41];
  temp16   = vec[12] - vec[44];
  temp17   = vec[13] - vec[45];
  temp18   = vec[16] - vec[48];
  temp19   = vec[17] - vec[49];
  temp110  = vec[20] - vec[52];
  temp111  = vec[21] - vec[53];
  temp112  = vec[24] - vec[56];
  temp113  = vec[25] - vec[57];
  temp114  = vec[28] - vec[60];
  temp115  = vec[29] - vec[61];

  ADD(12); MULT(12);
  tmp9  = (temp12 + temp114) * -COS_3PI_DIV8;
  tmp10 = (temp12 - temp114) * COS_PI_DIV8;
  tmp8  = (temp13 + temp115) * COS_3PI_DIV8;
  tmp11 = (temp13 - temp115) *  COS_PI_DIV8;
  tmp5  = (temp14 + temp112) * -INV_SQRT2;
  tmp6  = (temp14 - temp112) * INV_SQRT2;
  tmp4  = (temp15 + temp113) * INV_SQRT2;
  tmp7  = (temp15 - temp113) *  INV_SQRT2;
  tmp13 = (temp16 + temp110) * -COS_PI_DIV8;
  tmp14 = (temp16 - temp110) * COS_3PI_DIV8;
  tmp12 = (temp17 + temp111) * COS_PI_DIV8;
  tmp15 = (temp17 - temp111) *  COS_3PI_DIV8;

  MULT(4); MAC(4);
  temp12 = tmp8  * SQRT2PLUS1  - tmp12 * SQRT2MINUS1;
  temp13 = tmp9  * SQRT2PLUS1  - tmp13 * SQRT2MINUS1;
  temp14 = tmp10 * SQRT2MINUS1 - tmp14 * SQRT2PLUS1;
  temp15 = tmp11 * SQRT2MINUS1 - tmp15 * SQRT2PLUS1;

  ADD(8);
  tmp8 += tmp12;
  tmp9 += tmp13;
  tmp10+= tmp14;
  tmp11+= tmp15;
  temp16   = temp10  + tmp4;
  temp110  = temp10  - tmp4;
  temp17   = temp11  + tmp5;
  temp111  = temp11  - tmp5;

  ADD(4);
  temp112  = tmp6 - temp19;
  temp114  = tmp6 + temp19;
  temp113  = temp18  + tmp7;
  temp115  = temp18  - tmp7;

  ADD(8);
  tmp0  = temp16  - temp114;
  tmp2  = temp16  + temp114;
  tmp1  = temp17  + temp115;
  tmp3  = temp17  - temp115;
  tmp4  = temp110 + temp112;
  tmp6  = temp110 - temp112;
  tmp5  = temp111 + temp113;
  tmp7  = temp111 - temp113;

  ADD(4);
  temp110  = tmp8 + tmp10;
  tmp10 = tmp8 - tmp10;
  temp111  = tmp9 + tmp11;
  tmp11 = tmp9 - tmp11;

  ADD(4);
  tmp12 = temp12  + temp14;
  tmp14 = temp12  - temp14;
  tmp13 = temp13  + temp15;
  tmp15 = temp13  - temp15;


  ADD(16);
  temp10  = tmp2 + temp110;
  temp18  = tmp2 - temp110;
  temp11  = tmp3 + temp111;
  temp19  = tmp3 - temp111;
  temp12  = tmp0 + tmp12;
  temp110 = tmp0 - tmp12;
  temp13  = tmp1 + tmp13;
  temp111 = tmp1 - tmp13;
  temp16  = tmp4 + tmp10;
  temp114 = tmp4 - tmp10;
  temp17  = tmp5 + tmp11;
  temp115 = tmp5 - tmp11;
  temp14  = tmp6 + tmp14;
  temp112 = tmp6 - tmp14;
  temp15  = tmp7 + tmp15;
  temp113 = tmp7 - tmp15;

  ADD(32); STORE(32);
  MOVE(32);
  *vec++ = temp30;
  *vec++ = temp31;
  *vec++ = temp10 + temp40;
  *vec++ = temp11 + temp41;
  *vec++ = temp32;
  *vec++ = temp33;
  *vec++ = temp12 + temp42;
  *vec++ = temp13 + temp43;
  *vec++ = temp34;
  *vec++ = temp35;
  *vec++ = temp14 + temp44;
  *vec++ = temp15 + temp45;
  *vec++ = temp36;
  *vec++ = temp37;
  *vec++ = temp16 + temp46;
  *vec++ = temp17 + temp47;
  *vec++ = temp38;
  *vec++ = temp39;
  *vec++ = temp18 + temp48;
  *vec++ = temp19 + temp49;
  *vec++ = temp310;
  *vec++ = temp311;
  *vec++ = temp110 + temp410;
  *vec++ = temp111 + temp411;
  *vec++ = temp312;
  *vec++ = temp313;
  *vec++ = temp112 + temp412;
  *vec++ = temp113 + temp413;
  *vec++ = temp314;
  *vec++ = temp315;
  *vec++ = temp114 + temp414;
  *vec++ = temp115 + temp415;
  *vec++ = temp316;
  *vec++ = temp317;
  *vec++ = temp10 - temp40;
  *vec++ = temp11 - temp41;
  *vec++ = temp318;
  *vec++ = temp319;
  *vec++ = temp12 - temp42;
  *vec++ = temp13 - temp43;
  *vec++ = temp320;
  *vec++ = temp321;
  *vec++ = temp14 - temp44;
  *vec++ = temp15 - temp45;
  *vec++ = temp322;
  *vec++ = temp323;
  *vec++ = temp16 - temp46;
  *vec++ = temp17 - temp47;
  *vec++ = temp324;
  *vec++ = temp325;
  *vec++ = temp18 - temp48;
  *vec++ = temp19 - temp49;
  *vec++ = temp326;
  *vec++ = temp327;
  *vec++ = temp110 - temp410;
  *vec++ = temp111 - temp411;
  *vec++ = temp328;
  *vec++ = temp329;
  *vec++ = temp112 - temp412;
  *vec++ = temp113 - temp413;
  *vec++ = temp330;
  *vec++ = temp331;
  *vec++ = temp114 - temp414;
  *vec++ = temp115 - temp415;

  COUNT_sub_end();
}


/*
  Patch function for SBR FFT routines.
*/
void sbrfft(float *in, int length)
{
  COUNT_sub_start("sbrfft");

  BRANCH(2); FUNC(1);
  switch(length) {
  case 4:
    fft4(in);
    break;
  case 8:
    fft8(in);
   break;
  case 16:
    fft16(in);
    break;
  case 32:
    fft32(in);
    break;

  default:

    assert (0) ;
    break ;

  }
  COUNT_sub_end();
}
