#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "base/systems/isystem.h"
#include "base/comp/management/componentmanagement.h"
#include <memory>
#include <map>


class RenderSystem: public ISystem
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
	std::shared_ptr<ComponentManagement> mComponentManagement;
};

#endif // RENDERSYSTEM_H
