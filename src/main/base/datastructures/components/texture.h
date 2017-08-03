#ifndef TEXTURE_H
#define TEXTURE_H

#include "base/datastructures/components/component.h"

class Texture: public Component
{
public:
	Texture();
	Texture(std::string & textureFile);

	virtual void init();
	virtual void update();
};

#endif // TEXTURE_H
