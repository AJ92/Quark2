#ifndef MATERIAL_H
#define MATERIAL_H

#include "base/comp/icomponent.h"

class Material: public IComponent
{
public:
	Material();
	~Material();

	virtual void init();
	virtual void update();

private:

};

#endif // MATERIAL_H
