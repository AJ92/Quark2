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
	auto& comps = mComponentManagement->getAllComponentsByType(IComponent::Type::Renderer);
	for (auto const& comp : comps) {
		comp->init();
	}
}

void RenderSystem::update() {
	auto& comps = mComponentManagement->getAllComponentsByType(IComponent::Type::Renderer);
	for (auto const& comp : comps) {
		comp->update();
	}
}

///////////////////////////////////////////////
//
//		PRIVATE

