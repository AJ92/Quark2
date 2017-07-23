#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "base/systems/system.h"
#include "base/datastructures/components/script.h"
#include "base/datastructures/components/scriptinstance.h"
#include "base/datastructures/components/pyscriptinstance.h"
#include "base/datastructures/componentmanagement.h"
//#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
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
