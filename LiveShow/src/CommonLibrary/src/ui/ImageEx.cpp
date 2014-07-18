// GDIPlusHelper.cpp: implementation of the CGDIPlusHelper class. 
// 
////////////////////////////////////////////////////////////////////// 

#include "stdafx.h"
#include "ui/ImageEx.h" 
#include "assert.h"

namespace common 
{
	namespace ui
	{
		int gIndex = 0;
			ImageEx::ImageEx(LPCTSTR filename, BOOL useEmbeddedColorManagement) 
			: Image(filename, useEmbeddedColorManagement), m_PauseTime(70)
		{   
			Initialize();
			TestForAnimatedGIF();
			m_bPause = false;
		}    

		ImageEx::~ImageEx() 
		{ 
			Destroy(); 

			for (std::vector<Image*>::iterator it = m_vecImage.begin(); it != m_vecImage.end(); ++it)
			{
				if ((*it) != NULL )
				{
					delete *it;
					*it = NULL;
				}
			}
			m_vecImage.clear();
		} 

		CSize ImageEx::GetSize() 
		{ 
			return CSize(GetWidth(), GetHeight()); 
		}

		bool ImageEx::TestForAnimatedGIF() 
		{ 
			UINT count = 0; 
			count = GetFrameDimensionsCount(); //获得维数
			GUID* pDimensionIDs = new GUID[count]; 

			// Get the list of frame dimensions from the Image object. 
			GetFrameDimensionsList(pDimensionIDs, count); //获得Dimension的GUID值

			// Get the number of frames in the first dimension. 
			m_nFrameCount = GetFrameCount(&pDimensionIDs[0]); //获得帧数

			// Assume that the image has a property item of type PropertyItemEquipMake. 
			// Get the size of that property item. 
			int nSize = GetPropertyItemSize(PropertyTagFrameDelay); //获得需要的缓冲区大小

			// Allocate a buffer to receive the property item. 
			m_pPropertyItem = NULL;
			m_pPropertyItem = (PropertyItem*) malloc(nSize); 

			GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem); //获得图像属性

			for(int i=0;m_nFrameCount>1&&i<m_nFrameCount;i++)
			{
				if(((long*) m_pPropertyItem->value)[i] == 0)
				{
					//assert(0 && L"亲，gif资源不对，找许晔修改");
				}
			}

			GUID pageGuid = FrameDimensionTime; 
			if (m_nFrameCount > 1)
			{
				for (int i = 0; i < m_nFrameCount; i++)
				{
					Image * pImage = Clone();
					pImage->SelectActiveFrame(&pageGuid, i);
					m_vecImage.push_back(pImage);
				}
			}

			delete[]  pDimensionIDs; 

			return m_nFrameCount > 1; 
		} 

		void ImageEx::Initialize() 
		{ 
			m_nFramePosition = 0; 
			m_nFrameCount = 0;
			lastResult = InvalidParameter;
			m_pPropertyItem = NULL; 
			m_nFramePosition = 0; 
			m_bPause = true;
			m_Index = gIndex++;
		} 

		bool ImageEx::DrawFrameGIF() 
		{ 	
			if (m_bPause) return false;
			if (!m_PauseTime.isReady(GetTickCount())) return false;

			//GUID pageGuid = FrameDimensionTime; 

			//Status st = SelectActiveFrame(&pageGuid, m_nFramePosition++);	
			m_nFramePosition++;

			if (m_nFramePosition >= m_nFrameCount) 
				m_nFramePosition = 0; 

			long lPause = 0;
			if (m_pPropertyItem)
			{
				lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
			}

			m_PauseTime.SetUpdatePeriod(lPause);
			return true; 
		} 

		void ImageEx::SetPause(bool bPause) 
		{ 
			if (!IsAnimatedGIF()) 
				return; 

			m_bPause = bPause; 
		} 

		void ImageEx::Destroy() 
		{
			if(m_pPropertyItem != NULL)
			{
				free(m_pPropertyItem); 
				m_pPropertyItem = NULL;
			}	
		}

		UINT ImageEx::GetFrameCounts()
		{
			return m_nFrameCount;
		}

		bool ImageEx::DrawNextFrame()
		{
			GUID pageGuid = FrameDimensionTime; 

			SelectActiveFrame(&pageGuid, m_nFramePosition++);		 

			if (m_nFramePosition >= m_nFrameCount) 
				m_nFramePosition = 0; 

			return true; 
		}

		UINT ImageEx::GetCurrentFramePos()
		{
			return m_nFramePosition;
		}

		Image* ImageEx::GetCurImage()
		{
			if (m_nFramePosition < m_vecImage.size())
			{
				return m_vecImage[m_nFramePosition];
			}

			return this;
		}
		//////////////////////////////////////////////////////////////////////////////////

	}
}