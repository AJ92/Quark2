#include "ishader.h"
#include <iostream>

IShader::IShader(
		std::string& name, 
		std::string & vertexShaderFile,
		std::string & fragmentShaderFile) :
	IComponent(Type::Shader)
	, mName(name)
	, mVShaderFile(vertexShaderFile)
	, mFShaderFile(fragmentShaderFile)
{

}

