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

//namespace plugin_script {
	//PYBIND11_MODULE(Vulkan0Script, m)
	PYBIND11_EMBEDDED_MODULE(Vulkan0Script, m)
	{
		//if (!_py_module_initialized) {
			m.doc() = "Python based Vulkan0-Scripting";

			py::class_<Script>(m, "Script")
				;

			py::class_<ScriptInstance, PyScriptInstance>(m, "ScriptInstance")
				.def(py::init<Script*>()) // constructor
				.def("test", &ScriptInstance::test) //member function
				.def("log", &ScriptInstance::log)
				;

			_py_module_initialized = true;
		//}
	}
//}


Script::Script() :
	Component(Type::Script)
{

}

Script::Script(const Script &script):
	Component(Type::Script),
	_script_file(script._script_file)
{

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

void Script::setScript(std::string script){
	_script_file = script;
	_init();
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
		//pybind11_init_wrapper();
		
		//auto py_module = py::module::import(_script_file.c_str());
		_module = py::module::import("script1");
		_module_vscript = _module.attr("VScript");


		_vscript = _module_vscript(this);

		_py_init_f = _vscript.attr("init");
		
		_py_update_f = _vscript.attr("update");
		
	}
	catch (const std::runtime_error& e)
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
