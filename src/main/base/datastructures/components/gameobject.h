#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "base/datastructures/components/component.h"

class GameObject: public Component
{
public:
	GameObject();
	~GameObject();

	virtual void init();
	virtual void update();

private:

};

#endif // GAMEOBJECT_H
