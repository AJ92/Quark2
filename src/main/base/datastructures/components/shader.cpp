#include "shader.h"
#include <iostream>

Shader::Shader() :
	Component(Type::Shader)
{

}

Shader::Shader(std::string & vertexShaderFile,
		std::string & fragmentShaderFile) :
	Component(Type::Shader),
	mVShaderFile(vertexShaderFile),
	mFShaderFile(fragmentShaderFile)
{

}

Shader::~Shader()
{

}

void Shader::update() {
	
}

void Shader::init() {
	
}

