#include "StdAfx.h"
#include "Global.h"
#include "AudioOutputSpeech.h"

AudioOutputSpeech::AudioOutputSpeech(ClientUser *user, unsigned int freq, MessageHandler::UDPMessageType type) : AudioOutputUser(user->qsName) {
	int err;
	p = user;
	umtType = type;

	unsigned int srate = 0;

	cCodec = NULL;
	cdDecoder = NULL;

	if (umtType == MessageHandler::UDPVoiceCELT) {
		srate = SAMPLE_RATE;
		iFrameSize = srate / 50;

		dsSpeex = NULL;
	} 
	else {
		assert(0);
	}

	iOutputSize = (iFrameSize * freq) / srate;

	resizeBuffer(iOutputSize*2);

	iBufferOffset = iBufferFilled = iLastConsume = 0;
	bLastAlive = true;

	iMissCount = 0;
	iMissedFrames = 0;

	ucFlags = 0xFF;

	jbJitter = jitter_buffer_init(iFrameSize);
	int margin = g_struct.s.iJitterBufferSize*iFrameSize;
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);

	fFadeIn = new float[iFrameSize*2];
	fFadeOut = new float[iFrameSize*2];

	float mul = static_cast<float>(M_PI / (2.0 * static_cast<double>(iFrameSize*2)));
	for (unsigned int i=0;i<iFrameSize*2;++i)
		fFadeIn[i] = fFadeOut[iFrameSize*2-i-1] = sinf(static_cast<float>(i) * mul);
}

AudioOutputSpeech::~AudioOutputSpeech() {
	if (cdDecoder) {
		cCodec->decoder_destroy(cdDecoder);
	} else if (dsSpeex) {
		speex_bits_destroy(&sbBits);
		speex_decoder_destroy(dsSpeex);
	}

	jitter_buffer_destroy(jbJitter);

	delete [] fFadeIn;
	delete [] fFadeOut;
}

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, unsigned int iSeq) {
	MutexLocker lock(&qmJitter);

	if (qbaPacket.size() < 2)
		return;

	PacketDataStream pds(qbaPacket);

	pds.next();

	int frames = 0;
	unsigned int header = 0;
	do {
		header = static_cast<unsigned char>(pds.next());
		frames++;
		pds.skip(header);
	} while (header && pds.left());

	if (pds.isValid()) {
		JitterBufferPacket jbp;
		jbp.data = (char*)(&qbaPacket[0]);//00 bc ...
		jbp.len = qbaPacket.size();//60*2+3  当前包的size
		jbp.span = iFrameSize * frames;//480*2  当前包占用的timestamp
		jbp.timestamp = iFrameSize * iSeq;//当前包开始的timestamp
		jitter_buffer_put(jbJitter, &jbp);
	}
}

bool AudioOutputSpeech::needSamples(unsigned int snum) {
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;

	iLastConsume = snum;

	if (iBufferFilled >= snum)
		return bLastAlive;

	short *pOut = NULL;

	bool nextalive = bLastAlive;

	while (iBufferFilled < snum) {

		pOut = (pfBuffer + iBufferFilled);

		if (! bLastAlive) {
			memset(pOut, 0, iFrameSize*sizeof(short)*2);
		} 
		else {
			if (p == &LoopUser::lpLoopy)
				LoopUser::lpLoopy.fetchFrames();

			int avail = 0;
			int ts = jitter_buffer_get_pointer_timestamp(jbJitter);
			jitter_buffer_ctl(jbJitter, JITTER_BUFFER_GET_AVAILABLE_COUNT, &avail);

			if (p && (ts == 0)) {
				int want = iroundf(p->fAverageAvailable);
				if (avail < want) {
					++iMissCount;
					if (iMissCount < 20) {
						memset(pOut, 0, iFrameSize*sizeof(short)*2);
						goto nextframe;
					}
				}
			}

			if (qlFrames.empty()) {
				MutexLocker lock(&qmJitter);

				char data[4096];
				JitterBufferPacket jbp;
				jbp.data = data;
				jbp.len = 4096;

				spx_int32_t startofs = 0;

				int error = 0;

				if ((error =jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs)) == JITTER_BUFFER_OK) {
					
					PacketDataStream pds(jbp.data, jbp.len);

					iMissCount = 0;
					ucFlags = static_cast<unsigned char>(pds.next());
					bHasTerminator = false;

					unsigned int header = 0;
					do {
						header = static_cast<unsigned int>(pds.next());
						if (header)
							qlFrames.push_back(pds.dataBlock(header));
						else
							bHasTerminator = true;
					} while ((header) && pds.left());

					if(qlFrames.size() != 2)
					{
						assert(0);
					}

					if (p) {
						float a = static_cast<float>(avail);
						if (avail >= p->fAverageAvailable)
							p->fAverageAvailable = a;
						else
							p->fAverageAvailable *= 0.99f;
					}
				}
				else{

					jitter_buffer_update_delay(jbJitter, &jbp, NULL);

					iMissCount++;
					if (iMissCount > 10)
					{
						nextalive = false;
					}
				}
			}

			if (! qlFrames.empty()) {			

				QByteArray qba = qlFrames.front();
				qlFrames.pop_front();

				if(qba.size() < 110 || qba.size() > 120)
				{
					//assert(0);
				}

				if (umtType == MessageHandler::UDPVoiceCELT) {
					if (! cCodec) {
						cCodec = CELTCodec::instance();
						if (cCodec) {
							cdDecoder = cCodec->decoderCreate();
						}
					}					
					if (cdDecoder)
						cCodec->decode(cdDecoder, qba.empty() ? NULL : reinterpret_cast<const unsigned char *>(&qba[0]), 
							qba.size(), (celt_int16 *)pOut, SAMPLE_RATE / 50);
					else
						memset(pOut, 0, iFrameSize*sizeof(short)*2);
				} 
				else {
					assert(0);
				}

				bool update = true;
				if (p) {
					float &fPowerMax = p->fPowerMax;
					float &fPowerMin = p->fPowerMin;

					float pow = 0.0f;
					for (unsigned int i=0;i<iFrameSize*2;++i)
						pow += ((float)pOut[i]/32768.f*(float)pOut[i]/32768.f);
					pow = sqrtf(pow / static_cast<float>(iFrameSize*2));

					if (pow >= fPowerMax) {
						fPowerMax = pow;
					} 
					else {
						if (pow <= fPowerMin) {
							fPowerMin = pow;
						} else {
							fPowerMax = 0.99f * fPowerMax;
							fPowerMin += 0.0001f * pow;
						}
					}

					update = (pow < (fPowerMin + 0.01f * (fPowerMax - fPowerMin)));
				}
				if (qlFrames.empty() && update)
					jitter_buffer_update_delay(jbJitter, NULL, NULL);

				if (qlFrames.empty() && bHasTerminator)
					nextalive = false;
			} 
			else {
				if (umtType == MessageHandler::UDPVoiceCELT) {
					if (! cCodec) {
						cCodec = CELTCodec::instance();
						if (cCodec) {
							cdDecoder = cCodec->decoderCreate();
						}
					}
					//if (cdDecoder)
					//	cCodec->decode(cdDecoder, NULL, 0, pOut, SAMPLE_RATE /50);
					//else
					//	memset(pOut, 0, iFrameSize*sizeof(short));
				} 
				else {
					assert(0);
				}
			}

			if (! nextalive) {
				for (unsigned int i=0;i<iFrameSize*2;++i)
					pOut[i] *= fFadeOut[i];
			} else if (ts == 0) {
				for (unsigned int i=0;i<iFrameSize*2;++i)
					pOut[i] *= fFadeIn[i];
			}

			jitter_buffer_tick(jbJitter);
		}
nextframe:		
		iBufferFilled += snum;
	}

	bool tmp = bLastAlive;
	bLastAlive = nextalive;
	return tmp;
}
