#pragma once

extern "C"
{
#include "extras/stdint.h"
#include "libavcodec/avcodec.h"			//ffmpeg h264 decoder
#include "libswscale/swscale.h"
};

#pragma pack (push ,1)
typedef struct { /* bmfh */
	uint16_t bfType;  
	uint32_t bfSize; 
	uint16_t bfReserved1; 
	uint16_t bfReserved2; 
	uint32_t bfOffBits;
} BMP_FILE_HDR;

typedef struct { /* bmih */
	uint32_t biSize; 
	uint32_t biWidth; 
	uint32_t biHeight; 
	uint16_t biPlanes; 
	uint16_t biBitCount; 
	uint32_t biCompression; 
	uint32_t biSizeImage; 
	uint32_t biXPelsPerMeter; 
	uint32_t biYPelsPerMeter; 
	uint32_t biClrUsed; 
	uint32_t biClrImportant;
} BMP_INFO_HDR;
#pragma pack (pop)

class CRGB2YUV
{
public:
	CRGB2YUV();
	~CRGB2YUV();

	bool Rgb2Yuv(unsigned int width,unsigned int height,unsigned char* src,unsigned char* dst,bool reverse=true)
	{
		if (!m_bInit || !m_swsContext)
		{
			m_swsContext = sws_getContext(width,height,PIX_FMT_RGB24,width,height,PIX_FMT_YUV420P,SWS_SINC,NULL,NULL,NULL);
			m_bInit = true;
			m_iWidth = width;
			m_iHeight = height;
		}
		if(m_iWidth!=width || m_iHeight!=height)
		{
			if (m_swsContext) 
				sws_freeContext(m_swsContext);
			m_swsContext = sws_getContext(width,height,PIX_FMT_RGB24,width,height,PIX_FMT_YUV420P,SWS_SINC,NULL,NULL,NULL);
			m_iWidth = width;
			m_iHeight = height;
		}

		if (!frame_dst || !frame_src || !m_swsContext) return false;

		avpicture_fill((AVPicture *)frame_dst, dst, PIX_FMT_YUV420P, m_iWidth, m_iHeight);
		avpicture_fill((AVPicture *)frame_src, src, PIX_FMT_RGB24, m_iWidth, m_iHeight);
		try
		{
			if (reverse)
			{
				frame_src->data[0]  += frame_src->linesize[0] * (height - 1);   
				frame_src->linesize[0] *= -1;                      
				frame_src->data[1]  += frame_src->linesize[1] * (height / 2 - 1);   
				frame_src->linesize[1] *= -1;   
				frame_src->data[2]  += frame_src->linesize[2] * (height / 2 - 1);   
				frame_src->linesize[2] *= -1;   
			}

			int h = sws_scale(m_swsContext, (const uint8_t** const)frame_src->data, frame_src->linesize,0, height, frame_dst->data, frame_dst->linesize);

			if (h != m_iHeight) {
				return false;
			}
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	inline uint8_t *	f2Rgb(uint8_t *src, enum PixelFormat fmt, int width, int height, int *len, bool reverse);
protected:
	SwsContext* m_swsContext;
	bool m_bInit;
	int m_iWidth;
	int m_iHeight;
	AVFrame *frame_src;
	AVFrame *frame_dst;
};