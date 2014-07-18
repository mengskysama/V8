#pragma once
#include "Basic.h"
#include "51ktvtype.h"
#include "RC.h"

namespace FFTWav
{
	//绘制波形图执行类
	class CAnalyser;
	class CAnalyserProcess:public RCObject
	{
	public:
		CAnalyserProcess(HWND hWnd);
		~CAnalyserProcess(void);
	protected:
		CAnalyser* m_Analyser;
		CCriticalSection cs;
		bool bNew;
		unsigned int iCount;
	protected:
		void processSamples(int nPosition);

	public:
		//执行操作
		void Process();
		//为执行Process前准备数据 pBuf音频数据 音频数据长度byte
		void Prepare(byte* pBuf,int nLen);

		void Draw(HWND hWnd,HDC hDc);
	};

}
