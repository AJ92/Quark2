#ifndef PYSCRIPTINSTANCE_H
#define PYSCRIPTINSTANCE_H

#include "scriptinstance.h"
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>

class ScriptInstance;

namespace py = pybind11;

class PyScriptInstance final : public ScriptInstance
{
	using ScriptInstance::ScriptInstance;

public:
	virtual void init() override;
	virtual void update() override;

private:

};

#endif // PYSCRIPTINSTANCE_H
