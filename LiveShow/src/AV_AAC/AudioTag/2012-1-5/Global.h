#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "Settings.h"
#include "audiotransport.h"

// Global helper class to spread variables around across threads.

extern bool bIsVistaSP1;
extern bool bIsWin7;

class AudioInput;
class AudioOutput;
class CELTCodec;
struct Global {
public:
	static Global *g_global_struct;
	Settings s;
	boost::shared_ptr<AudioInput> ai;
	boost::shared_ptr<AudioOutput> ao;
	int iTarget; 
	int iPrevTarget;
	bool bPushToMute;
	unsigned int uiSession;
	bool bAttenuateOthers;
	Global(AudioTransport *ptrans);
	~Global();

	AudioTransport *trans;
};

void osInit();


class DeferInit {
protected:
	static std::multimap<int, DeferInit *> *qmDeferers;
	void add(int priority);
public:
	DeferInit(int priority) {
		add(priority);
	};
	DeferInit() {
		add(0);
	};
	virtual ~DeferInit();
	virtual void initialize() { };
	virtual void destroy() { };
	static void run_initializers();
	static void run_destroyers();
};


#define g_struct (*Global::g_global_struct)

#endif
