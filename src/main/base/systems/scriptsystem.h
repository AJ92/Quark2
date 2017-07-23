#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "base/systems/system.h"
#include "base/datastructures/componentmanagement.h"
#include <Python.h>
#include <memory>
#include <map>

class ScriptSystem: public System
{
public:
	ScriptSystem();
	ScriptSystem(std::shared_ptr<ComponentManagement> cm);
	~ScriptSystem();

	//update all script objects...
	virtual void update();

private:
	std::shared_ptr<ComponentManagement> _component_management;

	bool _init();
	bool _deinit();
	bool _init_python();
	bool _deint_python();
};

#endif // PYTHONSCRIPTING_H
