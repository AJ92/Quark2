#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "base/systems/system.h"
#include "base/datastructures/management/componentmanagement.h"
#include <memory>
#include <map>


class RenderSystem: public System
{
public:
	RenderSystem();
	RenderSystem(std::shared_ptr<ComponentManagement> cm);
	~RenderSystem();

	//initialize all render objects
	virtual void init();
	//update all render objects...
	virtual void update();
	
private:
	std::shared_ptr<ComponentManagement> _component_management;
};

#endif // RENDERSYSTEM_H
