#ifndef SCRIPT_H
#define SCRIPT_H

#include "base/datastructures/components/component.h"
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>


namespace py = pybind11;

static bool _py_module_initialized = false;

class Script: public Component
{
public:
	Script();
	Script(std::string & scriptFile);
	~Script();

	virtual void init();
	virtual void update();

private:
	bool _init();
	bool _deint();

	py::object _import(const std::string& module, const std::string& path, py::object& globals);

	std::string _script_file;

	py::object _main;
	py::object _globals;
	py::object _module;
	py::object _module_vscript;
	py::object _vscript;

	py::object _py_init_f;
	py::object _py_update_f;

};

#endif // SCRIPT_H
