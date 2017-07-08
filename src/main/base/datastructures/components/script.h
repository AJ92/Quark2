#ifndef SCRIPT_H
#define SCRIPT_H

#include "base\datastructures\components\component.h"
#include <Python.h>
#include <memory>
#include <map>

class Script: public Component
{
public:
	Script();
	~Script();

	//update all script objects...
	virtual void update();

private:
	bool _init();
	bool _deint();
};

#endif // SCRIPT_H