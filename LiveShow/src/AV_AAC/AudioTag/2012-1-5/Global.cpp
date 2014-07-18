#include "stdafx.h"
#include "Global.h"

Global *Global::g_global_struct = NULL;

bool bIsVistaSP1 = false;
bool bIsWin7 = false;

Global::Global(AudioTransport *ptrans) {
	uiSession = 0;
	iTarget = 0;
	iPrevTarget = 0;
	bPushToMute = false;


	bAttenuateOthers = false;
	trans = ptrans;

}

Global::~Global() {
}

void DeferInit::add(int priority) {
	if (qmDeferers == NULL) {
		qmDeferers = new std::multimap<int, DeferInit *>();
	}
	qmDeferers->insert(make_pair(priority, this));
}

std::multimap<int, DeferInit *> *DeferInit::qmDeferers = NULL;

DeferInit::~DeferInit() {
}

void DeferInit::run_initializers() {
	if (! qmDeferers)
		return;
	std::multimap<int, DeferInit *>::iterator mit;
	for (mit = qmDeferers->begin(); mit != qmDeferers->end(); mit ++)
	{
		mit->second->initialize();
	}
}

void DeferInit::run_destroyers() {
	if (! qmDeferers)
		return;
	std::multimap<int, DeferInit *>::iterator mit;
	for (mit = qmDeferers->begin(); mit != qmDeferers->end(); mit ++)
	{
		mit->second->destroy();
	}
	delete qmDeferers;
	qmDeferers = NULL;
}


void osInit()
{
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionExW(reinterpret_cast<OSVERSIONINFOW *>(&ovi));
	bIsWin7 = (ovi.dwMajorVersion >= 7) || ((ovi.dwMajorVersion == 6) &&(ovi.dwBuildNumber >= 7100));
	bIsVistaSP1 = (ovi.dwMajorVersion >= 7) || ((ovi.dwMajorVersion == 6) &&(ovi.dwBuildNumber >= 6001));
}