#include "engine.h"
#include <iostream>


Engine::Engine() {
	bool stage_1_done = init_stage_1_stuff();

	if (stage_1_done) {
		ready = true;
	}
}

bool Engine::isDebug() {
	return debug;
}

bool Engine::isReady() {
	return ready;
}

bool Engine::init_ugly_stuff() {
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