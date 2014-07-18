#pragma once 

#pragma warning(disable:4786)

#include <map>
#include <vector>
#include <utility>
#include <string>
#include <process.h>
#include "ui/Regulator.h"

#include "51ktvtype.h"
#include "../Common.h"

namespace common 
{
	namespace ui
	{
		using namespace Gdiplus;
		class COMMON_INTERFACE ImageEx : public Image 
		{ 
		public:
			ImageEx(LPCTSTR filename, BOOL useEmbeddedColorManagement = FALSE); 
			~ImageEx(); 

			CSize	GetSize(); 
			bool	IsAnimatedGIF() { return m_nFrameCount > 1; } 
			void	SetPause(bool bPause); 
			bool	IsPaused() { return m_bPause; }	
			void	Destroy();
			bool	DrawFrameGIF();
			bool	DrawNextFrame();
			UINT	GetFrameCounts();
			UINT	GetCurrentFramePos();	//获得当前的帧id，配合DrawFrameGIF()一起使用
			Image*  GetCurImage();

		private:

			bool	TestForAnimatedGIF(); 	 	
			void	Initialize(); //protected

			UINT			m_nFrameCount; 
			UINT			m_nFramePosition; 
			bool			m_bIsInitialized; 
			bool			m_bPause; 
			PropertyItem*	m_pPropertyItem;
			Regulator<long> m_PauseTime;
			int				m_Index;
			std::vector<Image*> m_vecImage;
		};

	}
}