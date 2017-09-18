#include "base/systems/rendersystem.h"
#include <iostream>


RenderSystem::RenderSystem()
{
	//_init();
}

RenderSystem::RenderSystem(std::shared_ptr<ComponentManagement> cm) :
	_component_management(cm)
{
	//_init();
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::init() {
	std::vector<std::shared_ptr<Component> > comps = _component_management->getAllComponentsByType(Component::Renderer);
	for (auto const& comp : comps) {
		comp->init();
	}
}

void RenderSystem::update() {
	std::vector<std::shared_ptr<Component> > comps = _component_management->getAllComponentsByType(Component::Renderer);
	for (auto const& comp : comps) {
		comp->update();
	}
}

///////////////////////////////////////////////
//
//		PRIVATE

