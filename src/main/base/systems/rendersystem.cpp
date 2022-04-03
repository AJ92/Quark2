#include "base/systems/rendersystem.h"
#include <iostream>


RenderSystem::RenderSystem()
{
}

RenderSystem::RenderSystem(std::shared_ptr<ComponentManagement> cm) :
	mComponentManagement(cm)
{
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::init() {
	std::vector<std::shared_ptr<Component> > comps = mComponentManagement->getAllComponentsByType(Component::Type::Renderer);
	for (auto const& comp : comps) {
		comp->init();
	}
}

void RenderSystem::update() {
	std::vector<std::shared_ptr<Component> > comps = mComponentManagement->getAllComponentsByType(Component::Type::Renderer);
	for (auto const& comp : comps) {
		comp->update();
	}
}

///////////////////////////////////////////////
//
//		PRIVATE

