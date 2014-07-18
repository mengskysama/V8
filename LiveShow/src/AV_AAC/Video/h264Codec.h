#pragma once
#include "AvDefine.h"
#include "VideoRateControl.h"
#include "AVPacketMacros.h"
#include "h264Cache.h"

extern "C"
{
	#include "extras/stdint.h"
	#include "x264.h"						//x264encoder
	#include "libavcodec/avcodec.h"			//ffmpeg h264 decoder
	#include "libswscale/swscale.h"
};

#define SCORE_CALC_LEN 49

class h264Codec
{
public:
	h264Codec();
	~h264Codec(void);

	bool		   Init(bool bDecode,int iWidth = 320, int iHeight = 240,int iQP = 26,enuVIDEO_TYPE enType=VIDEO_PRIV_MIC,enuROOM_AV_VIDEO_MODE eMode = ROOM_VIDEO_CRF_MODE);
	bool           SetMode(enuROOM_AV_VIDEO_MODE eMode = ROOM_VIDEO_CRF_MODE);
	bool		   SetType(enuVIDEO_TYPE enType);

	//encoder
	bool		   COConnect();
	void		   CORelease();
	bool		   EncodeVideoData(unsigned char* pInData, int iLen,unsigned char* pOutBuf,int* iOutLen,unsigned char *cKeyFrame,bool& bStop);
	void		   ResetQP(int iQPIndex);

	//decoder
	bool		   DeConnect();
	void		   DeRelease();
	bool	       DecodeVideoData(unsigned char*pInData,int iLen,unsigned char* pOutBuf,int &iOutLen,int& err,bool bForceKey=false);
	bool		   DeChangeInit(int iWidth = 320, int iHeight = 240);

	unsigned int	GetWidth() { return   m_nWidth;}
	unsigned int	GetHeight(){ return  m_nHeight;}
	unsigned char*  GetBuffer(){ return  m_pBuf; }
	unsigned int    GetEncodeType(){ return m_encodeType; }
	enuVIDEO_TYPE	GetUsedType(){return m_enUsedType;}

	void	SetKeyFrame(x264_picture_t* p_en_picture);
	void    SetKeyFrame(x264_picture_t* p_en_picture,int index);

	void	SetWaittingForKeyFrame(bool bTrue)
	{
		m_bIsWaittingForKeyFrame = bTrue;
	}

	bool	IsWaittingForKeyFrame()
	{
		return m_bIsWaittingForKeyFrame;
	}

	unsigned int  m_iCurSeqNum ;    //当前丢给解码器的序列号(给外部使用)
	unsigned int  m_iCurKeySeqNum;  //当前关键桢的序列号(给外部使用)
	unsigned int  m_iQPIndex;
private:
	bool		   SetTypeSize(enuVIDEO_TYPE enType);
	bool		   RestEncodeSWS();
	bool		   RestDecodeSWS();
	bool		   ResetSize();

	unsigned int	m_encodeType;
	unsigned char*	m_pBuf;//(给外部使用)

	volatile unsigned int m_nWidth;
	volatile unsigned int m_nHeight;

	bool			m_bIsWaittingForKeyFrame;//是否正在等待关键帧(多线程?)

	bool			m_bEncode;
	volatile bool	m_bIsChange;
	SwsContext*		m_swsContext;
	
	/******************encoder*******************************/
	x264_picture_t	m_en_picture;
	x264_t*			m_en_h;
	x264_param_t	m_en_param;	
	unsigned int	m_en_nQP;		//压缩质量	
	
	/******************decoder******************************/
	AVFrame*		m_de_frame;
	AVCodec*		m_de_codec;
	AVCodecContext*	m_de_context;	
	AVPacket		m_avpkt;
	AVPicture		m_outpic;
	
	typedef  struct  
	{
		unsigned char   *YY;
		unsigned char   *UU;
		unsigned char   *VV;
	} YUV_struct;
	/********************************************************/
	unsigned int m_h264_codec_width; //压缩视频宽度
	unsigned int m_h264_codec_height;//压缩视频高度
	enuROOM_AV_VIDEO_MODE m_eh264_codec_mode;  //视频模式

	/*******************帧控制*********************************/
	unsigned int m_keyFrame_Count;
	bool m_last_p;
	bool m_is_stop;
	
	unsigned int m_iframe;
	unsigned int m_iframeEx;
	char m_cframe[49+7]; //重置编码器时 7帧缓冲数据
	int  m_score[SCORE_CALC_LEN];
	bool m_bCacheStart;
	float  m_flastRate;
	
	int m_qp_index;
	int m_qp_index_old;
	DWORD dwStart;

	CVideoDataCacheList m_cache;
	
	//动态码流调整
	std::vector<float> m_vecFirst;
	std::vector<float> m_vecSecond;
	DWORD m_dwFirstMapStartTime;
	DWORD m_dwSecondMapStartTime;
	float m_fAverageRate;

	enuVIDEO_TYPE m_enNewType; //新类型 切换麦时改变
	enuVIDEO_TYPE m_enUsedType;//当前编码器使用的类型
};
