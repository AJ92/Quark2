#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "base/systems/isystem.h"
#include "base/comp/scripting/script.h"
#include "base/comp/management/componentmanagement.h"
//#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>
#include <memory>
#include <map>

namespace py = pybind11;

class ScriptSystem: public ISystem
{
public:
	ScriptSystem() = delete;
	ScriptSystem(const std::shared_ptr<ComponentManagement> cm);
	~ScriptSystem();

	//initialize all script objects
	virtual void init();
	//update all script objects...
	virtual void update();
	
private:
	bool initPython();
	bool cleanUpPython();

	py::scoped_interpreter guard{};

	// has to be after the interpreter for dtor order.
	std::shared_ptr<ComponentManagement> mComponentManagement;
};

#endif // SCRIPTSYSTEM_H
