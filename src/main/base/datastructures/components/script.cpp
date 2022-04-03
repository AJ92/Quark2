#include "script.h"
#include <iostream>
#include <fstream>

namespace plugin_script {
	PYBIND11_EMBEDDED_MODULE(Quark2, m)
	{
		if(!PY_MODULE_INITIALIZED) {
			m.doc() = "Python scripting access to Quark2 engine";

			auto script = py::class_<Script>(m, "VScript")
				.def("log", &Script::log);

			script.doc() = "Allows calling functions on the c++ side of Quark2";

			PY_MODULE_INITIALIZED = true;
		}
	}
}

Script::Script() :
	Component(Type::Script)
{

}

Script::Script(const Script &script):
	Component(Type::Script),
	mScriptFile(script.mScriptFile)
{

}

Script::~Script()
{

}

void Script::init() {
	try {
		if (mPyInitF.ptr() != nullptr && mVscript.ptr() != nullptr)
			mPyInitF(); //call init in python object...
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

void Script::update() {

	std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_end;

	time_start = std::chrono::high_resolution_clock::now();

	time_end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_time = time_end - time_start;

	try {
		if(mPyUpdateF.ptr() != nullptr && mVscript.ptr() != nullptr)
			mPyUpdateF(mVscript); //call update in python object...
		}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

void Script::log(std::string str) {
	std::cout << mScriptFile << ": " << str << std::endl;
}

void Script::setScript(std::string script){
	mScriptFile = script;
	initScript();
}

///////////////////////////////////////////////
//
//		PRIVATE

int Script::scriptSize(std::string script) {
	if (script.empty()) {
		return 0;
	}
	std::ifstream in(mScriptFile, std::ifstream::ate | std::ifstream::binary);
	std::streampos pos = in.tellg();
	return (int)pos;
}

bool Script::hasScriptChanged() {
	bool changed = false;
	int new_script_size = scriptSize(mScriptFile);
	if (mScriptSize != new_script_size) {
		changed = true;
	}
	mScriptSize = new_script_size;
	return changed;
}

bool Script::initScript() {
	//try init the python scripting instance and run init there
	if (mScriptFile.empty()) {
		std::cerr << ">>> Error! empty script cannot be initialized." << std::endl;
		return false;
	}

	mScriptSize = scriptSize(mScriptFile);

	try
	{
		//pybind11_init_wrapper();
		
		mModule = py::module::import(mScriptFile.c_str());

		mModuleVscript = mModule.attr("Script");
		mVscript = mModuleVscript(this);
		mPyInitF = mVscript.attr("init");
		mPyUpdateF = mVscript.attr("update");
		
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << ">>> Error! Uncaught exception in " << mScriptFile << ":\n";
		std::cerr << e.what() << std::endl;
		PyErr_Print();
	}

	return true;
}

bool Script::cleanUpScript() {
	//TODO: ...
	return true;
}

bool Script::reinitScript() {
	if (mScriptFile.empty()) {
		return false;
	}
	std::ifstream in(mScriptFile, std::ifstream::ate | std::ifstream::binary);
	std::streampos pos = in.tellg();

	return true;
}
