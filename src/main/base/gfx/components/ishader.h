#ifndef ISHADER_H
#define ISHADER_H

#include "base/comp/icomponent.h"
#include <string>

class IShader: public IComponent
{
public:
	IShader();
	IShader(std::string & vertexShaderFile, std::string & fragmentShaderFile);
	virtual ~IShader() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:

	std::string mVShaderFile;
	std::string mFShaderFile;
};

#endif // ISHADER_H
