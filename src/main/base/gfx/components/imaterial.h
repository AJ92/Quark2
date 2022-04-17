#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "base/comp/icomponent.h"

class IMaterial: public IComponent
{
public:
	IMaterial();
	virtual ~IMaterial() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:

};

#endif // IMATERIAL_H
