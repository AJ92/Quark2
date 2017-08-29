#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "base/datastructures/components/component.h"

class Geometry: public Component
{
public:
	Geometry();
	~Geometry();

	virtual void init();
	virtual void update();

private:

};

#endif // GEOMETRY_H
