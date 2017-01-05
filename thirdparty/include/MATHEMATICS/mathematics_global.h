#ifndef MATHEMATICS_GLOBAL_H
#define MATHEMATICS_GLOBAL_H

#if defined(MATHEMATICS_LIBRARY)
#  define MATHEMATICSSHARED_EXPORT __declspec(dllexport) 
#else
#  define MATHEMATICSSHARED_EXPORT __declspec(dllimport) 
#endif

#endif // MATHEMATICS_GLOBAL_H
