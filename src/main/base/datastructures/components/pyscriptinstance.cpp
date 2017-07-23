#include "pyscriptinstance.h"
#include <iostream>


void PyScriptInstance::update() {
	PYBIND11_OVERLOAD_PURE_NAME(
		void,					// return type
		ScriptInstance,				// super class
		"update",				// python function name
		updates					// function name
	);
}

void PyScriptInstance::init() {
	PYBIND11_OVERLOAD_PURE_NAME(
		void,					// return type
		ScriptInstance,				// super class
		"init",					// python function name
		init					// function name
	);
}


