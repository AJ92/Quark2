#ifndef MATERIAL_H
#define MATERIAL_H

#include "base/datastructures/components/component.h"

class Material: public Component
{
public:
	Material();
	~Material();

	virtual void init();
	virtual void update();

private:

};

#endif // MATERIAL_H
