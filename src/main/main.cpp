#include "engine.h"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


#include <iostream>
#include <sstream>
#include <stdexcept>
#include <functional>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define NOMINMAX
	#include <Windows.h>


    #define DBOUT( s )            \
    {                             \
       std::wostringstream os_;    \
       os_ << s;                   \
       OutputDebugStringW( os_.str().c_str() );  \
    }
#endif


int main(void)
{
	Engine e;

	try {
		e.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		DBOUT("runtime error: " << e.what());
		#endif
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}
