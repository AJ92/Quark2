#include "engine.h"
#include <iostream>





Engine::Engine() {
	bool pre_init_done = pre_init();

	bool init_graphics_done = init_graphics();
	bool init_physics_done = init_physics();
	bool init_sound_done = init_audio();

	bool post_init_done = post_init();

	if (pre_init_done &&
		init_graphics_done &&
		init_physics_done &&
		init_sound_done &&
		post_init_done) {
		std::cout << "Engine initialized" << std::endl;
		ready = true;
	}
	else {
		std::cout << "Engine initialization failed" << std::endl;
	}
}

Engine::~Engine() {
	bool pre_deinit_done = pre_deinit();

	bool deinit_graphics_done = deinit_graphics();
	bool deinit_physics_done = deinit_physics();
	bool deinit_sound_done = deinit_audio();

	bool post_deinit_done = post_deinit();

	if (pre_deinit_done &&
		deinit_graphics_done &&
		deinit_physics_done &&
		deinit_sound_done &&
		post_deinit_done) {
		std::cout << "Engine deinitialized" << std::endl;
	}
	else {
		std::cout << "Engine deinitialization failed" << std::endl;
	}
	ready = false;
}

bool Engine::isDebug() {
	return debug;
}

bool Engine::isReady() {
	return ready;
}


std::shared_ptr<Audio> Engine::getAudio() {
	return audio;
}




///////////////////////////////////////////////
//
//		PRIVATE

bool Engine::pre_init() {
#ifndef _DEBUG
#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	FreeConsole();
#	endif
	std::cout << "RELEASE" << std::endl;
	debug = false;
#else
	std::cout << "DEBUG" << std::endl;
	debug = true;
#endif
	return true;
}

bool Engine::init_graphics() {
	return true;
}
bool Engine::init_physics() {
	return true;
}
bool Engine::init_audio() {
	audio = std::make_shared<Audio>(debug);
	return true;
}
bool Engine::post_init() {

	//std::string filename = "C:/Code/VS/engine-main/engine/build/Debug/test.flac";
	//audio->processEvent(AudioEventType::start, &audio->getSound(filename), true);
	std::string filename2 = "C:/Code/VS/engine-main/engine/build/Debug/test3.flac";
	audio->processEvent(AudioEventType::start, &audio->getSound(filename2), true);

	return true;
}

bool Engine::pre_deinit() {
	return true;
}
bool Engine::deinit_graphics() {
	return true;
}
bool Engine::deinit_physics() {
	return true;
}
bool Engine::deinit_audio() {
	return true;
}
bool Engine::post_deinit() {
	return true;
}
