#ifndef MATHEMATICS_GLOBAL_H
#define MATHEMATICS_GLOBAL_H


#if defined(MATHEMATICS_LIBRARY_STATIC)
#	define MATHEMATICSSHARED_EXPORT
#else
#	ifndef MATHEMATICSSHARED_EXPORT
#		if defined(MATHEMATICS_LIBRARY_DYNAMIC)
#			define MATHEMATICSSHARED_EXPORT __declspec(dllexport) 
#		else
#			define MATHEMATICSSHARED_EXPORT __declspec(dllimport) 
#		endif
#	endif
#endif


#endif // MATHEMATICS_GLOBAL_H
