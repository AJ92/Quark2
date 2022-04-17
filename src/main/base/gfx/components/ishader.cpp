#include "ishader.h"
#include <iostream>

Shader::Shader() :
	IComponent(Type::Shader)
{

}

Shader::Shader(std::string & vertexShaderFile,
		std::string & fragmentShaderFile) :
	IComponent(Type::Shader),
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

