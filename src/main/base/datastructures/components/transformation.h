#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "base/datastructures/components/component.h"

class Transformation: public Component
{
public:
	Transformation();
	~Transformation();

	virtual void init();
	virtual void update();

private:

};

#endif // TRANSFORMATION_H
