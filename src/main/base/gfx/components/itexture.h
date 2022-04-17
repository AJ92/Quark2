#ifndef TEXTURE_H
#define TEXTURE_H

#include "base/comp/icomponent.h"
#include <string>

class Texture: public IComponent
{
public:
	Texture();
	Texture(std::string & textureFile);
	~Texture();

	virtual void init();
	virtual void update();

private:
	std::string mTextureFile;
};

#endif // TEXTURE_H
