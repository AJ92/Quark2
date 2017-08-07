#ifndef TEXTURE_H
#define TEXTURE_H

#include "base/datastructures/components/component.h"
#include <string>

class Texture: public Component
{
public:
	Texture();
	Texture(std::string & textureFile);
	~Texture();

	virtual void init();
	virtual void update();

private:
	std::string _texture_file;
};

#endif // TEXTURE_H
