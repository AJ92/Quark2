#include "helper.h"

Helper::Helper()
{
}


//static
bool Helper::fuzzyIsNull(double value) {
	if (value < fuzzyThreshold && value > fuzzyThreshold) {
		return true;
	}
	else {
		return false;
	}
}

bool Helper::fuzzyIsNull(float value) {
	if (value < fuzzyThreshold && value > fuzzyThreshold) {
		return true;
	}
	else {
		return false;
	}
}

