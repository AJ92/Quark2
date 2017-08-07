#ifndef SHADER_H
#define SHADER_H

#include "base/datastructures/components/component.h"
#include <string>

class Shader: public Component
{
public:
	Shader();
	Shader(std::string & vertexShaderFile,
		std::string & fragmentShaderFile);
	~Shader();

	virtual void init();
	virtual void update();

private:

	std::string _vshader_file;
	std::string _fshader_file;
};

#endif // SHADER_H
