#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "base/comp/icomponent.h"

class Transformation: public IComponent
{
public:
	Transformation();
	~Transformation() = default;

	virtual void init();
	virtual void update();

private:

};

#endif // TRANSFORMATION_H
