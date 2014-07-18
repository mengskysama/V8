#include "StdAfx.h"
#include "Global.h"
#include "AudioOutputSpeech.h"
#include "../Video/crc/Crc32.h"
#include "FloatAACPlusDecodeCodec.h"
#include "FactoryImpl.h"

AudioOutputSpeech::AudioOutputSpeech(ClientUser *user, unsigned int freq, MessageHandler::UDPMessageType type)
: AudioOutputUser(user->qsName),
p(user),
umtType(type)
{
	m_cCodec = NULL;
	m_cdDecoder = NULL;

	m_pCodec = NULL;
	m_pFactory = NULL;

	if (umtType != MessageHandler::UDPVoiceCELT && umtType!=MessageHandler::UDPVoiceAACPlus) 
	{
		Assert(0);
	}
	
	if (umtType==MessageHandler::UDPVoiceAACPlus)
	{
		memset(m_pBufTemp,0,sizeof(m_pBufTemp));
	}

	m_putIndex = m_getIndex = 0;
}

AudioOutputSpeech::~AudioOutputSpeech() 
{
	if (umtType == MessageHandler::UDPVoiceCELT )
	{
		if (m_cdDecoder) 
		{
			m_cCodec->decoder_destroy(m_cdDecoder);
		}
	}
	else if (umtType == MessageHandler::UDPVoiceAACPlus)
	{
		SAFE_DELETE(m_pCodec);
	}
}

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, unsigned int iSeq) 
{
	MutexLocker lock(&m_cacheMutex);	

	if (qbaPacket.size() < 2)
	{
		return;
	}

	PacketDataStream pds(qbaPacket);

	if (umtType == MessageHandler::UDPVoiceCELT)
	{
		if (! m_cCodec) 
		{
			m_cCodec = CELTCodec::instance();
			if (m_cCodec)
			{
				m_cdDecoder = m_cCodec->decoderCreate();
			}
		}

		if (m_cdDecoder == NULL)
		{
			assert(0);
			return;
		}

		int count = 0;
		short header = 0;
		do
		{
			header = static_cast<short>(ntohs(pds.next16()));
			if (header)
			{
				m_cCodec->decode(m_cdDecoder, pds.dataPtr(), header, 
					(celt_int16 *)m_cacheSample[m_putIndex%MAX_CACHE_SAMPLE_COUNT], SAMPLE_RATE/SAMPLE_DIV);

				pds.skip(header);

				count++;
				m_putIndex++;
			}
			else
			{
				assert(0);
			}
		} while ((header) && pds.left());
	}
	else if (umtType == MessageHandler::UDPVoiceAACPlus)
	{
		if (!m_pCodec)
		{
			m_pFactory = GetFloatDecodeFactory();
			m_pCodec = m_pFactory->RegisterCodec();
			if (!m_pCodec->Create(CODEC_DECODE))
			{
				ErrTrace("m_pCodec->Create CODEC_DECODE Error!\n");
			}
		}

		if (m_pCodec == NULL)
		{
			assert(0);
			return;
		}

		short header = 0;
		do
		{
			header = ntohs(pds.next16());
			if (header)
			{
				int outlen = 0;
				m_pCodec->Decode(pds.dataPtr(),header,m_pBufTemp,outlen);
				if (!outlen) break;

				m_cache.Write(m_pBufTemp,outlen);
				
				while (m_cache.Size()>(SAMPLE_RATE/SAMPLE_DIV)*4)
				{
				   m_cache.Read((unsigned char*)m_cacheSample[m_putIndex%MAX_CACHE_SAMPLE_COUNT],(SAMPLE_RATE/SAMPLE_DIV)*4);
				   m_putIndex++;
				}

				pds.skip(header);
			}
			else
			{
				assert(0);
			}
		} while ((header) && pds.left());
	}
}

bool AudioOutputSpeech::needSamples(unsigned int snum) 
{
	MutexLocker lock(&m_cacheMutex);

	if (p == &LoopUser::lpLoopy)
		LoopUser::lpLoopy.fetchFrames();

	if(m_putIndex - m_getIndex < MIN_CACHE_SAMPLE_COUNT)
	{
		return false;
	}

	if (m_getIndex>m_putIndex)
	{
		return false;
	}

	if(m_putIndex - m_getIndex > MAX_CACHE_SAMPLE_COUNT)
	{
		m_getIndex = m_putIndex - MAX_CACHE_SAMPLE_COUNT;
	}
	
	setBuffer(m_cacheSample[m_getIndex%MAX_CACHE_SAMPLE_COUNT]);
	m_getIndex++;

	return true;
}
