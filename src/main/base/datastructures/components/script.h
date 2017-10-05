#ifndef SCRIPT_H
#define SCRIPT_H

#include "base/datastructures/components/component.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>


namespace py = pybind11;

static bool _py_module_initialized = false;

class Script: public Component
{
public:
	Script();
	Script(const Script &script);
	~Script();

	virtual void init();
	virtual void update();

	void setScript(std::string script);

	int scriptSize(std::string script);
	bool hasScriptChanged();

private:
	bool _init();
	bool _deint();

	bool _reinit();
	
	py::object _import(const std::string& module, const std::string& path, py::object& globals);

	std::string _script_file;
	int _script_size;

	py::object _main;
	py::object _globals;
	py::object _module;
	py::object _module_vscript;
	py::object _vscript;

	py::object _py_init_f;
	py::object _py_update_f;

	

};

#endif // SCRIPT_H
