#ifndef SCRIPT_H
#define SCRIPT_H

#include "base/datastructures/components/component.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>
#include <chrono>


namespace py = pybind11;

static bool PY_MODULE_INITIALIZED = false;

class Script: public Component
{
public:
	Script();
	Script(const Script &script);
	~Script();

	void setScript(std::string script);

	virtual void init() override;
	virtual void update() override;

	void log(std::string str);

private:
	
	int scriptSize(std::string script);
	bool hasScriptChanged();

	bool initScript();
	bool cleanUpScript();

	bool reinitScript();

	std::string mScriptFile;
	int mScriptSize = 0;

	py::object mMain;
	py::object mGlobals;
	py::object mModule;
	py::object mModuleVscript;
	py::object mVscript;

	py::object mPyInitF;
	py::object mPyUpdateF;

	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> mCheckTime;
};

#endif // SCRIPT_H
