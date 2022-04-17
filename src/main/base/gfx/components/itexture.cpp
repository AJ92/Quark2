#include "itexture.h"
#include <iostream>

ITexture::ITexture() :
	IComponent(Type::Texture)
{

}

ITexture::ITexture(std::string & textureFile) :
	IComponent(Type::Texture),
	mTextureFile(textureFile)
{

}
