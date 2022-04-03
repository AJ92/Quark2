#ifndef SHADER_H
#define SHADER_H

#include "base/datastructures/components/component.h"
#include <string>

class Shader: public Component
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
