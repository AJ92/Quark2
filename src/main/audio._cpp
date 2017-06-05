#include "audio.h"
#include <iostream>

Audio::Audio(bool debug) :
	data()
{
	bool pre_init_done = pre_init();
	bool post_init_done = post_init();

	if (pre_init_done &&
		post_init_done) {
		std::cout << "Audio initialized" << std::endl;
		ready = true;
	}
	else {
		std::cout << "Audio initialization failed" << std::endl;
	}
}

Audio::~Audio() {
	bool pre_deinit_done = pre_deinit();
	bool post_deinit_done = post_deinit();

	if (pre_deinit_done &&
		post_deinit_done) {
		std::cout << "Audio deinitialized" << std::endl;
	}
	else {
		std::cout << "Audio deinitialization failed" << std::endl;
	}
	ready = false;
}

// This is the function that gets called when we need to generate sound! 
int Audio::PortAudioCallback(const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{
	Audio * handler = (Audio *)userData;

	unsigned long stereoFrameCount = frameCount * handler->CHANNEL_COUNT;
	memset((int *)output, 0, stereoFrameCount * sizeof(int));


	if (handler->data.size() > 0)
	{
		auto it = handler->data.begin();
		while (it != handler->data.end())
		{
			Playback * data = (*it);
			AudioFile * audioFile = data->audioFile;

			int * outputBuffer = new int[stereoFrameCount];
			int * bufferCursor = outputBuffer;

			unsigned int framesLeft = (unsigned int)frameCount;
			unsigned int framesRead;

			bool playbackEnded = false;
			while (framesLeft > 0)
			{
				sf_seek(audioFile->data, data->position, SEEK_SET);

				if (framesLeft > (audioFile->info.frames - data->position))
				{
					framesRead = (unsigned int)(audioFile->info.frames - data->position);
					if (data->loop)
					{
						data->position = 0;
					}
					else
					{
						playbackEnded = true;
						framesLeft = framesRead;
					}
				}
				else
				{
					framesRead = framesLeft;
					data->position += framesRead;
				}

				sf_readf_int(audioFile->data, bufferCursor, framesRead);

				bufferCursor += framesRead;

				framesLeft -= framesRead;
			}


			int * outputCursor = (int *)output;
			if (audioFile->info.channels == 1) {
				for (unsigned long i = 0; i < stereoFrameCount; ++i)
				{
					*outputCursor += 0.8 * outputBuffer[i];
					++outputCursor;
					*outputCursor += 0.8 * outputBuffer[i];
					++outputCursor;
				}
			}
			else {
				for (unsigned long i = 0; i < stereoFrameCount; ++i)
				{
					*outputCursor += 0.8 * outputBuffer[i];
					++outputCursor;
				}
			}


			if (playbackEnded) {
				it = handler->data.erase(it);
				delete data;
			}
			else
			{
				++it;
			}


			delete outputBuffer;
		}
	}
	return paContinue;
}

void Audio::processEvent(AudioEventType audioEventType, AudioFile * audioFile, bool loop)
{
	switch (audioEventType) {
	case start:
		if (Pa_IsStreamStopped(stream))
		{
			Pa_StartStream(stream);
		}

		data.push_back(new Playback{
			audioFile,
			0,
			loop
		});

		break;
	case stop:
		Pa_StopStream(stream);
		for (auto instance : data)
		{
			delete instance;
		}
		data.clear();
		break;
	}
}

bool Audio::isDebug() {
	return debug;
}

bool Audio::isReady() {
	return ready;
}

bool Audio::pre_init() {
	PaError errorCode;
	errorCode = Pa_Initialize();
	if (errorCode != paNoError)
	{
		std::cout << "pre_init error: " << Pa_GetErrorText(errorCode) << std::endl;
		ready = false;
		return false;
	}

	PaStreamParameters outputParameters;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = CHANNEL_COUNT;
	outputParameters.sampleFormat = paInt32;
	outputParameters.suggestedLatency = 0.125;
	outputParameters.hostApiSpecificStreamInfo = 0;

	errorCode = Pa_OpenStream(&stream,
		NO_INPUT,
		&outputParameters,
		SAMPLE_RATE,
		paFramesPerBufferUnspecified,
		paNoFlag,
		&PortAudioCallback,
		this);

	if (errorCode != paNoError)
	{
		Pa_Terminate();

		std::cout << "pre_init error: " << Pa_GetErrorText(errorCode) << std::endl;
		ready = false;
		return false;
	}
	return true;
}

AudioFile & Audio::getSound(std::string filename) {
	//check if we already have such a file
	{
		auto search = sounds.find(filename);
		if (search != sounds.end()) {
			return search->second;
		}
	}

	SF_INFO info;
	info.format = 0;
	SNDFILE * audioFile = sf_open(filename.c_str(), SFM_READ, &info);

	AudioFile sound{
		audioFile,
		info
	};

	if (!audioFile)
	{
		std::cout << "Unable to open audio file: " << filename << std::endl;
	}
	sounds[filename] = sound;
	return sounds[filename];
}

bool Audio::post_init() {
	return true;
}

bool Audio::pre_deinit() {
	return true;
}
bool Audio::post_deinit() {
	PaError err = Pa_CloseStream(stream);
	if (err != paNoError) {
		std::cout << "post_deinit error: " << Pa_GetErrorText(err) << std::endl;
		return false;
	}
	//clean up playbacks
	for (auto wrapper : data)
	{
		delete wrapper;
	}
	err = Pa_Terminate();
	if (err != paNoError) {
		std::cout << "post_deinit error: " << Pa_GetErrorText(err) << std::endl;
		return false;
	}
	//clean up audiofiles
	for (auto entry : sounds)
	{
		sf_close(entry.second.data);
	}
	return true;
}

