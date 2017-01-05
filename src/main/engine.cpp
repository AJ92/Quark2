#include "engine.h"
#include <iostream>


Engine::Engine() {
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
}

bool Engine::isDebug() {
	return debug;
}