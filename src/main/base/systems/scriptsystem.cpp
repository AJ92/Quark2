#include "base\systems\scriptsystem.h"
#include <iostream>


ScriptSystem::ScriptSystem()
{
	bool init_done = _init_python();

	if (init_done) {
		std::cout << "Python initialized" << std::endl;
	}
	else {
		std::cout << "Python initialization failed" << std::endl;
	}
}

ScriptSystem::~ScriptSystem()
{

}

void ScriptSystem::update() {

}

///////////////////////////////////////////////
//
//		PRIVATE

bool ScriptSystem::_init_python() {
	Py_SetProgramName("Quark2");  /* optional but recommended */
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");
	Py_Finalize();
	return true;
}

bool ScriptSystem::_deint_python() {
	Py_Finalize();
	return true;
}
