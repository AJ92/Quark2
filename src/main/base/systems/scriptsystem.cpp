#include "base/systems/scriptsystem.h"
#include <iostream>


ScriptSystem::ScriptSystem()
{
	init();
}

ScriptSystem::ScriptSystem(std::shared_ptr<ComponentManagement> cm) :
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

	std::vector<std::shared_ptr<Component> > comps = mComponentManagement->getAllComponentsByType(Component::Type::Script);
	for (auto const& comp : comps) {
		comp->init();
	}
}

void ScriptSystem::update() {
	std::vector<std::shared_ptr<Component> > comps = mComponentManagement->getAllComponentsByType(Component::Type::Script);
	for (auto const& comp : comps) {
		comp->update();
	}
}


bool ScriptSystem::initPython() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	//windows
	Py_SetProgramName(L"Vulkan0Script");
#elif
	//linux 
	//Py_SetProgramName("Vulkan0Script");
#endif	
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
