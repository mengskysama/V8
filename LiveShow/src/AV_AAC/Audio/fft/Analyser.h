#pragma once
#include "51ktvtype.h"
namespace FFTWav
{
	//音频数据计算类
	class CAnalyserProcess;
	class CFFTrans;
	class CAnalyser
	{
	friend class CAnalyserProcess;
	public:
		CAnalyser(HWND hWnd);
		virtual ~CAnalyser(void);

	protected:
		//************数据计算部分
		int m_nSampleSize;//每帧音频数据大小  当前帧数据大小
		DWORD m_nAudioDataBufferLength;//音频数据缓冲区
		byte* m_pAudioDataBuffer;//音频缓冲区
		FLOAT* m_Left; //存放左声道数据音量百分比 - 占总音量
		FLOAT* m_Right;//存放右声道数据音量百分比
		INT m_offset; //保存偏移
		LONG m_FpsAsNS; //计算延迟
		LONG m_DesiredFpsAsNS; //计算延迟
		INT m_saFFTSampleSize;//傅里叶变换初始帧大小
		CFFTrans* m_pFFT; //傅里叶变换实现类
		FLOAT m_saDecay;//延迟系数
		INT m_width; //宽度
		INT m_height;//高度
		INT m_saBands;//波形条条数
		FLOAT m_saMultiplier;//
		FLOAT* m_oldFFT;//旧的傅里叶变换数据
		INT* m_peaks; //自由落子高度
		INT* m_peaksDelay;//自由落子延迟
		bool m_peaksEnabled; //是否显示自由落子
		INT m_peakDelay;//自由落子延迟
		bool m_bIsVistaOrWin7;

		//绘图准备部分
		HWND m_hWnd; //音频显示的窗口句柄
		HDC m_hdcScreen;//屏幕画刷
		HDC m_hdcMem;//内存波形图形DC
		HDC m_hdcMem1;//一个波形条图形 
		HBITMAP m_hbmMem;//内存波形图关联位图
		HBITMAP m_hbmMem1;//一个波形条图关联位图
		HBITMAP m_hOld;//内存波形图关联前老位图
		HBITMAP m_hOld1;//一个波形条图关联位图前的老位图
		HBRUSH m_hbrush;//背景画刷  默认黑色
		HBRUSH m_hbrush1;//自由落子 画刷 默认灰白
		HBRUSH m_hbrushOld; //选择背景画刷前的老画刷
		HBRUSH m_hbrushOld1;//选择自由落子画刷前的老画刷
		
		
	protected:
		void drawSpectrumAnalyserBar(RECT* pRect, int pX, int pY, int pWidth, int pHeight, int band);
	public:
		void Process(float pFrameRateRatioHint);//执行计算操作
		void Draw(HWND hWnd,HDC hDc);
		void TransparentBlt2( HDC hdcDest,      // 目标DC
			int nXOriginDest,   // 目标X偏移
			int nYOriginDest,   // 目标Y偏移
			int nWidthDest,     // 目标宽度
			int nHeightDest,    // 目标高度
			HDC hdcSrc,         // 源DC
			int nXOriginSrc,    // 源X起点
			int nYOriginSrc,    // 源Y起点
			int nWidthSrc,      // 源宽度
			int nHeightSrc,     // 源高度
			UINT crTransparent  // 透明色,COLORREF类型
			);
	};

}
