#ifndef HELPER_H
#define HELPER_H

#include "mathematics_global.h"

#define fuzzyThreshold   0.000001

class MATHEMATICSSHARED_EXPORT Helper
{
public:
	Helper();

	static bool fuzzyIsNull(double value);
	static bool fuzzyIsNull(float value);

};

#endif // HELPER_H