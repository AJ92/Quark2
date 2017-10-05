#include "scriptinstance.h"
#include <iostream>


ScriptInstance::ScriptInstance(Script* script)
	: _script(*script)
{
}

void ScriptInstance::test() {
	std::cout << "Script::test() called!" << std::endl;
}

void ScriptInstance::log(std::string str) {
	std::cout << str << std::endl;
}
