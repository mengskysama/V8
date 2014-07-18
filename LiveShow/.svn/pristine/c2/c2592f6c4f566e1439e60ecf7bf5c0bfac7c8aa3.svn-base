#pragma once
#pragma warning(disable:4244)

namespace FFTWav
{
	//全局数据参数
	#define DEFAULT_SAMPLE_SIZE 320   //原始960，只保留1/3参与波形运算(浮点运算太耗时)
	#define DEFAULT_BUFFER_SIZE 48000
	#define DEFAULT_FPS 10  //每秒10帧
	#define DEFAULT_SAMPLE_BIT  4  //每个样本字节数
	#define DEFAULT_SPECTRUM_ANALYSER_FFT_SAMPLE_SIZE 320 //傅里叶变换初始帧大小 原始960，只保留1/3参与波形运算(浮点运算太耗时)
	#define DEFAULT_SPECTRUM_ANALYSER_DECAY 0.25F //波形条变化系数
	#define DEFAULT_SPECTRUM_ANALYSER_BAND_COUNT 16 //波形条条数
	#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY 14 //自由落子延迟
	#define DEFAULT_WIDTH  16*2+6 //30*1.4-2  //波形图形显示宽度
	#define DEFAULT_HEIGHT 18*1.2  //波形图形显示高度

	/************************************************************************/
	/* CSystem                                                              */
	/************************************************************************/
	typedef __int64				jlong;
	typedef unsigned int		juint;
	typedef unsigned __int64	julong;
	typedef long				jint;
	typedef signed char			jbyte;

	#define CONST64(x)				(x ## LL)
	#define NANOS_PER_SEC			CONST64(1000000000)
	#define NANOS_PER_MILLISEC		1000000

	jlong as_long(LARGE_INTEGER x);
	void set_high(jlong* value, jint high);
	void set_low(jlong* value, jint low);

	class CSystem
	{
	private:
		static jlong frequency;
		static int ready;

		static void init()
		{
			LARGE_INTEGER liFrequency = {0};
			QueryPerformanceFrequency(&liFrequency);
			frequency = as_long(liFrequency);
			ready = 1;
		}
	public:
		static jlong nanoTime()
		{
			if(ready != 1)
				init();

			LARGE_INTEGER liCounter = {0};
			QueryPerformanceCounter(&liCounter);
			double current = as_long(liCounter);
			double freq = frequency;
			return (jlong)((current / freq) * NANOS_PER_SEC);
		}
	};

	/************************************************************************/
	/* CCriticalSection                                                     */
	/************************************************************************/
	class CCriticalSection
	{
	protected:
		CRITICAL_SECTION m_cs;
	public:
		CCriticalSection() { InitializeCriticalSection(&m_cs); }
		~CCriticalSection() { DeleteCriticalSection(&m_cs); }

		void Enter() { EnterCriticalSection(&m_cs); }
		void Leave() { LeaveCriticalSection(&m_cs); }
	};


	BOOL DrawGradient(HDC hdc, CONST RECT* pRect, CONST DWORD* cl, int Num, DWORD dwMode);
}

