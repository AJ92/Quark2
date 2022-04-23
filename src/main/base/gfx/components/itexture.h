#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "base/comp/icomponent.h"
#include <string>

class ITexture: public IComponent
{
public:
	ITexture();
	ITexture(const std::string & textureFile);
	virtual ~ITexture() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:
	const std::string mTextureFile;
};

#endif // ITEXTURE_H
