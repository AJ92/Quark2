#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "base/systems/system.h"
#include "base/datastructures/components/script.h"
#include "base/datastructures/components/scriptinstance.h"
#include "base/datastructures/components/pyscriptinstance.h"
#include "base/datastructures/management/componentmanagement.h"
//#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>
#include <memory>
#include <map>

namespace py = pybind11;

class ScriptSystem: public System
{
public:
	ScriptSystem();
	ScriptSystem(std::shared_ptr<ComponentManagement> cm);
	~ScriptSystem();

	//initialize all script objects
	virtual void init();
	//update all script objects...
	virtual void update();
	
private:
	std::shared_ptr<ComponentManagement> _component_management;

	bool _init();
	bool _deinit();
	bool _init_python();
	bool _deint_python();

	py::scoped_interpreter guard{};
};

#endif // SCRIPTSYSTEM_H
