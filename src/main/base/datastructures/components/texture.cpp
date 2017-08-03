#include "texture.h"
#include <iostream>

Texture::Texture() :
	Component(Type::Texture)
{

}

Texture::Texture(std::string & textureFile) :
	Component(Type::Texture),
	_texture_file(textureFile)
{

}

Texture::~Texture()
{

}

void Texture::update() {
	
}

void Texture::init() {
	
}
