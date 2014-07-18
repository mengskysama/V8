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

	if (umtType != MessageHandler::UDPVoiceSpeex) {
		srate = SAMPLE_RATE;
		iFrameSize = srate / 100;

		dsSpeex = NULL;
	} else {
		speex_bits_init(&sbBits);

		dsSpeex = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_WB));
		int iArg=1;
		speex_decoder_ctl(dsSpeex, SPEEX_SET_ENH, &iArg);
		speex_decoder_ctl(dsSpeex, SPEEX_GET_FRAME_SIZE, &iFrameSize);
		speex_decoder_ctl(dsSpeex, SPEEX_GET_SAMPLING_RATE, &srate);
	}

	if (freq != srate)
		srs = speex_resampler_init(1, srate, freq, 3, &err);
	else
		srs = NULL;

	iOutputSize = int(ceilf(static_cast<float>(iFrameSize * freq) / static_cast<float>(srate)));

	iBufferOffset = iBufferFilled = iLastConsume = 0;
	bLastAlive = true;

	iMissCount = 0;
	iMissedFrames = 0;

	ucFlags = 0xFF;

	jbJitter = jitter_buffer_init(iFrameSize);
	int margin = g_struct.s.iJitterBufferSize * iFrameSize;
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);

	fFadeIn = new float[iFrameSize];
	fFadeOut = new float[iFrameSize];

	float mul = static_cast<float>(M_PI / (2.0 * static_cast<double>(iFrameSize)));
	for (unsigned int i=0;i<iFrameSize;++i)
		fFadeIn[i] = fFadeOut[iFrameSize-i-1] = sinf(static_cast<float>(i) * mul);
}

AudioOutputSpeech::~AudioOutputSpeech() {
	if (cdDecoder) {
		cCodec->decoder_destroy(cdDecoder);
	} else if (dsSpeex) {
		speex_bits_destroy(&sbBits);
		speex_decoder_destroy(dsSpeex);
	}

	if (srs)
		speex_resampler_destroy(srs);

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
		pds.skip(header & 0x7f);
	} while ((header & 0x80) && pds.isValid());

	if (pds.isValid()) {
		JitterBufferPacket jbp;
		jbp.data = (char*)(&qbaPacket[0]);//00 bc ...
		jbp.len = qbaPacket.size();//60*2+3
		jbp.span = iFrameSize * frames;//480*2
		jbp.timestamp = iFrameSize * iSeq;
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

	float *pOut;
	STACKVAR(float, fOut, iFrameSize + 4096);

	bool nextalive = bLastAlive;

	while (iBufferFilled < snum) {
		resizeBuffer(iBufferFilled + iOutputSize);

		pOut = (srs) ? fOut : (pfBuffer + iBufferFilled);

		if (! bLastAlive) {
			memset(pOut, 0, iFrameSize * sizeof(float));
		} else {
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
						memset(pOut, 0, iFrameSize * sizeof(float));
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

				if (jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs) == JITTER_BUFFER_OK) {
					
					PacketDataStream pds(jbp.data, jbp.len);

					iMissCount = 0;
					ucFlags = static_cast<unsigned char>(pds.next());
					bHasTerminator = false;

					unsigned int header = 0;
					do {
						header = static_cast<unsigned int>(pds.next());
						if (header)
							qlFrames.push_back(pds.dataBlock(header & 0x7f));
						else
							bHasTerminator = true;
					} while ((header & 0x80) && pds.isValid());

					if (p) {
						float a = static_cast<float>(avail);
						if (avail >= p->fAverageAvailable)
							p->fAverageAvailable = a;
						else
							p->fAverageAvailable *= 0.99f;
					}
				} else {
					jitter_buffer_update_delay(jbJitter, &jbp, NULL);

					iMissCount++;
					if (iMissCount > 10)
						nextalive = false;
				}
			}

			if (! qlFrames.empty()) {
				QByteArray qba = qlFrames.front();
				qlFrames.pop_front();

				if (umtType != MessageHandler::UDPVoiceSpeex) {
					if (! cCodec) {
						cCodec = new CELTCodec;
						if (cCodec) {
							cdDecoder = cCodec->decoderCreate();
						}
					}
					if (cdDecoder)
						cCodec->decode(cdDecoder, qba.empty() ? NULL : reinterpret_cast<const unsigned char *>(&qba[0]), 
							qba.size(), (celt_int16 *)pOut, SAMPLE_RATE / 100);
					else
						memset(pOut, 0, sizeof(float) * iFrameSize);
				} else {
					if (qba.empty()) {
						speex_decode(dsSpeex, NULL, pOut);
					} else {
						speex_bits_read_from(&sbBits, (char*)(&qba[0]), qba.size());
						speex_decode(dsSpeex, &sbBits, pOut);
					}
					for (unsigned int i=0;i<iFrameSize;++i)
						pOut[i] *= (1.0f / 32767.f);
				}

			bool update = true;
					if (p) {
					float &fPowerMax = p->fPowerMax;
					float &fPowerMin = p->fPowerMin;

					float pow = 0.0f;
					for (unsigned int i=0;i<iFrameSize;++i)
						pow += pOut[i] * pOut[i];
					pow = sqrtf(pow / static_cast<float>(iFrameSize));

					if (pow >= fPowerMax) {
						fPowerMax = pow;
					} else {
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
			} else {
				if (umtType != MessageHandler::UDPVoiceSpeex) {
					if (cdDecoder)
						cCodec->decode_float(cdDecoder, NULL, 0, pOut, SAMPLE_RATE / 100);
					else
						memset(pOut, 0, sizeof(float) * iFrameSize);
				} else {
					speex_decode(dsSpeex, NULL, pOut);
					for (unsigned int i=0;i<iFrameSize;++i)
						pOut[i] *= (1.0f / 32767.f);
				}
			}

			if (! nextalive) {
				for (unsigned int i=0;i<iFrameSize;++i)
					pOut[i] *= fFadeOut[i];
			} else if (ts == 0) {
				for (unsigned int i=0;i<iFrameSize;++i)
					pOut[i] *= fFadeIn[i];
			}

			jitter_buffer_tick(jbJitter);
		}
nextframe:
		spx_uint32_t inlen = iFrameSize;
		spx_uint32_t outlen = iOutputSize;
		if (srs && bLastAlive)
			speex_resampler_process_float(srs, 0, fOut, &inlen, pfBuffer + iBufferFilled, &outlen);
		iBufferFilled += outlen;
	}

	bool tmp = bLastAlive;
	bLastAlive = nextalive;
	return tmp;
}
