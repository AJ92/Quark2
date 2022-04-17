#ifndef SHADER_H
#define SHADER_H

#include "base/comp/icomponent.h"
#include <string>

class Shader: public IComponent
{
public:
	Shader();
	Shader(std::string & vertexShaderFile, std::string & fragmentShaderFile);
	~Shader();

	virtual void init();
	virtual void update();

private:

	std::string mVShaderFile;
	std::string mFShaderFile;
};

#endif // SHADER_H
