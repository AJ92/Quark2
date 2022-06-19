#include "base/systems/scriptsystem.h"
#include <iostream>

ScriptSystem::ScriptSystem(const std::shared_ptr<ComponentManagement> cm) :
	mComponentManagement(cm)
{
	init();
}

ScriptSystem::~ScriptSystem()
{
	cleanUpPython();
}

void ScriptSystem::init() {
	bool init_done = initPython();

	if (init_done) {
		std::cout << "Python initialized" << std::endl;
	}
	else {
		std::cout << "Python initialization failed" << std::endl;
	}

	auto& comps = mComponentManagement->getAllComponentsByType(IComponent::Type::Script);
	for (auto const& comp : comps) {
		comp->init();
	}
}

void ScriptSystem::update() {
	auto& comps = mComponentManagement->getAllComponentsByType(IComponent::Type::Script);
	for (auto const& comp : comps) {
		comp->update();
	}
}


bool ScriptSystem::initPython() {
	Py_SetProgramName(L"Vulkan0Script");
	
	py::dict locals;
	locals["resource_path"] = py::cast("resources/scripts");
	py::exec(
		"import sys\n"
		"sys.path.append(resource_path)\n",
		py::globals(),
		locals
	);
	
	return true;
}

bool ScriptSystem::cleanUpPython() {
	//Py_Finalize(); //segfault, need to check...
	return true;
}
