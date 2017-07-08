#include "script.h"
#include <iostream>


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

Script::~Script()
{

}

void Script::update() {

}

///////////////////////////////////////////////
//
//		PRIVATE

bool Script::_init() {
	Py_SetProgramName("Quark2");  /* optional but recommended */
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");
	Py_Finalize();
	return true;
}

bool Script::_deint() {
	Py_Finalize();
	return true;
}
