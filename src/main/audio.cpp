#include "audio.h"
#include <iostream>


// All audio will be handled as stereo.
const int NUM_CHANNELS = 2;

// Declaration for audio callback called by PortAudio.
int audioCallback(const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData);

// This is the function that gets called when we need to generate sound! 
// In this example, we're going to decode some audio using libaudiodecoder
// and fill the "output" buffer with that. In other words, we're going to
// decode demo.mp3 and send that audio to the soundcard. Easy!
int audioCallback(const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{

	AudioDecoder* pAudioDecoder = static_cast<AudioDecoder*>(userData);

	// Play it safe when debugging and coding, protect your ears by clearing
	// the output buffer.
	memset(output, 0, frameCount * NUM_CHANNELS * sizeof(float));

	// Decode the number of samples that PortAudio said it needs to send to the 
	// soundcard. This is where we're grabbing audio from demo.mp3!
	int samplesRead = pAudioDecoder->read(frameCount * NUM_CHANNELS,
		static_cast<SAMPLE*>(output));

	// IMPORTANT:
	// Note that in a real application, you will probably want to call read()
	// in a separate thread because it is NOT realtime safe. That means it does
	// not run in constant time because it might be allocating memory, calling
	// system functions, or doing other things that can take an 
	// unpredictably long amount of time.
	//
	// The danger is that if read() takes too long, then audioCallback() might not
	// finish quickly enough, and there will be a "dropout" or pop in the audio
	// that comes out your speakers. 
	//
	// If you want to guard against dropouts, you should either decode the entire file into
	// memory or decode it on-the-fly in a separate thread. To implement the latter, you would:
	//   - Set up a secondary thread and read() a few seconds of audio into a ringbuffer
	//   - When the callback runs, you want to fetch audio from that ringbuffer.
	//   - Have the secondary thread keep read()ing and ensuring the ringbuffer is full.
	//
	// Ross Bencina has a great introduction to realtime programming and goes into
	// more detail here:
	// http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing

	return paContinue;
}

Audio::Audio(bool debug) {
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

bool Audio::isDebug() {
	return debug;
}

bool Audio::isReady() {
	return ready;
}

bool Audio::pre_init() {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		std::cout << "error: " << Pa_GetErrorText(err) << std::endl;
		return false;
	}
	return true;
}
bool Audio::post_init() {
	test();
	return true;
}

bool Audio::pre_deinit() {
	return true;
}
bool Audio::post_deinit() {
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		std::cout << "deinit_sound error: " << Pa_GetErrorText(err) << std::endl;
		return false;
	}
	return true;
}

/**
* gen saw tooth wave...
*/
bool Audio::test() {
	// Initialize an AudioDecoder object and open demo.mp3
	std::string filename = "demo.mp3";
	AudioDecoder* pAudioDecoder = new AudioDecoder(filename);

	if (pAudioDecoder->open() != AUDIODECODER_OK)
	{
		std::cerr << "Failed to open " << filename << std::endl;
		return 1;
	}

	// Initialize the PortAudio library.
	if (Pa_Initialize() != paNoError) {
		std::cerr << "Failed to initialize PortAudio." << std::endl;
		return 1;
	};

	PaStream* pStream = NULL;

	// Open the PortAudio stream (opens the soundcard device).
	if (Pa_OpenDefaultStream(&pStream,
		0, // No input channels
		2, // 2 output channel
		paFloat32, // Sample format (see PaSampleFormat)           
		44100, // Sample Rate
		paFramesPerBufferUnspecified,  // Frames per buffer 
		&audioCallback,
		static_cast<void*>(pAudioDecoder)) != paNoError)
	{
		std::cerr << "Failed to open the default PortAudio stream." << std::endl;
		return 1;
	}

	// Start the audio stream. PortAudio will then start calling our callback function
	// every time the soundcard needs audio.
	// Note that this is non-blocking by default!
	if (Pa_StartStream(pStream) != paNoError)
	{
		std::cerr << "Failed to start the PortAudio stream." << std::endl;
		return 1;
	}

	// So here's where control would normally go back to your program, probably
	// your GUI thread. The audio will be decoded and played back in a separate thread
	// (managed by PortAudio) via the callback function we've defined below.
	// Since we have no GUI to in this example, we're just going to sleep here
	// for a while.

	//Sleep(20000);


	// Shutdown:
	// First, stop the PortAudio stream (closes the soundcard device).
	/*
	if (Pa_StopStream(pStream) != paNoError)
	{
		std::cerr << "Failed to stop the PortAudio stream." << std::endl;
		return 1;
	}

	// Tell the PortAudio library that we're all done with it.
	if (Pa_Terminate() != paNoError)
	{
		std::cerr << "Failed to terminate PortAudio." << std::endl;
		return 1;
	}
	*/

	// Close the AudioDecoder object and free it.
	delete pAudioDecoder;
	

	return 0;
}
