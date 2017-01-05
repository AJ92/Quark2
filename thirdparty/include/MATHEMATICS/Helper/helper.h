#ifndef HELPER_H
#define HELPER_H

#include "mathematics_global.h"

#define fuzzyThreshold   0.000001

class Helper
{
public:
	MATHEMATICSSHARED_EXPORT Helper();

	MATHEMATICSSHARED_EXPORT static bool fuzzyIsNull(double value);
	MATHEMATICSSHARED_EXPORT static bool fuzzyIsNull(float value);
};

#endif // HELPER_H