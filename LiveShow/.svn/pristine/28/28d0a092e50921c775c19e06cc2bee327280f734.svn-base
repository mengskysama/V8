#include "stdafx.h"
#include "Audio.h"
#include "AudioOutput.h"
#include "Global.h"
#include "MyAssert.h"

#define DOUBLE_RAND (rand()/static_cast<double>(RAND_MAX))

LoopUser LoopUser::lpLoopy;

#define RESOLVE(var) { * reinterpret_cast<void **>(&var) = static_cast<void *>(celt_##var);}

CELTCodec* CELTCodec::m_static_celtCodec = NULL;

CELTCodec* CELTCodec::instance()
{
	if(m_static_celtCodec == NULL)
	{
		m_static_celtCodec = new CELTCodec();
	}
	return m_static_celtCodec;
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
	cmMode = mode_create(SAMPLE_RATE, SAMPLE_RATE / 50, &error);	
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
	CELTEncoder * p = celt_encoder_create_custom(cmMode, 2, &error);
	return p;
}

CELTDecoder *CELTCodec::decoderCreate() {
	return celt_decoder_create_custom(cmMode, 2, NULL);
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
			MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((packet.at(0) >> 5) & 0x7);
			ao->addFrameToBuffer(this, QByteArray(), 0, msgType);
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
		if (i->first > cmp)
			break;

		int iSeq;
		const QByteArray &data = i->second;
		PacketDataStream pds((char*)&data[0], data.size());

		unsigned int msgFlags = static_cast<unsigned int>(pds.next());

		pds >> iSeq;

		QByteArray qba;
		qba.reserve(pds.left() + 1);
		qba.push_back(static_cast<char>(msgFlags));
		QByteArray qbatmp = pds.dataBlock(pds.left());
		for(int i=0; i<qbatmp.size(); i++)
		{
			qba.push_back(qbatmp[i]);
		}

		MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((msgFlags >> 5) & 0x7);

		ao->addFrameToBuffer(this, qba, iSeq, msgType);
		i = qmPackets.erase(i);
	}

	qtLastFetch.restart();
}
