#include "base/systems/scriptsystem.h"
#include <iostream>


ScriptSystem::ScriptSystem()
{
	_init();
}

ScriptSystem::ScriptSystem(std::shared_ptr<ComponentManagement> cm) :
	_component_management(cm)
{
	_init();
}

ScriptSystem::~ScriptSystem()
{
    _deint_python();
}

void ScriptSystem::update() {
	std::vector<std::shared_ptr<Component> > comps = _component_management->getAllComponentsByType(Component::Script);
	for (auto const& comp : comps) {
		comp->update();
	}
}

///////////////////////////////////////////////
//
//		PRIVATE

bool ScriptSystem::_init() {
	bool init_done = _init_python();

	if (init_done) {
		std::cout << "Python initialized" << std::endl;
	}
	else {
		std::cout << "Python initialization failed" << std::endl;
	}
	return init_done;
}

bool ScriptSystem::_deinit() {
	bool deinit_done = _deint_python();

	if (deinit_done) {
		std::cout << "Python deinitialized" << std::endl;
	}
	else {
		std::cout << "Python deinitialization failed" << std::endl;
	}
	return deinit_done;
}

bool ScriptSystem::_init_python() {

	Py_SetProgramName("Vulkan0");  /* optional but recommended */
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");

	return true;
}

bool ScriptSystem::_deint_python() {
	//Py_Finalize(); //segfault, need to check...
	return true;
}
