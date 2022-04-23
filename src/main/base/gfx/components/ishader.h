#ifndef ISHADER_H
#define ISHADER_H

#include "base/comp/icomponent.h"
#include <string>

class IShader: public IComponent
{
public:
	IShader(
		const std::string & name, 
		const std::string & vertexShaderFile, 
		const std::string & fragmentShaderFile
	);
	virtual ~IShader() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:
	const std::string mName;
	std::string mVShaderFile;
	std::string mFShaderFile;
};

#endif // ISHADER_H
