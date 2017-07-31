#ifndef SCRIPTINSTANCE_H
#define SCRIPTINSTANCE_H

#include "base/datastructures/components/script.h"
#include <memory>
#include <map>

class ScriptInstance
{

public:
	ScriptInstance(Script* script);
	virtual ~ScriptInstance() = default;

	//python functions...
	virtual void init() = 0;
	virtual void update() = 0;

	virtual void log(std::string str);

	//todo some functions here...
	virtual void test();

private:
	Script & _script;
};

#endif // SCRIPTINSTANCE_H
