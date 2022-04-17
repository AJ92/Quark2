#include "itexture.h"
#include <iostream>

Texture::Texture() :
	IComponent(Type::Texture)
{

}

Texture::Texture(std::string & textureFile) :
	IComponent(Type::Texture),
	mTextureFile(textureFile)
{

}

Texture::~Texture()
{

}

void Texture::update() {
	
}

void Texture::init() {
	
}
