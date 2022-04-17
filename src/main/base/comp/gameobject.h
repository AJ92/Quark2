#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "base/comp/icomponent.h"

class GameObject: public IComponent
{
public:
	GameObject();
	~GameObject() = default;

	virtual void init();
	virtual void update();

private:

};

#endif // GAMEOBJECT_H
