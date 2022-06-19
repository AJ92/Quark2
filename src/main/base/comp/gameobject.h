#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "engine.h"
#include "base/comp/icomponent.h"
#include "base/comp/transformation.h"
#include <string>

class GameObject: public IComponent
{
public:
	GameObject();
	explicit GameObject(
		const std::string & name, 
		std::shared_ptr<GameObject> parent = nullptr
	);
	~GameObject() = default;

	virtual void init();
	virtual void update();

	const std::string getName() const;
	const std::shared_ptr<GameObject> getParent() const;

private:
	std::shared_ptr<Entity> mEntity;
	std::shared_ptr<GameObject> mParent;
	Transformation mTransform;
};

#endif // GAMEOBJECT_H
