#include "script.h"
#include <iostream>
#include "scriptinstance.h"
#include "pyscriptinstance.h"

// DEFINE THE PYTHON MODULE
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

Script::Script() :
	Component(Type::Script)
{
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

}

void Script::init() {

}

///////////////////////////////////////////////
//
//		PRIVATE

bool Script::_init() {
	//try init the python scripting instance and run init there
	if (_script_file.empty()) {
		return false;
	}

	try
	{
		pybind11_init();

		py::object main = py::module::import("__main__");
		py::object globals = main.attr("__dict__");
		py::object module = _import("vscript", _script_file, globals);
		py::object VScript = module.attr("VScript");
		py::object vscript = VScript(this);

		vscript.attr("init")();
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
