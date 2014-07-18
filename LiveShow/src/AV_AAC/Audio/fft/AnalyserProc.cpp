#include "stdafx.h"
#include "AnalyserProc.h"
#include "Basic.h"
#include "Analyser.h"

namespace FFTWav
{
	CAnalyserProcess::CAnalyserProcess(HWND hWnd)
	{
		m_Analyser = new CAnalyser(hWnd);
		m_Analyser->m_hWnd = hWnd; //初始化显示图形的句柄
		iCount = 0;
		bNew = TRUE;
	}

	CAnalyserProcess::~CAnalyserProcess(void)
	{
		if (m_Analyser)
		{
			delete m_Analyser;
		}
	}
	
	void CAnalyserProcess::processSamples(int nPosition)
	{
		int c = nPosition;//数据开始位置

		for (int a = 0; a < m_Analyser->m_nSampleSize;) {
			if ((DWORD)c >= m_Analyser->m_nAudioDataBufferLength) {//计算是否小于缓冲区长度
				m_Analyser->m_offset++;
				c -= m_Analyser->m_nAudioDataBufferLength;
			}
			//计算左右声道百分比
			short * pSample = (short*)&m_Analyser->m_pAudioDataBuffer[c];
			m_Analyser->m_Left[a] = (*pSample) / 32767.0F;
			pSample = (short*)&m_Analyser->m_pAudioDataBuffer[c+2];
			m_Analyser->m_Right[a] = (*pSample) / 32767.0F;

// 			m_Analyser->m_Left[a] = (float) ((m_Analyser->m_pAudioDataBuffer[c + 1] << 8) +
// 				m_Analyser->m_pAudioDataBuffer[c]) / 32767.0F;
// 			m_Analyser->m_Right[a] = (float) ((m_Analyser->m_pAudioDataBuffer[c + 3] << 8) +
// 				m_Analyser->m_pAudioDataBuffer[c + 2]) / 32767.0F;
			a++;
			c += 4;
		}
	}

	//执行操作
	void CAnalyserProcess::Process()
	{
		if (!bNew) return;
		jlong wStn = CSystem::nanoTime(); //当前开始时间
		int wSdp = 0;//当前音频的开始位置

		processSamples(wSdp); //开始计算音频帧数据

		for (int a = 0; a < 1; a++)
		{
			float wFrr = (float) m_Analyser->m_FpsAsNS / (float) m_Analyser->m_DesiredFpsAsNS;
			m_Analyser->Process(wFrr);
		}

		jlong wEdn = CSystem::nanoTime();//结束时间
		long wDelay = m_Analyser->m_FpsAsNS - (long)(wEdn - wStn);

		if (wDelay > 0L) 
		{
			DWORD ms = (DWORD)wDelay / 0xf4240L;
			DWORD ns = (DWORD)wDelay % 0xf4240L;
			if(ns >= 500000) ms++;			
			//Sleep(10);

			if (m_Analyser->m_FpsAsNS > m_Analyser->m_DesiredFpsAsNS)
				m_Analyser->m_FpsAsNS -= wDelay;
			else
				m_Analyser->m_FpsAsNS = m_Analyser->m_DesiredFpsAsNS;
		}
		else
		{
			m_Analyser->m_FpsAsNS += -wDelay;
			//Sleep(2);
		}
		bNew = FALSE;
	}

	//为执行Process前准备数据 pBuf音频数据 音频数据长度byte
	void CAnalyserProcess::Prepare(byte* pBuf,int nLen)
	{
		memset(m_Analyser->m_pAudioDataBuffer,0,m_Analyser->m_nAudioDataBufferLength);
		int cpLen = nLen>m_Analyser->m_nAudioDataBufferLength?m_Analyser->m_nAudioDataBufferLength:nLen;
		CopyMemory(m_Analyser->m_pAudioDataBuffer,pBuf,cpLen);
		m_Analyser->m_nSampleSize = DEFAULT_SAMPLE_SIZE;//nLen / DEFAULT_SAMPLE_BIT;
		iCount = 0;
		bNew = TRUE;
	}

	void CAnalyserProcess::Draw(HWND hWnd,HDC hDc)
	{
		if(++iCount > 5) 
		{
			bNew = TRUE;
			memset(m_Analyser->m_pAudioDataBuffer,0,m_Analyser->m_nAudioDataBufferLength);
			Process();
		}
		else if (iCount>15)
		{
			bNew = FALSE;
		}
		m_Analyser->Draw(hWnd,hDc);
	}

}
