#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "base\systems\system.h"
#include <Python.h>
#include <memory>
#include <map>

class ScriptSystem: public System
{
public:
	ScriptSystem();
	~ScriptSystem();

	//update all script objects...
	virtual void update();

private:
	bool _init_python();
	bool _deint_python();
};

#endif // PYTHONSCRIPTING_H