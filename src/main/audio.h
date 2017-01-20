#ifndef AUDIO_H
#define AUDIO_H

#include <windows.h>
#include <iostream>
#include "audiodecoder.h" 
#include "portaudio.h"


class Audio
{
public:
	Audio(bool debug = false);
	~Audio();

	bool isDebug();
	bool isReady();

private:
	bool debug;
	bool ready;

	bool pre_init();
	bool post_init();

	bool pre_deinit();
	bool post_deinit();

	bool test();
};

#endif // AUDIO_H