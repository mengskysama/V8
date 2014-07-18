/*
  Memory layout
  This module declares all static and dynamic memory spaces
*/
#include <stdio.h>
#include "sbrdecoder.h"
#include "sbr_ram.h"
#include "FloatFR.h"
#include <map>
using namespace std;
#include "LibMutex.h"




//extern float WorkBufferCore[];

/*
  \name StaticSbrData

  Static memory areas, must not be overwritten in other sections of the decoder
*/
/* @{ */

// /*  Filter states for QMF-analysis. <br>
//   Dimension: #MAXNRQMFCHANNELS * #QMF_FILTER_STATE_ANA_SIZE */
// float sbr_CodecQmfStatesAnalysis[MAXNRQMFCHANNELS * QMF_FILTER_STATE_ANA_SIZE];

// /*  Filter states for QMF-synthesis.  <br>
//   Dimension: #MAXNRQMFCHANNELS * #QMF_FILTER_STATE_SYN_SIZE */
// float sbr_QmfStatesSynthesis[MAXNRQMFCHANNELS * QMF_FILTER_STATE_SYN_SIZE];

// /*  Gains (matissas) of last envelope, needed for smoothing.  <br>
//   Dimension: #MAXNRSBRCHANNELS x #MAX_FREQ_COEFFS */
// float sbr_GainSmooth[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];

// /*  Noise levels of last envelope, needed for smoothing.  <br>
//   Dimension: #MAXNRSBRCHANNELS x #MAX_FREQ_COEFFS */
// float sbr_NoiseSmooth[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];

// /*  Frequency tables derived from the SBR-header */
// FREQ_BAND_DATA FreqBandData;
// 
// /*  Delayed spectral data needed for the dynamic framing of SBR */
// #ifdef LP_SBR_ONLY
// float sbr_OverlapBuffer[MAXNRQMFCHANNELS][MAX_OV_COLS * NO_SYNTHESIS_CHANNELS];
// #else
// float sbr_OverlapBuffer[MAXNRQMFCHANNELS][2 * MAX_OV_COLS * NO_SYNTHESIS_CHANNELS];
// #endif


// /*  Filter states (real part) for LPC used in the LPP-transposer */
// float sbr_LpcFilterStatesReal[MAXNRQMFCHANNELS][LPC_ORDER][NO_ANALYSIS_CHANNELS];

// #ifndef LP_SBR_ONLY
// /*  Filter states (imaginary part) for LPC used in the LPP-transposer */
// float sbr_LpcFilterStatesImag[MAXNRQMFCHANNELS][LPC_ORDER][NO_ANALYSIS_CHANNELS];
// #endif

// /*  Transposer settings derived from the SBR-header */
// TRANSPOSER_SETTINGS sbr_TransposerSettings;

// /*  Some data from the previous frame needed for delta-coding over time
//   and for concealment. */
// SBR_PREV_FRAME_DATA PrevFrameData[MAXNRSBRCHANNELS];

// /*  Bitstream data for the previous frame needed for concealment. */
// SBRBITSTREAM sbr_PrevBitstream;

/* @} */

/*
  \name DynamicSbrData

  Dynamic memory areas, might be reused in other algorithm sections,
  e.g. the core decoder
*/
/* @{ */

#ifdef LP_SBR_ONLY
/*  This buffer stores half of the reconstructed left time data signal
  until the right channel is completely finished */
float *InterimResult = WorkBufferCore;
#else
/*  The work buffer #WorkBufferCore of the aac-core (see aac_ram.cpp)
  will be reused as #WorkBuffer2 in the SBR part. Minimum size of
  #WorkBufferCore must be #MAX_COLS * #NO_SYNTHESIS_CHANNELS.
  #WorkBuffer2 is the second half of the SBR work buffer. */
//float *WorkBuffer2 = WorkBufferCore;
extern float* getWorkBufferCoreArray(unsigned int flag,int pos);

/*  This buffer stores half of the reconstructed left time data signal
  until the right channel is completely finished */
//float InterimResult[MAX_FRAME_SIZE];
/* @} */
#endif /* LP_SBR_ONLY */

////////////自定义修改////////////////////////////////
class CSbrDataInfoStore
{
public:
	CSbrDataInfoStore(){sbr_PrevBitstream = new SBRBITSTREAM();}
	~CSbrDataInfoStore(){delete sbr_PrevBitstream;}
	

	/*  Filter states for QMF-analysis. <br>
	Dimension: #MAXNRQMFCHANNELS * #QMF_FILTER_STATE_ANA_SIZE */
	float sbr_CodecQmfStatesAnalysis[MAXNRQMFCHANNELS * QMF_FILTER_STATE_ANA_SIZE];

	/*  Filter states for QMF-synthesis.  <br>
	Dimension: #MAXNRQMFCHANNELS * #QMF_FILTER_STATE_SYN_SIZE */
	float sbr_QmfStatesSynthesis[MAXNRQMFCHANNELS * QMF_FILTER_STATE_SYN_SIZE];

	/*  Gains (matissas) of last envelope, needed for smoothing.  <br>
	Dimension: #MAXNRSBRCHANNELS x #MAX_FREQ_COEFFS */
	float sbr_GainSmooth[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];

	/*  Noise levels of last envelope, needed for smoothing.  <br>
	Dimension: #MAXNRSBRCHANNELS x #MAX_FREQ_COEFFS */
	float sbr_NoiseSmooth[MAXNRSBRCHANNELS][MAX_FREQ_COEFFS];

	/*  Frequency tables derived from the SBR-header */
	FREQ_BAND_DATA FreqBandData;

	/*  Delayed spectral data needed for the dynamic framing of SBR */
#ifdef LP_SBR_ONLY
	float sbr_OverlapBuffer[MAXNRQMFCHANNELS][MAX_OV_COLS * NO_SYNTHESIS_CHANNELS];
#else
	float sbr_OverlapBuffer[MAXNRQMFCHANNELS][2 * MAX_OV_COLS * NO_SYNTHESIS_CHANNELS];
#endif

	/*  Filter states (real part) for LPC used in the LPP-transposer */
	float sbr_LpcFilterStatesReal[MAXNRQMFCHANNELS][LPC_ORDER][NO_ANALYSIS_CHANNELS];

#ifndef LP_SBR_ONLY
	/*  Filter states (imaginary part) for LPC used in the LPP-transposer */
	float sbr_LpcFilterStatesImag[MAXNRQMFCHANNELS][LPC_ORDER][NO_ANALYSIS_CHANNELS];
#endif

	/*  Transposer settings derived from the SBR-header */
	TRANSPOSER_SETTINGS sbr_TransposerSettings;

	/*  Some data from the previous frame needed for delta-coding over time
	and for concealment. */
	SBR_PREV_FRAME_DATA PrevFrameData[MAXNRSBRCHANNELS];

	/*  Bitstream data for the previous frame needed for concealment. */
	SBRBITSTREAM* sbr_PrevBitstream;

	float InterimResult[MAX_FRAME_SIZE];
public:
	void ResetSbrOverlapBuffer()
	{
		memset(sbr_OverlapBuffer,0,sizeof(sbr_OverlapBuffer)/sizeof(float));
	}

	int getSbrOverlapBufferLength()
	{
		return sizeof(sbr_OverlapBuffer)/sizeof(float);
	}
};

static map<unsigned int,CSbrDataInfoStore*> g_mapSbrDataInfoStore;
typedef map<unsigned int,CSbrDataInfoStore*>::iterator SbrDataInfoStoreIterator;
LibMutex g_mapSbrDataInfoStoreMutex;

void ReleaseMapSbrDataInfoStore(unsigned int flag)
{
	LibMutexLocker locker(&g_mapSbrDataInfoStoreMutex);
	SbrDataInfoStoreIterator iter = g_mapSbrDataInfoStore.find(flag);
	if (iter!=g_mapSbrDataInfoStore.end())
	{
		delete iter->second;
		g_mapSbrDataInfoStore.erase(iter);
	}
}

CSbrDataInfoStore* getSbrDataInfoStore(unsigned int flag)
{
	SbrDataInfoStoreIterator iter = g_mapSbrDataInfoStore.find(flag);
	if (iter!=g_mapSbrDataInfoStore.end())
	{
		return iter->second;
	}
	LibMutexLocker locker(&g_mapSbrDataInfoStoreMutex);
	CSbrDataInfoStore* pSbrDataInfoStore = new CSbrDataInfoStore();
	g_mapSbrDataInfoStore.insert(pair<unsigned int,CSbrDataInfoStore*>(flag,pSbrDataInfoStore));
	return pSbrDataInfoStore;
}

float* getSbrCodecQmfStatesAnalysis(unsigned int flag,int index)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_CodecQmfStatesAnalysis[index];
}

float* getSbrQmfStatesSynthesis(unsigned int flag,int index)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_QmfStatesSynthesis[index];
}

float* getSbrGainSmooth(unsigned int flag,int channel)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->sbr_GainSmooth[channel];
}

float* getSbrGainSmooth(unsigned int flag,int channel,int coeffs)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_GainSmooth[channel][coeffs];
}

float* getSbrNoiseSmooth(unsigned int flag,int channel)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->sbr_NoiseSmooth[channel];
}

float* getSbrNoiseSmooth(unsigned int flag,int channel,int coeffs)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_NoiseSmooth[channel][coeffs];
}


FREQ_BAND_DATA* getFreqBandData(unsigned int flag)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->FreqBandData;
}

float* getSbrOverlapBuffer(unsigned int flag,int one,int two)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &(pSbrDataInfo->sbr_OverlapBuffer[one][two]);
}

float* getSbrOverlapBuffer(unsigned int flag,int one)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->sbr_OverlapBuffer[one];
}

void ResetSbrOverlapBuffer(unsigned int flag)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->ResetSbrOverlapBuffer();
}

int getSbrOverlapBufferLength(unsigned int flag)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->getSbrOverlapBufferLength();
}

float* getSbrLpcFilterStatesReal(unsigned int flag,int one,int two)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->sbr_LpcFilterStatesReal[one][two];
}

float* getSbrLpcFilterStatesReal(unsigned int flag,int one,int two,int three)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_LpcFilterStatesReal[one][two][three];
}

float* getSbrLpcFilterStatesImag(unsigned int flag,int one,int two)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->sbr_LpcFilterStatesImag[one][two];
}

float* getSbrLpcFilterStatesImag(unsigned int flag,int one,int two,int three)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_LpcFilterStatesImag[one][two][three];
}

TRANSPOSER_SETTINGS* getSbrTransposerSettings(unsigned int flag)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &pSbrDataInfo->sbr_TransposerSettings;
}

SBR_PREV_FRAME_DATA* getPrevFrameData(unsigned int flag,int channel)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &(pSbrDataInfo->PrevFrameData[channel]);
}

SBRBITSTREAM* getSbrPrevBitstream(unsigned int flag)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return pSbrDataInfo->sbr_PrevBitstream;
}

float* getInterimResultSbr(unsigned int flag,int pos)
{
	CSbrDataInfoStore* pSbrDataInfo = getSbrDataInfoStore(flag);
	return &(pSbrDataInfo->InterimResult[pos]);
}





