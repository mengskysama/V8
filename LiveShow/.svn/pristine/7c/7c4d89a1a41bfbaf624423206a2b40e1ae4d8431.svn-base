#include "stdafx.h"
#include "Audio.h"
#include "AudioOutput.h"
#include "Global.h"
#include "MyAssert.h"
#include "AvDefine.h"

#define DOUBLE_RAND (rand()/static_cast<double>(RAND_MAX))

LoopUser LoopUser::lpLoopy;

#define RESOLVE(var) { * reinterpret_cast<void **>(&var) = static_cast<void *>(celt_##var);}

CELTCodec* CELTCodec::m_static_celtCodec = NULL;
RCPtr<CELTCodec> CELTCodec::m_static_celtptr(NULL);

CELTCodec* CELTCodec::instance()
{
	if(m_static_celtCodec == NULL)
	{
		m_static_celtCodec = new CELTCodec();
		RCPtr<CELTCodec> ptr(m_static_celtCodec);
		m_static_celtptr = ptr;
	}
	return m_static_celtCodec;
}

void CELTCodec::deleteInstance()
{
	if(m_static_celtCodec != NULL)
	{
		//delete m_static_celtCodec;
		m_static_celtCodec = NULL;
		RCPtr<CELTCodec> ptr(NULL);
		m_static_celtptr = ptr;
	}
}

CELTCodec::CELTCodec() {
	cmMode = NULL;
	RESOLVE(mode_create);
	RESOLVE(mode_destroy);
	RESOLVE(mode_info);

	RESOLVE(encoder_destroy);
	RESOLVE(encode_float);
	RESOLVE(encode);
	RESOLVE(encoder_ctl);

	RESOLVE(decoder_destroy);
	RESOLVE(decode_float);
	RESOLVE(decode);
	RESOLVE(decoder_ctl);

	RESOLVE(mode_create);
	RESOLVE(strerror);

	int error=0;
	cmMode = mode_create(SAMPLE_RATE, SAMPLE_RATE / SAMPLE_DIV, &error);	
	if (!cmMode)
	{
		Assert(0);
		Trace("error:celt_mode_create:%s\n",strerror(error));
	}
}

CELTCodec::~CELTCodec() {
	if (cmMode)
		mode_destroy(const_cast<CELTMode *>(cmMode));
}


int CELTCodec::bitstreamVersion() const {
	int v = INT_MIN;

	if (cmMode)
		mode_info(cmMode, CELT_GET_BITSTREAM_VERSION, reinterpret_cast<celt_int32 *>(&v));
	return v;
}


CELTEncoder *CELTCodec::encoderCreate() {
	int error=0;
	CELTEncoder * p = celt_encoder_create_custom(cmMode, AUDIO_CHANNEL, &error);
	if (!p)
	{
		Assert(0);
		Trace("error:celt_encoder_create_custom:%s",strerror(error));
	}
	return p;
}

CELTDecoder *CELTCodec::decoderCreate() {
	int error=0;
	CELTDecoder* p= celt_decoder_create_custom(cmMode, AUDIO_CHANNEL, &error);
	if (!p)
	{
		Assert(0);
		Trace("error:celt_decoder_create_custom:%s",strerror(error));
	}
	return p;
}

LoopUser::LoopUser() {
	qsName = wstring(L"Loopy");
	uiSession = 0;
}

//加入到音频播放队列
void LoopUser::addFrame(const QByteArray &packet) {
	if (DOUBLE_RAND < g_struct.s.dPacketLoss) {
		Trace("Drop");
		return;
	}

	bool restart = (qtLastFetch.elapsed()*CLOCKS_PER_SEC > 100);

	{
		MutexLocker l(&qmLock);

		double time = qtTicker.elapsed();

		double r;
		if (restart)
			r = 0.0;
		else
			r = DOUBLE_RAND * g_struct.s.dMaxPacketDelay;

		qmPackets.insert(make_pair(static_cast<float>(time + r), packet));

	}

	// Restart check
	if (qtLastFetch.elapsed()*CLOCKS_PER_SEC > 100) {
		AudioOutputPtr ao = g_struct.ao;
		if (ao) {
			MessageHandler::UDPMessageType msgType = MessageHandler::UDPVoiceAACPlus/*MessageHandler::UDPVoiceCELT*/;//static_cast<MessageHandler::UDPMessageType>((packet.at(0) >> 5) & 0x7);
			for (int i=0;i<PER_FRAME_OF_SAMEPLE;i++)
			{
				ao->addFrameToBuffer(this, QByteArray(), 0, msgType);
				char buf[512] = {0};
				sprintf(buf,"LoopUser::addFrame QByteArray()  t=%d\n",qtLastFetch.elapsed()*CLOCKS_PER_SEC );
				//OutputDebugStringA(buf);
			}
		}
	}

}

//从音频队列中取帧
void LoopUser::fetchFrames() {
	MutexLocker l(&qmLock);

	AudioOutputPtr ao = g_struct.ao;
	if (!ao || qmPackets.empty()) {
		return;
	}

	double cmp = qtTicker.elapsed();

	std::multimap<float, QByteArray>::iterator i = qmPackets.begin();

	while (i != qmPackets.end()) {
// 		if (i->first > cmp)
// 		{
// 			char buf[512] = {0};
// 			sprintf(buf,"LoopUser::fetchFrames() cmp=%f\n",i->first );
// 			OutputDebugStringA(buf);
// 			break;
// 		}
			

		const QByteArray &data = i->second;
		PacketDataStream pds((char*)&data[0], data.size());

		unsigned int msgFlags = 0;
		int iSeq = static_cast<unsigned int>(pds.next8());

		QByteArray qba;
		pds.dataBlock(pds.left(), qba);

		ao->addFrameToBuffer(this, qba, iSeq, MessageHandler::UDPVoiceAACPlus/*MessageHandler::UDPVoiceCELT*/);
		i = qmPackets.erase(i);
	}

	qtLastFetch.restart();
}
