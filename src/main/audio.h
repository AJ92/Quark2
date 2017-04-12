#ifndef AUDIO_H
#define AUDIO_H

#include <windows.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include "portaudio.h"
#include "sndfile.h"


struct AudioFile
{
	SNDFILE * data;
	SF_INFO info;
};

struct Playback
{
	AudioFile * audioFile;
	int position;
	bool loop;
};
enum AudioEventType
{
	start, stop
};


class Audio
{
public:
	Audio(bool debug = false);
	~Audio();

	bool isDebug();
	bool isReady();

	void processEvent(AudioEventType audioEventType,
		AudioFile * audioFile = nullptr,
		bool loop = false);

	static int PortAudioCallback(const void *input, void *output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

	AudioFile & getSound(std::string filename);

private:
	bool debug;
	bool ready;

	const int CHANNEL_COUNT = 2;
	const int SAMPLE_RATE = 44000;
	const PaStreamParameters * NO_INPUT = nullptr;

	PaStream * stream;
	std::vector<Playback *> data;
	std::map<std::string, AudioFile> sounds;

	bool pre_init();
	bool post_init();

	bool pre_deinit();
	bool post_deinit();
};

#endif // AUDIO_H