
//#include <vld.h> //check for mem leaks

//#include "base/mem/memmanagement.h"
#if defined(_DEBUG)
#include "base/mem/allocator/ltalloc.cc"
#endif

#include "engine.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <cstdio>
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

  std::cerr << "output is redirected to out.txt" << std::endl;
  std::ofstream out("out.txt");
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

  quark::Engine e;

  bool success = true;

  try {
    e.run();
  }
  catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    DBOUT("runtime error: " << e.what());
    #endif

    success = false;
  }

  std::cout.rdbuf(coutbuf); //reset to standard output again

  //memuse();

  if (!success) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
