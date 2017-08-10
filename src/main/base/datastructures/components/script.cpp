#include "script.h"
#include <iostream>
#include <fstream>
#include "scriptinstance.h"
#include "pyscriptinstance.h"

//deprecated
/*
PYBIND11_PLUGIN(Vulkan0Script)
{
	py::module m("Vulkan0Script", "Python based Vulkan0-Scripting");

	py::class_<Script>(m, "Script")
		;

	py::class_<ScriptInstance, PyScriptInstance>(m, "ScriptInstance")
		.def(py::init<Script*>()) // constructor
		.def("test", &ScriptInstance::test) //member function
		;

	return m.ptr();
}
*/


PYBIND11_MODULE(Vulkan0Script, m)
{
	if (!_py_module_initialized) {
		m.doc() = "Python based Vulkan0-Scripting";

		py::class_<Script>(m, "Script")
			;

		py::class_<ScriptInstance, PyScriptInstance>(m, "ScriptInstance")
			.def(py::init<Script*>()) // constructor
			.def("test", &ScriptInstance::test) //member function
			.def("log", &ScriptInstance::log)
			;

		_py_module_initialized = true;
	}
}


Script::Script() :
	Component(Type::Script)
{
	_script_size = 0;

	bool init_done = _init();

	if (init_done) {
		std::cout << "Script initialized" << std::endl;
	}
	else {
		std::cout << "Script initialization failed" << std::endl;
	}
}

Script::Script(std::string & scriptFile) :
	Component(Type::Script),
	_script_file(scriptFile)
{
	bool init_done = _init();

	if (init_done) {
		std::cout << "Script " << scriptFile << " initialized" << std::endl;
	}
	else {
		std::cout << "Script " << scriptFile << " initialization failed" << std::endl;
	}
}

Script::~Script()
{

}

void Script::update() {
	try {
	if(_py_update_f.ptr() != nullptr && _vscript.ptr() != nullptr)
		_py_update_f(_vscript); //call update in python object...
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

void Script::init() {
	try {
		if (_py_init_f.ptr() != nullptr && _vscript.ptr() != nullptr)
			_py_init_f(); //call init in python object...
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

int Script::scriptSize(std::string script) {
	if (script.empty()) {
		return 0;
	}
	std::ifstream in(_script_file, std::ifstream::ate | std::ifstream::binary);
	std::streampos pos = in.tellg();
	return (int)pos;
}

bool Script::hasScriptChanged() {
	bool changed = false;
	int new_script_size = scriptSize(_script_file);
	if (_script_size != new_script_size) {
		changed = true;
	}
	_script_size = new_script_size;
	return changed;
}

///////////////////////////////////////////////
//
//		PRIVATE

bool Script::_init() {
	//try init the python scripting instance and run init there
	if (_script_file.empty()) {
		return false;
	}

	_script_size = scriptSize(_script_file);

	try
	{
		pybind11_init_wrapper();
	
		_main = py::module::import("__main__");
		_globals = _main.attr("__dict__");
		_module = _import("vscript", _script_file, _globals);
		_module_vscript = _module.attr("VScript");
		_vscript = _module_vscript(this);

		_py_init_f = _vscript.attr("init");
		
		_py_update_f = _vscript.attr("update");
		
	}
	catch (const py::error_already_set& e)
	{
		std::cerr << ">>> Error! Uncaught exception:\n";
		std::cerr << e.what() << std::endl;
		PyErr_Print();
	}

	return true;
}

bool Script::_deint() {
	//TODO: ...
	return true;
}

bool Script::_reinit() {
	if (_script_file.empty()) {
		return false;
	}
	std::ifstream in(_script_file, std::ifstream::ate | std::ifstream::binary);
	std::streampos pos = in.tellg();

	return true;
}

py::object Script::_import(const std::string& module, const std::string& path, py::object& globals)
{
	py::dict locals;
	locals["module_name"] = py::cast(module);
	locals["path"] = py::cast(path);

	py::eval<py::eval_statements>(
		"import imp\n"
		"new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
		globals,
		locals);

	return locals["new_module"];
}
