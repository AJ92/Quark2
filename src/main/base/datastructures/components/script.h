#ifndef SCRIPT_H
#define SCRIPT_H

#include "base/datastructures/components/component.h"
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>


namespace py = pybind11;

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
};


#endif // SCRIPT_H
