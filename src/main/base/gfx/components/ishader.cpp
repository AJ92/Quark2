#include "ishader.h"
#include <iostream>

IShader::IShader(
		const std::string & name, 
		const std::string & vertexShaderFile,
		const std::string & fragmentShaderFile) :
	IComponent(Type::Shader)
	, mName(name)
	, mVShaderFile(vertexShaderFile)
	, mFShaderFile(fragmentShaderFile)
{

}

