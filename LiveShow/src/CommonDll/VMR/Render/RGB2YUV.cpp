#include "stdafx.h"
#include "RGB2YUV.h"

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")

CRGB2YUV::CRGB2YUV()
{
	m_swsContext = NULL;
	m_bInit = false;
	m_iWidth = 0;
	m_iHeight = 0;
	frame_src = avcodec_alloc_frame();
	frame_dst = avcodec_alloc_frame();
};

CRGB2YUV::~CRGB2YUV()
{
	if (m_swsContext) 
		sws_freeContext(m_swsContext);
	if (frame_src)
		av_free(frame_src);
	if (frame_dst)
		av_free(frame_dst);
};


uint8_t * CRGB2YUV::f2Rgb(uint8_t *src, enum PixelFormat fmt, int width, int height, int *len, bool reverse)
{
	AVFrame *frame_src = avcodec_alloc_frame();
	AVFrame *frame_dst = avcodec_alloc_frame();
	enum PixelFormat bmp_fmt = PIX_FMT_BGR24;

	int bytes = avpicture_get_size(bmp_fmt, width, height);
	*len = bytes + sizeof(BMP_FILE_HDR) + sizeof(BMP_INFO_HDR);
	uint8_t *dst_data = (uint8_t *)malloc(*len);

	memset(dst_data, 0x0, *len);
	uint8_t *img_data = dst_data + sizeof(BMP_FILE_HDR) + sizeof(BMP_INFO_HDR);

	bool is_cvt = false;
	if (fmt == bmp_fmt) {
		memcpy(img_data, src, bytes);
		is_cvt = true;
	}else {
		avpicture_fill((AVPicture *)frame_dst, img_data, bmp_fmt, width, height);
		avpicture_fill((AVPicture *)frame_src, src, fmt, width, height);
		struct SwsContext *sws = 
			sws_getContext(width, height, fmt,
			width, height, bmp_fmt, 
			SWS_FAST_BILINEAR, NULL, NULL, NULL);
		if (sws) {
			int h = sws_scale(sws, (const uint8_t** const)frame_src->data, frame_src->linesize, 
				0, height, frame_dst->data, frame_dst->linesize);
			if (h == height) {
				is_cvt = true;
			}
			sws_freeContext(sws);
		}
		av_free(frame_src);
		av_free(frame_dst);
	}

	if (!is_cvt) {
		free(dst_data);
		return NULL;
	}

	if (reverse) {
		int line_bytes = bytes / height;
		uint8_t *buf_tmp = (uint8_t *)malloc(line_bytes);
		for (int j = 0; j < height / 2; j++) {
			memcpy(buf_tmp, img_data + line_bytes * j, line_bytes);
			memcpy(img_data + line_bytes * j, img_data + line_bytes * 
				(height - j - 1), line_bytes);
			memcpy(img_data + line_bytes * (height - j - 1), buf_tmp, line_bytes);
		}
		free(buf_tmp);
	}

	BMP_FILE_HDR *fh = (BMP_FILE_HDR *)dst_data;
	BMP_INFO_HDR *bh = (BMP_INFO_HDR *)(dst_data + sizeof(BMP_FILE_HDR));
	fh->bfType = 0x4D42;
	fh->bfSize = sizeof(BMP_FILE_HDR) + sizeof(BMP_INFO_HDR) + bytes;
	fh->bfOffBits = sizeof(BMP_FILE_HDR) + sizeof(BMP_INFO_HDR);
	bh->biSize = sizeof(BMP_INFO_HDR);
	bh->biWidth = width;
	bh->biHeight = height;
	bh->biPlanes = 1;
	bh->biBitCount = 24;
	bh->biCompression = 0;
	bh->biSizeImage = 0;
	bh->biXPelsPerMeter = 0;
	bh->biYPelsPerMeter = 0;
	bh->biClrUsed = 0;
	bh->biClrImportant = 0;
	return dst_data;
};