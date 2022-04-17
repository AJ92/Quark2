#include "ishader.h"
#include <iostream>

IShader::IShader() :
	IComponent(Type::Shader)
{

}

IShader::IShader(std::string & vertexShaderFile,
		std::string & fragmentShaderFile) :
	IComponent(Type::Shader),
	mVShaderFile(vertexShaderFile),
	mFShaderFile(fragmentShaderFile)
{

}

