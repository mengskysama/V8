#pragma once
namespace FFTWav
{
	#define PI_2 6.2831853071795862F
	#define PI   3.1415926535897931F
	
	//∏µ¿Ô“∂±‰ªª¿‡
	class CFFTrans
	{
	private:
		float* xre;
		float* xim;
		float* mag;
		float* fftSin;
		float* fftCos;
		int* fftBr;
		int ss;
		int ss2;
		int nu;
		int nu1;

		int BitRev(int j, int nu);
		void PrepareFFTTables();
	public:
		CFFTrans(int pSampleSize);
		~CFFTrans(void);

		float* Calculate(float* pSample, size_t pSampleSize);
	};

}
