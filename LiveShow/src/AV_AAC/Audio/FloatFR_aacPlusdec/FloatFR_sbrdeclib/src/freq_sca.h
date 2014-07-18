/*
  Frequency scale prototypes
*/
#ifndef __FREQ_SCA_H
#define __FREQ_SCA_H

#include "env_extr.h"

int
sbrdecUpdateFreqScale(unsigned char * v_k_master,
                      unsigned char *numMaster,
                      SBR_HEADER_DATA * headerData);

void sbrdecUpdateHiRes(unsigned char * v_hires, unsigned char *num_hires, unsigned char * v_k_master, unsigned char num_bands, unsigned char xover_band);
void sbrdecUpdateLoRes(unsigned char * v_lores, unsigned char *num_lores, unsigned char * v_hires, unsigned char num_hires);


void sbrdecDownSampleLoRes(unsigned char *v_result, unsigned char num_result,
                           unsigned char *freqBandTableRef, unsigned char num_Ref);

int sbrdecGetSbrStartFreqRAW (int startFreq, int QMFbands, int fs );
int sbrdecGetSbrStopFreqRAW  (int stopFreq, int QMFbands, int fs);

void shellsort(unsigned char *in, unsigned char n);

int
resetFreqBandTables(HANDLE_SBR_HEADER_DATA hHeaderData);

#endif
